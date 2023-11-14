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

#include "EpsilonLinkConfiguration.h"
#include "EpsilonLinkInterface.h"
#include "EpsilonMAVLinkProtocol.h"
#include "QGCLoggingCategory.h"
#include "QGCToolbox.h"
#if !defined(__mobile__)
#include "LogReplayLink.h"
#include "UdpIODevice.h"
#endif
#include "QmlObjectListModel.h"

Q_DECLARE_LOGGING_CATEGORY(EpsilonLinkManagerLog)
Q_DECLARE_LOGGING_CATEGORY(EpsilonLinkManagerVerboseLog)

class QGCApplication;
class EpsilonUDPConfiguration;
class AutoConnectSettings;

/// @brief Manage communication links
///
/// The Link Manager organizes the physical Links. It can manage arbitrary
/// links and takes care of connecting them as well assigning the correct
/// protocol instance to transport the link data into the application.

class EpsilonLinkManager : public QGCTool
{
    Q_OBJECT

  public:
    EpsilonLinkManager(QGCApplication *app, QGCToolbox *toolbox);
    ~EpsilonLinkManager();

    Q_PROPERTY(QmlObjectListModel *linkConfigurations READ _qmlLinkConfigurations CONSTANT)
    Q_PROPERTY(QStringList linkTypeStrings READ linkTypeStrings CONSTANT)

    /// Create/Edit Link Configuration
    Q_INVOKABLE EpsilonLinkConfiguration *createConfiguration(int type, const QString &name);
    Q_INVOKABLE EpsilonLinkConfiguration *startConfigurationEditing(EpsilonLinkConfiguration *config);
    Q_INVOKABLE void cancelConfigurationEditing(EpsilonLinkConfiguration *config)
    {
        delete config;
    }
    Q_INVOKABLE bool endConfigurationEditing(EpsilonLinkConfiguration *config, EpsilonLinkConfiguration *editedConfig);
    Q_INVOKABLE bool endCreateConfiguration(EpsilonLinkConfiguration *config);
    Q_INVOKABLE void removeConfiguration(EpsilonLinkConfiguration *config);
    Q_INVOKABLE void selectConfiguration(EpsilonLinkConfiguration *config);

    // Called to signal app shutdown. Disconnects all links while turning off auto-connect.
    Q_INVOKABLE void shutdown(void);

    // Property accessors

    QList<SharedEpsilonLinkInterfacePtr> links(void)
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
    bool createConnectedLink(SharedEpsilonLinkConfigurationPtr &config);

    // This should only be used by Qml code
    Q_INVOKABLE void createConnectedLink(EpsilonLinkConfiguration *config);

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

    static constexpr uint8_t invalidMavlinkChannel(void)
    {
        return std::numeric_limits<uint8_t>::max();
    }

    /// Allocates a mavlink channel for use
    /// @return Mavlink channel index, invalidMavlinkChannel() for no channels available
    uint8_t allocateMavlinkChannel(void);
    void freeMavlinkChannel(uint8_t channel);

    /// If you are going to hold a reference to a LinkInterface* in your object you must reference count it
    /// by using this method to get access to the shared pointer.
    SharedEpsilonLinkInterfacePtr sharedLinkInterfacePointerForLink(EpsilonLinkInterface *link, bool ignoreNull = false);
    SharedEpsilonLinkInterfacePtr selectedSharedLinkInterfacePointerForLink(void);

    bool containsLink(EpsilonLinkInterface *link);

    SharedEpsilonLinkConfigurationPtr addConfiguration(EpsilonLinkConfiguration *config);

    static const char *settingsGroup;

  private slots:
    void _linkDisconnected(void);

  private:
    QmlObjectListModel *_qmlLinkConfigurations(void)
    {
        return &_qmlConfigurations;
    }
    bool _connectionsSuspendedMsg(void);
    void _removeConfiguration(EpsilonLinkConfiguration *config);

    int _selectedConfigurationId;

    bool _configUpdateSuspended;         ///< true: stop updating configuration list
    bool _configurationsLoaded;          ///< true: Link configurations have been loaded
    bool _connectionsSuspended;          ///< true: all new connections should not be allowed
    QString _connectionsSuspendedReason; ///< User visible reason for suspension
    QTimer _portListTimer;
    uint32_t _mavlinkChannelsUsedBitMask;

    EpsilonMAVLinkProtocol *_mavlinkProtocol;

    QList<SharedEpsilonLinkInterfacePtr> _rgLinks;
    QList<SharedEpsilonLinkConfigurationPtr> _rgLinkConfigs;
    QmlObjectListModel _qmlConfigurations;

    static const char *_defaultUDPLinkName;
};
