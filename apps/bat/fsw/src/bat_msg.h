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

#ifndef BAT_MSG_H
#define BAT_MSG_H

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
** BAT Command Codes
*************************************************************************/

/** \batcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the BAT task is alive
**
**  \batcmdmnemonic \BAT_NOOP
**
**  \par Command Structure
**       #BAT_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \BAT_CMDACPTCNT - command counter will increment
**       - The #BAT_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \BAT_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #BAT_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #BAT_RESET_CC
*/
#define BAT_NOOP_CC                 (0)

/** \batcmd Reset Counters
**  
**  \par Description
**       Resets the BAT housekeeping counters
**
**  \batcmdmnemonic \BAT_TLMRST
**
**  \par Command Structure
**       #BAT_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \BAT_CMDACTPCNT       - command counter will be cleared
**       - \b \c \BAT_CMDRJCTCNT       - command error counter will be cleared
**       - The #BAT_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \BAT_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #BAT_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #BAT_NOOP_CC
*/
#define BAT_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #BAT_NOOP_CC, #BAT_RESET_CC
**  Also see #BAT_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} BAT_NoArgCmd_t;

/** 
**  \brief BAT application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \battlmmnemonic \BAT_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \battlmmnemonic \BAT_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \brief Timestamp of sampled battery status */
    uint64             Timestamp;
    
    /** \brief Sampled voltage */
    float              Voltage;                    /* V */
    
    /** \brief Filtered voltage value */
    float              VoltageFiltered;            /* V */
    
    /** \brief Sampled Current */
    float              Current;                    /* A */
    
    /** \brief Filtered current vlaue */
    float              CurrentFiltered;            /* A */
    
    /** \brief Discharged mAh from battery */
    float              Discharged;                /* mAh */
    
    /** \brief Percentage of battery charge remaining */
    float              Remaining;
    
    /** \brief Scale of voltage */
    float              Scale;
    
    /** \brief Number of cells for battery */
    int32              CellCount;
    
    /** \brief Flag for connection to battery */
    osalbool           Connected;
    
    /** \brief Warning flag enumeration */
    PX4_BatteryWarningSeverity_t Warning;
    
    /** \brief Number of times BAT has woken up. */
	uint32             WakeupCount;

    /** \brief Number of ActuatorControls0 messages BAT has successfully received. */
	uint32             ActuatorControls0MsgCount;

    /** \brief Number of ActuatorArmed messages BAT has successfully received. */
	uint32             ActuatorArmedMsgCount;
} BAT_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* BAT_MSG_H */

/************************/
/*  End of File Comment */
/************************/
