#include "CommtactLinkManagement.h"

CommtactLinkManagement::CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox) : QGCTool(app, toolbox)
{
    qmlRegisterUncreatableType<CommtactLinkManagement>("QGroundControl", 1, 0, "CommtactLinkManagement", "Reference only");
}
void CommtactLinkManagement::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    this->_commtactLinkManager = _toolbox->commtactLinkManager();
}

void CommtactLinkManagement::setGDTOperationalModeCommand(uint transmitter_operational_mode)
{
    /* Sending the camera command */
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_operational_mode_pack(&message, transmitter_operational_mode, 0, 0, 0, 0, 0, 0, 0x01, 0x01, 0, 0x10, 0);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

bool CommtactLinkManagement::getShouldUiEnabledRescueElement()
{
    /* Set Focus State */
    return m_should_enable_rescue_btn == 1;
}
/*Parse incomming messages*/
void CommtactLinkManagement::_commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message)
{
    switch (message.opcode)
    {
    case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

        CommtactLinkProtocol *linkProtocol = this->_commtactLinkManager->linkProtocol();
        CommtactLinkProtocol::commtact_gdt_operational_modes_report_t operational_modes_report = CommtactLinkProtocol::commtact_gdt_operational_modes_report_t{};
        linkProtocol->commtact_link_msg_operational_modes_report_decode(&message, &operational_modes_report);

        setTransmitterOperationalMode(operational_modes_report.transmitter_operational_mode);

        break;
    }
}
