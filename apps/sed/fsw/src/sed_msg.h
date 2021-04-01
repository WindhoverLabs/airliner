/****************************************************************************
*
*   Copyright (c) 2020 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef SED_MSG_H
#define SED_MSG_H

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
** SED Command Codes
*************************************************************************/

/** \sedcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the SED task is alive
**
**  \sedcmdmnemonic \SED_NOOP
**
**  \par Command Structure
**       #SED_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SED_CMDACPTCNT - command counter will increment
**       - The #SED_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SED_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SED_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SED_RESET_CC
*/
#define SED_NOOP_CC                 (0)

/** \sedcmd Reset Counters
**  
**  \par Description
**       Resets the SED housekeeping counters
**
**  \sedcmdmnemonic \SED_TLMRST
**
**  \par Command Structure
**       #SED_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SED_CMDACTPCNT       - command counter will be cleared
**       - \b \c \SED_CMDRJCTCNT       - command error counter will be cleared
**       - The #SED_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SED_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SED_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SED_NOOP_CC
*/
#define SED_RESET_CC                (1)

/** \sedcmd Send Diagnostic Message
**  
**  \par Description
**       Sends a diagnostic message.
**
**  \sedcmdmnemonic \SED_DIAG
**
**  \par Command Structure
**       #SED_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SED_CMDACTPCNT       - command counter will be cleared
**       - \b \c \SED_CMDRJCTCNT       - command error counter will be cleared
**       - The #SED_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SED_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SED_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SED_SEND_DIAG_CC
*/
#define SED_SEND_DIAG_CC            (2)

/** \sedcmd Set Calibration Values
**  
**  \par Description
**       Sets the sensor calibrations 
**
**  \par Command Structure
**       #SED_SetCalibrationCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \SED_CMDACTPCNT       - command counter will be incremented
**       - The #SED_CALIBRATE_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \SED_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #SED_CALIBRATE_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #SED_SET_CALIBRATION_CC
*/
#define SED_SET_CALIBRATION_CC      (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #SED_NOOP_CC, #SED_RESET_CC
**  Also see #SED_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} SED_NoArgCmd_t;


/** 
**  \brief SED application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \sedtlmmnemonic \SED_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \sedtlmmnemonic \SED_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
} SED_HkTlm_t;


/** 
**  \brief SED application struct for calibration
*/
typedef struct
{
    /*! Acceleration X-axis calibration param scale. */
    float AccXScale;
    /*! Acceleration Y-axis calibration param scale. */
    float AccYScale;
    /*! Acceleration Z-axis calibration param scale. */
    float AccZScale;
    /*! Acceleration X-axis calibration param offset. */
    float AccXOffset;
    /*! Acceleration Y-axis calibration param offset. */
    float AccYOffset;
    /*! Acceleration Z-axis calibration param offset. */
    float AccZOffset;
    /*! Gyro X-axis calibration param scale. */
    float GyroXScale;
    /*! Gyro Y-axis calibration param scale. */
    float GyroYScale;
    /*! Gyro Z-axis calibration param scale. */
    float GyroZScale;
    /*! Gyro X-axis calibration param offset. */
    float GyroXOffset;
    /*! Gyro Y-axis calibration param offset. */
    float GyroYOffset;
    /*! Gyro Z-axis calibration param offset. */
    float GyroZOffset;
    /*! Platform rotation. */
    uint8 Rotation;
} SED_CalibrationMsg_t;


/** 
**  \brief SED application struct for unit conversion 
*/
typedef struct
{
    /*! Accel unit value */
    float AccUnit;
    /*! Gyro unit value */
    float GyroUnit;
    /*! Accel divider value */
    float AccDivider;
    /*! Gyro divider value */
    float GyroDivider;
    /*! Room temperature offset value */
    float TempOffset;
    /*! Temperature sensitivity value */
    float TempSensitivity;
    /*! Accel scale value*/
    uint16 AccScale;
    /*! Gyro scale value*/
    uint16 GyroScale;
} SED_ConversionMsg_t;


/** 
**  \brief SED diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /*! All calibration info */
    SED_CalibrationMsg_t          Calibration;
    /*! All conversion info */
    SED_ConversionMsg_t           Conversion;
    /*! Filtered fifo samples per cycle. */
    float FifoSamplesPerCycle;
    /*! Fifo high water mark. */
    uint16 FifoHighWaterMark;
} SED_DiagPacket_t;

/** 
**  \brief SED application struct for calibration cmds
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! All calibration info */
    SED_CalibrationMsg_t Calibration;
} SED_SetCalibrationCmd_t;

typedef struct
{
    int16 AX; 
    int16 AY; 
    int16 AZ;
    int16 GX; 
    int16 GY; 
    int16 GZ;
} SED_Measurement_t;

typedef struct
{
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    SED_Measurement_t Sample;
} SED_MeasurementMsg_t;


#ifdef __cplusplus
}
#endif

#endif /* SED_MSG_H */

/************************/
/*  End of File Comment */
/************************/
