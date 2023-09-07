/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <QApplication>
#include <QDebug>
#include <QList>
#include <QSignalSpy>

#include <memory>

#include "NextVisionLinkManager.h"
#include "NextVisionTCPLink.h"
#include "NextVisionUDPLink.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/mdns.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

QGC_LOGGING_CATEGORY(NextVisionLinkManagerLog, "NextVisionLinkManagerLog")
QGC_LOGGING_CATEGORY(NextvisionLinkManagerVerboseLog, "NextVisionLinkManagerVerboseLog")

const char *NextVisionLinkManager::_defaultUDPLinkName = "UDP Link (AutoConnect)";

NextVisionLinkManager::NextVisionLinkManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), _selectedConfigurationId(-1), _configUpdateSuspended(false), _configurationsLoaded(false), _connectionsSuspended(false),
      _mavlinkChannelsUsedBitMask(1) // We never use channel 0 to avoid sequence numbering problems
      ,
      _mavlinkProtocol(nullptr)
{
    qmlRegisterUncreatableType<NextVisionLinkManager>("QGroundControl", 1, 0, "NextVisionLinkManager", "Reference only");
    qmlRegisterUncreatableType<NextVisionLinkConfiguration>("QGroundControl", 1, 0, "NextVisionLinkConfiguration", "Reference only");
    qmlRegisterUncreatableType<NextVisionLinkInterface>("QGroundControl", 1, 0, "NextVisionLinkInterface", "Reference only");
}

NextVisionLinkManager::~NextVisionLinkManager()
{
}

void NextVisionLinkManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    _mavlinkProtocol = _toolbox->nextVisionMavlinkProtocol();
}

// This should only be used by Qml code
void NextVisionLinkManager::createConnectedLink(NextVisionLinkConfiguration *config)
{
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedNextVisionLinkConfigurationPtr &sharedConfig = _rgLinkConfigs[i];
        if (sharedConfig.get() == config)
            createConnectedLink(sharedConfig);
    }
}

bool NextVisionLinkManager::createConnectedLink(SharedNextVisionLinkConfigurationPtr &config)
{
    SharedNextVisionLinkInterfacePtr link = nullptr;

    switch (config->type())
    {
    case NextVisionLinkConfiguration::TypeUdp:
        link = std::make_shared<NextVisionUDPLink>(config);
        break;
    case NextVisionLinkConfiguration::TypeTcp:
        link = std::make_shared<NextVisionTCPLink>(config);
        break;
    case NextVisionLinkConfiguration::TypeLast:
        break;
    }

    if (link)
    {
        if (false == link->_allocateMavlinkChannel())
        {
            qCWarning(NextVisionLinkManagerLog) << "Link failed to setup mavlink channels";
            return false;
        }

        _rgLinks.append(link);
        _selectedConfigurationId = _rgLinks.count() - 1;
        config->setLink(link);

        connect(link.get(), &NextVisionLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
        connect(link.get(), &NextVisionLinkInterface::bytesReceived, _mavlinkProtocol, &NextVisionMAVLinkProtocol::receiveBytes);
        connect(link.get(), &NextVisionLinkInterface::bytesSent, _mavlinkProtocol, &NextVisionMAVLinkProtocol::logSentBytes);
        connect(link.get(), &NextVisionLinkInterface::disconnected, this, &NextVisionLinkManager::_linkDisconnected);

        if (!link->_connect())
        {
            return false;
        }

        return true;
    }

    return false;
}

void NextVisionLinkManager::disconnectAll(void)
{
    QList<SharedNextVisionLinkInterfacePtr> links = _rgLinks;

    for (const SharedNextVisionLinkInterfacePtr &sharedLink : links)
    {
        sharedLink->disconnect();
    }
}

void NextVisionLinkManager::_linkDisconnected(void)
{
    NextVisionLinkInterface *link = qobject_cast<NextVisionLinkInterface *>(sender());

    if (!link || !containsLink(link))
    {
        return;
    }

    disconnect(link, &NextVisionLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
    disconnect(link, &NextVisionLinkInterface::disconnected, this, &NextVisionLinkManager::_linkDisconnected);

    link->_freeMavlinkChannel();
    for (int i = 0; i < _rgLinks.count(); i++)
    {
        if (_rgLinks[i].get() == link)
        {
            qCDebug(NextVisionLinkManagerLog) << "LinkManager::_linkDisconnected" << _rgLinks[i]->linkConfiguration()->name() << _rgLinks[i].use_count();
            _rgLinks.removeAt(i);
            return;
        }
    }
}

SharedNextVisionLinkInterfacePtr NextVisionLinkManager::sharedLinkInterfacePointerForLink(NextVisionLinkInterface *link, bool ignoreNull)
{
    for (int i = 0; i < _rgLinks.count(); i++)
    {
        if (_rgLinks[i].get() == link)
        {
            return _rgLinks[i];
        }
    }

    if (!ignoreNull)
        qWarning() << "LinkManager::sharedLinkInterfaceForLink returning nullptr";
    return SharedNextVisionLinkInterfacePtr(nullptr);
}

SharedNextVisionLinkInterfacePtr NextVisionLinkManager::selectedSharedLinkInterfacePointerForLink(void)
{
    SharedNextVisionLinkInterfacePtr returnValue = nullptr;

    if (_rgLinks.count() > _selectedConfigurationId && _selectedConfigurationId > -1)
    {
        returnValue = _rgLinks[_selectedConfigurationId];
    }

    return returnValue;
}

/// @brief If all new connections should be suspended a message is displayed to the user and true
///         is returned;
bool NextVisionLinkManager::_connectionsSuspendedMsg(void)
{
    if (_connectionsSuspended)
    {
        qgcApp()->showAppMessage(tr("Connect not allowed: %1").arg(_connectionsSuspendedReason));
        return true;
    }
    else
    {
        return false;
    }
}

void NextVisionLinkManager::setConnectionsSuspended(QString reason)
{
    _connectionsSuspended = true;
    _connectionsSuspendedReason = reason;
}

void NextVisionLinkManager::suspendConfigurationUpdates(bool suspend)
{
    _configUpdateSuspended = suspend;
}

void NextVisionLinkManager::saveLinkConfigurationList()
{
    QSettings settings;
    settings.remove(NextVisionLinkConfiguration::settingsRoot());
    int trueCount = 0;
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedNextVisionLinkConfigurationPtr linkConfig = _rgLinkConfigs[i];
        if (linkConfig)
        {
            QString root = NextVisionLinkConfiguration::settingsRoot();
            root += QString("/Link%1").arg(trueCount++);
            settings.setValue(root + "/name", linkConfig->name());
            settings.setValue(root + "/type", linkConfig->type());
            // Have the instance save its own values
            linkConfig->saveSettings(settings, root);
        }
        else
        {
            qWarning() << "Internal error for link configuration in LinkManager";
        }
    }
    QString root(NextVisionLinkConfiguration::settingsRoot());
    settings.setValue(root + "/count", trueCount);
}

void NextVisionLinkManager::loadLinkConfigurationList()
{
    QSettings settings;
    // Is the group even there?
    if (settings.contains(NextVisionLinkConfiguration::settingsRoot() + "/count"))
    {
        // Find out how many configurations we have
        int count = settings.value(NextVisionLinkConfiguration::settingsRoot() + "/count").toInt();
        for (int i = 0; i < count; i++)
        {
            QString root(NextVisionLinkConfiguration::settingsRoot());
            root += QString("/Link%1").arg(i);
            if (settings.contains(root + "/type"))
            {
                NextVisionLinkConfiguration::LinkType type = static_cast<NextVisionLinkConfiguration::LinkType>(settings.value(root + "/type").toInt());
                if (type < NextVisionLinkConfiguration::TypeLast)
                {
                    if (settings.contains(root + "/name"))
                    {
                        QString name = settings.value(root + "/name").toString();
                        if (!name.isEmpty())
                        {
                            NextVisionLinkConfiguration *link = nullptr;

                            switch (type)
                            {
#
                            case NextVisionLinkConfiguration::TypeUdp:
                                link = new NextVisionUDPConfiguration(name);
                                break;
                            case NextVisionLinkConfiguration::TypeTcp:
                                link = new NextVisionTCPConfiguration(name);
                                break;
                            case NextVisionLinkConfiguration::TypeLast:
                                break;
                            }
                            if (link)
                            {
                                //-- Have the instance load its own values
                                link->loadSettings(settings, root);
                                addConfiguration(link);
                            }
                        }
                        else
                        {
                            qWarning() << "Link Configuration" << root << "has an empty name.";
                        }
                    }
                    else
                    {
                        qWarning() << "Link Configuration" << root << "has no name.";
                    }
                }
                else
                {
                    qWarning() << "Link Configuration" << root << "an invalid type: " << type;
                }
            }
            else
            {
                qWarning() << "Link Configuration" << root << "has no type.";
            }
        }
    }

    // Enable automatic Serial PX4/3DR Radio hunting
    _configurationsLoaded = true;
}

void NextVisionLinkManager::shutdown(void)
{
    setConnectionsSuspended(tr("Shutdown"));
    disconnectAll();
}

QStringList NextVisionLinkManager::linkTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("UDP");
        list += tr("TCP");
        if (list.size() != static_cast<int>(NextVisionLinkConfiguration::TypeLast))
        {
            qWarning() << "Internal error";
        }
    }
    return list;
}

bool NextVisionLinkManager::endConfigurationEditing(NextVisionLinkConfiguration *config, NextVisionLinkConfiguration *editedConfig)
{
    if (config && editedConfig)
    {
        config->copyFrom(editedConfig);
        saveLinkConfigurationList();
        emit config->nameChanged(config->name());
        // Discard temporary duplicate
        delete editedConfig;
    }
    else
    {
        qWarning() << "Internal error";
    }
    return true;
}

bool NextVisionLinkManager::endCreateConfiguration(NextVisionLinkConfiguration *config)
{
    if (config)
    {
        addConfiguration(config);
        saveLinkConfigurationList();
    }
    else
    {
        qWarning() << "Internal error";
    }
    return true;
}

NextVisionLinkConfiguration *NextVisionLinkManager::createConfiguration(int type, const QString &name)
{
    return NextVisionLinkConfiguration::createSettings(type, name);
}

NextVisionLinkConfiguration *NextVisionLinkManager::startConfigurationEditing(NextVisionLinkConfiguration *config)
{
    if (config)
    {
        return NextVisionLinkConfiguration::duplicateSettings(config);
    }
    else
    {
        qWarning() << "Internal error";
        return nullptr;
    }
}

void NextVisionLinkManager::selectConfiguration(NextVisionLinkConfiguration *config)
{
    if (config)
    {
        if (_rgLinks.count() > 0)
        {
            for (int i = 0; i < _rgLinks.count(); i++)
            {
                if (_rgLinkConfigs[i].get() == config)
                {
                    _selectedConfigurationId = i;
                    break;
                }
            }
        }
    }
}

void NextVisionLinkManager::removeConfiguration(NextVisionLinkConfiguration *config)
{
    if (config)
    {
        NextVisionLinkInterface *link = config->link();
        if (link)
        {
            link->disconnect();
        }

        _removeConfiguration(config);
        saveLinkConfigurationList();

        _selectedConfigurationId = _rgLinks.count() - 1;
    }
    else
    {
        qWarning() << "Internal error";
    }
}

void NextVisionLinkManager::_removeConfiguration(NextVisionLinkConfiguration *config)
{
    _qmlConfigurations.removeOne(config);
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        if (_rgLinkConfigs[i].get() == config)
        {
            _rgLinkConfigs.removeAt(i);
            return;
        }
    }
    qWarning() << "LinkManager::_removeConfiguration called with unknown config";
}

bool NextVisionLinkManager::containsLink(NextVisionLinkInterface *link)
{
    for (int i = 0; i < _rgLinks.count(); i++)
    {
        if (_rgLinks[i].get() == link)
        {
            return true;
        }
    }
    return false;
}

SharedNextVisionLinkConfigurationPtr NextVisionLinkManager::addConfiguration(NextVisionLinkConfiguration *config)
{
    _qmlConfigurations.append(config);
    _rgLinkConfigs.append(SharedNextVisionLinkConfigurationPtr(config));

    return _rgLinkConfigs.last();
}

uint8_t NextVisionLinkManager::allocateMavlinkChannel(void)
{
    // Find a mavlink channel to use for this link
    for (uint8_t mavlinkChannel = 0; mavlinkChannel < MAVLINK_COMM_NUM_BUFFERS; mavlinkChannel++)
    {
        if (!(_mavlinkChannelsUsedBitMask & 1 << mavlinkChannel))
        {
            mavlink_reset_channel_status(mavlinkChannel);
            // Start the channel on Mav 1 protocol
            mavlink_status_t *mavlinkStatus = mavlink_get_channel_status(mavlinkChannel);
            mavlinkStatus->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
            _mavlinkChannelsUsedBitMask |= 1 << mavlinkChannel;
            qCDebug(NextVisionLinkManagerLog) << "allocateMavlinkChannel" << mavlinkChannel;
            return mavlinkChannel;
        }
    }
    qWarning(NextVisionLinkManagerLog) << "allocateMavlinkChannel: all channels reserved!";
    return invalidMavlinkChannel(); // All channels reserved
}

void NextVisionLinkManager::freeMavlinkChannel(uint8_t channel)
{
    qCDebug(NextVisionLinkManagerLog) << "freeMavlinkChannel" << channel;
    if (invalidMavlinkChannel() == channel)
    {
        return;
    }
    _mavlinkChannelsUsedBitMask &= ~(1 << channel);
}
