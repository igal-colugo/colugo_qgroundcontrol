/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QByteArray>
#include <QFile>
#include <QLoggingCategory>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QTimer>

#include "NextVisionLinkInterface.h"
#include "QGC.h"
#include "QGCTemporaryFile.h"
#include "QGCToolbox.h"

class NextVisionLinkManager;
class QGCApplication;

Q_DECLARE_LOGGING_CATEGORY(NextVisionMAVLinkProtocolLog)

/**
 * @brief MAVLink micro air vehicle protocol reference implementation.
 *
 * MAVLink is a generic communication protocol for micro air vehicles.
 * for more information, please see the official website: https://mavlink.io
 **/
class NextVisionMAVLinkProtocol : public QGCTool
{
    Q_OBJECT

  public:
    NextVisionMAVLinkProtocol(QGCApplication *app, QGCToolbox *toolbox);
    ~NextVisionMAVLinkProtocol();

    /** @brief Get the human-friendly name of this protocol */
    QString getName();
    /** @brief Get the system id of this application */
    int getSystemId() const;
    /** @brief Get the component id of this application */
    int getComponentId();
    /** @brief Get protocol version check state */
    bool versionCheckEnabled() const
    {
        return m_enable_version_check;
    }
    /** @brief Get the protocol version */
    int getVersion()
    {
        return MAVLINK_VERSION;
    }
    /** @brief Get the currently configured protocol version */
    unsigned getCurrentVersion() const
    {
        return _current_version;
    }
    /**
     * Reset the counters for all metadata for this link.
     */
    virtual void resetMetadataForLink(NextVisionLinkInterface *link);

    /// Suspend/Restart logging during replay.
    void suspendLogForReplay(bool suspend);

    /// Set protocol version
    void setVersion(unsigned version);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

  public slots:
    /** @brief Receive bytes from a communication interface */
    void receiveBytes(NextVisionLinkInterface *link, QByteArray b);

    /** @brief Log bytes sent from a communication interface */
    void logSentBytes(NextVisionLinkInterface *link, QByteArray b);

    /** @brief Set the system id of this application */
    void setSystemId(int id);

    /** @brief Enable / disable version check */
    void enableVersionCheck(bool enabled);

    /** @brief Load protocol settings */
    void loadSettings();
    /** @brief Store protocol settings */
    void storeSettings();

    /// @brief Deletes any log files which are in the temp directory
    static void deleteTempLogFiles(void);

    /// Checks for lost log files
    void checkForLostLogFiles(void);

  protected:
    bool m_enable_version_check;                            ///< Enable checking of version match of MAV and QGC
    uint8_t lastIndex[256][256];                            ///< Store the last received sequence ID for each system/componenet pair
    uint8_t firstMessage[256][256];                         ///< First message flag
    uint64_t totalReceiveCounter[MAVLINK_COMM_NUM_BUFFERS]; ///< The total number of successfully received messages
    uint64_t totalLossCounter[MAVLINK_COMM_NUM_BUFFERS];    ///< Total messages lost during transmission.
    float runningLossPercent[MAVLINK_COMM_NUM_BUFFERS];     ///< Loss rate

    mavlink_message_t _message;
    mavlink_status_t _status;

    bool versionMismatchIgnore;
    int systemId;
    unsigned _current_version;

  signals:
    /// Heartbeat received on link
    void vehicleHeartbeatInfo(NextVisionLinkInterface *link, int vehicleId, int componentId, int vehicleFirmwareType, int vehicleType);

    /** @brief Message received and directly copied via signal */
    void messageReceived(NextVisionLinkInterface *link, mavlink_message_t message);
    /** @brief Emitted if version check is enabled / disabled */
    void versionCheckChanged(bool enabled);
    /** @brief Emitted if a message from the protocol should reach the user */
    void protocolStatusMessage(const QString &title, const QString &message);
    /** @brief Emitted if a new system ID was set */
    void systemIdChanged(int systemId);

    void mavlinkMessageStatus(int uasId, uint64_t totalSent, uint64_t totalReceived, uint64_t totalLoss, float lossPercent);

    /// Emitted when a temporary telemetry log file is ready for saving
    void saveTelemetryLog(QString tempLogfile);

    /// Emitted when a telemetry log is started to save.
    void checkTelemetrySavePath(void);

  private:
    bool _closeLogFile(void);
    void _startLogging(void);
    void _stopLogging(void);

    bool _logSuspendError;  ///< true: Logging suspended due to error
    bool _logSuspendReplay; ///< true: Logging suspended due to replay

    QGCTemporaryFile _tempLogFile;           ///< File to log to
    static const char *_tempLogFileTemplate; ///< Template for temporary log file
    static const char *_logFileExtension;    ///< Extension for log files

    NextVisionLinkManager *_linkMgr;
};
