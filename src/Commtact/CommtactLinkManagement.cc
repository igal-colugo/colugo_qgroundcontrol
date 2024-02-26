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
QStringList CommtactLinkManagement::operationalModeTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("TX OFF");
        list += tr("TX LOW");
        list += tr("TX HIGH");
        list += tr("IBIT");
        list += tr("24dBm");
        list += tr("27dBm");
        list += tr("30dBm");
        if (list.size() != static_cast<int>(CommtactLinkConfiguration::TypeLast))
        {
            qWarning() << "Internal error";
        }
    }
    return list;
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_operational_mode_pack(
            &message, transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, _operational_modes_report.bit, _operational_modes_report.unit_mode);

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
        linkProtocol->commtact_link_msg_operational_modes_report_decode(&message, &_operational_modes_report);

        setTransmitterOperationalMode(_operational_modes_report.transmitter_operational_mode);

        break;
    }
}
