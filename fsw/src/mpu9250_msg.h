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

#ifndef MPU9250_MSG_H
#define MPU9250_MSG_H

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
** MPU9250 Command Codes
*************************************************************************/

/** \mpu9250cmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the MPU9250 task is alive
**
**  \mpu9250cmdmnemonic \MPU9250_NOOP
**
**  \par Command Structure
**       #MPU9250_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU9250_CMDACPTCNT - command counter will increment
**       - The #MPU9250_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU9250_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU9250_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU9250_RESET_CC
*/
#define MPU9250_NOOP_CC                 (0)

/** \mpu9250cmd Reset Counters
**  
**  \par Description
**       Resets the MPU9250 housekeeping counters
**
**  \mpu9250cmdmnemonic \MPU9250_TLMRST
**
**  \par Command Structure
**       #MPU9250_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU9250_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MPU9250_CMDRJCTCNT       - command error counter will be cleared
**       - The #MPU9250_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU9250_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU9250_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU9250_NOOP_CC
*/
#define MPU9250_RESET_CC                (1)

/** \mpu9250cmd Send Diagnostic Message
**  
**  \par Description
**       Sends a diagnostic message.
**
**  \mpu9250cmdmnemonic \MPU9250_DIAG
**
**  \par Command Structure
**       #MPU9250_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU9250_CMDACTPCNT       - command counter will be cleared
**       - \b \c \MPU9250_CMDRJCTCNT       - command error counter will be cleared
**       - The #MPU9250_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU9250_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU9250_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU9250_SEND_DIAG_CC
*/
#define MPU9250_SEND_DIAG_CC            (2)

/** \mpu9250cmd Set Calibration Values
**  
**  \par Description
**       Sets the sensor calibrations 
**
**  \par Command Structure
**       #MPU9250_SetCalibrationCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \MPU9250_CMDACTPCNT       - command counter will be incremented
**       - The #MPU9250_CALIBRATE_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \MPU9250_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #MPU9250_CALIBRATE_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #MPU9250_SET_CALIBRATION_CC
*/
#define MPU9250_SET_CALIBRATION_CC      (3)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #MPU9250_NOOP_CC, #MPU9250_RESET_CC
**  Also see #MPU9250_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} MPU9250_NoArgCmd_t;


/** 
**  \brief MPU9250 application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \mpu9250tlmmnemonic \MPU9250_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \mpu9250tlmmnemonic \MPU9250_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 
    /** \brief App State */
    uint8              State;
} MPU9250_HkTlm_t;


/** 
**  \brief MPU9250 application struct for calibration
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
    /*! Mag X-axis calibration param scale. */
    //float MagXScale;
    /*! Mag Y-axis calibration param scale. */
    //float MagYScale;
    /*! Mag Z-axis calibration param scale. */
    //float MagZScale;
    /*! Mag X-axis calibration param offset. */
    //float MagXOffset;
    /*! Mag Y-axis calibration param offset. */
    //float MagYOffset;
    /*! Mag Z-axis calibration param offset. */
    //float MagZOffset;
    /*! Mag Platform Rotation */
    //uint8 MagRotation;
} MPU9250_CalibrationMsg_t;


/** 
**  \brief MPU9250 application struct for unit conversion 
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
    /*! Mag X-axis sensitivity adjustment value */
    //uint8 MagXAdj;
    /*! Mag Y-axis sensitivity adjustment value */
    //uint8 MagYAdj;
    /*! Mag Z-axis sensitivity adjustment value */
    //uint8 MagZAdj;
} MPU9250_ConversionMsg_t;


/** 
**  \brief MPU9250 diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /*! All calibration info */
    MPU9250_CalibrationMsg_t          Calibration;
    /*! All conversion info */
    MPU9250_ConversionMsg_t           Conversion;
} MPU9250_DiagPacket_t;

/** 
**  \brief MPU9250 application struct for calibration cmds
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! All calibration info */
    MPU9250_CalibrationMsg_t Calibration;
} MPU9250_SetCalibrationCmd_t;

#ifdef __cplusplus
}
#endif

#endif /* MPU9250_MSG_H */

/************************/
/*  End of File Comment */
/************************/
