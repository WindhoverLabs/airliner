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
#include "ak8963_custom.h"
#include "ak8963_app.h"
#include "ak8963_msg.h"
#include "ak8963_version.h"
#include "math/Vector3F.hpp"
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AK8963 oAK8963;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AK8963::AK8963()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AK8963::~AK8963()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AK8963::InitEvent()
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
    EventTbl[  ind].EventID = AK8963_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = AK8963_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Add custom events to the filter table */
    customEventCount = AK8963_Custom_Init_EventFilters(ind, EventTbl);
    
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
        (void) CFE_ES_WriteToSysLog("AK8963 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AK8963::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            AK8963_SCH_PIPE_DEPTH,
            AK8963_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(AK8963_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, AK8963_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(AK8963_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to AK8963_MEASURE_MID. (0x%08lX)",
                    iStatus);
            goto AK8963_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(AK8963_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, AK8963_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(AK8963_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to AK8963_SEND_HK_MID. (0x%08X)",
                    (unsigned int)iStatus);
            goto AK8963_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(AK8963_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create SCH pipe (0x%08lX)",
            iStatus);
        goto AK8963_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            AK8963_CMD_PIPE_DEPTH,
            AK8963_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(AK8963_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(AK8963_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to AK8963_CMD_MID. (0x%08lX)",
                iStatus);
            goto AK8963_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(AK8963_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create CMD pipe (0x%08lX)",
            iStatus);
        goto AK8963_InitPipe_Exit_Tag;
    }

AK8963_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AK8963::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, AK8963_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            AK8963_DIAG_TLM_MID, sizeof(AK8963_DiagPacket_t), TRUE);

    /* Init output message magnetometer */
    CFE_SB_InitMsg(&SensorMag,
            PX4_SENSOR_MAG_MID, sizeof(PX4_SensorMagMsg_t), TRUE);

    /* Start initialization of user calibration values */
    UpdateParamsFromTable();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AK8963 initialization                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AK8963::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("AK8963 - Failed to init events (0x%08lX)\n", iStatus);
        goto AK8963_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto AK8963_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto AK8963_InitApp_Exit_Tag;
    }

    InitData();

    /* Perform custom initialization. */
    returnBool = AK8963_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
                "Custom init failed");
        goto AK8963_InitApp_Exit_Tag;
    }
    
    /* Validate device ID i.e. read who am i register. */
    returnBool = ValidateDevice();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
                "Validate device failed");
        goto AK8963_InitApp_Exit_Tag;
    }

    /*  Get the factory magnetometer sensitivity adjustment values */
    returnBool = ReadSensitivityAdjustment();
    if(FALSE == returnBool)
    {
        iStatus = -1;
         (void) CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
                "Get Mag sens adjustment values failed.");
        goto AK8963_InitApp_Exit_Tag;
    }
    
    /* Run self-test */
//    returnBool = AK8963_RunSelfTest();
//    if(FALSE == returnBool)
//    {
//        iStatus = -1;
//         (void) CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
//                "Self test failed.");
//        goto AK8963_InitApp_Exit_Tag;
//    }

    /*  Poweron and set measurement mode  */
    returnBool = AK8963_PowerOn();
    if(FALSE == returnBool)
    {
        iStatus = -1;
         (void) CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
                "Power On failed.");
        goto AK8963_InitApp_Exit_Tag;
    }

    HkTlm.State = AK8963_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&AK8963_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(AK8963_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto AK8963_InitApp_Exit_Tag;
    }

    /* Get the rotation from custom for diag */
    AK8963_Get_Rotation(&Diag.Calibration.MagRotation);

AK8963_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(AK8963_INIT_INF_EID, CFE_EVS_INFORMATION,
                                "Initialized. Version %d.%d.%d.%d",
                                AK8963_MAJOR_VERSION,
                                AK8963_MINOR_VERSION,
                                AK8963_REVISION,
                                AK8963_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(ADSBR_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("AK8963 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 AK8963::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(AK8963_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(AK8963_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case AK8963_WAKEUP_MID:
            {
                ReadDevice();
                break;
            }

            case AK8963_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(AK8963_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(AK8963_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void AK8963::ProcessCmdPipe()
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
                case AK8963_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(AK8963_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(AK8963_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process AK8963 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void AK8963::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case AK8963_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(AK8963_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    AK8963_MAJOR_VERSION,
                    AK8963_MINOR_VERSION,
                    AK8963_REVISION,
                    AK8963_MISSION_REV);
                break;
            }
            case AK8963_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case AK8963_SEND_DIAG_CC:
            {
                SendDiag();
                break;
            }
            case AK8963_SET_CALIBRATION_CC:
            {
                if(CFE_SUCCESS == UpdateCalibrationValues((AK8963_SetCalibrationCmd_t *) MsgPtr))
                {
                    UpdateParamsFromTable();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(AK8963_CALIBRATE_INF_EID, CFE_EVS_INFORMATION,
                                  "Calibration values updated");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(AK8963_CALIBRATE_ERR_EID, CFE_EVS_ERROR,
                                  "Calibration values failed to update");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(AK8963_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send AK8963 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void AK8963::ReportHousekeeping()
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
void AK8963::SendSensorMag()
{

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorMag);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorMag);
    return;
}


void AK8963::SendDiag()
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
boolean AK8963::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(AK8963_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* AK8963 Application C style main entry point.                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void AK8963_AppMain()
{
    oAK8963.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AK8963 Application C++ style main entry point.                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AK8963::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("AK8963 - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(AK8963_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(AK8963_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(AK8963_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(AK8963_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(AK8963_SCH_PIPE_PEND_TIME);
        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(AK8963_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AK8963::ReadDevice(void)
{
    float rawX_f       = 0;
    float rawY_f       = 0;
    float rawZ_f       = 0;
    boolean returnBool = TRUE;
    float magXAdj_f = Diag.Conversion.MagXAdj;
    float magYAdj_f = Diag.Conversion.MagYAdj;
    float magZAdj_f = Diag.Conversion.MagZAdj;

    /* Set measurement timestamps */
    SensorMag.Timestamp   = PX4LIB_GetPX4TimeUs();

    /* Read mag, read can fail since device uses continuous measure
     * mode and data may not be ready. */
    returnBool = AK8963_Read_Mag(&SensorMag.XRaw, &SensorMag.YRaw, &SensorMag.ZRaw);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* Mag unit conversion */
    rawX_f = static_cast<float>(SensorMag.XRaw) * magXAdj_f * AK8963_RAW_TO_GAUSS;
    rawY_f = static_cast<float>(SensorMag.YRaw) * magYAdj_f * AK8963_RAW_TO_GAUSS;
    rawZ_f = static_cast<float>(SensorMag.ZRaw) * magZAdj_f * AK8963_RAW_TO_GAUSS;

    /* Apply platform rotation */
    returnBool = AK8963_Apply_Platform_Rotation(&rawX_f, &rawY_f, &rawZ_f);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* Apply any user calibration */
    SensorMag.X = (rawX_f - Diag.Calibration.MagXOffset) * Diag.Calibration.MagXScale;
    SensorMag.Y = (rawY_f - Diag.Calibration.MagYOffset) * Diag.Calibration.MagYScale;
    SensorMag.Z = (rawZ_f - Diag.Calibration.MagZOffset) * Diag.Calibration.MagZScale;

    /* Mag Scale, Range, DeviceID */
    //SensorMag.Scaling = -1.0f;
    //SensorMag.Range = -1.0f;
    //SensorMag.DeviceID = AK8963_PX4_DEVICE_ID;

    /* Send the sensor data */
    SendSensorMag();

end_of_function:

    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate the Device via the Device ID                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean AK8963::ValidateDevice(void)
{
    uint8 value = 0;
    boolean returnBool =  TRUE;

    returnBool = AK8963_Read_MagDeviceID(&value);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }
    if (AK8963_WAI_ID != value)
    {
        (void) CFE_EVS_SendEvent(AK8963_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "AK8963 device ID match failed.  Returned 0x%02hx", value);
        returnBool = FALSE;
    }

end_of_function:
    if(FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(AK8963_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                "AK8963 validate failed");
    }
    return (returnBool);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read the sensitivity adjustment values                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean AK8963::ReadSensitivityAdjustment(void)
{
    boolean returnBool = FALSE;
    uint8 MagXAdj = 0;
    uint8 MagYAdj = 0;
    uint8 MagZAdj = 0;

    returnBool = AK8963_Read_MagAdj(&MagXAdj, &MagYAdj, &MagZAdj);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* H_adj = H x ((ASA-128) x 0.5/128 + 1)
     *       = H x ((ASA-128) / 256 + 1)
     * H is the raw compass reading.
     */
    Diag.Conversion.MagXAdj = (MagXAdj - 128.0f) / 256.0f + 1.0f;
    Diag.Conversion.MagYAdj = (MagYAdj - 128.0f) / 256.0f + 1.0f;
    Diag.Conversion.MagZAdj = (MagZAdj - 128.0f) / 256.0f + 1.0f;

end_of_function:
    return returnBool;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AK8963_CleanupCallback(void)
{
    oAK8963.HkTlm.State = AK8963_UNINITIALIZED;
    if(AK8963_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(AK8963_UNINIT_ERR_EID, CFE_EVS_ERROR,"AK8963_Uninit failed");
        oAK8963.HkTlm.State = AK8963_INITIALIZED;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Params from the Config Table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AK8963::UpdateParamsFromTable(void)
{
    if(0 != ConfigTblPtr)
    {
        /* Copy to m_Params from the config table */
        m_Params.MagXScale      = ConfigTblPtr->MagXScale;
        m_Params.MagYScale      = ConfigTblPtr->MagYScale;
        m_Params.MagZScale      = ConfigTblPtr->MagZScale;
        m_Params.MagXOffset     = ConfigTblPtr->MagXOffset;
        m_Params.MagYOffset     = ConfigTblPtr->MagYOffset;
        m_Params.MagZOffset     = ConfigTblPtr->MagZOffset;

        /* Copy to the diag message */
        Diag.Calibration.MagXScale       = m_Params.MagXScale;
        Diag.Calibration.MagYScale       = m_Params.MagYScale;
        Diag.Calibration.MagZScale       = m_Params.MagZScale;
        Diag.Calibration.MagXOffset      = m_Params.MagXOffset;
        Diag.Calibration.MagYOffset      = m_Params.MagYOffset;
        Diag.Calibration.MagZOffset      = m_Params.MagZOffset; 
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Calibration Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AK8963::UpdateCalibrationValues(AK8963_SetCalibrationCmd_t *CalibrationMsgPtr) 
{
    int32 Status = -1;
    
    if(0 != ConfigTblPtr)
    {
        ConfigTblPtr->MagXScale = CalibrationMsgPtr->Calibration.MagXScale;
        ConfigTblPtr->MagYScale = CalibrationMsgPtr->Calibration.MagYScale;
        ConfigTblPtr->MagZScale = CalibrationMsgPtr->Calibration.MagZScale;
        ConfigTblPtr->MagXOffset = CalibrationMsgPtr->Calibration.MagXOffset;
        ConfigTblPtr->MagYOffset = CalibrationMsgPtr->Calibration.MagYOffset;
        ConfigTblPtr->MagZOffset = CalibrationMsgPtr->Calibration.MagZOffset;
        
        Status = CFE_TBL_Modified(ConfigTblHdl);
    }
    
    return Status;
}


/************************/
/*  End of File Comment */
/************************/
