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

#ifndef GPS_MSG_H
#define GPS_MSG_H

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
** GPS Command Codes
*************************************************************************/

/** \gpscmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the GPS task is alive
**
**  \gpscmdmnemonic \GPS_NOOP
**
**  \par Command Structure
**       #GPS_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \GPS_CMDACPTCNT - command counter will increment
**       - The #GPS_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \GPS_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #GPS_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #GPS_RESET_CC
*/
#define GPS_NOOP_CC                 (0)

/** \gpscmd Reset Counters
**  
**  \par Description
**       Resets the GPS housekeeping counters
**
**  \gpscmdmnemonic \GPS_TLMRST
**
**  \par Command Structure
**       #GPS_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \GPS_CMDACTPCNT       - command counter will be cleared
**       - \b \c \GPS_CMDRJCTCNT       - command error counter will be cleared
**       - The #GPS_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \GPS_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #GPS_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #GPS_NOOP_CC
*/
#define GPS_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #GPS_NOOP_CC, #GPS_RESET_CC
**  Also see #GPS_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} GPS_NoArgCmd_t;

/** 
**  \brief GPS application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \gpstlmmnemonic \GPS_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \gpstlmmnemonic \GPS_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;

    /** \brief The vehicle gps position message */
    PX4_VehicleGpsPositionMsg_t VehicleGpsMsg;
} GPS_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* GPS_MSG_H */

/************************/
/*  End of File Comment */
/************************/
