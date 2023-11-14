/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "EpsilonTCPLink.h"
#include "EpsilonLinkManager.h"
#include "QGC.h"
#include <QDebug>
#include <QHostInfo>
#include <QList>
#include <QMutexLocker>
#include <QSignalSpy>
#include <QTimer>
#include <iostream>

EpsilonTCPLink::EpsilonTCPLink(SharedEpsilonLinkConfigurationPtr &config)
    : EpsilonLinkInterface(config), _tcpConfig(qobject_cast<EpsilonTCPConfiguration *>(config.get())), _socket(nullptr), _socketIsConnected(false)
{
    Q_ASSERT(_tcpConfig);
}

EpsilonTCPLink::~EpsilonTCPLink()
{
    disconnect();
}

#ifdef TCPLINK_READWRITE_DEBUG
void EpsilonTCPLink::_writeDebugBytes(const QByteArray data)
{
    QString bytes;
    QString ascii;
    for (int i = 0, size = data.size(); i < size; i++)
    {
        unsigned char v = data[i];
        bytes.append(QString::asprintf("%02x ", v));
        if (data[i] > 31 && data[i] < 127)
        {
            ascii.append(data[i]);
        }
        else
        {
            ascii.append(219);
        }
    }
    qDebug() << "Sent" << size << "bytes to" << _tcpConfig->address().toString() << ":" << _tcpConfig->port() << "data:";
    qDebug() << bytes;
    qDebug() << "ASCII:" << ascii;
}
#endif

void EpsilonTCPLink::_writeBytes(const QByteArray data)
{
#ifdef TCPLINK_READWRITE_DEBUG
    _writeDebugBytes(data);
#endif

    if (_socket)
    {
        _socket->write(data);
        emit bytesSent(this, data);
    }
}

void EpsilonTCPLink::_readBytes()
{
    if (_socket)
    {
        qint64 byteCount = _socket->bytesAvailable();
        if (byteCount)
        {
            QByteArray buffer;
            buffer.resize(byteCount);
            _socket->read(buffer.data(), buffer.size());
            emit bytesReceived(this, buffer);
#ifdef TCPLINK_READWRITE_DEBUG
            writeDebugBytes(buffer.data(), buffer.size());
#endif
        }
    }
}

void EpsilonTCPLink::disconnect(void)
{
    if (_socket)
    {
        // This prevents stale signal from calling the link after it has been deleted
        QObject::disconnect(_socket, &QIODevice::readyRead, this, &EpsilonTCPLink::_readBytes);
        _socketIsConnected = false;
        _socket->disconnectFromHost(); // Disconnect tcp
        _socket->deleteLater();        // Make sure delete happens on correct thread
        _socket = nullptr;
        emit disconnected();
    }
}

void EpsilonTCPLink::testSend(void)
{
}

bool EpsilonTCPLink::_connect(void)
{
    if (_socket)
    {
        qWarning() << "connect called while already connected";
        return true;
    }

    return _hardwareConnect();
}

bool EpsilonTCPLink::_hardwareConnect()
{
    Q_ASSERT(_socket == nullptr);
    _socket = new QTcpSocket();
    QObject::connect(_socket, &QIODevice::readyRead, this, &EpsilonTCPLink::_readBytes);

    QSignalSpy errorSpy(_socket, &QAbstractSocket::errorOccurred);
    QObject::connect(_socket, &QAbstractSocket::errorOccurred, this, &EpsilonTCPLink::_socketError);

    _socket->connectToHost(_tcpConfig->address(), _tcpConfig->port());

    // Give the socket a second to connect to the other side otherwise error out
    if (!_socket->waitForConnected(1000))
    {
        // Whether a failed connection emits an error signal or not is platform specific.
        // So in cases where it is not emitted, we emit one ourselves.
        if (errorSpy.count() == 0)
        {
            emit communicationError(tr("Link Error"), tr("Error on link %1. Connection failed").arg(_config->name()));
        }
        delete _socket;
        _socket = nullptr;
        return false;
    }
    _socketIsConnected = true;
    emit connected();
    return true;
}

void EpsilonTCPLink::_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit communicationError(tr("Link Error"), tr("Error on link %1. Error on socket: %2.").arg(_config->name()).arg(_socket->errorString()));
}

/**
 * @brief Check if connection is active.
 *
 * @return True if link is connected, false otherwise.
 **/
bool EpsilonTCPLink::isConnected() const
{
    return _socketIsConnected;
}

//--------------------------------------------------------------------------
//-- TCPConfiguration

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
        return address;
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
    return {};
}

EpsilonTCPConfiguration::EpsilonTCPConfiguration(const QString &name) : EpsilonLinkConfiguration(name)
{
    _port = QGC_TCP_PORT;
    _address = QHostAddress::Any;
}

EpsilonTCPConfiguration::EpsilonTCPConfiguration(EpsilonTCPConfiguration *source) : EpsilonLinkConfiguration(source)
{
    _port = source->port();
    _address = source->address();
}

void EpsilonTCPConfiguration::copyFrom(EpsilonLinkConfiguration *source)
{
    EpsilonLinkConfiguration::copyFrom(source);
    auto *usource = qobject_cast<EpsilonTCPConfiguration *>(source);
    Q_ASSERT(usource != nullptr);
    _port = usource->port();
    _address = usource->address();
}

void EpsilonTCPConfiguration::setPort(quint16 port)
{
    _port = port;
}

void EpsilonTCPConfiguration::setAddress(const QHostAddress &address)
{
    _address = address;
}

void EpsilonTCPConfiguration::setHost(const QString host)
{
    QString ipAdd = get_ip_address(host);
    if (ipAdd.isEmpty())
    {
        qWarning() << "TCP:"
                   << "Could not resolve host:" << host;
    }
    else
    {
        _address = QHostAddress(ipAdd);
    }
}

void EpsilonTCPConfiguration::saveSettings(QSettings &settings, const QString &root)
{
    settings.beginGroup(root);
    settings.setValue("port", (int) _port);
    settings.setValue("host", address().toString());
    settings.endGroup();
}

void EpsilonTCPConfiguration::loadSettings(QSettings &settings, const QString &root)
{
    settings.beginGroup(root);
    _port = (quint16) settings.value("port", QGC_TCP_PORT).toUInt();
    QString address = settings.value("host", _address.toString()).toString();
    _address = QHostAddress(address);
    settings.endGroup();
}
