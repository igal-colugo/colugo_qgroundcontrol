/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QList>
#include <QMultiMap>
#include <QMutex>

#include <limits>

#include "CommtactLinkConfiguration.h"
#include "CommtactLinkInterface.h"
#include "CommtactLinkProtocol.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCToolbox.h"
#include "Settings/SettingsManager.h"
#if !defined(__mobile__)
#include "LogReplayLink.h"
#include "UdpIODevice.h"
#endif
#include "QmlControls/QmlObjectListModel.h"

Q_DECLARE_LOGGING_CATEGORY(CommtactLinkManagerLog)
Q_DECLARE_LOGGING_CATEGORY(CommtactLinkManagerVerboseLog)

class QGCApplication;
class CommtactUDPConfiguration;
class AutoConnectSettings;

/// @brief Manage communication links
///
/// The Link Manager organizes the physical Links. It can manage arbitrary
/// links and takes care of connecting them as well assigning the correct
/// protocol instance to transport the link data into the application.
class CommtactLinkManager : public QGCTool
{
    Q_OBJECT

  public:
    CommtactLinkManager(QGCApplication *app, QGCToolbox *toolbox);
    ~CommtactLinkManager();

    Q_PROPERTY(CommtactLinkProtocol *_linkProtocol READ linkProtocol CONSTANT)
    Q_PROPERTY(QmlObjectListModel *linkConfigurations READ _qmlLinkConfigurations CONSTANT)
    Q_PROPERTY(QStringList linkTypeStrings READ linkTypeStrings CONSTANT)

    /// Create/Edit Link Configuration
    Q_INVOKABLE CommtactLinkConfiguration *createConfiguration(int type, const QString &name);
    Q_INVOKABLE CommtactLinkConfiguration *startConfigurationEditing(CommtactLinkConfiguration *config);
    Q_INVOKABLE void cancelConfigurationEditing(CommtactLinkConfiguration *config)
    {
        delete config;
    }
    Q_INVOKABLE bool endConfigurationEditing(CommtactLinkConfiguration *config, CommtactLinkConfiguration *editedConfig);
    Q_INVOKABLE bool endCreateConfiguration(CommtactLinkConfiguration *config);
    Q_INVOKABLE void removeConfiguration(CommtactLinkConfiguration *config);
    Q_INVOKABLE void selectConfiguration(CommtactLinkConfiguration *config);

    // Called to signal app shutdown. Disconnects all links while turning off auto-connect.
    Q_INVOKABLE void shutdown(void);

    // Property accessors

    CommtactLinkProtocol *linkProtocol(void)
    {
        return _linkProtocol;
    }

    QList<SharedCommtactLinkInterfacePtr> links(void)
    {
        return _rgLinks;
    }
    QStringList linkTypeStrings(void) const;

    void loadLinkConfigurationList();
    void saveLinkConfigurationList();

    /// Suspend automatic confguration updates (during link maintenance for instance)
    void suspendConfigurationUpdates(bool suspend);

    /// Sets the flag to suspend the all new connections
    ///     @param reason User visible reason to suspend connections
    void setConnectionsSuspended(QString reason);

    /// Sets the flag to allow new connections to be made
    void setConnectionsAllowed(void)
    {
        _connectionsSuspended = false;
    }

    /// Creates, connects (and adds) a link  based on the given configuration instance.
    bool createConnectedLink(SharedCommtactLinkConfigurationPtr &config);

    // This should only be used by Qml code
    Q_INVOKABLE void createConnectedLink(CommtactLinkConfiguration *config);

    void disconnectAll(void);

#ifdef QT_DEBUG
    // Only used by unit test tp restart after a shutdown
    void restart(void)
    {
        setConnectionsAllowed();
    }
#endif

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    /// If you are going to hold a reference to a LinkInterface* in your object you must reference count it
    /// by using this method to get access to the shared pointer.
    SharedCommtactLinkInterfacePtr sharedLinkInterfacePointerForLink(CommtactLinkInterface *link, bool ignoreNull = false);
    SharedCommtactLinkInterfacePtr selectedSharedLinkInterfacePointerForLink(void);

    bool containsLink(CommtactLinkInterface *link);

    SharedCommtactLinkConfigurationPtr addConfiguration(CommtactLinkConfiguration *config);

    static const char *settingsGroup;

  private slots:
    void _linkDisconnected(void);

  private:
    QmlObjectListModel *_qmlLinkConfigurations(void)
    {
        return &_qmlConfigurations;
    }
    bool _connectionsSuspendedMsg(void);
    void _removeConfiguration(CommtactLinkConfiguration *config);

    int _selectedConfigurationId;

    bool _configUpdateSuspended;         ///< true: stop updating configuration list
    bool _configurationsLoaded;          ///< true: Link configurations have been loaded
    bool _connectionsSuspended;          ///< true: all new connections should not be allowed
    QString _connectionsSuspendedReason; ///< User visible reason for suspension
    QTimer _portListTimer;

    CommtactLinkProtocol *_linkProtocol;

    QList<SharedCommtactLinkInterfacePtr> _rgLinks;
    QList<SharedCommtactLinkConfigurationPtr> _rgLinkConfigs;
    QmlObjectListModel _qmlConfigurations;

    static const char *_defaultUDPLinkName;
};
