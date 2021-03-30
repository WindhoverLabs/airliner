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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include "cfe.h"
#include "sed_app.h"
#include "sed_msg.h"
#include "sed_version.h"
#include "math/Vector3F.hpp"
#include "px4lib.h"
#include "px4lib_msgids.h"
#include "cfs_utils.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SED oSED;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SED::SED() :
    _accel_filter_x(SED_ACCEL_SAMPLE_RATE, SED_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_y(SED_ACCEL_SAMPLE_RATE, SED_ACCEL_FILTER_CUTOFF_FREQ),
    _accel_filter_z(SED_ACCEL_SAMPLE_RATE, SED_ACCEL_FILTER_CUTOFF_FREQ),
    _gyro_filter_x(SED_GYRO_SAMPLE_RATE, SED_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_y(SED_GYRO_SAMPLE_RATE, SED_GYRO_FILTER_CUTOFF_FREQ),
    _gyro_filter_z(SED_GYRO_SAMPLE_RATE, SED_GYRO_FILTER_CUTOFF_FREQ),
    _accel_int(SED_ACCEL_INT_PUB_RATE, TRUE),
    _gyro_int(SED_GYRO_INT_PUB_RATE, TRUE)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor                                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SED::~SED()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SED::InitEvent()
{
    int32  iStatus = CFE_SUCCESS;
    int32  ind     = 0;

    CFE_EVS_BinFilter_t EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset(EventTbl, 0x00, sizeof(EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    EventTbl[  ind].EventID = SED_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = SED_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, ind, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SED - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SED::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            SED_SCH_PIPE_DEPTH,
            SED_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SED_MEASURE_MID, SchPipeId, CFE_SB_Default_Qos, SED_MEASURE_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to SED_MEASURE_MID. (0x%08lX)",
                    iStatus);
            goto SED_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(SED_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, SED_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to SED_SEND_HK_MID. (0x%08X)",
                    (unsigned int)iStatus);
            goto SED_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SED_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create SCH pipe (0x%08lX)",
            iStatus);
        goto SED_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            SED_CMD_PIPE_DEPTH,
            SED_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SED_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to SED_CMD_MID. (0x%08lX)",
                iStatus);
            goto SED_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SED_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create CMD pipe (0x%08lX)",
            iStatus);
        goto SED_InitPipe_Exit_Tag;
    }

SED_InitPipe_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SED::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, SED_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            SED_DIAG_TLM_MID, sizeof(SED_DiagPacket_t), TRUE);

    /* Init output message accelerometer */
    CFE_SB_InitMsg(&SensorAccel,
            PX4_SENSOR_ACCEL_MID, sizeof(PX4_SensorAccelMsg_t), TRUE);

    /* Init output message gyroscope */
    CFE_SB_InitMsg(&SensorGyro,
            PX4_SENSOR_GYRO_MID, sizeof(PX4_SensorGyroMsg_t), TRUE);

    /* Start initialization of user calibration values */
    UpdateParamsFromTable();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SED initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SED::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SED - Failed to init events (0x%08lX)\n", iStatus);
        goto SED_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto SED_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto SED_InitApp_Exit_Tag;
    }

    InitData();

    /* Initialize Mailbox. */
    MboxConfigPtr = XMbox_LookupConfig(XPAR_SED_MBOX_MAILBOX_CPD_TO_SED_IF_1_DEVICE_ID);
    if(MboxConfigPtr == (XMbox_Config *)NULL)
    {
        /* TODO update to event. */
        OS_printf("XMbox_LookupConfig Failed %u.\n", Status);
        iStatus = -1;
        goto end_of_function;
    }

    Status = XMbox_CfgInitialize(&Mbox, 
                                 MboxConfigPtr, 
                                 MboxConfigPtr->BaseAddress);
    if (Status != XST_SUCCESS)
    {
        /* TODO update to event. */
        OS_printf("XMbox_CfgInitialize Failed %u.\n", Status);
        iStatus = -1;
        goto end_of_function;
    }

    /* Reset the FIFOS. */
    XMbox_ResetFifos(&SBN_Mailbox_Data.Mbox);

    HkTlm.State = SED_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&SED_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SED_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto SED_InitApp_Exit_Tag;
    }

    /* Get the rotation for diag */
    SED_Get_Rotation(&Diag.Calibration.Rotation);

SED_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SED_INIT_INF_EID, CFE_EVS_INFORMATION,
                                "Initialized. Version %d.%d.%d.%d",
                                SED_MAJOR_VERSION,
                                SED_MINOR_VERSION,
                                SED_REVISION,
                                SED_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("SED - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SED::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SED_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SED_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case SED_MEASURE_MID:
            {
                ReadDevice();
                SendSensorGyro();
                SendSensorAccel();
                break;
            }

            case SED_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(SED_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(SED_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SED::ProcessCmdPipe()
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
                case SED_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(SED_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(SED_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process SED Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SED::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SED_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(SED_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    SED_MAJOR_VERSION,
                    SED_MINOR_VERSION,
                    SED_REVISION,
                    SED_MISSION_REV);
                break;
            }
            case SED_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case SED_SEND_DIAG_CC:
            {
                SendDiag();
                break;
            }
            case SED_SET_CALIBRATION_CC:
            {
                if(CFE_SUCCESS == UpdateCalibrationValues((SED_SetCalibrationCmd_t *) MsgPtr))
                {
                    UpdateParamsFromTable();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(SED_CALIBRATE_INF_EID, CFE_EVS_INFORMATION,
                                  "Calibration values updated");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(SED_CALIBRATE_ERR_EID, CFE_EVS_ERROR,
                                  "Calibration values failed to update");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(SED_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send SED Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SED::ReportHousekeeping()
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
void SED::SendSensorAccel()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorAccel);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorAccel);
    return;
}


void SED::SendSensorGyro()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorGyro);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorGyro);
    return;
}


void SED::SendDiag()
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
boolean SED::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(SED_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* SED Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void SED_AppMain()
{
    oSED.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SED Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SED::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SED - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SED_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SED_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SED_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SED_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(SED_SCH_PIPE_PEND_TIME);
        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SED_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SED::ReadDevice(void)
{
    float rawX_f       = 0;
    float rawY_f       = 0;
    float rawZ_f       = 0;
    float calX_f       = 0;
    float calY_f       = 0;
    float calZ_f       = 0;
    CFE_TIME_SysTime_t timeStamp;
    uint16 rawTemp     = 0;
    int16 calTemp      = 0;
    uint16 i           = 0;
    boolean returnBool = TRUE;
    math::Vector3F gval;
    math::Vector3F gval_integrated;
    math::Vector3F aval;
    math::Vector3F aval_integrated;
    static boolean firstReadFlag = FALSE;

    /* Set integrals to zero. */
    SensorGyro.XIntegral  = 0;
    SensorGyro.YIntegral  = 0;
    SensorGyro.ZIntegral  = 0;
    SensorAccel.XIntegral = 0;
    SensorAccel.YIntegral = 0;
    SensorAccel.ZIntegral = 0;

    /* Get a timestamp */
    timeStamp = CFE_TIME_GetTime();

    /* Set measurement timestamps */
    CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&SensorGyro, timeStamp);
    CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&SensorAccel, timeStamp);

    /* Discard the first measurements. */
    if(firstReadFlag == FALSE)
    {
        firstReadFlag = TRUE;
        XMbox_ResetFifos(&SBN_Mailbox_Data.Mbox);
        goto end_of_function;
    }

    /* Get measurements. */
    returnBool = GetMeasurements();
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    for(i = 0; i < SampleQueue.SampleCount; ++i)
    {
        SensorGyro.XRaw = SampleQueue.Samples[i].GX;
        SensorGyro.YRaw = SampleQueue.Samples[i].GY;
        SensorGyro.ZRaw = SampleQueue.Samples[i].GZ;
        SensorAccel.XRaw = SampleQueue.Samples[i].AX;
        SensorAccel.YRaw = SampleQueue.Samples[i].AY;
        SensorAccel.ZRaw = SampleQueue.Samples[i].AZ;
        //rawTemp = SED_SampleQueue.Samples[i].Temp;

        rawX_f = SensorGyro.XRaw;
        rawY_f = SensorGyro.YRaw;
        rawZ_f = SensorGyro.ZRaw;

        returnBool = SED_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }

        /* Gyro unit conversion */
        calX_f = rawX_f * (ConfigTblPtr->GyroUnit / ConfigTblPtr->GyroDivider);
        calY_f = rawY_f * (ConfigTblPtr->GyroUnit / ConfigTblPtr->GyroDivider);
        calZ_f = rawZ_f * (ConfigTblPtr->GyroUnit / ConfigTblPtr->GyroDivider); 

        /* Gyro Calibrate */
        calX_f = (calX_f - ConfigTblPtr->GyroXOffset) * ConfigTblPtr->GyroXScale;
        calY_f = (calY_f - ConfigTblPtr->GyroYOffset) * ConfigTblPtr->GyroYScale;
        calZ_f = (calZ_f - ConfigTblPtr->GyroZOffset) * ConfigTblPtr->GyroZScale;

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

        _gyro_int.put_with_interval(SampleQueue.SampleIntervalUs, 
                gval, gval_integrated, SensorGyro.IntegralDt);

        /* Sum rotation rate. */
        SensorGyro.XIntegral += gval_integrated[0];
        SensorGyro.YIntegral += gval_integrated[1];
        SensorGyro.ZIntegral += gval_integrated[2];

        /* Gyro Scale, Range, DeviceID */
        SensorGyro.Scaling = (ConfigTblPtr->GyroUnit / ConfigTblPtr->GyroDivider);
        SensorGyro.Range   = (ConfigTblPtr->AccScale * ConfigTblPtr->GyroUnit);

        /* TODO deviceID */
        /* SensorGyro.DeviceID = SED_GYRO_PX4_DEVICE_ID; */

        /* Accel */
        rawX_f = SensorAccel.XRaw;
        rawY_f = SensorAccel.YRaw;
        rawZ_f = SensorAccel.ZRaw;

        returnBool = SED_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }

        /* Accel unit conversion */
        calX_f = rawX_f * (ConfigTblPtr->AccUnit / ConfigTblPtr->AccDivider);
        calY_f = rawY_f * (ConfigTblPtr->AccUnit / ConfigTblPtr->AccDivider);
        calZ_f = rawZ_f * (ConfigTblPtr->AccUnit / ConfigTblPtr->AccDivider);

        /* Accel Calibrate */
        calX_f = (calX_f - ConfigTblPtr->AccXOffset) * ConfigTblPtr->AccXScale;
        calY_f = (calY_f - ConfigTblPtr->AccYOffset) * ConfigTblPtr->AccYScale;
        calZ_f = (calZ_f - ConfigTblPtr->AccZOffset) * ConfigTblPtr->AccZScale;

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

        _accel_int.put_with_interval(SampleQueue.SampleIntervalUs, 
                aval, aval_integrated, SensorAccel.IntegralDt);

        /* Sum of velocity. */
        SensorAccel.XIntegral += aval_integrated[0];
        SensorAccel.YIntegral += aval_integrated[1];
        SensorAccel.ZIntegral += aval_integrated[2];

        /* Accel Scale, Range, DeviceID */
        SensorAccel.Scaling = (ConfigTblPtr->AccUnit / ConfigTblPtr->AccDivider);
        SensorAccel.Range_m_s2 = (ConfigTblPtr->AccScale * ConfigTblPtr->AccUnit);

        /* TODO deviceID */
        /* SensorAccel.DeviceID = SED_ACCEL_PX4_DEVICE_ID; */

        /* Temperature */
        //SensorGyro.TemperatureRaw = SensorAccel.TemperatureRaw = (int16) rawTemp;
        //calTemp = (SensorAccel.TemperatureRaw / ConfigTblPtr->TempSensitivity) + ConfigTblPtr->TempOffset;
        //SensorGyro.Temperature  = calTemp;
        //SensorAccel.Temperature = calTemp;
    }

end_of_function:

    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(SED_READ_ERR_EID, CFE_EVS_ERROR,
                "SED read failed");
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SED_CleanupCallback(void)
{
    oSED.HkTlm.State = SED_UNINITIALIZED;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Params from the Config Table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SED::UpdateParamsFromTable(void)
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

        /* Also copy the conversion values. */
        Diag.Conversion.AccDivider       = ConfigTblPtr->AccDivider;
        Diag.Conversion.AccScale         = ConfigTblPtr->AccScale;
        Diag.Conversion.AccUnit          = ConfigTblPtr->AccUnit;
        Diag.Conversion.GyroDivider      = ConfigTblPtr->GyroDivider;
        Diag.Conversion.GyroScale        = ConfigTblPtr->GyroScale;
        Diag.Conversion.GyroUnit         = ConfigTblPtr->GyroUnit;
        Diag.Conversion.TempOffset       = ConfigTblPtr->TempOffset;
        Diag.Conversion.TempSensitivity  = ConfigTblPtr->TempSensitivity;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Calibration Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SED::UpdateCalibrationValues(SED_SetCalibrationCmd_t *CalibrationMsgPtr) 
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


boolean SED::SED_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        CFE_EVS_SendEvent(SED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "SED Apply_Platform_Rotation Null Pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* ROTATION_ROLL_180 */
    *Y = -*Y; 
    *Z = -*Z;

end_of_function:
    return returnBool;
}


void SED::SED_Get_Rotation(uint8 *Rotation)
{
    /* Null pointer check */
    if(0 == Rotation)
    {
        CFE_EVS_SendEvent(SED_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "SED Get_Rotation Null Pointer");
        goto end_of_function;
    }

    *Rotation = ROTATION_PITCH_180;

end_of_function:
    return;
}


/* Non-blocking read, size in bytes, returns size in bytes. */
int SED::SED_MailboxRead(XMbox *instance, unsigned int *buffer, unsigned int size)
{
    int Status              = 0;
    unsigned int BytesRecvd = 0;

    Status = XMbox_Read(instance, buffer, size, &BytesRecvd);

    if(Status == XST_NO_DATA)
    {
        Status = 0;
        goto end_of_function;
    }

    if(Status == XST_SUCCESS)
    {
        Status = BytesRecvd;
    }
    else
    {
        /* TODO update to event. */
        OS_printf("XMbox_Read Failed %u.\r\n", Status);
    }

end_of_function:
    return Status;
}


boolean SED::GetMeasurements(void)
{
    int SizeRead            = 0;
    uint16 i                = 0;
    uint16 index            = 0;
    boolean ReturnBool      = TRUE;
    boolean SuccessFlag     = TRUE;

    for(i = 0; i < SED_MBOX_MAX_BUFFER_SIZE_WORDS; ++i)
    {
        unsigned int InputBuffer = 0;
        SizeRead = MailboxRead(&Mbox, &InputBuffer, SED_MBOX_WORD_SIZE);

        if(SizeRead > 0)
        {
            unsigned int Size = SED_MBOX_MAX_BUFFER_SIZE_WORDS;
            unsigned int Status = ParseMessage(&Parser,
                                               InputBuffer,
                                               &ParserBuffer[0],
                                               &Size);
            if(Status == MPS_MESSAGE_COMPLETE)
            {
                boolean ProcessStatus = TRUE;

                /* Process the mailbox message. */
                ProcessStatus = ProcessMboxMsg((CFE_SB_Msg_t *)&ParserBuffer[0], index++);
                if(ProcessStatus == FALSE)
                {
                    SuccessFlag = FALSE;
                }
                continue;
            }
        }
        else
        {
            break;
        }
    }

    if(SuccessFlag == FALSE)
    {
        ReturnBool = FALSE;
        goto end_of_function;
    }

    /* If samples were parsed... */
    if(index > 0)
    {
        uint16 SamplesParsed = index + 1;
        /* Calculate filtered fifo samples per cycle. This value should
         * stay close to 4. */
        Diag.FifoSamplesPerCycle = (0.95f * Diag.FifoSamplesPerCycle) + 
                (0.05f * SamplesParsed * sizeof(SED_Measurement_t));
        /* Calculate the number of samples in the fifo queue. */
        SampleQueue.SampleCount = SamplesParsed;
        /* Calculate the sampling interval in micro seconds. */
        SampleQueue.SampleIntervalUs = round(1000 / Diag.FifoSamplesPerCycle);
        /* Calculate high water mark. */
        if(Diag.FifoHighWaterMark < SamplesParsed)
        {
            Diag.FifoHighWaterMark = SamplesParsed;
        }
    }
    else
    {
        ReturnBool = FALSE;
    }

end_of_function:
    return ReturnBool;
}


boolean SED::ProcessMboxMsg(CFE_SB_Msg_t* MsgPtr, uint16 Index)
{
    boolean returnBool = TRUE;
    CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);

    switch(MsgId)
    {
        case IMU_TLM_MSG_ID:
        {
            uint16 ExpectedLength = sizeof(SED_Measurement_t);
            if(VerifyCmdLength(MsgPtr, ExpectedLength))
            {
                SED_Measurement_t *UserDataPtr = (SED_Measurement_t *)CFE_SB_GetUserData(MsgPtr);
                SampleQueue.Samples[Index].GX = UserDataPtr->GX;
                SampleQueue.Samples[Index].GY = UserDataPtr->GY;
                SampleQueue.Samples[Index].GZ = UserDataPtr->GZ;
                SampleQueue.Samples[Index].AX = UserDataPtr->AX;
                SampleQueue.Samples[Index].AY = UserDataPtr->AY;
                SampleQueue.Samples[Index].AZ = UserDataPtr->AZ;
            }
            else
            {
                returnBool = FALSE;
            }
        }
        default:
        {
            OS_printf("SED unknown message ID\n");
            returnBool = FALSE;
        }
    }

    return returnBool;
}


/************************/
/*  End of File Comment */
/************************/
