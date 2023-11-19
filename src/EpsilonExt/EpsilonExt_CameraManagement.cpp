#include "EpsilonExt_CameraManagement.h"
#include "JoystickManager.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

// Q_GLOBAL_STATIC(TerrainTileManager, _terrainTileManager)
EpsilonCameraManagement::EpsilonCameraManagement(QObject *parent, MultiVehicleManager *multiVehicleManager, JoystickManager *joystickManager)
    : QObject(parent), _multiVehicleManager(nullptr), activeVehicle(nullptr), _joystickManager(nullptr)
{
    this->_multiVehicleManager = multiVehicleManager;
    this->_joystickManager = joystickManager;
    activeVehicle = _multiVehicleManager->activeVehicle();
    this->_epsilonLinkManager = qgcApp()->toolbox()->epsilonLinkManager();

    connect(_multiVehicleManager, &MultiVehicleManager::activeVehicleChanged, this, &EpsilonCameraManagement::_activeVehicleChanged);
    connect(this->_joystickManager, &JoystickManager::activeCamJoystickChanged, this, &EpsilonCameraManagement::_activeCamJoystickChanged);
}

void EpsilonCameraManagement::setUseSeparatedLink(bool set)
{
    _useSeparatedLink = set;
}

bool EpsilonCameraManagement::getUseSeparatedLink(void)
{
    return _useSeparatedLink;
}

void EpsilonCameraManagement::_activeVehicleChanged(Vehicle *activeVehicle)
{
    this->activeVehicle = activeVehicle;
}

void EpsilonCameraManagement::_activeCamJoystickChanged(Joystick *activeCamJoystick)
{
    if (activeCamJoystick)
    {
        /* connect to joystick manual cam control message */
        connect(activeCamJoystick, &Joystick::manualControlCam, this, &EpsilonCameraManagement::manualCamControl);
        this->_activeCamJoystick = activeCamJoystick;

        /* clear the camera button state machine vars */
        for (int i = 0; i < 32; i++)
        {
            _camButtonFuncState[i] = JoyBtnReleased;
            _camButtonFuncValue[i] = 0;
        }
    }
    else
    {
        if (this->_activeCamJoystick)
            disconnect(this->_activeCamJoystick, &Joystick::manualControlCam, this, &EpsilonCameraManagement::manualCamControl);
        this->_activeCamJoystick = activeCamJoystick;
    }
}

void EpsilonCameraManagement::manualCamControl(float cam_roll_yaw, float cam_pitch, unsigned char *buttons)
{
    static int prev_zoom_value = -1;
    QList<AssignedButtonAction *> button_actions;
    if (!activeVehicle)
    {
        return;
    }

    WeakLinkInterfacePtr weakLink = activeVehicle->vehicleLinkManager()->primaryLink();
    if (weakLink.expired())
    {
        return;
    }

    if (!_activeCamJoystick)
    {
        return;
    }

    /* read the current joystick configuration */
    button_actions = _activeCamJoystick->_buttonCamActionArray;

    /* call the button functions for each button */
    for (int buttonIndex = 0; buttonIndex < _activeCamJoystick->totalButtonCount(); buttonIndex++)
    {
        // bool button_value = (buttons & (1 << buttonIndex)) ? true :false;
        AssignedButtonAction *button_action = button_actions.at(buttonIndex);
        if (!button_action)
            continue;
        doCamAction(button_action->action, buttons[buttonIndex], buttonIndex);
    }

    /* call the button functions for each button */
    /* for (int buttonIndex=0; buttonIndex<activeJoystick->totalButtonCount(); buttonIndex++)
     {
         bool button_value = (buttons & (1 << buttonIndex)) ? true :false;
         doCamAction(_camButtonActionsMap[buttonIndex],button_value,buttonIndex);

     }*/

    /* Calculating the zoom value */
    int zoomValue = getZoomValue(buttons, button_actions);
    if (prev_zoom_value != zoomValue)
    {
        prev_zoom_value = zoomValue;

        switch (zoomValue)
        {
        case MavExtCmdArg_ZoomIn:
            setSysZoomInCommand();
            break;
        case MavExtCmdArg_ZoomOut:
            setSysZoomOutCommand();
            break;
        case MavExtCmdArg_ZoomStop:
            setSysZoomStopCommand();
            break;
        }
    }

    /* send the gimbal command to the system only when virtual joystick is disabled */
    if (qgcApp()->toolbox()->settingsManager()->appSettings()->virtualJoystick()->rawValue().toBool() == false)
    {
        sendGimbalCommand((cam_roll_yaw / (-32768)) * 100.0, (cam_pitch / (-32768)) * 100.0);
    }
}

bool EpsilonCameraManagement::doBtnFuncToggle(bool pressed, int buttonIndex)
{
    switch (_camButtonFuncState[buttonIndex])
    {
    case JoyBtnReleased: {
        if (pressed)
        {
            _camButtonFuncState[buttonIndex] = JoyBtnPressed;
        }
    }
    break;
    case JoyBtnPressed: {
        if (!pressed)
        {
            _camButtonFuncValue[buttonIndex] ^= 1;
            _camButtonFuncState[buttonIndex] = JoyBtnReleased;
            return true;
        }
    }
    break;
    }
    return false;
}

void EpsilonCameraManagement::doCamAction(QString buttonAction, bool pressed, int buttonIndex)
{
    bool doAction = doBtnFuncToggle(pressed, buttonIndex);

    if (buttonAction.isEmpty())
        return;

    if (!activeVehicle)
        return;

    if (!activeVehicle->joystickCamEnabled())
    {
        return;
    }

    if (buttonAction == "Day / IR")
    {
        /* Day/IR toggle */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "White Hot / Black Hot")
    {
        /* Polarity Toggle */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Image Capture")
    {
        /* Image Capture */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Single Yaw")
    {
        /* Single Yaw */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "GRR")
    {
        /* GRR */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "NUC")
    {
        /* NUC */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Stow")
    {
        /* Stow */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Pilot")
    {
        /* Pilot */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Retract")
    {
        /* Retract */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Hold Coordinate")
    {
        /* Hold Coordinate */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Observation")
    {
        /* Observation */
        if (doAction)
        {
        }
    }
    else if (buttonAction == "Record")
    {
        /* Record */
        if (doAction)
        {
        }
    }
}

/* Returning the zoom value according to the buttons pressed */
EpsilonCameraManagement::MavlinkExtSetGimbalArgs EpsilonCameraManagement::getZoomValue(unsigned char *buttons, QList<AssignedButtonAction *> button_actions)
{
    if (!_activeCamJoystick)
        return MavExtCmdArg_ZoomStop;

    int zoomInVal = 0;
    int zoomOutVal = 0;

    /* call the button functions for each button */
    for (int buttonIndex = 0; buttonIndex < _activeCamJoystick->totalButtonCount(); buttonIndex++)
    {
        // bool button_value = (buttons & (1 << buttonIndex)) ? true :false;
        AssignedButtonAction *button_action = button_actions.at(buttonIndex);
        if (!button_action)
            continue;
        if ((button_action->action == "Zoom In") && (buttons[buttonIndex] != 0))
            zoomInVal = 1;
        else if ((button_action->action == "Zoom Out") && (buttons[buttonIndex] != 0))
            zoomOutVal = 1;
    }

    if ((zoomInVal == 0 && zoomOutVal == 0) || (zoomInVal == 1 && zoomOutVal == 1))
        return MavExtCmdArg_ZoomStop;
    else if (zoomInVal == 1)
        return MavExtCmdArg_ZoomIn;
    else
        return MavExtCmdArg_ZoomOut;
}

/* Sending gimbal Command Messages */
void EpsilonCameraManagement::sendGimbalCommand(float cam_roll_yaw, float cam_pitch)
{
    if (_useSeparatedLink == true)
    {
        WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
        if (weakLink.expired())
        {
            return;
        }
        SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

        if (sharedLink != nullptr)
        {
            EpsilonLinkProtocol::epsilon_link_message_t message = {};
            uint8_t buffer[255] = {};
            epsilon_link_msg_rate_control_pack(&message, cam_roll_yaw, cam_pitch, 0, 0, 0, 0, 0);
            int len = epsilon_link_msg_to_send_buffer(&buffer[0], &message);

            sharedLink->writeBytesThreadSafe((const char *) buffer, len);
        }
    }
}

uint16_t EpsilonCameraManagement::epsilon_link_msg_to_send_buffer(uint8_t *buf, const EpsilonLinkProtocol::epsilon_link_message_t *msg)
{
    buf[0] = msg->magic[0];
    buf[1] = msg->magic[1];
    buf[2] = msg->device_id;
    buf[3] = msg->message_id;
    buf[4] = msg->length;
    buf[5] = msg->header_checksum;
    memcpy(&buf[6], _EPSILON_PAYLOAD(msg), msg->length);
    buf[6 + msg->length] = msg->checksum;
    buf[6 + msg->length + 1] = msg->terminator;

    return 6 + msg->length + 2;
}

uint16_t EpsilonCameraManagement::epsilon_link_msg_rate_control_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, int8_t pan_speed, int8_t tilt_speed, int8_t nudge_column,
                                                                     int8_t nudge_raw, int8_t optical_zoom_speed, int8_t focus_adjustment, int16_t geo_dted)
{

    EpsilonLinkProtocol::epsilon_rate_control_message_t packet = {};

    msg->magic[0] = 0xAA;
    msg->magic[1] = 0x55;
    msg->device_id = 0;
    msg->message_id = 0x05;
    msg->length = 8;
    msg->terminator = 0xFF;

    uint8_t data[3] = {};
    data[0] = msg->device_id;
    data[1] = msg->message_id;
    data[2] = msg->length;

    msg->header_checksum = check_sum_calculation(&data[0], 3);

    packet.pan_speed = pan_speed;
    packet.tilt_speed = tilt_speed;
    packet.nudge_column = nudge_column;
    packet.nudge_raw = nudge_raw;
    packet.optical_zoom_speed = optical_zoom_speed;
    packet.focus_adjustment = focus_adjustment;
    packet.geo_dted = geo_dted;

    uint8_t payload[msg->length] = {};

    memcpy(&payload, &packet, sizeof(packet));

    msg->checksum = check_sum_calculation(&payload[0], msg->length);

    memcpy(_EPSILON_PAYLOAD_NON_CONST(msg), &packet, sizeof(packet));

    return 0;
}

uint8_t EpsilonCameraManagement::check_sum_calculation(uint8_t *data, int8_t length)
{
    uint8_t return_value = 1;

    for (int i = 0; i < length; i++)
    {
        return_value = crc8_Table[return_value ^ (*(data + i))];
    }

    return return_value;
}

void EpsilonCameraManagement::setSysModeObsCommand()
{
    /* Sending the OBS command */
}

void EpsilonCameraManagement::setSysModeGrrCommand()
{
    /* Sending the GRR command */
}

void EpsilonCameraManagement::setSysModePilotCommand()
{
    /* Sending the Pilot command */
}

void EpsilonCameraManagement::setSysModeStowCommand()
{
    /* Sending the Stow command */
}

void EpsilonCameraManagement::setSysZoomStopCommand()
{
    /* Sending retract release command */
}

void EpsilonCameraManagement::setSysZoomInCommand()
{
    /* Sending retract release command */
}

void EpsilonCameraManagement::setSysZoomOutCommand()
{
    /* Sending retract release command */
}

void EpsilonCameraManagement::setSysSensorDayCommand(void)
{
    /* Set the system sensor */
}

void EpsilonCameraManagement::setSysSensorIrCommand(void)
{
    /* Set the system sensor */
}

void EpsilonCameraManagement::setSysRecOnCommand(int chan)
{
    /* Set the system sensor */
}

void EpsilonCameraManagement::setSysRecOffCommand(int chan)
{
    /* Set the system sensor */
}

void EpsilonCameraManagement::setSysSnapshotCommand(int chan)
{
    /* Set the system sensor */
}

void EpsilonCameraManagement::setSysFOVCommand(float fov_value)
{
    /* Set the system sensor */
}

bool EpsilonCameraManagement::getShouldUiEnabledRescueElement()
{
    /* Set Focus State */
    return m_should_enable_rescue_btn == 1;
}
