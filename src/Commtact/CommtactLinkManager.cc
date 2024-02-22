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

#include "CommtactLinkManager.h"
#include "CommtactTCPLink.h"
#include "CommtactUDPLink.h"

#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/mdns.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

QGC_LOGGING_CATEGORY(CommtactLinkManagerLog, "CommtactLinkManagerLog")
QGC_LOGGING_CATEGORY(CommtactLinkManagerVerboseLog, "CommtactLinkManagerVerboseLog")

const char *CommtactLinkManager::_defaultUDPLinkName = "UDP Link (AutoConnect)";

CommtactLinkManager::CommtactLinkManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), _selectedConfigurationId(-1), _configUpdateSuspended(false), _configurationsLoaded(false), _connectionsSuspended(false), _linkProtocol(nullptr)
{
    qmlRegisterUncreatableType<CommtactLinkManager>("QGroundControl", 1, 0, "CommtactLinkManager", "Reference only");
    qmlRegisterUncreatableType<CommtactLinkConfiguration>("QGroundControl", 1, 0, "CommtactLinkConfiguration", "Reference only");
    qmlRegisterUncreatableType<CommtactLinkInterface>("QGroundControl", 1, 0, "CommtactLinkInterface", "Reference only");
}

CommtactLinkManager::~CommtactLinkManager()
{
}

void CommtactLinkManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    _linkProtocol = _toolbox->commtactLinkProtocol();
}

// This should only be used by Qml code
void CommtactLinkManager::createConnectedLink(CommtactLinkConfiguration *config)
{
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedCommtactLinkConfigurationPtr &sharedConfig = _rgLinkConfigs[i];
        if (sharedConfig.get() == config)
            createConnectedLink(sharedConfig);
    }
}

bool CommtactLinkManager::createConnectedLink(SharedCommtactLinkConfigurationPtr &config)
{
    SharedCommtactLinkInterfacePtr link = nullptr;

    switch (config->type())
    {
    case CommtactLinkConfiguration::TypeUdp:
        link = std::make_shared<CommtactUDPLink>(config);
        break;
    case CommtactLinkConfiguration::TypeTcp:
        link = std::make_shared<CommtactTCPLink>(config);
        break;
    case CommtactLinkConfiguration::TypeLast:
        break;
    }

    if (link)
    {
        _rgLinks.append(link);
        _selectedConfigurationId = _rgLinks.count() - 1;
        config->setLink(link);

        connect(link.get(), &CommtactLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
        connect(link.get(), &CommtactLinkInterface::bytesReceived, _linkProtocol, &CommtactLinkProtocol::receiveBytes);
        connect(link.get(), &CommtactLinkInterface::bytesSent, _linkProtocol, &CommtactLinkProtocol::logSentBytes);
        connect(link.get(), &CommtactLinkInterface::disconnected, this, &CommtactLinkManager::_linkDisconnected);

        if (!link->_connect())
        {
            return false;
        }

        return true;
    }

    return false;
}

void CommtactLinkManager::disconnectAll(void)
{
    QList<SharedCommtactLinkInterfacePtr> links = _rgLinks;

    for (const SharedCommtactLinkInterfacePtr &sharedLink : links)
    {
        sharedLink->disconnect();
    }
}

void CommtactLinkManager::_linkDisconnected(void)
{
    CommtactLinkInterface *link = qobject_cast<CommtactLinkInterface *>(sender());

    if (!link || !containsLink(link))
    {
        return;
    }

    disconnect(link, &CommtactLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
    disconnect(link, &CommtactLinkInterface::disconnected, this, &CommtactLinkManager::_linkDisconnected);

    for (int i = 0; i < _rgLinks.count(); i++)
    {
        if (_rgLinks[i].get() == link)
        {
            qCDebug(CommtactLinkManagerLog) << "LinkManager::_linkDisconnected" << _rgLinks[i]->linkConfiguration()->name() << _rgLinks[i].use_count();
            _rgLinks.removeAt(i);
            return;
        }
    }
}

SharedCommtactLinkInterfacePtr CommtactLinkManager::sharedLinkInterfacePointerForLink(CommtactLinkInterface *link, bool ignoreNull)
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
    return SharedCommtactLinkInterfacePtr(nullptr);
}

SharedCommtactLinkInterfacePtr CommtactLinkManager::selectedSharedLinkInterfacePointerForLink(void)
{
    SharedCommtactLinkInterfacePtr returnValue = nullptr;

    if (_rgLinks.count() > _selectedConfigurationId && _selectedConfigurationId > -1)
    {
        returnValue = _rgLinks[_selectedConfigurationId];
    }

    return returnValue;
}

/// @brief If all new connections should be suspended a message is displayed to the user and true
///         is returned;
bool CommtactLinkManager::_connectionsSuspendedMsg(void)
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

void CommtactLinkManager::setConnectionsSuspended(QString reason)
{
    _connectionsSuspended = true;
    _connectionsSuspendedReason = reason;
}

void CommtactLinkManager::suspendConfigurationUpdates(bool suspend)
{
    _configUpdateSuspended = suspend;
}

void CommtactLinkManager::saveLinkConfigurationList()
{
    QSettings settings;
    settings.remove(CommtactLinkConfiguration::settingsRoot());
    int trueCount = 0;
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedCommtactLinkConfigurationPtr linkConfig = _rgLinkConfigs[i];
        if (linkConfig)
        {
            QString root = CommtactLinkConfiguration::settingsRoot();
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
    QString root(CommtactLinkConfiguration::settingsRoot());
    settings.setValue(root + "/count", trueCount);
}

void CommtactLinkManager::loadLinkConfigurationList()
{
    QSettings settings;
    // Is the group even there?
    if (settings.contains(CommtactLinkConfiguration::settingsRoot() + "/count"))
    {
        // Find out how many configurations we have
        int count = settings.value(CommtactLinkConfiguration::settingsRoot() + "/count").toInt();
        for (int i = 0; i < count; i++)
        {
            QString root(CommtactLinkConfiguration::settingsRoot());
            root += QString("/Link%1").arg(i);
            if (settings.contains(root + "/type"))
            {
                CommtactLinkConfiguration::LinkType type = static_cast<CommtactLinkConfiguration::LinkType>(settings.value(root + "/type").toInt());
                if (type < CommtactLinkConfiguration::TypeLast)
                {
                    if (settings.contains(root + "/name"))
                    {
                        QString name = settings.value(root + "/name").toString();
                        if (!name.isEmpty())
                        {
                            CommtactLinkConfiguration *link = nullptr;

                            switch (type)
                            {
#
                            case CommtactLinkConfiguration::TypeUdp:
                                link = new CommtactUDPConfiguration(name);
                                break;
                            case CommtactLinkConfiguration::TypeTcp:
                                link = new CommtactTCPConfiguration(name);
                                break;
                            case CommtactLinkConfiguration::TypeLast:
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

void CommtactLinkManager::shutdown(void)
{
    setConnectionsSuspended(tr("Shutdown"));
    disconnectAll();
}

QStringList CommtactLinkManager::linkTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("UDP");
        list += tr("TCP");
        if (list.size() != static_cast<int>(CommtactLinkConfiguration::TypeLast))
        {
            qWarning() << "Internal error";
        }
    }
    return list;
}

bool CommtactLinkManager::endConfigurationEditing(CommtactLinkConfiguration *config, CommtactLinkConfiguration *editedConfig)
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

bool CommtactLinkManager::endCreateConfiguration(CommtactLinkConfiguration *config)
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

CommtactLinkConfiguration *CommtactLinkManager::createConfiguration(int type, const QString &name)
{
    return CommtactLinkConfiguration::createSettings(type, name);
}

CommtactLinkConfiguration *CommtactLinkManager::startConfigurationEditing(CommtactLinkConfiguration *config)
{
    if (config)
    {
        return CommtactLinkConfiguration::duplicateSettings(config);
    }
    else
    {
        qWarning() << "Internal error";
        return nullptr;
    }
}

void CommtactLinkManager::selectConfiguration(CommtactLinkConfiguration *config)
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

void CommtactLinkManager::removeConfiguration(CommtactLinkConfiguration *config)
{
    if (config)
    {
        CommtactLinkInterface *link = config->link();
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

void CommtactLinkManager::_removeConfiguration(CommtactLinkConfiguration *config)
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

bool CommtactLinkManager::containsLink(CommtactLinkInterface *link)
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

SharedCommtactLinkConfigurationPtr CommtactLinkManager::addConfiguration(CommtactLinkConfiguration *config)
{
    _qmlConfigurations.append(config);
    _rgLinkConfigs.append(SharedCommtactLinkConfigurationPtr(config));

    return _rgLinkConfigs.last();
}
