/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "NextVisionLinkInterface.h"
#include "NextVisionLinkManager.h"
#include "QGCApplication.h"

QGC_LOGGING_CATEGORY(NextVisionLinkInterfaceLog, "NextVisionLinkInterfaceLog")

// The LinkManager is only forward declared in the header, so the static_assert is here instead.
static_assert(NextVisionLinkManager::invalidMavlinkChannel() == std::numeric_limits<uint8_t>::max(), "update NextVisionLinkInterface::_mavlinkChannel");

NextVisionLinkInterface::NextVisionLinkInterface(SharedNextVisionLinkConfigurationPtr &config, bool isPX4Flow) : QThread(0), _config(config)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<NextVisionLinkInterface *>("NextVisionLinkInterface*");

    // This will cause the writeBytes calls to end up on the thread of the link
    QObject::connect(this, &NextVisionLinkInterface::_invokeWriteBytes, this, &NextVisionLinkInterface::_writeBytes);
}

NextVisionLinkInterface::~NextVisionLinkInterface()
{
    if (_vehicleReferenceCount != 0)
    {
        qCWarning(LinkInterfaceLog) << "~LinkInterface still have vehicle references:" << _vehicleReferenceCount;
    }
    _config.reset();
}

uint8_t NextVisionLinkInterface::mavlinkChannel(void) const
{
    if (!mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "mavlinkChannel isSet() == false";
    }
    return _mavlinkChannel;
}

bool NextVisionLinkInterface::mavlinkChannelIsSet(void) const
{
    return (NextVisionLinkManager::invalidMavlinkChannel() != _mavlinkChannel);
}

bool NextVisionLinkInterface::_allocateMavlinkChannel()
{
    // should only be called by the LinkManager during setup
    Q_ASSERT(!mavlinkChannelIsSet());
    if (mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "_allocateMavlinkChannel already have " << _mavlinkChannel;
        return true;
    }

    auto mgr = qgcApp()->toolbox()->nextVisionLinkManager();
    _mavlinkChannel = mgr->allocateMavlinkChannel();
    if (!mavlinkChannelIsSet())
    {
        qCWarning(LinkInterfaceLog) << "_allocateMavlinkChannel failed";
        return false;
    }
    qCDebug(LinkInterfaceLog) << "_allocateMavlinkChannel" << _mavlinkChannel;
    return true;
}

void NextVisionLinkInterface::_freeMavlinkChannel()
{
    qCDebug(LinkInterfaceLog) << "_freeMavlinkChannel" << _mavlinkChannel;
    if (NextVisionLinkManager::invalidMavlinkChannel() == _mavlinkChannel)
    {
        return;
    }

    auto mgr = qgcApp()->toolbox()->nextVisionLinkManager();
    mgr->freeMavlinkChannel(_mavlinkChannel);
    _mavlinkChannel = NextVisionLinkManager::invalidMavlinkChannel();
}

void NextVisionLinkInterface::writeBytesThreadSafe(const char *bytes, int length)
{
    emit _invokeWriteBytes(QByteArray(bytes, length));
}

void NextVisionLinkInterface::addVehicleReference(void)
{
    _vehicleReferenceCount++;
}

void NextVisionLinkInterface::removeVehicleReference(void)
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

void NextVisionLinkInterface::_connectionRemoved(void)
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
