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

#include "EpsilonLinkConfiguration.h"
#include "MavlinkMessagesTimer.h"
#include "QGCMAVLink.h"

class EpsilonLinkManager;

Q_DECLARE_LOGGING_CATEGORY(EpsilonLinkInterfaceLog)

/**
 * @brief The link interface defines the interface for all links used to communicate
 * with the ground station application.
 **/
class EpsilonLinkInterface : public QThread
{
    Q_OBJECT

    friend class EpsilonLinkManager;

  public:
    virtual ~EpsilonLinkInterface();

    // Property accessors
    bool isPX4Flow(void) const
    {
        return _isPX4Flow;
    }

    SharedEpsilonLinkConfigurationPtr linkConfiguration(void)
    {
        return _config;
    }

    Q_INVOKABLE virtual void disconnect(void) = 0;
    Q_INVOKABLE virtual void testSend(void) = 0;

    virtual bool isConnected(void) const = 0;

    void writeBytesThreadSafe(const char *bytes, int length);
    void addVehicleReference(void);
    void removeVehicleReference(void);

  signals:
    void bytesReceived(EpsilonLinkInterface *link, QByteArray data);
    void bytesSent(EpsilonLinkInterface *link, QByteArray data);
    void connected(void);
    void disconnected(void);
    void communicationError(const QString &title, const QString &error);
    void _invokeWriteBytes(QByteArray);

  protected:
    // Links are only created by LinkManager so constructor is not public
    EpsilonLinkInterface(SharedEpsilonLinkConfigurationPtr &config, bool isPX4Flow = false);

    void _connectionRemoved(void);

    SharedEpsilonLinkConfigurationPtr _config;

  private slots:
    virtual void _writeBytes(const QByteArray) = 0; // Not thread safe if called directly, only writeBytesThreadSafe is thread safe

  private:
    // connect is private since all links should be created through LinkManager::createConnectedLink calls
    virtual bool _connect(void) = 0;

    bool _isPX4Flow = false;
    int _vehicleReferenceCount = 0;
};

typedef std::shared_ptr<EpsilonLinkInterface> SharedEpsilonLinkInterfacePtr;
typedef std::weak_ptr<EpsilonLinkInterface> WeakEpsilonLinkInterfacePtr;
