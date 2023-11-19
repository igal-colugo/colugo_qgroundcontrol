#ifndef EPSILON_CAMERAMANAGEMENT_H
#define EPSILON_CAMERAMANAGEMENT_H

#include <QObject>
#include <QVector>
#include <QWidget>
#include <atomic>

#include "../Epsilon/EpsilonLinkProtocol.h"
#include "Epsilon/EpsilonLinkManager.h"
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

  private:
    bool _useSeparatedLink = false;
    const uint8_t crc8_Table[256] = {0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,  65,  157, 195, 33,  127, 252, 162, 64,  30,  95,  1,   227, 189, 62,
                                     96,  130, 220, 35,  125, 159, 193, 66,  28,  254, 160, 225, 191, 93,  3,   128, 222, 60,  98,  190, 224, 2,   92,  223, 129, 99,  61,  124, 34,
                                     192, 158, 29,  67,  161, 255, 70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,  102, 229, 187, 89,  7,   219, 133, 103, 57,  186, 228, 6,
                                     88,  25,  71,  165, 251, 120, 38,  196, 154, 101, 59,  217, 135, 4,   90,  184, 230, 167, 249, 27,  69,  198, 152, 122, 36,  248, 166, 68,  26,
                                     153, 199, 37,  123, 58,  100, 134, 216, 91,  5,   231, 185, 140, 210, 48,  110, 237, 179, 81,  15,  78,  16,  242, 172, 47,  113, 147, 205, 17,
                                     79,  173, 243, 112, 46,  204, 146, 211, 141, 111, 49,  178, 236, 14,  80,  175, 241, 19,  77,  206, 144, 114, 44,  109, 51,  209, 143, 12,  82,
                                     176, 238, 50,  108, 142, 208, 83,  13,  239, 177, 240, 174, 76,  18,  145, 207, 45,  115, 202, 148, 118, 40,  171, 245, 23,  73,  8,   86,  180,
                                     234, 105, 55,  213, 139, 87,  9,   235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,  233, 183, 85,  11,  136, 214, 52,  106,
                                     43,  117, 151, 201, 74,  20,  246, 168, 116, 42,  200, 150, 21,  75,  169, 247, 182, 232, 10,  84,  215, 137, 107, 53};

  public:
    explicit EpsilonCameraManagement(QObject *parent = nullptr, MultiVehicleManager *multiVehicleManager = nullptr, JoystickManager *joystickManager = nullptr);
    EpsilonCameraManagement(QGCApplication *app, QGCToolbox *toolbox);

    void sendGimbalCommand(float cam_roll_yaw, float cam_pitch);
    void sendGimbalVirtualCommand(float cam_roll_yaw, float cam_pitch);

    JoyBtnState _camButtonFuncState[32];
    int _camButtonFuncValue[32];

    std::atomic<uint8_t> m_should_enable_rescue_btn{};

    Q_INVOKABLE void setSysModeObsCommand(void);
    Q_INVOKABLE void setSysModeGrrCommand(void);
    Q_INVOKABLE void setSysModePilotCommand(void);
    Q_INVOKABLE void setSysModeStowCommand(void);
    Q_INVOKABLE void setSysZoomStopCommand(void);
    Q_INVOKABLE void setSysZoomInCommand(void);
    Q_INVOKABLE void setSysZoomOutCommand(void);
    Q_INVOKABLE void setSysSensorDayCommand(void);
    Q_INVOKABLE void setSysSensorIrCommand(void);
    Q_INVOKABLE void setSysRecOnCommand(int chan);
    Q_INVOKABLE void setSysRecOffCommand(int chan);
    Q_INVOKABLE void setSysSnapshotCommand(int chan);
    Q_INVOKABLE void setSysFOVCommand(float fov_value);
    Q_INVOKABLE void setUseSeparatedLink(bool set);
    Q_INVOKABLE bool getUseSeparatedLink(void);

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
    void sendMavCommandLong(MAV_CMD command, float param1, float param2, float param3, float param4, float param5, float param6, float param7);
    uint16_t epsilon_link_msg_to_send_buffer(uint8_t *buf, const EpsilonLinkProtocol::epsilon_link_message_t *msg);
    uint16_t epsilon_link_msg_rate_control_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, int8_t pan_speed, int8_t tilt_speed, int8_t nudge_column, int8_t nudge_raw,
                                                int8_t optical_zoom_speed, int8_t focus_adjustment, int16_t geo_dted);
    uint8_t check_sum_calculation(uint8_t *data, int8_t length);

  public slots:

    void _activeVehicleChanged(Vehicle *activeVehicle);
    void _activeCamJoystickChanged(Joystick *activeCamJoystick);
    void manualCamControl(float cam_roll_yaw, float cam_pitch, unsigned char *buttons);
};

#endif // EPSILON_CAMERAMANAGEMENT_H
