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

#ifndef MS5607_MSG_H
#define MS5607_MSG_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief The number of MS5607 coefficients in PROM */
#define MS5607_COEF_SIZE               (8)

/************************************************************************
** MS5607 Command Codes
*************************************************************************/

/** \ms5607cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MS5607 task is alive
**
**  \ms5607cmdmnemonic \MS5607_NOOP
**
**  \par Command Structure
**       #MS5607_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MS5607_CMDACPTCNT - command counter will increment
**       - The #MS5607_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MS5607_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MS5607_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MS5607_NOOP_CC
*/
#define MS5607_NOOP_CC                 (0)

/** \ms5607cmd Reset Counters
**  
**  \par Description
**       Resets the MS5607 housekeeping counters
**
**  \ms5607cmdmnemonic \MS5607_TLMRST
**
**  \par Command Structure
**       #MS5607_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MS5607_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MS5607_CMDRJCTCNT       - command error counter will be cleared
**       - The #MS5607_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MS5607_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MS5607_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MS5607_RESET_CC
*/
#define MS5607_RESET_CC                (1)

/** \ms5607cmd Send Diagnostic Message
**  
**  \par Description
**       Sends the MS5607 diag message
**
**  \ms5607cmdmnemonic \MS5607_DIAG
**
**  \par Command Structure
**       #MS5607_DiagPacket_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MS5607_CMDACPTCNT - command counter will increment
**       - The #MS5607_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MS5607_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MS5607_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MS5607_SEND_DIAG_TLM_CC
*/
#define MS5607_SEND_DIAG_TLM_CC        (2)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #MS5607_NOOP_CC, #MS5607_RESET_CC
**  Also see #MS5607_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MS5607_NoArgCmd_t;


/** 
**  \brief MS5607 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \ms5607tlmmnemonic \MS5607_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \ms5607tlmmnemonic \MS5607_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8                 State;
} MS5607_HkTlm_t;


/** 
**  \brief MS5607 diagnostic data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /** \brief MS5607 PROM Coefficients */
    uint16          Coefficients[MS5607_COEF_SIZE];
    /** \brief Measurement count */
    uint8           MeasureCount;
    /** \brief Raw pressure measurement */
    int32           RawPressure;
    /** \brief Raw temperature measurement */
    int32           RawTemperature;
    /** \brief Scaled pressure measurement */
    float           Pressure;
    /** \brief Scaled temperature measurement */
    float           Temperature;
    /** \brief Calculated altitude measurement */
    float           Altitude;
} MS5607_DiagPacket_t;


#ifdef __cplusplus
}
#endif

#endif /* MS5607_MSG_H */

/************************/
/*  End of File Comment */
/************************/
