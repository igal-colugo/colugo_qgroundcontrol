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
    int recieved_buffer_size = b.size();

    // Since receiveBytes signals cross threads we can end up with signals in the queue
    // that come through after the link is disconnected. For these we just drop the data
    // since the link is closed.
    SharedCommtactLinkInterfacePtr linkPtr = _linkMgr->sharedLinkInterfacePointerForLink(link, true);
    if (!linkPtr)
    {
        qCDebug(CommtactLinkProtocolLog) << "receiveBytes: link gone!" << b.size() << " bytes arrived too late";
        return;
    }

    for (int position = 0; position < recieved_buffer_size; position++)
    {
        if (_commtact_link_parse_char(static_cast<uint8_t>(b[position]), recieved_buffer_size, &_message, &_status))
        {
            // Got a valid message
            // Increase receive counter
            totalReceiveCounter++;

            // The packet is emitted as a whole
            // kind of inefficient, but no issue for a groundstation pc.
            // It buys as reentrancy for the whole code over all threads
            emit messageReceived(link, _message, recieved_buffer_size);

            // Reset message parsing
            memset(&_status, 0, sizeof(_status));
            memset(&_message, 0, sizeof(_message));
        }
    }
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

uint8_t CommtactLinkProtocol::_commtact_link_parse_char(uint8_t c, int buffer_size, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status)
{
    uint8_t msg_received = _commtact_link_frame_char(c, buffer_size, r_message, r_mavlink_status);

    //    if (msg_received == COMMTACT_LINK_FRAMING_BAD_CRC || msg_received == COMMTACT_LINK_PARSE_STATE_GOT_BAD_TERMINATOR || msg_received == COMMTACT_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC)
    //    {
    //        commtact_link_message_t *rxmsg = r_message;
    //        commtact_link_status_t *status = r_mavlink_status;

    //        _commtact_parse_error(status);

    //        status->msg_received = COMMTACT_LINK_FRAMING_INCOMPLETE;
    //        status->parse_state = COMMTACT_LINK_PARSE_STATE_IDLE;
    //        if (c == COMMTACT_LINK_STX_1)
    //        {
    //            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_STX_1;
    //            rxmsg->length = 0;
    //            _commtact_link_start_header_checksum(rxmsg);
    //            _commtact_link_start_checksum(rxmsg);
    //        }

    //        return 0;
    //    }

    return msg_received;
}

uint8_t CommtactLinkProtocol::_commtact_link_frame_char(uint8_t c, int buffer_size, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status)
{
    return _commtact_link_frame_char_buffer(r_message, buffer_size, r_mavlink_status, c);
}

uint8_t CommtactLinkProtocol::_commtact_link_frame_char_buffer(commtact_link_message_t *rxmsg, int buffer_size, commtact_link_status_t *status, uint8_t c)
{
    status->msg_received = COMMTACT_LINK_FRAMING_INCOMPLETE;

    switch (status->parse_state)
    {
    case COMMTACT_LINK_PARSE_STATE_UNINIT:
    case COMMTACT_LINK_PARSE_STATE_IDLE:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_1;
        rxmsg->time_stamp[0] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_1:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_2;
        rxmsg->time_stamp[1] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_2:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_3;
        rxmsg->time_stamp[2] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_3:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_4;
        rxmsg->time_stamp[3] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_4:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_1;
        rxmsg->seq_num[0] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_1:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_2;
        rxmsg->seq_num[1] = c;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_2:
        status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_OPCODE;
        rxmsg->opcode = c;
        status->packet_idx = 0;
        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_OPCODE:

        _COMMTACT_PAYLOAD_NON_CONST(rxmsg)[status->packet_idx++] = (char) c;

        if (status->packet_idx == buffer_size - 7 - 1)
        {
            status->parse_state = COMMTACT_LINK_PARSE_STATE_GOT_DATA;
        }

        break;

    case COMMTACT_LINK_PARSE_STATE_GOT_DATA:

        // zero-fill the packet to cope with short incoming packets
        //        if (status->packet_idx < rxmsg->length)
        //        {
        //            memset(&_COMMTACT_PAYLOAD_NON_CONST(rxmsg)[status->packet_idx], 0, buffer_size - status->packet_idx);
        //        }

        // Successfully got message
        _COMMTACT_PAYLOAD_NON_CONST(rxmsg)[status->packet_idx] = (char) c; // get last byte of recieved data
        status->packet_idx = 0;
        status->msg_received = COMMTACT_LINK_FRAMING_OK;
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
    // msg->checksum = crcTmp;
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
    //    uint8_t checksum = msg->checksum;

    //    _crc_accumulate(c, &checksum);

    //    msg->checksum = checksum;
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

uint16_t CommtactLinkProtocol::commtact_link_msg_get_report_message_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t gdt_required_message)
{
    CommtactLinkProtocol::commtact_gdt_get_report_t packet = {};

    msg->time_stamp[0] = 0;
    msg->time_stamp[1] = 0;
    msg->time_stamp[2] = 0;
    msg->time_stamp[3] = 0;

    msg->seq_num[0] = 0;
    msg->seq_num[1] = 0;

    msg->opcode = GDT_GET_REPORT_MESSAGE;

    packet.gdt_required_message = gdt_required_message;

    uint8_t payload[sizeof(commtact_gdt_get_report_t)] = {};

    memcpy(&payload, &packet, sizeof(commtact_gdt_get_report_t));

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return sizeof(commtact_gdt_get_report_t);
}

//-------------------------- GDT -----------------------------------
uint16_t CommtactLinkProtocol::commtact_link_msg_gdt_operational_frequency_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint16_t gdt_operational_frequency)
{
    CommtactLinkProtocol::commtact_gdt_constant_frequency_t packet = {};

    msg->time_stamp[0] = 0;
    msg->time_stamp[1] = 0;
    msg->time_stamp[2] = 0;
    msg->time_stamp[3] = 0;

    msg->seq_num[0] = 0;
    msg->seq_num[1] = 0;

    msg->opcode = GDT_CONSTANT_FREQUENCY_SET;

    packet.gdt_opeartion_frequency = gdt_operational_frequency;

    uint8_t payload[sizeof(commtact_gdt_constant_frequency_t)] = {};

    memcpy(&payload, &packet, sizeof(commtact_gdt_constant_frequency_t));

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return sizeof(commtact_gdt_constant_frequency_t);
}

uint16_t CommtactLinkProtocol::commtact_link_msg_operational_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t transmitter_operational_mode, uint8_t pedestal_track__mode,
                                                                       uint8_t gdt_antenna_select, uint16_t set_azimuth, int16_t set_elevation, uint8_t frequency_mode, uint8_t reserved_1,
                                                                       uint8_t tdd_operational_mode, uint8_t aes_encryption_enable, uint8_t reserved_2, uint8_t symbol_rate,
                                                                       uint8_t unit_mode)
{
    CommtactLinkProtocol::commtact_gdt_operational_mode_t packet = {};

    msg->time_stamp[0] = 0;
    msg->time_stamp[1] = 0;
    msg->time_stamp[2] = 0;
    msg->time_stamp[3] = 0;

    msg->seq_num[0] = 0;
    msg->seq_num[1] = 0;

    msg->opcode = GDT_OPERATIONAL_MODE_SET;

    packet.transmitter_operational_mode = transmitter_operational_mode;
    packet.gdt_pedestal_track_mode = pedestal_track__mode;
    packet.gdt_antenna_select = gdt_antenna_select;
    packet.set_azimuth = set_azimuth;
    packet.set_elevation = set_elevation;
    packet.frequency_mode = frequency_mode;
    packet.reserved_1 = reserved_1;
    packet.tdd_operational_mode = tdd_operational_mode;
    packet.aes_encryption = aes_encryption_enable;
    packet.reserved_2 = reserved_2;
    packet.symbol_rate = symbol_rate;
    packet.unit_mode = unit_mode;

    uint8_t payload[sizeof(commtact_gdt_operational_mode_t)] = {};

    memcpy(&payload, &packet, sizeof(commtact_gdt_operational_mode_t));

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return sizeof(commtact_gdt_operational_mode_t);
}

uint16_t CommtactLinkProtocol::commtact_link_msg_to_send_buffer(uint8_t *buf, const CommtactLinkProtocol::commtact_link_message_t *msg, uint32_t payload_size)
{
    buf[0] = msg->time_stamp[0];
    buf[1] = msg->time_stamp[1];
    buf[2] = msg->time_stamp[2];
    buf[3] = msg->time_stamp[3];
    buf[4] = msg->seq_num[0];
    buf[5] = msg->seq_num[1];
    buf[6] = msg->opcode;
    memcpy(&buf[7], _COMMTACT_PAYLOAD(msg), payload_size);

    return 7 + payload_size;
}

void CommtactLinkProtocol::commtact_link_msg_operational_modes_report_decode(const commtact_link_message_t *msg, commtact_gdt_operational_modes_report_t *operational_modes_report)
{
    memset(operational_modes_report, 0, sizeof(commtact_gdt_operational_modes_report_t));
    memcpy(operational_modes_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_gdt_operational_modes_report_t));
}

void CommtactLinkProtocol::commtact_link_msg_gdt_status_report_decode(const commtact_link_message_t *msg, commtact_gdt_status_report_t *gdt_status_report)
{
    memcpy(gdt_status_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_gdt_status_report_t));
}

void CommtactLinkProtocol::commtact_link_msg_gdt_cbit_report_decode(const commtact_link_message_t *msg, commtact_gdt_cbit_report_t *gdt_cbit_report)
{
    memcpy(gdt_cbit_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_gdt_cbit_report_t));
}

void CommtactLinkProtocol::commtact_link_msg_gdt_constant_frequency_report_decode(const commtact_link_message_t *msg, commtact_gdt_constant_frequency_report_t *gdt_constant_frequency_report)
{
    memcpy(gdt_constant_frequency_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_gdt_constant_frequency_report_t));
}

void CommtactLinkProtocol::commtact_link_msg_gdt_mission_adt_status_report_decode(const commtact_link_message_t *msg, commtact_gdt_mission_adt_status_report_t *gdt_mission_adt_status_report)
{
    memcpy(gdt_mission_adt_status_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_gdt_mission_adt_status_report_t));
}

void CommtactLinkProtocol::commtact_link_msg_adt_operational_modes_report_decode(const commtact_link_message_t *msg, commtact_adt_operational_modes_report_t *adt_operational_modes_report)
{
    memset(adt_operational_modes_report, 0, sizeof(commtact_adt_operational_modes_report_t));
    memcpy(adt_operational_modes_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_adt_operational_modes_report_t));
}

//-------------------------- ADT -----------------------------------
uint16_t CommtactLinkProtocol::commtact_link_msg_adt_operational_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t transmitter_operational_mode, uint8_t antenna_select,
                                                                           uint8_t reserved_1, uint8_t reserved_2, uint8_t unit_mode, uint8_t adt_frequency_mode,
                                                                           uint8_t video_transmit_enable, uint8_t reserved_3, uint8_t reserved_4, uint8_t tdd_operational_mode,
                                                                           uint8_t aes_encryption_enable, uint8_t telemetry_metadata_separation, uint8_t byte_12, uint8_t byte_13)
{
    CommtactLinkProtocol::commtact_adt_operational_mode_t packet = {};

    msg->time_stamp[0] = 0;
    msg->time_stamp[1] = 0;
    msg->time_stamp[2] = 0;
    msg->time_stamp[3] = 0;

    msg->seq_num[0] = 0;
    msg->seq_num[1] = 0;

    msg->opcode = ADT_OPERATIONAL_MODE_SET;

    packet.transmitter_operational_mode = transmitter_operational_mode;
    packet.antenna_select = antenna_select;
    packet.reserved_1 = reserved_1;
    packet.reserved_2 = reserved_2;
    packet.unit_mode = unit_mode;
    packet.adt_frequency_mode = adt_frequency_mode;
    packet.video_transmit_enable = video_transmit_enable;
    packet.reserved_3 = reserved_3;
    packet.reserved_4 = reserved_4;
    packet.tdd_operational_mode = tdd_operational_mode;
    packet.aes_encryption_enable = aes_encryption_enable;
    packet.telemetry_metadata_separation = telemetry_metadata_separation;
    packet.byte_12.byte = byte_12;
    packet.byte_13.byte = byte_13;

    uint8_t payload[sizeof(commtact_adt_operational_mode_t)] = {};

    memcpy(&payload, &packet, sizeof(commtact_adt_operational_mode_t));

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return sizeof(commtact_adt_operational_mode_t);
}
void CommtactLinkProtocol::commtact_link_msg_adt_constant_frequency_report_decode(const commtact_link_message_t *msg, commtact_adt_constant_frequency_report_t *adt_constant_frequency_report)
{
    memcpy(adt_constant_frequency_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_adt_constant_frequency_report_t));
}
uint16_t CommtactLinkProtocol::commtact_link_msg_adt_operational_frequency_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint16_t adt_operational_frequency)
{
    CommtactLinkProtocol::commtact_adt_constant_frequency_t packet = {};

    msg->time_stamp[0] = 0;
    msg->time_stamp[1] = 0;
    msg->time_stamp[2] = 0;
    msg->time_stamp[3] = 0;

    msg->seq_num[0] = 0;
    msg->seq_num[1] = 0;

    msg->opcode = ADT_CONSTANT_FREQUENCY_SET;

    packet.adt_main_channel_frequency = adt_operational_frequency;
    packet.reserved = 0;

    uint8_t payload[sizeof(commtact_adt_constant_frequency_t)] = {};

    memcpy(&payload, &packet, sizeof(commtact_adt_constant_frequency_t));

    memcpy(_COMMTACT_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return sizeof(commtact_adt_constant_frequency_t);
}
void CommtactLinkProtocol::commtact_link_msg_adt_status_report_decode(const commtact_link_message_t *msg, commtact_adt_status_report_t *adt_status_report)
{
    memcpy(adt_status_report, _COMMTACT_PAYLOAD(msg), sizeof(commtact_adt_status_report_t));
}
//------------------------------------------------------------------

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
