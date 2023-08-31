/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QDateTime>
#include <QDebug>
#include <QLoggingCategory>
#include <QMetaType>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QThread>
#include <QTimer>

#include <memory>

#include "MavlinkMessagesTimer.h"
#include "NextVisionLinkConfiguration.h"
#include "QGCMAVLink.h"

class NextVisionLinkManager;

Q_DECLARE_LOGGING_CATEGORY(NextVisionLinkInterfaceLog)

/**
 * @brief The link interface defines the interface for all links used to communicate
 * with the ground station application.
 **/
class NextVisionLinkInterface : public QThread
{
    Q_OBJECT

    friend class NextVisionLinkManager;

  public:
    virtual ~NextVisionLinkInterface();

    // Property accessors
    bool isPX4Flow(void) const
    {
        return _isPX4Flow;
    }

    SharedNextVisionLinkConfigurationPtr linkConfiguration(void)
    {
        return _config;
    }

    Q_INVOKABLE virtual void disconnect(void) = 0;
    Q_INVOKABLE virtual void testSend(void) = 0;

    virtual bool isConnected(void) const = 0;

    uint8_t mavlinkChannel(void) const;
    bool mavlinkChannelIsSet(void) const;

    bool decodedFirstMavlinkPacket(void) const
    {
        return _decodedFirstMavlinkPacket;
    }
    bool setDecodedFirstMavlinkPacket(bool decodedFirstMavlinkPacket)
    {
        return _decodedFirstMavlinkPacket = decodedFirstMavlinkPacket;
    }
    void writeBytesThreadSafe(const char *bytes, int length);
    void addVehicleReference(void);
    void removeVehicleReference(void);

  signals:
    void bytesReceived(NextVisionLinkInterface *link, QByteArray data);
    void bytesSent(NextVisionLinkInterface *link, QByteArray data);
    void connected(void);
    void disconnected(void);
    void communicationError(const QString &title, const QString &error);
    void _invokeWriteBytes(QByteArray);

  protected:
    // Links are only created by LinkManager so constructor is not public
    NextVisionLinkInterface(SharedNextVisionLinkConfigurationPtr &config, bool isPX4Flow = false);

    void _connectionRemoved(void);

    SharedNextVisionLinkConfigurationPtr _config;

    ///
    /// \brief _allocateMavlinkChannel
    ///     Called by the LinkManager during LinkInterface construction
    /// instructing the link to setup channels.
    ///
    /// Default implementation allocates a single channel. But some link types
    /// (such as MockLink) need more than one.
    ///
    virtual bool _allocateMavlinkChannel();
    virtual void _freeMavlinkChannel();

  private slots:
    virtual void _writeBytes(const QByteArray) = 0; // Not thread safe if called directly, only writeBytesThreadSafe is thread safe

  private:
    // connect is private since all links should be created through LinkManager::createConnectedLink calls
    virtual bool _connect(void) = 0;

    uint8_t _mavlinkChannel = std::numeric_limits<uint8_t>::max();
    bool _decodedFirstMavlinkPacket = false;
    bool _isPX4Flow = false;
    int _vehicleReferenceCount = 0;

    QMap<int /* vehicle id */, MavlinkMessagesTimer *> _mavlinkMessagesTimers;
};

typedef std::shared_ptr<NextVisionLinkInterface> SharedNextVisionLinkInterfacePtr;
typedef std::weak_ptr<NextVisionLinkInterface> WeakNextVisionLinkInterfacePtr;
