/****************************************************************************
 *
 *   Copyright (c) 2016-2017 Windhover Labs, L.L.C. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Windhover Labs nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef _PX4_MSGS_H_
#define _PX4_MSGS_H_

#include "cfe.h"

#define PX4_ACTUATOR_DIRECT_MAX                  (   16 )
#define PX4_ACTUATOR_OUTPUTS_MAX                 (   16 )
#define PX4_ACTUATOR_OUTPUTS_GROUP_MAX           (    4 )
#define PX4_ADC_CHANNEL_MAX                      (    8 )
#define PX4_ESC_CONNECTED_ESC_MAX                (    8 )
#define PX4_ESTIMATOR_STATES_MAX                 (   32 )
#define PX4_ESTIMATOR_VIBE_MAX                   (    3 )
#define PX4_ESTIMATOR_COVARIANCES_MAX            (   28 )
#define PX4_FENCE_MAX_VERTICES                   (   16 )
#define PX4_GPS_DUMP_DATA_MAX                    (   79 )
#define PX4_GPS_INJECT_DATA_MAX                  (  182 )
#define PX4_RC_INPUT_MAX_CHANNELS                (   18 )
#define PX4_PWM_OUTPUT_MAX_CHANNELS              (   16 )
#define PX4_QSHELL_REQ_MAX_STRLEN                (  100 )
#define PX4_RC_PARAM_MAP_NCHAN                   (    3 )
#define PX4_RC_PARAM_MAP_ID_LEN                  (   51 )
#define PX4_SAT_INFO_MAX_SATELLITES              (   20 )
#define PX4_NUM_MOTOR_OUTPUTS                    (    8 )
#define PX4_ADSB_CALLSIGN_LEN                    (    8 )
#define PX4_OPTICAL_FLOW_FRAME_SIZE              ( 4096 )

typedef enum
{
    PX4_GPS_NONE0_FIX                            = 0,
    PX4_GPS_NONE1_FIX                            = 1,
    PX4_GPS_2D_FIX                               = 2,
    PX4_GPS_3D_FIX                               = 3,
    PX4_GPS_DGPS_FIX                             = 4,
    PX4_GPS_RTK_FIX                              = 5

} PX4_GpsFixType_t;

typedef enum
{
    PX4_DISTANCE_SENSOR_LASER                    = 0,
    PX4_DISTANCE_SENSOR_ULTRASOUND               = 1,
    PX4_DISTANCE_SENSOR_INFRARED                 = 2,
    PX4_DISTANCE_SENSOR_RADAR                    = 3
} PX4_DistanceSensorType_t;

typedef enum
{
    PX4_ACTUATOR_CONTROL_ROLL                    = 0,
    PX4_ACTUATOR_CONTROL_PITCH                   = 1,
    PX4_ACTUATOR_CONTROL_YAW                     = 2,
    PX4_ACTUATOR_CONTROL_THROTTLE                = 3,
    PX4_ACTUATOR_CONTROL_FLAPS                   = 4,
    PX4_ACTUATOR_CONTROL_SPOILERS                = 5,
    PX4_ACTUATOR_CONTROL_AIRBRAKES               = 6,
    PX4_ACTUATOR_CONTROL_LANDING_GEAR            = 7,
    PX4_ACTUATOR_CONTROL_COUNT                   = 8
} PX4_ActuatorControlIndex_t;

typedef enum
{
    PX4_ACTUATOR_CONTROL_GROUP_ATTITUDE           = 0,
    PX4_ACTUATOR_CONTROL_GROUP_ATTITUDE_ALTERNATE = 1,
    PX4_ACTUATOR_CONTROL_GROUP_COUNT              = 4
} PX4_ActuatorControlGroupIndex_t;

typedef enum
{
    PX4_BATTERY_WARNING_NONE                     = 0,
    PX4_BATTERY_WARNING_LOW                      = 1,
    PX4_BATTERY_WARNING_CRITICAL                 = 2,
    PX4_BATTERY_WARNING_EMERGENCY                = 3
} PX4_BatteryWarningSeverity_t;

typedef enum
{
    PX4_COMMANDER_MAIN_STATE_MANUAL              =  0,
    PX4_COMMANDER_MAIN_STATE_ALTCTL              =  1,
    PX4_COMMANDER_MAIN_STATE_POSCTL              =  2,
    PX4_COMMANDER_MAIN_STATE_AUTO_MISSION        =  3,
    PX4_COMMANDER_MAIN_STATE_AUTO_LOITER         =  4,
    PX4_COMMANDER_MAIN_STATE_AUTO_RTL            =  5,
    PX4_COMMANDER_MAIN_STATE_ACRO                =  6,
    PX4_COMMANDER_MAIN_STATE_OFFBOARD            =  7,
    PX4_COMMANDER_MAIN_STATE_STAB                =  8,
    PX4_COMMANDER_MAIN_STATE_RATTITUDE           =  9,
    PX4_COMMANDER_MAIN_STATE_AUTO_TAKEOFF        = 10,
    PX4_COMMANDER_MAIN_STATE_AUTO_LAND           = 11,
    PX4_COMMANDER_MAIN_STATE_AUTO_FOLLOW_TARGET  = 12,
    PX4_COMMANDER_MAIN_STATE_AUTO_PRECLAND       = 13,
    PX4_COMMANDER_MAIN_STATE_DESCEND             = 14,
    PX4_COMMANDER_MAIN_STATE_TERMINATION         = 15,
    PX4_COMMANDER_MAIN_STATE_AUTO_OFFBOARD       = 16,
    PX4_COMMANDER_MAIN_STATE_AUTO_RTGS           = 17,
    PX4_COMMANDER_MAIN_STATE_AUTO_RATTITUDE      = 18,
    PX4_COMMANDER_MAIN_STATE_AUTO_LANDENGFAIL    = 19,
    PX4_COMMANDER_MAIN_STATE_AUTO_LANDGPSFAIL    = 20,
    PX4_COMMANDER_MAIN_STATE_AUTO_RCRECOVER      = 21,
    PX4_COMMANDER_MAIN_STATE_MAX                 = 22
} PX4_CommanderMainState_t;

typedef enum
{
    PX4_AIRSPEED_MODE_MEAS                       = 0,
    PX4_AIRSPEED_MODE_EST                        = 1,
    PX4_AIRSPEED_MODE_DISABLED                   = 2
} PX4_AirspeedMode_t;

typedef enum
{
    PX4_ESC_VENDOR_GENERIC                       = 0,
    PX4_ESC_VENDOR_MIKROKOPTER                   = 1,
    PX4_ESC_VENDOR_GRAUPNER_HOTT                 = 2,
    PX4_ESC_VENDOR_TAP                           = 3
} PX4_EscVendor_t;

typedef enum
{
    PX4_ESC_CONNECTION_TYPE_PPM                  = 0,
    PX4_ESC_CONNECTION_TYPE_SERIAL               = 1,
    PX4_ESC_CONNECTION_TYPE_ONESHOOT             = 2,
    PX4_ESC_CONNECTION_TYPE_I2C                  = 3,
    PX4_ESC_CONNECTION_TYPE_CAN                  = 4
} PX4_EscConnectionType_t;

typedef enum
{
    PX4_GEOFENCE_ACTION_NONE                     = 0,
    PX4_GEOFENCE_ACTION_WARN                     = 1,
    PX4_GEOFENCE_ACTION_LOITER                   = 2,
    PX4_GEOFENCE_ACTION_RTL                      = 3,
    PX4_GEOFENCE_ACTION_TERMINATE                = 4
} PX4_GeofenceAction_t;

typedef enum
{
    PX4_MAGNETOMETER_MODE_NORMAL                 = 0,
    PX4_MAGNETOMETER_MODE_POSITIVE_BIAS          = 1,
    PX4_MAGNETOMETER_MODE_NEGATIVE_BIAS          = 2
} PX4_MagnetometerMode_t;

typedef enum
{
    PX4_RC_INPUT_SOURCE_UNKNOWN                  =  0,
    PX4_RC_INPUT_SOURCE_PX4FMU_PPM               =  1,
    PX4_RC_INPUT_SOURCE_PX4IO_PPM                =  2,
    PX4_RC_INPUT_SOURCE_PX4IO_SPEKTRUM           =  3,
    PX4_RC_INPUT_SOURCE_PX4IO_SBUS               =  4,
    PX4_RC_INPUT_SOURCE_PX4IO_ST24               =  5,
    PX4_RC_INPUT_SOURCE_MAVLINK                  =  6,
    PX4_RC_INPUT_SOURCE_QURT                     =  7,
    PX4_RC_INPUT_SOURCE_PX4FMU_SPEKTRUM          =  8,
    PX4_RC_INPUT_SOURCE_PX4FMU_SBUS              =  9,
    PX4_RC_INPUT_SOURCE_PX4FMU_ST24              = 10,
    PX4_RC_INPUT_SOURCE_PX4FMU_SUMD              = 11,
    PX4_RC_INPUT_SOURCE_PX4FMU_DSM               = 12,
    PX4_RC_INPUT_SOURCE_PX4IO_SUMD               = 13
} PX4_RcInputSource_t;

typedef enum
{
    PX4_SWITCH_POS_NONE                          = 0,
    PX4_SWITCH_POS_ON                            = 1,
    PX4_SWITCH_POS_MIDDLE                        = 2,
    PX4_SWITCH_POS_OFF                           = 3
} PX4_SwitchPos_t;

typedef enum
{
    PX4_MANUAL_CONTROL_SOURCE_RC                 = 1,
    PX4_MANUAL_CONTROL_SOURCE_MAVLINK_0          = 2,
    PX4_MANUAL_CONTROL_SOURCE_MAVLINK_1          = 3,
    PX4_MANUAL_CONTROL_SOURCE_MAVLINK_2          = 4,
    PX4_MANUAL_CONTROL_SOURCE_MAVLINK_3          = 5
} PX4_ManualControlDataSource_t;


typedef enum
{
    PX4_MODE_SLOT_NONE                           = -1,
    PX4_MODE_SLOT_1                              =  0,
    PX4_MODE_SLOT_2                              =  1,
    PX4_MODE_SLOT_3                              =  2,
    PX4_MODE_SLOT_4                              =  3,
    PX4_MODE_SLOT_5                              =  4,
    PX4_MODE_SLOT_6                              =  5,
    PX4_MODE_SLOT_MAX                            =  6
} PX4_ModeSlot_t;

typedef enum
{
    PX4_SETPOINT_TYPE_POSITION                   = 0,
    PX4_SETPOINT_TYPE_VELOCITY                   = 1,
    PX4_SETPOINT_TYPE_LOITER                     = 2,
    PX4_SETPOINT_TYPE_TAKEOFF                    = 3,
    PX4_SETPOINT_TYPE_LAND                       = 4,
    PX4_SETPOINT_TYPE_IDLE                       = 5,
    PX4_SETPOINT_TYPE_OFFBOARD                   = 6,
    PX4_SETPOINT_TYPE_FOLLOW_TARGET              = 7
} PX4_SetpointType_t;

typedef enum
{
    PX4_VELOCITY_FRAME_LOCAL_NED                 = 1,
    PX4_VELOCITY_FRAME_BODY_NED                  = 8
} PX4_VelocityFrameType_t;

typedef enum
{
    PX4_RC_CHANNELS_FUNCTION_THROTTLE            =  0,
    PX4_RC_CHANNELS_FUNCTION_ROLL                =  1,
    PX4_RC_CHANNELS_FUNCTION_PITCH               =  2,
    PX4_RC_CHANNELS_FUNCTION_YAW                 =  3,
    PX4_RC_CHANNELS_FUNCTION_MODE                =  4,
    PX4_RC_CHANNELS_FUNCTION_RETURN              =  5,
    PX4_RC_CHANNELS_FUNCTION_POSCTL              =  6,
    PX4_RC_CHANNELS_FUNCTION_LOITER              =  7,
    PX4_RC_CHANNELS_FUNCTION_OFFBOARD            =  8,
    PX4_RC_CHANNELS_FUNCTION_ACRO                =  9,
    PX4_RC_CHANNELS_FUNCTION_FLAPS               = 10,
    PX4_RC_CHANNELS_FUNCTION_AUX_1               = 11,
    PX4_RC_CHANNELS_FUNCTION_AUX_2               = 12,
    PX4_RC_CHANNELS_FUNCTION_AUX_3               = 13,
    PX4_RC_CHANNELS_FUNCTION_AUX_4               = 14,
    PX4_RC_CHANNELS_FUNCTION_AUX_5               = 15,
    PX4_RC_CHANNELS_FUNCTION_PARAM_1             = 16,
    PX4_RC_CHANNELS_FUNCTION_PARAM_2             = 17,
    PX4_RC_CHANNELS_FUNCTION_PARAM_3             = 18,
    PX4_RC_CHANNELS_FUNCTION_RATTITUDE           = 19,
    PX4_RC_CHANNELS_FUNCTION_KILLSWITCH          = 20,
    PX4_RC_CHANNELS_FUNCTION_TRANSITION          = 21,
    PX4_RC_CHANNELS_FUNCTION_GEAR                = 22,
    PX4_RC_CHANNELS_FUNCTION_ARMSWITCH           = 23,
    PX4_RC_CHANNELS_FUNCTION_STAB                = 24,
    PX4_RC_CHANNELS_FUNCTION_MAN                 = 25,
    PX4_RC_CHANNELS_FUNCTION_ALTCTL              = 26,
    PX4_RC_CHANNELS_FUNCTION_COUNT               = 27
} PX4_RcChannelFunction_t;

typedef enum
{
    PX4_SUBSYSTEM_TYPE_GYRO                      = 0b00000000000000000001,  /*      1 */
    PX4_SUBSYSTEM_TYPE_ACC                       = 0b00000000000000000010,  /*      2 */
    PX4_SUBSYSTEM_TYPE_MAG                       = 0b00000000000000000100,  /*      4 */
    PX4_SUBSYSTEM_TYPE_ABSPRESSURE               = 0b00000000000000001000,  /*      8 */
    PX4_SUBSYSTEM_TYPE_DIFFPRESSURE              = 0b00000000000000010000,  /*     16 */
    PX4_SUBSYSTEM_TYPE_GPS                       = 0b00000000000000100000,  /*     32 */
    PX4_SUBSYSTEM_TYPE_OPTICALFLOW               = 0b00000000000001000000,  /*     64 */
    PX4_SUBSYSTEM_TYPE_CVPOSITION                = 0b00000000000010000000,  /*    128 */
    PX4_SUBSYSTEM_TYPE_LASERPOSITION             = 0b00000000000100000000,  /*    256 */
    PX4_SUBSYSTEM_TYPE_EXTERNALGROUNDTRUTH       = 0b00000000001000000000,  /*    512 */
    PX4_SUBSYSTEM_TYPE_ANGULARRATECONTROL        = 0b00000000010000000000,  /*   1024 */
    PX4_SUBSYSTEM_TYPE_ATTITUDESTABILIZATION     = 0b00000000100000000000,  /*   2048 */
    PX4_SUBSYSTEM_TYPE_YAWPOSITION               = 0b00000001000000000000,  /*   4096 */
    /* Skip 8192 */
    PX4_SUBSYSTEM_TYPE_ALTITUDECONTROL           = 0b00000100000000000000,  /*  16384 */
    PX4_SUBSYSTEM_TYPE_POSITIONCONTROL           = 0b00001000000000000000,  /*  32768 */
    PX4_SUBSYSTEM_TYPE_MOTORCONTROL              = 0b00010000000000000000,  /*  65536 */
    PX4_SUBSYSTEM_TYPE_RANGEFINDER               = 0b00100000000000000000   /* 131072 */
} PX4_SubsystemType_t;

typedef enum
{
    PX4_TECS_MODE_NORMAL                         = 0,
    PX4_TECS_MODE_UNDERSPEED                     = 1,
    PX4_TECS_MODE_TAKEOFF                        = 2,
    PX4_TECS_MODE_LAND                           = 3,
    PX4_TECS_MODE_LAND_THROTTLELIM               = 4,
    PX4_TECS_MODE_BAD_DESCENT                    = 5,
    PX4_TECS_MODE_CLIMBOUT                       = 6
} PX4_TecsMode_t;

typedef enum
{
    PX4_TELEMETRY_STATUS_RADIO_TYPE_GENERIC         = 0,
    PX4_TELEMETRY_STATUS_RADIO_TYPE_3DR_RADIO       = 1,
    PX4_TELEMETRY_STATUS_RADIO_TYPE_UBIQUITY_BULLET = 2,
    PX4_TELEMETRY_STATUS_RADIO_TYPE_WIRE            = 3,
    PX4_TELEMETRY_STATUS_RADIO_TYPE_USB             = 4
} PX4_TelemetryStatusRadioType_t;

typedef enum
{
    PX4_ADSB_ALTITUDE_TYPE_PRESSURE_QNH          = 0,
    PX4_ADSB_ALTITUDE_TYPE_GEOMETRIC             = 1
} PX4_AdsbAltitudeType_t;

typedef enum
{
    PX4_ADSB_EMITTER_TYPE_NO_INFO                =  0,
    PX4_ADSB_EMITTER_TYPE_LIGHT                  =  1,
    PX4_ADSB_EMITTER_TYPE_SMALL                  =  2,
    PX4_ADSB_EMITTER_TYPE_LARGE                  =  3,
    PX4_ADSB_EMITTER_TYPE_HIGH_VORTEX_LARGE      =  4,
    PX4_ADSB_EMITTER_TYPE_HEAVY                  =  5,
    PX4_ADSB_EMITTER_TYPE_HIGHLY_MANUV           =  6,
    PX4_ADSB_EMITTER_TYPE_ROTOCRAFT              =  7,
    PX4_ADSB_EMITTER_TYPE_UNASSIGNED             =  8,
    PX4_ADSB_EMITTER_TYPE_GLIDER                 =  9,
    PX4_ADSB_EMITTER_TYPE_LIGHTER_AIR            = 10,
    PX4_ADSB_EMITTER_TYPE_PARACHUTE              = 11,
    PX4_ADSB_EMITTER_TYPE_ULTRA_LIGHT            = 12,
    PX4_ADSB_EMITTER_TYPE_UNASSIGNED2            = 13,
    PX4_ADSB_EMITTER_TYPE_UAV                    = 14,
    PX4_ADSB_EMITTER_TYPE_SPACE                  = 15,
    PX4_ADSB_EMITTER_TYPE_UNASSGINED3            = 16,
    PX4_ADSB_EMITTER_TYPE_EMERGENCY_SURFACE      = 17,
    PX4_ADSB_EMITTER_TYPE_SERVICE_SURFACE        = 18,
    PX4_ADSB_EMITTER_TYPE_POINT_OBSTACLE         = 19
} PX4_AdsbEmitterType_t;

typedef enum
{
    PX4_ADSB_FLAGS_VALID_COORDS                  = 0b00000001,  /*  1 */
    PX4_ADSB_FLAGS_VALID_ALTITUDE                = 0b00000010,  /*  2 */
    PX4_ADSB_FLAGS_VALID_HEADING                 = 0b00000100,  /*  4 */
    PX4_ADSB_FLAGS_VALID_VELOCITY                = 0b00001000,  /*  8 */
    PX4_ADSB_FLAGS_VALID_CALLSIGN                = 0b00010000,  /* 16 */
    PX4_ADSB_FLAGS_VALID_SQUAWK                  = 0b00100000,  /* 32 */
    PX4_ADSB_FLAGS_SIMULATED                     = 0b01000000   /* 64 */
} PX4_AdsbFlags_t;

typedef enum
{
    PX4_VEHICLE_COMMAND_RESULT_ACCEPTED             = 0,
    PX4_VEHICLE_COMMAND_RESULT_TEMPORARILY_REJECTED = 1,
    PX4_VEHICLE_COMMAND_RESULT_DENIED               = 2,
    PX4_VEHICLE_COMMAND_RESULT_UNSUPPORTED          = 3,
    PX4_VEHICLE_COMMAND_RESULT_FAILED               = 4
} PX4_VehicleCommandResult_t;

typedef enum
{
    PX4_VEHICLE_CMD_CUSTOM_0                     =     0,
    PX4_VEHICLE_CMD_CUSTOM_1                     =     1,
    PX4_VEHICLE_CMD_CUSTOM_2                     =     2,
    PX4_VEHICLE_CMD_NAV_WAYPOINT                 =    16,
    PX4_VEHICLE_CMD_NAV_LOITER_UNLIM             =    17,
    PX4_VEHICLE_CMD_NAV_LOITER_TURNS             =    18,
    PX4_VEHICLE_CMD_NAV_LOITER_TIME              =    19,
    PX4_VEHICLE_CMD_NAV_RETURN_TO_LAUNCH         =    20,
    PX4_VEHICLE_CMD_NAV_LAND                     =    21,
    PX4_VEHICLE_CMD_NAV_TAKEOFF                  =    22,
    PX4_VEHICLE_CMD_NAV_LOITER_TO_ALT            =    31,
    PX4_VEHICLE_CMD_NAV_ROI                      =    80,
    PX4_VEHICLE_CMD_NAV_PATHPLANNING             =    81,
    PX4_VEHICLE_CMD_NAV_VTOL_TAKEOFF             =    84,
    PX4_VEHICLE_CMD_NAV_VTOL_LAND                =    85,
    PX4_VEHICLE_CMD_NAV_GUIDED_LIMITS            =    90,
    PX4_VEHICLE_CMD_NAV_GUIDED_MASTER            =    91,
    PX4_VEHICLE_CMD_NAV_GUIDED_ENABLE            =    92,
    PX4_VEHICLE_CMD_NAV_LAST                     =    95,
    PX4_VEHICLE_CMD_CONDITION_DELAY              =   112,
    PX4_VEHICLE_CMD_CONDITION_CHANGE_ALT         =   113,
    PX4_VEHICLE_CMD_CONDITION_DISTANCE           =   114,
    PX4_VEHICLE_CMD_CONDITION_YAW                =   115,
    PX4_VEHICLE_CMD_CONDITION_LAST               =   159,
    PX4_VEHICLE_CMD_DO_SET_MODE                  =   176,
    PX4_VEHICLE_CMD_DO_JUMP                      =   177,
    PX4_VEHICLE_CMD_DO_CHANGE_SPEED              =   178,
    PX4_VEHICLE_CMD_DO_SET_HOME                  =   179,
    PX4_VEHICLE_CMD_DO_SET_PARAMETER             =   180,
    PX4_VEHICLE_CMD_DO_SET_RELAY                 =   181,
    PX4_VEHICLE_CMD_DO_REPEAT_RELAY              =   182,
    PX4_VEHICLE_CMD_DO_SET_SERVO                 =   183,
    PX4_VEHICLE_CMD_DO_REPEAT_SERVO              =   184,
    PX4_VEHICLE_CMD_DO_FLIGHTTERMINATION         =   185,
    PX4_VEHICLE_CMD_DO_GO_AROUND                 =   191,
    PX4_VEHICLE_CMD_DO_REPOSITION                =   192,
    PX4_VEHICLE_CMD_DO_PAUSE_CONTINUE            =   193,
    PX4_VEHICLE_CMD_DO_CONTROL_VIDEO             =   200,
    PX4_VEHICLE_CMD_DO_DIGICAM_CONTROL           =   203,
    PX4_VEHICLE_CMD_DO_MOUNT_CONFIGURE           =   204,
    PX4_VEHICLE_CMD_DO_MOUNT_CONTROL             =   205,
    PX4_VEHICLE_CMD_DO_SET_CAM_TRIGG_DIST        =   206,
    PX4_VEHICLE_CMD_DO_FENCE_ENABLE              =   207,
    PX4_VEHICLE_CMD_DO_PARACHUTE                 =   208,
    PX4_VEHICLE_CMD_DO_INVERTED_FLIGHT           =   210,
    PX4_VEHICLE_CMD_DO_MOUNT_CONTROL_QUAT        =   220,
    PX4_VEHICLE_CMD_DO_GUIDED_MASTER             =   221,
    PX4_VEHICLE_CMD_DO_GUIDED_LIMITS             =   222,
    PX4_VEHICLE_CMD_DO_LAST                      =   240,
    PX4_VEHICLE_CMD_PREFLIGHT_CALIBRATION        =   241,
    PX4_VEHICLE_CMD_PREFLIGHT_SET_SENSOR_OFFSETS =   242,
    PX4_VEHICLE_CMD_PREFLIGHT_UAVCAN             =   243,
    PX4_VEHICLE_CMD_PREFLIGHT_STORAGE            =   245,
    PX4_VEHICLE_CMD_PREFLIGHT_REBOOT_SHUTDOWN    =   246,
    PX4_VEHICLE_CMD_OVERRIDE_GOTO                =   252,
    PX4_VEHICLE_CMD_MISSION_START                =   300,
    PX4_VEHICLE_CMD_COMPONENT_ARM_DISARM         =   400,
    PX4_VEHICLE_CMD_START_RX_PAIR                =   500,
    PX4_VEHICLE_CMD_DO_TRIGGER_CONTROL           =  2003,
    PX4_VEHICLE_CMD_DO_VTOL_TRANSITION           =  3000,
    PX4_VEHICLE_CMD_PAYLOAD_PREPARE_DEPLOY       = 30001,
    PX4_VEHICLE_CMD_PAYLOAD_CONTROL_DEPLOY       = 30002
} PX4_VehicleCmd_t;

typedef enum
{
    PX4_ARMING_STATE_INIT                        = 0,
    PX4_ARMING_STATE_STANDBY                     = 1,
    PX4_ARMING_STATE_ARMED                       = 2,
    PX4_ARMING_STATE_ARMED_ERROR                 = 3,
    PX4_ARMING_STATE_STANDBY_ERROR               = 4,
    PX4_ARMING_STATE_REBOOT                      = 5,
    PX4_ARMING_STATE_IN_AIR_RESTORE              = 6,
    PX4_ARMING_STATE_MAX                         = 7
} PX4_ArmingState_t;

typedef enum
{
    PX4_HIL_STATE_OFF                            = 0,
    PX4_HIL_STATE_ON                             = 1
} PX4_HilState_t;

typedef enum
{
    PX4_NAVIGATION_STATE_MANUAL                  =  0,
    PX4_NAVIGATION_STATE_ALTCTL                  =  1,
    PX4_NAVIGATION_STATE_POSCTL                  =  2,
    PX4_NAVIGATION_STATE_AUTO_MISSION            =  3,
    PX4_NAVIGATION_STATE_AUTO_LOITER             =  4,
    PX4_NAVIGATION_STATE_AUTO_RTL                =  5,
    PX4_NAVIGATION_STATE_AUTO_RCRECOVER          =  6,
    PX4_NAVIGATION_STATE_AUTO_RTGS               =  7,
    PX4_NAVIGATION_STATE_AUTO_LANDENGFAIL        =  8,
    PX4_NAVIGATION_STATE_AUTO_LANDGPSFAIL        =  9,
    PX4_NAVIGATION_STATE_ACRO                    = 10,
    PX4_NAVIGATION_STATE_UNUSED                  = 11,
    PX4_NAVIGATION_STATE_DESCEND                 = 12,
    PX4_NAVIGATION_STATE_TERMINATION             = 13,
    PX4_NAVIGATION_STATE_OFFBOARD                = 14,
    PX4_NAVIGATION_STATE_STAB                    = 15,
    PX4_NAVIGATION_STATE_RATTITUDE               = 16,
    PX4_NAVIGATION_STATE_AUTO_TAKEOFF            = 17,
    PX4_NAVIGATION_STATE_AUTO_LAND               = 18,
    PX4_NAVIGATION_STATE_AUTO_FOLLOW_TARGET      = 19,
    PX4_NAVIGATION_STATE_AUTO_PRECLAND           = 20,
    PX4_NAVIGATION_STATE_MAX                     = 21
} PX4_NavigationState_t;

typedef enum
{
    PX4_RC_IN_MODE_DEFAULT                       = 0,
    PX4_RC_IN_MODE_OFF                           = 1,
    PX4_RC_IN_MODE_GENERATED                     = 2
} PX4_RcInMode_t;

typedef enum
{
    PX4_VEHICLE_MOUNT_MODE_RETRACT               = 0,
    PX4_VEHICLE_MOUNT_MODE_NEUTRAL               = 1,
    PX4_VEHICLE_MOUNT_MODE_MAVLINK_TARGETING     = 2,
    PX4_VEHICLE_MOUNT_MODE_RC_TARGETING          = 3,
    PX4_VEHICLE_MOUNT_MODE_GPS_POINT             = 4,
    PX4_VEHICLE_MOUNT_MODE_ENUM_END              = 5
} PX4_VehicleMountMode_t;

typedef enum
{
    PX4_SYSTEM_TYPE_GENERIC                      =  0,
    PX4_SYSTEM_TYPE_FIXED_WING                   =  1,
    PX4_SYSTEM_TYPE_QUADROTOR                    =  2,
    PX4_SYSTEM_TYPE_COAXIAL                      =  3,
    PX4_SYSTEM_TYPE_HELICOPTER                   =  4,
    PX4_SYSTEM_TYPE_ANTENNA_TRACKER              =  5,
    PX4_SYSTEM_TYPE_GCS                          =  6,
    PX4_SYSTEM_TYPE_AIRSHIP                      =  7,
    PX4_SYSTEM_TYPE_FREE_BALLOON                 =  8,
    PX4_SYSTEM_TYPE_ROCKET                       =  9,
    PX4_SYSTEM_TYPE_GROUND_ROVER                 = 10,
    PX4_SYSTEM_TYPE_SURFACE_BOAT                 = 11,
    PX4_SYSTEM_TYPE_SUBMARINE                    = 12,
    PX4_SYSTEM_TYPE_HEXAROTOR                    = 13,
    PX4_SYSTEM_TYPE_OCTOROTOR                    = 14,
    PX4_SYSTEM_TYPE_TRICOPTER                    = 15,
    PX4_SYSTEM_TYPE_FLAPPING_WING                = 16,
    PX4_SYSTEM_TYPE_KITE                         = 17,
    PX4_SYSTEM_TYPE_ONBOARD_CONTROLLER           = 18,
    PX4_SYSTEM_TYPE_VTOL_DUOROTOR                = 19,
    PX4_SYSTEM_TYPE_VTOL_QUADROTOR               = 20,
    PX4_SYSTEM_TYPE_VTOL_TILTROTOR               = 21,
    PX4_SYSTEM_TYPE_VTOL_RESERVED2               = 22,
    PX4_SYSTEM_TYPE_VTOL_RESERVED3               = 23,
    PX4_SYSTEM_TYPE_VTOL_RESERVED4               = 24,
    PX4_SYSTEM_TYPE_VTOL_RESERVED5               = 25,
    PX4_SYSTEM_TYPE_GIMBAL                       = 26,
    PX4_SYSTEM_TYPE_ADSB                         = 27
} PX4_SystemType_t;

typedef enum
{
    PX4_VEHICLE_VTOL_STATE_UNDEFINED             = 0,
    PX4_VEHICLE_VTOL_STATE_TRANSITION_TO_FW      = 1,
    PX4_VEHICLE_VTOL_STATE_TRANSITION_TO_MC      = 2,
    PX4_VEHICLE_VTOL_STATE_MC                    = 3,
    PX4_VEHICLE_VTOL_STATE_FW                    = 4
} PX4_VehicleVtolState_t;


typedef enum
{
    PX4_SENSOR_ORIENTATION_NONE                       =  0,
    PX4_SENSOR_ORIENTATION_YAW_45                     =  1,
    PX4_SENSOR_ORIENTATION_YAW_90                     =  2,
    PX4_SENSOR_ORIENTATION_YAW_135                    =  3,
    PX4_SENSOR_ORIENTATION_YAW_180                    =  4,
    PX4_SENSOR_ORIENTATION_YAW_225                    =  5,
    PX4_SENSOR_ORIENTATION_YAW_270                    =  6,
    PX4_SENSOR_ORIENTATION_YAW_315                    =  7,
    PX4_SENSOR_ORIENTATION_ROLL_180                   =  8,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_45            =  9,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_90            = 10,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_135           = 11,
    PX4_SENSOR_ORIENTATION_PITCH_180                  = 12,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_225           = 13,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_270           = 14,
    PX4_SENSOR_ORIENTATION_ROLL_180_YAW_315           = 15,
    PX4_SENSOR_ORIENTATION_ROLL_90                    = 16,
    PX4_SENSOR_ORIENTATION_ROLL_90_YAW_45             = 17,
    PX4_SENSOR_ORIENTATION_ROLL_90_YAW_90             = 18,
    PX4_SENSOR_ORIENTATION_ROLL_90_YAW_135            = 19,
    PX4_SENSOR_ORIENTATION_ROLL_270                   = 20,
    PX4_SENSOR_ORIENTATION_ROLL_270_YAW_45            = 21,
    PX4_SENSOR_ORIENTATION_ROLL_270_YAW_90            = 22,
    PX4_SENSOR_ORIENTATION_ROLL_270_YAW_135           = 23,
    PX4_SENSOR_ORIENTATION_PITCH_90                   = 24,
    PX4_SENSOR_ORIENTATION_PITCH_270                  = 25,
    PX4_SENSOR_ORIENTATION_PITCH_180_YAW_90           = 26,
    PX4_SENSOR_ORIENTATION_PITCH_180_YAW_270          = 27,
    PX4_SENSOR_ORIENTATION_ROLL_90_PITCH_90           = 28,
    PX4_SENSOR_ORIENTATION_ROLL_180_PITCH_90          = 29,
    PX4_SENSOR_ORIENTATION_ROLL_270_PITCH_90          = 30,
    PX4_SENSOR_ORIENTATION_ROLL_90_PITCH_180          = 31,
    PX4_SENSOR_ORIENTATION_ROLL_270_PITCH_180         = 32,
    PX4_SENSOR_ORIENTATION_ROLL_90_PITCH_270          = 33,
    PX4_SENSOR_ORIENTATION_ROLL_180_PITCH_270         = 34,
    PX4_SENSOR_ORIENTATION_ROLL_270_PITCH_270         = 35,
    PX4_SENSOR_ORIENTATION_ROLL_90_PITCH_180_YAW_90   = 36,
    PX4_SENSOR_ORIENTATION_ROLL_90_YAW_270            = 37,
    PX4_SENSOR_ORIENTATION_ROLL_315_PITCH_315_YAW_315 = 39
} PX4_SensorOrientation_t;


typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    boolean Armed;
    boolean Prearmed;
    boolean ReadyToArm;
    boolean Lockdown;
    boolean ManualLockdown;
    boolean ForceFailsafe;
    boolean InEscCalibrationMode;
} PX4_ActuatorArmedMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 SampleTime;
    float  Control[PX4_ACTUATOR_CONTROL_COUNT];
} PX4_ActuatorControlsMsg_t;


typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint32 Count;
    float  Output[PX4_ACTUATOR_OUTPUTS_MAX];
} PX4_ActuatorOutputsMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  IndicatedAirspeed;		/* m/s */
    float  TrueAirspeed;				/* m/s */
    float  TrueAirspeedUnfiltered;	/* m/s */
    float  AirTemperature;			/* Celsius */
    float  Confidence;
} PX4_AirspeedMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    float   Voltage;					/* V */
    float   VoltageFiltered;			/* V */
    float   Current;					/* A */
    float   CurrentFiltered;			/* A */
    float   Discharged;				/* mAh */
    float   Remaining;
    float   Scale;
    int32   CellCount;
    boolean Connected;
    PX4_BatteryWarningSeverity_t Warning;
} PX4_BatteryStatusMsg_t;


typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    PX4_CommanderMainState_t MainState;
} PX4_CommanderStateMsg_t;


typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    float   AccX;
    float   AccY;
    float   AccZ;
    float   VelX;
    float   VelY;
    float   VelZ;
    float   PosX;
    float   PosY;
    float   PosZ;
    float   Airspeed;
    float   VelVariance[3];
    float   PosVariance[3];
    float   Q[4];
    float   DeltaQReset[4];
    float   RollRate;
    float   PitchRate;
    float   YawRate;
    float   HorzAccMag;
    float   RollRateBias;
    float   PitchRateBias;
    float   YawRateBias;
    boolean AirspeedValid;
    uint8   QuatResetCounter;
} PX4_ControlStateMsg_t;


typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 ErrorCount;
    float DifferentialPressureRaw;
    float DifferentialPressureFiltered;
    float Temperature;
} PX4_DifferentialPressureMsg_t;


typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float MinDistance;
    float MaxDistance;
    float CurrentDistance;
    float Covariance;
    PX4_DistanceSensorType_t Type : 8;
    uint8 ID;
    PX4_SensorOrientation_t  Orientation : 8;
} PX4_DistanceSensorMsg_t;


typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  States[PX4_ESTIMATOR_STATES_MAX];
    uint32 NumStates;
    float  Vibe[PX4_ESTIMATOR_VIBE_MAX];
    float  Covariances[PX4_ESTIMATOR_COVARIANCES_MAX];
    uint16 GpsCheckFailFlags;
    uint16 ControlModeFlags;
    uint16 FilterFaultFlags;
    uint8  NanFlags;
    uint8  HealthFlags;
    uint8  TimeoutFlags;
} PX4_EstimatorStatusMsg_t;

typedef struct
{
	float Lat;
	float Lon;
} PX4_FenceVertex_t;

typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint8 Len;
    uint8 Flags;
    char  Data[PX4_GPS_INJECT_DATA_MAX];
} PX4_GpsInjectDataMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    double Lat;
    double Lon;
    float  Alt;
    float  X;
    float  Y;
    float  Z;
    float  Yaw;
    float  DirectionX;
    float  DirectionY;
    float  DirectionZ;
} PX4_HomePositionMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint64  LastSignal;
    uint32  ChannelCount;
    int32   RSSI;
    uint16  RcLostFrameCount;
    uint16  RcTotalFrameCount;
    uint16  RcPpmFrameLength;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];
    boolean RcFailsafe;
    boolean RcLost;
    PX4_RcInputSource_t InputSource;
} PX4_InputRcMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint8  Severity;
    char   Text[127];
} PX4_LogMessageMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  X;
    float  Y;
    float  Z;
    float  R;
    float  Flaps;
    float  Aux1;
    float  Aux2;
    float  Aux3;
    float  Aux4;
    float  Aux5;
    PX4_SwitchPos_t ModeSwitch;
    PX4_SwitchPos_t ReturnSwitch;
    PX4_SwitchPos_t RattitudeSwitch;
    PX4_SwitchPos_t PosctlSwitch;
    PX4_SwitchPos_t LoiterSwitch;
    PX4_SwitchPos_t AcroSwitch;
    PX4_SwitchPos_t OffboardSwitch;
    PX4_SwitchPos_t KillSwitch;
    PX4_SwitchPos_t TransitionSwitch;
    PX4_SwitchPos_t GearSwitch;
    PX4_SwitchPos_t ArmSwitch;
    PX4_SwitchPos_t StabSwitch;
    PX4_SwitchPos_t ManSwitch;
    PX4_ModeSlot_t ModeSlot;
    PX4_ManualControlDataSource_t DataSource;
    PX4_SwitchPos_t AltctlSwitch;
} PX4_ManualControlSetpointMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  RollRateInteg;
    float  PitchRateInteg;
    float  YawRateInteg;
} PX4_McAttCtrlStatusMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    int32  DatamanID;
    uint32 Count;
    int32  CurrentSeq;
} PX4_MissionMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint32  InstanceCount;
    uint32  SeqReached;
    uint32  SeqCurrent;
    uint32  SeqTotal;
    uint32  ItemChangedIndex;
    uint32  ItemDoJumpRemaining;
    boolean Valid;
    boolean Warning;
    boolean Reached;
    boolean Finished;
    boolean Failure;
    boolean StayInFailsafe;
    boolean FlightTermination;
    boolean ItemDoJumpChanged;
} PX4_MissionResultMsg_t;


typedef union
{
    struct
    {
        uint16 MotorPos	 : 1; // 0 - true when any motor has saturated in the positive direction
        uint16 MotorNeg	 : 1; // 1 - true when any motor has saturated in the negative direction
        uint16 RollPos	 : 1; // 2 - true when a positive roll demand change will increase saturation
        uint16 RollNeg	 : 1; // 3 - true when a negative roll demand change will increase saturation
        uint16 PitchPos	 : 1; // 4 - true when a positive pitch demand change will increase saturation
        uint16 PitchNeg	 : 1; // 5 - true when a negative pitch demand change will increase saturation
        uint16 YawPos	 : 1; // 6 - true when a positive yaw demand change will increase saturation
        uint16 YawNeg	 : 1; // 7 - true when a negative yaw demand change will increase saturation
        uint16 ThrustPos : 1; // 8 - true when a positive thrust demand change will increase saturation
        uint16 ThrustNeg : 1; // 9 - true when a negative thrust demand change will increase saturation
    } Flags;
    uint16 Value;
} PX4_SaturationStatus_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    PX4_SaturationStatus_t SaturationStatus;
} PX4_MultirotorMotorLimitsMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  PixelFlowXIntegral;
    float  PixelFlowYIntegral;
    float  GyroXRateIntegral;
    float  GyroYRateIntegral;
    float  GyroZRateIntegral;
    float  GroundDistance;
    uint32 IntegrationTimespan;
    uint32 TimeSinceLastSonarUpdate;
    uint16 FrameCountSinceLastReadout;
    int16  GyroTemperature;
    uint8  SensorID;
    uint8  Quality;
} PX4_OpticalFlowMsg_t;

typedef struct
{
    uint64 Timestamp;
    double  Lat;
    double  Lon;
    float   X;
    float   Y;
    float   Z;
    float   VX;
    float   VY;
    float   VZ;
    float   Alt;
    float   Yaw;
    float   Yawspeed;
    float   LoiterRadius;
    float   PitchMin;
    float   AX;
    float   AY;
    float   AZ;
    float   AcceptanceRadius;
    float   CruisingSpeed;
    float   CruisingThrottle;
    boolean Valid;
    PX4_SetpointType_t Type;
    boolean PositionValid;
    boolean VelocityValid;
    uint8   VelocityFrame;
    boolean AltValid;
    boolean YawValid;
    boolean DisableMcYawControl;
    boolean YawspeedValid;
    int8    LoiterDirection;
    boolean AccelerationValid;
    boolean AccelerationIsForce;
} PX4_PositionSetpoint_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    PX4_PositionSetpoint_t Previous;
    PX4_PositionSetpoint_t Current;
    PX4_PositionSetpoint_t Next;
} PX4_PositionSetpointTripletMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint64  TimestampLastValid;
    float   Channels[PX4_RC_INPUT_MAX_CHANNELS];
    uint32  FrameDropCount;
    uint8   ChannelCount;
    PX4_RcChannelFunction_t Function[PX4_RC_CHANNELS_FUNCTION_COUNT];
    uint8   RSSI;
    boolean SignalLost;
} PX4_RcChannelsMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    boolean SafetySwitchAvailable;
    boolean SafetyOff;
} PX4_SafetyMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint8  Count;
    uint8  SVID[PX4_SAT_INFO_MAX_SATELLITES];
    uint8  Used[PX4_SAT_INFO_MAX_SATELLITES];
    uint8  Elevation[PX4_SAT_INFO_MAX_SATELLITES];
    uint8  Azimuth[PX4_SAT_INFO_MAX_SATELLITES];
    uint8  SNR[PX4_SAT_INFO_MAX_SATELLITES];
} PX4_SatelliteInfoMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 IntegralDt;
    uint64 ErrorCount;
    float  X;
    float  Y;
    float  Z;
    float  XIntegral;
    float  YIntegral;
    float  ZIntegral;
    float  Temperature;
    float  Range_m_s2;
    float  Scaling;
    uint32 DeviceID;
    int16  XRaw;
    int16  YRaw;
    int16  ZRaw;
    int16  TemperatureRaw;
} PX4_SensorAccelMsg_t;

typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint32 ErrorCount;
    float Pressure;
    float Altitude;
    float Temperature;
} PX4_SensorBaroMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    float   GyroRad[3];
    float   GyroIntegralDt;
    uint64  AccTimestamp;
    boolean AccInvalid;
    float   Acc[3];
    float   AccIntegralDt;
    uint64  MagTimestamp;
    boolean MagInvalid;
    float   Mag[3];
    uint64  BaroTimestamp;
    boolean BaroInvalid;
    float   BaroAlt;
    float   BaroTemp;
} PX4_SensorCombinedMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 IntegralDt;
    uint64 ErrorCount;
    float  X;
    float  Y;
    float  Z;
    float  XIntegral;
    float  YIntegral;
    float  ZIntegral;
    float  Temperature;
    float  Range;
    float  Scaling;
    uint32 DeviceID;
    int16  XRaw;
    int16  YRaw;
    int16  ZRaw;
    int16  TemperatureRaw;
} PX4_SensorGyroMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 ErrorCount;
    float  X;
    float  Y;
    float  Z;
    float  Range;
    float  Scaling;
    float  Temperature;
    uint32 DeviceID;
    int16  XRaw;
    int16  YRaw;
    int16  ZRaw;
} PX4_SensorMagMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    PX4_SubsystemType_t SubsystemType;
    boolean Present;
    boolean Enabled;
    boolean Ok;
} PX4_SubsystemInfoMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint64 HeartbeatTime;
    uint64 TelemTime;
    uint16 RxErrors;
    uint16 Fixed;
    PX4_TelemetryStatusRadioType_t Type;
    uint8  RSSI;
    uint8  RemoteRSSI;
    uint8  Noise;
    uint8  RemoteNoise;
    uint8  TxBuf;
    uint8  SystemID;
    uint8  ComponentID;
} PX4_TelemetryStatusMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  RollSpeed;
    float  PitchSpeed;
    float  YawSpeed;
    float  Q[4];
} PX4_VehicleAttitudeMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    float   RollBody;
    float   PitchBody;
    float   YawBody;
    float   YawSpMoveRate;
    float   Q_D[4];
    boolean Q_D_Valid;
    float   Thrust;
    boolean RollResetIntegral;
    boolean PitchResetIntegral;
    boolean YawResetIntegral;
    boolean FwControlYaw;
    boolean DisableMcYawControl;
    boolean ApplyFlaps;
    float   LandingGear;
} PX4_VehicleAttitudeSetpointMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint16 Command;
    PX4_VehicleCommandResult_t Result;
} PX4_VehicleCommandAckMsg_t;


typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    double Param5;
    double Param6;
    float  Param1;
    float  Param2;
    float  Param3;
    float  Param4;
    float  Param7;
    PX4_VehicleCmd_t Command;
    uint32 TargetSystem;
    uint32 TargetComponent;
    uint32 SourceSystem;
    uint32 SourceComponent;
    uint8  Confirmation;
} PX4_VehicleCommandMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    boolean Armed;
    boolean ExternalManualOverrideOk;
    boolean SystemHilEnabled;
    boolean ControlManualEnabled;
    boolean ControlAutoEnabled;
    boolean ControlOffboardEnabled;
    boolean ControlRatesEnabled;
    boolean ControlAttitudeEnabled;
    boolean ControlRattitudeEnabled;
    boolean ControlForceEnabled;
    boolean ControlAccelerationEnabled;
    boolean ControlVelocityEnabled;
    boolean ControlPositionEnabled;
    boolean ControlAltitudeEnabled;
    boolean ControlClimbRateEnabled;
    boolean ControlTerminationEnabled;
    boolean ControlFixedHdgEnabled;
} PX4_VehicleControlModeMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint64  TimeUtcUsec;
    double  Lat;
    double  Lon;
    float   Alt;
    double  DeltaLatLon[2];
    float   DeltaAlt;
    uint8   LatLonResetCounter;
    uint8   AltResetCounter;
    float   VelN;
    float   VelE;
    float   VelD;
    float   Yaw;
    float   EpH;
    float   EpV;
    float   EvH;
    float   EvV;
    float   TerrainAlt;
    float   PressureAlt;
    boolean TerrainAltValid;
    boolean DeadReckoning;
} PX4_VehicleGlobalPositionMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  VX;
    float  VY;
    float  VZ;
} PX4_VehicleGlobalVelocitySetpointMsg_t;


typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint64  TimeUtcUsec;
    int32   Lat;
    int32   Lon;
    int32   Alt;
    int32   AltEllipsoid;
    float   SVariance;
    float   CVariance;
    float   EpH;
    float   EpV;
    float   HDOP;
    float   VDOP;
    int32   NoisePerMs;
    int32   JammingIndicator;
    float   Vel_m_s;
    float   Vel_n_m_s;
    float   Vel_e_m_s;
    float   Vel_d_m_s;
    float   COG;
    int32   TimestampTimeRelative;
    PX4_GpsFixType_t FixType;
    boolean VelNedValid;
    uint8   SatellitesUsed;
} PX4_VehicleGpsPositionMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    float   AltMax;
    boolean Landed;
    boolean Freefall;
    boolean GroundContact;
} PX4_VehicleLandDetectedMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint64  RefTimestamp;
    double  RefLat;
    double  RefLon;
    uint64  SurfaceBottomTimestamp;
    float   X;
    float   Y;
    float   Z;
    float   Delta_XY[2];
    float   Delta_Z;
    float   VX;
    float   VY;
    float   VZ;
    float   Delta_VXY[2];
    float   Delta_VZ;
    float   AX;
    float   AY;
    float   AZ;
    float   Yaw;
    float   RefAlt;
    float   DistBottom;
    float   DistBottomRate;
    float   EpH;
    float   EpV;
    float   EvH;
    float   EvV;
    uint8   EstimatorType;
    boolean XY_Valid;
    boolean Z_Valid;
    boolean V_XY_Valid;
    boolean V_Z_Valid;
    uint8   XY_ResetCounter;
    uint8   Z_ResetCounter;
    uint8   VXY_ResetCounter;
    uint8   VZ_ResetCounter;
    boolean XY_Global;
    boolean Z_Global;
    boolean DistBottomValid;
} PX4_VehicleLocalPositionMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  X;
    float  Y;
    float  Z;
    float  Yaw;
    float  VX;
    float  VY;
    float  VZ;
    float  AccX;
    float  AccY;
    float  AccZ;
} PX4_VehicleLocalPositionSetpointMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  Roll;
    float  Pitch;
    float  Yaw;
    float  Thrust;
} PX4_VehicleRatesSetpointMsg_t;

typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64  Timestamp;
    uint32  SystemID;
    uint32  ComponentID;
    uint32  OnboardControlSensorsPresent;
    uint32  OnboardControlSensorsEnabled;
    uint32  OnboardControlSensorsHealth;
    PX4_NavigationState_t NavState;
    PX4_ArmingState_t ArmingState;
    PX4_HilState_t HilState;
    boolean Failsafe;
    PX4_SystemType_t SystemType;
    boolean IsRotaryWing;
    boolean IsVtol;
    boolean VtolFwPermanentStab;
    boolean InTransitionMode;
    boolean RcSignalLost;
    PX4_RcInMode_t RcInputMode;
    boolean DataLinkLost;
    uint8   DataLinkLostCounter;
    boolean EngineFailure;
    boolean EngineFailureCmd;
    boolean MissionFailure;
} PX4_VehicleStatusMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    float  gyro_offset_0[3];
    float  gyro_scale_0[3];
    float  gyro_offset_1[3];
    float  gyro_scale_1[3];
    float  gyro_offset_2[3];
    float  gyro_scale_2[3];
    float  accel_offset_0[3];
    float  accel_scale_0[3];
    float  accel_offset_1[3];
    float  accel_scale_1[3];
    float  accel_offset_2[3];
    float  accel_scale_2[3];
    float  baro_offset_0;
    float  baro_scale_0;
    float  baro_offset_1;
    float  baro_scale_1;
    float  baro_offset_2;
    float  baro_scale_2;
    uint8  selected_gyro_instance;
    uint8  selected_accel_instance;
    uint8  selected_baro_instance;
    uint8  gyro_mapping[3];
    uint8  accel_mapping[3];
    uint8  baro_mapping[3];
} PX4_SensorCorrectionMsg_t;


typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint8  LedMask;
    uint8  Color;
    uint8  Mode;
    uint8  NumBlinks;
    uint8  Priority;
} PX4_LedControlMsg_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint64 Timestamp;
    uint8  Frame[4096];
} PX4_OpticalFlowFrameMsg_t;


#endif


