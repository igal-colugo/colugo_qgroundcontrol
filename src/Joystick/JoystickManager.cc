/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "JoystickManager.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

#include <QQmlEngine>

#ifndef __mobile__
#include "JoystickSDL.h"
#define __sdljoystick__
#endif

#ifdef __android__
#include "JoystickAndroid.h"
#endif

QGC_LOGGING_CATEGORY(JoystickManagerLog, "JoystickManagerLog")

const char *JoystickManager::_settingsGroup = "JoystickManager";
const char *JoystickManager::_settingsKeyActiveJoystick = "ActiveJoystick";
const char *JoystickManager::_settingsKeyActiveCamJoystick = "ActiveCamJoystick"; /* NextVision */

JoystickManager::JoystickManager(QGCApplication *app, QGCToolbox *toolbox)
    : QGCTool(app, toolbox), _activeJoystick(nullptr), _multiVehicleManager(nullptr), _activeCamJoystick(nullptr) /* NextVision */
      ,
      _cameraManagement(nullptr) /* NextVision */, _epsilonCameraManagement(nullptr) /* Epsilon */
{
}

JoystickManager::~JoystickManager()
{
    QMap<QString, Joystick *>::iterator i;
    for (i = _name2JoystickMap.begin(); i != _name2JoystickMap.end(); ++i)
    {
        qCDebug(JoystickManagerLog) << "Releasing joystick:" << i.key();
        i.value()->stop();
        delete i.value();
    }
    qDebug() << "Done";
}

void JoystickManager::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);

    _multiVehicleManager = _toolbox->multiVehicleManager();

    int8_t nextVisionEnabled = qgcApp()->toolbox()->settingsManager()->appSettings()->enableNextVision()->rawValue().toInt();
    int8_t epsilonEnabled = qgcApp()->toolbox()->settingsManager()->appSettings()->enableEpsilon()->rawValue().toInt();

    if (nextVisionEnabled > 0)
    {
        _cameraManagement = new CameraManagement(nullptr, _multiVehicleManager, this);
    }
    if (epsilonEnabled > 0)
    {
        _epsilonCameraManagement = new EpsilonCameraManagement(nullptr, _multiVehicleManager, this);
    }

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

CameraManagement *JoystickManager::cameraManagement()
{
    return _cameraManagement;
}

void JoystickManager::setCameraManagement(CameraManagement *camManagement)
{
    _cameraManagement = camManagement;
    emit activeCameraManagementChanged();
}

EpsilonCameraManagement *JoystickManager::epsilonCameraManagement()
{
    return _epsilonCameraManagement;
}

void JoystickManager::setEpsilonCameraManagement(EpsilonCameraManagement *epsilonCamManagement)
{
    _epsilonCameraManagement = epsilonCamManagement;
    emit activeCameraManagementChanged();
}

void JoystickManager::init()
{
#ifdef __sdljoystick__
    if (!JoystickSDL::init())
    {
        return;
    }
    _setActiveJoystickFromSettings();
#elif defined(__android__)
    if (!JoystickAndroid::init(this))
    {
        return;
    }
    connect(this, &JoystickManager::updateAvailableJoysticksSignal, this, &JoystickManager::restartJoystickCheckTimer);
#endif
    connect(&_joystickCheckTimer, &QTimer::timeout, this, &JoystickManager::_updateAvailableJoysticks);
    _joystickCheckTimerCounter = 5;
    _joystickCheckTimer.start(1000);
}

void JoystickManager::_setActiveJoystickFromSettings(void)
{
    QMap<QString, Joystick *> newMap;

#ifdef __sdljoystick__
    // Get the latest joystick mapping
    newMap = JoystickSDL::discover(_multiVehicleManager);
#elif defined(__android__)
    newMap = JoystickAndroid::discover(_multiVehicleManager);
#endif

    if (_activeJoystick && !newMap.contains(_activeJoystick->name()))
    {
        qCDebug(JoystickManagerLog) << "Active joystick removed";
        setActiveJoystick(nullptr);
        setActiveCamJoystick(nullptr); /* NextVision */
    }

    // Check to see if our current mapping contains any joysticks that are not in the new mapping
    // If so, those joysticks have been unplugged, and need to be cleaned up
    QMap<QString, Joystick *>::iterator i;
    for (i = _name2JoystickMap.begin(); i != _name2JoystickMap.end(); ++i)
    {
        if (!newMap.contains(i.key()))
        {
            qCDebug(JoystickManagerLog) << "Releasing joystick:" << i.key();
            i.value()->stopPolling();
            i.value()->wait(1000);
            i.value()->deleteLater();
        }
    }

    _name2JoystickMap = newMap;
    emit availableJoysticksChanged();

    if (!_name2JoystickMap.count())
    {
        setActiveJoystick(nullptr);
        setActiveCamJoystick(nullptr); /* NextVision */
        return;
    }

    /* NextVision */
    /* ------------------------------------------------------------------------------------------------------*/
    QSettings settings;

    settings.beginGroup(_settingsGroup);
    QString joy_name = settings.value(_settingsKeyActiveJoystick).toString();
    if (joy_name.isEmpty())
    {
        joy_name = _name2JoystickMap.first()->name();
    }
    QString cam_joy_name = settings.value(_settingsKeyActiveCamJoystick).toString();
    if (cam_joy_name.isEmpty())
    {
        cam_joy_name = _name2JoystickMap.first()->name();
    }

    setActiveJoystick(_name2JoystickMap.value(joy_name, _name2JoystickMap.first()));
    if (_activeJoystick)
    {
        settings.setValue(_settingsKeyActiveJoystick, _activeJoystick->name());
    }

    setActiveCamJoystick(_name2JoystickMap.value(cam_joy_name, _name2JoystickMap.first()));
    if (_activeCamJoystick)
    {
        settings.setValue(_settingsKeyActiveCamJoystick, _activeCamJoystick->name());
    }

    /* ------------------------------------------------------------------------------------------------------*/
}

Joystick *JoystickManager::activeJoystick(void)
{
    return _activeJoystick;
}

void JoystickManager::setActiveJoystick(Joystick *joystick)
{
    QSettings settings;

    if (joystick != nullptr && !_name2JoystickMap.contains(joystick->name()))
    {
        qCWarning(JoystickManagerLog) << "Set active not in map" << joystick->name();
        return;
    }
    if (_activeJoystick == joystick)
    {
        return;
    }
    // if desired joystick eq to camera joystick destroy it
    if (_activeCamJoystick == joystick)
    {
        if (_activeCamJoystick != nullptr)
        {
            _activeCamJoystick->stopPolling();
            _activeCamJoystick = nullptr;
        }
    }
    if (_activeJoystick)
    {
        _activeJoystick->stopPolling();
        _activeJoystick = nullptr;
    }

    _activeJoystick = joystick;
    if (_activeJoystick)
    {
        _activeJoystick->isCameraJoystick = false; /* NextVision */

        qCDebug(JoystickManagerLog) << "Set active:" << _activeJoystick->name();

        settings.beginGroup(_settingsGroup);
        settings.setValue(_settingsKeyActiveJoystick, _activeJoystick->name());
    }

    // notify out
    emit activeJoystickChanged(_activeJoystick);
    emit activeJoystickNameChanged(_activeJoystick ? _activeJoystick->name() : "");
}

QVariantList JoystickManager::joysticks(void)
{
    QVariantList list;

    for (const QString &name : _name2JoystickMap.keys())
    {
        list += QVariant::fromValue(_name2JoystickMap[name]);
    }

    return list;
}

QStringList JoystickManager::joystickNames(void)
{
    return _name2JoystickMap.keys();
}

QString JoystickManager::activeJoystickName(void)
{
    return _activeJoystick ? _activeJoystick->name() : QString();
}

void JoystickManager::setActiveJoystickName(const QString &name)
{
    if (!_name2JoystickMap.contains(name))
    {
        qCWarning(JoystickManagerLog) << "Set active not in map" << name;
        return;
    }

    setActiveJoystick(_name2JoystickMap[name]);
}

/* NextVision Added code for Camera Joystick*/
/* ------------------------------------------------------------------------------------------------------*/
QString JoystickManager::activeCamJoystickName(void)
{
    return _activeCamJoystick ? _activeCamJoystick->name() : QString();
}

void JoystickManager::setActiveCamJoystickName(const QString &name)
{
    if (!_name2JoystickMap.contains(name))
    {
        qCWarning(JoystickManagerLog) << "Set cam active not in map" << name;
        return;
    }

    setActiveCamJoystick(_name2JoystickMap[name]);
}

Joystick *JoystickManager::activeCamJoystick(void)
{
    return _activeCamJoystick;
}

void JoystickManager::setActiveCamJoystick(Joystick *joystick)
{
    QSettings settings;

    if (joystick != nullptr && !_name2JoystickMap.contains(joystick->name()))
    {
        qCWarning(JoystickManagerLog) << "Set cam active not in map" << joystick->name();
        return;
    }
    // avoid set joystick to manual system joystick

    if (_activeJoystick != nullptr)
    {
        if (_activeJoystick == joystick)
        {
            if (_activeJoystick->enabled() == true)
            {
                qgcApp()->showAppMessage(QStringLiteral("Joystick occupied by manual control.Disable it before."));
                emit activeCamJoystickNameChanged(_activeCamJoystick ? _activeCamJoystick->name() : "");
                return;
            }
            else
            {
                _activeJoystick->stopPolling();
                _activeJoystick = nullptr;
            }
        }
    }
    if (_activeCamJoystick == joystick)
    {
        return;
    }
    if (_activeCamJoystick)
    {
        _activeCamJoystick->stopPolling();
    }

    _activeCamJoystick = joystick;
    if (_activeCamJoystick)
    {
        _activeCamJoystick->isCameraJoystick = true;

        qCDebug(JoystickManagerLog) << "Set cam active:" << _activeCamJoystick->name();

        settings.beginGroup(_settingsGroup);
        settings.setValue(_settingsKeyActiveCamJoystick, _activeCamJoystick->name());
    }

    // notify out
    emit activeCamJoystickChanged(_activeCamJoystick);
    emit activeCamJoystickNameChanged(_activeCamJoystick ? _activeCamJoystick->name() : "");
}
/* ------------------------------------------------------------------------------------------------------*/

/*
 * TODO: move this to the right place: JoystickSDL.cc and JoystickAndroid.cc respectively and call through Joystick.cc
 */
void JoystickManager::_updateAvailableJoysticks()
{
#ifdef __sdljoystick__
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            qCDebug(JoystickManagerLog) << "SDL ERROR:" << SDL_GetError();
            break;
        case SDL_JOYDEVICEADDED:
            qCDebug(JoystickManagerLog) << "Joystick added:" << event.jdevice.which;
            _setActiveJoystickFromSettings();
            break;
        case SDL_JOYDEVICEREMOVED:
            qCDebug(JoystickManagerLog) << "Joystick removed:" << event.jdevice.which;
            _setActiveJoystickFromSettings();
            break;
        default:
            break;
        }
    }
#elif defined(__android__)
    _joystickCheckTimerCounter--;
    _setActiveJoystickFromSettings();
    if (_joystickCheckTimerCounter <= 0)
    {
        _joystickCheckTimer.stop();
    }
#endif
}

void JoystickManager::restartJoystickCheckTimer()
{
    _joystickCheckTimerCounter = 5;
    _joystickCheckTimer.start(1000);
}
