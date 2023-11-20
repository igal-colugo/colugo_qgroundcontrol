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

#include "EpsilonLinkInterface.h"
#include "QGC.h"
#include "QGCTemporaryFile.h"
#include "QGCToolbox.h"

#define PACKED_STRUCT(__Declaration__) __Declaration__ __attribute__((packed))

#define EPSILON_LINK_MAX_PAYLOAD_LEN 255 ///< Maximum payload length
#define EPSILON_LINK_NUM_CHECKSUM_BYTES 2
#define EPSILON_LINK_STX_1 0xAA
#define EPSILON_LINK_STX_2 0x55
#define EPSILON_LINK_TERMINATOR 0xFF

#define EPSILON_GLOBAL_STATUS_MSG_ID 0x80
#define EPSILON_LINK_MSG_ID_GLOBAL_STATUS_LEN 55

#define _EPSILON_PAYLOAD_NON_CONST(msg) ((char *) (&((msg)->payload64[0])))
#define _EPSILON_PAYLOAD(msg) ((const char *) (&((msg)->payload64[0])))

class EpsilonLinkManager;
class QGCApplication;

Q_DECLARE_LOGGING_CATEGORY(EpsilonLinkProtocolLog)

/**
 * @brief MAVLink micro air vehicle protocol reference implementation.
 *
 * MAVLink is a generic communication protocol for micro air vehicles.
 * for more information, please see the official website: https://mavlink.io
 **/
class EpsilonLinkProtocol : public QGCTool
{
    Q_OBJECT

  public:
    PACKED_STRUCT(typedef struct __epsilon_link_message {
        uint8_t magic[2];        ///< protocol magic marker
        uint8_t device_id;       ///< ID of device
        uint8_t message_id;      ///< ID of message in payload
        uint8_t length;          ///< Length of payload
        uint8_t header_checksum; ///< Header checksum
        uint64_t payload64[EPSILON_LINK_MAX_PAYLOAD_LEN / 8];
        uint8_t checksum;   ///< Data checksum
        uint8_t terminator; ///< Terminator
    })
    epsilon_link_message_t;

    PACKED_STRUCT(typedef struct __epsilon_link_zero_length_message {
        uint8_t magic[2];        ///< protocol magic marker
        uint8_t device_id;       ///< ID of device
        uint8_t message_id;      ///< ID of message in payload
        uint8_t length;          ///< Length of payload
        uint8_t header_checksum; ///< Header checksum
        uint8_t terminator;      ///< Terminator
    })
    epsilon_link_zero_length_message_t;

    PACKED_STRUCT(typedef struct __epsilon_rate_control_message {
        int8_t pan_speed;
        int8_t tilt_speed;
        int8_t nudge_column;
        int8_t nudge_raw;
        int8_t optical_zoom_speed;
        int8_t focus_adjustment;
        int16_t geo_dted;
    })
    epsilon_rate_control_message_t;

    PACKED_STRUCT(typedef struct __epsilon_digital_zoom_message { int8_t digital_zoom; })
    epsilon_digital_zoom_message_t;

    PACKED_STRUCT(typedef struct __epsilon_on_screen_information_message { uint16_t switch_flags; })
    epsilon_on_screen_information_message_t;

    PACKED_STRUCT(typedef struct __epsilon_camera_order_message { uint8_t order; })
    epsilon_camera_order_message_t;

    PACKED_STRUCT(typedef struct __epsilon_control_mode_message {
        int8_t mode;
        uint16_t pixel_pos_x;
        uint16_t pixel_pos_y;
        uint8_t box_sizes;
        uint8_t tracking_advanced;
        uint8_t options;
    })
    epsilon_control_mode_message_t;

    PACKED_STRUCT(typedef struct __epsilon_global_status {
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
    epsilon_global_status_t;

    typedef enum
    {
        EPSILON_LINK_PARSE_STATE_UNINIT = 0,
        EPSILON_LINK_PARSE_STATE_IDLE,
        EPSILON_LINK_PARSE_STATE_GOT_STX_1,
        EPSILON_LINK_PARSE_STATE_GOT_STX_2,
        EPSILON_LINK_PARSE_STATE_GOT_DEVICE_ID,
        EPSILON_LINK_PARSE_STATE_GOT_MESSAGE_ID,
        EPSILON_LINK_PARSE_STATE_GOT_LENGTH,
        EPSILON_LINK_PARSE_STATE_GOT_HEADER_CRC,
        EPSILON_LINK_PARSE_STATE_GOT_PAYLOAD,
        EPSILON_LINK_PARSE_STATE_GOT_CRC,
        EPSILON_LINK_PARSE_STATE_GOT_TERMINATOR,
        EPSILON_LINK_PARSE_STATE_GOT_BAD_CRC,
        EPSILON_LINK_PARSE_STATE_GOT_BAD_HEADER_CRC,
        EPSILON_LINK_PARSE_STATE_GOT_BAD_TERMINATOR
    } epsilon_link_parse_state_t; ///< The state machine for the comm parser

    typedef struct __epsilon_link_status
    {
        uint8_t msg_received;                   ///< Number of received messages
        uint8_t buffer_overrun;                 ///< Number of buffer overruns
        uint8_t parse_error;                    ///< Number of parse errors
        epsilon_link_parse_state_t parse_state; ///< Parsing state machine
        uint8_t packet_idx;                     ///< Index in current packet
        uint8_t current_rx_seq;                 ///< Sequence number of last packet received
        uint8_t current_tx_seq;                 ///< Sequence number of last packet sent
        uint16_t packet_rx_success_count;       ///< Received packets
        uint16_t packet_rx_drop_count;          ///< Number of packet drops
        uint8_t flags;                          ///< MAVLINK_STATUS_FLAG_*
    } epsilon_link_status_t;

    typedef enum
    {
        EPSILON_LINK_FRAMING_INCOMPLETE = 0,
        EPSILON_LINK_FRAMING_OK = 1,
        EPSILON_LINK_FRAMING_BAD_CRC = 2,
        EPSILON_LINK_FRAMING_BAD_SIGNATURE = 3
    } epsilon_link_framing_t;

    typedef struct __epsilon_link_msg_entry
    {
        uint32_t msgid;
        uint8_t crc_extra;
        uint8_t min_msg_len;          // minimum message length
        uint8_t max_msg_len;          // maximum message length (e.g. including mavlink2 extensions)
        uint8_t flags;                // MAV_MSG_ENTRY_FLAG_*
        uint8_t target_system_ofs;    // payload offset to target_system, or 0
        uint8_t target_component_ofs; // payload offset to target_component, or 0
    } epsilon_link_msg_entry_t;

    typedef enum
    {
        EPSILON_NO_CHANGE = 0,
        EPSILON_RATE,
        EPSILON_RATE_AID,
        EPSILON_TRACKING_STATIONARY,
        EPSILON_TRACKING_VEHICLE,
        EPSILON_TRACKING_SCENE,
        EPSILON_TRACKING_STATIC,
        EPSILON_GEO_LOCK,
        EPSILON_STOW,
        EPSILON_PILOT
    } epsilon_control_mode_t; ///< The state machine for the comm parser

  public:
    EpsilonLinkProtocol(QGCApplication *app, QGCToolbox *toolbox);
    ~EpsilonLinkProtocol();

    /** @brief Get the human-friendly name of this protocol */
    QString getName();

    /**
     * Reset the counters for all metadata for this link.
     */
    virtual void resetMetadataForLink(EpsilonLinkInterface *link);

    /// Suspend/Restart logging during replay.
    void suspendLogForReplay(bool suspend);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    uint16_t epsilon_link_msg_rate_control_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, int8_t pan_speed, int8_t tilt_speed, int8_t nudge_column, int8_t nudge_raw,
                                                int8_t optical_zoom_speed, int8_t focus_adjustment, int16_t geo_dted);

    uint16_t epsilon_link_msg_control_mode_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, uint8_t mode, uint16_t pixel_pos_x, uint16_t pixel_pos_y, uint8_t box_sizes,
                                                uint8_t tracking_advanced, uint8_t options);

    uint16_t epsilon_link_msg_digital_zoom_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, uint8_t digital_zoom);

    uint16_t epsilon_link_msg_on_screen_information_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, uint16_t switch_flags);

    uint16_t epsilon_link_msg_camera_order_pack(EpsilonLinkProtocol::epsilon_link_message_t *msg, uint8_t order);

    uint16_t epsilon_link_msg_to_send_buffer(uint8_t *buf, const EpsilonLinkProtocol::epsilon_link_message_t *msg);

  public slots:
    /** @brief Receive bytes from a communication interface */
    void receiveBytes(EpsilonLinkInterface *link, QByteArray b);

    /** @brief Log bytes sent from a communication interface */
    void logSentBytes(EpsilonLinkInterface *link, QByteArray b);

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

    epsilon_link_message_t _message;
    epsilon_link_status_t _status;

  signals:

    /** @brief Message received and directly copied via signal */
    void messageReceived(EpsilonLinkInterface *link, mavlink_message_t message);

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

    EpsilonLinkManager *_linkMgr;

    uint8_t _epsilon_link_parse_char(uint8_t c, epsilon_link_message_t *r_message, epsilon_link_status_t *r_mavlink_status);
    uint8_t _epsilon_link_frame_char(uint8_t c, epsilon_link_message_t *r_message, epsilon_link_status_t *r_mavlink_status);
    uint8_t _epsilon_link_frame_char_buffer(epsilon_link_message_t *rxmsg, epsilon_link_status_t *status, uint8_t c);
    void _epsilon_link_start_checksum(epsilon_link_message_t *msg);
    void _epsilon_link_start_header_checksum(epsilon_link_message_t *msg);
    void _crc_init(uint8_t *crcAccum);
    void _epsilon_parse_error(epsilon_link_status_t *status);
    void _epsilon_link_update_checksum(epsilon_link_message_t *msg, uint8_t c);
    void _epsilon_link_update_header_checksum(epsilon_link_message_t *msg, uint8_t c);
    void _crc_accumulate(uint8_t data, uint8_t *crcAccum);
    uint8_t _check_sum_calculation(uint8_t *data, int8_t length);

    void _epsilon_link_msg_global_status_decode(const epsilon_link_message_t *msg, epsilon_global_status_t *global_status);
    void _swap_bytes(uint16_t *data);
    void _swap_bytes(uint32_t *data);
};
