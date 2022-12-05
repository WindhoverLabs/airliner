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
#include <math.h>
#include <float.h>
#include "cfe.h"
#include "hmc5883_custom.h"
#include "hmc5883_app.h"
#include "hmc5883_msg.h"
#include "hmc5883_version.h"
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HMC5883 oHMC5883;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HMC5883::HMC5883()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HMC5883::~HMC5883()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HMC5883::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    int32 customEventCount = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset(EventTbl, 0x00, sizeof(EventTbl));
    
    /* CFE_EVS_MAX_EVENT_FILTERS limits the number of filters per app.  
     * An explicit CFE_EVS_NO_FILTER (the default) has been provided as
     * an example. */
    EventTbl[  ind].EventID = HMC5883_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = HMC5883_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;
    
    /* Add custom events to the filter table */
    customEventCount = HMC5883_Custom_Init_EventFilters(ind, EventTbl);
    
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
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HMC5883::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            HMC5883_SCH_PIPE_DEPTH,
            HMC5883_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(HMC5883_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, HMC5883_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to HMC5883_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(HMC5883_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, HMC5883_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to HMC5883_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create SCH pipe (0x%08lX)",
             iStatus);
        goto HMC5883_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            HMC5883_CMD_PIPE_DEPTH,
            HMC5883_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(HMC5883_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                 "CMD Pipe failed to subscribe to HMC5883_CMD_MID. (0x%08lX)",
                 iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create CMD pipe (0x%08lX)",
             iStatus);
        goto HMC5883_InitPipe_Exit_Tag;
    }

HMC5883_InitPipe_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, HMC5883_HK_TLM_MID, sizeof(HkTlm), TRUE);
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            HMC5883_DIAG_TLM_MID, sizeof(HMC5883_DiagPacket_t), TRUE);
    /* Init output messages */
    CFE_SB_InitMsg(&SensorMagMsg, PX4_SENSOR_MAG_MID, 
            sizeof(PX4_SensorMagMsg_t), TRUE);
    /* Init custom data */
    HMC5883_Custom_InitData();
    /* Set initial values for calibration */
    UpdateParamsFromTable();
    /* Set sane internal calibration values */
    Diag.Calibration.x_scale_internal = 1.0f;
    Diag.Calibration.y_scale_internal = 1.0f;
    Diag.Calibration.z_scale_internal = 1.0f;
    /* Conversion values */
    Diag.Conversion.ConfigA   = (HMC5883_BITS_CONFIG_A_DEFAULT | HMC5983_TEMP_SENSOR_ENABLE);
    Diag.Conversion.ConfigB   = HMC5883_BITS_CONFIG_B_RANGE_1GA9;
    /* Set range and scale */
    Diag.Conversion.Range     = HMC5883_CALC_MAG_RANGE;
    Diag.Conversion.Scaling   = (HMC5883_MAG_UNIT / HMC5883_MAG_DIVIDER);
    Diag.Conversion.Unit      = HMC5883_MAG_UNIT;
    Diag.Conversion.Divider   = HMC5883_MAG_DIVIDER;

    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HMC5883 initialization                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HMC5883::InitApp()
{
    int32  iStatus     = CFE_SUCCESS;
    int8   hasEvents   = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to init events (0x%08lX)\n", iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto HMC5883_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto HMC5883_InitApp_Exit_Tag;
    }

    InitData();

    returnBool = HMC5883_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                "Custom init failed");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = HMC5883_Custom_ValidateID();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed validate ID");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = SelfCalibrate(&Diag.Calibration);
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed calibration");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = HMC5883_Custom_Set_Range(Diag.Conversion.ConfigB);
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed set range");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = HMC5883_Custom_Check_Range(Diag.Conversion.ConfigB);
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed check range");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = HMC5883_Custom_Set_Config(Diag.Conversion.ConfigA);
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed set config");
        goto HMC5883_InitApp_Exit_Tag;
    }
    returnBool = HMC5883_Custom_Check_Config(Diag.Conversion.ConfigA);
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 Device failed check config");
        goto HMC5883_InitApp_Exit_Tag;
    }

    /* Get the rotation from custom for diag */
    HMC5883_Get_Rotation(&Diag.Calibration.Rotation);

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&HMC5883_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

HMC5883_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_INF_EID, CFE_EVS_INFORMATION,
                                "Initialized.  Version %d.%d.%d.%d",
                                HMC5883_MAJOR_VERSION,
                                HMC5883_MINOR_VERSION,
                                HMC5883_REVISION,
                                HMC5883_MISSION_REV);
        /* Set the app state to initialized */
        oHMC5883.HkTlm.State = HMC5883_INITIALIZED;
    }
    else
    {
        if (hasEvents == 1)
        {
            CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            CFE_ES_WriteToSysLog("HMC5883 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HMC5883::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case HMC5883_WAKEUP_MID:
            {
                ReadDevice();
                SendSensorMagMsg();
                break;
            }
            case HMC5883_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(HMC5883_MSGID_ERR_EID, CFE_EVS_ERROR,
                    "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_RCVMSG_ERR_EID, CFE_EVS_ERROR,
            "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::ProcessCmdPipe()
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
                case HMC5883_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(HMC5883_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(HMC5883_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }

    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process HMC5883 Commands                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case HMC5883_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(HMC5883_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    HMC5883_MAJOR_VERSION,
                    HMC5883_MINOR_VERSION,
                    HMC5883_REVISION,
                    HMC5883_MISSION_REV);
                break;
            }
            case HMC5883_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case HMC5883_SEND_DIAG_CC:
            {
                SendDiag();
                break;
            }
            case HMC5883_SET_CALIBRATION_CC:
            {
                if(CFE_SUCCESS == UpdateCalibrationValues((HMC5883_SetCalibrationCmd_t *) MsgPtr))
                {
                    UpdateParamsFromTable();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(HMC5883_CALIBRATE_INF_EID, CFE_EVS_INFORMATION,
                                  "Calibration values updated");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(HMC5883_CALIBRATE_ERR_EID, CFE_EVS_ERROR,
                                  "Calibration values failed to update");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(HMC5883_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send HMC5883 Housekeeping                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883::ReportHousekeeping()
{
    memcpy(&HkTlm.SensorMagMsg, &SensorMagMsg, sizeof(SensorMagMsg));
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::SendSensorMagMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorMagMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorMagMsg);
    return;
}


void HMC5883::SendDiag()
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
boolean HMC5883::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(HMC5883_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* HMC5883 Application C style main entry point.                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void HMC5883_AppMain()
{
    oHMC5883.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HMC5883 Application C++ style main entry point.                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(HMC5883_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(HMC5883_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::ReadDevice(void)
{
    boolean returnBool      = FALSE;
    static uint8 temp_count = 0;
    int16 temp              = 0;
    uint64 timeStamp        = PX4LIB_GetPX4TimeUs();
    float xraw_f            = 0;
    float yraw_f            = 0;
    float zraw_f            = 0;

    /* Timestamp */
    SensorMagMsg.Timestamp = timeStamp;

    /* Mag */
    returnBool = HMC5883_Custom_Measure(&SensorMagMsg.XRaw, &SensorMagMsg.YRaw, &SensorMagMsg.ZRaw);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }
    
    xraw_f = SensorMagMsg.XRaw;
    yraw_f = SensorMagMsg.YRaw;
    zraw_f = SensorMagMsg.ZRaw;
    
    /* Apply internal calibration before rotation */
    xraw_f = xraw_f * Diag.Calibration.x_scale_internal;
    yraw_f = yraw_f * Diag.Calibration.y_scale_internal;
    zraw_f = zraw_f * Diag.Calibration.z_scale_internal;

    /* Apply any rotation */
    returnBool = HMC5883_Apply_Platform_Rotation(&xraw_f, &yraw_f, &zraw_f);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    /* Apply rotation to raw report as well */
    SensorMagMsg.XRaw = xraw_f;
    SensorMagMsg.YRaw = yraw_f;
    SensorMagMsg.ZRaw = zraw_f;
    
    /* Apply unit conversion */
    SensorMagMsg.X = xraw_f * (Diag.Conversion.Unit / Diag.Conversion.Divider);
    SensorMagMsg.Y = yraw_f * (Diag.Conversion.Unit / Diag.Conversion.Divider);
    SensorMagMsg.Z = zraw_f * (Diag.Conversion.Unit / Diag.Conversion.Divider);

    /* Appy calibration */
    SensorMagMsg.X = (SensorMagMsg.X - m_Params.x_offset) * m_Params.x_scale;
    SensorMagMsg.Y = (SensorMagMsg.Y - m_Params.y_offset) * m_Params.y_scale;
    SensorMagMsg.Z = (SensorMagMsg.Z - m_Params.z_offset) * m_Params.z_scale;

    /* Set range */
    SensorMagMsg.Range = Diag.Conversion.Range;
    /* Set scale */
    SensorMagMsg.Scaling = (Diag.Conversion.Unit / Diag.Conversion.Divider);
    
    /* Measure temperature every 10 mag samples */
    if(temp_count == 10)
    {
        returnBool = HMC5883_Custom_Measure_Temp(&temp);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }
        
        SensorMagMsg.Temperature = 25 + (temp / (16 * 8.0f));
        temp_count = 0;
    }
    else
    {
        temp_count++;
    }

end_of_function:
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Run Self-Calibration Routine                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean HMC5883::SelfCalibrate(HMC5883_CalibrationMsg_t *Calibration)
{
    uint8 range         = 0;
    uint8 config        = 0;
    uint8 i             = 0;
    uint8 good_count    = 0;
    int16 rawX          = 0;
    int16 rawY          = 0;
    int16 rawZ          = 0;
    float rangeScale    = 0.0f;
    boolean returnBool  = FALSE;
    boolean rangeSet    = FALSE;
    boolean configSet   = FALSE;
    
    /* expected axis scaling. The datasheet says that 766 will
     * be places in the X and Y axes and 713 in the Z
     * axis. Experiments show that in fact 766 is placed in X,
     * and 713 in Y and Z. This is relative to a base of 660
     * LSM/Ga, giving 1.16 and 1.08 */
    float expected_cal[3] = { 1.16f, 1.08f, 1.08f };
    float sum_excited[3]  = { 0.0f, 0.0f, 0.0f };
    float cal[3]          = { 0.0f, 0.0f, 0.0f };
    float scaling[3]      = { 0.0f, 0.0f, 0.0f };
    
    /* Save the current range setting */
    returnBool = HMC5883_Custom_Get_Range(&range);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }
    /* Save the current configuration */
    returnBool = HMC5883_Custom_Get_Config(&config);
    if (FALSE == returnBool)
    {
        goto end_of_function;
    }
    /* Set to 2.5 Gauss.*/
    returnBool = HMC5883_Custom_Set_Range(HMC5883_BITS_CONFIG_B_RANGE_2GA5);
    if (FALSE == returnBool)
    {
        rangeSet = TRUE;
        goto end_of_function;
    }
    /* Set the range scale to the range setting above */
    rangeScale = 1.0f / 660.0f;
    /* Set negative bias enable */
    config |= HMC5883_NEG_BIAS_ENABLE;
    returnBool = HMC5883_Custom_Set_Config(config);
    if (FALSE == returnBool)
    {
        configSet = TRUE;
        goto end_of_function;
    }
    /* Set the saved config back to normal */
    config &= ~HMC5883_NEG_BIAS_ENABLE;

    /* Discard 10 samples to let the sensor settle */
    for (i = 0; i < 10; i++) 
    {
        returnBool = HMC5883_Custom_Measure(&rawX, &rawY, &rawZ);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }
    }

    /* read the sensor up to 150x, stopping when we have 50 good values */
    for (i = 0; i < 150 && good_count < 50; i++) 
    {
        returnBool = HMC5883_Custom_Measure(&rawX, &rawY, &rawZ);
        if(FALSE == returnBool)
        {
            goto end_of_function;
        }

        cal[0] = fabsf(expected_cal[0] / (rawX * rangeScale));
        cal[1] = fabsf(expected_cal[1] / (rawY * rangeScale));
        cal[2] = fabsf(expected_cal[2] / (rawZ * rangeScale));

        if (cal[0] > 0.3f && cal[0] < 1.7f &&
            cal[1] > 0.3f && cal[1] < 1.7f &&
            cal[2] > 0.3f && cal[2] < 1.7f) 
        {
            good_count++;
            sum_excited[0] += cal[0];
            sum_excited[1] += cal[1];
            sum_excited[2] += cal[2];
        }
    }

    /* Check that we have at least 5 good values for an average */
    if (good_count < 5) 
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Get the average */
    scaling[0] = sum_excited[0] / good_count;
    scaling[1] = sum_excited[1] / good_count;
    scaling[2] = sum_excited[2] / good_count;

    /* Sanity check scale values */
    returnBool = CheckScale(scaling[0], scaling[1], scaling[2]);
    if (TRUE == returnBool)
    {
        /* Set scaling  */
        Calibration->x_scale_internal  = 1.0f / scaling[0];
        Calibration->y_scale_internal  = 1.0f / scaling[1];
        Calibration->z_scale_internal  = 1.0f / scaling[2];
    }

end_of_function:

    if (TRUE == rangeSet)
    {
        /* return the range setting back to normal */
        HMC5883_Custom_Set_Range(range);
    }
    if (TRUE == configSet)
    {   
        /* return the configuration setting back to normal */
        HMC5883_Custom_Set_Config(config);
    }
    return (returnBool);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Sanity Check Scale Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean HMC5883::CheckScale(float X, float Y, float Z)
{
    boolean returnBool = FALSE;

    if ((-FLT_EPSILON + 1.0f < X && X < FLT_EPSILON + 1.0f) &&
        (-FLT_EPSILON + 1.0f < Y && Y < FLT_EPSILON + 1.0f) &&
        (-FLT_EPSILON + 1.0f < Z && Z < FLT_EPSILON + 1.0f))
    {
        (void) CFE_EVS_SendEvent(HMC5883_SCALE_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 device failed check scale");
    }
    else
    {
        returnBool = TRUE;
    }

    return (returnBool);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Sanity Check Offset Values                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean CheckOffset(float X, float Y, float Z)
{
    boolean returnBool = FALSE;

    if ((-2.0f * FLT_EPSILON < X && X < 2.0f * FLT_EPSILON) &&
        (-2.0f * FLT_EPSILON < Y && Y < 2.0f * FLT_EPSILON) &&
        (-2.0f * FLT_EPSILON < Z && Z < 2.0f * FLT_EPSILON))
    {
        (void) CFE_EVS_SendEvent(HMC5883_OFFSET_ERR_EID, CFE_EVS_ERROR,
            "HMC5883 device failed check offset");
    } 
    else 
    {
        returnBool = TRUE;
    }

    return (returnBool);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Params from the Config Table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883::UpdateParamsFromTable(void)
{
    if(0 != ConfigTblPtr)
    {
        /* Copy to m_Params from the config table */
        m_Params.x_scale = ConfigTblPtr->x_scale;
        m_Params.y_scale = ConfigTblPtr->y_scale;
        m_Params.z_scale = ConfigTblPtr->z_scale;
        m_Params.x_offset = ConfigTblPtr->x_offset;
        m_Params.y_offset = ConfigTblPtr->y_offset;
        m_Params.z_offset = ConfigTblPtr->z_offset;
        /* Copy to diagnostic message */
        Diag.Calibration.x_scale  = m_Params.x_scale;
        Diag.Calibration.y_scale  = m_Params.y_scale;
        Diag.Calibration.z_scale  = m_Params.z_scale;
        Diag.Calibration.x_offset = m_Params.x_offset;
        Diag.Calibration.y_offset = m_Params.y_offset;
        Diag.Calibration.z_offset = m_Params.z_offset;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Calibration Values                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HMC5883::UpdateCalibrationValues(HMC5883_SetCalibrationCmd_t *CalibrationMsgPtr) 
{
    int32 Status = -1;
    
    if(0 != ConfigTblPtr)
    {
        ConfigTblPtr->x_scale = CalibrationMsgPtr->Calibration.x_scale;
        ConfigTblPtr->y_scale = CalibrationMsgPtr->Calibration.y_scale;
        ConfigTblPtr->z_scale = CalibrationMsgPtr->Calibration.z_scale;
        ConfigTblPtr->x_offset = CalibrationMsgPtr->Calibration.x_offset;
        ConfigTblPtr->y_offset = CalibrationMsgPtr->Calibration.y_offset;
        ConfigTblPtr->z_offset = CalibrationMsgPtr->Calibration.z_offset;
        
        Status = CFE_TBL_Modified(ConfigTblHdl);
    }
    
    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup prior to exit                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HMC5883_CleanupCallback(void)
{
    oHMC5883.HkTlm.State = HMC5883_UNINITIALIZED;
    if(HMC5883_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(HMC5883_UNINIT_ERR_EID, CFE_EVS_ERROR,"HMC5883_Uninit failed");
        oHMC5883.HkTlm.State = HMC5883_INITIALIZED;
    }
    return;
}


/************************/
/*  End of File Comment */
/************************/
