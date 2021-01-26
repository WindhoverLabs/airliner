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

#ifndef EA_MSG_H
#define EA_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "ea_platform_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** EA Command Codes
*************************************************************************/

/** \eacmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the EA task is alive
**
**  \eacmdmnemonic \EA_NOOP
**
**  \par Command Structure
**       #EA_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EA_CMDACPTCNT - command counter will increment
**       - The #EA_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \EA_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EA_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EA_RESET_CC
*/
#define EA_NOOP_CC                 (0)

/** \eacmd Reset Counters
**  
**  \par Description
**       Resets the ea housekeeping counters
**
**  \eacmdmnemonic \EA_TLMRST
**
**  \par Command Structure
**       #EA_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EA_CMDACTPCNT       - command counter will be cleared
**       - \b \c \EA_CMDRJCTCNT       - command error counter will be cleared
**       - The #EA_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \EA_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EA_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EA_NOOP_CC
*/
#define EA_RESET_CC                (1)

/** \eacmd Start Application
**
**  \par Description
**       Starts external application
**
**  \par Command Structure
**       #EA_StartCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EA_CMDACTPCNT       - command counter will increment
**       - The #EA_INF_APP_START_EID info event message will be
**         generated when the app has started
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - External app path is already running
**       - External app path is invalid
**       - Error starting child task
**       - Error forking child task
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \EA_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EA_CMD_ERR_EID
**       - Error specific event message #EA_CHILD_TASK_START_ERR_EID
**       - Error specific event message #EA_APP_ARG_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EA_START_APP_CC
*/
#define EA_START_APP_CC                (2)

/** \eacmd Terminate Application
**
**  \par Description
**       Stops external application
**
**  \par Command Structure
**       #EA_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EA_CMDACTPCNT       - command counter will increment
**       - The #EA_INF_APP_TERM_EID debug event message will be
**         generated when the app has stopped
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - No active running external app
**       - Error executing kill
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \EA_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EA_CMD_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EA_TERM_APP_CC
*/
#define EA_TERM_APP_CC                (3)

/** \eacmd Perfmon
**
**  \par Description
**       Calculates the CPU utilization of the external application
**
**  \par Command Structure
**       #EA_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EA_CMDACTPCNT       - command counter will increment
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Error parsing proc tree
**
**  \par Evidence of failure may be found in the following telemetry:
**       - ActiveAppUtil will not be a valid number
**
**  \par Criticality
**       None
**
**  \sa #EA_PERFMON_CC
*/
#define EA_PERFMON_CC                (4)



/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #EA_NOOP_CC, #EA_RESET_CC
**  Also see #EA_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} EA_NoArgCmd_t;

/** 
**  \brief Start Command
**  For command details see #EA_START_APP_CC
*/
typedef struct
{
    /** \brief cFE SB Cmd Msg Hdr */
    uint8  CmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Command to execute */
    char   Cmd[EA_MAX_PATH_LEN];

    /** \brief Arguments for the command */
    char   Args[EA_MAX_PATH_LEN];

} EA_StartCmd_t;

/** 
**  \brief EA application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \eatlmmnemonic \EA_CMDACPTCNT
    \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \eatlmmnemonic \EA_CMDRJCTCNT
    \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \eatlmmnemonic \EA_
    \brief Name of current running application */
    char			   ActiveApp[EA_MAX_PATH_LEN];

    /** \eatlmmnemonic \EA_
		\brief CPU utilization of current running application */
    float			   ActiveAppUtil;

    /** \eatlmmnemonic \EA_
		\brief PID of current running application */
    int32			   ActiveAppPID;

    /** \eatlmmnemonic \EA_
	\brief Name of last run application */
    char			   LastAppRun[EA_MAX_PATH_LEN];

    /** \eatlmmnemonic \EA_
	\brief Last run application return code */
    int32			   LastAppStatus;

} EA_HkTlm_t;

/**
**  \brief 
**  Data that is utilized by the start command child task
**  Also see #EA_START_APP_CC
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    
    /** \brief Command being executed */
    char	Cmd[EA_MAX_PATH_LEN];
    
    /** \brief Arguments for the command */
    char	Args[EA_MAX_PATH_LEN];

} EA_ChildData_t;


#ifdef __cplusplus
}
#endif

#endif /* EA_MSG_H */

/************************/
/*  End of File Comment */
/************************/
