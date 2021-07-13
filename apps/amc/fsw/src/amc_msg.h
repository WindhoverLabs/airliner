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

#ifndef AMC_MSG_H
#define AMC_MSG_H

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
#include "px4_msgs.h"


/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** AMC Command Codes
*************************************************************************/

/** \amccmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the AMC task is alive
**
**  \amccmdmnemonic \AMC_NOOP
**
**  \par Command Structure
**       #AMC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AMC_CMDACPTCNT - command counter will increment
**       - The #AMC_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AMC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AMC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AMC_RESET_CC
*/
#define AMC_NOOP_CC                 (0)

/** \amccmd Reset Counters
**  
**  \par Description
**       Resets the amc housekeeping counters
**
**  \amccmdmnemonic \AMC_TLMRST
**
**  \par Command Structure
**       #AMC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AMC_CMDACTPCNT       - command counter will be cleared
**       - \b \c \AMC_CMDRJCTCNT       - command error counter will be cleared
**       - The #AMC_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AMC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AMC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AMC_NOOP_CC
*/
#define AMC_RESET_CC                (1)

#define AMC_ARM_DEBUG_CC            (2)

#define AMC_DISARM_DEBUG_CC         (3)

#define AMC_ENGAGE_DEBUG_CC         (4)

#define AMC_DISENGAGE_DEBUG_CC      (5)

#define AMC_DEBUG_CMD_CC            (6)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #AMC_NOOP_CC, #AMC_RESET_CC
**  Also see #AMC_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} AMC_NoArgCmd_t;


typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16 Index;
    uint16 Cmd;
} AMC_DebugCmd_t;


/** 
**  \brief AMC application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \amctlmmnemonic \AMC_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \amctlmmnemonic \AMC_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    CFE_TIME_SysTime_t Timestamp;
    uint32             Count;
    float              Output[PX4_ACTUATOR_OUTPUTS_MAX];
    uint8              DebugArmed;
    uint8              DebugEngaged;
    int                ArmedTimeout;
} AMC_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* AMC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
