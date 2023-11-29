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

#include "EpsilonLinkManager.h"
#include "EpsilonTCPLink.h"
#include "EpsilonUDPLink.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/mdns.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

QGC_LOGGING_CATEGORY(EpsilonLinkManagerLog, "EpsilonLinkManagerLog")
QGC_LOGGING_CATEGORY(EpsilonLinkManagerVerboseLog, "EpsilonLinkManagerVerboseLog")

const char *EpsilonLinkManager::_defaultUDPLinkName = "UDP Link (AutoConnect)";

EpsilonLinkManager::EpsilonLinkManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), _selectedConfigurationId(-1), _configUpdateSuspended(false), _configurationsLoaded(false), _connectionsSuspended(false), _linkProtocol(nullptr)
{
    qmlRegisterUncreatableType<EpsilonLinkManager>("QGroundControl", 1, 0, "EpsilonLinkManager", "Reference only");
    qmlRegisterUncreatableType<EpsilonLinkConfiguration>("QGroundControl", 1, 0, "EpsilonLinkConfiguration", "Reference only");
    qmlRegisterUncreatableType<EpsilonLinkInterface>("QGroundControl", 1, 0, "EpsilonLinkInterface", "Reference only");
}

EpsilonLinkManager::~EpsilonLinkManager()
{
}

void EpsilonLinkManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    _linkProtocol = _toolbox->epsilonLinkProtocol();
}

// This should only be used by Qml code
void EpsilonLinkManager::createConnectedLink(EpsilonLinkConfiguration *config)
{
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedEpsilonLinkConfigurationPtr &sharedConfig = _rgLinkConfigs[i];
        if (sharedConfig.get() == config)
            createConnectedLink(sharedConfig);
    }
}

bool EpsilonLinkManager::createConnectedLink(SharedEpsilonLinkConfigurationPtr &config)
{
    SharedEpsilonLinkInterfacePtr link = nullptr;

    switch (config->type())
    {
    case EpsilonLinkConfiguration::TypeUdp:
        link = std::make_shared<EpsilonUDPLink>(config);
        break;
    case EpsilonLinkConfiguration::TypeTcp:
        link = std::make_shared<EpsilonTCPLink>(config);
        break;
    case EpsilonLinkConfiguration::TypeLast:
        break;
    }

    if (link)
    {
        _rgLinks.append(link);
        _selectedConfigurationId = _rgLinks.count() - 1;
        config->setLink(link);

        connect(link.get(), &EpsilonLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
        connect(link.get(), &EpsilonLinkInterface::bytesReceived, _linkProtocol, &EpsilonLinkProtocol::receiveBytes);
        connect(link.get(), &EpsilonLinkInterface::bytesSent, _linkProtocol, &EpsilonLinkProtocol::logSentBytes);
        connect(link.get(), &EpsilonLinkInterface::disconnected, this, &EpsilonLinkManager::_linkDisconnected);

        if (!link->_connect())
        {
            return false;
        }

        return true;
    }

    return false;
}

void EpsilonLinkManager::disconnectAll(void)
{
    QList<SharedEpsilonLinkInterfacePtr> links = _rgLinks;

    for (const SharedEpsilonLinkInterfacePtr &sharedLink : links)
    {
        sharedLink->disconnect();
    }
}

void EpsilonLinkManager::_linkDisconnected(void)
{
    EpsilonLinkInterface *link = qobject_cast<EpsilonLinkInterface *>(sender());

    if (!link || !containsLink(link))
    {
        return;
    }

    disconnect(link, &EpsilonLinkInterface::communicationError, _app, &QGCApplication::criticalMessageBoxOnMainThread);
    disconnect(link, &EpsilonLinkInterface::disconnected, this, &EpsilonLinkManager::_linkDisconnected);

    for (int i = 0; i < _rgLinks.count(); i++)
    {
        if (_rgLinks[i].get() == link)
        {
            qCDebug(EpsilonLinkManagerLog) << "LinkManager::_linkDisconnected" << _rgLinks[i]->linkConfiguration()->name() << _rgLinks[i].use_count();
            _rgLinks.removeAt(i);
            return;
        }
    }
}

SharedEpsilonLinkInterfacePtr EpsilonLinkManager::sharedLinkInterfacePointerForLink(EpsilonLinkInterface *link, bool ignoreNull)
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
    return SharedEpsilonLinkInterfacePtr(nullptr);
}

SharedEpsilonLinkInterfacePtr EpsilonLinkManager::selectedSharedLinkInterfacePointerForLink(void)
{
    SharedEpsilonLinkInterfacePtr returnValue = nullptr;

    if (_rgLinks.count() > _selectedConfigurationId && _selectedConfigurationId > -1)
    {
        returnValue = _rgLinks[_selectedConfigurationId];
    }

    return returnValue;
}

/// @brief If all new connections should be suspended a message is displayed to the user and true
///         is returned;
bool EpsilonLinkManager::_connectionsSuspendedMsg(void)
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

void EpsilonLinkManager::setConnectionsSuspended(QString reason)
{
    _connectionsSuspended = true;
    _connectionsSuspendedReason = reason;
}

void EpsilonLinkManager::suspendConfigurationUpdates(bool suspend)
{
    _configUpdateSuspended = suspend;
}

void EpsilonLinkManager::saveLinkConfigurationList()
{
    QSettings settings;
    settings.remove(EpsilonLinkConfiguration::settingsRoot());
    int trueCount = 0;
    for (int i = 0; i < _rgLinkConfigs.count(); i++)
    {
        SharedEpsilonLinkConfigurationPtr linkConfig = _rgLinkConfigs[i];
        if (linkConfig)
        {
            QString root = EpsilonLinkConfiguration::settingsRoot();
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
    QString root(EpsilonLinkConfiguration::settingsRoot());
    settings.setValue(root + "/count", trueCount);
}

void EpsilonLinkManager::loadLinkConfigurationList()
{
    QSettings settings;
    // Is the group even there?
    if (settings.contains(EpsilonLinkConfiguration::settingsRoot() + "/count"))
    {
        // Find out how many configurations we have
        int count = settings.value(EpsilonLinkConfiguration::settingsRoot() + "/count").toInt();
        for (int i = 0; i < count; i++)
        {
            QString root(EpsilonLinkConfiguration::settingsRoot());
            root += QString("/Link%1").arg(i);
            if (settings.contains(root + "/type"))
            {
                EpsilonLinkConfiguration::LinkType type = static_cast<EpsilonLinkConfiguration::LinkType>(settings.value(root + "/type").toInt());
                if (type < EpsilonLinkConfiguration::TypeLast)
                {
                    if (settings.contains(root + "/name"))
                    {
                        QString name = settings.value(root + "/name").toString();
                        if (!name.isEmpty())
                        {
                            EpsilonLinkConfiguration *link = nullptr;

                            switch (type)
                            {
#
                            case EpsilonLinkConfiguration::TypeUdp:
                                link = new EpsilonUDPConfiguration(name);
                                break;
                            case EpsilonLinkConfiguration::TypeTcp:
                                link = new EpsilonTCPConfiguration(name);
                                break;
                            case EpsilonLinkConfiguration::TypeLast:
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

void EpsilonLinkManager::shutdown(void)
{
    setConnectionsSuspended(tr("Shutdown"));
    disconnectAll();
}

QStringList EpsilonLinkManager::linkTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("UDP");
        list += tr("TCP");
        if (list.size() != static_cast<int>(EpsilonLinkConfiguration::TypeLast))
        {
            qWarning() << "Internal error";
        }
    }
    return list;
}

bool EpsilonLinkManager::endConfigurationEditing(EpsilonLinkConfiguration *config, EpsilonLinkConfiguration *editedConfig)
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

bool EpsilonLinkManager::endCreateConfiguration(EpsilonLinkConfiguration *config)
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

EpsilonLinkConfiguration *EpsilonLinkManager::createConfiguration(int type, const QString &name)
{
    return EpsilonLinkConfiguration::createSettings(type, name);
}

EpsilonLinkConfiguration *EpsilonLinkManager::startConfigurationEditing(EpsilonLinkConfiguration *config)
{
    if (config)
    {
        return EpsilonLinkConfiguration::duplicateSettings(config);
    }
    else
    {
        qWarning() << "Internal error";
        return nullptr;
    }
}

void EpsilonLinkManager::selectConfiguration(EpsilonLinkConfiguration *config)
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

void EpsilonLinkManager::removeConfiguration(EpsilonLinkConfiguration *config)
{
    if (config)
    {
        EpsilonLinkInterface *link = config->link();
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

void EpsilonLinkManager::_removeConfiguration(EpsilonLinkConfiguration *config)
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

bool EpsilonLinkManager::containsLink(EpsilonLinkInterface *link)
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

SharedEpsilonLinkConfigurationPtr EpsilonLinkManager::addConfiguration(EpsilonLinkConfiguration *config)
{
    _qmlConfigurations.append(config);
    _rgLinkConfigs.append(SharedEpsilonLinkConfigurationPtr(config));

    return _rgLinkConfigs.last();
}
