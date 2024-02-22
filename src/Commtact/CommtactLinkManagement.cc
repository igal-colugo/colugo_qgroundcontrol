#include "CommtactLinkManagement.h"

CommtactLinkManagement::CommtactLinkManagement(QObject *parent) : QObject(parent)
{
    this->_commtactLinkManager = qgcApp()->toolbox()->commtactLinkManager();
}
CommtactLinkManagement::CommtactLinkManagement(CommtactLinkManager *commtactLinkManager)
{
    this->_commtactLinkManager = commtactLinkManager;

    qmlRegisterUncreatableType<CommtactLinkManagement>("QGroundControl", 1, 0, "CommtactLinkManagement", "Reference only");
}
/* Sending gimbal Command Messages */
void CommtactLinkManagement::sendGimbalCommand(float cam_roll_yaw, float cam_pitch)
{
    WeakCommtactLinkInterfacePtr weakLink = this->_commtactLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedCommtactLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        CommtactLinkProtocol *linkProtocol = this->_commtactLinkManager->linkProtocol();

        CommtactLinkProtocol::commtact_link_message_t message = {};
        uint8_t buffer[COMMTACT_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->commtact_link_msg_rate_control_pack(&message, cam_roll_yaw, cam_pitch, 0, 0, 0, 0, 0);
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
bool CommtactLinkManagement::getShouldUiEnabledRescueElement()
{
    /* Set Focus State */
    return m_should_enable_rescue_btn == 1;
}
void CommtactLinkManagement::setCameraModeCommand(uint control_mode)
{
    /* Sending the camera command */

    CommtactLinkProtocol::commtact_control_mode_t mode = (CommtactLinkProtocol::commtact_control_mode_t) control_mode;
    WeakCommtactLinkInterfacePtr weakLink = this->_commtactLinkManager->selectedSharedLinkInterfacePointerForLink();
    if (weakLink.expired())
    {
        return;
    }
    SharedCommtactLinkInterfacePtr sharedLink = weakLink.lock();

    if (sharedLink != nullptr)
    {
        CommtactLinkProtocol *linkProtocol = this->_commtactLinkManager->linkProtocol();

        CommtactLinkProtocol::commtact_link_message_t message = {};
        uint8_t buffer[COMMTACT_LINK_MAX_PAYLOAD_LEN] = {};
        linkProtocol->commtact_link_msg_control_mode_pack(&message, mode, 0, 0, 0, 0, 0);
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
