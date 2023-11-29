/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "EpsilonLinkInterface.h"
#include "EpsilonLinkManager.h"
#include "QGCApplication.h"

QGC_LOGGING_CATEGORY(EpsilonLinkInterfaceLog, "EpsilonLinkInterfaceLog")

EpsilonLinkInterface::EpsilonLinkInterface(SharedEpsilonLinkConfigurationPtr &config, bool isPX4Flow) : QThread(0), _config(config)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<EpsilonLinkInterface *>("EpsilonLinkInterface*");

    // This will cause the writeBytes calls to end up on the thread of the link
    QObject::connect(this, &EpsilonLinkInterface::_invokeWriteBytes, this, &EpsilonLinkInterface::_writeBytes);
}

EpsilonLinkInterface::~EpsilonLinkInterface()
{
    if (_vehicleReferenceCount != 0)
    {
        qCWarning(LinkInterfaceLog) << "~LinkInterface still have vehicle references:" << _vehicleReferenceCount;
    }
    _config.reset();
}

void EpsilonLinkInterface::writeBytesThreadSafe(const char *bytes, int length)
{
    emit _invokeWriteBytes(QByteArray(bytes, length));
}

void EpsilonLinkInterface::addVehicleReference(void)
{
    _vehicleReferenceCount++;
}

void EpsilonLinkInterface::removeVehicleReference(void)
{
    if (_vehicleReferenceCount != 0)
    {
        _vehicleReferenceCount--;
        if (_vehicleReferenceCount == 0)
        {
            disconnect();
        }
    }
    else
    {
        qCWarning(LinkInterfaceLog) << "removeVehicleReference called with no vehicle references";
    }
}

void EpsilonLinkInterface::_connectionRemoved(void)
{
    if (_vehicleReferenceCount == 0)
    {
        // Since there are no vehicles on the link we can disconnect it right now
        disconnect();
    }
    else
    {
        // If there are still vehicles on this link we allow communication lost to trigger and don't automatically disconect until all the vehicles go away
    }
}
