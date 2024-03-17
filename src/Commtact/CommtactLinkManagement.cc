#include "CommtactLinkManagement.h"

static bool is_ip(const QString &address)
{
    int a, b, c, d;
    if (sscanf(address.toStdString().c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4 && strcmp("::1", address.toStdString().c_str()))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static QString get_ip_address(const QString &address)
{
    if (is_ip(address))
    {
        return address;
    }
    // Need to look it up
    QHostInfo info = QHostInfo::fromName(address);
    if (info.error() == QHostInfo::NoError)
    {
        QList<QHostAddress> hostAddresses = info.addresses();
        for (int i = 0; i < hostAddresses.size(); i++)
        {
            // Exclude all IPv6 addresses
            if (!hostAddresses.at(i).toString().contains(":"))
            {
                return hostAddresses.at(i).toString();
            }
        }
    }
    return QString();
}

CommtactLinkManagement::CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox) : QGCTool(app, toolbox)
{
    qmlRegisterUncreatableType<CommtactLinkManagement>("QGroundControl", 1, 0, "CommtactLinkManagement", "Reference only");
    connect(&_updateTimer, &QTimer::timeout, this, &CommtactLinkManagement::updateTimeout);
    _updateTimer.setInterval(500);
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
    if (counter == 2)
    {
        // Common ethernet settings report
        getGDTRequiredMessageCommand(0x89);
    }
    if (counter == 3)
    {
        // Common version report
        getGDTRequiredMessageCommand(0x8A);
    }

    counter++;
    if (counter > 3)
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_gdt_operational_frequency_pack(&message, gdt_operational_frequency);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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
        list += tr("Antenna1");
        list += tr("Antenna2");
        list += tr("Auto Select");
        list += tr("Antenna 2 SP4T-1");
        list += tr("Antenna 2 SP4T-2");
        list += tr("Antenna 2 SP4T-3");
        list += tr("Antenna 2 SP4T-4");
        list += tr("Antenna 6P no.1");
        list += tr("Antenna 6P no.2");
        list += tr("Antenna 6P no.3");
        list += tr("Antenna 6P no.4");
        list += tr("Antenna 6P no.5");
        list += tr("Antenna 6P no.6");
        list += tr("Advanced");
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
        list += tr("NORMAL(P2P)");
        list += tr("TRANSIT(P2MP)");
        list += tr("NORMAL TMO");
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

QStringList CommtactLinkManagement::adtVideoRateTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("1.8Mbps");
        list += tr("1.2Mbps");
        list += tr("900kbps");
    }
    return list;
}
QStringList CommtactLinkManagement::adtVideoSourceTypeStrings(void) const
{
    //-- Must follow same order as enum LinkType in LinkConfiguration.h
    static QStringList list;
    if (!list.size())
    {
        list += tr("ANALOG VIDEO");
        list += tr("DIGITAL VIDEO");
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTVideoTransmitEnableCommand(uint adt_video_transmit_enable)
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
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode, adt_video_transmit_enable,
            _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4, _adt_operational_modes_report.tdd_operational_mode,
            _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation, _adt_operational_modes_report.byte_12.byte,
            _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTTelemetryMetadataSeparationCommand(uint adt_telemetry_metadata_separation)
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
            _adt_operational_modes_report.tdd_operational_mode, _adt_operational_modes_report.aes_encryption_enable, adt_telemetry_metadata_separation,
            _adt_operational_modes_report.byte_12.byte, _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTVideoRateCommand(uint adt_video_rate)
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

        _adt_operational_modes_report.byte_12.bit_t.video_rate = adt_video_rate;

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode,
            _adt_operational_modes_report.video_transmit_enable, _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4,
            _adt_operational_modes_report.tdd_operational_mode, _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation,
            _adt_operational_modes_report.byte_12.byte, _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTVideoSourceCommand(uint adt_video_source)
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

        _adt_operational_modes_report.byte_12.bit_t.video_source = adt_video_source;

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_mode_pack(
            &message, _adt_operational_modes_report.transmitter_operational_mode, _adt_operational_modes_report.antenna_select, _adt_operational_modes_report.reserved_1,
            _adt_operational_modes_report.reserved_2, _adt_operational_modes_report.unit_mode, _adt_operational_modes_report.adt_frequency_mode,
            _adt_operational_modes_report.video_transmit_enable, _adt_operational_modes_report.reserved_3, _adt_operational_modes_report.reserved_4,
            _adt_operational_modes_report.tdd_operational_mode, _adt_operational_modes_report.aes_encryption_enable, _adt_operational_modes_report.telemetry_metadata_separation,
            _adt_operational_modes_report.byte_12.byte, _adt_operational_modes_report.byte_13.byte);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setADTOperationalFrequencyCommand(uint adt_operational_frequency)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_adt_operational_frequency_pack(&message, adt_operational_frequency);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}

//--------------------------------------------------------

//----------------------- COMMON commands ----------------
void CommtactLinkManagement::setCommonEthernetICDIPAddressCommand(QString icd_ip_address_port_string)
{
    QString icd_ip_address_string;
    uint icd_ip_address = 0;
    uint icd_listen_port = 0;
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

        // Handle x.x.x.x:p
        if (icd_ip_address_port_string.contains(":"))
        {
            icd_ip_address_string = icd_ip_address_port_string.split(":").first();
            icd_listen_port = icd_ip_address_port_string.split(":").last().toUInt();
        }
        else
        {
            icd_ip_address_string = icd_ip_address_port_string;
            icd_listen_port = _common_ethernet_settings_report.icd_listen_port;
        }

        QString ipAdd = get_ip_address(icd_ip_address_string);
        unsigned char *pointer_to_ip_address = (unsigned char *) &icd_ip_address;
        *(pointer_to_ip_address + 3) = (unsigned char) ipAdd.split(".")[0].toUInt();
        *(pointer_to_ip_address + 2) = (unsigned char) ipAdd.split(".")[1].toUInt();
        *(pointer_to_ip_address + 1) = (unsigned char) ipAdd.split(".")[2].toUInt();
        *(pointer_to_ip_address + 0) = (unsigned char) ipAdd.split(".")[3].toUInt();

        uint16_t payload_size = linkProtocol->commtact_link_msg_ethernet_settings_pack(
            &message, icd_ip_address, icd_listen_port, _common_ethernet_settings_report.icd_subnet_mask, _common_ethernet_settings_report.icd_default_gateway,
            _common_ethernet_settings_report.encoder_ip_address, _common_ethernet_settings_report.metadata_input_port, _common_ethernet_settings_report.reserved_1,
            _common_ethernet_settings_report.reserved_2, _common_ethernet_settings_report.host_ip, _common_ethernet_settings_report.host_port,
            _common_ethernet_settings_report.transceiver_video_dest_ip, _common_ethernet_settings_report.transceiver_video_dest_port, _common_ethernet_settings_report.user_payload_dest_ip,
            _common_ethernet_settings_report.user_payload_port, _common_ethernet_settings_report.discovery_port, _common_ethernet_settings_report.encoded_video_dest_aux_ip,
            _common_ethernet_settings_report.encoded_video_dest_aux_port, _common_ethernet_settings_report.encoded_video_dest_ip, _common_ethernet_settings_report.encoded_video_dest_port,
            _common_ethernet_settings_report.dsp_subnet_mask, _common_ethernet_settings_report.dsp_default_gateway, _common_ethernet_settings_report.ebox_controller_ip,
            _common_ethernet_settings_report.ebox_controller_port);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setCommonEthernetICDSubnetMaskCommand(QString icd_subnet_mask_string)
{
    uint icd_subnet_mask = 0;
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

        unsigned char *pointer_to_subnet_mask = (unsigned char *) &icd_subnet_mask;
        *(pointer_to_subnet_mask + 3) = (unsigned char) icd_subnet_mask_string.split(".")[0].toUInt();
        *(pointer_to_subnet_mask + 2) = (unsigned char) icd_subnet_mask_string.split(".")[1].toUInt();
        *(pointer_to_subnet_mask + 1) = (unsigned char) icd_subnet_mask_string.split(".")[2].toUInt();
        *(pointer_to_subnet_mask + 0) = (unsigned char) icd_subnet_mask_string.split(".")[3].toUInt();

        uint16_t payload_size = linkProtocol->commtact_link_msg_ethernet_settings_pack(
            &message, _common_ethernet_settings_report.icd_ip_address, _common_ethernet_settings_report.icd_listen_port, icd_subnet_mask,
            _common_ethernet_settings_report.icd_default_gateway, _common_ethernet_settings_report.encoder_ip_address, _common_ethernet_settings_report.metadata_input_port,
            _common_ethernet_settings_report.reserved_1, _common_ethernet_settings_report.reserved_2, _common_ethernet_settings_report.host_ip, _common_ethernet_settings_report.host_port,
            _common_ethernet_settings_report.transceiver_video_dest_ip, _common_ethernet_settings_report.transceiver_video_dest_port, _common_ethernet_settings_report.user_payload_dest_ip,
            _common_ethernet_settings_report.user_payload_port, _common_ethernet_settings_report.discovery_port, _common_ethernet_settings_report.encoded_video_dest_aux_ip,
            _common_ethernet_settings_report.encoded_video_dest_aux_port, _common_ethernet_settings_report.encoded_video_dest_ip, _common_ethernet_settings_report.encoded_video_dest_port,
            _common_ethernet_settings_report.dsp_subnet_mask, _common_ethernet_settings_report.dsp_default_gateway, _common_ethernet_settings_report.ebox_controller_ip,
            _common_ethernet_settings_report.ebox_controller_port);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setCommonEthernetICDDefaultGatewayCommand(QString icd_default_gateway_string)
{
    uint icd_default_gateway = 0;
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

        QString default_gateway = get_ip_address(icd_default_gateway_string);
        unsigned char *pointer_to_default_gateway = (unsigned char *) &icd_default_gateway;
        *(pointer_to_default_gateway + 3) = (unsigned char) default_gateway.split(".")[0].toUInt();
        *(pointer_to_default_gateway + 2) = (unsigned char) default_gateway.split(".")[1].toUInt();
        *(pointer_to_default_gateway + 1) = (unsigned char) default_gateway.split(".")[2].toUInt();
        *(pointer_to_default_gateway + 0) = (unsigned char) default_gateway.split(".")[3].toUInt();

        uint16_t payload_size = linkProtocol->commtact_link_msg_ethernet_settings_pack(
            &message, _common_ethernet_settings_report.icd_ip_address, _common_ethernet_settings_report.icd_listen_port, _common_ethernet_settings_report.icd_subnet_mask,
            icd_default_gateway, _common_ethernet_settings_report.encoder_ip_address, _common_ethernet_settings_report.metadata_input_port, _common_ethernet_settings_report.reserved_1,
            _common_ethernet_settings_report.reserved_2, _common_ethernet_settings_report.host_ip, _common_ethernet_settings_report.host_port,
            _common_ethernet_settings_report.transceiver_video_dest_ip, _common_ethernet_settings_report.transceiver_video_dest_port, _common_ethernet_settings_report.user_payload_dest_ip,
            _common_ethernet_settings_report.user_payload_port, _common_ethernet_settings_report.discovery_port, _common_ethernet_settings_report.encoded_video_dest_aux_ip,
            _common_ethernet_settings_report.encoded_video_dest_aux_port, _common_ethernet_settings_report.encoded_video_dest_ip, _common_ethernet_settings_report.encoded_video_dest_port,
            _common_ethernet_settings_report.dsp_subnet_mask, _common_ethernet_settings_report.dsp_default_gateway, _common_ethernet_settings_report.ebox_controller_ip,
            _common_ethernet_settings_report.ebox_controller_port);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setCommonEthernetICDHostIPCommand(QString icd_host_ip_port_string)
{
    QString icd_host_ip_address_string;
    uint icd_host_ip_address = 0;
    uint icd_host_port = 0;
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

        // Handle x.x.x.x:p
        if (icd_host_ip_port_string.contains(":"))
        {
            icd_host_ip_address_string = icd_host_ip_port_string.split(":").first();
            icd_host_port = icd_host_ip_port_string.split(":").last().toUInt();
        }
        else
        {
            icd_host_ip_address_string = icd_host_ip_port_string;
            icd_host_port = _common_ethernet_settings_report.host_port;
        }

        QString ipAdd = get_ip_address(icd_host_ip_address_string);
        unsigned char *pointer_to_ip_address = (unsigned char *) &icd_host_ip_address;
        *(pointer_to_ip_address + 3) = (unsigned char) ipAdd.split(".")[0].toUInt();
        *(pointer_to_ip_address + 2) = (unsigned char) ipAdd.split(".")[1].toUInt();
        *(pointer_to_ip_address + 1) = (unsigned char) ipAdd.split(".")[2].toUInt();
        *(pointer_to_ip_address + 0) = (unsigned char) ipAdd.split(".")[3].toUInt();

        uint16_t payload_size = linkProtocol->commtact_link_msg_ethernet_settings_pack(
            &message, _common_ethernet_settings_report.icd_ip_address, _common_ethernet_settings_report.icd_listen_port, _common_ethernet_settings_report.icd_subnet_mask,
            _common_ethernet_settings_report.icd_default_gateway, _common_ethernet_settings_report.encoder_ip_address, _common_ethernet_settings_report.metadata_input_port,
            _common_ethernet_settings_report.reserved_1, _common_ethernet_settings_report.reserved_2, icd_host_ip_address, icd_host_port,
            _common_ethernet_settings_report.transceiver_video_dest_ip, _common_ethernet_settings_report.transceiver_video_dest_port, _common_ethernet_settings_report.user_payload_dest_ip,
            _common_ethernet_settings_report.user_payload_port, _common_ethernet_settings_report.discovery_port, _common_ethernet_settings_report.encoded_video_dest_aux_ip,
            _common_ethernet_settings_report.encoded_video_dest_aux_port, _common_ethernet_settings_report.encoded_video_dest_ip, _common_ethernet_settings_report.encoded_video_dest_port,
            _common_ethernet_settings_report.dsp_subnet_mask, _common_ethernet_settings_report.dsp_default_gateway, _common_ethernet_settings_report.ebox_controller_ip,
            _common_ethernet_settings_report.ebox_controller_port);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
        int len = linkProtocol->commtact_link_msg_to_send_buffer(&buffer[0], &message, payload_size);

        sharedLink->writeBytesThreadSafe((const char *) buffer, len);
    }
}
void CommtactLinkManagement::setCommonEthernetICDDiscoveryPortCommand(uint icd_discovery_port)
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

        uint16_t payload_size = linkProtocol->commtact_link_msg_ethernet_settings_pack(
            &message, _common_ethernet_settings_report.icd_ip_address, _common_ethernet_settings_report.icd_listen_port, _common_ethernet_settings_report.icd_subnet_mask,
            _common_ethernet_settings_report.icd_default_gateway, _common_ethernet_settings_report.encoder_ip_address, _common_ethernet_settings_report.metadata_input_port,
            _common_ethernet_settings_report.reserved_1, _common_ethernet_settings_report.reserved_2, _common_ethernet_settings_report.host_ip, _common_ethernet_settings_report.host_port,
            _common_ethernet_settings_report.transceiver_video_dest_ip, _common_ethernet_settings_report.transceiver_video_dest_port, _common_ethernet_settings_report.user_payload_dest_ip,
            _common_ethernet_settings_report.user_payload_port, icd_discovery_port, _common_ethernet_settings_report.encoded_video_dest_aux_ip,
            _common_ethernet_settings_report.encoded_video_dest_aux_port, _common_ethernet_settings_report.encoded_video_dest_ip, _common_ethernet_settings_report.encoded_video_dest_port,
            _common_ethernet_settings_report.dsp_subnet_mask, _common_ethernet_settings_report.dsp_default_gateway, _common_ethernet_settings_report.ebox_controller_ip,
            _common_ethernet_settings_report.ebox_controller_port);

        uint8_t buffer[sizeof(CommtactLinkProtocol::commtact_link_message_header_t) + payload_size] = {};
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
    static int adtDataRecieved = 0;
    static int gdtDataRecieved = 0;

    if (link->linkConfiguration()->name().toLower().contains("gdt"))
    {
        gdtDataRecieved++;
        if (gdtDataRecieved <= 10)
        {
            if (getGDTDataRecieved() == 0)
            {
                setGDTDataRecieved(1);
            }
        }
        else if (gdtDataRecieved > 10 && gdtDataRecieved < 20)
        {
            if (getGDTDataRecieved() == 1)
            {
                setGDTDataRecieved(0);
            }
        }
        else
        {
            gdtDataRecieved = 0;
        }

        switch (message.opcode)
        {
        case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_operational_modes_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
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

            break;

        case CommtactLinkProtocol::GDT_STATUS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
            {
                linkProtocol->commtact_link_msg_gdt_status_report_decode(&message, &_gdt_status_report);

                setGdtTDDSync(_gdt_status_report.byte_0.bit_t.tdd_sync);
                setGdtLinkRSSI(_gdt_status_report.link_rssi);
                setGdtLinkTransferedPackets(_gdt_status_report.link_transfered_packets);
                setGdtLinkErrorPackets(_gdt_status_report.link_error_packets);
                setGdtLinkCRCErrorPackets(_gdt_status_report.link_crc_errors_packets);
            }

            break;

        case CommtactLinkProtocol::GDT_CONSTANT_FREQUENCY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_constant_frequency_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
            {
                linkProtocol->commtact_link_msg_gdt_constant_frequency_report_decode(&message, &_gdt_constant_frequency_report);

                setGdtOperationFrequency(_gdt_constant_frequency_report.gdt_operation_frequency);
            }

            break;

        case CommtactLinkProtocol::GDT_CBIT_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_cbit_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_gdt_cbit_report_decode(&message, &_gdt_cbit_report);

                setGdtCBITPAPowerOutput(_gdt_cbit_report.pa_power_output);
                setGdtCBITPAReturnPower(_gdt_cbit_report.pa_return_power);
            }

            break;

        case CommtactLinkProtocol::GDT_MISSION_ADT_STATUS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_mission_adt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_gdt_mission_adt_status_report_decode(&message, &_gdt_mission_adt_status_report);
            }

            break;

        case CommtactLinkProtocol::COMMON_ETHERNET_SETTINGS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_basic_ethernet_settings_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_ethernet_settings_report_decode(&message, &_common_ethernet_settings_report);

                setCommonICDIPAddressInt(_common_ethernet_settings_report.icd_ip_address);
                setCommonICDPort(_common_ethernet_settings_report.icd_listen_port);
                setCommonICDSubnetMaskInt(_common_ethernet_settings_report.icd_subnet_mask);
                setCommonICDDefaultGatewayInt(_common_ethernet_settings_report.icd_default_gateway);
                setCommonICDHostIPAddressInt(_common_ethernet_settings_report.host_ip);
                setCommonICDHostPort(_common_ethernet_settings_report.host_port);
                setCommonICDDiscoveryPort(_common_ethernet_settings_report.discovery_port);
            }

            break;

        case CommtactLinkProtocol::COMMON_VERSION_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_version_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_version_report_decode(&message, &_common_version_report);

                setCommonICDGDTFWVersion(_common_version_report.fw_version);
                setCommonICDGDTSW0Version(_common_version_report.sw0_version);
                setCommonICDGDTSW1Version(_common_version_report.sw1_version);
            }

            break;

        case CommtactLinkProtocol::COMMON_DISCOVERY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_discovery_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_discovery_report_decode(&message, &_common_discovery_report);

                setCommonICDDiscoveryGDTIPAddressInt(_common_discovery_report.icd_ip_address);
                setCommonICDDiscoveryGDTPort(_common_discovery_report.icd_listen_port);
                setCommonICDDiscoveryDestGDTIPAddressInt(_common_discovery_report.icd_dest_ip_address);
                setCommonICDDiscoveryDestGDTPort(_common_discovery_report.icd_dest_port);
                setCommonICDDiscoveryGDTSubnetMaskInt(_common_discovery_report.subnet_mask);
                setCommonICDDiscoveryGDTDefaultGatewayInt(_common_discovery_report.default_gateway);
                setCommonICDGDTDiscoveryVer(_common_discovery_report.icd_ver);
                setCommonICDGDTDiscoveryRev(_common_discovery_report.icd_rev);
                setCommonICDGDTDiscoveryDevType(_common_discovery_report.dev_type);
                setCommonICDGDTDiscoveryDestPort(_common_discovery_report.discovery_dest_port);
            }

            break;
        }
    }
    else if (link->linkConfiguration()->name().toLower().contains("adt"))
    {
        adtDataRecieved++;
        if (adtDataRecieved <= 10)
        {
            if (getADTDataRecieved() == 0)
            {
                setADTDataRecieved(1);
            }
        }
        else if (adtDataRecieved > 10 && adtDataRecieved < 20)
        {
            if (getADTDataRecieved() == 1)
            {
                setADTDataRecieved(0);
            }
        }
        else
        {
            adtDataRecieved = 0;
        }

        switch (message.opcode)
        {
        case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_operational_modes_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_operational_modes_report_decode(&message, &_adt_operational_modes_report);

                setAdtTransmitterOperationalMode(_adt_operational_modes_report.transmitter_operational_mode);
                setAdtAntennaSelect(_adt_operational_modes_report.antenna_select);
                setAdtTddOperationalMode(_adt_operational_modes_report.tdd_operational_mode);
                setAdtFrequencyMode(_adt_operational_modes_report.adt_frequency_mode);
                setAdtVideoTransmitEnable(_adt_operational_modes_report.video_transmit_enable);
                setAdtUnitMode(_adt_operational_modes_report.unit_mode);
                setAdtSymbolRate(_adt_operational_modes_report.byte_13.bit_t.symbol_rate);
                setAdtAesEncryption(_adt_operational_modes_report.aes_encryption_enable);
                setAdtTelemetryMetadataSeparation(_adt_operational_modes_report.telemetry_metadata_separation);
                setAdtVideoRate(_adt_operational_modes_report.byte_12.bit_t.video_rate);
                setAdtVideoSource(_adt_operational_modes_report.byte_12.bit_t.video_source);
            }

            break;

        case CommtactLinkProtocol::GDT_STATUS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_status_report_decode(&message, &_adt_status_report);

                setAdtTDDSync(_adt_status_report.primary_tdd_sync);
                setAdtLinkRSSI(_adt_status_report.link_rssi);
                setAdtLinkTransferedPackets(_adt_status_report.link_transfered_packets);
                setAdtLinkErrorPackets(_adt_status_report.link_error_packets);
                setAdtLinkCRCErrorPackets(_adt_status_report.link_crc_errors_packets);
            }

            break;

        case CommtactLinkProtocol::GDT_CONSTANT_FREQUENCY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_constant_frequency_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_constant_frequency_report_decode(&message, &_adt_constant_frequency_report);

                setAdtOperationFrequency(_adt_constant_frequency_report.adt_primary_channel_frequency);
            }

            break;

        case CommtactLinkProtocol::GDT_CBIT_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_cbit_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_gdt_cbit_report_decode(&message, &_gdt_cbit_report);

                setGdtCBITPAPowerOutput(_gdt_cbit_report.pa_power_output);
                setGdtCBITPAReturnPower(_gdt_cbit_report.pa_return_power);
            }

            break;

        case CommtactLinkProtocol::COMMON_ETHERNET_SETTINGS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_basic_ethernet_settings_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_ethernet_settings_report_decode(&message, &_common_ethernet_settings_report);

                setCommonICDADTIPAddressInt(_common_ethernet_settings_report.icd_ip_address);
                setCommonICDADTPort(_common_ethernet_settings_report.icd_listen_port);
                setCommonICDADTSubnetMaskInt(_common_ethernet_settings_report.icd_subnet_mask);
                setCommonICDADTDefaultGatewayInt(_common_ethernet_settings_report.icd_default_gateway);
                setCommonICDADTHostIPAddressInt(_common_ethernet_settings_report.host_ip);
                setCommonICDADTHostPort(_common_ethernet_settings_report.host_port);
                setCommonICDADTDiscoveryPort(_common_ethernet_settings_report.discovery_port);
            }

            break;

        case CommtactLinkProtocol::COMMON_VERSION_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_version_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_version_report_decode(&message, &_common_version_report);

                setCommonICDADTFWVersion(_common_version_report.fw_version);
                setCommonICDADTSW0Version(_common_version_report.sw0_version);
                setCommonICDADTSW1Version(_common_version_report.sw1_version);
            }

            break;

        case CommtactLinkProtocol::COMMON_DISCOVERY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_discovery_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_discovery_report_decode(&message, &_common_discovery_report);

                setCommonICDDiscoveryADTIPAddressInt(_common_discovery_report.icd_ip_address);
                setCommonICDDiscoveryADTPort(_common_discovery_report.icd_listen_port);
                setCommonICDDiscoveryDestADTIPAddressInt(_common_discovery_report.icd_dest_ip_address);
                setCommonICDDiscoveryDestADTPort(_common_discovery_report.icd_dest_port);
                setCommonICDDiscoveryADTSubnetMaskInt(_common_discovery_report.subnet_mask);
                setCommonICDDiscoveryADTDefaultGatewayInt(_common_discovery_report.default_gateway);
                setCommonICDADTDiscoveryVer(_common_discovery_report.icd_ver);
                setCommonICDADTDiscoveryRev(_common_discovery_report.icd_rev);
                setCommonICDADTDiscoveryDevType(_common_discovery_report.dev_type);
                setCommonICDADTDiscoveryDestPort(_common_discovery_report.discovery_dest_port);
            }

            break;
        }
    }
    else
    {
        switch (message.opcode)
        {
        case CommtactLinkProtocol::GDT_OPERATIONAL_MODES_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_operational_modes_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
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
            else if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_operational_modes_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_operational_modes_report_decode(&message, &_adt_operational_modes_report);

                setAdtTransmitterOperationalMode(_adt_operational_modes_report.transmitter_operational_mode);
                setAdtAntennaSelect(_adt_operational_modes_report.antenna_select);
                setAdtTddOperationalMode(_adt_operational_modes_report.tdd_operational_mode);
                setAdtFrequencyMode(_adt_operational_modes_report.adt_frequency_mode);
                setAdtVideoTransmitEnable(_adt_operational_modes_report.video_transmit_enable);
                setAdtUnitMode(_adt_operational_modes_report.unit_mode);
                setAdtSymbolRate(_adt_operational_modes_report.byte_13.bit_t.symbol_rate);
                setAdtAesEncryption(_adt_operational_modes_report.aes_encryption_enable);
                setAdtTelemetryMetadataSeparation(_adt_operational_modes_report.telemetry_metadata_separation);
                setAdtVideoRate(_adt_operational_modes_report.byte_12.bit_t.video_rate);
                setAdtVideoSource(_adt_operational_modes_report.byte_12.bit_t.video_source);
            }

            break;

        case CommtactLinkProtocol::GDT_STATUS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
            {
                linkProtocol->commtact_link_msg_gdt_status_report_decode(&message, &_gdt_status_report);

                setGdtTDDSync(_gdt_status_report.byte_0.bit_t.tdd_sync);
                setGdtLinkRSSI(_gdt_status_report.link_rssi);
                setGdtLinkTransferedPackets(_gdt_status_report.link_transfered_packets);
                setGdtLinkErrorPackets(_gdt_status_report.link_error_packets);
                setGdtLinkCRCErrorPackets(_gdt_status_report.link_crc_errors_packets);
            }
            else if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_status_report_decode(&message, &_adt_status_report);

                setAdtTDDSync(_adt_status_report.primary_tdd_sync);
                setAdtLinkRSSI(_adt_status_report.link_rssi);
                setAdtLinkTransferedPackets(_adt_status_report.link_transfered_packets);
                setAdtLinkErrorPackets(_adt_status_report.link_error_packets);
                setAdtLinkCRCErrorPackets(_adt_status_report.link_crc_errors_packets);
            }

            break;

        case CommtactLinkProtocol::GDT_CONSTANT_FREQUENCY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_constant_frequency_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // GDT message
            {
                linkProtocol->commtact_link_msg_gdt_constant_frequency_report_decode(&message, &_gdt_constant_frequency_report);

                setGdtOperationFrequency(_gdt_constant_frequency_report.gdt_operation_frequency);
            }
            else if (message_size == sizeof(CommtactLinkProtocol::commtact_adt_constant_frequency_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t)) // ADT message
            {
                linkProtocol->commtact_link_msg_adt_constant_frequency_report_decode(&message, &_adt_constant_frequency_report);

                setAdtOperationFrequency(_adt_constant_frequency_report.adt_primary_channel_frequency);
            }

            break;

        case CommtactLinkProtocol::GDT_CBIT_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_cbit_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_gdt_cbit_report_decode(&message, &_gdt_cbit_report);

                setGdtCBITPAPowerOutput(_gdt_cbit_report.pa_power_output);
                setGdtCBITPAReturnPower(_gdt_cbit_report.pa_return_power);
            }

            break;

        case CommtactLinkProtocol::GDT_MISSION_ADT_STATUS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_gdt_mission_adt_status_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_gdt_mission_adt_status_report_decode(&message, &_gdt_mission_adt_status_report);
            }

            break;

        case CommtactLinkProtocol::COMMON_ETHERNET_SETTINGS_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_basic_ethernet_settings_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_ethernet_settings_report_decode(&message, &_common_ethernet_settings_report);

                if (_common_ethernet_settings_report.discovery_port == 56000) // GDT
                {
                    setCommonICDIPAddressInt(_common_ethernet_settings_report.icd_ip_address);
                    setCommonICDPort(_common_ethernet_settings_report.icd_listen_port);
                    setCommonICDSubnetMaskInt(_common_ethernet_settings_report.icd_subnet_mask);
                    setCommonICDDefaultGatewayInt(_common_ethernet_settings_report.icd_default_gateway);
                    setCommonICDHostIPAddressInt(_common_ethernet_settings_report.host_ip);
                    setCommonICDHostPort(_common_ethernet_settings_report.host_port);
                    setCommonICDDiscoveryPort(_common_ethernet_settings_report.discovery_port);
                }
                else if (_common_ethernet_settings_report.discovery_port == 57000) // ADT
                {
                    setCommonICDADTIPAddressInt(_common_ethernet_settings_report.icd_ip_address);
                    setCommonICDADTPort(_common_ethernet_settings_report.icd_listen_port);
                    setCommonICDADTSubnetMaskInt(_common_ethernet_settings_report.icd_subnet_mask);
                    setCommonICDADTDefaultGatewayInt(_common_ethernet_settings_report.icd_default_gateway);
                    setCommonICDADTHostIPAddressInt(_common_ethernet_settings_report.host_ip);
                    setCommonICDADTHostPort(_common_ethernet_settings_report.host_port);
                    setCommonICDADTDiscoveryPort(_common_ethernet_settings_report.discovery_port);
                }
            }

            break;

        case CommtactLinkProtocol::COMMON_VERSION_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_version_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_version_report_decode(&message, &_common_version_report);
            }

            break;

        case CommtactLinkProtocol::COMMON_DISCOVERY_REPORT:

            if (message_size == sizeof(CommtactLinkProtocol::commtact_discovery_report_t) + sizeof(CommtactLinkProtocol::commtact_link_message_header_t))
            {
                linkProtocol->commtact_link_msg_common_discovery_report_decode(&message, &_common_discovery_report);

                if (_common_discovery_report.dev_type == 0) // GDT
                {
                    setCommonICDDiscoveryGDTIPAddressInt(_common_discovery_report.icd_ip_address);
                    setCommonICDDiscoveryGDTPort(_common_discovery_report.icd_listen_port);
                    setCommonICDDiscoveryDestGDTIPAddressInt(_common_discovery_report.icd_dest_ip_address);
                    setCommonICDDiscoveryDestGDTPort(_common_discovery_report.icd_dest_port);
                    setCommonICDDiscoveryGDTSubnetMaskInt(_common_discovery_report.subnet_mask);
                    setCommonICDDiscoveryGDTDefaultGatewayInt(_common_discovery_report.default_gateway);
                    setCommonICDGDTDiscoveryVer(_common_discovery_report.icd_ver);
                    setCommonICDGDTDiscoveryRev(_common_discovery_report.icd_rev);
                    setCommonICDGDTDiscoveryDevType(_common_discovery_report.dev_type);
                    setCommonICDGDTDiscoveryDestPort(_common_discovery_report.discovery_dest_port);
                }
                else if (_common_discovery_report.dev_type == 1) // ADT
                {
                    setCommonICDDiscoveryADTIPAddressInt(_common_discovery_report.icd_ip_address);
                    setCommonICDDiscoveryADTPort(_common_discovery_report.icd_listen_port);
                    setCommonICDDiscoveryDestADTIPAddressInt(_common_discovery_report.icd_dest_ip_address);
                    setCommonICDDiscoveryDestADTPort(_common_discovery_report.icd_dest_port);
                    setCommonICDDiscoveryADTSubnetMaskInt(_common_discovery_report.subnet_mask);
                    setCommonICDDiscoveryADTDefaultGatewayInt(_common_discovery_report.default_gateway);
                    setCommonICDADTDiscoveryVer(_common_discovery_report.icd_ver);
                    setCommonICDADTDiscoveryRev(_common_discovery_report.icd_rev);
                    setCommonICDADTDiscoveryDevType(_common_discovery_report.dev_type);
                    setCommonICDADTDiscoveryDestPort(_common_discovery_report.discovery_dest_port);
                }
            }

            break;
        }
    }
}
