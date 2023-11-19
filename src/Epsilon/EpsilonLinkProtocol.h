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

#define EPSILON_LINK_STX_EPSILON_LINK1 0xFE // marker for old protocol

#define EPSILON_LINK_STATUS_FLAG_IN_EPSILON_LINK1 1  // last incoming packet was MAVLink1
#define EPSILON_LINK_STATUS_FLAG_OUT_EPSILON_LINK1 2 // generate MAVLink1 by default
#define EPSILON_LINK_STATUS_FLAG_IN_SIGNED 4         // last incoming packet was signed and validated
#define EPSILON_LINK_STATUS_FLAG_IN_BADSIG 8         // last incoming packet had a bad signature
#define EPSILON_LINK_SIGNATURE_BLOCK_LEN 13

#define EPSILON_LINK_IFLAG_MASK 0x01 // mask of all understood bits

#define EPSILON_LINK_MSG_ID_GLOBAL_STATUS_LEN 55

#define _EPSILON_PAYLOAD_NON_CONST(msg) ((char *) (&((msg)->payload64[0])))
#define _EPSILON_PAYLOAD(msg) ((const char *) (&((msg)->payload64[0])))

#define EPSILON_LINK_MESSAGE_CRCS                                                                                                                                                            \
    {                                                                                                                                                                                        \
        {0, 50, 9, 9, 0, 0, 0}, {1, 124, 31, 31, 0, 0, 0}, {2, 137, 12, 12, 0, 0, 0}, {4, 237, 14, 14, 3, 12, 13}, {5, 217, 28, 28, 1, 0, 0}, {6, 104, 3, 3, 0, 0, 0},                       \
            {7, 119, 32, 32, 0, 0, 0}, {8, 117, 36, 36, 0, 0, 0}, {11, 89, 6, 6, 1, 4, 0}, {20, 214, 20, 20, 3, 2, 3}, {21, 159, 2, 2, 3, 0, 1}, {22, 220, 25, 25, 0, 0, 0},                 \
            {23, 168, 23, 23, 3, 4, 5}, {24, 24, 30, 52, 0, 0, 0}, {25, 23, 101, 101, 0, 0, 0}, {26, 170, 22, 24, 0, 0, 0}, {27, 144, 26, 29, 0, 0, 0}, {28, 67, 16, 16, 0, 0, 0},           \
            {29, 115, 14, 16, 0, 0, 0}, {30, 39, 28, 28, 0, 0, 0}, {31, 246, 32, 48, 0, 0, 0}, {32, 185, 28, 28, 0, 0, 0}, {33, 104, 28, 28, 0, 0, 0}, {34, 237, 22, 22, 0, 0, 0},           \
            {35, 244, 22, 22, 0, 0, 0}, {36, 222, 21, 37, 0, 0, 0}, {37, 212, 6, 7, 3, 4, 5}, {38, 9, 6, 7, 3, 4, 5}, {39, 254, 37, 38, 3, 32, 33}, {40, 230, 4, 5, 3, 2, 3},                \
            {41, 28, 4, 4, 3, 2, 3}, {42, 28, 2, 2, 0, 0, 0}, {43, 132, 2, 3, 3, 0, 1}, {44, 221, 4, 5, 3, 2, 3}, {45, 232, 2, 3, 3, 0, 1}, {46, 11, 2, 2, 0, 0, 0},                         \
            {47, 153, 3, 4, 3, 0, 1}, {48, 41, 13, 21, 1, 12, 0}, {49, 39, 12, 20, 0, 0, 0}, {50, 78, 37, 37, 3, 18, 19}, {51, 196, 4, 5, 3, 2, 3}, {54, 15, 27, 27, 3, 24, 25},             \
            {55, 3, 25, 25, 0, 0, 0}, {61, 167, 72, 72, 0, 0, 0}, {62, 183, 26, 26, 0, 0, 0}, {63, 119, 181, 181, 0, 0, 0}, {64, 191, 225, 225, 0, 0, 0}, {65, 118, 42, 42, 0, 0, 0},        \
            {66, 148, 6, 6, 3, 2, 3}, {67, 21, 4, 4, 0, 0, 0}, {69, 243, 11, 18, 1, 10, 0}, {70, 124, 18, 38, 3, 16, 17}, {73, 38, 37, 38, 3, 32, 33}, {74, 20, 20, 20, 0, 0, 0},            \
            {75, 158, 35, 35, 3, 30, 31}, {76, 152, 33, 33, 3, 30, 31}, {77, 143, 3, 10, 3, 8, 9}, {80, 14, 4, 4, 3, 2, 3}, {81, 106, 22, 22, 0, 0, 0}, {82, 49, 39, 51, 3, 36, 37},         \
            {83, 22, 37, 37, 0, 0, 0}, {84, 143, 53, 53, 3, 50, 51}, {85, 140, 51, 51, 0, 0, 0}, {86, 5, 53, 53, 3, 50, 51}, {87, 150, 51, 51, 0, 0, 0}, {89, 231, 28, 28, 0, 0, 0},         \
            {90, 183, 56, 56, 0, 0, 0}, {91, 63, 42, 42, 0, 0, 0}, {92, 54, 33, 33, 0, 0, 0}, {93, 47, 81, 81, 0, 0, 0}, {100, 175, 26, 34, 0, 0, 0}, {101, 102, 32, 117, 0, 0, 0},          \
            {102, 158, 32, 117, 0, 0, 0}, {103, 208, 20, 57, 0, 0, 0}, {104, 56, 32, 116, 0, 0, 0}, {105, 93, 62, 63, 0, 0, 0}, {106, 138, 44, 44, 0, 0, 0}, {107, 108, 64, 65, 0, 0, 0},    \
            {108, 32, 84, 84, 0, 0, 0}, {109, 185, 9, 9, 0, 0, 0}, {110, 84, 254, 254, 3, 1, 2}, {111, 34, 16, 16, 0, 0, 0}, {112, 174, 12, 12, 0, 0, 0}, {113, 124, 36, 39, 0, 0, 0},       \
            {114, 237, 44, 44, 0, 0, 0}, {115, 4, 64, 64, 0, 0, 0}, {116, 76, 22, 24, 0, 0, 0}, {117, 128, 6, 6, 3, 4, 5}, {118, 56, 14, 14, 0, 0, 0}, {119, 116, 12, 12, 3, 10, 11},        \
            {120, 134, 97, 97, 0, 0, 0}, {121, 237, 2, 2, 3, 0, 1}, {122, 203, 2, 2, 3, 0, 1}, {123, 250, 113, 113, 3, 0, 1}, {124, 87, 35, 57, 0, 0, 0}, {125, 203, 6, 6, 0, 0, 0},         \
            {126, 220, 79, 81, 3, 79, 80}, {127, 25, 35, 35, 0, 0, 0}, {128, 226, 35, 35, 0, 0, 0}, {129, 46, 22, 24, 0, 0, 0}, {130, 29, 13, 13, 0, 0, 0}, {131, 223, 255, 255, 0, 0, 0},   \
            {132, 85, 14, 39, 0, 0, 0}, {133, 6, 18, 18, 0, 0, 0}, {134, 229, 43, 43, 0, 0, 0}, {135, 203, 8, 8, 0, 0, 0}, {136, 1, 22, 22, 0, 0, 0}, {137, 195, 14, 16, 0, 0, 0},           \
            {138, 109, 36, 120, 0, 0, 0}, {139, 168, 43, 43, 3, 41, 42}, {140, 181, 41, 41, 0, 0, 0}, {141, 47, 32, 32, 0, 0, 0}, {142, 72, 243, 243, 0, 0, 0}, {143, 131, 14, 16, 0, 0, 0}, \
            {144, 127, 93, 93, 0, 0, 0}, {146, 103, 100, 100, 0, 0, 0}, {147, 154, 36, 54, 0, 0, 0}, {148, 178, 60, 78, 0, 0, 0}, {149, 200, 30, 60, 0, 0, 0}, {150, 134, 42, 42, 0, 0, 0},  \
            {151, 219, 8, 8, 3, 6, 7}, {152, 208, 4, 8, 0, 0, 0}, {153, 188, 12, 12, 0, 0, 0}, {154, 84, 15, 15, 3, 6, 7}, {155, 22, 13, 13, 3, 4, 5}, {156, 19, 6, 6, 3, 0, 1},             \
            {157, 21, 15, 15, 3, 12, 13}, {158, 134, 14, 14, 3, 12, 13}, {160, 78, 12, 12, 3, 8, 9}, {161, 68, 3, 3, 3, 0, 1}, {162, 189, 8, 9, 0, 0, 0}, {163, 127, 28, 28, 0, 0, 0},       \
            {164, 154, 44, 44, 0, 0, 0}, {165, 21, 3, 3, 0, 0, 0}, {166, 21, 9, 9, 0, 0, 0}, {167, 144, 22, 22, 0, 0, 0}, {168, 1, 12, 12, 0, 0, 0}, {169, 234, 18, 18, 0, 0, 0},            \
            {170, 73, 34, 34, 0, 0, 0}, {171, 181, 66, 66, 0, 0, 0}, {172, 22, 98, 98, 0, 0, 0}, {173, 83, 8, 8, 0, 0, 0}, {174, 167, 48, 48, 0, 0, 0}, {175, 138, 19, 19, 3, 14, 15},       \
            {176, 234, 3, 3, 3, 0, 1}, {177, 240, 20, 20, 0, 0, 0}, {178, 47, 24, 24, 0, 0, 0}, {179, 189, 29, 29, 1, 26, 0}, {180, 52, 45, 47, 1, 42, 0}, {181, 174, 4, 4, 0, 0, 0},        \
            {182, 229, 40, 40, 0, 0, 0}, {183, 85, 2, 2, 3, 0, 1}, {184, 159, 206, 206, 3, 4, 5}, {185, 186, 7, 7, 3, 4, 5}, {186, 72, 29, 29, 3, 0, 1}, {191, 92, 27, 27, 0, 0, 0},         \
            {192, 36, 44, 54, 0, 0, 0}, {193, 71, 22, 26, 0, 0, 0}, {194, 98, 25, 33, 0, 0, 0}, {195, 120, 37, 37, 0, 0, 0}, {200, 134, 42, 42, 3, 40, 41}, {201, 205, 14, 14, 3, 12, 13},   \
            {214, 69, 8, 8, 3, 6, 7}, {215, 101, 3, 3, 0, 0, 0}, {216, 50, 3, 3, 3, 0, 1}, {217, 202, 6, 6, 0, 0, 0}, {218, 17, 7, 7, 3, 0, 1}, {219, 162, 2, 2, 0, 0, 0},                   \
            {225, 208, 65, 65, 0, 0, 0}, {226, 207, 8, 8, 0, 0, 0}, {230, 163, 42, 42, 0, 0, 0}, {231, 105, 40, 40, 0, 0, 0}, {232, 151, 63, 65, 0, 0, 0}, {233, 35, 182, 182, 0, 0, 0},     \
            {234, 150, 40, 40, 0, 0, 0}, {235, 179, 42, 42, 0, 0, 0}, {241, 90, 32, 32, 0, 0, 0}, {242, 104, 52, 60, 0, 0, 0}, {243, 85, 53, 61, 1, 52, 0}, {244, 95, 6, 6, 0, 0, 0},        \
            {245, 130, 2, 2, 0, 0, 0}, {246, 184, 38, 38, 0, 0, 0}, {247, 81, 19, 19, 0, 0, 0}, {248, 8, 254, 254, 3, 3, 4}, {249, 204, 36, 36, 0, 0, 0}, {250, 49, 30, 30, 0, 0, 0},        \
            {251, 170, 18, 18, 0, 0, 0}, {252, 44, 18, 18, 0, 0, 0}, {253, 83, 51, 54, 0, 0, 0}, {254, 46, 9, 9, 0, 0, 0}, {256, 71, 42, 42, 3, 8, 9}, {257, 131, 9, 9, 0, 0, 0},            \
            {258, 187, 32, 232, 3, 0, 1}, {259, 92, 235, 235, 0, 0, 0}, {260, 146, 5, 13, 0, 0, 0}, {261, 179, 27, 61, 0, 0, 0}, {262, 12, 18, 22, 0, 0, 0}, {263, 133, 255, 255, 0, 0, 0},  \
            {264, 49, 28, 28, 0, 0, 0}, {265, 26, 16, 20, 0, 0, 0}, {266, 193, 255, 255, 3, 2, 3}, {267, 35, 255, 255, 3, 2, 3}, {268, 14, 4, 4, 3, 2, 3}, {269, 109, 213, 213, 0, 0, 0},    \
            {270, 59, 19, 19, 0, 0, 0}, {271, 22, 52, 52, 0, 0, 0}, {275, 126, 31, 31, 0, 0, 0}, {276, 18, 49, 49, 0, 0, 0}, {280, 70, 33, 33, 0, 0, 0}, {281, 48, 13, 13, 0, 0, 0},         \
            {282, 123, 35, 35, 3, 32, 33}, {283, 74, 144, 144, 0, 0, 0}, {284, 99, 32, 32, 3, 30, 31}, {285, 137, 40, 40, 3, 38, 39}, {286, 210, 53, 53, 3, 50, 51},                         \
            {287, 1, 23, 23, 3, 20, 21}, {288, 20, 23, 23, 3, 20, 21}, {290, 251, 46, 46, 0, 0, 0}, {291, 10, 57, 57, 0, 0, 0}, {299, 19, 96, 98, 0, 0, 0}, {300, 217, 22, 22, 0, 0, 0},     \
            {301, 243, 58, 58, 0, 0, 0}, {310, 28, 17, 17, 0, 0, 0}, {311, 95, 116, 116, 0, 0, 0}, {320, 243, 20, 20, 3, 2, 3}, {321, 88, 2, 2, 3, 0, 1}, {322, 243, 149, 149, 0, 0, 0},     \
            {323, 78, 147, 147, 3, 0, 1}, {324, 132, 146, 146, 0, 0, 0}, {330, 23, 158, 167, 0, 0, 0}, {331, 91, 230, 232, 0, 0, 0}, {332, 236, 239, 239, 0, 0, 0},                          \
            {333, 231, 109, 109, 0, 0, 0}, {334, 72, 10, 10, 0, 0, 0}, {335, 225, 24, 24, 0, 0, 0}, {336, 245, 84, 84, 0, 0, 0}, {339, 199, 5, 5, 0, 0, 0}, {340, 99, 70, 70, 0, 0, 0},      \
            {350, 232, 20, 252, 0, 0, 0}, {360, 11, 25, 25, 0, 0, 0}, {370, 75, 87, 109, 0, 0, 0}, {373, 117, 42, 42, 0, 0, 0}, {375, 251, 140, 140, 0, 0, 0}, {380, 232, 20, 20, 0, 0, 0},  \
            {385, 147, 133, 133, 3, 2, 3}, {390, 156, 238, 238, 0, 0, 0}, {395, 0, 212, 212, 0, 0, 0}, {400, 110, 254, 254, 3, 4, 5}, {401, 183, 6, 6, 3, 4, 5},                             \
            {410, 160, 53, 53, 0, 0, 0}, {411, 106, 3, 3, 0, 0, 0}, {412, 33, 6, 6, 3, 4, 5}, {413, 77, 7, 7, 3, 4, 5}, {9000, 113, 137, 137, 0, 0, 0}, {9005, 117, 34, 34, 0, 0, 0},        \
            {10001, 209, 20, 20, 0, 0, 0}, {10002, 186, 41, 41, 0, 0, 0}, {10003, 4, 1, 1, 0, 0, 0}, {11000, 134, 51, 52, 3, 4, 5}, {11001, 15, 135, 136, 0, 0, 0},                          \
            {11002, 234, 179, 180, 3, 4, 5}, {11003, 64, 5, 5, 0, 0, 0}, {11010, 46, 49, 49, 0, 0, 0}, {11011, 106, 44, 44, 0, 0, 0}, {11020, 205, 16, 16, 0, 0, 0},                         \
            {11030, 144, 44, 44, 0, 0, 0}, {11031, 133, 44, 44, 0, 0, 0}, {11032, 85, 44, 44, 0, 0, 0}, {11033, 195, 37, 37, 3, 16, 17}, {11034, 79, 5, 5, 0, 0, 0},                         \
            {11035, 128, 8, 8, 3, 4, 5}, {11036, 177, 34, 34, 0, 0, 0}, {11037, 130, 28, 28, 0, 0, 0}, {11038, 47, 38, 38, 0, 0, 0}, {11039, 142, 9, 9, 0, 0, 0},                            \
            {12900, 114, 44, 44, 3, 0, 1}, {12901, 254, 59, 59, 3, 30, 31}, {12902, 140, 53, 53, 3, 4, 5}, {12903, 249, 46, 46, 3, 0, 1}, {12904, 150, 50, 50, 3, 24, 25},                   \
            {12905, 49, 43, 43, 3, 0, 1}, {12915, 94, 249, 249, 3, 0, 1}, {12920, 20, 5, 5, 0, 0, 0}, {40601, 201, 35, 35, 0, 0, 0}, {42000, 227, 1, 1, 0, 0, 0},                            \
        {                                                                                                                                                                                    \
            42001, 239, 46, 46, 0, 0, 0                                                                                                                                                      \
        }                                                                                                                                                                                    \
    }

#define EPSILON_GLOBAL_STATUS 0x80

class EpsilonLinkManager;
class QGCApplication;

Q_DECLARE_LOGGING_CATEGORY(EpsilonMAVLinkProtocolLog)

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
        uint64_t payload64[(MAVLINK_MAX_PAYLOAD_LEN + MAVLINK_NUM_CHECKSUM_BYTES + 7) / 8];
        uint8_t checksum;   ///< sent at end of packet
        uint8_t terminator; ///< sent at end of packet
    })
    epsilon_link_message_t;

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

  public:
    EpsilonLinkProtocol(QGCApplication *app, QGCToolbox *toolbox);
    ~EpsilonLinkProtocol();

    /** @brief Get the human-friendly name of this protocol */
    QString getName();
    /** @brief Get the system id of this application */
    int getSystemId() const;
    /** @brief Get the component id of this application */
    int getComponentId();
    /** @brief Get protocol version check state */
    bool versionCheckEnabled() const
    {
        return m_enable_version_check;
    }
    /** @brief Get the protocol version */
    int getVersion()
    {
        return MAVLINK_VERSION;
    }
    /** @brief Get the currently configured protocol version */
    unsigned getCurrentVersion() const
    {
        return _current_version;
    }
    /**
     * Reset the counters for all metadata for this link.
     */
    virtual void resetMetadataForLink(EpsilonLinkInterface *link);

    /// Suspend/Restart logging during replay.
    void suspendLogForReplay(bool suspend);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

  public slots:
    /** @brief Receive bytes from a communication interface */
    void receiveBytes(EpsilonLinkInterface *link, QByteArray b);

    /** @brief Log bytes sent from a communication interface */
    void logSentBytes(EpsilonLinkInterface *link, QByteArray b);

    /** @brief Set the system id of this application */
    void setSystemId(int id);

    /** @brief Enable / disable version check */
    void enableVersionCheck(bool enabled);

    /** @brief Load protocol settings */
    void loadSettings();
    /** @brief Store protocol settings */
    void storeSettings();

    /// @brief Deletes any log files which are in the temp directory
    static void deleteTempLogFiles(void);

    /// Checks for lost log files
    void checkForLostLogFiles(void);

  protected:
    bool m_enable_version_check;  ///< Enable checking of version match of MAV and QGC
    uint8_t lastIndex[256][256];  ///< Store the last received sequence ID for each system/componenet pair
    uint8_t firstMessage[256];    ///< First message flag
    uint64_t totalReceiveCounter; ///< The total number of successfully received messages
    uint64_t totalLossCounter;    ///< Total messages lost during transmission.
    float runningLossPercent;     ///< Loss rate

    epsilon_link_message_t _message;
    epsilon_link_status_t _status;

    bool versionMismatchIgnore;
    int systemId;
    unsigned _current_version;

  signals:
    /// Heartbeat received on link
    void vehicleHeartbeatInfo(EpsilonLinkInterface *link, int vehicleId, int componentId, int vehicleFirmwareType, int vehicleType);

    /** @brief Message received and directly copied via signal */
    void messageReceived(EpsilonLinkInterface *link, mavlink_message_t message);
    /** @brief Emitted if version check is enabled / disabled */
    void versionCheckChanged(bool enabled);
    /** @brief Emitted if a message from the protocol should reach the user */
    void protocolStatusMessage(const QString &title, const QString &message);
    /** @brief Emitted if a new system ID was set */
    void systemIdChanged(int systemId);

    void mavlinkMessageStatus(int uasId, uint64_t totalSent, uint64_t totalReceived, uint64_t totalLoss, float lossPercent);

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

    uint8_t epsilon_link_parse_char(uint8_t c, epsilon_link_message_t *r_message, epsilon_link_status_t *r_mavlink_status);
    uint8_t epsilon_link_frame_char(uint8_t c, epsilon_link_message_t *r_message, epsilon_link_status_t *r_mavlink_status);
    uint8_t epsilon_link_frame_char_buffer(epsilon_link_message_t *rxmsg, epsilon_link_status_t *status, uint8_t c);
    void epsilon_link_start_checksum(epsilon_link_message_t *msg);
    void epsilon_link_start_header_checksum(epsilon_link_message_t *msg);
    void crc_init(uint8_t *crcAccum);
    void _epsilon_parse_error(epsilon_link_status_t *status);
    void epsilon_link_update_checksum(epsilon_link_message_t *msg, uint8_t c);
    void epsilon_link_update_header_checksum(epsilon_link_message_t *msg, uint8_t c);
    void crc_accumulate(uint8_t data, uint8_t *crcAccum);
    const epsilon_link_msg_entry_t *epsilon_link_get_msg_entry(uint32_t msgid);

    void epsilon_link_msg_global_status_decode(const epsilon_link_message_t *msg, epsilon_global_status_t *global_status);
    void swap_bytes(uint16_t *data);
    void swap_bytes(uint32_t *data);
};
