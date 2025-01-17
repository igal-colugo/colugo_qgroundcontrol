/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "CommtactTCPLink.h"
#include "CommtactLinkManager.h"
#include <QDebug>
#include <QHostInfo>
#include <QList>
#include <QMutexLocker>
#include <QSignalSpy>
#include <QTimer>
#include <iostream>

CommtactTCPLink::CommtactTCPLink(SharedCommtactLinkConfigurationPtr &config)
    : CommtactLinkInterface(config), _tcpConfig(qobject_cast<CommtactTCPConfiguration *>(config.get())), _socket(nullptr), _socketIsConnected(false)
{
    Q_ASSERT(_tcpConfig);
}

CommtactTCPLink::~CommtactTCPLink()
{
    disconnect();
}

#ifdef TCPLINK_READWRITE_DEBUG
void CommtactTCPLink::_writeDebugBytes(const QByteArray data)
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

void CommtactTCPLink::_writeBytes(const QByteArray data)
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

void CommtactTCPLink::_readBytes()
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

void CommtactTCPLink::disconnect(void)
{
    if (_socket)
    {
        // This prevents stale signal from calling the link after it has been deleted
        QObject::disconnect(_socket, &QIODevice::readyRead, this, &CommtactTCPLink::_readBytes);
        _socketIsConnected = false;
        _socket->disconnectFromHost(); // Disconnect tcp
        _socket->deleteLater();        // Make sure delete happens on correct thread
        _socket = nullptr;
        emit disconnected();
    }
}

void CommtactTCPLink::testSend(void)
{
}

bool CommtactTCPLink::_connect(void)
{
    if (_socket)
    {
        qWarning() << "connect called while already connected";
        return true;
    }

    return _hardwareConnect();
}

bool CommtactTCPLink::_hardwareConnect()
{
    Q_ASSERT(_socket == nullptr);
    _socket = new QTcpSocket();
    QObject::connect(_socket, &QIODevice::readyRead, this, &CommtactTCPLink::_readBytes);

    QSignalSpy errorSpy(_socket, &QAbstractSocket::errorOccurred);
    QObject::connect(_socket, &QAbstractSocket::errorOccurred, this, &CommtactTCPLink::_socketError);

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

void CommtactTCPLink::_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit communicationError(tr("Link Error"), tr("Error on link %1. Error on socket: %2.").arg(_config->name()).arg(_socket->errorString()));
}

/**
 * @brief Check if connection is active.
 *
 * @return True if link is connected, false otherwise.
 **/
bool CommtactTCPLink::isConnected() const
{
    return _socketIsConnected;
}

//---------------------- TCPConfiguration ----------------------------------

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

CommtactTCPConfiguration::CommtactTCPConfiguration(const QString &name) : CommtactLinkConfiguration(name)
{
    _port = COMMTACT_TCP_PORT;
    _address = QHostAddress::Any;
}

CommtactTCPConfiguration::CommtactTCPConfiguration(CommtactTCPConfiguration *source) : CommtactLinkConfiguration(source)
{
    _port = source->port();
    _address = source->address();
}

void CommtactTCPConfiguration::copyFrom(CommtactLinkConfiguration *source)
{
    CommtactLinkConfiguration::copyFrom(source);
    auto *usource = qobject_cast<CommtactTCPConfiguration *>(source);
    Q_ASSERT(usource != nullptr);
    _port = usource->port();
    _address = usource->address();
}

void CommtactTCPConfiguration::setPort(quint16 port)
{
    _port = port;
}

void CommtactTCPConfiguration::setAddress(const QHostAddress &address)
{
    _address = address;
}

void CommtactTCPConfiguration::setHost(const QString host)
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

void CommtactTCPConfiguration::saveSettings(QSettings &settings, const QString &root)
{
    settings.beginGroup(root);
    settings.setValue("port", (int) _port);
    settings.setValue("host", address().toString());
    settings.endGroup();
}

void CommtactTCPConfiguration::loadSettings(QSettings &settings, const QString &root)
{
    settings.beginGroup(root);
    _port = (quint16) settings.value("port", COMMTACT_TCP_PORT).toUInt();
    QString address = settings.value("host", _address.toString()).toString();
    _address = QHostAddress(address);
    settings.endGroup();
}
