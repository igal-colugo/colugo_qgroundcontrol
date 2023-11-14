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

#include "EpsilonLinkManager.h"
#include "EpsilonMAVLinkProtocol.h"
#include "QGC.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCMAVLink.h"
#include "SettingsManager.h"
#include "UAS.h"
#include "UASInterface.h"

Q_DECLARE_METATYPE(mavlink_message_t)

QGC_LOGGING_CATEGORY(EpsilonMAVLinkProtocolLog, "EpsilonMAVLinkProtocolLog")

const char *EpsilonMAVLinkProtocol::_tempLogFileTemplate = "FlightDataXXXXXX"; ///< Template for temporary log file
const char *EpsilonMAVLinkProtocol::_logFileExtension = "mavlink";             ///< Extension for log files

/**
 * The default constructor will create a new MAVLink object sending heartbeats at
 * the MAVLINK_HEARTBEAT_DEFAULT_RATE to all connected links.
 */
EpsilonMAVLinkProtocol::EpsilonMAVLinkProtocol(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), m_enable_version_check(true), _message({}), _status({}), versionMismatchIgnore(false), systemId(255), _current_version(100), _logSuspendError(false),
      _logSuspendReplay(false), _tempLogFile(QString("%2.%3").arg(_tempLogFileTemplate).arg(_logFileExtension)), _linkMgr(nullptr)
{
    memset(totalReceiveCounter, 0, sizeof(totalReceiveCounter));
    memset(totalLossCounter, 0, sizeof(totalLossCounter));
    memset(runningLossPercent, 0, sizeof(runningLossPercent));
    memset(firstMessage, 1, sizeof(firstMessage));
    memset(&_status, 0, sizeof(_status));
    memset(&_message, 0, sizeof(_message));
}

EpsilonMAVLinkProtocol::~EpsilonMAVLinkProtocol()
{
    storeSettings();
    _closeLogFile();
}

void EpsilonMAVLinkProtocol::setVersion(unsigned version)
{
    QList<SharedEpsilonLinkInterfacePtr> sharedLinks = _linkMgr->links();

    for (int i = 0; i < sharedLinks.length(); i++)
    {
        mavlink_status_t *mavlinkStatus = mavlink_get_channel_status(sharedLinks[i].get()->mavlinkChannel());

        // Set flags for version
        if (version < 200)
        {
            mavlinkStatus->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
        }
        else
        {
            mavlinkStatus->flags &= ~MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
        }
    }

    _current_version = version;
}

void EpsilonMAVLinkProtocol::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);

    _linkMgr = _toolbox->epsilonLinkManager();

    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");

    loadSettings();

    // All the *Counter variables are not initialized here, as they should be initialized
    // on a per-link basis before those links are used. @see resetMetadataForLink().

    connect(this, &EpsilonMAVLinkProtocol::protocolStatusMessage, _app, &QGCApplication::criticalMessageBoxOnMainThread);
    connect(this, &EpsilonMAVLinkProtocol::saveTelemetryLog, _app, &QGCApplication::saveTelemetryLogOnMainThread);
    connect(this, &EpsilonMAVLinkProtocol::checkTelemetrySavePath, _app, &QGCApplication::checkTelemetrySavePathOnMainThread);

    emit versionCheckChanged(m_enable_version_check);
}

void EpsilonMAVLinkProtocol::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("QGC_EPSILON_MAVLINK_PROTOCOL");
    enableVersionCheck(settings.value("VERSION_CHECK_ENABLED", m_enable_version_check).toBool());

    // Only set system id if it was valid
    int temp = settings.value("GCS_SYSTEM_ID", systemId).toInt();
    if (temp > 0 && temp < 256)
    {
        systemId = temp;
    }
}

void EpsilonMAVLinkProtocol::storeSettings()
{
    // Store settings
    QSettings settings;
    settings.beginGroup("QGC_EPSILON_MAVLINK_PROTOCOL");
    settings.setValue("VERSION_CHECK_ENABLED", m_enable_version_check);
    settings.setValue("GCS_SYSTEM_ID", systemId);
    // Parameter interface settings
}

void EpsilonMAVLinkProtocol::resetMetadataForLink(EpsilonLinkInterface *link)
{
    int channel = link->mavlinkChannel();
    totalReceiveCounter[channel] = 0;
    totalLossCounter[channel] = 0;
    runningLossPercent[channel] = 0.0f;
    for (int i = 0; i < 256; i++)
    {
        firstMessage[channel][i] = 1;
    }
    link->setDecodedFirstMavlinkPacket(false);
}

/**
 * This method parses all outcoming bytes and log a MAVLink packet.
 * @param link The interface to read from
 * @see LinkInterface
 **/
void EpsilonMAVLinkProtocol::logSentBytes(EpsilonLinkInterface *link, QByteArray b)
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
            // If there's an error logging data, raise an alert and stop logging.
            emit protocolStatusMessage(tr("MAVLink Protocol"), tr("MAVLink Logging failed. Could not write to file %1, logging disabled.").arg(_tempLogFile.fileName()));
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
void EpsilonMAVLinkProtocol::receiveBytes(EpsilonLinkInterface *link, QByteArray b)
{
    // Since receiveBytes signals cross threads we can end up with signals in the queue
    // that come through after the link is disconnected. For these we just drop the data
    // since the link is closed.
    SharedEpsilonLinkInterfacePtr linkPtr = _linkMgr->sharedLinkInterfacePointerForLink(link, true);
    if (!linkPtr)
    {
        qCDebug(EpsilonMAVLinkProtocolLog) << "receiveBytes: link gone!" << b.size() << " bytes arrived too late";
        return;
    }

    uint8_t mavlinkChannel = link->mavlinkChannel();

    for (int position = 0; position < b.size(); position++)
    {
        if (mavlink_parse_char(mavlinkChannel, static_cast<uint8_t>(b[position]), &_message, &_status))
        {
            // Got a valid message
            if (!link->decodedFirstMavlinkPacket())
            {
                link->setDecodedFirstMavlinkPacket(true);
                mavlink_status_t *mavlinkStatus = mavlink_get_channel_status(mavlinkChannel);
                if (!(mavlinkStatus->flags & MAVLINK_STATUS_FLAG_IN_MAVLINK1) && (mavlinkStatus->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1))
                {
                    qCDebug(MAVLinkProtocolLog) << "Switching outbound to mavlink 2.0 due to incoming mavlink 2.0 packet:" << mavlinkStatus << mavlinkChannel << mavlinkStatus->flags;
                    mavlinkStatus->flags &= ~MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
                    // Set all links to v2
                    setVersion(200);
                }
            }
            //-----------------------------------------------------------------
            // MAVLink Status
            uint8_t lastSeq = lastIndex[_message.sysid][_message.compid];
            uint8_t expectedSeq = lastSeq + 1;
            // Increase receive counter
            totalReceiveCounter[mavlinkChannel]++;
            // Determine what the next expected sequence number is, accounting for
            // never having seen a message for this system/component pair.
            if (firstMessage[_message.sysid][_message.compid])
            {
                firstMessage[_message.sysid][_message.compid] = 0;
                lastSeq = _message.seq;
                expectedSeq = _message.seq;
            }
            // And if we didn't encounter that sequence number, record the error
            // int foo = 0;
            if (_message.seq != expectedSeq)
            {
                // foo = 1;
                int lostMessages = 0;
                //-- Account for overflow during packet loss
                if (_message.seq < expectedSeq)
                {
                    lostMessages = (_message.seq + 255) - expectedSeq;
                }
                else
                {
                    lostMessages = _message.seq - expectedSeq;
                }
                // Log how many were lost
                totalLossCounter[mavlinkChannel] += static_cast<uint64_t>(lostMessages);
            }
            // And update the last sequence number for this system/component pair
            lastIndex[_message.sysid][_message.compid] = _message.seq;
            // Calculate new loss ratio
            uint64_t totalSent = totalReceiveCounter[mavlinkChannel] + totalLossCounter[mavlinkChannel];
            float receiveLossPercent = static_cast<float>(static_cast<double>(totalLossCounter[mavlinkChannel]) / static_cast<double>(totalSent));
            receiveLossPercent *= 100.0f;
            receiveLossPercent = (receiveLossPercent * 0.5f) + (runningLossPercent[mavlinkChannel] * 0.5f);
            runningLossPercent[mavlinkChannel] = receiveLossPercent;

            // qDebug() << foo << _message.seq << expectedSeq << lastSeq << totalLossCounter[mavlinkChannel] << totalReceiveCounter[mavlinkChannel] << totalSentCounter[mavlinkChannel] << "("
            // << _message.sysid << _message.compid << ")";

            //-----------------------------------------------------------------
            // Log data
            if (!_logSuspendError && !_logSuspendReplay && _tempLogFile.isOpen())
            {
                uint8_t buf[MAVLINK_MAX_PACKET_LEN + sizeof(quint64)];

                // Write the uint64 time in microseconds in big endian format before the message.
                // This timestamp is saved in UTC time. We are only saving in ms precision because
                // getting more than this isn't possible with Qt without a ton of extra code.
                quint64 time = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() * 1000);
                qToBigEndian(time, buf);

                // Then write the message to the buffer
                int len = mavlink_msg_to_send_buffer(buf + sizeof(quint64), &_message);

                // Determine how many bytes were written by adding the timestamp size to the message size
                len += sizeof(quint64);

                // Now write this timestamp/message pair to the log.
                QByteArray b(reinterpret_cast<const char *>(buf), len);
                if (_tempLogFile.write(b) != len)
                {
                    // If there's an error logging data, raise an alert and stop logging.
                    emit protocolStatusMessage(tr("MAVLink Protocol"), tr("MAVLink Logging failed. Could not write to file %1, logging disabled.").arg(_tempLogFile.fileName()));
                    _stopLogging();
                    _logSuspendError = true;
                }
            }

            if (_message.msgid == MAVLINK_MSG_ID_HEARTBEAT)
            {
                _startLogging();
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&_message, &heartbeat);
                emit vehicleHeartbeatInfo(link, _message.sysid, _message.compid, heartbeat.autopilot, heartbeat.type);
            }

            // Update MAVLink status on every 32th packet
            if ((totalReceiveCounter[mavlinkChannel] & 0x1F) == 0)
            {
                emit mavlinkMessageStatus(_message.sysid, totalSent, totalReceiveCounter[mavlinkChannel], totalLossCounter[mavlinkChannel], receiveLossPercent);
            }

            // The packet is emitted as a whole, as it is only 255 - 261 bytes short
            // kind of inefficient, but no issue for a groundstation pc.
            // It buys as reentrancy for the whole code over all threads
            emit messageReceived(link, _message);

            // Anyone handling the message could close the connection, which deletes the link,
            // so we check if it's expired
            if (1 == linkPtr.use_count())
            {
                break;
            }

            // Reset message parsing
            memset(&_status, 0, sizeof(_status));
            memset(&_message, 0, sizeof(_message));
        }
    }
}

/**
 * @return The name of this protocol
 **/
QString EpsilonMAVLinkProtocol::getName()
{
    return tr("Next Vision MAVLink protocol");
}

/** @return System id of this application */
int EpsilonMAVLinkProtocol::getSystemId() const
{
    return systemId;
}

void EpsilonMAVLinkProtocol::setSystemId(int id)
{
    systemId = id;
}

/** @return Component id of this application */
int EpsilonMAVLinkProtocol::getComponentId()
{
    return MAV_COMP_ID_MISSIONPLANNER;
}

void EpsilonMAVLinkProtocol::enableVersionCheck(bool enabled)
{
    m_enable_version_check = enabled;
    emit versionCheckChanged(enabled);
}

/// @brief Closes the log file if it is open
bool EpsilonMAVLinkProtocol::_closeLogFile(void)
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

void EpsilonMAVLinkProtocol::_startLogging(void)
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
#ifdef __mobile__
    //-- Mobile build don't write to /tmp unless told to do so
    if (!appSettings->telemetrySave()->rawValue().toBool())
    {
        return;
    }
#endif
    //-- Log is always written to a temp file. If later the user decides they want
    //   it, it's all there for them.
    if (!_tempLogFile.isOpen())
    {
        if (!_logSuspendReplay)
        {
            if (!_tempLogFile.open())
            {
                emit protocolStatusMessage(tr("MAVLink Protocol"), tr("Opening Flight Data file for writing failed. "
                                                                      "Unable to write to %1. Please choose a different file location.")
                                                                       .arg(_tempLogFile.fileName()));
                _closeLogFile();
                _logSuspendError = true;
                return;
            }

            qCDebug(MAVLinkProtocolLog) << "Temp log" << _tempLogFile.fileName();
            emit checkTelemetrySavePath();

            _logSuspendError = false;
        }
    }
}

void EpsilonMAVLinkProtocol::_stopLogging(void)
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
void EpsilonMAVLinkProtocol::checkForLostLogFiles(void)
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

void EpsilonMAVLinkProtocol::suspendLogForReplay(bool suspend)
{
    _logSuspendReplay = suspend;
}

void EpsilonMAVLinkProtocol::deleteTempLogFiles(void)
{
    QDir tempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    QString filter(QString("*.%1").arg(_logFileExtension));
    QFileInfoList fileInfoList = tempDir.entryInfoList(QStringList(filter), QDir::Files);

    for (const QFileInfo &fileInfo : fileInfoList)
    {
        QFile::remove(fileInfo.filePath());
    }
}
