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
#include <QList>
#include <QMap>
#include <QMutex>
#include <QQueue>
#include <QString>
#include <QUdpSocket>

#if defined(QGC_ZEROCONF_ENABLED)
#include <dns_sd.h>
#endif

#include "NextVisionLinkConfiguration.h"
#include "NextVisionLinkInterface.h"
#include "QGCConfig.h"

class NextVisionLinkManager;

class NextVisionUDPCLient
{
  public:
    NextVisionUDPCLient(const QHostAddress &address_, quint16 port_) : address(address_), port(port_)
    {
    }
    NextVisionUDPCLient(const NextVisionUDPCLient *other) : address(other->address), port(other->port)
    {
    }
    QHostAddress address;
    quint16 port;
};

class NextVisionUDPConfiguration : public NextVisionLinkConfiguration
{
    Q_OBJECT
  public:
    Q_PROPERTY(quint16 localPort READ localPort WRITE setLocalPort NOTIFY localPortChanged)
    Q_PROPERTY(QStringList hostList READ hostList NOTIFY hostListChanged)

    NextVisionUDPConfiguration(const QString &name);
    NextVisionUDPConfiguration(NextVisionUDPConfiguration *source);
    ~NextVisionUDPConfiguration();

    quint16 localPort() const
    {
        return _localPort;
    }

    /// @param[in] host Host name in standard formatt, e.g. localhost:14551 or 192.168.1.1:14551
    Q_INVOKABLE void addHost(const QString host);

    /// @param[in] host Host name, e.g. localhost or 192.168.1.1
    /// @param[in] port Port number
    void addHost(const QString &host, quint16 port);

    /// @param[in] host Host name, e.g. localhost or 192.168.1.1
    Q_INVOKABLE void removeHost(const QString host);

    void setLocalPort(quint16 port);
    QStringList hostList(void)
    {
        return _hostList;
    }
    const QList<NextVisionUDPCLient *> targetHosts(void)
    {
        return _targetHosts;
    }

    /// LinkConfiguration overrides
    LinkType type(void) override
    {
        return NextVisionLinkConfiguration::TypeUdp;
    }
    void copyFrom(NextVisionLinkConfiguration *source) override;
    void loadSettings(QSettings &settings, const QString &root) override;
    void saveSettings(QSettings &settings, const QString &root) override;
    QString settingsURL(void) override
    {
        return "UdpSettings.qml";
    }
    QString settingsTitle(void) override
    {
        return tr("UDP Link Settings");
    }

  signals:
    void localPortChanged(void);
    void hostListChanged(void);

  private:
    void _updateHostList(void);
    void _clearTargetHosts(void);
    void _copyFrom(NextVisionLinkConfiguration *source);

    QList<NextVisionUDPCLient *> _targetHosts;
    QStringList _hostList;
    quint16 _localPort;
};

class NextVisionUDPLink : public NextVisionLinkInterface
{
    Q_OBJECT

  public:
    NextVisionUDPLink(SharedNextVisionLinkConfigurationPtr &config);
    virtual ~NextVisionUDPLink();

    // LinkInterface overrides
    bool isConnected(void) const override;
    void disconnect(void) override;
    void testSend(void) override;

    // QThread overrides
    void run(void) override;

  public slots:
    void readBytes(void);

  private slots:
    // LinkInterface overrides
    void _writeBytes(const QByteArray data) override;

  private:
    // LinkInterface overrides
    bool _connect(void) override;

    bool _isIpLocal(const QHostAddress &add);
    bool _hardwareConnect(void);
    void _registerZeroconf(uint16_t port, const std::string &regType);
    void _deregisterZeroconf(void);
    void _writeDataGram(const QByteArray data, const NextVisionUDPCLient *target);

    bool _running;
    QUdpSocket *_socket;
    NextVisionUDPConfiguration *_udpConfig;
    bool _connectState;
    QList<NextVisionUDPCLient *> _sessionTargets;
    QMutex _sessionTargetsMutex;
    QList<QHostAddress> _localAddresses;
#if defined(QGC_ZEROCONF_ENABLED)
    DNSServiceRef _dnssServiceRef;
#endif
};
