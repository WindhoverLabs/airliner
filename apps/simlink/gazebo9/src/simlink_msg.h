/****************************************************************************
 *
 *   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
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
 * 3. Neither the name Windhover Labs nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *****************************************************************************/
    
#ifndef SIMLINK_MSG_H
#define SIMLINK_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "simlink_platform_cfg.h"
#include "simlink_mission_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** SIMLINK Command Codes
*************************************************************************/

/** \simlinkcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the SIMLINK task is alive
**
**  \simlinkcmdmnemonic \SIMLINK_NOOP
**
**  \par Command Structure
**       #SIMLINK_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SIMLINK_CMDACPTCNT - command counter will increment
**       - The #SIMLINK_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SIMLINK_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SIMLINK_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SIMLINK_RESET_CC
*/
#define SIMLINK_NOOP_CC                 (0)

/** \simlinkcmd Reset Counters
**  
**  \par Description
**       Resets the simlink housekeeping counters
**
**  \simlinkcmdmnemonic \SIMLINK_TLMRST
**
**  \par Command Structure
**       #SIMLINK_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SIMLINK_CMDACTPCNT       - command counter will be cleared
**       - \b \c \SIMLINK_CMDRJCTCNT       - command error counter will be cleared
**       - The #SIMLINK_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SIMLINK_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SIMLINK_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SIMLINK_NOOP_CC
*/
#define SIMLINK_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #SIMLINK_NOOP_CC, #SIMLINK_RESET_CC
**  Also see #SIMLINK_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} SIMLINK_NoArgCmd_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific incoming data
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices.
    **        Option: for data that is already defined by another app, include
    **        that app's message header above.
    */

} SIMLINK_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} SIMLINK_OutData_t;


typedef struct
{
	uint32 Count;
	uint16 checksum;      ///< sent at end of packet
	uint8  magic;          ///< protocol magic marker
	uint8  len;            ///< Length of payload
	uint8  incompat_flags; ///< flags that must be understood
	uint8  compat_flags;   ///< flags that can be ignored if not understood
	uint8  seq;            ///< Sequence of packet
	uint8  sysid;          ///< ID of message sender system/aircraft
	uint8  compid;         ///< ID of the message sender component
	uint64 time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
	uint16 chan1_raw; /*< [us] RC channel 1 value*/
	uint16 chan2_raw; /*< [us] RC channel 2 value*/
	uint16 chan3_raw; /*< [us] RC channel 3 value*/
	uint16 chan4_raw; /*< [us] RC channel 4 value*/
	uint16 chan5_raw; /*< [us] RC channel 5 value*/
	uint16 chan6_raw; /*< [us] RC channel 6 value*/
	uint16 chan7_raw; /*< [us] RC channel 7 value*/
	uint16 chan8_raw; /*< [us] RC channel 8 value*/
	uint16 chan9_raw; /*< [us] RC channel 9 value*/
	uint16 chan10_raw; /*< [us] RC channel 10 value*/
	uint16 chan11_raw; /*< [us] RC channel 11 value*/
	uint16 chan12_raw; /*< [us] RC channel 12 value*/
	uint8  rssi; /*<  Receive signal strength indicator in device-dependent units/scale. Values: [0-254], 255: invalid/unknown.*/
} SIMLINK_HilRcInputsRawMetrics_t;


typedef struct
{
	uint32   Count;
	uint16   checksum;      ///< sent at end of packet
	uint8    magic;          ///< protocol magic marker
	uint8    len;            ///< Length of payload
	uint8    incompat_flags; ///< flags that must be understood
	uint8    compat_flags;   ///< flags that can be ignored if not understood
	uint8    seq;            ///< Sequence of packet
	uint8    sysid;          ///< ID of message sender system/aircraft
	uint8    compid;         ///< ID of the message sender component
    uint64   time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
    float    xacc; /*< [m/s/s] X acceleration*/
    float    yacc; /*< [m/s/s] Y acceleration*/
    float    zacc; /*< [m/s/s] Z acceleration*/
    float    xgyro; /*< [rad/s] Angular speed around X axis in body frame*/
    float    ygyro; /*< [rad/s] Angular speed around Y axis in body frame*/
    float    zgyro; /*< [rad/s] Angular speed around Z axis in body frame*/
    float    xmag; /*< [gauss] X Magnetic field*/
    float    ymag; /*< [gauss] Y Magnetic field*/
    float    zmag; /*< [gauss] Z Magnetic field*/
    float    abs_pressure; /*< [hPa] Absolute pressure*/
    float    diff_pressure; /*< [hPa] Differential pressure (airspeed)*/
    float    pressure_alt; /*<  Altitude calculated from pressure*/
    float    temperature; /*< [degC] Temperature*/
    uint32   fields_updated; /*<  Bitmap for fields that have updated since last message, bit 0 = xacc, bit 12: temperature, bit 31: full reset of attitude/position/velocities/etc was performed in sim.*/
    uint8    id; /*<  Sensor ID (zero indexed). Used for multiple sensor inputs*/
} SIMLINK_HilSensorMetrics_t;

typedef struct
{
	uint32   Count;
	uint16   checksum;      ///< sent at end of packet
	uint8    magic;          ///< protocol magic marker
	uint8    len;            ///< Length of payload
	uint8    incompat_flags; ///< flags that must be understood
	uint8    compat_flags;   ///< flags that can be ignored if not understood
	uint8    seq;            ///< Sequence of packet
	uint8    sysid;          ///< ID of message sender system/aircraft
	uint8    compid;         ///< ID of the message sender component
    uint64   time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
    int32    lat; /*< [degE7] Latitude (WGS84)*/
    int32    lon; /*< [degE7] Longitude (WGS84)*/
    int32    alt; /*< [mm] Altitude (MSL). Positive for up.*/
    uint16   eph; /*<  GPS HDOP horizontal dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    uint16   epv; /*<  GPS VDOP vertical dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    uint16   vel; /*< [cm/s] GPS ground speed. If unknown, set to: 65535*/
    int16    vn; /*< [cm/s] GPS velocity in north direction in earth-fixed NED frame*/
    int16    ve; /*< [cm/s] GPS velocity in east direction in earth-fixed NED frame*/
    int16    vd; /*< [cm/s] GPS velocity in down direction in earth-fixed NED frame*/
    uint16   cog; /*< [cdeg] Course over ground (NOT heading, but direction of movement), 0.0..359.99 degrees. If unknown, set to: 65535*/
    uint8    fix_type; /*<  0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.*/
    uint8    satellites_visible; /*<  Number of satellites visible. If unknown, set to 255*/
    uint8    id; /*<  GPS ID (zero indexed). Used for multiple GPS inputs*/
    uint16   yaw; /*< [cdeg] Yaw of vehicle relative to Earth's North, zero means not available, use 36000 for north*/
} SIMLINK_HilGpsMetrics_t;


typedef struct
{
	uint32   Count;
	uint16   checksum;      ///< sent at end of packet
	uint8    magic;          ///< protocol magic marker
	uint8    len;            ///< Length of payload
	uint8    incompat_flags; ///< flags that must be understood
	uint8    compat_flags;   ///< flags that can be ignored if not understood
	uint8    seq;            ///< Sequence of packet
	uint8    sysid;          ///< ID of message sender system/aircraft
	uint8    compid;         ///< ID of the message sender component
    uint64   time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
    float    attitude_quaternion[4]; /*<  Vehicle attitude expressed as normalized quaternion in w, x, y, z order (with 1 0 0 0 being the null-rotation)*/
    float    rollspeed; /*< [rad/s] Body frame roll / phi angular speed*/
    float    pitchspeed; /*< [rad/s] Body frame pitch / theta angular speed*/
    float    yawspeed; /*< [rad/s] Body frame yaw / psi angular speed*/
    int32    lat; /*< [degE7] Latitude*/
    int32    lon; /*< [degE7] Longitude*/
    int32    alt; /*< [mm] Altitude*/
    int16    vx; /*< [cm/s] Ground X Speed (Latitude)*/
    int16    vy; /*< [cm/s] Ground Y Speed (Longitude)*/
    int16    vz; /*< [cm/s] Ground Z Speed (Altitude)*/
    uint16   ind_airspeed; /*< [cm/s] Indicated airspeed*/
    uint16   true_airspeed; /*< [cm/s] True airspeed*/
    int16    xacc; /*< [mG] X acceleration*/
    int16    yacc; /*< [mG] Y acceleration*/
    int16    zacc; /*< [mG] Z acceleration*/
} SIMLINK_HilStateQuaternion_t;


typedef struct
{
	uint32   Count;
	uint16   checksum;      ///< sent at end of packet
	uint8    magic;          ///< protocol magic marker
	uint8    len;            ///< Length of payload
	uint8    incompat_flags; ///< flags that must be understood
	uint8    compat_flags;   ///< flags that can be ignored if not understood
	uint8    seq;            ///< Sequence of packet
	uint8    sysid;          ///< ID of message sender system/aircraft
	uint8    compid;         ///< ID of the message sender component
	uint32   custom_mode; /*<  A bitfield for use for autopilot-specific flags*/
	uint8    type; /*<  Vehicle or component type. For a flight controller component the vehicle type (quadrotor, helicopter, etc.). For other components the component type (e.g. camera, gimbal, etc.). This should be used in preference to component id for identifying the component type.*/
	uint8    autopilot; /*<  Autopilot type / class. Use MAV_AUTOPILOT_INVALID for components that are not flight controllers.*/
	uint8    base_mode; /*<  System mode bitmap.*/
	uint8    system_status; /*<  System status flag.*/
	uint8    mavlink_version; /*<  MAVLink version, not writable by user, gets added by protocol because of magic data type: uint8_t_mavlink_version*/
} SIMLINK_Heartbeat_t;

typedef struct
{
    uint32 Count;
	uint16   checksum;       ///< sent at end of packet
	uint8    magic;          ///< protocol magic marker
	uint8    len;            ///< Length of payload
	uint8    incompat_flags; ///< flags that must be understood
	uint8    compat_flags;   ///< flags that can be ignored if not understood
	uint8    seq;            ///< Sequence of packet
	uint8    sysid;          ///< ID of message sender system/aircraft
	uint8    compid;         ///< ID of the message sender component
    uint64 time_usec;        /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
    uint64 flags;            /*<  Flags as bitfield, 1: indicate simulation using lockstep.*/
    float  controls[16];     /*<  Control outputs -1 .. 1. Channel assignment depends on the simulated hardware.*/
    uint8  mode;             /*<  System mode. Includes arming state.*/
} SIMLINK_ActuatorControls_t;


typedef struct
{
    SIMLINK_HilRcInputsRawMetrics_t HilRcInputsRaw;
    SIMLINK_HilSensorMetrics_t      HilSensor;
    SIMLINK_HilGpsMetrics_t         HilGps;
    SIMLINK_HilStateQuaternion_t    HilStateQuaternion;
    SIMLINK_Heartbeat_t             Heartbeat;
} SIMLINK_DataInMetrics_t;


typedef struct
{
    uint32 GpsMsgCount[SIMLINK_GPS_DEVICE_COUNT];
    uint32 GyroMsgCount[SIMLINK_GYRO_DEVICE_COUNT];
    uint32 AccelMsgCount[SIMLINK_ACCEL_DEVICE_COUNT];
    uint32 MagMsgCount[SIMLINK_MAG_DEVICE_COUNT];
    uint32 BaroMsgCount[SIMLINK_BARO_DEVICE_COUNT];
    uint32 DiffPressMsgCount[SIMLINK_DEFFPRESS_DEVICE_COUNT];
    SIMLINK_Heartbeat_t         Heartbeat;
    SIMLINK_ActuatorControls_t  PWM;
} SIMLINK_DataOutMetrics_t;


/** 
**  \brief SIMLINK application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \simlinktlmmnemonic \SIMLINK_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \simlinktlmmnemonic \SIMLINK_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    SIMLINK_DataInMetrics_t  DataInMetrics;
    SIMLINK_DataOutMetrics_t DataOutMetrics;

} SIMLINK_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* SIMLINK_MSG_H */

/************************/
/*  End of File Comment */
/************************/
