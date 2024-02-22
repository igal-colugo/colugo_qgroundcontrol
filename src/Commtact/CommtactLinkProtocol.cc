/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <inttypes.h>
#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMetaType>
#include <QSettings>
#include <QStandardPaths>
#include <QTime>
#include <QtEndian>

#include "CommtactLinkManager.h"
#include "CommtactLinkProtocol.h"

Q_DECLARE_METATYPE(mavlink_message_t)

QGC_LOGGING_CATEGORY(CommtactLinkProtocolLog, "CommtactLinkProtocolLog")

const char *CommtactLinkProtocol::_tempLogFileTemplate = "FlightDataXXXXXX"; ///< Template for temporary log file
const char *CommtactLinkProtocol::_logFileExtension = "commtact_link";       ///< Extension for log files

/**
 * The default constructor will create a new MAVLink object sending heartbeats at
 * the MAVLINK_HEARTBEAT_DEFAULT_RATE to all connected links.
 */
CommtactLinkProtocol::CommtactLinkProtocol(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), _message({}), _status({}), _logSuspendError(false), _logSuspendReplay(false), _tempLogFile(QString("%2.%3").arg(_tempLogFileTemplate).arg(_logFileExtension)),
      _linkMgr(nullptr)
{
    totalReceiveCounter = 0;
    totalLossCounter = 0;
    runningLossPercent = 0.0;

    memset(&_status, 0, sizeof(_status));
    memset(&_message, 0, sizeof(_message));
}

CommtactLinkProtocol::~CommtactLinkProtocol()
{
    storeSettings();
    _closeLogFile();
}

void CommtactLinkProtocol::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);

    _linkMgr = _toolbox->commtactLinkManager();

    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");

    loadSettings();

    // All the *Counter variables are not initialized here, as they should be initialized
    // on a per-link basis before those links are used. @see resetMetadataForLink().

    connect(this, &CommtactLinkProtocol::saveTelemetryLog, _app, &QGCApplication::saveTelemetryLogOnMainThread);
    connect(this, &CommtactLinkProtocol::checkTelemetrySavePath, _app, &QGCApplication::checkTelemetrySavePathOnMainThread);
}

void CommtactLinkProtocol::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("QGC_COMMTACT_LINK_PROTOCOL");
}

void CommtactLinkProtocol::storeSettings()
{
    // Store settings
    QSettings settings;
    settings.beginGroup("QGC_COMMTACT_LINK_PROTOCOL");
    // settings.setValue("GCS_SYSTEM_ID", systemId);
    // Parameter interface settings
}

void CommtactLinkProtocol::resetMetadataForLink(CommtactLinkInterface *link)
{
    totalReceiveCounter = 0;
    totalLossCounter = 0;
    runningLossPercent = 0.0f;
}

/**
 * This method parses all outcoming bytes and log a MAVLink packet.
 * @param link The interface to read from
 * @see LinkInterface
 **/
void CommtactLinkProtocol::logSentBytes(CommtactLinkInterface *link, QByteArray b)
{

    uint8_t bytes_time[sizeof(quint64)];

    Q_UNUSED(link);
    if (!_logSuspendError && !_logSuspendReplay && _tempLogFile.isOpen())
    {

        quint64 time = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() * 1000);

        qToBigEndian(time, bytes_time);

        b.insert(0, QByteArray((const char *) bytes_time, sizeof(bytes_time)));

        int len = b.count();

        if (_tempLogFile.write(b) != len)
        {
            _stopLogging();
            _logSuspendError = true;
        }
    }
}

/**
 * This method parses all incoming bytes and constructs a MAVLink packet.
 * It can handle multiple links in parallel, as each link has it's own buffer/
 * parsing state machine.
 * @param link The interface to read from
 * @see LinkInterface
 **/
void CommtactLinkProtocol::receiveBytes(CommtactLinkInterface *link, QByteArray b)
{
    // Since receiveBytes signals cross threads we can end up with signals in the queue
    // that come through after the link is disconnected. For these we just drop the data
    // since the link is closed.
    SharedCommtactLinkInterfacePtr linkPtr = _linkMgr->sharedLinkInterfacePointerForLink(link, true);
    if (!linkPtr)
    {
        qCDebug(CommtactLinkProtocolLog) << "receiveBytes: link gone!" << b.size() << " bytes arrived too late";
        return;
    }

    for (int position = 0; position < b.size(); position++)
    {
        if (_commtact_link_parse_char(static_cast<uint8_t>(b[position]), &_message, &_status))
        {
            // Got a valid message
            // Increase receive counter
            totalReceiveCounter++;

            if (_message.message_id == COMMTACT_GLOBAL_STATUS_MSG_ID)
            {
                //_startLogging();
                commtact_global_status_t global_status;
                _commtact_link_msg_global_status_decode(&_message, &global_status);
            }

            // The packet is emitted as a whole
            // kind of inefficient, but no issue for a groundstation pc.
            // It buys as reentrancy for the whole code over all threads
            //            emit messageReceived(link, _message);

            // Reset message parsing
            memset(&_status, 0, sizeof(_status));
            memset(&_message, 0, sizeof(_message));
        }
    }
}

void CommtactLinkProtocol::_commtact_link_msg_global_status_decode(const commtact_link_message_t *msg, commtact_global_status_t *global_status)
{
    uint8_t len = msg->length < COMMTACT_LINK_MSG_ID_GLOBAL_STATUS_LEN ? msg->length : COMMTACT_LINK_MSG_ID_GLOBAL_STATUS_LEN;
    memset(global_status, 0, COMMTACT_LINK_MSG_ID_GLOBAL_STATUS_LEN);
    memcpy(global_status, _COMMTACT_PAYLOAD(msg), len);
}

/**
 * @return The name of this protocol
 **/
QString CommtactLinkProtocol::getName()
{
    return tr("Commtact Link protocol");
}

/// @brief Closes the log file if it is open
bool CommtactLinkProtocol::_closeLogFile(void)
{
    if (_tempLogFile.isOpen())
    {
        if (_tempLogFile.size() == 0)
        {
            // Don't save zero byte files
            _tempLogFile.remove();
            return false;
        }
        else
        {
            _tempLogFile.flush();
            _tempLogFile.close();
            return true;
        }
    }
    return false;
}

void CommtactLinkProtocol::_startLogging(void)
{
    //-- Are we supposed to write logs?
    if (qgcApp()->runningUnitTests())
    {
        return;
    }
    AppSettings *appSettings = _app->toolbox()->settingsManager()->appSettings();
    if (appSettings->disableAllPersistence()->rawValue().toBool())
    {
        return;
    }
    //-- Log is always written to a temp file. If later the user decides they want
    //   it, it's all there for them.
    if (!_tempLogFile.isOpen())
    {
        if (!_logSuspendReplay)
        {
            if (!_tempLogFile.open())
            {
                _closeLogFile();
                _logSuspendError = true;
                return;
            }

            emit checkTelemetrySavePath();

            _logSuspendError = false;
        }
    }
}

void CommtactLinkProtocol::_stopLogging(void)
{
    if (_tempLogFile.isOpen())
    {
        if (_closeLogFile())
        {
            if ((_app->toolbox()->settingsManager()->appSettings()->telemetrySaveNotArmed()->rawValue().toBool()) &&
                _app->toolbox()->settingsManager()->appSettings()->telemetrySave()->rawValue().toBool() &&
                !_app->toolbox()->settingsManager()->appSettings()->disableAllPersistence()->rawValue().toBool())
            {
                emit saveTelemetryLog(_tempLogFile.fileName());
            }
            else
            {
                QFile::remove(_tempLogFile.fileName());
            }
        }
    }
}

/// @brief Checks the temp directory for log files which may have been left there.
///         This could happen if QGC crashes without the temp log file being saved.
///         Give the user an option to save these orphaned files.
void CommtactLinkProtocol::checkForLostLogFiles(void)
{
    QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    QString filter(QString("*.%1").arg(_logFileExtension));
    QFileInfoList fileInfoList = tempDir.entryInfoList(QStringList(filter), QDir::Files);
    // qDebug() << "Orphaned log file count" << fileInfoList.count();

    for (const QFileInfo &fileInfo : fileInfoList)
    {
        // qDebug() << "Orphaned log file" << fileInfo.filePath();
        if (fileInfo.size() == 0)
        {
            // Delete all zero length files
            QFile::remove(fileInfo.filePath());
            continue;
        }
        emit saveTelemetryLog(fileInfo.filePath());
    }
}

void CommtactLinkProtocol::suspendLogForReplay(bool suspend)
{
    _logSuspendReplay = suspend;
}

void CommtactLinkProtocol::deleteTempLogFiles(void)
{
    QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    QString filter(QString("*.%1").arg(_logFileExtension));
    QFileInfoList fileInfoList = tempDir.entryInfoList(QStringList(filter), QDir::Files);

    for (const QFileInfo &fileInfo : fileInfoList)
    {
        QFile::remove(fileInfo.filePath());
    }
}

uint8_t CommtactLinkProtocol::_commtact_link_parse_char(uint8_t c, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status)
{
    uint8_t msg_received = _commtact_link_frame_char(c, r_message, r_mavlink_status);

    if (msg_received == COMMTACT_LINK_FRAMING_BAD_CRC || msg_received == COMMTACT_LINK_PARSE_STATE_GOT_BAD_TERMINATOR || msg_received == COMMTACT_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC)
    {
        commtact_link_message_t *rxmsg = r_message;
        commtact_link_status_t *status = r_mavlink_status;

        _commtact_parse_error(status);

        status->msg_received = COMMTACT_LINK_FRAMING_INCOMPLETE;
        status->parse_state = COMMTACT_LINK_PARSE_STATE_IDLE;
        if (c == COMMTACT_LINK_STX_1)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_STX_1;
            rxmsg->length = 0;
            _commtact_link_start_header_checksum(rxmsg);
            _commtact_link_start_checksum(rxmsg);
        }

        return 0;
    }

    return msg_received;
}

uint8_t CommtactLinkProtocol::_commtact_link_frame_char(uint8_t c, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status)
{
    return _commtact_link_frame_char_buffer(r_message, r_mavlink_status, c);
}

uint8_t CommtactLinkProtocol::_commtact_link_frame_char_buffer(commtact_link_message_t *rxmsg, commtact_link_status_t *status, uint8_t c)
{
    // int bufferIndex = 0;

    status->msg_received = COMMTACT_LINK_FRAMING_INCOMPLETE;

    switch (status->parse_state)
    {
    case COMMTACT_LINK_PARSE_STATE_UNINIT:
    case COMMTACT_LINK_PARSE_STATE_IDLE:
        if (c == COMMTACT_LINK_STX_1)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_STX_1;
            rxmsg->length = 0;
            rxmsg->magic[0] = c;
        }
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_STX_1:
        if (c == COMMTACT_LINK_STX_2)
        {
            _commtact_link_start_header_checksum(rxmsg);

            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_STX_2;
            rxmsg->magic[1] = c;
        }
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_STX_2:

        _commtact_link_update_header_checksum(rxmsg, c);

        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_DEVICE_ID;
        rxmsg->device_id = c;

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_DEVICE_ID:

        _commtact_link_update_header_checksum(rxmsg, c);

        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_MESSAGE_ID;
        rxmsg->message_id = c;

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_MESSAGE_ID:

        _commtact_link_update_header_checksum(rxmsg, c);

        rxmsg->length = c;
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_LENGTH;

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_LENGTH:

        if (rxmsg->header_checksum == c)
        {
            _commtact_link_start_checksum(rxmsg);

            status->packet_idx = 0;
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_HEADER_CRC;
        }
        else
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC;
        }

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_HEADER_CRC:

        _commtact_link_update_checksum(rxmsg, c);

        _COMMTACT_PAYLOAD_NON_CONST(rxmsg)[status->packet_idx++] = (char) c;

        if (status->packet_idx == rxmsg->length)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_PAYLOAD;
        }

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_PAYLOAD:

        if (c != rxmsg->checksum)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_BAD_CRC;
        }
        else
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_TERMINATOR;
        }

        rxmsg->checksum = c;

        // zero-fill the packet to cope with short incoming packets
        if (status->packet_idx < rxmsg->length)
        {
            memset(&_COMMTACT_PAYLOAD_NON_CONST(rxmsg)[status->packet_idx], 0, rxmsg->length - status->packet_idx);
        }

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_TERMINATOR: {

        rxmsg->terminator = c;

        if (rxmsg->terminator != COMMTACT_LINK_TERMINATOR)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_BAD_TERMINATOR;
        }
        else
        {
            // Successfully got message
            status->msg_received = COMMTACT_LINK_FRAMING_OK;
        }

        break;
    }

    case COMMTACT_LINK_PARSE_STATE_GOT_CRC:
    case COMMTACT_LINK_PARSE_STATE_GOT_BAD_CRC:
        if (status->parse_state == COMMTACT_LINK_PARSE_STATE_GOT_BAD_CRC || c != (rxmsg->checksum))
        {
            // got a bad CRC message
            status->msg_received = COMMTACT_LINK_FRAMING_BAD_CRC;
        }
        else
        {
            // Successfully got message
            status->msg_received = COMMTACT_LINK_FRAMING_OK;
        }

        status->parse_state = COMMTACT_LINK_PARSE_STATE_IDLE;

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC:

        // got a bad header CRC message
        status->msg_received = COMMTACT_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC;
        status->parse_state = COMMTACT_LINK_PARSE_STATE_IDLE;

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_BAD_TERMINATOR:

        status->msg_received = COMMTACT_LINK_PARSE_STATE_GOT_BAD_TERMINATOR;
        status->parse_state = COMMTACT_LINK_PARSE_STATE_IDLE;

        break;
    }

    // If a message has been successfully decoded, check index
    if (status->msg_received == COMMTACT_LINK_FRAMING_OK)
    {
        // Initial condition: If no packet has been received so far, drop count is undefined
        if (status->packet_rx_success_count == 0)
        {
            status->packet_rx_drop_count = 0;
        }
        // Count this packet as received
        status->packet_rx_success_count++;
    }

    status->parse_error = 0;

    return status->msg_received;
}

void CommtactLinkProtocol::_commtact_link_start_checksum(commtact_link_message_t *msg)
{
    uint8_t crcTmp = 0;
    _crc_init(&crcTmp);
    msg->checksum = crcTmp;
}

void CommtactLinkProtocol::_commtact_link_start_header_checksum(commtact_link_message_t *msg)
{
    uint8_t crcTmp = 0;
    _crc_init(&crcTmp);
    msg->header_checksum = crcTmp;
}

void CommtactLinkProtocol::_crc_init(uint8_t *crcAccum)
{
    *crcAccum = 1;
}

void CommtactLinkProtocol::_commtact_parse_error(commtact_link_status_t *status)
{
    status->parse_error++;
}

void CommtactLinkProtocol::_commtact_link_update_checksum(commtact_link_message_t *msg, uint8_t c)
{
    uint8_t checksum = msg->checksum;

    _crc_accumulate(c, &checksum);

    msg->checksum = checksum;
}

void CommtactLinkProtocol::_commtact_link_update_header_checksum(commtact_link_message_t *msg, uint8_t c)
{
    uint8_t checksum = msg->header_checksum;

    _crc_accumulate(c, &checksum);

    msg->header_checksum = checksum;
}

void CommtactLinkProtocol::_crc_accumulate(uint8_t data, uint8_t *crcAccum)
{
    /*Accumulate one byte of data into the CRC*/
    uint8_t tmp;

    tmp = crc8_Table[data ^ *crcAccum];

    *crcAccum = tmp;
}

uint8_t CommtactLinkProtocol::_check_sum_calculation(uint8_t *data, int8_t length)
{
    uint8_t return_value = 1;

    for (int i = 0; i < length; i++)
    {
        return_value = crc8_Table[return_value ^ (*(data + i))];
    }

    return return_value;
}

void CommtactLinkProtocol::_swap_bytes(uint16_t *data)
{
    uint16_t temp = 0;

    uint8_t *data_byte = (uint8_t *) data;
    uint8_t *temp_byte = (uint8_t *) &temp;

    // swap
    *(temp_byte + 0) = *(data_byte + 1);
    *(temp_byte + 1) = *(data_byte + 0);

    *(data) = temp;
}

void CommtactLinkProtocol::_swap_bytes(uint32_t *data)
{
    uint32_t temp = 0;

    uint8_t *data_byte = (uint8_t *) data;
    uint8_t *temp_byte = (uint8_t *) &temp;

    // swap
    *(temp_byte + 0) = *(data_byte + 3);
    *(temp_byte + 1) = *(data_byte + 2);
    *(temp_byte + 2) = *(data_byte + 1);
    *(temp_byte + 3) = *(data_byte + 0);

    *(data) = temp;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_rate_control_pack(CommtactLinkProtocol::commtact_link_message_t *msg, int8_t pan_speed, int8_t tilt_speed, int8_t nudge_column,
                                                                   int8_t nudge_raw, int8_t optical_zoom_speed, int8_t focus_adjustment, int16_t geo_dted)
{
    CommtactLinkProtocol::commtact_rate_control_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x05;
    msg->length = 8;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.pan_speed = pan_speed;
    packet.tilt_speed = tilt_speed;
    packet.nudge_column = nudge_column;
    packet.nudge_raw = nudge_raw;
    packet.optical_zoom_speed = optical_zoom_speed;
    packet.focus_adjustment = focus_adjustment;
    packet.geo_dted = geo_dted;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_control_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t mode, uint16_t pixel_pos_x, uint16_t pixel_pos_y,
                                                                   uint8_t box_sizes, uint8_t tracking_advanced, uint8_t options)
{
    CommtactLinkProtocol::commtact_control_mode_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x14;
    msg->length = 8;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.mode = mode;
    packet.pixel_pos_x = pixel_pos_x;
    packet.pixel_pos_y = pixel_pos_y;
    packet.box_sizes = box_sizes;
    packet.tracking_advanced = tracking_advanced;
    packet.options = options;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_digital_zoom_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t digital_zoom)
{
    CommtactLinkProtocol::commtact_digital_zoom_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x0B;
    msg->length = 1;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.digital_zoom = digital_zoom;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_on_screen_information_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint16_t switch_flags)
{
    CommtactLinkProtocol::commtact_on_screen_information_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x0C;
    msg->length = 1;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.switch_flags = switch_flags;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_camera_order_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t order)
{
    CommtactLinkProtocol::commtact_camera_order_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x1A;
    msg->length = 1;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.order = order;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_focus_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t switch_mode)
{
    CommtactLinkProtocol::commtact_focus_mode_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x10;
    msg->length = 1;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.switch_mode = switch_mode;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_do_snapshot_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t frame_step, uint8_t number_of_snapshots, uint8_t source,
                                                                  uint8_t format, uint8_t metadata, uint8_t file_name[32])
{
    CommtactLinkProtocol::commtact_do_snapshot_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x08;
    msg->length = 37;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.frame_step = frame_step;
    packet.number_of_snapshots = number_of_snapshots;
    packet.source = source;
    packet.format = format;
    packet.metadata = metadata;

    memcpy(&packet.file_name, &file_name, 32);

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_video_recording_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t recording_state, uint8_t file_name[32])
{
    CommtactLinkProtocol::commtact_video_recording_message_t packet = {};

    msg->magic[0] = COMMTACT_LINK_STX_1;
    msg->magic[1] = COMMTACT_LINK_STX_2;
    msg->device_id = 0;
    msg->message_id = 0x09;
    msg->length = 33;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = _check_sum_calculation(&data[0], 3);

    packet.recording_state = recording_state;

    memcpy(&packet.file_name, &file_name, 32);

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = _check_sum_calculation(&payload[0], msg->length);

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint16_t CommtactLinkProtocol::commtact_link_msg_to_send_buffer(uint8_t *buf, const CommtactLinkProtocol::commtact_link_message_t *msg)
{
    buf[0] = msg->magic[0];
    buf[1] = msg->magic[1];
    buf[2] = msg->device_id;
    buf[3] = msg->message_id;
    buf[4] = msg->length;
    buf[5] = msg->header_checksum;
    memcpy(&buf[6], _COMMTACT_PAYLOAD(msg), msg->length);
    buf[6 + msg->length] = msg->checksum;
    buf[6 + msg->length + 1] = msg->terminator;

    return 6 + msg->length + 2;
}
