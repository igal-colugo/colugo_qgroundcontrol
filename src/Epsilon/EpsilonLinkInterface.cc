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

// The LinkManager is only forward declared in the header, so the static_assert is here instead.
static_assert(EpsilonLinkManager::invalidMavlinkChannel() == std::numeric_limits<uint8_t>::max(), "update EpsilonLinkInterface::_mavlinkChannel");

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

uint8_t EpsilonLinkInterface::mavlinkChannel(void) const
{
    if (!mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "mavlinkChannel isSet() == false";
    }
    return _mavlinkChannel;
}

bool EpsilonLinkInterface::mavlinkChannelIsSet(void) const
{
    return (EpsilonLinkManager::invalidMavlinkChannel() != _mavlinkChannel);
}

bool EpsilonLinkInterface::_allocateMavlinkChannel()
{
    // should only be called by the LinkManager during setup
    Q_ASSERT(!mavlinkChannelIsSet());
    if (mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "_allocateMavlinkChannel already have " << _mavlinkChannel;
        return true;
    }

    auto mgr = qgcApp()->toolbox()->epsilonLinkManager();
    _mavlinkChannel = mgr->allocateMavlinkChannel();
    if (!mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "_allocateMavlinkChannel failed";
        return false;
    }
    qCDebug(LinkInterfaceLog) << "_allocateMavlinkChannel" << _mavlinkChannel;
    return true;
}

void EpsilonLinkInterface::_freeMavlinkChannel()
{
    qCDebug(LinkInterfaceLog) << "_freeMavlinkChannel" << _mavlinkChannel;
    if (EpsilonLinkManager::invalidMavlinkChannel() == _mavlinkChannel)
    {
        return;
    }

    auto mgr = qgcApp()->toolbox()->epsilonLinkManager();
    mgr->freeMavlinkChannel(_mavlinkChannel);
    _mavlinkChannel = EpsilonLinkManager::invalidMavlinkChannel();
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
