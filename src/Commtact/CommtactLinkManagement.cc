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

//------------------------- GDT commands -----------------
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
    }
    return list;
}
QStringList CommtactLinkManagement::gdtAntennaSelectTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("OMNI");
        list += tr("DIRECTIONAL");
        list += tr("ADVANCED");
    }
    return list;
}
QStringList CommtactLinkManagement::gdtPedestalTrackModeTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("STOP");
        list += tr("RSSI");
        list += tr("GPS");
        list += tr("MANUAL");
        list += tr("AZIMUTH");
        list += tr("STOW");
    }
    return list;
}
QStringList CommtactLinkManagement::gdtTddOperationalModeTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("SLAVE");
        list += tr("MASTER");
    }
    return list;
}
QStringList CommtactLinkManagement::gdtFrequencyModeTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("FIX");
        list += tr("HOPPING");
    }
    return list;
}
QStringList CommtactLinkManagement::gdtUnitModeTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("NORMAL");
        list += tr("MISSION RELAY");
        list += tr("RELAY");
        list += tr("MISSION");
        list += tr("TRANSIT");
        list += tr("MISSION TMO");
        list += tr("MISSION TRANSIT");
    }
    return list;
}
QStringList CommtactLinkManagement::gdtSymbolRateTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("16MSPS");
        list += tr("8MSPS");
        list += tr("4MSPS");
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
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTAntennaSelectCommand(uint gdt_antenna_select)
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
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTPedestalTrackModeCommand(uint gdt_pedestal_track_mode)
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
            &message, _operational_modes_report.transmitter_operational_mode, gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTTddOperationalModeCommand(uint gdt_tdd_operational_mode)
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
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0, gdt_tdd_operational_mode,
            _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTFrequencyModeCommand(uint gdt_frequency_mode)
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
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, gdt_frequency_mode, 0, _operational_modes_report.tdd_operational_mode,
            _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTUnitModeCommand(uint gdt_unit_mode)
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
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, _operational_modes_report.byte_20.byte, gdt_unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTSymbolRateCommand(uint gdt_symbol_rate)
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

        CommtactLinkProtocol::gdt_operational_mode_report_byte_20 byte_20 = {};
        byte_20.bit_t.symbol_rate = gdt_symbol_rate;

        uint16_t payload_size = linkProtocol->commtact_link_msg_operational_mode_pack(
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, _operational_modes_report.aes_encryption, 0, byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setGDTAesEncryptionCommand(uint gdt_aes_encryption)
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
            &message, _operational_modes_report.transmitter_operational_mode, _operational_modes_report.gdt_pedestal_track_mode, _operational_modes_report.gdt_antenna_select,
            _operational_modes_report.azimuth_position, _operational_modes_report.elevation_position, _operational_modes_report.frequency_mode, 0,
            _operational_modes_report.tdd_operational_mode, gdt_aes_encryption, 0, _operational_modes_report.byte_20.byte, _operational_modes_report.unit_mode);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
//--------------------------------------------------------

bool CommtactLinkManagement::getShouldUiEnabledRescueElement()
{
    /* Set Focus State */
    return m_should_enable_rescue_btn == 1;
}
/*Parse incomming messages*/
void CommtactLinkManagement::_commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message)
{
    CommtactLinkProtocol *linkProtocol = this->_commtactLinkManager->linkProtocol();

    switch (message.opcode)
    {
    case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

        linkProtocol->commtact_link_msg_operational_modes_report_decode(&message, &_operational_modes_report);

        setTransmitterOperationalMode(_operational_modes_report.transmitter_operational_mode);
        setGdtAntennaSelect(_operational_modes_report.gdt_antenna_select);
        setGdtPedestalTrackMode(_operational_modes_report.gdt_pedestal_track_mode);
        setGdtTddOperationalMode(_operational_modes_report.tdd_operational_mode);
        setGdtFrequencyMode(_operational_modes_report.frequency_mode);
        setGdtUnitMode(_operational_modes_report.unit_mode);
        setGdtSymbolRate(_operational_modes_report.byte_20.bit_t.symbol_rate);
        setGdtAesEncryption(_operational_modes_report.aes_encryption);

        break;

    case CommtactLinkProtocol::GDT_STATUS_REPORT:

        linkProtocol->commtact_link_msg_gdt_status_report_decode(&message, &_gdt_status_report);

        break;
    }
}
