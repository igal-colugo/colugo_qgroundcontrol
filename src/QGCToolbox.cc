/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "AudioOutput.h"
#include "FactSystem.h"
#include "FirmwarePluginManager.h"
#ifndef __mobile__
#include "GPSManager.h"
#endif
#include "ADSBVehicleManager.h"
#include "Commtact/CommtactLinkManagement.h"
#include "Commtact/CommtactLinkManager.h"
#include "Epsilon/EpsilonLinkManager.h"
#include "FollowMe.h"
#include "JoystickManager.h"
#include "LinkManager.h"
#include "MAVLinkLogManager.h"
#include "MAVLinkProtocol.h"
#include "MissionCommandTree.h"
#include "MultiVehicleManager.h"
#include "NextVision/NextVisionLinkManager.h"
#include "PositionManager.h"
#include "QGCApplication.h"
#include "QGCCorePlugin.h"
#include "QGCImageProvider.h"
#include "QGCMapEngineManager.h"
#include "QGCOptions.h"
#include "SettingsManager.h"
#include "UASMessageHandler.h"
#include "VideoManager.h"
#if defined(QGC_ENABLE_PAIRING)
#include "PairingManager.h"
#endif
#if defined(QGC_AIRMAP_ENABLED)
#include "AirMapManager.h"
#else
#include "AirspaceManager.h"
#endif
#if defined(QGC_GST_TAISYNC_ENABLED)
#include "TaisyncManager.h"
#endif
#if defined(QGC_GST_MICROHARD_ENABLED)
#include "MicrohardManager.h"
#endif

#if defined(QGC_CUSTOM_BUILD)
#include CUSTOMHEADER
#endif

QGCToolbox::QGCToolbox(QGCApplication *app)
{
    // SettingsManager must be first so settings are available to any subsequent tools
    _settingsManager = new SettingsManager(app, this);
    //-- Scan and load plugins
    _scanAndLoadPlugins(app);
    _audioOutput = new AudioOutput(app, this);
    _factSystem = new FactSystem(app, this);
    _firmwarePluginManager = new FirmwarePluginManager(app, this);
#ifndef __mobile__
    _gpsManager = new GPSManager(app, this);
#endif
    _imageProvider = new QGCImageProvider(app, this);
    _joystickManager = new JoystickManager(app, this);
    _linkManager = new LinkManager(app, this);
    _nextVisionLinkManager = new NextVisionLinkManager(app, this);
    _nextVisionMavlinkProtocol = new NextVisionMAVLinkProtocol(app, this);
    //--------------- Vlad Epsilon camera -------------------
    _epsilonLinkManager = new EpsilonLinkManager(app, this);
    _epsilonLinkProtocol = new EpsilonLinkProtocol(app, this);
    //-------------------------------------------------------
    //--------------- Vlad Commtact communication -----------
    _commtactLinkManager = new CommtactLinkManager(app, this);
    _commtactLinkProtocol = new CommtactLinkProtocol(app, this);
    _commtactLinkManagement = new CommtactLinkManagement(app, this);
    //-------------------------------------------------------
    _mavlinkProtocol = new MAVLinkProtocol(app, this);
    _missionCommandTree = new MissionCommandTree(app, this);
    _multiVehicleManager = new MultiVehicleManager(app, this);
    _mapEngineManager = new QGCMapEngineManager(app, this);
    _uasMessageHandler = new UASMessageHandler(app, this);
    _qgcPositionManager = new QGCPositionManager(app, this);
    _followMe = new FollowMe(app, this);
    _videoManager = new VideoManager(app, this);
    _mavlinkLogManager = new MAVLinkLogManager(app, this);
    _adsbVehicleManager = new ADSBVehicleManager(app, this);
#if defined(QGC_ENABLE_PAIRING)
    _pairingManager = new PairingManager(app, this);
#endif
    //-- Airmap Manager
    //-- This should be "pluggable" so an arbitrary AirSpace manager can be used
    //-- For now, we instantiate the one and only AirMap provider
#if defined(QGC_AIRMAP_ENABLED)
    _airspaceManager = new AirMapManager(app, this);
#else
    _airspaceManager = new AirspaceManager(app, this);
#endif
#if defined(QGC_GST_TAISYNC_ENABLED)
    _taisyncManager = new TaisyncManager(app, this);
#endif
#if defined(QGC_GST_MICROHARD_ENABLED)
    _microhardManager = new MicrohardManager(app, this);
#endif
}

void QGCToolbox::setChildToolboxes(void)
{
    // SettingsManager must be first so settings are available to any subsequent tools
    _settingsManager->setToolbox(this);

    _corePlugin->setToolbox(this);
    _audioOutput->setToolbox(this);
    _factSystem->setToolbox(this);
    _firmwarePluginManager->setToolbox(this);
#ifndef __mobile__
    _gpsManager->setToolbox(this);
#endif
    _imageProvider->setToolbox(this);
    _joystickManager->setToolbox(this);
    _linkManager->setToolbox(this);
    _nextVisionLinkManager->setToolbox(this);
    _epsilonLinkManager->setToolbox(this);
    _commtactLinkManager->setToolbox(this);
    _commtactLinkManagement->setToolbox(this);
    _mavlinkProtocol->setToolbox(this);
    _nextVisionMavlinkProtocol->setToolbox(this);
    _epsilonLinkProtocol->setToolbox(this);
    _commtactLinkProtocol->setToolbox(this);
    _missionCommandTree->setToolbox(this);
    _multiVehicleManager->setToolbox(this);
    _mapEngineManager->setToolbox(this);
    _uasMessageHandler->setToolbox(this);
    _followMe->setToolbox(this);
    _qgcPositionManager->setToolbox(this);
    _videoManager->setToolbox(this);
    _mavlinkLogManager->setToolbox(this);
    _airspaceManager->setToolbox(this);
    _adsbVehicleManager->setToolbox(this);
#if defined(QGC_GST_TAISYNC_ENABLED)
    _taisyncManager->setToolbox(this);
#endif
#if defined(QGC_GST_MICROHARD_ENABLED)
    _microhardManager->setToolbox(this);
#endif
#if defined(QGC_ENABLE_PAIRING)
    _pairingManager->setToolbox(this);
#endif
}

void QGCToolbox::_scanAndLoadPlugins(QGCApplication *app)
{
#if defined(QGC_CUSTOM_BUILD)
    //-- Create custom plugin (Static)
    _corePlugin = (QGCCorePlugin *) new CUSTOMCLASS(app, this);
    if (_corePlugin)
    {
        return;
    }
#endif
    //-- No plugins found, use default instance
    _corePlugin = new QGCCorePlugin(app, this);
}

QGCTool::QGCTool(QGCApplication *app, QGCToolbox *toolbox) : QObject(toolbox), _app(app), _toolbox(nullptr)
{
}

void QGCTool::setToolbox(QGCToolbox *toolbox)
{
    _toolbox = toolbox;
}
