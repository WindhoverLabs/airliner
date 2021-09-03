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

#ifndef PE_MSG_H
#define PE_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"


/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** PE Command Codes
*************************************************************************/

/** \pecmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the PE task is alive
**
**  \pecmdmnemonic \PE_NOOP
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PE_CMDACPTCNT - command counter will increment
**       - The #PE_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \PE_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PE_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #PE_RESET_CC
*/
#define PE_NOOP_CC                 (0)

/** \pecmd Reset Counters
**  
**  \par Description
**       Resets the PE housekeeping counters
**
**  \pecmdmnemonic \PE_TLMRST
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PE_CMDACTPCNT       - command counter will be cleared
**       - \b \c \PE_CMDRJCTCNT       - command error counter will be cleared
**       - The #PE_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \PE_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PE_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #PE_NOOP_CC
*/
#define PE_RESET_CC                (1)

/** \pecmd Fuse Distance Sensor
**  
**  \par Description
**       Fuses distance sensor
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be 
**         generated when the command is executed
**       - The Dist Fused HK tlm will be set to true
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already enabled
** 
**  \par Criticality
**       May impact flight performance
*/
#define PE_FUSE_DIST_SENS_CC                (2)

/** \pecmd Disable Distance Sensor
**  
**  \par Description
**       Disables distance sensor fusion
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be 
**         generated when the command is executed
**       - The Dist Fused HK tlm will be set to false
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already disabled
** 
**  \par Criticality
**       May impact flight performance
*/
#define PE_DISABLE_DIST_SENS_CC            (3)

/** \pecmd Fuse GPS
**
**  \par Description
**       Fuses GPS
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The GPS Fused HK tlm will be set to true
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already enabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_FUSE_GPS_CC                (4)

/** \pecmd Disable GPS
**
**  \par Description
**       Disables GPS fusion
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The GPS Fused HK tlm will be set to false
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already disabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_DISABLE_GPS_CC            (5)

/** \pecmd Fuse Baro
**
**  \par Description
**       Fuses Baro
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Baro Fused HK tlm will be set to true
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already enabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_FUSE_BARO_CC                (6)

/** \pecmd Disable Baro
**
**  \par Description
**       Disables Baro fusion
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Baro Fused HK tlm will be set to false
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already disabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_DISABLE_BARO_CC            (7)

/** \pecmd Fuse Land
**
**  \par Description
**       Fuses Land
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Land Fused HK tlm will be set to true
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already enabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_FUSE_LAND_CC                (8)

/** \pecmd Disable Land
**
**  \par Description
**       Disables Land fusion
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Land Fused HK tlm will be set to false
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already disabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_DISABLE_LAND_CC            (9)

/** \pecmd Fuse Flow
**
**  \par Description
**       Fuses Flow
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Flow Fused HK tlm will be set to true
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already enabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_FUSE_FLOW_CC                (10)

/** \pecmd Disable Flow
**
**  \par Description
**       Disables Flow fusion
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_CMD_INF_EID debug event message will be
**         generated when the command is executed
**       - The Flow Fused HK tlm will be set to false
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Already disabled
**
**  \par Criticality
**       May impact flight performance
*/
#define PE_DISABLE_FLOW_CC            (11)

/** \pecmd Send diagnostic telemetry
**
**  \par Description
**       Command to send diagnostic telemetry
**
**  \par Command Structure
**       #PE_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #PE_SEND_DIAG_INF_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Criticality
**       None
*/
#define PE_SEND_DIAG_CC            (12)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #PE_NOOP_CC, #PE_RESET_CC
**  Also see #PE_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} PE_NoArgCmd_t;

/** 
**  \brief PE application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \petlmmnemonic \PE_CMDACPTCNT
        \brief Count of accepted commands */
    uint8   CmdCnt;

    /** \petlmmnemonic \PE_CMDRJCTCNT
        \brief Count of failed commands */
    uint8   CmdErrCnt;

	/** \brief Timestamp */
    CFE_TIME_SysTime_t Timestamp;

    CFE_TIME_SysTime_t Timestamp_Hist;

	/** \brief Timestamp of last baro read */
    CFE_TIME_SysTime_t TimeLastBaro;

	/** \brief Timestamp of last gps read */
    CFE_TIME_SysTime_t TimeLastGps;

	/** \brief Timestamp of last landing read */
    CFE_TIME_SysTime_t TimeLastLand;

	/** \brief Timestamp of last dist read */
    CFE_TIME_SysTime_t TimeLastDist;

	/** \brief Timestamp of last flow read */
    CFE_TIME_SysTime_t TimeLastFlow;

	/** \brief Origin altitude */
	float   AltOrigin;

	/** \brief Flag for if global estimator is initialized */
    boolean EstimatorGlobalInitialized;

	/** \brief Origin altitude initialized flag */
	boolean AltOriginInitialized;

    /** \brief Flag for if baro is initialized */
    boolean BaroInitialized;

	/** \brief Flag for baro fault */
	boolean BaroFault;

	/** \brief Origin altitude of baro */
	float   BaroAltOrigin;

	/** \brief Flag for baro timeout */
	boolean BaroTimeout;

    /** \brief Flag for if gps is initialized */
    boolean GpsInitialized;

	/** \brief Flag for GPS fault */
	boolean GpsFault;

	/** \brief Flag for GPS timeout */
	boolean GpsTimeout;

	/** \brief Origin altitude of GPS */
	float   GpsAltOrigin;

    /** \brief Flag for if land is initialized */
    boolean LandInitialized;

	/** \brief Flag for landing fault */
	boolean LandFault;

	/** \brief Flag for landing timeout */
	boolean LandTimeout;

    /** \brief Flag for XY estimation validity */
    boolean XyEstValid;

    /** \brief Origin altitude of DIST */
    float   DistAltOrigin;

    /** \brief Flag for Z estimation validity */
	boolean ZEstValid;

	/** \brief Flag for terrain estimation validity */
	boolean TzEstValid;

    /* TODO */
    uint16  LandCount;

    /* TODO */
	boolean ReceivedGps;

    /* TODO */
	boolean LastArmedState;

	/** \brief Flag for if local estimator is initialized */
	boolean EstimatorLocalInitialized;

	/** \brief Flag for if Dist is initialized */
    boolean DistInitialized;

	/** \brief Flag for Dist fault */
	boolean DistFault;

	/** \brief Flag for Dist timeout */
	boolean DistTimeout;

    /** \brief Flag for if Dist is being integrated into estimation */
    boolean DistFuse;

    /** \brief Flag for if Gps is being integrated into estimation */
    boolean GpsFuse;

    /** \brief Flag for if Baro is being integrated into estimation */
    boolean BaroFuse;

    /** \brief Flag for if Land is being integrated into estimation */
    boolean LandFuse;

    /** \brief Flag for if Flow is being integrated into estimation */
	boolean FlowFuse;

	/** \brief Flag for if Flow is initialized */
    boolean FlowInitialized;

	/** \brief Flag for Flow fault */
	boolean FlowFault;

	/** \brief Flag for Flow timeout */
	boolean FlowTimeout;

	uint32  WakeupCount;
    uint32  VehicleGpsPositionMsgCount;
    uint32  VehicleStatusMsgCount;
    uint32  VehicleLandDetectedMsgCount;
    uint32  ActuatorArmedMsgCount;
    uint32  VehicleAttitudeMsgCount;
    uint32  SensorCombinedMsgCount;
    uint32  VehicleAttitudeSetpointMsgCount;
    uint32  DistanceSensorMsgCount;
    uint32  OpticalFlowMsgCount;
} PE_HkTlm_t;
	
/** 
**  \brief PE GPS state
*/
typedef enum
{
    PE_GPS_STATE_UNKNOWN   = 0,
    PE_GPS_STATE_FIX       = 1,
    PE_GPS_STATE_NSAT      = 2,
    PE_GPS_STATE_EPH_EPV   = 3,
    PE_GPS_STATE_MEASURE   = 4,
    PE_GPS_STATE_COUNT     = 5,
    PE_GPS_STATE_RECEIVED  = 6,
    PE_GPS_STATE_CORRECT   = 7,
    PE_GPS_STATE_DELAY     = 8,
    PE_GPS_STATE_BETA      = 9,
    PE_GPS_STATE_FAULT     = 10,
    PE_GPS_STATE_INIT      = 11
} PE_GpsState_t;

/** 
**  \brief PE Baro state
*/
typedef enum
{
    PE_BARO_STATE_UNKNOWN = 0,
    PE_BARO_STATE_MEASURE = 1,
    PE_BARO_STATE_COUNT   = 2,
    PE_BARO_STATE_INIT    = 3,
    PE_BARO_STATE_CORRECT = 4,
    PE_BARO_STATE_BETA    = 5
} PE_BaroState_t;

/** 
**  \brief PE Land state
*/
typedef enum
{
    PE_LAND_STATE_UNKNOWN = 0,
    PE_LAND_STATE_MEASURE = 1,
    PE_LAND_STATE_COUNT   = 2,
    PE_LAND_STATE_CORRECT = 3,
    PE_LAND_STATE_BETA    = 4,
    PE_LAND_STATE_INIT    = 5
} PE_LandState_t;

/** 
**  \brief PE Distance state
*/
typedef enum
{
    PE_DIST_STATE_UNKNOWN = 0,
    PE_DIST_STATE_MEASURE = 1,
    PE_DIST_STATE_COUNT   = 2,
    PE_DIST_STATE_EPS     = 3,
    PE_DIST_STATE_DIST    = 4,
    PE_DIST_STATE_CORRECT = 5,
    PE_DIST_STATE_BETA    = 6,
    PE_DIST_STATE_INIT    = 7
} PE_DistState_t;

/** 
**  \brief PE Flow state
*/
typedef enum
{
    PE_FLOW_STATE_UNKNOWN    = 0,
    PE_FLOW_STATE_MEASURE    = 1,
    PE_FLOW_STATE_COUNT      = 2,
    PE_FLOW_STATE_PITCH_ROLL = 3,
    PE_FLOW_STATE_AGL_VAL    = 4,
    PE_FLOW_STATE_AGL_HEIGHT = 5,
    PE_FLOW_STATE_TERR_VAL   = 6,
    PE_FLOW_STATE_QUAL       = 7,
    PE_FLOW_STATE_DT         = 8,
    PE_FLOW_STATE_CORRECT    = 9,
    PE_FLOW_STATE_BETA       = 10,
    PE_FLOW_STATE_INIT       = 11,
    PE_FLOW_STATE_FILTER     = 12
} PE_FlowState_t;

/** 
**  \brief PE application diagnostic data
*/
typedef struct
{
    float BaroBeta;
    float GpsBeta;
    float LandBeta;
    float DistBeta;
    float FlowBeta;
    PE_GpsState_t GpsState;
    PE_BaroState_t BaroState;
    PE_LandState_t LandState;
    PE_DistState_t DistState;
    PE_FlowState_t FlowState;
} PE_DiagTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* PE_MSG_H */

/************************/
/*  End of File Comment */
/************************/

