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
    
#ifndef MS5611_MSG_H
#define MS5611_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "sedlib.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** MS5611 Command Codes
*************************************************************************/

/** \ms5611cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MS5611 task is alive
**
**  \ms5611cmdmnemonic \MS5611_NOOP
**
**  \par Command Structure
**       #MS5611_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MS5611_CMDACPTCNT - command counter will increment
**       - The #MS5611_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MS5611_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MS5611_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MS5611_RESET_CC
*/
#define MS5611_NOOP_CC                 (0)

/** \ms5611cmd Reset Counters
**  
**  \par Description
**       Resets the ms5611 housekeeping counters
**
**  \ms5611cmdmnemonic \MS5611_TLMRST
**
**  \par Command Structure
**       #MS5611_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MS5611_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MS5611_CMDRJCTCNT       - command error counter will be cleared
**       - The #MS5611_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MS5611_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MS5611_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MS5611_NOOP_CC
*/
#define MS5611_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #MS5611_NOOP_CC, #MS5611_RESET_CC
**  Also see #MS5611_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MS5611_NoArgCmd_t;

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

} MS5611_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} MS5611_OutData_t;

/** 
**  \brief MS5611 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \ms5611tlmmnemonic \MS5611_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \ms5611tlmmnemonic \MS5611_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt;

//    SPI_TransferAction_t  SPI_TransferAction;

    SEDLIB_ReturnCode_t                 SED_RC;


} MS5611_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* MS5611_MSG_H */

/************************/
/*  End of File Comment */
/************************/
