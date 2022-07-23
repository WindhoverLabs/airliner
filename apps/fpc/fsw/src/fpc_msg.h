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
#define FPC_RESET_CC                (1)

#define FPC_DO_GO_AROUND_CC         (2)

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
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} FPC_NoArgCmd_t;

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

} FPC_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} FPC_OutData_t;

typedef enum {
    THROTTLE_RAMP      = 0, /**< ramping up throttle */
    CLAMPED_TO_RUNWAY  = 1, /**< clamped to runway, controlling yaw directly (wheel or rudder) */
    TAKEOFF            = 2, /**< taking off, get ground clearance, roll 0 */
    CLIMBOUT           = 3, /**< climbout to safe height before navigation, roll limited */
        FLY                = 4 /**< fly towards takeoff waypoint */
}RunwayTakeoffState;

typedef struct
{
    /** state variables **/
    RunwayTakeoffState _state;
    osalbool _initialized;
    uint64 _initialized_time;
    float _init_yaw;
    osalbool _climbout;
    unsigned _throttle_ramp_time;
//    math::Vector2F _start_wp;

        /** parameters **/
    osalbool _runway_takeoff_enabled;
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


typedef enum  {
    FW_POSCTRL_MODE_AUTO = 0,
    FW_POSCTRL_MODE_POSITION = 1,
    FW_POSCTRL_MODE_ALTITUDE = 2,
    FW_POSCTRL_MODE_OTHER = 3
} HK_FW_POSCTRL_MODE;		///< used to check the mode in the last control loop iteration. Use to check if the last iteration was in the same mode.

/** 
**  \brief FPC application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \fpctlmmnemonic \FPC_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \fpctlmmnemonic \FPC_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    HK_FW_POSCTRL_MODE ControlModeCurrent;

    float	       _hold_alt;
    float              m_Hold_Alt;

    Runway             _runway_takeoff;

} FPC_HkTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* FPC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
