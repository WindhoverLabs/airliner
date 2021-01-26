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

#ifndef MPU6050_MSG_H
#define MPU6050_MSG_H

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
** MPU6050 Command Codes
*************************************************************************/

/** \mpu6050cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MPU6050 task is alive
**
**  \mpu6050cmdmnemonic \MPU6050_NOOP
**
**  \par Command Structure
**       #MPU6050_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU6050_CMDACPTCNT - command counter will increment
**       - The #MPU6050_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU6050_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU6050_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU6050_RESET_CC
*/
#define MPU6050_NOOP_CC                 (0)

/** \mpu6050cmd Reset Counters
**  
**  \par Description
**       Resets the MPU6050 housekeeping counters
**
**  \mpu6050cmdmnemonic \MPU6050_TLMRST
**
**  \par Command Structure
**       #MPU6050_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU6050_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MPU6050_CMDRJCTCNT       - command error counter will be cleared
**       - The #MPU6050_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU6050_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU6050_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU6050_NOOP_CC
*/
#define MPU6050_RESET_CC                (1)

/** \mpu6050cmd Send Diagnostic Message
**  
**  \par Description
**       Sends a diagnostic message.
**
**  \mpu6050cmdmnemonic \MPU6050_DIAG
**
**  \par Command Structure
**       #MPU6050_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU6050_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MPU6050_CMDRJCTCNT       - command error counter will be cleared
**       - The #MPU6050_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU6050_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU6050_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU6050_SEND_DIAG_CC
*/
#define MPU6050_SEND_DIAG_CC            (2)

/** \mpu6050cmd Set Calibration Values
**  
**  \par Description
**       Sets the sensor calibrations 
**
**  \par Command Structure
**       #MPU6050_SetCalibrationCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU6050_CMDACTPCNT       - command counter will be incremented
**       - The #MPU6050_CALIBRATE_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU6050_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU6050_CALIBRATE_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU6050_SET_CALIBRATION_CC
*/
#define MPU6050_SET_CALIBRATION_CC      (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #MPU6050_NOOP_CC, #MPU6050_RESET_CC
**  Also see #MPU6050_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MPU6050_NoArgCmd_t;


/** 
**  \brief MPU6050 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \mpu6050tlmmnemonic \MPU6050_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \mpu6050tlmmnemonic \MPU6050_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
} MPU6050_HkTlm_t;


/** 
**  \brief MPU6050 application struct for calibration
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
} MPU6050_CalibrationMsg_t;


/** 
**  \brief MPU6050 application struct for unit conversion 
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
    float RoomTempOffset;
    /*! Temperature sensitivity value */
    float TempSensitivity;
    /*! Accel scale value*/
    uint16 AccScale;
    /*! Gyro scale value*/
    uint16 GyroScale;
} MPU6050_ConversionMsg_t;


/** 
**  \brief MPU6050 diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /*! All calibration info */
    MPU6050_CalibrationMsg_t          Calibration;
    /*! All conversion info */
    MPU6050_ConversionMsg_t           Conversion;
} MPU6050_DiagPacket_t;

/** 
**  \brief MPU6050 application struct for calibration cmds
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! All calibration info */
    MPU6050_CalibrationMsg_t Calibration;
} MPU6050_SetCalibrationCmd_t;

#ifdef __cplusplus
}
#endif

#endif /* MPU6050_MSG_H */

/************************/
/*  End of File Comment */
/************************/
