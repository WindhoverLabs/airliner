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

#ifndef HMC5883_MSG_H
#define HMC5883_MSG_H

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
#include "px4_msgs.h"


/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** HMC5883 Command Codes
*************************************************************************/

/** \hmc5883cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the HMC5883 task is alive
**
**  \hmc5883cmdmnemonic \HMC5883_NOOP
**
**  \par Command Structure
**       #HMC5883_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \HMC5883_CMDACPTCNT - command counter will increment
**       - The #HMC5883_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \HMC5883_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #HMC5883_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #HMC5883_NOOP_CC
*/
#define HMC5883_NOOP_CC                 (0)

/** \hmc5883cmd Reset Counters
**  
**  \par Description
**       Resets the HMC5883 housekeeping counters
**
**  \hmc5883cmdmnemonic \HMC5883_TLMRST
**
**  \par Command Structure
**       #HMC5883_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \HMC5883_CMDACTPCNT       - command counter will be cleared
**       - \b \c \HMC5883_CMDRJCTCNT       - command error counter will be cleared
**       - The #HMC5883_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \HMC5883_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #HMC5883_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #HMC5883_RESET_CC
*/
#define HMC5883_RESET_CC                (1)

/** \hmc5883cmd Send Diagnostic Message
**  
**  \par Description
**       Sends a diagnostic message.
**
**  \hmc5883cmdmnemonic \HMC5883_DIAG
**
**  \par Command Structure
**       #HMC5883_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \HMC5883_CMDACPTCNT - command counter will increment
**       - The #HMC5883_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \HMC5883_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #HMC5883_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #HMC5883_SEND_DIAG_CC
*/
#define HMC5883_SEND_DIAG_CC            (2)

/** \hmc5883cmd Set Calibration Values
**  
**  \par Description
**       Sets the sensor calibrations 
**
**  \par Command Structure
**       #HMC5883_SetCalibrationCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \HMC5883_CMDACTPCNT       - command counter will be incremented
**       - The #HMC5883_CALIBRATE_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \HMC5883_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #HMC5883_CALIBRATE_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #HMC5883_SET_CALIBRATION_CC
*/
#define HMC5883_SET_CALIBRATION_CC      (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #HMC5883_NOOP_CC, #HMC5883_RESET_CC
**  Also see #HMC5883_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} HMC5883_NoArgCmd_t;

/** 
**  \brief HMC5883 calibration data
*/
typedef struct
{
    /** \brief Current external X-axis calibration */
    float              x_scale;
    /** \brief Current external Y-axis calibration */
    float              y_scale;
    /** \brief Current external Z-axis calibration */
    float              z_scale;
    /** \brief Current external X-axis offset */
    float              x_offset;
    /** \brief Current external Y-axis offset */
    float              y_offset;
    /** \brief Current external Z-axis offset */
    float              z_offset;
    /** \brief Current platform rotation */
    uint8              Rotation;
    /** \brief Current internal X-axis self-calibration */
    float              x_scale_internal;
    /** \brief Current internal Y-axis self-calibration */
    float              y_scale_internal;
    /** \brief Current internal Z-axis self-calibration */
    float              z_scale_internal;
} HMC5883_CalibrationMsg_t;

/** 
**  \brief HMC5883 conversion data
*/
typedef struct
{
    /** \brief Current range value */
    float              Range;
    /** \brief Current scale value */
    float              Scaling;
    /** \brief Current unit value */
    float              Unit;
    /** \brief Current divider value */
    float              Divider;
    /** \brief Current Device configuration register A value */
    uint8              ConfigA;
    /** \brief Current Device configuration register B value */
    uint8              ConfigB;
} HMC5883_ConversionMsg_t;

/** 
**  \brief HMC5883 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /** \hmc5883tlmmnemonic \HMC5883_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   
    /** \hmc5883tlmmnemonic \HMC5883_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
    /** \brief SensorMag message */
    PX4_SensorMagMsg_t SensorMagMsg;
} HMC5883_HkTlm_t;

/** 
**  \brief HMC5883 diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /** \brief Calibration data */
    HMC5883_CalibrationMsg_t          Calibration;
    /** \brief Conversion data */
    HMC5883_ConversionMsg_t           Conversion;
} HMC5883_DiagPacket_t;


/** 
**  \brief HMC5883 application struct for calibration cmd
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! All calibration info */
    HMC5883_CalibrationMsg_t Calibration;
} HMC5883_SetCalibrationCmd_t;

#ifdef __cplusplus
}
#endif

#endif /* HMC5883_MSG_H */

/************************/
/*  End of File Comment */
/************************/
