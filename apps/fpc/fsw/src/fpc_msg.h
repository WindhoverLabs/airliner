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

#ifndef FPC_MSG_H
#define FPC_MSG_H

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"
#include "px4_msgs.h"
#include "fpc_tbldefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** FPC Command Codes
 *************************************************************************/

/** \fpccmd Noop 
 **
 **  \par Description
 **       Implements the Noop command that demonstrates the FPC task is alive
 **
 **  \fpccmdmnemonic \FPC_NOOP
 **
 **  \par Command Structure
 **       #FPC_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \FPC_CMDACPTCNT - command counter will increment
 **       - The #FPC_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \FPC_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #FPC_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa #FPC_RESET_CC
 */
#define FPC_NOOP_CC                 (0)

/** \fpccmd Reset Counters
 **
 **  \par Description
 **       Resets the fpc housekeeping counters
 **
 **  \fpccmdmnemonic \FPC_TLMRST
 **
 **  \par Command Structure
 **       #FPC_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \FPC_CMDACTPCNT       - command counter will be cleared
 **       - \b \c \FPC_CMDRJCTCNT       - command error counter will be cleared
 **       - The #FPC_CMD_INF_EID debug event message will be
 **         generated when the command is executed
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \FPC_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #FPC_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa #FPC_NOOP_CC
 */
#define FPC_RESET_CC                              (1)
#define FPC_DO_GO_AROUND_CC                       (2)
#define FPC_UPDATE_L1_PERIOD_CC                   (3)
#define FPC_UPDATE_L1_DAMPING_CC                  (4)
#define FPC_UPDATE_T_TIME_CONST_CC                (5)
#define FPC_UPDATE_T_THRO_CONST_CC                (6)
#define FPC_UPDATE_T_SINK_MIN_CC                  (7)
#define FPC_UPDATE_T_SINK_MAX_CC                  (8)
#define FPC_UPDATE_T_CLMB_MAX_CC                  (9)
#define FPC_UPDATE_CLMBOUT_DIFF_CC                (10)
#define FPC_UPDATE_T_HRATE_P_CC                   (11)
#define FPC_UPDATE_T_HRATE_FF_CC                  (12)
#define FPC_UPDATE_T_SRATE_P_CC                   (13)
#define FPC_UPDATE_T_THR_DAMP_CC                  (14)
#define FPC_UPDATE_T_INTEG_GAIN_CC                (15)
#define FPC_UPDATE_T_VERT_ACC_CC                  (16)
#define FPC_UPDATE_T_HGT_OMEGA_CC                 (17)
#define FPC_UPDATE_T_SPD_OMEGA_CC                 (18)
#define FPC_UPDATE_T_RLL2THR_CC                   (19)
#define FPC_UPDATE_T_SPDWEIGHT_CC                 (20)
#define FPC_UPDATE_T_PTCH_DAMP_CC                 (21)
#define FPC_UPDATE_AIRSPD_MIN_CC                  (22)
#define FPC_UPDATE_AIRSPD_TRIM_CC                 (23)
#define FPC_UPDATE_AIRSPD_MAX_CC                  (24)
#define FPC_UPDATE_ARSP_MODE_CC                   (25)
#define FPC_UPDATE_P_LIM_MIN_RADIANS_CC           (26)
#define FPC_UPDATE_P_LIM_MAX_RADIANS_CC           (27)
#define FPC_UPDATE_R_LIM_RADIANS_CC               (28)
#define FPC_UPDATE_THR_MIN_CC                     (29)
#define FPC_UPDATE_THR_MAX_CC                     (30)
#define FPC_UPDATE_THR_IDLE_CC                    (31)
#define FPC_UPDATE_THR_CRUISE_CC                  (32)
#define FPC_UPDATE_THR_SLEW_MAX_CC                (33)
#define FPC_UPDATE_THR_ALT_SCL_CC                 (34)
#define FPC_UPDATE_MAN_R_MAX_RADIANS_CC           (35)
#define FPC_UPDATE_MAN_P_MAX_RADIANS_CC           (36)
#define FPC_UPDATE_RSP_OFF_RADIANS_CC             (37)
#define FPC_UPDATE_PSP_OFF_RADIANS_CC             (38)
#define FPC_UPDATE_THR_LND_MAX_CC                 (39)
#define FPC_UPDATE_LND_ANG_RADIANS_CC             (40)
#define FPC_UPDATE_LND_HVIRT_CC                   (41)
#define FPC_UPDATE_LND_FLALT_CC                   (42)
#define FPC_UPDATE_LND_TLALT_CC                   (43)
#define FPC_UPDATE_LND_HHDIST_CC                  (44)
#define FPC_UPDATE_LND_FL_PMIN_RADIANS_CC         (45)
#define FPC_UPDATE_LND_FL_PMAX_RADIANS_CC         (46)
#define FPC_UPDATE_LND_USETER_CC                  (47)
#define FPC_UPDATE_LND_AIRSPD_SC_CC               (48)
#define FPC_UPDATE_LAUN_ALL_ON_CC                 (49)
#define FPC_UPDATE_LAUN_CAT_A_CC                  (50)
#define FPC_UPDATE_LAUN_CAT_T_CC                  (51)
#define FPC_UPDATE_LAUN_CAT_MDEL_CC               (52)
#define FPC_UPDATE_LAUN_CAT_PMAX_RADIANS_CC       (53)
#define FPC_UPDATE_RWTO_TKOFF_CC                  (54)
#define FPC_UPDATE_RWTO_HDG_CC                    (55)
#define FPC_UPDATE_NAV_ALT_CC                     (56)
#define FPC_UPDATE_MAX_THR_CC                     (57)
#define FPC_UPDATE_PSP_CC                         (58)
#define FPC_UPDATE_MAX_PITCH_RADIANS_CC           (59)
#define FPC_UPDATE_MAX_ROLL_RADIANS_CC            (60)
#define FPC_UPDATE_AIRSPD_SCL_CC                  (61)
#define FPC_UPDATE_RUNWAY_AIRSPD_MIN_CC           (62)
#define FPC_UPDATE_RUNWAY_CLMBOUT_DIFF_CC         (63)
#define FPC_OVERRIDE_ALTITUDE_CC                  (64)
#define FPC_OVERRIDE_HEADING_CC                   (65)
/************************************************************************
 ** Local Structure Declarations
 *************************************************************************/

/** 
 **  \brief No Arguments Command
 **  For command details see #FPC_NOOP_CC, #FPC_RESET_CC
 **  Also see #FPC_SEND_HK_MID
 */
typedef struct
{
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} FPC_NoArgCmd_t;

/**
 **  \brief Add Message Flow command.
 */
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief The param to update */
    float param;
} FPC_UpdateParamFloatCmd_t;
/**
 **  \brief Add Message Flow command.
 */
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief The param to update */
    int32 param;
} FPC_UpdateParamInt32Cmd_t;
/**
 **  \brief Add Message Flow command.
 */
typedef struct
{
    /** \brief TC message headers */
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Index of the channel for which this command applies */
    boolean param;

} FPC_UpdateParamBooleanCmd_t;

/** 
 **  \brief TODO Elaborate this struct
 **  Boilerplate example of application-specific incoming data
 */
typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
     **        devices.
     **        Option: for data that is already defined by another app, include
     **        that app's message header above.
     */

} FPC_InData_t;

/** 
 **  \brief TODO Elaborate this struct
 **  Boilerplate example of application-specific outgoing data
 */
typedef struct
{
    uint8 ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 uiCounter;
} FPC_OutData_t;

typedef enum
{
    THROTTLE_RAMP = 0, /**< ramping up throttle */
    CLAMPED_TO_RUNWAY = 1, /**< clamped to runway, controlling yaw directly (wheel or rudder) */
    TAKEOFF = 2, /**< taking off, get ground clearance, roll 0 */
    CLIMBOUT = 3, /**< climbout to safe height before navigation, roll limited */
    FLY = 4 /**< fly towards takeoff waypoint */
} RunwayTakeoffState;

typedef struct
{
    /** state variables **/
    RunwayTakeoffState _state;
    boolean _initialized;
    uint64 _initialized_time;
    float _init_yaw;
    boolean _climbout;
    unsigned int _throttle_ramp_time;
//    math::Vector2F _start_wp;

    /** parameters **/
    boolean _runway_takeoff_enabled;
    int32 _heading_mode;
    float _nav_alt;
    float _takeoff_throttle;
    float _runway_pitch_sp;
    float _max_takeoff_pitch;
    float _max_takeoff_roll;
    float _min_airspeed_scaling;
    float _airspeed_min;
    float _climbout_diff;
} Runway;

typedef enum
{
    FW_POSCTRL_MODE_AUTO = 0,
    FW_POSCTRL_MODE_POSITION = 1,
    FW_POSCTRL_MODE_ALTITUDE = 2,
    FW_POSCTRL_MODE_OTHER = 3
} HK_FW_POSCTRL_MODE; ///< used to check the mode in the last control loop iteration. Use to check if the last iteration was in the same mode.

typedef enum
{
    ECL_TECS_MODE_NORMAL = 0,
    ECL_TECS_MODE_UNDERSPEED,
    ECL_TECS_MODE_BAD_DESCENT,
    ECL_TECS_MODE_CLIMBOUT
} ECL_TECS_MODE;

/** 
 **  \brief FPC application housekeeping data
 */
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \fpctlmmnemonic \FPC_CMDACPTCNT
     \brief Count of accepted commands */
    uint8 usCmdCnt;

    /** \fpctlmmnemonic \FPC_CMDRJCTCNT
     \brief Count of failed commands */
    uint8 usCmdErrCnt;
    HK_FW_POSCTRL_MODE ControlModeCurrent;

    float _hold_alt;
    float _hdg_hold_yaw;

    float dist_to_next_waypoint;

    boolean m_Hdg_Hold_Enabled;
    boolean _yaw_lock_engaged;

    ECL_TECS_MODE tecsMode;

    uint64 _time_started_landing;
    boolean use_tecs_pitch;
    PX4_PositionSetpoint_t _hdg_hold_prev_wp; ///< position where heading hold started */
    PX4_PositionSetpoint_t _hdg_hold_curr_wp; ///< position to which heading hold flies */

    /* throttle and airspeed states */
    boolean _airspeed_valid;	///< flag if a valid airspeed estimate exists
    uint64 _airspeed_last_received;	///< last time airspeed was received. Used to detect timeouts.
    float _airspeed;
    float _eas2tas;

    uint64 m_PositionSetpointTripletMsgCount;

    float TecsPitch;

    boolean inControl;

    boolean _runway_takeoff_enabled;
    boolean _runway_takeoff_initialized;
    Runway _runway_takeoff;

    FPC_ConfigTbl_t Params;
} FPC_HkTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* FPC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
