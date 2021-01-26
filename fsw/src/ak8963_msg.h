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

#ifndef AK8963_MSG_H
#define AK8963_MSG_H

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

/************************************************************************
** AK8963 Command Codes
*************************************************************************/

/** \ak8963cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the AK8963 task is alive
**
**  \ak8963cmdmnemonic \AK8963_NOOP
**
**  \par Command Structure
**       #AK8963_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AK8963_CMDACPTCNT - command counter will increment
**       - The #AK8963_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AK8963_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AK8963_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AK8963_RESET_CC
*/
#define AK8963_NOOP_CC                 (0)

/** \ak8963cmd Reset Counters
**  
**  \par Description
**       Resets the AK8963 housekeeping counters
**
**  \ak8963cmdmnemonic \AK8963_TLMRST
**
**  \par Command Structure
**       #AK8963_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AK8963_CMDACTPCNT       - command counter will be cleared
**       - \b \c \AK8963_CMDRJCTCNT       - command error counter will be cleared
**       - The #AK8963_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AK8963_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AK8963_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AK8963_NOOP_CC
*/
#define AK8963_RESET_CC                (1)

/** \ak8963cmd Send Diagnostic Message
**  
**  \par Description
**       Sends a diagnostic message.
**
**  \ak8963cmdmnemonic \AK8963_DIAG
**
**  \par Command Structure
**       #AK8963_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AK8963_CMDACTPCNT       - command counter will be cleared
**       - \b \c \AK8963_CMDRJCTCNT       - command error counter will be cleared
**       - The #AK8963_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AK8963_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AK8963_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AK8963_SEND_DIAG_CC
*/
#define AK8963_SEND_DIAG_CC            (2)

/** \ak8963cmd Set Calibration Values
**  
**  \par Description
**       Sets the sensor calibrations 
**
**  \par Command Structure
**       #AK8963_SetCalibrationCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \AK8963_CMDACTPCNT       - command counter will be incremented
**       - The #AK8963_CALIBRATE_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \AK8963_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #AK8963_CALIBRATE_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #AK8963_SET_CALIBRATION_CC
*/
#define AK8963_SET_CALIBRATION_CC      (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #AK8963_NOOP_CC, #AK8963_RESET_CC
**  Also see #AK8963_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} AK8963_NoArgCmd_t;


/** 
**  \brief AK8963 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \ak8963tlmmnemonic \AK8963_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \ak8963tlmmnemonic \AK8963_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
} AK8963_HkTlm_t;


/** 
**  \brief AK8963 application struct for calibration
*/
typedef struct
{
    /*! Mag X-axis calibration param scale. */
    float MagXScale;
    /*! Mag Y-axis calibration param scale. */
    float MagYScale;
    /*! Mag Z-axis calibration param scale. */
    float MagZScale;
    /*! Mag X-axis calibration param offset. */
    float MagXOffset;
    /*! Mag Y-axis calibration param offset. */
    float MagYOffset;
    /*! Mag Z-axis calibration param offset. */
    float MagZOffset;
    /*! Mag Platform Rotation */
    uint8 MagRotation;
} AK8963_CalibrationMsg_t;


/** 
**  \brief AK8963 application struct for unit conversion 
*/
typedef struct
{
    /*! Mag X-axis sensitivity adjustment value */
    float MagXAdj;
    /*! Mag Y-axis sensitivity adjustment value */
    float MagYAdj;
    /*! Mag Z-axis sensitivity adjustment value */
    float MagZAdj;
} AK8963_ConversionMsg_t;


/** 
**  \brief AK8963 diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /*! All calibration info */
    AK8963_CalibrationMsg_t          Calibration;
    /*! All conversion info */
    AK8963_ConversionMsg_t           Conversion;
} AK8963_DiagPacket_t;


/** 
**  \brief AK8963 application struct for calibration cmds
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! All calibration info */
    AK8963_CalibrationMsg_t Calibration;
} AK8963_SetCalibrationCmd_t;


#ifdef __cplusplus
}
#endif

#endif /* AK8963_MSG_H */

/************************/
/*  End of File Comment */
/************************/
