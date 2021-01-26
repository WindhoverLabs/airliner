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

#ifndef MPC_MSG_H
#define MPC_MSG_H

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
** MPC Command Codes
*************************************************************************/

/** \mpccmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MPC task is alive
**
**  \mpccmdmnemonic \MPC_NOOP
**
**  \par Command Structure
**       #MPC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**
**  \par Criticality
**       None
**
**  \sa #MPC_RESET_CC
*/
#define MPC_NOOP_CC                              (0)

/** \mpccmd Reset Counters
**  
**  \par Description
**       Resets the MPC housekeeping counters
**
**  \mpccmdmnemonic \MPC_TLMRST
**
**  \par Command Structure
**       #MPC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MPC_CMDRJCTCNT       - command error counter will be cleared
**       - The #MPC_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**
**  \par Criticality
**       None
**
**  \sa #MPC_NOOP_CC
*/
#define MPC_RESET_CC                             (1)

/** \mpccmd
**
**  \par Description
**       Updates XY PID values.
**
**  \par Command Structure
**       #MPC_SetPidCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_PID_UPDATE_EID information event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected.
**       - Arguments are invalid or out of bounds.
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**       - The #MPC_INVLD_PARAM_ERR_EID error event message will be
**         generated if the an argument is invalid.
**
**  \par Criticality
**       TODO
**
**  \sa #UpdateXyPids, #XY_P, #XY_VEL_P, #XY_VEL_I, #XY_VEL_D
*/
#define MPC_SET_XY_PID_CC                         (2)

/** \mpccmd
**
**  \par Description
**       Updates Z PID values.
**
**  \par Command Structure
**       #MPC_SetPidCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_PID_UPDATE_EID information event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**       - The #MPC_INVLD_PARAM_ERR_EID error event message will be
**         generated if the an argument is invalid.
**
**  \par Criticality
**       TODO
**
**  \sa #UpdateZPids, #Z_P, #Z_VEL_P, #Z_VEL_I, #Z_VEL_D
*/
#define MPC_SET_Z_PID_CC                         (3)

/** \mpccmd
**
**  \par Description
**       This commands causes MPC to send a diagnostic message.
**
**  \par Command Structure
**       #MPC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_SEND_DIAG_EID informational event message will be
**         generated when the command is received
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**
**  \par Criticality
**       TODO
**
**  \sa #ReportDiagnostic, #MPC_DiagPacket_t
*/
#define MPC_SEND_DIAG_CC                         (4)

/** \mpccmd
**
**  \par Description
**       Update the HOLD_DZ configuration parameter.
**
**  \par Command Structure
**       #MPC_SetDzCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_SET_DZ_EID informational event message will be
**         generated when the command is received
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**       - The #MPC_INVLD_PARAM_ERR_EID error event message will be
**         generated if the an argument is invalid.
**
**  \par Criticality
**       TODO
**
**  \sa #UpdateHoldDz, #HOLD_DZ
*/
#define MPC_SET_HOLD_DZ_CC                       (5)

/** \mpccmd
**
**  \par Description
**       Update the XY_MAN_EXPO, and Z_MAN_EXPO configuration
**       parameters.
**
**  \par Command Structure
**       #MPC_SetStickExpoCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_SET_EXPO_EID informational event message will be
**         generated when the command is received
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**       - The #MPC_INVLD_PARAM_ERR_EID error event message will be
**         generated if the an argument is invalid.
**
**  \par Criticality
**       TODO
**
**  \sa #UpdateStickExpo
*/
#define MPC_SET_STICK_EXPO_CC                    (6)

/** \mpccmd
**
**  \par Description
**       Update the TKO_RAMP_T configuration parameter.
**
**  \par Command Structure
**       #MPC_SetTkoRampCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPC_CMDACPTCNT - command counter will increment
**       - The #MPC_SET_TKO_RAMP_EID informational event message will be
**         generated when the command is received
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \MPC_CMDRJCTCNT - command error counter will increment
**       - The #MPC_MSGLEN_ERR_EID error event message will be
**         generated if the command has the wrong length.
**       - The #MPC_INVLD_PARAM_ERR_EID error event message will be
**         generated if the an argument is invalid.
**
**  \par Criticality
**       TODO
**
**  \sa #UpdateTakeoffRampTime
*/
#define MPC_SET_TKO_RAMP_CC                      (7)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

enum ManualStickInput
{
    BRAKE            = 0,
    DIRECTION_CHANGE = 1,
    ACCELERATION     = 2,
    DECELERATION     = 3,
    NONE             = 4
};

/** 
**  \brief No Arguments Command
**  For command details see #MPC_NOOP_CC, #MPC_RESET_CC
**  Also see #MPC_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MPC_NoArgCmd_t;

/** 
**  \brief MPC application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \mpctlmmnemonic \MPC_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \mpctlmmnemonic \MPC_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt;

    /** \brief See #m_AccelerationStateLimitXY */
    float              AccelerationStateLimitXY;

    /** \brief See #m_AccelerationStateLimitZ */
    float              AccelerationStateLimitZ;

    /** \brief See #m_ManualJerkLimitXY */
    float              ManualJerkLimitXY;

    /** \brief See #m_ManualJerkLimitZ */
    float              ManualJerkLimitZ;

    /** \brief See #m_TakeoffVelLimit */
    float              TakeoffVelLimit;

    /** \brief See #m_VelMaxXy */
    float              VelMaxXy;

    /** \brief See #m_YawTakeoff */
    float              YawTakeoff;

    /** \brief See #m_Yaw */
    float              Yaw;

    /** \brief See #m_UserIntentionXY */
    ManualStickInput   UserIntentionXY;

    /** \brief See #m_UserIntentionZ */
    ManualStickInput   UserIntentionZ;

    /** \brief See #m_ModeAuto */
    osalbool            ModeAuto;

    /** \brief See #m_PositionHoldEngaged */
    osalbool            PositionHoldEngaged;

    /** \brief See #m_AltitudeHoldEngaged */
    osalbool            AltitudeHoldEngaged;

    /** \brief See #m_RunPosControl */
    osalbool            RunPosControl;

    /** \brief See #m_RunAltControl */
    osalbool            RunAltControl;

    /** \brief See #m_InTakeoff */
    osalbool            InTakeoff;

    /** \brief See #m_TripletLatLonFinite */
    osalbool            TripletLatLonFinite;

    /** \brief See #m_WasLanded */
    osalbool            WasLanded;

    /** \brief See #m_WasArmed */
    osalbool            WasArmed;

} MPC_HkTlm_t;

/**
**  \brief MPC diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \brief See #MPC_ConfigTbl_t.Z_P */
    float            Z_P;

    /** \brief See #MPC_ConfigTbl_t.Z_VEL_P */
    float            Z_VEL_P;

    /** \brief See #MPC_ConfigTbl_t.Z_VEL_I */
    float            Z_VEL_I;

    /** \brief See #MPC_ConfigTbl_t.Z_VEL_D */
    float            Z_VEL_D;

    /** \brief See #MPC_ConfigTbl_t.Z_VEL_MAX_UP */
    float            Z_VEL_MAX_UP;

    /** \brief See #MPC_ConfigTbl_t.Z_VEL_MAX_DN */
    float            Z_VEL_MAX_DN;

    /** \brief See #MPC_ConfigTbl_t.Z_FF */
    float            Z_FF;

    /** \brief See #MPC_ConfigTbl_t.XY_P */
    float            XY_P;

    /** \brief See #MPC_ConfigTbl_t.XY_VEL_P */
    float            XY_VEL_P;

    /** \brief See #MPC_ConfigTbl_t.XY_VEL_I */
    float            XY_VEL_I;

    /** \brief See #MPC_ConfigTbl_t.XY_VEL_D */
    float            XY_VEL_D;

    /** \brief See #MPC_ConfigTbl_t.XY_CRUISE */
    float            XY_CRUISE;

    /** \brief See #MPC_ConfigTbl_t.MPC_VEL_MANUAL */
    float            MPC_VEL_MANUAL;

    /** \brief See #MPC_ConfigTbl_t.XY_VEL_MAX */
    float            XY_VEL_MAX;

    /** \brief See #MPC_ConfigTbl_t.XY_FF */
    float            XY_FF;

    /** \brief See #MPC_ConfigTbl_t.TILTMAX_AIR */
    float            TILTMAX_AIR;

    /** \brief See #MPC_ConfigTbl_t.ACC_HOR_MAX */
    float            ACC_HOR_MAX;

    /** \brief See #MPC_ConfigTbl_t.ACC_UP_MAX */
    float            ACC_UP_MAX;

    /** \brief See #MPC_ConfigTbl_t.ACC_DOWN_MAX */
    float            ACC_DOWN_MAX;

    /** \brief See #MPC_ConfigTbl_t.MPC_DEC_HOR_SLOW */
    float            MPC_DEC_HOR_SLOW;

    /** \brief See #MPC_ConfigTbl_t.MPC_HOLD_DZ */
    float            MPC_HOLD_DZ;

    /** \brief See #MPC_ConfigTbl_t.XY_MAN_EXPO */
    float            XY_MAN_EXPO;

    /** \brief See #MPC_ConfigTbl_t.Z_MAN_EXPO */
    float            Z_MAN_EXPO;

    /** \brief See #MPC_ConfigTbl_t.TKO_RAMP_T */
    float            TKO_RAMP_T;

} MPC_DiagPacket_t;

/**
**  \brief MPC set PID cmd
**
**  \sa #MPC_SET_XY_PID_CC, #MPC_SET_Z_PID_CC
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief PID Gain */
    float  PidGain;

    /** \brief PID P value */
    float  PidVelP;

    /** \brief PID I value */
    float  PidVelI;

    /** \brief PID D value */
    float  PidVelD;

} MPC_SetPidCmd_t;

/**
**  \brief MPC set Hold DZ
**
**  \sa #MPC_SET_HOLD_DZ_CC
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Deadzone */
    float  Deadzone;

} MPC_SetDzCmd_t;

/**
**  \brief MPC set stick exponential curve values
**
**  \sa #MPC_SET_STICK_EXPO_CC
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief XY Expo */
    float  XY;
    
    /** \brief Z Expo */
    float  Z;

} MPC_SetStickExpoCmd_t;

/**
**  \brief MPC set takeoff ramp time
**
**  \sa #MPC_SET_TKO_RAMP_CC
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief XY Expo */
    float  TKO_RAMP_T;
    
} MPC_SetTkoRampCmd_t;

#ifdef __cplusplus
}
#endif

#endif /* MPC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
