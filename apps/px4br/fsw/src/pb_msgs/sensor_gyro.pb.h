/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.6 at Wed Oct 26 00:46:16 2016. */

#ifndef PB_SENSOR_GYRO_PB_H_INCLUDED
#define PB_SENSOR_GYRO_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _px4_sensor_gyro_pb {
    uint64_t timestamp;
    uint64_t integral_dt;
    uint64_t error_count;
    float x;
    float y;
    float z;
    float x_integral;
    float y_integral;
    float z_integral;
    float temperature;
    float range_rad_s;
    float scaling;
    uint32_t device_id;
    int32_t x_raw;
    int32_t y_raw;
    int32_t z_raw;
    int32_t temperature_raw;
/* @@protoc_insertion_point(struct:px4_sensor_gyro_pb) */
} px4_sensor_gyro_pb;

/* Default values for struct fields */

/* Initializer values for message structs */
#define px4_sensor_gyro_pb_init_default          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define px4_sensor_gyro_pb_init_zero             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define px4_sensor_gyro_pb_timestamp_tag         1
#define px4_sensor_gyro_pb_integral_dt_tag       2
#define px4_sensor_gyro_pb_error_count_tag       3
#define px4_sensor_gyro_pb_x_tag                 4
#define px4_sensor_gyro_pb_y_tag                 5
#define px4_sensor_gyro_pb_z_tag                 6
#define px4_sensor_gyro_pb_x_integral_tag        7
#define px4_sensor_gyro_pb_y_integral_tag        8
#define px4_sensor_gyro_pb_z_integral_tag        9
#define px4_sensor_gyro_pb_temperature_tag       10
#define px4_sensor_gyro_pb_range_rad_s_tag       11
#define px4_sensor_gyro_pb_scaling_tag           12
#define px4_sensor_gyro_pb_device_id_tag         13
#define px4_sensor_gyro_pb_x_raw_tag             14
#define px4_sensor_gyro_pb_y_raw_tag             15
#define px4_sensor_gyro_pb_z_raw_tag             16
#define px4_sensor_gyro_pb_temperature_raw_tag   17

/* Struct field encoding specification for nanopb */
extern const pb_field_t px4_sensor_gyro_pb_fields[18];

/* Maximum encoded size of messages (where known) */
#define px4_sensor_gyro_pb_size                  130

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define SENSOR_GYRO_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
