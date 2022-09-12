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

#ifndef FAC_MSG_H
#define FAC_MSG_H

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
#include "fac_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** FAC Command Codes
*************************************************************************/

/** \faccmd Noop
**  
**  \par Description
**       Implements the Noop command that demonstrates the FAC task is alive
**
**  \faccmdmnemonic \FAC_NOOP
**
**  \par Command Structure
**       #FAC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FAC_CMDACPTCNT - command counter will increment
**       - The #FAC_CMD_INF_EID informational event message will be
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \FAC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FAC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FAC_RESET_CC
*/
#define FAC_NOOP_CC                    (0)

/** \faccmd Reset Counters
**  
**  \par Description
**       Resets the fac housekeeping counters
**
**  \faccmdmnemonic \FAC_TLMRST
**
**  \par Command Structure
**       #FAC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FAC_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FAC_CMDRJCTCNT       - command error counter will be cleared
**       - The #FAC_CMD_INF_EID debug event message will be
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \FAC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FAC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FAC_NOOP_CC
*/
#define FAC_RESET_CC                   (1)

#define FAC_UPDATE_FW_R_TC_CC          (2)

#define FAC_UPDATE_FW_P_TC_CC          (3)

#define FAC_UPDATE_FW_PR_P_CC          (4)

#define FAC_UPDATE_FW_PR_I_CC          (5)

#define FAC_UPDATE_FW_P_RMAX_POS_CC    (6)

#define FAC_UPDATE_FW_P_RMAX_NEG_CC    (7)

#define FAC_UPDATE_FW_PR_IMAX_CC       (8)

#define FAC_UPDATE_FW_RR_P_CC          (9)

#define FAC_UPDATE_FW_RR_I_CC          (10)

#define FAC_UPDATE_FW_RR_IMAX_CC       (11)

#define FAC_UPDATE_FW_R_RMAX_CC        (12)

#define FAC_UPDATE_FW_YR_P_CC          (13)

#define FAC_UPDATE_FW_YR_I_CC          (14)

#define FAC_UPDATE_FW_YR_IMAX_CC       (15)

#define FAC_UPDATE_FW_Y_RMAX_CC        (16)

#define FAC_UPDATE_FW_RLL_TO_YAW_FF_CC (17)

#define FAC_UPDATE_FW_W_EN_CC          (18)

#define FAC_UPDATE_FW_WR_P_CC          (19)

#define FAC_UPDATE_FW_WR_I_CC          (20)

#define FAC_UPDATE_FW_WR_IMAX_CC       (21)

#define FAC_UPDATE_FW_W_RMAX_CC        (22)

#define FAC_UPDATE_FW_RR_FF_CC         (23)

#define FAC_UPDATE_FW_PR_FF_CC         (24)

#define FAC_UPDATE_FW_YR_FF_CC         (25)

#define FAC_UPDATE_FW_WR_FF_CC         (26)

#define FAC_UPDATE_FW_YCO_VMIN_CC      (27)

#define FAC_UPDATE_FW_YCO_METHOD_CC    (28)

#define FAC_UPDATE_FW_RSP_OFF_CC       (29)

#define FAC_UPDATE_FW_PSP_OFF_CC       (30)

#define FAC_UPDATE_FW_MAN_R_MAX_CC     (31)

#define FAC_UPDATE_FW_MAN_P_MAX_CC     (32)

#define FAC_UPDATE_FW_FLAPS_SCL_CC     (33)

#define FAC_UPDATE_FW_FLAPERON_SCL_CC  (34)

#define FAC_UPDATE_FW_ARSP_MODE_CC     (35)

#define FAC_UPDATE_FW_MAN_R_SC_CC      (36)

#define FAC_UPDATE_FW_MAN_P_SC_CC      (37)

#define FAC_UPDATE_FW_MAN_Y_SC_CC      (38)

#define FAC_UPDATE_FW_BAT_SCALE_EN_CC  (39)

#define FAC_UPDATE_FW_ACRO_X_MAX_CC    (40)

#define FAC_UPDATE_FW_ACRO_Y_MAX_CC    (41)

#define FAC_UPDATE_FW_ACRO_Z_MAX_CC    (42)

#define FAC_UPDATE_FW_RATT_TH_CC       (43)

#define FAC_UPDATE_FW_AIRSPD_MIN_CC    (44)

#define FAC_UPDATE_FW_AIRSPD_MAX_CC    (45)

#define FAC_UPDATE_FW_AIRSPD_TRIM_CC   (46)

#define FAC_UPDATE_TRIM_ROLL_CC        (47)

#define FAC_UPDATE_TRIM_PITCH_CC       (48)

#define FAC_UPDATE_TRIM_YAW_CC         (49)

#define FAC_UPDATE_VT_TYPE_CC          (50)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #FAC_NOOP_CC, #FAC_RESET_CC
**  Also see #FAC_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} FAC_NoArgCmd_t;

/**
**  \brief Update param float.
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief The param to update */
    float  param;
} FAC_UpdateParamFloatCmd_t;

/**
**  \brief Update param int32.
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief The param to update */
    int32  param;
} FAC_UpdateParamInt32Cmd_t;

/**
**  \brief Update param uint32.
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief The param to update */
    uint32  param;
} FAC_UpdateParamUint32Cmd_t;

/** 
**  \brief FAC application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \factlmmnemonic \FAC_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \factlmmnemonic \FAC_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \brief Count of failed sch msg */
    uint8              usSchErrCnt;

    /** \brief Count of failed input data msg */
    uint8              usDataErrCnt;


    /** \brief Count of SendHk Msg received */
    uint32             SendHkMsgRcvCnt;

    /** \brief Count of RunController Msg received */
    uint32             RunControllerMsgRcvCnt;


    /** \brief Count of AirSpeed Msg received */
    uint32             AirSpeedMsgRcvCnt;

    /** \brief Count of BatteryStatus Msg received */
    uint32             BatteryStatusMsgRcvCnt;

    /** \brief Count of ManualControlSp Msg received */
    uint32             ManualControlSpMsgRcvCnt;

    /** \brief Count of VAtt Msg received */
    uint32             VAttMsgRcvCnt;

    /** \brief Count of VAttSp Msg received */
    uint32             VAttSpMsgRcvCnt;

    /** \brief Count of VControlMode Msg received */
    uint32             VControlModeMsgRcvCnt;

    /** \brief Count of VGlobalPosition Msg received */
    uint32             VGlobalPositionMsgRcvCnt;

    /** \brief Count of VLandDetected Msg received */
    uint32             VLandDetectedMsgRcvCnt;

    /** \brief Count of VehicleStatus Msg received */
    uint32             VehicleStatusMsgRcvCnt;


    /** \brief Count of ActuatorControls0 Msg sent */
    uint32             ActuatorControls0MsgSndCnt;

    /** \brief Count of ActuatorControls2 Msg sent */
    uint32             ActuatorControls2MsgSndCnt;

    /** \brief Count of VehicleRatesSetpoint Msg sent */
    uint32             VehicleRatesSetpointMsgSndCnt;

    /** \brief Count of Hk Msg sent */
    uint32             HkMsgSndCnt;

    FAC_ParamTbl_t    Params;

} FAC_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* FAC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
