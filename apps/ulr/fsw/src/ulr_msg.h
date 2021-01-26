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

#ifndef ULR_MSG_H
#define ULR_MSG_H

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
** ULR Command Codes
*************************************************************************/

/** \ulrcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the ULR task is alive
**
**  \ulrcmdmnemonic \ULR_NOOP
**
**  \par Command Structure
**       #ULR_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \ULR_CMDACPTCNT - command counter will increment
**       - The #ULR_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \ULR_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #ULR_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #ULR_RESET_CC
*/
#define ULR_NOOP_CC                 (0)

/** \ulrcmd Reset Counters
**  
**  \par Description
**       Resets the ULR housekeeping counters
**
**  \ulrcmdmnemonic \ULR_TLMRST
**
**  \par Command Structure
**       #ULR_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \ULR_CMDACTPCNT       - command counter will be cleared
**       - \b \c \ULR_CMDRJCTCNT       - command error counter will be cleared
**       - The #ULR_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \ULR_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #ULR_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #ULR_NOOP_CC
*/
#define ULR_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #ULR_NOOP_CC, #ULR_RESET_CC
**  Also see #ULR_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} ULR_NoArgCmd_t;

/** 
**  \brief ULR application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8                    TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \ulrtlmmnemonic \ULR_CMDACPTCNT
        \brief Count of accepted commands */
    uint8                    usCmdCnt;

    /** \ulrtlmmnemonic \ULR_CMDRJCTCNT
        \brief Count of failed commands */
    uint8                    usCmdErrCnt;

    /** \brief Minimum distance that the device should be valid. */
    float                    MinDistance;

    /** \brief Maximum distance that the device should be valid. */
    float                    MaxDistance;

    /** \brief Current distance measured by the device. */
    float                    CurrentDistance;

    /** \brief Covariance of the measurement.  See #ULR_SENS_VARIANCE. */
    float                    Covariance;

    /** \brief Type of the sensor.  This will always be #PX4_DISTANCE_SENSOR_RADAR. */
    PX4_DistanceSensorType_t Type;

    /** \brief ID of the sensor.  See #ULR_SENSOR_ID. */
    uint32                   SensorID;

    /** \brief Orientation of the sensor.  See #PX4_SENSOR_ORIENTATION_ROLL_180. */
    PX4_SensorOrientation_t  SensorOrientation;

} ULR_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* ULR_MSG_H */

/************************/
/*  End of File Comment */
/************************/
