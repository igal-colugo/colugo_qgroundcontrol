/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QByteArray>
#include <QFile>
#include <QLoggingCategory>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QTimer>

#include "CommtactLinkInterface.h"
#include "QGC.h"
#include "QGCApplication.h"
#include "QGCLoggingCategory.h"
#include "QGCTemporaryFile.h"
#include "QGCToolbox.h"
#include "Settings/SettingsManager.h"
#include "uas/UAS.h"
#include "uas/UASInterface.h"

#define PACKED_STRUCT(__Declaration__) __Declaration__ __attribute__((packed))

#define COMMTACT_LINK_MAX_PAYLOAD_LEN 255 ///< Maximum payload length
#define COMMTACT_LINK_NUM_CHECKSUM_BYTES 2
#define COMMTACT_LINK_STX_1 0xAA
#define COMMTACT_LINK_STX_2 0x55
#define COMMTACT_LINK_TERMINATOR 0xFF

#define COMMTACT_GLOBAL_STATUS_MSG_ID 0x80
#define COMMTACT_LINK_MSG_ID_GLOBAL_STATUS_LEN 55

#define _COMMTACT_PAYLOAD_NON_CONST(msg) ((char *) (&((msg)->payload64[0])))
#define _COMMTACT_PAYLOAD(msg) ((const char *) (&((msg)->payload64[0])))

class CommtactLinkManager;
class QGCApplication;

/**
 * @brief MAVLink micro air vehicle protocol reference implementation.
 *
 * MAVLink is a generic communication protocol for micro air vehicles.
 * for more information, please see the official website: https://mavlink.io
 **/
class CommtactLinkProtocol : public QGCTool
{
    Q_OBJECT

  public:
    //-------------------- GDT -------------------
    PACKED_STRUCT(typedef struct __commtact_gdt_get_report { uint8_t gdt_required_message; })
    commtact_gdt_get_report_t;

    PACKED_STRUCT(typedef struct __commtact_link_message_header {
        uint8_t time_stamp[4]; ///< protocol magic marker
        uint8_t seq_num[2];    ///< ID of device
        uint8_t opcode;        ///< ID of message in payload
    })
    commtact_link_message_header_t;

    PACKED_STRUCT(typedef struct __commtact_link_message {
        uint8_t time_stamp[4]; ///< protocol magic marker
        uint8_t seq_num[2];    ///< ID of device
        uint8_t opcode;        ///< ID of message in payload
        uint64_t payload64[COMMTACT_LINK_MAX_PAYLOAD_LEN / 8];
    })
    commtact_link_message_t;

    PACKED_STRUCT(typedef struct __commtact_global_status {
        uint8_t status_flags_0;
        uint8_t status_flags_1;
        uint8_t status_flags_2;
        uint8_t status_flags_3;
        uint16_t pan_position;
        int16_t tilt_position;
        uint8_t zoom_position;
        uint8_t focus_position;
        uint16_t field_of_view_v;
        uint16_t field_of_view_h;
        int8_t temperature_pan;
        int8_t temperature_tilt;
        int8_t temperature_vp;
        int32_t cam_latitude;
        int32_t cam_longitude;
        int16_t cam_altitude;
        int16_t ext_roll_angle;
        int16_t ext_pitch_angle;
        int16_t ext_yaw_angle;
        int16_t geo_azimuth_angle;
        int16_t geo_tilt_angle;
        uint16_t geo_distance;
        int32_t geo_latitude;
        int32_t geo_longitude;
        uint8_t gps_pdop_a;
        uint8_t gps_pdop_b;
        uint8_t gps_sat_count_a;
        uint8_t gps_sat_count_b;
        int16_t reserved3;
        uint8_t gps_antenna_signal_level;
        uint8_t geo_att_uncertainty;
    })
    commtact_global_status_t;

    union gdt_operational_mode_report_byte_20 {
        uint8_t byte;
        struct bit
        {
            uint8_t reserved : 4;
            uint8_t symbol_rate : 4;
        } bit_t;
    };

    PACKED_STRUCT(typedef struct __commtact_gdt_operational_modes_report {
        uint8_t transmitter_operational_mode;
        uint8_t gdt_pedestal_track_mode;
        uint8_t gdt_antenna_select;
        uint8_t reserved_1;
        uint8_t reserved_2;
        uint8_t reserved_3;
        uint8_t frequency_mode;
        uint8_t reserved_4;
        uint16_t azimuth_position;
        int16_t elevation_position;
        uint8_t reserved_5;
        uint32_t reserved_6;
        uint8_t tdd_operational_mode;
        uint8_t aes_encryption;
        uint8_t reserved_7;
        gdt_operational_mode_report_byte_20 byte_20;
        uint8_t unit_mode;
    })
    commtact_gdt_operational_modes_report_t;

    union gdt_status_report_byte_0 {
        uint8_t byte;
        struct bit
        {
            uint8_t tdd_sync : 1;
            uint8_t track_mode_status : 1;
            uint8_t pedestal_calibration_stat : 1;
        } bit_t;
    };

    PACKED_STRUCT(typedef struct __commtact_gdt_status_report {
        gdt_status_report_byte_0 byte_0;
        int8_t link_rssi;
        uint16_t link_transfered_packets;
        uint16_t link_error_packets;
        uint16_t link_crc_errors_packets;
        uint16_t pedestal_azimuth_pointing;
        int16_t pedestal_elevation_pointing;
        int16_t gdt_base_left_right;
        int16_t gdt_base_back_forward;
        int16_t gdt_base_roll;
        uint16_t range_meter;
        uint32_t range_meter_1m;
    })
    commtact_gdt_status_report_t;

    PACKED_STRUCT(typedef struct __commtact_gdt_constant_frequency_report { uint16_t gdt_operation_frequency; })
    commtact_gdt_constant_frequency_report_t;

    PACKED_STRUCT(typedef struct __commtact_gdt_operational_mode {
        uint8_t transmitter_operational_mode;
        uint8_t gdt_pedestal_track_mode;
        uint8_t gdt_antenna_select;
        uint16_t set_azimuth;
        int16_t set_elevation;
        uint8_t frequency_mode;
        uint8_t reserved_1;
        uint8_t tdd_operational_mode;
        uint8_t aes_encryption;
        uint8_t reserved_2;
        uint8_t symbol_rate;
        uint8_t unit_mode;
    })
    commtact_gdt_operational_mode_t;

    PACKED_STRUCT(typedef struct __commtact_gdt_constant_frequency { uint16_t gdt_opeartion_frequency; })
    commtact_gdt_constant_frequency_t;

    union gdt_cbit_report_byte_0 {
        uint8_t byte;
        struct bit
        {
            uint8_t reserved : 1;
            uint8_t adt_sp4t_failure_gdt_reserved : 1;
            uint8_t internal_dc_voltage_failure : 1;
            uint8_t digital_section_failure : 1;
            uint8_t rf_failure : 1;
            uint8_t synthesizers_lock_detect : 1;
            uint8_t pedestal_failure : 1;
            uint8_t active_antenna_reserved : 1;
        } bit_t;
    };
    union gdt_cbit_report_byte_1 {
        uint8_t byte;
        struct bit
        {
            uint8_t pa_over_current_failure : 1;
            uint8_t rx_input_over_power : 1;
            uint8_t digital_section_over_heat : 1;
            uint8_t rf_section_over_heat : 1;
            uint8_t system_startup : 1;
            uint8_t reserved : 1;
            uint8_t pa_power_output_low : 1;
            uint8_t pa_return_power : 1;
        } bit_t;
    };

    PACKED_STRUCT(typedef struct __commtact_gdt_cbit_report {
        gdt_cbit_report_byte_0 byte_0;
        gdt_cbit_report_byte_1 byte_1;
        int16_t pa_power_output;
        int16_t pa_return_power;
        uint8_t pedestal_failure_status_reserved;
        uint8_t gps_error;
        uint16_t ebox_status_reserved;
    })
    commtact_gdt_cbit_report_t;

    PACKED_STRUCT(typedef struct __commtact_gdt_mission_adt_status_report {
        uint8_t tdd_sync;
        int8_t link_rssi;
        uint16_t link_transfered_packets;
        uint16_t link_error_packets;
        uint16_t link_crc_error_packets;
        uint8_t adt_cbit_byte_0;
        uint8_t adt_cbit_byte_1;
        float_t gps_latitude;
        float_t gps_longitude;
        float_t altitude;
    })
    commtact_gdt_mission_adt_status_report_t;

    //-------------------- ADT -------------------
    union adt_operational_mode_report_byte_12 {
        uint8_t byte;
        struct bit
        {
            uint8_t video_rate : 4;
            uint8_t video_source : 2;
            uint8_t external_encoder_mode : 2;
        } bit_t;
    };
    union adt_operational_mode_report_byte_13 {
        uint8_t byte;
        struct bit
        {
            uint8_t reserved : 4;
            uint8_t symbol_rate : 4;
        } bit_t;
    };

    PACKED_STRUCT(typedef struct __commtact_adt_operational_mode {
        uint8_t transmitter_operational_mode;
        uint8_t antenna_select;
        uint8_t reserved_1;
        uint8_t reserved_2;
        uint8_t unit_mode;
        uint8_t adt_frequency_mode;
        uint8_t video_transmit_enable;
        uint8_t reserved_3;
        uint8_t reserved_4;
        uint8_t tdd_operational_mode;
        uint8_t aes_encryption_enable;
        uint8_t telemetry_metadata_separation;
        adt_operational_mode_report_byte_12 byte_12;
        adt_operational_mode_report_byte_13 byte_13;
    })
    commtact_adt_operational_mode_t;

    PACKED_STRUCT(typedef struct __commtact_adt_operational_modes_report {
        uint8_t transmitter_operational_mode;
        uint8_t antenna_select;
        uint8_t reserved_1;
        uint8_t reserved_2;
        uint8_t unit_mode;
        uint8_t adt_frequency_mode;
        uint8_t reserved_3;
        uint8_t video_transmit_enable;
        uint8_t reserved_4;
        uint8_t tdd_operational_mode;
        uint8_t aes_encryption_enable;
        uint8_t telemetry_metadata_separation;
        adt_operational_mode_report_byte_12 byte_12;
        adt_operational_mode_report_byte_13 byte_13;
    })
    commtact_adt_operational_modes_report_t;

    PACKED_STRUCT(typedef struct __commtact_adt_constant_frequency {
        uint16_t adt_main_channel_frequency;
        uint16_t reserved;
    })
    commtact_adt_constant_frequency_t;

    PACKED_STRUCT(typedef struct __commtact_adt_constant_frequency_report {
        uint16_t adt_primary_channel_frequency;
        uint16_t reserved;
    })
    commtact_adt_constant_frequency_report_t;

    PACKED_STRUCT(typedef struct __commtact_adt_status_report {
        uint8_t primary_tdd_sync;
        int8_t link_rssi;
        uint16_t link_transfered_packets;
        uint16_t link_error_packets;
        uint16_t link_crc_errors_packets;
        uint8_t reserved_1;
        int8_t reserved_2;
        uint16_t reserved_3;
        uint16_t reserved_4;
        uint16_t reserved_5;
    })
    commtact_adt_status_report_t;

    //--------------------------------------------

    //------------------ Common ------------------

    PACKED_STRUCT(typedef struct __commtact_basic_ethernet_settings {
        uint32_t icd_ip_address;
        uint16_t icd_listen_port;
        uint32_t icd_subnet_mask;
        uint32_t icd_default_gateway;
        uint32_t encoder_ip_address;
        uint16_t metadata_input_port;
        uint32_t reserved_1;
        uint16_t reserved_2;
        uint32_t host_ip;
        uint16_t host_port;
        uint32_t transceiver_video_dest_ip;
        uint16_t transceiver_video_dest_port;
        uint16_t user_payload_dest_ip;
        uint16_t user_payload_port;
        uint16_t discovery_port;
        uint32_t encoded_video_dest_aux_ip;
        uint16_t encoded_video_dest_aux_port;
        uint32_t encoded_video_dest_ip;
        uint16_t encoded_video_dest_port;
        uint32_t dsp_subnet_mask;
        uint32_t dsp_default_gateway;
        uint32_t ebox_controller_ip;
        uint16_t ebox_controller_port;
    })
    commtact_basic_ethernet_settings_t;

    PACKED_STRUCT(typedef struct __commtact_basic_ethernet_settings_report {
        uint32_t icd_ip_address;
        uint16_t icd_listen_port;
        uint32_t icd_subnet_mask;
        uint32_t icd_default_gateway;
        uint32_t encoder_ip_address;
        uint16_t metadata_input_port;
        uint32_t reserved_1;
        uint16_t reserved_2;
        uint32_t host_ip;
        uint16_t host_port;
        uint32_t transceiver_video_dest_ip;
        uint16_t transceiver_video_dest_port;
        uint32_t user_payload_dest_ip;
        uint16_t user_payload_port;
        uint16_t discovery_port;
        uint32_t encoded_video_dest_aux_ip;
        uint16_t encoded_video_dest_aux_port;
        uint32_t encoded_video_dest_ip;
        uint16_t encoded_video_dest_port;
        uint32_t dsp_subnet_mask;
        uint32_t dsp_default_gateway;
        uint32_t ebox_controller_ip;
        uint16_t ebox_controller_port;
    })
    commtact_basic_ethernet_settings_report_t;

    PACKED_STRUCT(typedef struct __commtact_discovery_report {
        uint32_t icd_ip_address;
        uint16_t icd_listen_port;
        uint32_t icd_dest_ip_address;
        uint16_t icd_dest_port;
        uint32_t subnet_mask;
        uint32_t default_gateway;
        uint8_t product_name[20];
        uint8_t setup_name[20];
        uint8_t icd_ver;
        uint8_t icd_rev;
        uint8_t dev_type;
        uint16_t discovery_dest_port;
        uint8_t pedestal_type;
        uint8_t link_encoding_type;
    })
    commtact_discovery_report_t;

    PACKED_STRUCT(typedef struct __commtact_version_report {
        uint8_t device_type;
        uint16_t fw_version;
        uint16_t sw0_version;
        uint16_t ebox_version;
        uint16_t sw1_version;
        uint16_t dsp_version;
        uint8_t reserved;
        uint8_t hw_version;
        uint8_t rf_hw_version;
    })
    commtact_version_report_t;

    PACKED_STRUCT(typedef struct __commtact_key_length_value_report {
        uint32_t key;
        uint32_t length;
        uint8_t data[256];
    })
    commtact_key_length_value_report_t;

    PACKED_STRUCT(typedef struct __commtact_extended_report {
        uint8_t required_message;
        uint8_t parameter_size;
        uint8_t parameters[256];
    })
    commtact_extended_report_t;

    PACKED_STRUCT(typedef struct __commtact_antennas_per_link_configuration {
        uint8_t role;
        uint16_t antenna_port_involved;
        uint8_t antenna_switching_mode;
        uint16_t ber_to_switch;
        uint16_t ber_to_peep;
        uint16_t test_period;
        uint16_t good_to_select;
        uint16_t peep_period;
        uint16_t ber_to_unsync;
    })
    commtact_antennas_per_link_configuration_t;

    PACKED_STRUCT(typedef struct __commtact_antennas_per_link_configuration_set {
        uint32_t key = 0x07000005;
        uint32_t length;
        uint8_t number_of_possible_configurations;
        commtact_antennas_per_link_configuration_t configuration[256];
    })
    commtact_antennas_per_link_configuration_set_t;

    PACKED_STRUCT(typedef struct __commtact_antennas_per_link_configuration_report {
        uint32_t key = 0x07000006;
        uint32_t length;
        uint8_t number_of_possible_configurations;
        commtact_antennas_per_link_configuration_t configuration[10];
    })
    commtact_antennas_per_link_configuration_report_t;

    //--------------------------------------------

    typedef enum
    {
        COMMTACT_LINK_PARSE_STATE_UNINIT = 0,
        COMMTACT_LINK_PARSE_STATE_IDLE,
        COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_1,
        COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_2,
        COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_3,
        COMMTACT_LINK_PARSE_STATE_GOT_TIME_STAMP_4,
        COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_1,
        COMMTACT_LINK_PARSE_STATE_GOT_SEQ_NUM_2,
        COMMTACT_LINK_PARSE_STATE_GOT_OPCODE,
        COMMTACT_LINK_PARSE_STATE_GOT_DATA
    } commtact_link_parse_state_t; ///< The state machine for the comm parser

    typedef struct __commtact_link_status
    {
        uint8_t msg_received;                    ///< Number of received messages
        uint8_t buffer_overrun;                  ///< Number of buffer overruns
        uint8_t parse_error;                     ///< Number of parse errors
        commtact_link_parse_state_t parse_state; ///< Parsing state machine
        uint8_t packet_idx;                      ///< Index in current packet
        uint8_t current_rx_seq;                  ///< Sequence number of last packet received
        uint8_t current_tx_seq;                  ///< Sequence number of last packet sent
        uint16_t packet_rx_success_count;        ///< Received packets
        uint16_t packet_rx_drop_count;           ///< Number of packet drops
        uint8_t flags;                           ///< MAVLINK_STATUS_FLAG_*
    } commtact_link_status_t;

    typedef enum
    {
        COMMTACT_LINK_FRAMING_INCOMPLETE = 0,
        COMMTACT_LINK_FRAMING_OK = 1
    } commtact_link_framing_t;

    typedef enum
    {
        GDT_OPERATIONAL_MODE_SET = 0x01,
        GDT_CONSTANT_FREQUENCY_SET = 0x02,
        GDT_GET_REPORT_MESSAGE = 0x09,
        GDT_OPERATIONAL_MODES_REPORT = 0X80,
        GDT_STATUS_REPORT = 0X81,
        GDT_MISSION_ADT_STATUS_REPORT = 0X82,
        GDT_CBIT_REPORT = 0X85,
        GDT_PBIT_REPORT = 0X86,
        GDT_CONSTANT_FREQUENCY_REPORT = 0X88,
    } commtact_link_opcode_t;

    typedef enum
    {
        ADT_OPERATIONAL_MODE_SET = 0x01,
        ADT_CONSTANT_FREQUENCY_SET = 0x02,
        ADT_VIDEO_SETTINGS = 0x04,
        ADT_GPS_SET = 0x06,
        ADT_OPERATIONAL_MODES_REPORT = 0X80,
        ADT_GPS_REPORT = 0X8B,
        ADT_VIDEO_SETTINGS_REPORT = 0X84,
        ADT_CONSTANT_FREQUENCY_REPORT = 0X88,
    } commtact_link_adt_opcode_t;

    typedef enum
    {
        COMMON_ETHERNET_SETTINGS = 0x05,
        COMMON_KEY_LENGTH_VALUE_REPORT = 0x67,
        COMMON_EXTENDED_REPORT = 0x69,
        COMMON_ETHERNET_SETTINGS_REPORT = 0x89,
        COMMON_VERSION_REPORT = 0x8A,
        COMMON_DISCOVERY_REPORT = 0xA7
    } commtact_link_common_opcode_t;

    typedef enum
    {
        GDT,
        ADT,
        RVT
    } commtact_device_type_t;

  public:
    CommtactLinkProtocol(QGCApplication *app, QGCToolbox *toolbox);
    ~CommtactLinkProtocol();

    /** @brief Get the human-friendly name of this protocol */
    QString getName();

    /**
     * Reset the counters for all metadata for this link.
     */
    virtual void resetMetadataForLink(CommtactLinkInterface *link);

    /// Suspend/Restart logging during replay.
    void suspendLogForReplay(bool suspend);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    uint16_t commtact_link_msg_to_send_buffer(uint8_t *buf, const CommtactLinkProtocol::commtact_link_message_t *msg, uint32_t payload_size);

    //--------------------- GDT ---------------------------------
    uint16_t commtact_link_msg_get_report_message_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t required_message);

    uint16_t commtact_link_msg_gdt_operational_frequency_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint16_t gdt_operational_frequency);

    uint16_t commtact_link_msg_operational_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t transmitter_operational_mode, uint8_t pedestal_track__mode,
                                                     uint8_t gdt_antenna_select, uint16_t set_azimuth, int16_t set_elevation, uint8_t frequency_mode, uint8_t reserved_1,
                                                     uint8_t tdd_operational_mode, uint8_t aes_encryption_enable, uint8_t reserved_2, uint8_t bit, uint8_t unit_mode);

    void commtact_link_msg_operational_modes_report_decode(const commtact_link_message_t *msg, commtact_gdt_operational_modes_report_t *operational_modes_report);
    void commtact_link_msg_gdt_status_report_decode(const commtact_link_message_t *msg, commtact_gdt_status_report_t *gdt_status_report);
    void commtact_link_msg_gdt_cbit_report_decode(const commtact_link_message_t *msg, commtact_gdt_cbit_report_t *gdt_cbit_report);
    void commtact_link_msg_gdt_constant_frequency_report_decode(const commtact_link_message_t *msg, commtact_gdt_constant_frequency_report_t *gdt_constant_frequency_report);
    void commtact_link_msg_gdt_mission_adt_status_report_decode(const commtact_link_message_t *msg, commtact_gdt_mission_adt_status_report_t *gdt_mission_adt_status_report);

    //--------------------- ADT ---------------------------------
    void commtact_link_msg_adt_operational_modes_report_decode(const commtact_link_message_t *msg, commtact_adt_operational_modes_report_t *adt_operational_modes_report);
    uint16_t commtact_link_msg_adt_operational_mode_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t transmitter_operational_mode, uint8_t antenna_select, uint8_t reserved_1,
                                                         uint8_t reserved_2, uint8_t unit_mode, uint8_t adt_frequency_mode, uint8_t video_transmit_enable, uint8_t reserved_3,
                                                         uint8_t reserved_4, uint8_t tdd_operational_mode, uint8_t aes_encryption_enable, uint8_t telemetry_metadata_separation,
                                                         uint8_t byte_12, uint8_t byte_13);
    void commtact_link_msg_adt_constant_frequency_report_decode(const commtact_link_message_t *msg, commtact_adt_constant_frequency_report_t *adt_constant_frequency_report);
    uint16_t commtact_link_msg_adt_operational_frequency_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint16_t adt_operational_frequency);
    void commtact_link_msg_adt_status_report_decode(const commtact_link_message_t *msg, commtact_adt_status_report_t *adt_status_report);
    //-----------------------------------------------------------

    //------------------- COMMON --------------------------------
    uint16_t commtact_link_msg_get_extended_report_message_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint8_t required_message, uint8_t parameter_size, uint8_t *parameters);
    void commtact_link_msg_common_ethernet_settings_report_decode(const commtact_link_message_t *msg, commtact_basic_ethernet_settings_report_t *basic_ethernet_settings_report);
    uint16_t commtact_link_msg_ethernet_settings_pack(CommtactLinkProtocol::commtact_link_message_t *msg, uint32_t icd_ip_address, uint16_t icd_listen_port, uint32_t icd_subnet_mask,
                                                      uint32_t icd_default_gateway, uint32_t encoder_ip_address, uint16_t metadata_input_port, uint32_t reserved_1, uint16_t reserved_2,
                                                      uint32_t host_ip, uint16_t host_port, uint32_t transceiver_video_dest_ip, uint16_t transceiver_video_dest_port,
                                                      uint16_t user_payload_dest_ip, uint16_t user_payload_port, uint16_t discovery_port, uint32_t encoded_video_dest_aux_ip,
                                                      uint16_t encoded_video_dest_aux_port, uint32_t encoded_video_dest_ip, uint16_t encoded_video_dest_port, uint32_t dsp_subnet_mask,
                                                      uint32_t dsp_default_gateway, uint32_t ebox_controller_ip, uint16_t ebox_controller_port);
    void commtact_link_msg_common_discovery_report_decode(const commtact_link_message_t *msg, commtact_discovery_report_t *discovery_report);
    void commtact_link_msg_common_version_report_decode(const commtact_link_message_t *msg, commtact_version_report_t *version_report);
    void commtact_link_msg_common_extended_report_decode(const commtact_link_message_t *msg, int msg_size, commtact_key_length_value_report_t *extended_report);
    void commtact_link_msg_common_antennas_per_link_configuration_report_decode(const commtact_link_message_t *msg, int msg_size,
                                                                                commtact_antennas_per_link_configuration_report_t *antennas_per_link_configuration_report);
    //-----------------------------------------------------------

  public slots:
    /** @brief Receive bytes from a communication interface */
    void receiveBytes(CommtactLinkInterface *link, QByteArray b);
    /** @brief Log bytes sent from a communication interface */
    void logSentBytes(CommtactLinkInterface *link, QByteArray b);
    /** @brief Load protocol settings */
    void loadSettings();
    /** @brief Store protocol settings */
    void storeSettings();
    /// @brief Deletes any log files which are in the temp directory
    static void deleteTempLogFiles(void);
    /// Checks for lost log files
    void checkForLostLogFiles(void);

  protected:
    uint64_t totalReceiveCounter; ///< The total number of successfully received messages
    uint64_t totalLossCounter;    ///< Total messages lost during transmission.
    float runningLossPercent;     ///< Loss rate

    commtact_link_message_t _message;
    commtact_link_status_t _status;

  signals:

    /** @brief Message received and directly copied via signal */
    void messageReceived(CommtactLinkInterface *link, commtact_link_message_t message, int message_size);
    /// Emitted when a temporary telemetry log file is ready for saving
    void saveTelemetryLog(QString tempLogfile);
    /// Emitted when a telemetry log is started to save.
    void checkTelemetrySavePath(void);

  private:
    const uint8_t crc8_Table[256] = {0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,  65,  157, 195, 33,  127, 252, 162, 64,  30,  95,  1,   227, 189, 62,
                                     96,  130, 220, 35,  125, 159, 193, 66,  28,  254, 160, 225, 191, 93,  3,   128, 222, 60,  98,  190, 224, 2,   92,  223, 129, 99,  61,  124, 34,
                                     192, 158, 29,  67,  161, 255, 70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,  102, 229, 187, 89,  7,   219, 133, 103, 57,  186, 228, 6,
                                     88,  25,  71,  165, 251, 120, 38,  196, 154, 101, 59,  217, 135, 4,   90,  184, 230, 167, 249, 27,  69,  198, 152, 122, 36,  248, 166, 68,  26,
                                     153, 199, 37,  123, 58,  100, 134, 216, 91,  5,   231, 185, 140, 210, 48,  110, 237, 179, 81,  15,  78,  16,  242, 172, 47,  113, 147, 205, 17,
                                     79,  173, 243, 112, 46,  204, 146, 211, 141, 111, 49,  178, 236, 14,  80,  175, 241, 19,  77,  206, 144, 114, 44,  109, 51,  209, 143, 12,  82,
                                     176, 238, 50,  108, 142, 208, 83,  13,  239, 177, 240, 174, 76,  18,  145, 207, 45,  115, 202, 148, 118, 40,  171, 245, 23,  73,  8,   86,  180,
                                     234, 105, 55,  213, 139, 87,  9,   235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,  233, 183, 85,  11,  136, 214, 52,  106,
                                     43,  117, 151, 201, 74,  20,  246, 168, 116, 42,  200, 150, 21,  75,  169, 247, 182, 232, 10,  84,  215, 137, 107, 53};

    bool _closeLogFile(void);
    void _startLogging(void);
    void _stopLogging(void);

    bool _logSuspendError;  ///< true: Logging suspended due to error
    bool _logSuspendReplay; ///< true: Logging suspended due to replay

    QGCTemporaryFile _tempLogFile;           ///< File to log to
    static const char *_tempLogFileTemplate; ///< Template for temporary log file
    static const char *_logFileExtension;    ///< Extension for log files

    CommtactLinkManager *_linkMgr;

    uint8_t _commtact_link_parse_char(uint8_t c, int buffer_size, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status);
    uint8_t _commtact_link_frame_char(uint8_t c, int buffer_size, commtact_link_message_t *r_message, commtact_link_status_t *r_mavlink_status);
    uint8_t _commtact_link_frame_char_buffer(commtact_link_message_t *rxmsg, int buffer_size, commtact_link_status_t *status, uint8_t c);
    void _commtact_link_start_checksum(commtact_link_message_t *msg);
    void _crc_init(uint8_t *crcAccum);
    void _commtact_parse_error(commtact_link_status_t *status);
    void _commtact_link_update_checksum(commtact_link_message_t *msg, uint8_t c);
    void _crc_accumulate(uint8_t data, uint8_t *crcAccum);
    uint8_t _check_sum_calculation(uint8_t *data, int8_t length);

    void _swap_bytes(uint16_t *data);
    void _swap_bytes(uint32_t *data);
};
