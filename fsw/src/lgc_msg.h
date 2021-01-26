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

#ifndef LGC_MSG_H
#define LGC_MSG_H

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
** LGC Command Codes
*************************************************************************/

/** \lgccmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the LGC task is alive
**
**  \lgccmdmnemonic \LGC_NOOP
**
**  \par Command Structure
**       #LGC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \LGC_CMDACPTCNT - command counter will increment
**       - The #LGC_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \LGC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #LGC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #LGC_RESET_CC
*/
#define LGC_NOOP_CC                 (0)

/** \lgccmd Reset Counters
**  
**  \par Description
**       Resets the LGC housekeeping counters
**
**  \lgccmdmnemonic \LGC_TLMRST
**
**  \par Command Structure
**       #LGC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \LGC_CMDACTPCNT       - command counter will be cleared
**       - \b \c \LGC_CMDRJCTCNT       - command error counter will be cleared
**       - The #LGC_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \LGC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #LGC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #LGC_NOOP_CC
*/
#define LGC_RESET_CC                (1)

/** \lgccmd Extend Landing Gear.
**  
**  \par Description
**       Extends the landing gear.
**
**  \lgccmdmnemonic \LGC_EXTEND
**
**  \par Command Structure
**       #LGC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \LGC_CMDACPTCNT - command counter will increment
**       - The #LGC_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \LGC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #LGC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #LGC_EXTEND_CC
*/
#define LGC_EXTEND_CC                (2)

/** \lgccmd Retract Landing Gear.
**  
**  \par Description
**       Retracts the landing gear.
**
**  \lgccmdmnemonic \LGC_RETRACT
**
**  \par Command Structure
**       #LGC_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \LGC_CMDACPTCNT - command counter will increment
**       - The #LGC_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \LGC_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #LGC_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #LGC_RETRACT_CC
*/
#define LGC_RETRACT_CC                (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #LGC_NOOP_CC, #LGC_RESET_CC
**  Also see #LGC_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} LGC_NoArgCmd_t;

/** 
**  \brief LGC application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \lgctlmmnemonic \LGC_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \lgctlmmnemonic \LGC_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
} LGC_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* LGC_MSG_H */

/************************/
/*  End of File Comment */
/************************/
