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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include "cfe.h"
#include "cfs_utils.h"
#include "mpu6050_app.h"
#include "mpu6050_msg.h"
#include "mpu6050_version.h"
#include "math/Vector3F.hpp"
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU6050 oMPU6050;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU6050::MPU6050() :
    _accel_filter_x(MPU6050_ACCEL_SAMPLE_RATE, MPU6050_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_y(MPU6050_ACCEL_SAMPLE_RATE, MPU6050_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_z(MPU6050_ACCEL_SAMPLE_RATE, MPU6050_ACCEL_FILTER_CUTOFF_FREQ),
    _gyro_filter_x(MPU6050_GYRO_SAMPLE_RATE, MPU6050_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_y(MPU6050_GYRO_SAMPLE_RATE, MPU6050_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_z(MPU6050_GYRO_SAMPLE_RATE, MPU6050_GYRO_FILTER_CUTOFF_FREQ),
    _accel_int(MPU6050_ACCEL_INT_PUB_RATE, TRUE),
    _gyro_int(MPU6050_GYRO_INT_PUB_RATE, TRUE)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU6050::~MPU6050()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU6050::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    int32 customEventCount = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset(EventTbl, 0x00, sizeof(EventTbl));
    
    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    EventTbl[  ind].EventID = MPU6050_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    /* TODO add back in after debug. */
    //EventTbl[  ind].EventID = MPU6050_READ_ERR_EID;
    //EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Add custom events to the filter table */
    customEventCount = MPU6050_Custom_Init_EventFilters(ind, EventTbl);
    
    if(-1 == customEventCount)
    {
        iStatus = CFE_EVS_APP_FILTER_OVERLOAD;
        (void) CFE_ES_WriteToSysLog("Failed to init custom event filters (0x%08X)\n", (unsigned int)iStatus);
        goto end_of_function;
    }

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, (ind + customEventCount), CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU6050 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU6050::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            MPU6050_SCH_PIPE_DEPTH,
            MPU6050_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MPU6050_MEASURE_MID, SchPipeId, CFE_SB_Default_Qos, MPU6050_MEASURE_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU6050_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to MPU6050_MEASURE_MID. (0x%08lX)",
                    iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MPU6050_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, MPU6050_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU6050_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to MPU6050_SEND_HK_MID. (0x%08X)",
                    (unsigned int)iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPU6050_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create SCH pipe (0x%08lX)",
            iStatus);
        goto MPU6050_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            MPU6050_CMD_PIPE_DEPTH,
            MPU6050_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MPU6050_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU6050_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to MPU6050_CMD_MID. (0x%08lX)",
                iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPU6050_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create CMD pipe (0x%08lX)",
            iStatus);
        goto MPU6050_InitPipe_Exit_Tag;
    }

MPU6050_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU6050::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, MPU6050_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            MPU6050_DIAG_TLM_MID, sizeof(MPU6050_DiagPacket_t), TRUE);

    /* Init output message accelerometer */
    CFE_SB_InitMsg(&SensorAccel,
            PX4_SENSOR_ACCEL_MID, sizeof(PX4_SensorAccelMsg_t), TRUE);

    /* Init output message gyroscope */
    CFE_SB_InitMsg(&SensorGyro,
            PX4_SENSOR_GYRO_MID, sizeof(PX4_SensorGyroMsg_t), TRUE);

    /* Set a sane initial value, should be overwritten in SetAccScale */
    Diag.Conversion.AccDivider      = 1.0;
    /* Set a sane initial value, should be overwritten in SetGyroScale */
    Diag.Conversion.GyroDivider     = 1.0;
    Diag.Conversion.AccUnit         = MPU6050_ONE_G;
    Diag.Conversion.GyroUnit        = MPU6050_RADIANS_PER_DEGREE;
    /* These initial scale values determine the calculated range and 
     * divider. See SetAccScale */
    Diag.Conversion.AccScale        = MPU6050_ACC_SCALE;
    Diag.Conversion.GyroScale       = MPU6050_GYRO_SCALE;
    /* Temperature */
    Diag.Conversion.RoomTempOffset  = MPU6050_ROOM_TEMP_OFFSET;
    Diag.Conversion.TempSensitivity = MPU6050_TEMP_SENS;
    /* Start initialization of user calibration values */
    UpdateParamsFromTable();
    /* Initialize the sample queue struct. */
    memset(&MPU6050_SampleQueue, 0, sizeof(MPU6050_SampleQueue));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPU6050 initialization                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU6050::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU6050 - Failed to init events (0x%08lX)\n", iStatus);
        goto MPU6050_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPU6050_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPU6050_InitApp_Exit_Tag;
    }

    InitData();

    returnBool = MPU6050_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_ERROR,
                "Custom init failed");
        goto MPU6050_InitApp_Exit_Tag;
    }

    returnBool = ValidateDevice();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_ERROR,
                "Validate device failed");
        goto MPU6050_InitApp_Exit_Tag;
    }

    returnBool = MPU6050_SetAccScale(Diag.Conversion.AccScale, &Diag.Conversion.AccDivider);
    if(FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_ERROR,
                "Set accelerometer scale failed");
        goto MPU6050_InitApp_Exit_Tag;
    }

    returnBool = MPU6050_SetGyroScale(Diag.Conversion.GyroScale, &Diag.Conversion.GyroDivider);
    if(FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_ERROR,
                "Set gyroscope scale failed");
        goto MPU6050_InitApp_Exit_Tag;
    }

    HkTlm.State = MPU6050_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&MPU6050_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto MPU6050_InitApp_Exit_Tag;
    }

    /* Get the rotation from custom for diag */
    MPU6050_Get_Rotation(&Diag.Calibration.Rotation);

MPU6050_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MPU6050_INIT_INF_EID, CFE_EVS_INFORMATION,
                                "Initialized. Version %d.%d.%d.%d",
                                MPU6050_MAJOR_VERSION,
                                MPU6050_MINOR_VERSION,
                                MPU6050_REVISION,
                                MPU6050_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("MPU6050 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MPU6050::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;
    boolean returnBool = FALSE;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case MPU6050_MEASURE_MID:
            {
                returnBool = ReadDevice();
                if(TRUE == returnBool)
                {
                    SendSensorGyro();
                    SendSensorAccel();
                }
                break;
            }

            case MPU6050_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, do nothing here, 
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* If there's no incoming message within a specified time (via 
         * the iBlocking arg, you can do nothing here.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPU6050_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU6050::ProcessCmdPipe()
{
    int32 iStatus             = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr = NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MPU6050_CMD_MID:
                {
                    ProcessAppCmds(CmdMsgPtr);
                    break;
                }
                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    break;
                }
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(MPU6050_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MPU6050 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU6050::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MPU6050_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(MPU6050_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    MPU6050_MAJOR_VERSION,
                    MPU6050_MINOR_VERSION,
                    MPU6050_REVISION,
                    MPU6050_MISSION_REV);
                break;
            }
            case MPU6050_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case MPU6050_SEND_DIAG_CC:
            {
                SendDiag();
                break;
            }
            case MPU6050_SET_CALIBRATION_CC:
            {
                if(CFE_SUCCESS == UpdateCalibrationValues((MPU6050_SetCalibrationCmd_t *) MsgPtr))
                {
                    UpdateParamsFromTable();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(MPU6050_CALIBRATE_INF_EID, CFE_EVS_INFORMATION,
                                  "Calibration values updated");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MPU6050_CALIBRATE_ERR_EID, CFE_EVS_ERROR,
                                  "Calibration values failed to update");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MPU6050_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MPU6050 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU6050::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU6050::SendSensorAccel()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorAccel);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorAccel);
    return;
}

void MPU6050::SendSensorGyro()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorGyro);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorGyro);
    return;
}


void MPU6050::SendDiag()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&Diag);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&Diag);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MPU6050::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPU6050 Application C style main entry point.                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void MPU6050_AppMain()
{
    oMPU6050.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPU6050 Application C++ style main entry point.                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU6050::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU6050 - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MPU6050_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(MPU6050_SCH_PIPE_PEND_TIME);
        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MPU6050::ReadDevice(void)
{
    float rawX_f       = 0;
    float rawY_f       = 0;
    float rawZ_f       = 0;
    float calX_f       = 0;
    float calY_f       = 0;
    float calZ_f       = 0;
    CFE_TIME_SysTime_t timeStamp;
    int16 rawTemp      = 0;
    int16 calTemp      = 0;
    boolean returnBool = TRUE;
    math::Vector3F gval;
    math::Vector3F gval_integrated;
    math::Vector3F aval;
    math::Vector3F aval_integrated;
    uint16 i = 0;
    static boolean firstReadFlag = FALSE;

    /* Set integrals to zero. */
    SensorGyro.XIntegral = 0;
    SensorGyro.YIntegral = 0;
    SensorGyro.ZIntegral = 0;
    SensorAccel.XIntegral = 0;
    SensorAccel.YIntegral = 0;
    SensorAccel.ZIntegral = 0;

    /* Get a timestamp */
    timeStamp = CFE_TIME_GetTime();

    /* Set measurement timestamps */
    CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&SensorGyro, timeStamp);
    CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&SensorAccel, timeStamp);

    /* Get a measurement. */
    returnBool = MPU6050_Measure(&MPU6050_SampleQueue);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    for(i = 0; i < MPU6050_SampleQueue.SampleCount; ++i)
    {
        SensorGyro.XRaw = MPU6050_SampleQueue.Samples[i].GX;
        SensorGyro.YRaw = MPU6050_SampleQueue.Samples[i].GY;
        SensorGyro.ZRaw = MPU6050_SampleQueue.Samples[i].GZ;
        SensorAccel.XRaw = MPU6050_SampleQueue.Samples[i].AX;
        SensorAccel.YRaw = MPU6050_SampleQueue.Samples[i].AY;
        SensorAccel.ZRaw = MPU6050_SampleQueue.Samples[i].AZ;
        rawTemp = MPU6050_SampleQueue.Samples[i].Temp;

        rawX_f = SensorGyro.XRaw;
        rawY_f = SensorGyro.YRaw;
        rawZ_f = SensorGyro.ZRaw;

        returnBool = MPU6050_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }

        /* Gyro unit conversion */
        calX_f = rawX_f * (Diag.Conversion.GyroUnit / Diag.Conversion.GyroDivider);
        calY_f = rawY_f * (Diag.Conversion.GyroUnit / Diag.Conversion.GyroDivider);
        calZ_f = rawZ_f * (Diag.Conversion.GyroUnit / Diag.Conversion.GyroDivider); 

        /* Gyro Calibrate */
        calX_f = (calX_f - m_Params.GyroXOffset) * m_Params.GyroXScale;
        calY_f = (calY_f - m_Params.GyroYOffset) * m_Params.GyroYScale;
        calZ_f = (calZ_f - m_Params.GyroZOffset) * m_Params.GyroZScale;

        /* Gyro Filter */
        SensorGyro.X = _gyro_filter_x.apply(calX_f);
        SensorGyro.Y = _gyro_filter_y.apply(calY_f);
        SensorGyro.Z = _gyro_filter_z.apply(calZ_f);

        /* Gyro Integrate */
        gval[0] = SensorGyro.X;
        gval[1] = SensorGyro.Y;
        gval[2] = SensorGyro.Z;
        gval_integrated[0] = 0.0f;
        gval_integrated[1] = 0.0f;
        gval_integrated[2] = 0.0f;

        _gyro_int.put_with_interval(MPU6050_SampleQueue.SampleIntervalUs, 
                gval, gval_integrated, SensorGyro.IntegralDt);

        /* Sum rotation rate. */
        SensorGyro.XIntegral += gval_integrated[0];
        SensorGyro.YIntegral += gval_integrated[1];
        SensorGyro.ZIntegral += gval_integrated[2];

        /* Gyro Scale, Range, DeviceID */
        SensorGyro.Scaling = (Diag.Conversion.GyroUnit / Diag.Conversion.GyroDivider);
        SensorGyro.Range   = (Diag.Conversion.AccScale * Diag.Conversion.GyroUnit);

        /* TODO deviceID */
        /* SensorGyro.DeviceID = MPU6050_GYRO_PX4_DEVICE_ID; */

        /* Accel */
        rawX_f = SensorAccel.XRaw;
        rawY_f = SensorAccel.YRaw;
        rawZ_f = SensorAccel.ZRaw;

        returnBool = MPU6050_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }
        /* Accel unit conversion */
        calX_f = rawX_f * (Diag.Conversion.AccUnit / Diag.Conversion.AccDivider);
        calY_f = rawY_f * (Diag.Conversion.AccUnit / Diag.Conversion.AccDivider);
        calZ_f = rawZ_f * (Diag.Conversion.AccUnit / Diag.Conversion.AccDivider);

        /* Accel Calibrate */
        calX_f = (calX_f - m_Params.AccXOffset) * m_Params.AccXScale;
        calY_f = (calY_f - m_Params.AccYOffset) * m_Params.AccYScale;
        calZ_f = (calZ_f - m_Params.AccZOffset) * m_Params.AccZScale;

        /* Accel Filter */
        SensorAccel.X = _accel_filter_x.apply(calX_f);
        SensorAccel.Y = _accel_filter_y.apply(calY_f);
        SensorAccel.Z = _accel_filter_z.apply(calZ_f);

        /* Accel Integrate */
        aval[0] = SensorAccel.X;
        aval[1] = SensorAccel.Y;
        aval[2] = SensorAccel.Z;
        aval_integrated[0] = 0.0f;
        aval_integrated[1] = 0.0f;
        aval_integrated[2] = 0.0f;

        _accel_int.put_with_interval(MPU6050_SampleQueue.SampleIntervalUs, 
                aval, aval_integrated, SensorAccel.IntegralDt);

        /* Sum of velocity. */
        SensorAccel.XIntegral += aval_integrated[0];
        SensorAccel.YIntegral += aval_integrated[1];
        SensorAccel.ZIntegral += aval_integrated[2];

        /* Accel Scale, Range, DeviceID */
        SensorAccel.Scaling = (Diag.Conversion.AccUnit / Diag.Conversion.AccDivider);
        SensorAccel.Range_m_s2 = (Diag.Conversion.AccScale * Diag.Conversion.AccUnit);

        /* TODO deviceID */
        /* SensorAccel.DeviceID = MPU6050_ACCEL_PX4_DEVICE_ID; */

        /* Temperature */
        SensorGyro.TemperatureRaw = SensorAccel.TemperatureRaw = (int16) rawTemp;
        calTemp = (SensorAccel.TemperatureRaw / Diag.Conversion.TempSensitivity) + 36.53f - Diag.Conversion.RoomTempOffset;
        SensorGyro.Temperature  = calTemp;
        SensorAccel.Temperature = calTemp;
    }

end_of_function:

    /* First read should fail with fifo overflow. */
    if(FALSE == returnBool && firstReadFlag != FALSE)
    {
        (void) CFE_EVS_SendEvent(MPU6050_READ_ERR_EID, CFE_EVS_ERROR,
                "MPU6050 read failed");
    }
    firstReadFlag = TRUE;
    return returnBool;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate the Device via the Device ID                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MPU6050::ValidateDevice(void)
{
    uint8 value = 0;
    boolean returnBool =  TRUE;
    
    returnBool = MPU6050_Read_WhoAmI(&value);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }
    if (MPU6050_DEVICE_ID != value)
    {
        (void) CFE_EVS_SendEvent(MPU6050_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "MPU6050 device ID match failed");
        returnBool = FALSE;
    }

end_of_function:
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MPU6050_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "MPU6050 validate failed");
    }
    return (returnBool);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU6050_CleanupCallback(void)
{
    oMPU6050.HkTlm.State = MPU6050_UNINITIALIZED;
    if(MPU6050_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(MPU6050_UNINIT_ERR_EID, CFE_EVS_ERROR,"MPU6050_Uninit failed");
        oMPU6050.HkTlm.State = MPU6050_INITIALIZED;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Params from the Config Table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU6050::UpdateParamsFromTable(void)
{
    if(0 != ConfigTblPtr)
    {
        /* Copy to m_Params from the config table */
        m_Params.AccXScale      = ConfigTblPtr->AccXScale;
        m_Params.AccYScale      = ConfigTblPtr->AccYScale;
        m_Params.AccZScale      = ConfigTblPtr->AccZScale;
        m_Params.AccXOffset     = ConfigTblPtr->AccXOffset;
        m_Params.AccYOffset     = ConfigTblPtr->AccYOffset;
        m_Params.AccZOffset     = ConfigTblPtr->AccZOffset;
        m_Params.GyroXScale     = ConfigTblPtr->GyroXScale;
        m_Params.GyroYScale     = ConfigTblPtr->GyroYScale;
        m_Params.GyroZScale     = ConfigTblPtr->GyroZScale;
        m_Params.GyroXOffset    = ConfigTblPtr->GyroXOffset;
        m_Params.GyroYOffset    = ConfigTblPtr->GyroYOffset;
        m_Params.GyroZOffset    = ConfigTblPtr->GyroZOffset;
        
        /* Copy to the diag message for downlink */
        Diag.Calibration.AccXScale       = m_Params.AccXScale;
        Diag.Calibration.AccYScale       = m_Params.AccYScale;
        Diag.Calibration.AccZScale       = m_Params.AccZScale;
        Diag.Calibration.AccXOffset      = m_Params.AccXOffset;
        Diag.Calibration.AccYOffset      = m_Params.AccYOffset;
        Diag.Calibration.AccZOffset      = m_Params.AccZOffset;
        Diag.Calibration.GyroXScale      = m_Params.GyroXScale;
        Diag.Calibration.GyroYScale      = m_Params.GyroYScale;
        Diag.Calibration.GyroZScale      = m_Params.GyroZScale;
        Diag.Calibration.GyroXOffset     = m_Params.GyroXOffset;
        Diag.Calibration.GyroYOffset     = m_Params.GyroYOffset;
        Diag.Calibration.GyroZOffset     = m_Params.GyroZOffset;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Calibration Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU6050::UpdateCalibrationValues(MPU6050_SetCalibrationCmd_t *CalibrationMsgPtr) 
{
    int32 Status = -1;
    
    if(0 != ConfigTblPtr)
    {
        ConfigTblPtr->AccXScale = CalibrationMsgPtr->Calibration.AccXScale;
        ConfigTblPtr->AccYScale = CalibrationMsgPtr->Calibration.AccYScale;
        ConfigTblPtr->AccZScale = CalibrationMsgPtr->Calibration.AccZScale;
        ConfigTblPtr->AccXOffset = CalibrationMsgPtr->Calibration.AccXOffset;
        ConfigTblPtr->AccYOffset = CalibrationMsgPtr->Calibration.AccYOffset;
        ConfigTblPtr->AccZOffset = CalibrationMsgPtr->Calibration.AccZOffset;
        ConfigTblPtr->GyroXScale = CalibrationMsgPtr->Calibration.GyroXScale;
        ConfigTblPtr->GyroYScale = CalibrationMsgPtr->Calibration.GyroYScale;
        ConfigTblPtr->GyroZScale = CalibrationMsgPtr->Calibration.GyroZScale;
        ConfigTblPtr->GyroXOffset = CalibrationMsgPtr->Calibration.GyroXOffset;
        ConfigTblPtr->GyroYOffset = CalibrationMsgPtr->Calibration.GyroYOffset;
        ConfigTblPtr->GyroZOffset = CalibrationMsgPtr->Calibration.GyroZOffset;
        
        Status = CFE_TBL_Modified(ConfigTblHdl);
    }
    
    return Status;
}

/************************/
/*  End of File Comment */
/************************/
