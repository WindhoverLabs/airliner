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
#include "mpu9250_custom.h"
#include "mpu9250_app.h"
#include "mpu9250_msg.h"
#include "mpu9250_version.h"
#include "math/Vector3F.hpp"
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU9250 oMPU9250;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU9250::MPU9250() :
    _accel_filter_x(MPU9250_ACCEL_SAMPLE_RATE, MPU9250_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_y(MPU9250_ACCEL_SAMPLE_RATE, MPU9250_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_z(MPU9250_ACCEL_SAMPLE_RATE, MPU9250_ACCEL_FILTER_CUTOFF_FREQ),
    _gyro_filter_x(MPU9250_GYRO_SAMPLE_RATE, MPU9250_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_y(MPU9250_GYRO_SAMPLE_RATE, MPU9250_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_z(MPU9250_GYRO_SAMPLE_RATE, MPU9250_GYRO_FILTER_CUTOFF_FREQ),
    //_accel_int(MPU9250_NEVER_AUTOPUBLISH_US, FALSE),
    //_gyro_int(MPU9250_NEVER_AUTOPUBLISH_US, TRUE)
    _accel_int(MPU9250_ACCEL_INT_PUB_RATE, TRUE),
    _gyro_int(MPU9250_GYRO_INT_PUB_RATE, TRUE)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPU9250::~MPU9250()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU9250::InitEvent()
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
    EventTbl[  ind].EventID = MPU9250_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = MPU9250_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Add custom events to the filter table */
    customEventCount = MPU9250_Custom_Init_EventFilters(ind, EventTbl);
    
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
        (void) CFE_ES_WriteToSysLog("MPU9250 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU9250::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            MPU9250_SCH_PIPE_DEPTH,
            MPU9250_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MPU9250_MEASURE_MID, SchPipeId, CFE_SB_Default_Qos, MPU9250_MEASURE_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU9250_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to MPU9250_MEASURE_MID. (0x%08lX)",
                    iStatus);
            goto MPU9250_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MPU9250_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, MPU9250_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU9250_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to MPU9250_SEND_HK_MID. (0x%08X)",
                    (unsigned int)iStatus);
            goto MPU9250_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPU9250_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create SCH pipe (0x%08lX)",
            iStatus);
        goto MPU9250_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            MPU9250_CMD_PIPE_DEPTH,
            MPU9250_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MPU9250_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPU9250_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to MPU9250_CMD_MID. (0x%08lX)",
                iStatus);
            goto MPU9250_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPU9250_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create CMD pipe (0x%08lX)",
            iStatus);
        goto MPU9250_InitPipe_Exit_Tag;
    }

    ///* Init param pipe and subscribe to param updated messages */
    //iStatus = CFE_SB_CreatePipe(&ParamPipeId,
                                //MPU9250_PARAM_PIPE_DEPTH,
                                //MPU9250_PARAM_PIPE_NAME);
    //if (iStatus == CFE_SUCCESS)
    //{
        ///* Subscribe to data messages */
        //iStatus = CFE_SB_Subscribe(PRMLIB_PARAM_UPDATED_MID, ParamPipeId);

        //if (iStatus != CFE_SUCCESS)
        //{
            //(void) CFE_EVS_SendEvent(MPU9250_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                                     //"DATA Pipe failed to subscribe to PRMLIB_PARAM_UPDATED_MID. (0x%08X)",
                                     //(unsigned int)iStatus);
            //goto MPU9250_InitPipe_Exit_Tag;
        //}
    //}
    //else
    //{
        //(void) CFE_EVS_SendEvent(MPU9250_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                                 //"Failed to create Data pipe (0x%08X)",
                                 //(unsigned int)iStatus);
        //goto MPU9250_InitPipe_Exit_Tag;
    //}

MPU9250_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU9250::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, MPU9250_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            MPU9250_DIAG_TLM_MID, sizeof(MPU9250_DiagPacket_t), TRUE);

    /* Init output message accelerometer */
    CFE_SB_InitMsg(&SensorAccel,
            PX4_SENSOR_ACCEL_MID, sizeof(PX4_SensorAccelMsg_t), TRUE);

    ///* Init output message magnetometer */
    //CFE_SB_InitMsg(&SensorMag,
            //PX4_SENSOR_MAG_MID, sizeof(PX4_SensorMagMsg_t), TRUE);

    /* Init output message gyroscope */
    CFE_SB_InitMsg(&SensorGyro,
            PX4_SENSOR_GYRO_MID, sizeof(PX4_SensorGyroMsg_t), TRUE);

    /* Set a sane initial value, should be overwritten in SetAccScale */
    Diag.Conversion.AccDivider      = 1.0;
    /* Set a sane initial value, should be overwritten in SetGyroScale */
    Diag.Conversion.GyroDivider     = 1.0;
    Diag.Conversion.AccUnit         = MPU9250_ONE_G;
    Diag.Conversion.GyroUnit        = MPU9250_RADIANS_PER_DEGREE;
    /* These initial scale values determine the calculated range and 
     * divider. See SetAccScale */
    Diag.Conversion.AccScale        = MPU9250_ACC_SCALE;
    Diag.Conversion.GyroScale       = MPU9250_GYRO_SCALE;
    /* Temperature */
    Diag.Conversion.RoomTempOffset  = MPU9250_ROOM_TEMP_OFFSET;
    Diag.Conversion.TempSensitivity = MPU9250_TEMP_SENS;
    /* Start initialization of user calibration values */
    UpdateParamsFromTable();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPU9250 initialization                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU9250::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU9250 - Failed to init events (0x%08lX)\n", iStatus);
        goto MPU9250_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPU9250_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPU9250_InitApp_Exit_Tag;
    }

    InitData();

    returnBool = MPU9250_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                "Custom init failed");
        goto MPU9250_InitApp_Exit_Tag;
    }

    returnBool = ValidateDevice();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                "Validate device failed");
        goto MPU9250_InitApp_Exit_Tag;
    }

    returnBool = MPU9250_SetAccScale(Diag.Conversion.AccScale, &Diag.Conversion.AccDivider);
    if(FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                "Set accelerometer scale failed");
        goto MPU9250_InitApp_Exit_Tag;
    }

    returnBool = MPU9250_SetGyroScale(Diag.Conversion.GyroScale, &Diag.Conversion.GyroDivider);
    if(FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                "Set gyroscope scale failed");
        goto MPU9250_InitApp_Exit_Tag;
    }
    ///*  Get the factory magnetometer sensitivity adjustment values */
    //returnBool = MPU9250_Read_MagAdj(&Diag.Conversion.MagXAdj, 
            //&Diag.Conversion.MagYAdj, &Diag.Conversion.MagZAdj);
    //if(FALSE == returnBool)
    //{
        //iStatus = -1;
        // (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                //"Get Mag adjustment values failed.");
        //goto MPU9250_InitApp_Exit_Tag;
    //}
    
    //iStatus = OS_MutSemCreate(&m_Params_Mutex, MPU9250_MUTEX_PARAMS, 0);
    //if (iStatus != CFE_SUCCESS)
    //{
        // (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
            //"Params mutex create failed");
        //returnBool = FALSE;
        //goto MPU9250_InitApp_Exit_Tag;
    //}
    
    HkTlm.State = MPU9250_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&MPU9250_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MPU9250_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto MPU9250_InitApp_Exit_Tag;
    }

    /* Get the rotation from custom for diag */
    MPU9250_Get_Rotation(&Diag.Calibration.Rotation);

MPU9250_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MPU9250_INIT_INF_EID, CFE_EVS_INFORMATION,
                                "Initialized. Version %d.%d.%d.%d",
                                MPU9250_MAJOR_VERSION,
                                MPU9250_MINOR_VERSION,
                                MPU9250_REVISION,
                                MPU9250_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("MPU9250 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MPU9250::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU9250_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU9250_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case MPU9250_MEASURE_MID:
            {
                //static uint32 i = 0;

                ReadDevice();
                SendSensorGyro();
                SendSensorAccel();
                //if(i >= 4)
                //{
                    //SendSensorMag();
                    //i = 0;
                //}
                //else
                //{
                    //++i;
                //}
                break;
            }

            case MPU9250_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(MPU9250_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(MPU9250_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU9250::ProcessCmdPipe()
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
                case MPU9250_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(MPU9250_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MPU9250_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MPU9250 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU9250::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MPU9250_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(MPU9250_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    MPU9250_MAJOR_VERSION,
                    MPU9250_MINOR_VERSION,
                    MPU9250_REVISION,
                    MPU9250_MISSION_REV);
                break;
            }
            case MPU9250_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case MPU9250_SEND_DIAG_CC:
            {
                SendDiag();
                break;
            }
            case MPU9250_SET_CALIBRATION_CC:
            {
                if(CFE_SUCCESS == UpdateCalibrationValues((MPU9250_SetCalibrationCmd_t *) MsgPtr))
                {
                    UpdateParamsFromTable();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(MPU9250_CALIBRATE_INF_EID, CFE_EVS_INFORMATION,
                                  "Calibration values updated");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MPU9250_CALIBRATE_ERR_EID, CFE_EVS_ERROR,
                                  "Calibration values failed to update");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MPU9250_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MPU9250 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MPU9250::ReportHousekeeping()
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
void MPU9250::SendSensorAccel()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorAccel);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorAccel);
    return;
}

//void MPU9250::SendSensorMag()
//{
    //static uint32 i = 0;
    //if(i >= 4)
    //{
        //CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorMag);
        //CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorMag);
        //i = 0;
    //}
    //++i;
    //return;
//}

void MPU9250::SendSensorGyro()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorGyro);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorGyro);
    return;
}


void MPU9250::SendDiag()
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
boolean MPU9250::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(MPU9250_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* MPU9250 Application C style main entry point.                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void MPU9250_AppMain()
{
    oMPU9250.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPU9250 Application C++ style main entry point.                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU9250::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU9250 - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU9250_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MPU9250_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MPU9250_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MPU9250_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(MPU9250_SCH_PIPE_PEND_TIME);
        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU9250_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU9250::ReadDevice(void)
{
    float rawX_f       = 0;
    float rawY_f       = 0;
    float rawZ_f       = 0;
    float calX_f       = 0;
    float calY_f       = 0;
    float calZ_f       = 0;
    uint64 timeStamp   = 0;
    uint16 rawTemp     = 0;
    int16 calTemp      = 0;
    boolean returnBool = TRUE;
    //float magXAdj_f = Diag.Calibration.MagXAdj;
    //float magYAdj_f = Diag.Calibration.MagYAdj;
    //float magZAdj_f = Diag.Calibration.MagZAdj;
    math::Vector3F gval;
    math::Vector3F gval_integrated;
    math::Vector3F aval;
    math::Vector3F aval_integrated;

    /* Get a timestamp */
    timeStamp = CFE_TIME_GetTimeInMicros();

    /* Set measurement timestamps */
    //SensorMag.Timestamp   = timeStamp;
    SensorGyro.Timestamp  = timeStamp;
    SensorAccel.Timestamp = timeStamp;

    /* Gyro */
    returnBool = MPU9250_Read_Gyro(&SensorGyro.XRaw, &SensorGyro.YRaw, &SensorGyro.ZRaw);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    rawX_f = SensorGyro.XRaw;
    rawY_f = SensorGyro.YRaw;
    rawZ_f = SensorGyro.ZRaw;

    returnBool = MPU9250_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
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

    _gyro_int.put(timeStamp, gval, gval_integrated, SensorGyro.IntegralDt);
    
    SensorGyro.XIntegral = gval_integrated[0];
    SensorGyro.YIntegral = gval_integrated[1];
    SensorGyro.ZIntegral = gval_integrated[2];
    
    /* Gyro Scale, Range, DeviceID */
    SensorGyro.Scaling = (Diag.Conversion.GyroUnit / Diag.Conversion.GyroDivider);
    SensorGyro.Range   = (Diag.Conversion.AccScale * Diag.Conversion.GyroUnit);

    /* TODO deviceID */
    SensorGyro.DeviceID = MPU9250_GYRO_PX4_DEVICE_ID;

    /* Accel */
    returnBool = MPU9250_Read_Accel(&SensorAccel.XRaw, &SensorAccel.YRaw, &SensorAccel.ZRaw);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    rawX_f = SensorAccel.XRaw;
    rawY_f = SensorAccel.YRaw;
    rawZ_f = SensorAccel.ZRaw;

    returnBool = MPU9250_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
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

    _accel_int.put(timeStamp, aval, aval_integrated, SensorAccel.IntegralDt);
    
    SensorAccel.XIntegral = aval_integrated[0];
    SensorAccel.YIntegral = aval_integrated[1];
    SensorAccel.ZIntegral = aval_integrated[2];

    /* Accel Scale, Range, DeviceID */
    SensorAccel.Scaling = (Diag.Conversion.AccUnit / Diag.Conversion.AccDivider);
    SensorAccel.Range_m_s2 = (Diag.Conversion.AccScale * Diag.Conversion.AccUnit);

    /* TODO deviceID */
    SensorAccel.DeviceID = MPU9250_ACCEL_PX4_DEVICE_ID;
    ///* Mag */
    //returnBool = MPU9250_Read_Mag(&SensorMag.XRaw, &SensorMag.YRaw, &SensorMag.ZRaw);
    //if(FALSE == returnBool)
    //{
        //goto end_of_function;
    //}

    //rawX_f = (float) SensorMag.XRaw;
    //rawY_f = (float) SensorMag.YRaw;
    //rawZ_f = (float) SensorMag.ZRaw;

    //returnBool = MPU9250_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
    //if(FALSE == returnBool)
    //{
        //goto end_of_function;
    //}

    ///* Mag Convert and Calibrate */
    //SensorMag.X = ((rawX_f * ((((Diag.Calibration.MagXAdj - 128.0f) * 0.5f) / 128.0f) + 1.0) * m_Params.MagXScale) + m_Params.MagXOffset) / 1000.0f;
    //SensorMag.Y = ((rawY_f * ((((Diag.Calibration.MagYAdj - 128.0f) * 0.5f) / 128.0f) + 1.0) * m_Params.MagYScale) + m_Params.MagYOffset) / 1000.0f;
    //SensorMag.Z = ((rawZ_f * ((((Diag.Calibration.MagZAdj - 128.0f) * 0.5f) / 128.0f) + 1.0) * m_Params.MagZScale) + m_Params.MagZOffset) / 1000.0f;

    /////* Mag Scale, Range, DeviceID */
    //SensorMag.Scaling = -1.0f;
    //SensorMag.Range = -1.0f;
    ///* TODO deviceID */
    //SensorMag.DeviceID = MPU9250_PX4_DEVICE_ID;

    /* Temperature */
    returnBool = MPU9250_Read_Temp(&rawTemp);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    SensorGyro.TemperatureRaw = SensorAccel.TemperatureRaw = (int16) rawTemp;

    calTemp = (SensorAccel.TemperatureRaw / Diag.Conversion.TempSensitivity) + 21.0 - Diag.Conversion.RoomTempOffset;
    //SensorMag.Temperature   = calTemp;
    SensorGyro.Temperature  = calTemp;
    SensorAccel.Temperature = calTemp;

end_of_function:

    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MPU9250_READ_ERR_EID, CFE_EVS_ERROR,
                "MPU9250 read failed");
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate the Device via the Device ID                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MPU9250::ValidateDevice(void)
{
    uint8 value = 0;
    boolean returnBool =  TRUE;
    
    returnBool = MPU9250_Read_WhoAmI(&value);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }
    if (MPU9250_DEVICE_ID != value)
    {
        (void) CFE_EVS_SendEvent(MPU9250_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "MPU9250 device ID match failed");
        returnBool = FALSE;
    }

//    returnBool = MPU9250_Read_MagDeviceID(&value);
//    if(FALSE == returnBool)
//    {
//        goto end_of_function;
//    }
//    if (MPU9250_AK8963_ID != value)
//    {
//        (void) CFE_EVS_SendEvent(MPU9250_VALIDATE_ERR_EID, CFE_EVS_ERROR,
//                "AK8963 device ID match failed.  Returned 0x%02hx", value);
//        returnBool = FALSE;
//    }

end_of_function:
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MPU9250_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "MPU9250 validate failed");
    }
    return (returnBool);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU9250_CleanupCallback(void)
{
    oMPU9250.HkTlm.State = MPU9250_UNINITIALIZED;
    if(MPU9250_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(MPU9250_UNINIT_ERR_EID, CFE_EVS_ERROR,"MPU9250_Uninit failed");
        oMPU9250.HkTlm.State = MPU9250_INITIALIZED;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Params from the Config Table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPU9250::UpdateParamsFromTable(void)
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
        //m_Params.MagXScale      = ConfigTblPtr->MagXScale;
        //m_Params.MagYScale      = ConfigTblPtr->MagYScale;
        //m_Params.MagZScale      = ConfigTblPtr->MagZScale;
        //m_Params.MagXOffset     = ConfigTblPtr->MagXOffset;
        //m_Params.MagYOffset     = ConfigTblPtr->MagYOffset;
        //m_Params.MagZOffset     = ConfigTblPtr->MagZOffset;
        
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
        //Diag.Calibration.MagXScale       = m_Params.MagXScale;
        //Diag.Calibration.MagYScale       = m_Params.MagYScale;
        //Diag.Calibration.MagZScale       = m_Params.MagZScale;
        //Diag.Calibration.MagXOffset      = m_Params.MagXOffset;
        //Diag.Calibration.MagYOffset      = m_Params.MagYOffset;
        //Diag.Calibration.MagZOffset      = m_Params.MagZOffset; 
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Calibration Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPU9250::UpdateCalibrationValues(MPU9250_SetCalibrationCmd_t *CalibrationMsgPtr) 
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
