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

#ifndef SG33BL_MSG_H
#define SG33BL_MSG_H

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
** SG33BL Command Codes
*************************************************************************/

/** \sg33blcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the SG33BL task is alive
**
**  \sg33blcmdmnemonic \SG33BL_NOOP
**
**  \par Command Structure
**       #SG33BL_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SG33BL_CMDACPTCNT - command counter will increment
**       - The #SG33BL_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SG33BL_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SG33BL_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SG33BL_RESET_CC
*/
#define SG33BL_NOOP_CC                 (0)

/** \sg33blcmd Reset Counters
**  
**  \par Description
**       Resets the SG33BL housekeeping counters
**
**  \sg33blcmdmnemonic \SG33BL_TLMRST
**
**  \par Command Structure
**       #SG33BL_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SG33BL_CMDACTPCNT       - command counter will be cleared
**       - \b \c \SG33BL_CMDRJCTCNT       - command error counter will be cleared
**       - The #SG33BL_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SG33BL_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SG33BL_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SG33BL_NOOP_CC
*/
#define SG33BL_RESET_CC                (1)

#define SG33BL_POSITION_CC             (2)

#define SG33BL_VELOCITY_CC             (3)

#define SG33BL_TORQUE_CC               (4)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #SG33BL_NOOP_CC, #SG33BL_RESET_CC
**  Also see #SG33BL_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} SG33BL_NoArgCmd_t;


typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16 Position;
} SG33BL_PositionCmd_t;


typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16 Velocity;
} SG33BL_VelocityCmd_t;


typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16 Torque;
} SG33BL_TorqueCmd_t;


/** 
**  \brief SG33BL state data.
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \brief current servo position. */
    uint16             Position;

    /** \brief current servo velocity. */
    uint16             Velocity;

    /** \brief current servo torque. */
    uint16             Torque;
    /** \brief current analog voltage raw. */
    int16             Analog;
    /** \brief current analog voltage mv. */
    float              AnalogmV;
} SG33BL_StatusTlm_t;


/** 
**  \brief SG33BL application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \sg33bltlmmnemonic \SG33BL_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \sg33bltlmmnemonic \SG33BL_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    SG33BL_StatusTlm_t Status;
} SG33BL_HkTlm_t;





#ifdef __cplusplus
}
#endif

#endif /* SG33BL_MSG_H */

/************************/
/*  End of File Comment */
/************************/
