/****************************************************************************
 *
 *   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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
    
#ifndef PRM_MSG_H
#define PRM_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "prm_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** PRM Command Codes
*************************************************************************/

/** \prmcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the PRM task is alive
**
**  \prmcmdmnemonic \PRM_NOOP
**
**  \par Command Structure
**       #PRM_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PRM_CMDACPTCNT - command counter will increment
**       - The #PRM_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \PRM_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PRM_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #PRM_RESET_CC
*/
#define PRM_NOOP_CC                 (0)

/** \prmcmd Reset Counters
**  
**  \par Description
**       Resets the prm housekeeping counters
**
**  \prmcmdmnemonic \PRM_TLMRST
**
**  \par Command Structure
**       #PRM_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PRM_CMDACTPCNT       - command counter will be cleared
**       - \b \c \PRM_CMDRJCTCNT       - command error counter will be cleared
**       - The #PRM_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \PRM_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PRM_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #PRM_NOOP_CC
*/
#define PRM_RESET_CC                (1)

/** \prmcmd Set Parameter
**
**  \par Description
**       Sets a parameter by name
**
**  \prmcmdmnemonic \PRM_TLMSETPARAM
**
**  \par Command Structure
**       #PRM_SetParameterCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PRM_CMDACTPCNT       - command counter will be cleared
**       - \b \c \PRM_CMDRJCTCNT       - command error counter will be cleared
**       - The #PRM_CMD_INF_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \PRM_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PRM_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
*/
#define PRM_SET_PARAM_CC            (2)

/** \prmcmd Get Parameter
**
**  \par Description
**       Gets a parameter by name
**
**  \prmcmdmnemonic \PRM_TLMGETPARM
**
**  \par Command Structure
**       #PRM_GetParameterCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \PRM_CMDACTPCNT       - command counter will be cleared
**       - \b \c \PRM_CMDRJCTCNT       - command error counter will be cleared
**       - The #PRM_CMD_INF_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \PRM_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #PRM_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
*/
#define PRM_GET_PARAM_CC            (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #PRM_NOOP_CC, #PRM_RESET_CC
**  Also see #PRM_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} PRM_NoArgCmd_t;

/** 
**  \brief Set Parameter command
**  For command details see #PRM_SET_PARAMETER_CC
*/
typedef struct
{
    uint8              ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    char               Name[PRMLIB_MSG_PARAM_NAME_LEN];
    uint8              Value[8];
    PRMLIB_ParamType_t Type;
} PRM_SetParameterCmd_t;

/**
**  \brief Get Parameter command
**  For command details see #PRM_GET_PARAMETER_CC
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    char   Name[PRMLIB_MSG_PARAM_NAME_LEN];
} PRM_GetParameterCmd_t;

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

} PRM_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} PRM_OutData_t;

/** 
**  \brief PRM application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \prmtlmmnemonic \PRM_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \prmtlmmnemonic \PRM_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

} PRM_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* PRM_MSG_H */

/************************/
/*  End of File Comment */
/************************/
