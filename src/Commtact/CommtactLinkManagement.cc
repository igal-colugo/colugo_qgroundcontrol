#include "CommtactLinkManagement.h"

CommtactLinkManagement::CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox) : QGCTool(app, toolbox)
{
    qmlRegisterUncreatableType<CommtactLinkManagement>("QGroundControl", 1, 0, "CommtactLinkManagement", "Reference only");
    connect(&_updateTimer, &QTimer::timeout, this, &CommtactLinkManagement::updateTimeout);
    _updateTimer.setInterval(1000);
    _updateTimer.start();
}
void CommtactLinkManagement::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    this->_commtactLinkManager = _toolbox->commtactLinkManager();
}

// every 1 sec send request to desired report
void CommtactLinkManagement::updateTimeout()
{
    static int counter = 0;

    if (counter == 0)
    {
        // GDT constant frequency report
        getGDTRequiredMessageCommand(0x88);
    }
    if (counter == 1)
    {
        // GDT mission ADT status report
        getGDTRequiredMessageCommand(0x82);
    }

    counter++;
    if (counter > 1)
    {
        counter = 0;
    }
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
void CommtactLinkManagement::setGDTOperationalFrequencyCommand(uint gdt_operational_frequency)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_gdt_opeartional_frequency_pack(&message, gdt_operational_frequency);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

void CommtactLinkManagement::getGDTRequiredMessageCommand(uint gdt_required_message)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_get_report_message_pack(&message, gdt_required_message);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
//--------------------------------------------------------

//------------------------- ADT commands -----------------
QStringList CommtactLinkManagement::adtOperationalModeTypeStrings(void) const
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
QStringList CommtactLinkManagement::adtAntennaSelectTypeStrings(void) const
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
QStringList CommtactLinkManagement::adtTddOperationalModeTypeStrings(void) const
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
QStringList CommtactLinkManagement::adtFrequencyModeTypeStrings(void) const
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
QStringList CommtactLinkManagement::adtUnitModeTypeStrings(void) const
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
QStringList CommtactLinkManagement::adtSymbolRateTypeStrings(void) const
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

void CommtactLinkManagement::setADTOperationalModeCommand(uint transmitter_operational_mode)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1, _adt_operational_modes_report.reserved_2,
            _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode, _adt_operational_modes_report.video_transmit_enable,
            _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, _adt_operational_modes_report.tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTAntennaSelectCommand(uint adt_antenna_select)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, adt_antenna_select, _adt_operational_modes_report.reserved_1, _adt_operational_modes_report.reserved_2,
            _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode, _adt_operational_modes_report.video_transmit_enable,
            _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, _adt_operational_modes_report.tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTTddOperationalModeCommand(uint adt_tdd_operational_mode)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode,
            _adt_operational_modes_report.video_transmit_enable, _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, adt_tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTFrequencyModeCommand(uint adt_frequency_mode)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, adt_frequency_mode, _adt_operational_modes_report.video_transmit_enable,
            _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, _adt_operational_modes_report.tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTUnitModeCommand(uint adt_unit_mode)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, adt_unit_mode, _adt_operational_modes_report.adt_frequency_mode, _adt_operational_modes_report.video_transmit_enable,
            _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, _adt_operational_modes_report.tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTSymbolRateCommand(uint adt_symbol_rate)
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

        CommtactLinkProtocol::adt_operational_mode_report_byte_13 byte_13 = {};
        byte_13.bit_t.symbol_rate = adt_symbol_rate;

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode,
            _adt_operational_modes_report.video_transmit_enable, _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4,
            _adt_operational_modes_report.tdd_operational_mode, _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation,
            _adt_operational_modes_report.byte_12.byte, byte_13.byte);

        uint8_t buffer[7 + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTAesEncryptionCommand(uint adt_aes_encryption)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode,
            _adt_operational_modes_report.video_transmit_enable, _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4,
            _adt_operational_modes_report.tdd_operational_mode, adt_aes_encryption, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

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
void CommtactLinkManagement::_commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message, int message_size)
{
    CommtactLinkProtocol *linkProtocol = this->_commtactLinkManager->linkProtocol();

    switch (message.opcode)
    {
    case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

        if (message_size == 29) // GDT message
        {
            linkProtocol->commtact_link_msg_operational_modes_report_decode(&message, &_operational_modes_report);

            setTransmitterOperationalMode(_operational_modes_report.transmitter_operational_mode);
            setGdtAntennaSelect(_operational_modes_report.gdt_antenna_select);
            setGdtPedestalTrackMode(_operational_modes_report.gdt_pedestal_track_mode);
            setGdtTddOperationalMode(_operational_modes_report.tdd_operational_mode);
            setGdtFrequencyMode(_operational_modes_report.frequency_mode);
            setGdtUnitMode(_operational_modes_report.unit_mode);
            setGdtSymbolRate(_operational_modes_report.byte_20.bit_t.symbol_rate);
            setGdtAesEncryption(_operational_modes_report.aes_encryption);
        }
        else if (message_size == 21) // ADT message
        {
            linkProtocol->commtact_link_msg_adt_operational_modes_report_decode(&message, &_adt_operational_modes_report);
        }

        break;

    case CommtactLinkProtocol::GDT_STATUS_REPORT:

        linkProtocol->commtact_link_msg_gdt_status_report_decode(&message, &_gdt_status_report);

        setGdtTDDSync(_gdt_status_report.byte_0.bit_t.tdd_sync);
        setGdtLinkRSSI(_gdt_status_report.link_rssi);
        setGdtLinkTransferedPackets(_gdt_status_report.link_transfered_packets);
        setGdtLinkErrorPackets(_gdt_status_report.link_error_packets);
        setGdtLinkCRCErrorPackets(_gdt_status_report.link_crc_errors_packets);

        break;

    case CommtactLinkProtocol::GDT_CONSTANT_FREQUENCY_REPORT:

        linkProtocol->commtact_link_msg_gdt_constant_frequency_report_decode(&message, &_gdt_constant_frequency_report);

        setGdtOperationFrequency(_gdt_constant_frequency_report.gdt_operation_frequency);

        break;

    case CommtactLinkProtocol::GDT_CBIT_REPORT:

        linkProtocol->commtact_link_msg_gdt_cbit_report_decode(&message, &_gdt_cbit_report);

        setGdtCBITPAPowerOutput(_gdt_cbit_report.pa_power_output);
        setGdtCBITPAReturnPower(_gdt_cbit_report.pa_return_power);

        break;

    case CommtactLinkProtocol::GDT_MISSION_ADT_STATUS_REPORT:

        linkProtocol->commtact_link_msg_gdt_mission_adt_status_report_decode(&message, &_gdt_mission_adt_status_report);

        break;
    }
}
