#include "EpsilonCameraManagement.h"
#include "JoystickManager.h"
#include "QGCApplication.h"
#include "SettingsManager.h"

EpsilonCameraManagement::EpsilonCameraManagement(QObject *parent, MultiVehicleManager *multiVehicleManager, JoystickManager *joystickManager)
    : QObject(parent), _multiVehicleManager(nullptr), activeVehicle(nullptr), _joystickManager(nullptr), _opticalZoomSpeed(0)
{
    this->_multiVehicleManager = multiVehicleManager;
    this->activeVehicle = _multiVehicleManager->activeVehicle();
    this->_joystickManager = joystickManager;
    this->_epsilonLinkManager = qgcApp()->toolbox()->epsilonLinkManager();

    connect(_multiVehicleManager, &MultiVehicleManager::activeVehicleChanged, this, &EpsilonCameraManagement::_activeVehicleChanged);
    connect(this->_joystickManager, &JoystickManager::activeCamJoystickChanged, this, &EpsilonCameraManagement::_activeCamJoystickChanged);
}

void EpsilonCameraManagement::setOpticalZoomSpeed(int opticalZoomSpeed)
{
    _opticalZoomSpeed = (uint8_t) opticalZoomSpeed;
}

int EpsilonCameraManagement::getOpticalZoomSpeed(void) const
{
    return (int) _opticalZoomSpeed;
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
        {
            continue;
        }
        doCamAction(button_action->action, buttons[buttonIndex], buttonIndex);
    }

    /* Calculating the zoom value */
    int zoomValue = getZoomValue(buttons, button_actions);
    if (prev_zoom_value != zoomValue)
    {
        prev_zoom_value = zoomValue;

        switch (zoomValue)
        {
        case MavExtCmdArg_ZoomIn:
            // setSysZoomInCommand();
            setOpticalZoomSpeed(6);
            break;
        case MavExtCmdArg_ZoomOut:
            // setSysZoomOutCommand();
            setOpticalZoomSpeed(-6);
            break;
        case MavExtCmdArg_ZoomStop:
            // etSysZoomStopCommand();
            setOpticalZoomSpeed(0);
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
    QString _file_name = "COLUGO";
    static int dayThermal = 0;

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
            if (dayThermal == 0)
            {
                setCameraOrderCommand(1);
                dayThermal = 1;
            }
            else
            {
                setCameraOrderCommand(0);
                dayThermal = 0;
            }
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
            setDoSnapshotCommand(1, 1, 1, 0, 0, _file_name);
        }
    }
    else if (buttonAction == "Rate")
    {
        /* Rate */
        if (doAction)
        {
            setCameraModeCommand(1);
        }
    }
    else if (buttonAction == "Rate Aid")
    {
        /* Rate Aid */
        if (doAction)
        {
            setCameraModeCommand(2);
        }
    }
    else if (buttonAction == "Stow")
    {
        /* Stow */
        if (doAction)
        {
            setCameraModeCommand(8);
        }
    }
    else if (buttonAction == "Pilot")
    {
        /* Pilot */
        if (doAction)
        {
            setCameraModeCommand(9);
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
    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_rate_control_pack(&message, cam_roll_yaw, cam_pitch, 0, 0, _opticalZoomSpeed, 0, 0);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setCameraScreenInformationCommand(uint switch_flags)
{
    /* Sending the Pilot command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_on_screen_information_pack(&message, (uint16_t) switch_flags);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setCameraOrderCommand(uint order)
{
    /* Sending the Pilot command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_camera_order_pack(&message, (uint8_t) order);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setFocusModeCommand(uint switch_mode)
{
    /* Sending the Pilot command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_focus_mode_pack(&message, (uint8_t) switch_mode);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setDoSnapshotCommand(uint frame_step, uint number_of_snapshots, uint source, uint format, uint metadata, QString file_name)
{
    /* Sending the Pilot command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        uint8_t _file_name[32] = {};
        for (int i = 0; i < file_name.size(); i++)
        {
            _file_name[i] = (uint8_t)(file_name.at(i).unicode());
        }
        linkProtocol->epsilon_link_msg_do_snapshot_pack(&message, (uint8_t) frame_step, (uint8_t) number_of_snapshots, (uint8_t) source, (uint8_t) format, (uint8_t) metadata, _file_name);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setVideoRecordingCommand(uint recording_state, QString file_name)
{
    /* Sending the Pilot command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        uint8_t _file_name[32] = {};
        for (int i = 0; i < file_name.size(); i++)
        {
            _file_name[i] = (uint8_t)(file_name.at(i).unicode());
        }
        linkProtocol->epsilon_link_msg_video_recording_pack(&message, (uint8_t) recording_state, _file_name);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setCameraModeCommand(uint control_mode)
{
    /* Sending the camera command */

    EpsilonLinkProtocol::epsilon_control_mode_t mode = (EpsilonLinkProtocol::epsilon_control_mode_t) control_mode;
    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_control_mode_pack(&message, mode, 0, 0, 0, 0, 0);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setSysDigitalZoomInCommand()
{
    /* Sending retract release command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_digital_zoom_pack(&message, 1);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void EpsilonCameraManagement::setSysDigitalZoomOutCommand()
{
    /* Sending retract release command */

    WeakEpsilonLinkInterfacePtr weakLink = this->_epsilonLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedEpsilonLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        EpsilonLinkProtocol *linkProtocol = this->_epsilonLinkManager->linkProtocol();

        EpsilonLinkProtocol::epsilon_link_message_t message = {};
        uint8_t buffer[EPSILON_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->epsilon_link_msg_digital_zoom_pack(&message, -1);
        int len = linkProtocol->epsilon_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
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
