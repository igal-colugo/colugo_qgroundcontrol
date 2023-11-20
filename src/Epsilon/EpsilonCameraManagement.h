#ifndef EPSILON_CAMERAMANAGEMENT_H
#define EPSILON_CAMERAMANAGEMENT_H

#include <QObject>
#include <QVector>
#include <QWidget>
#include <atomic>

#include "EpsilonLinkManager.h"
#include "EpsilonLinkProtocol.h"
#include "Joystick.h"
#include "QGCMapEngine.h"
#include "QGCMapEngineManager.h"
#include "QGeoMapReplyQGC.h"
#include "QGeoTileFetcherQGC.h"

class EpsilonCameraManagement : public QObject
{
    Q_OBJECT

    /* enums */
    typedef enum
    {
        JoyBtnReleased,
        JoyBtnPressed
    } JoyBtnState;

    /* Mavlink Extension Arguments for Set Gimbal Command */
    typedef enum
    {
        MavExtCmdArg_ZoomStop = 0,
        MavExtCmdArg_ZoomIn,
        MavExtCmdArg_ZoomOut,
        MavExtCmdArg_ZoomNoChange
    } MavlinkExtSetGimbalArgs;

  public:
    explicit EpsilonCameraManagement(QObject *parent = nullptr, MultiVehicleManager *multiVehicleManager = nullptr, JoystickManager *joystickManager = nullptr);
    EpsilonCameraManagement(QGCApplication *app, QGCToolbox *toolbox);

    void sendGimbalCommand(float cam_roll_yaw, float cam_pitch);
    void sendGimbalVirtualCommand(float cam_roll_yaw, float cam_pitch);

    JoyBtnState _camButtonFuncState[32];
    int _camButtonFuncValue[32];

    std::atomic<uint8_t> m_should_enable_rescue_btn{};

    Q_INVOKABLE void setCameraModeCommand(uint control_mode);
    Q_INVOKABLE void setCameraOrderCommand(uint order);

    Q_INVOKABLE void setSysZoomStopCommand(void);
    Q_INVOKABLE void setSysZoomInCommand(void);
    Q_INVOKABLE void setSysZoomOutCommand(void);

    Q_INVOKABLE void setSysSensorDayCommand(void);
    Q_INVOKABLE void setSysSensorIrCommand(void);
    Q_INVOKABLE void setSysRecOnCommand(int chan);
    Q_INVOKABLE void setSysRecOffCommand(int chan);
    Q_INVOKABLE void setSysSnapshotCommand(int chan);
    Q_INVOKABLE void setSysFOVCommand(float fov_value);
    Q_INVOKABLE void setCameraScreenInformationCommand(uint switch_flags);

    // UI state management functions (called periodically, their result is meant to update Qml proprties dynamically)
    Q_INVOKABLE bool getShouldUiEnabledRescueElement();

  protected:
    MultiVehicleManager *_multiVehicleManager;
    Vehicle *activeVehicle;
    JoystickManager *_joystickManager;
    Joystick *_activeCamJoystick;
    EpsilonLinkManager *_epsilonLinkManager;

  private:
    void doCamAction(QString buttonAction, bool pressed, int buttonIndex);
    bool doBtnFuncToggle(bool pressed, int buttonIndex);
    MavlinkExtSetGimbalArgs getZoomValue(unsigned char *buttons, QList<AssignedButtonAction *> button_actions);

  public slots:

    void _activeVehicleChanged(Vehicle *activeVehicle);
    void _activeCamJoystickChanged(Joystick *activeCamJoystick);
    void manualCamControl(float cam_roll_yaw, float cam_pitch, unsigned char *buttons);
};

#endif // EPSILON_CAMERAMANAGEMENT_H
