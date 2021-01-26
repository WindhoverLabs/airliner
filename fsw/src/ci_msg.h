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

#ifndef CI_MSG_H
#define CI_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "ci_private_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** CI Command Codes
*************************************************************************/

/** \cicmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the CI task is alive
**
**  \cicmdmnemonic \CI_NOOP
**
**  \par Command Structure
**       #CI_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \CI_CMDACPTCNT - command counter will increment
**       - The #CI_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_RESET_CC
*/
#define CI_NOOP_CC                 (0)

/** \cicmd Reset Counters
**  
**  \par Description
**       Resets the ci housekeeping counters
**
**  \cicmdmnemonic \CI_TLMRST
**
**  \par Command Structure
**       #CI_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \CI_CMDACTPCNT       - command counter will be cleared
**       - \b \c \CI_CMDRJCTCNT       - command error counter will be cleared
**       - The #CI_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_NOOP_CC
*/
#define CI_RESET_CC                (1)

/** \cicmd Authorize Command
**
**  \par Description
**       Authorizes a 2-step command
**
**  \par Command Structure
**       #CI_CmdAuthData_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #CI_CMD_AUTHORIZED_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Command to authorize not 2-step
**       - Command to authorize not registered
**       - Command to authorize already authorized
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_CMD_AUTH_INV_MODE_EID
**       - Error specific event message #CI_CMD_AUTH_NOT_REG_EID
**       - Error specific event message #CI_CMD_AUTH_INV_STATE_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_AUTH_CMD_CC
*/
#define CI_AUTH_CMD_CC                (2)

/** \cicmd Deauthorize Command
**
**  \par Description
**       Deauthorizes a 2-step command
**
**  \par Command Structure
**       #CI_CmdAuthData_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #CI_CMD_DEAUTHORIZED_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Command to deauthorize not 2-step
**       - Command to deauthorize not registered
**       - Command to deauthorize not authorized
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_CMD_DEAUTH_INV_MODE_EID
**       - Error specific event message #CI_CMD_DEAUTH_NOT_REG_EID
**       - Error specific event message #CI_CMD_DEAUTH_INV_STATE_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_DEAUTH_CMD_CC
*/
#define CI_DEAUTH_CMD_CC                (3)

/** \cicmd Register Command
**
**  \par Description
**       Registers a command with CI config table
**
**  \par Command Structure
**       #CI_CmdRegData_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #CI_CMD_REGISTERED_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Command to register already registered
**       - Config table is full
**
**  \par Evidence of failure may be found in the following telemetry:
**       - Error specific event message #CI_CMD_REG_ERR_EID
**       - Error specific event message #CI_CMD_ALREADY_REGISTERED_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_REG_CMD_CC
*/
#define CI_REG_CMD_CC                (4)

/** \cicmd Deregister Command
**
**  \par Description
**       Deregisters a command in CI config table
**
**  \par Command Structure
**       #CI_CmdAuthData_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #CI_CMD_DEREGISTERED_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Command to register not registered
**
**  \par Evidence of failure may be found in the following telemetry:
**       - Error specific event message #CI_CMD_REG_ERR_EID
**       - Error specific event message #CI_CMD_NOT_REGISTERED_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_DEREG_CMD_CC
*/
#define CI_DEREG_CMD_CC                (5)

/** \cicmd Update Command Registration
**
**  \par Description
**       Updates the command registration in the CI config table
**
**  \par Command Structure
**       #CI_CmdRegData_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - The #CI_CMD_UPDATE_REG_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Command to register not registered
**
**  \par Evidence of failure may be found in the following telemetry:
**       - Error specific event message #CI_CMD_NOT_REGISTERED_EID
**       - Error specific event message #CI_CMD_REG_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_UPDT_CMD_CC
*/
#define CI_UPDT_CMD_CC                (6)

/************************************************************************
** Local Structure Declarations
*************************************************************************/



/** 
**  \brief No Arguments Command
**  For command details see #CI_NOOP_CC, #CI_RESET_CC
**  Also see #CI_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} CI_NoArgCmd_t;

/** 
**  \brief Command Authorization Data
**  For command details see #CI_AUTH_CMD_CC, #CI_DEAUTH_CMD_CC
*/
typedef struct
{
    uint8  				ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    CFE_SB_MsgId_t		msgID;
    uint16				cmdCode;
} CI_CmdAuthData_t;

/**
**  \brief Command Register Data
**  For command details see #CI_REG_CMD_CC, #CI_UPDT_CMD_CC
*/
typedef struct
{
    uint8  				ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    CFE_SB_MsgId_t		msgID;
    uint16				cmdCode;
    CI_CmdStep_t		step;
    CI_CmdLog_t	 		log;
} CI_CmdRegData_t;

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

} CI_InData_t;



/**
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} CI_OutData_t;

/** 
**  \brief CI application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \citlmmnemonic \CI_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \citlmmnemonic \CI_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \brief Padding for struct alignment */
    uint8              padding[2];  
  
    /** \brief Ingest count of msgs */
    uint8              IngestMsgCount;

    /** \brief Ingest count of errors */
    uint8              IngestErrorCount;
} CI_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* CI_MSG_H */

/************************/
/*  End of File Comment */
/************************/
