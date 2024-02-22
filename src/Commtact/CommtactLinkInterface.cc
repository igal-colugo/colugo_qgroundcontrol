/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "CommtactLinkInterface.h"
#include "CommtactLinkManager.h"

CommtactLinkInterface::CommtactLinkInterface(SharedCommtactLinkConfigurationPtr &config) : QThread(0), _config(config)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<CommtactLinkInterface *>("CommtactLinkInterface*");

    // This will cause the writeBytes calls to end up on the thread of the link
    QObject::connect(this, &CommtactLinkInterface::_invokeWriteBytes, this, &CommtactLinkInterface::_writeBytes);
}

CommtactLinkInterface::~CommtactLinkInterface()
{
    _config.reset();
}

void CommtactLinkInterface::writeBytesThreadSafe(const char *bytes, int length)
{
    emit _invokeWriteBytes(QByteArray(bytes, length));
}

void CommtactLinkInterface::_connectionRemoved(void)
{
    disconnect();
}
