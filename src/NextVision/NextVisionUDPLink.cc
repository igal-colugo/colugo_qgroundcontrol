/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <QDebug>
#include <QHostInfo>
#include <QList>
#include <QMutexLocker>
#include <QNetworkInterface>
#include <QNetworkProxy>
#include <QTimer>
#include <QtGlobal>
#include <iostream>

#include "NextVisionUDPLink.h"
#include "QGC.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

static bool is_ip(const QString &address)
{
    int a, b, c, d;
    if (sscanf(address.toStdString().c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4 && strcmp("::1", address.toStdString().c_str()))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static QString get_ip_address(const QString &address)
{
    if (is_ip(address))
    {
        return address;
    }
    // Need to look it up
    QHostInfo info = QHostInfo::fromName(address);
    if (info.error() == QHostInfo::NoError)
    {
        QList<QHostAddress> hostAddresses = info.addresses();
        for (int i = 0; i < hostAddresses.size(); i++)
        {
            // Exclude all IPv6 addresses
            if (!hostAddresses.at(i).toString().contains(":"))
            {
                return hostAddresses.at(i).toString();
            }
        }
    }
    return QString();
}

static bool contains_target(const QList<NextVisionUDPCLient *> list, const QHostAddress &address, quint16 port)
{
    for (int i = 0; i < list.count(); i++)
    {
        NextVisionUDPCLient *target = list[i];
        if (target->address == address && target->port == port)
        {
            return true;
        }
    }
    return false;
}

NextVisionUDPLink::NextVisionUDPLink(SharedNextVisionLinkConfigurationPtr &config)
    : NextVisionLinkInterface(config), _running(false), _socket(nullptr), _udpConfig(qobject_cast<NextVisionUDPConfiguration *>(config.get())), _connectState(false)
{
    if (!_udpConfig)
    {
        qWarning() << "Internal error";
    }
    auto allAddresses = QNetworkInterface::allAddresses();
    for (int i = 0; i < allAddresses.count(); i++)
    {
        QHostAddress &address = allAddresses[i];
        _localAddresses.append(QHostAddress(address));
    }
    moveToThread(this);
}

NextVisionUDPLink::~NextVisionUDPLink()
{
    disconnect();
    // Tell the thread to exit
    _running = false;
    // Clear client list
    qDeleteAll(_sessionTargets);
    _sessionTargets.clear();
    quit();
    // Wait for it to exit
    wait();
    this->deleteLater();
}

void NextVisionUDPLink::run()
{
    if (_hardwareConnect())
    {
        exec();
    }
    if (_socket)
    {
        _socket->close();
    }
}

bool NextVisionUDPLink::_isIpLocal(const QHostAddress &add)
{
    // In simulation and testing setups the vehicle and the GCS can be
    // running on the same host. This leads to packets arriving through
    // the local network or the loopback adapter, which makes it look
    // like the vehicle is connected through two different links,
    // complicating routing.
    //
    // We detect this case and force all traffic to a simulated instance
    // onto the local loopback interface.
    // Run through all IPv4 interfaces and check if their canonical
    // IP address in string representation matches the source IP address
    //
    // On Windows, this is a very expensive call only Redmond would know
    // why. As such, we make it once and keep the list locally. If a new
    // interface shows up after we start, it won't be on this list.
    for (int i = 0; i < _localAddresses.count(); i++)
    {
        QHostAddress &address = _localAddresses[i];
        if (address == add)
        {
            // This is a local address of the same host
            return true;
        }
    }
    return false;
}

void NextVisionUDPLink::_writeBytes(const QByteArray data)
{
    if (!_socket)
    {
        return;
    }
    emit bytesSent(this, data);

    QMutexLocker locker(&_sessionTargetsMutex);

    // Send to all manually targeted systems
    for (int i = 0; i < _udpConfig->targetHosts().count(); i++)
    {
        NextVisionUDPCLient *target = _udpConfig->targetHosts()[i];
        // Skip it if it's part of the session clients below
        if (!contains_target(_sessionTargets, target->address, target->port))
        {
            _writeDataGram(data, target);
        }
    }
    // Send to all connected systems
    for (NextVisionUDPCLient *target : _sessionTargets)
    {
        _writeDataGram(data, target);
    }
}

void NextVisionUDPLink::_writeDataGram(const QByteArray data, const NextVisionUDPCLient *target)
{
    // qDebug() << "UDP Out" << target->address << target->port;
    if (_socket->writeDatagram(data, target->address, target->port) < 0)
    {
        qWarning() << "Error writing to" << target->address << target->port;
    }
}

void NextVisionUDPLink::readBytes()
{
    if (!_socket)
    {
        return;
    }
    QByteArray databuffer;
    while (_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        // If the other end is reset then it will still report data available,
        // but will fail on the readDatagram call
        qint64 slen = _socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (slen == -1)
        {
            break;
        }
        databuffer.append(datagram);
        //-- Wait a bit before sending it over
        if (databuffer.size() > 10 * 1024)
        {
            emit bytesReceived(this, databuffer);
            databuffer.clear();
        }
        // TODO: This doesn't validade the sender. Anything sending UDP packets to this port gets
        // added to the list and will start receiving datagrams from here. Even a port scanner
        // would trigger this.
        // Add host to broadcast list if not yet present, or update its port
        QHostAddress asender = sender;
        if (_isIpLocal(sender))
        {
            asender = QHostAddress(QString("127.0.0.1"));
        }
        QMutexLocker locker(&_sessionTargetsMutex);
        if (!contains_target(_sessionTargets, asender, senderPort))
        {
            qDebug() << "Adding target" << asender << senderPort;
            NextVisionUDPCLient *target = new NextVisionUDPCLient(asender, senderPort);
            _sessionTargets.append(target);
        }
        locker.unlock();
    }
    //-- Send whatever is left
    if (databuffer.size())
    {
        emit bytesReceived(this, databuffer);
    }
}

void NextVisionUDPLink::disconnect(void)
{
    _running = false;
    quit();
    wait();
    if (_socket)
    {
        // This prevents stale signal from calling the link after it has been deleted
        QObject::disconnect(_socket, &QUdpSocket::readyRead, this, &NextVisionUDPLink::readBytes);
        // Make sure delete happen on correct thread
        _socket->deleteLater();
        _socket = nullptr;
        emit disconnected();
    }
    _connectState = false;
}

void NextVisionUDPLink::testSend(void)
{
    QByteArray data("\x49\x49\x2A", 3);
    _writeBytes(data);
}

bool NextVisionUDPLink::_connect(void)
{
    if (this->isRunning() || _running)
    {
        _running = false;
        quit();
        wait();
    }
    _running = true;
    start(NormalPriority);
    return true;
}

bool NextVisionUDPLink::_hardwareConnect()
{
    if (_socket)
    {
        delete _socket;
        _socket = nullptr;
    }
    QHostAddress host = QHostAddress::AnyIPv4;
    _socket = new QUdpSocket(this);
    _socket->setProxy(QNetworkProxy::NoProxy);
    _connectState = _socket->bind(host, _udpConfig->localPort(), QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    if (_connectState)
    {
        _socket->joinMulticastGroup(QHostAddress("224.0.0.1"));
        //-- Make sure we have a large enough IO buffers
#ifdef __mobile__
        _socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 64 * 1024);
        _socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 128 * 1024);
#else
        _socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 256 * 1024);
        _socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 512 * 1024);
#endif
        QObject::connect(_socket, &QUdpSocket::readyRead, this, &NextVisionUDPLink::readBytes);
        emit connected();
    }
    else
    {
        emit communicationError(tr("UDP Link Error"), tr("Error binding UDP port: %1").arg(_socket->errorString()));
    }
    return _connectState;
}

bool NextVisionUDPLink::isConnected() const
{
    return _connectState;
}

void NextVisionUDPLink::_registerZeroconf(uint16_t port, const std::string &regType)
{
#if defined(QGC_ZEROCONF_ENABLED)
    DNSServiceErrorType result = DNSServiceRegister(&_dnssServiceRef, 0, 0, 0, regType.c_str(), NULL, NULL, htons(port), 0, NULL, NULL, NULL);
    if (result != kDNSServiceErr_NoError)
    {
        emit communicationError(tr("UDP Link Error"), tr("Error registering Zeroconf"));
        _dnssServiceRef = NULL;
    }
#else
    Q_UNUSED(port);
    Q_UNUSED(regType);
#endif
}

//--------------------------------------------------------------------------
//-- UDPConfiguration

NextVisionUDPConfiguration::NextVisionUDPConfiguration(const QString &name) : NextVisionLinkConfiguration(name)
{
    AutoConnectSettings *settings = qgcApp()->toolbox()->settingsManager()->autoConnectSettings();
    _localPort = settings->udpListenPort()->rawValue().toInt();
    QString targetHostIP = settings->udpTargetHostIP()->rawValue().toString();
    if (!targetHostIP.isEmpty())
    {
        addHost(targetHostIP, settings->udpTargetHostPort()->rawValue().toUInt());
    }
}

NextVisionUDPConfiguration::NextVisionUDPConfiguration(NextVisionUDPConfiguration *source) : NextVisionLinkConfiguration(source)
{
    _copyFrom(source);
}

NextVisionUDPConfiguration::~NextVisionUDPConfiguration()
{
    _clearTargetHosts();
}

void NextVisionUDPConfiguration::copyFrom(NextVisionLinkConfiguration *source)
{
    NextVisionLinkConfiguration::copyFrom(source);
    _copyFrom(source);
}

void NextVisionUDPConfiguration::_copyFrom(NextVisionLinkConfiguration *source)
{
    auto *usource = qobject_cast<NextVisionUDPConfiguration *>(source);
    if (usource)
    {
        _localPort = usource->localPort();
        _clearTargetHosts();
        for (int i = 0; i < usource->targetHosts().count(); i++)
        {
            NextVisionUDPCLient *target = usource->targetHosts()[i];
            if (!contains_target(_targetHosts, target->address, target->port))
            {
                NextVisionUDPCLient *newTarget = new NextVisionUDPCLient(target);
                _targetHosts.append(newTarget);
                _updateHostList();
            }
        }
    }
    else
    {
        qWarning() << "Internal error";
    }
}

void NextVisionUDPConfiguration::_clearTargetHosts()
{
    qDeleteAll(_targetHosts);
    _targetHosts.clear();
}

/**
 * @param host Hostname in standard formatt, e.g. localhost:14551 or 192.168.1.1:14551
 */
void NextVisionUDPConfiguration::addHost(const QString host)
{
    // Handle x.x.x.x:p
    if (host.contains(":"))
    {
        addHost(host.split(":").first(), host.split(":").last().toUInt());
    }
    else
    {
        // If no port, use default
        addHost(host, _localPort);
    }
}

void NextVisionUDPConfiguration::addHost(const QString &host, quint16 port)
{
    QString ipAdd = get_ip_address(host);
    if (ipAdd.isEmpty())
    {
        qWarning() << "UDP:"
                   << "Could not resolve host:" << host << "port:" << port;
    }
    else
    {
        QHostAddress address(ipAdd);
        if (!contains_target(_targetHosts, address, port))
        {
            NextVisionUDPCLient *newTarget = new NextVisionUDPCLient(address, port);
            _targetHosts.append(newTarget);
            _updateHostList();
        }
    }
}

void NextVisionUDPConfiguration::removeHost(const QString host)
{
    if (host.contains(":"))
    {
        QHostAddress address = QHostAddress(get_ip_address(host.split(":").first()));
        quint16 port = host.split(":").last().toUInt();
        for (int i = 0; i < _targetHosts.size(); i++)
        {
            NextVisionUDPCLient *target = _targetHosts.at(i);
            if (target->address == address && target->port == port)
            {
                _targetHosts.removeAt(i);
                delete target;
                _updateHostList();
                return;
            }
        }
    }
    qWarning() << "UDP:"
               << "Could not remove unknown host:" << host;
    _updateHostList();
}

void NextVisionUDPConfiguration::setLocalPort(quint16 port)
{
    _localPort = port;
}

void NextVisionUDPConfiguration::saveSettings(QSettings &settings, const QString &root)
{
    settings.beginGroup(root);
    settings.setValue("port", (int) _localPort);
    settings.setValue("hostCount", _targetHosts.size());
    for (int i = 0; i < _targetHosts.size(); i++)
    {
        NextVisionUDPCLient *target = _targetHosts.at(i);
        QString hkey = QString("host%1").arg(i);
        settings.setValue(hkey, target->address.toString());
        QString pkey = QString("port%1").arg(i);
        settings.setValue(pkey, target->port);
    }
    settings.endGroup();
}

void NextVisionUDPConfiguration::loadSettings(QSettings &settings, const QString &root)
{
    AutoConnectSettings *acSettings = qgcApp()->toolbox()->settingsManager()->autoConnectSettings();
    _clearTargetHosts();
    settings.beginGroup(root);
    _localPort = (quint16) settings.value("port", acSettings->udpListenPort()->rawValue().toInt()).toUInt();
    int hostCount = settings.value("hostCount", 0).toInt();
    for (int i = 0; i < hostCount; i++)
    {
        QString hkey = QString("host%1").arg(i);
        QString pkey = QString("port%1").arg(i);
        if (settings.contains(hkey) && settings.contains(pkey))
        {
            addHost(settings.value(hkey).toString(), settings.value(pkey).toUInt());
        }
    }
    settings.endGroup();
    _updateHostList();
}

void NextVisionUDPConfiguration::_updateHostList()
{
    _hostList.clear();
    for (int i = 0; i < _targetHosts.size(); i++)
    {
        NextVisionUDPCLient *target = _targetHosts.at(i);
        QString host = QString("%1").arg(target->address.toString()) + ":" + QString("%1").arg(target->port);
        _hostList << host;
    }
    emit hostListChanged();
}
