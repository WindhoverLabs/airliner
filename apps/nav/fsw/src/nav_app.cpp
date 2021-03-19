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
 ** Includes
 *************************************************************************/
#include <string.h>
#include "cfe.h"
#include "nav_app.h"
#include "nav_msg.h"
#include "nav_version.h"
#include <math/Limits.hpp>
#include "px4lib_msgids.h"
#include "cfs_utils.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NAV oNAV;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NAV::NAV()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NAV::~NAV()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::InitEvent()
{
    int32 iStatus = CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog(
                "NAV - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::InitPipe()
{
    int32 iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    NAV_SCH_PIPE_DEPTH,
    NAV_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(NAV_WAKEUP_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 
                                     NAV_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to NAV_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(NAV_SEND_HK_MID, SchPipeId,
                                     CFE_SB_Default_Qos,
                                     NAV_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to NAV_SEND_HK_MID. (0x%08X)",
                    (unsigned int) iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_HOME_POSITION_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_HOME_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MISSION_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_MISSION_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GLOBAL_POSITION_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_GLOBAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_COMMAND_MID, SchPipeId,
                                     CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(NAV_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create SCH pipe (0x%08lX)", iStatus);
        goto NAV_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
                                NAV_CMD_PIPE_DEPTH,
                                NAV_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(NAV_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(NAV_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to NAV_CMD_MID. (0x%08lX)",
                    iStatus);
            goto NAV_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(NAV_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create CMD pipe (0x%08lX)", iStatus);
        goto NAV_InitPipe_Exit_Tag;
    }

NAV_InitPipe_Exit_Tag: 
        return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void NAV::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, NAV_HK_TLM_MID, sizeof(HkTlm), TRUE);

    /* Init output messages */
    CFE_SB_InitMsg(&MissionResultMsg, PX4_MISSION_RESULT_MID,
                   sizeof(PX4_MissionResultMsg_t), TRUE);


    /* Init output messages */
    CFE_SB_InitMsg(&PositionSetpointTripletMsg,
                   PX4_POSITION_SETPOINT_TRIPLET_MID, 
                   sizeof(PX4_PositionSetpointTripletMsg_t),
                   TRUE);

    /* Initialize HK */
    HkTlm.NavState = PX4_NAVIGATION_STATE_MANUAL;
    HkTlm.RtlState = RTL_STATE_NONE;
    HkTlm.MissionItemReached = FALSE;
    HkTlm.WaypointPositionReached = FALSE;
    HkTlm.WaypointYawReached = FALSE;
    HkTlm.RtlForceDescentExecuting = FALSE;
    HkTlm.RtlForceDescentCompleted = FALSE;
    HkTlm.LandForceDescentExecuting = FALSE;
    HkTlm.LandForceDescentCompleted = FALSE;
    
    /* Initialize member attributes */
    RtlState = RTL_STATE_NONE;
    NewCommandArrived = false;
    CanLoiterAtSetpoint = false;
    LoiterPositionSet = false;
    WaypointPositionReached = false;
    WaypointYawReached = false;
    TimeFirstInsideOrbit = 0;
    ActionStart = 0;
    TimeWpReached = 0;
    MissionCruisingSpeed = -1.0f;
    MissionThrottle = -1.0f;
    VehicleStatusUpdateOnce = false;
    MissionResultUpdated = false;
    PositionSetpointTripletUpdated = false;
    ForceDescentTarget = 0;
    ForceDescentExecuting = false;
    ForceDescentCompleted = false;

    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* NAV initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::InitApp()
{
    int32 iStatus = CFE_SUCCESS;
    int8 hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("NAV - Failed to init events (0x%08lX)\n",
                                    iStatus);
        goto NAV_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto NAV_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto NAV_InitApp_Exit_Tag;
    }

NAV_InitApp_Exit_Tag: 
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(NAV_INIT_INF_EID, 
                                 CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 NAV_MAJOR_VERSION,
                                 NAV_MINOR_VERSION,
                                 NAV_REVISION,
                                 NAV_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("NAV - Application failed to initialize\n");
        }
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::RcvSchPipeMsg(int32 iBlocking)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = NULL;
    CFE_SB_MsgId_t MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(NAV_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(NAV_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case NAV_WAKEUP_MID:
                /* Check global position availability */
                if (!CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&CVT.VehicleGlobalPosition))
                {
                    Execute();
                }

                break;

            case NAV_SEND_HK_MID:
                ProcessCmdPipe();
                ReportHousekeeping();
                break;

            case PX4_HOME_POSITION_MID:
                CFE_PSP_MemCpy(&CVT.HomePositionMsg, MsgPtr, sizeof(CVT.HomePositionMsg));
                break;

            case PX4_MISSION_MID:
                CFE_PSP_MemCpy(&CVT.MissionMsg, MsgPtr, sizeof(CVT.MissionMsg));
                break;

            case PX4_VEHICLE_GLOBAL_POSITION_MID:
                CFE_PSP_MemCpy(&CVT.VehicleGlobalPosition, MsgPtr,
                        sizeof(CVT.VehicleGlobalPosition));
                break;

            case PX4_VEHICLE_STATUS_MID:
                CFE_PSP_MemCpy(&CVT.VehicleStatusMsg, MsgPtr, sizeof(CVT.VehicleStatusMsg));
                break;

            case PX4_VEHICLE_LAND_DETECTED_MID:
                CFE_PSP_MemCpy(&CVT.VehicleLandDetectedMsg, MsgPtr,
                        sizeof(CVT.VehicleLandDetectedMsg));
                break;

            case PX4_VEHICLE_LOCAL_POSITION_MID:
                CFE_PSP_MemCpy(&CVT.VehicleLocalPositionMsg, MsgPtr,
                        sizeof(CVT.VehicleLocalPositionMsg));
                break;

            case PX4_VEHICLE_COMMAND_MID:
                NewCommandArrived = TRUE;
                CFE_PSP_MemCpy(&CVT.VehicleCommandMsg, MsgPtr,
                        sizeof(CVT.VehicleCommandMsg));
                break;

            default:
                (void) CFE_EVS_SendEvent(NAV_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid SCH msgId (0x%04X)", MsgId);
                break;
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(NAV_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void NAV::ProcessCmdPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* CmdMsgPtr = NULL;
    CFE_SB_MsgId_t CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case NAV_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(NAV_MSGID_ERR_EID, CFE_EVS_ERROR,
                            "Recvd invalid CMD msgId (0x%04X)",
                            (unsigned short) CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(NAV_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process NAV Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void NAV::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32 uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case NAV_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(NAV_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                                         "Recvd NOOP. Version %d.%d.%d.%d",
                                         NAV_MAJOR_VERSION,
                                         NAV_MINOR_VERSION,
                                         NAV_REVISION,
                                         NAV_MISSION_REV);
                break;

            case NAV_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(NAV_CC_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid command code (%u)",
                        (unsigned int) uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send NAV Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void NAV::ReportHousekeeping()
{
    HkTlm.NavState = CVT.VehicleStatusMsg.NavState;
    HkTlm.RtlState = RtlState;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HkTlm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void NAV::SendMissionResultMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &MissionResultMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &MissionResultMsg);
}

void NAV::SendPositionSetpointTripletMsg()
{
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &PositionSetpointTripletMsg);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool NAV::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen)
{
    osalbool bResult = TRUE;
    uint16 usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(NAV_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                            "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                            "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                            usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return bResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* NAV Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void NAV_AppMain()
{
    oNAV.AppMain();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* NAV Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void NAV::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("NAV - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(NAV_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(NAV_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(NAV_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(NAV_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(NAV_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if (iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(NAV_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

int32 NAV::Execute()
{
	CFE_TIME_SysTime_t Now;
    
    /* Set vehicle arming state */
    if (!CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&CVT.VehicleStatusMsg) && !VehicleStatusUpdateOnce)
    {
        CVT.VehicleStatusMsg.ArmingState = PX4_ArmingState_t::PX4_ARMING_STATE_STANDBY;
        VehicleStatusUpdateOnce = TRUE;
    }

    /* Execute only on command event*/
    if (NewCommandArrived)
    {
        /* Reset new command flag*/
        NewCommandArrived = FALSE;
        /* Configure messages on command receipt */
        if (CVT.VehicleCommandMsg.Command
                == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_DO_REPOSITION)
        {
            /* EVENT: DO REPOSITION
             * store current position as previous and goal as next */
            RepositionTripletMsg.Previous.Yaw = CVT.VehicleGlobalPosition.Yaw;
            RepositionTripletMsg.Previous.Lat = CVT.VehicleGlobalPosition.Lat;
            RepositionTripletMsg.Previous.Lon = CVT.VehicleGlobalPosition.Lon;
            RepositionTripletMsg.Previous.Alt = CVT.VehicleGlobalPosition.Alt;

            /* Store new current position */
            RepositionTripletMsg.Current.LoiterRadius =
                    ConfigTblPtr->NAV_LOITER_RAD;
            /* Not clear as to what this 1 signifies, but PX4 still has it 
             * it there as of 1.9 */
            RepositionTripletMsg.Current.LoiterDirection = 1;
            RepositionTripletMsg.Current.Type =
                    PX4_SetpointType_t::PX4_SETPOINT_TYPE_LOITER;

            /* Assign yaw to current position set point */
            if (isfinite(CVT.VehicleCommandMsg.Param4))
            {
                RepositionTripletMsg.Current.Yaw =
                        CVT.VehicleCommandMsg.Param4;
            }
            else
            {
                RepositionTripletMsg.Current.Yaw = NAN;
            }

            /* Assign latitude and longitude to current set point */
            if (isfinite(CVT.VehicleCommandMsg.Param5) &&
                isfinite(CVT.VehicleCommandMsg.Param6))
            {
                RepositionTripletMsg.Current.Lat =
                        (CVT.VehicleCommandMsg.Param5 < NAV_LAT_SHORT_FORM) ?
                                CVT.VehicleCommandMsg.Param5 :
                                CVT.VehicleCommandMsg.Param5 / (double) CONVERT_DECIMAL_DEGREES;
                RepositionTripletMsg.Current.Lon =
                        (CVT.VehicleCommandMsg.Param6 < NAV_LON_SHORT_FORM) ?
                                CVT.VehicleCommandMsg.Param6 :
                                CVT.VehicleCommandMsg.Param6 / (double) CONVERT_DECIMAL_DEGREES;
            }
            else
            {
                RepositionTripletMsg.Current.Lat =
                        CVT.VehicleGlobalPosition.Lat;
                RepositionTripletMsg.Current.Lon =
                        CVT.VehicleGlobalPosition.Lon;
            }

            /* Assign altitude to current set point */
            if (isfinite(CVT.VehicleCommandMsg.Param7))
            {
                RepositionTripletMsg.Current.Alt =
                        CVT.VehicleCommandMsg.Param7;
            }
            else
            {
                RepositionTripletMsg.Current.Alt =
                        CVT.VehicleGlobalPosition.Alt;
            }
            /* Assign set point triplet validity */
            RepositionTripletMsg.Previous.Valid = TRUE;
            RepositionTripletMsg.Current.Valid = TRUE;
            RepositionTripletMsg.Next.Valid = FALSE;

        }
        else if (CVT.VehicleCommandMsg.Command
                == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF)
        {
            /* EVENT: TAKEOFF
             * store of command hist */
            CacheCommandEventHist();

            /* Store current position as previous and goal as next */
            TakeoffTripletMsg.Previous.Yaw =
                    CVT.VehicleGlobalPosition.Yaw;
            TakeoffTripletMsg.Previous.Lat =
                    CVT.VehicleGlobalPosition.Lat;
            TakeoffTripletMsg.Previous.Lon =
                    CVT.VehicleGlobalPosition.Lon;
            TakeoffTripletMsg.Previous.Alt =
                    CVT.VehicleGlobalPosition.Alt;

            /* Store new current position */
            TakeoffTripletMsg.Current.LoiterRadius =
                    ConfigTblPtr->NAV_LOITER_RAD;
            TakeoffTripletMsg.Current.LoiterDirection = 1;
            TakeoffTripletMsg.Current.Type =
                    PX4_SetpointType_t::PX4_SETPOINT_TYPE_TAKEOFF;

            /* Check if home position is valid, set current yaw and previous valid accordingly */
            if (!CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&CVT.HomePositionMsg))
            {
                TakeoffTripletMsg.Current.Yaw =
                        CVT.VehicleCommandMsg.Param4;
                TakeoffTripletMsg.Previous.Valid = TRUE;
            }
            else
            {
                TakeoffTripletMsg.Current.Yaw =
                        CVT.VehicleLocalPositionMsg.Yaw;
                TakeoffTripletMsg.Previous.Valid = FALSE;
            }

            /*  Check if param5 and param6 is finite, set Latitude, Longitude, Altitude and current and next position set point validity */
            if (isfinite(CVT.VehicleCommandMsg.Param5) &&
                isfinite(CVT.VehicleCommandMsg.Param6))
            {
                TakeoffTripletMsg.Current.Lat =
                        (CVT.VehicleCommandMsg.Param5 < 1000) ?
                                CVT.VehicleCommandMsg.Param5 :
                                CVT.VehicleCommandMsg.Param5 / (double) 1e7;
                TakeoffTripletMsg.Current.Lon =
                        (CVT.VehicleCommandMsg.Param6 < 1000) ?
                                CVT.VehicleCommandMsg.Param6 :
                                CVT.VehicleCommandMsg.Param6 / (double) 1e7;
            }
            else
            {
                TakeoffTripletMsg.Current.Lat = NAN;
                TakeoffTripletMsg.Current.Lon = NAN;
            }
            
            /* Assign set point triplet validity */
            TakeoffTripletMsg.Current.Alt = CVT.VehicleCommandMsg.Param7;
            TakeoffTripletMsg.Current.Valid = TRUE;
            TakeoffTripletMsg.Next.Valid = FALSE;
        }
    }

    /* Detect events for navigation actions. Find if a state is seen for first
     * time or has been active since a while */
    PX4_NavigationState_t CurrentState = CVT.VehicleStatusMsg.NavState;
    osalbool FirstRun = StateChangeDetect();
    osalbool Active = FALSE;
    if (!FirstRun)
    {
        Active = TRUE;
    }

    /* If a state is inactive */
    if (CurrentState != PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LOITER)
    {
        LoiterPositionSet = FALSE;
    }

    if (CurrentState != PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL)
    {
        RtlState = RTLState::RTL_STATE_NONE;
    }

    /* First run in a navigation mode */
    if (FirstRun)
    {
        if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF)
        {
            (void) CFE_EVS_SendEvent(NAV_ACTION_ST_EID, CFE_EVS_INFORMATION,
                    "Commencing %s", "Takeoff");
            Takeoff();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LOITER)
        {
            (void) CFE_EVS_SendEvent(NAV_ACTION_ST_EID, CFE_EVS_INFORMATION,
                    "Commencing %s", "Loiter");
            Loiter();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LAND)
        {
            (void) CFE_EVS_SendEvent(NAV_ACTION_ST_EID, CFE_EVS_INFORMATION,
                    "Commencing %s", "Land");
            Land();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL)
        {
            (void) CFE_EVS_SendEvent(NAV_ACTION_ST_EID, CFE_EVS_INFORMATION,
                    "Commencing %s", "Return to Launch");
            Rtl();

        }
        else
        {
            CanLoiterAtSetpoint = FALSE;

            /* Clear hk values revelant to mission if not in auto mode */
            HkTlm.MissionItemReached = FALSE;
            HkTlm.WaypointPositionReached = FALSE;
            HkTlm.WaypointYawReached = FALSE;
        }
    }

    /* If the mode is active */
    if (Active)
    {
        if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF)
        {
            TakeoffActive();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LOITER)
        {
            LoiterActive();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LAND)
        {
            LandActive();
        }
        else if (CurrentState
                == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL)
        {
            RtlActive();
        }
        else
        {
            CanLoiterAtSetpoint = FALSE;
        }
    }

    /* If we landed and have not received takeoff setpoint then stay in idle */
    if (CVT.VehicleLandDetectedMsg.Landed
            && !((CVT.VehicleStatusMsg.NavState
                    == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF)
                    || (CVT.VehicleStatusMsg.NavState
                            == PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_MISSION)))
    {
        PositionSetpointTripletMsg.Current.Type =
                PX4_SetpointType_t::PX4_SETPOINT_TYPE_IDLE;
        PositionSetpointTripletMsg.Current.Valid = TRUE;
        PositionSetpointTripletMsg.Previous.Valid = FALSE;
        PositionSetpointTripletMsg.Next.Valid = FALSE;
    }

    /* Time stamp out going messages */
    Now = CFE_TIME_GetTime();
    CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&MissionResultMsg, Now);

    if (PositionSetpointTripletUpdated)
    {
        CFE_SB_SetMsgTime((CFE_SB_MsgPtr_t)&PositionSetpointTripletMsg, Now);
        CFE_SB_SendMsg((CFE_SB_Msg_t*) &PositionSetpointTripletMsg);
        PositionSetpointTripletUpdated = FALSE;
    }

    if (MissionResultUpdated)
    {
        SendMissionResultMsg();
        MissionResultUpdated = FALSE;
    }

    return 0;
}

void NAV::Takeoff()
{
    float AbsoluteAltitude = 0.0f;
    float MinPitch = 0.0f;
    float MinAbsoluteAltitude = 0.0f;

    /* Use home position if valid */
    osalbool HomePosValidFlag = HomePositionValid();
    if (HomePosValidFlag)
    {
        MinAbsoluteAltitude = CVT.VehicleGlobalPosition.Alt
                + ConfigTblPtr->NAV_MIS_TAKEOFF_ALT;
    }
    else
    {
        MinAbsoluteAltitude = ConfigTblPtr->NAV_MIS_TAKEOFF_ALT;
    }

    /* Use altitude if it is already set. If home position is invalid use 
     * MinAbsoluteAltitude */
    if (TakeoffTripletMsg.Current.Valid &&
        isfinite(TakeoffTripletMsg.Current.Alt) && HomePosValidFlag)
    {
        AbsoluteAltitude = TakeoffTripletMsg.Current.Alt;

        /* If set altitude is less than minimum clearance raise it to that level and throw a warning */
        if (AbsoluteAltitude < MinAbsoluteAltitude)
        {
            AbsoluteAltitude = MinAbsoluteAltitude;
            
            if (AbsoluteAltitude < MinAbsoluteAltitude - 0.1f)
            {
                (void) CFE_EVS_SendEvent(NAV_LOW_CLEARANCE_ALT_EID,
                                         CFE_EVS_INFORMATION,
                                         "Using minimum takeoff altitude : %.2f meters",
                                         AbsoluteAltitude);
            }
        }
    }
    else
    {
        /* Raise to minimum clearance */
        AbsoluteAltitude = MinAbsoluteAltitude;
        (void) CFE_EVS_SendEvent(NAV_LOW_CLEARANCE_ALT_EID, CFE_EVS_INFORMATION,
                "Using minimum takeoff altitude: %.2f meters",
                AbsoluteAltitude);
    }

    /* If new altitude is lower than current altitude, don't go down */
    if (AbsoluteAltitude < CVT.VehicleGlobalPosition.Alt)
    {
        AbsoluteAltitude = CVT.VehicleGlobalPosition.Alt;
        (void) CFE_EVS_SendEvent(NAV_HIGH_ALT_EID, CFE_EVS_INFORMATION,
                "Set altitude already higher than take off altitude");
    }

    /* Set mission item to takeoff */
    MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF;

    /* Use current position */
    MissionItem.Lat = CVT.VehicleGlobalPosition.Lat;
    MissionItem.Lon = CVT.VehicleGlobalPosition.Lon;
    MissionItem.Yaw = CVT.VehicleGlobalPosition.Yaw;
    MissionItem.Altitude = AbsoluteAltitude;
    MissionItem.AltitudeIsRelative = FALSE;
    MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
    MissionItem.PitchMin = MinPitch;
    MissionItem.AutoContinue = FALSE;
    MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;

    /* Set mission result message */
    MissionResultMsg.Reached = FALSE;
    MissionResultMsg.Finished = FALSE;

    /* Record mission update event in osalbool */
    MissionResultUpdated = TRUE;

    /* Reset mission item reached */
    WaypointPositionReached = FALSE;
    WaypointYawReached = FALSE;
    TimeFirstInsideOrbit = 0;
    TimeWpReached = 0;

    /* Convert mission item to current set point */
    ConvertMissionItemToCurrentSetpoint(&PositionSetpointTripletMsg.Current,
            &MissionItem);
    PositionSetpointTripletMsg.Previous.Valid = FALSE;
    PositionSetpointTripletMsg.Next.Valid = FALSE;
    
    /* Set YawValid to true */
    PositionSetpointTripletMsg.Current.YawValid = TRUE;

    if (TakeoffTripletMsg.Current.Valid)
    {
        if (isfinite(TakeoffTripletMsg.Current.Yaw))
        {
            PositionSetpointTripletMsg.Current.Yaw =
                    TakeoffTripletMsg.Current.Yaw;
        }

        if (isfinite(TakeoffTripletMsg.Current.Lat) &&
            isfinite(TakeoffTripletMsg.Current.Lon))
        {
            PositionSetpointTripletMsg.Current.Lat =
                    TakeoffTripletMsg.Current.Lat;
            PositionSetpointTripletMsg.Current.Lon =
                    TakeoffTripletMsg.Current.Lon;
        }

        /* Mark this as done */
        CFE_PSP_MemSet(&TakeoffTripletMsg, 0, sizeof(TakeoffTripletMsg));
    }

    CanLoiterAtSetpoint = TRUE;
    PositionSetpointTripletUpdated = TRUE;
}

void NAV::TakeoffActive()
{
    osalbool MissionItemReachedFlag = IsMissionItemReached();

    if (TakeoffTripletMsg.Current.Valid)
    {
        Takeoff();
    }
    else if (MissionItemReachedFlag && !MissionResultMsg.Finished)
    {
        MissionResultMsg.Finished = TRUE;

        /* Set loiter item so position controllers stop doing takeoff logic */
        SetLoiterItem(&MissionItem);
        ConvertMissionItemToCurrentSetpoint(
                &PositionSetpointTripletMsg.Current, &MissionItem);
    }
}

void NAV::Land()
{
    /* Set the land item */
    MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND;

    /* Descend at current position */
    MissionItem.Lat = NAN;
    MissionItem.Lon = NAN;
    MissionItem.Yaw = CVT.VehicleGlobalPosition.Yaw;
    MissionItem.Altitude = 0; 
    MissionItem.AltitudeIsRelative = FALSE;
    MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
    MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
    MissionItem.TimeInside = 0.0f;
    MissionItem.AutoContinue = TRUE;
    MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;

    /* Set current mission item to Land */
    MissionResultMsg.Reached = FALSE;
    MissionResultMsg.Finished = FALSE;
    
    /* Update mission result and reset mission item reached */
    MissionResultUpdated = TRUE;
    WaypointPositionReached = FALSE;
    WaypointYawReached = FALSE;
    TimeFirstInsideOrbit = 0;
    TimeWpReached = 0;

    /* Convert mission item to current setpoint */
    PositionSetpointTripletMsg.Previous.Valid = FALSE;
    ConvertMissionItemToCurrentSetpoint(&PositionSetpointTripletMsg.Current,
            &MissionItem);
    PositionSetpointTripletMsg.Next.Valid = FALSE;

    CanLoiterAtSetpoint = FALSE;
    PositionSetpointTripletUpdated = TRUE;

}

void NAV::LandActive()
{
    osalbool MissionItemReachedFlag = IsMissionItemReached();

    /* Bogus Land */
    if (!CVT.VehicleLandDetectedMsg.Landed
            && !CVT.VehicleLandDetectedMsg.GroundContact
            && CVT.VehicleLocalPositionMsg.DistBottom < 0.1)
    {
        if (!ForceDescentCompleted && !ForceDescentExecuting)
        {
            ForceDescentExecuting = TRUE;

            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND;
            MissionItem.Lat = NAN;
            MissionItem.Lon = NAN;
            MissionItem.AltitudeIsRelative = FALSE;
            /* move down 1 m */
            ForceDescentTarget = CVT.VehicleGlobalPosition.Alt - 1;
            MissionItem.Altitude = ForceDescentTarget;
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            ConvertMissionItemToCurrentSetpoint(
                    &PositionSetpointTripletMsg.Current, &MissionItem);
            PositionSetpointTripletUpdated = TRUE;
        }
        else if (!ForceDescentCompleted && ForceDescentExecuting)
        {
            if (fabsf(CVT.VehicleGlobalPosition.Alt - ForceDescentTarget)
                    < 0.1)
            {
                ForceDescentExecuting = FALSE;
                ForceDescentCompleted = FALSE;
                ForceDescentTarget = 0;
            }
        }

    }
    else if (CVT.VehicleLandDetectedMsg.Landed
            || CVT.VehicleLandDetectedMsg.GroundContact)
    {
        ForceDescentExecuting = FALSE;
        ForceDescentCompleted = FALSE;
        ForceDescentTarget = 0;
    }
    else
    {
        if (MissionItemReachedFlag && !MissionResultMsg.Finished)
        {
            MissionResultMsg.Reached = TRUE;
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_CUSTOM_0;
            MissionItem.Lat = NAN;
            MissionItem.Lon = NAN;
            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.Altitude = CVT.VehicleGlobalPosition.Alt
                    - CVT.VehicleLocalPositionMsg.DistBottom - 0.5f; // TODO: Make this a parameter
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            ConvertMissionItemToCurrentSetpoint(
                    &PositionSetpointTripletMsg.Current, &MissionItem);
            PositionSetpointTripletUpdated = TRUE;
        }
    }

    HkTlm.LandForceDescentExecuting = ForceDescentExecuting;
    HkTlm.LandForceDescentCompleted = ForceDescentCompleted;

}

void NAV::Loiter()
{
    if (RepositionTripletMsg.Current.Valid)
    {
        LoiterReposition();
    }
    else
    {
        LoiterSetPosition();
    }
}

void NAV::LoiterActive()
{
    if (RepositionTripletMsg.Current.Valid)
    {
        LoiterReposition();
    }

    /* Reset loiter position if we get disarmed */
    if (CVT.VehicleStatusMsg.ArmingState
            != PX4_ArmingState_t::PX4_ARMING_STATE_ARMED)
    {
        LoiterPositionSet = FALSE;
    }
}

void NAV::LoiterReposition()
{
    float TravelDist = 0.0f;
    
    /* Cannot reposition if not armed */
    if (CVT.VehicleStatusMsg.ArmingState
            == PX4_ArmingState_t::PX4_ARMING_STATE_ARMED)
    {
        if (RepositionTripletMsg.Current.Valid)
        {
            PositionSetpointTripletMsg.Current.VelocityValid = FALSE;
            PositionSetpointTripletMsg.Previous.Yaw =
                    CVT.VehicleGlobalPosition.Yaw;
            PositionSetpointTripletMsg.Previous.Lat =
                    CVT.VehicleGlobalPosition.Lat;
            PositionSetpointTripletMsg.Previous.Lon =
                    CVT.VehicleGlobalPosition.Lon;
            PositionSetpointTripletMsg.Previous.Alt =
                    CVT.VehicleGlobalPosition.Alt;
            CFE_PSP_MemCpy(&PositionSetpointTripletMsg.Current,
                    &RepositionTripletMsg.Current,
                    sizeof(RepositionTripletMsg.Current));
            PositionSetpointTripletMsg.Next.Valid = FALSE;

            /* Set yaw */
            TravelDist = get_distance_to_next_waypoint(
                    CVT.VehicleGlobalPosition.Lat,
                    CVT.VehicleGlobalPosition.Lon,
                    PositionSetpointTripletMsg.Current.Lat,
                    PositionSetpointTripletMsg.Current.Lon);
            if (TravelDist > 1.0f)
            {
                /* Calculate direction the vehicle should point to */
                PositionSetpointTripletMsg.Current.Yaw =
                        get_bearing_to_next_waypoint(
                                CVT.VehicleGlobalPosition.Lat,
                                CVT.VehicleGlobalPosition.Lon,
                                PositionSetpointTripletMsg.Current.Lat,
                                PositionSetpointTripletMsg.Current.Lon);
            }

            if (PositionSetpointTripletMsg.Current.Type
                    == PX4_SetpointType_t::PX4_SETPOINT_TYPE_LOITER)
            {
                CanLoiterAtSetpoint = TRUE;
            }
            
            PositionSetpointTripletUpdated = TRUE;
            CFE_PSP_MemSet(&RepositionTripletMsg, 0, sizeof(RepositionTripletMsg));
        }
    }
}

void NAV::LoiterSetPosition()
{
    float MinClearance = ConfigTblPtr->NAV_MIS_LTRMIN_ALT;
    osalbool stopFuncExec = FALSE;

    if (CVT.VehicleStatusMsg.ArmingState
            != PX4_ArmingState_t::PX4_ARMING_STATE_ARMED
            && CVT.VehicleLandDetectedMsg.Landed)
    {
        /* Not setting loiter position if disarmed and landed, instead mark the current
         * setpoint as invalid and idle (both, just to be sure) */
        CanLoiterAtSetpoint = FALSE;
        PositionSetpointTripletMsg.Current.Type =
                PX4_SetpointType_t::PX4_SETPOINT_TYPE_IDLE;
        PositionSetpointTripletUpdated = TRUE;
        LoiterPositionSet = FALSE;
        stopFuncExec = TRUE;
    }
    else if (LoiterPositionSet)
    {
        /* Already set, nothing to do */
        stopFuncExec = TRUE;
    }

    if (!stopFuncExec)
    {
        LoiterPositionSet = TRUE;

        /* Set current mission item to loiter */
        if (CVT.VehicleLandDetectedMsg.Landed)
        {
            /* Landed, don't takeoff, but switch to IDLE mode */
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_CUSTOM_0;
        }
        else
        {
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM;

            if (CanLoiterAtSetpoint
                    && PositionSetpointTripletMsg.Current.Valid)
            {
                /* Use current position setpoint */
                MissionItem.Lat = PositionSetpointTripletMsg.Current.Lat;
                MissionItem.Lon = PositionSetpointTripletMsg.Current.Lon;
                MissionItem.Altitude = PositionSetpointTripletMsg.Current.Alt;

            }
            else
            {
                /* Use current position and use return altitude as clearance */
                MissionItem.Lat = CVT.VehicleGlobalPosition.Lat;
                MissionItem.Lon = CVT.VehicleGlobalPosition.Lon;
                MissionItem.Altitude = CVT.VehicleGlobalPosition.Alt;

                if (MinClearance > 0.0f
                        && MissionItem.Altitude
                                < CVT.HomePositionMsg.Alt + MinClearance)
                {
                    MissionItem.Altitude = CVT.HomePositionMsg.Alt + MinClearance;
                }
            }

            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = FALSE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
        }
        
        /* Convert mission item to current setpoint */
        PositionSetpointTripletMsg.Current.VelocityValid = FALSE;
        PositionSetpointTripletMsg.Previous.Valid = FALSE;
        ConvertMissionItemToCurrentSetpoint(
                &PositionSetpointTripletMsg.Current, &MissionItem);
        PositionSetpointTripletMsg.Next.Valid = FALSE;

        if (PositionSetpointTripletMsg.Current.Type
                == PX4_SetpointType_t::PX4_SETPOINT_TYPE_LOITER)
        {
            CanLoiterAtSetpoint = TRUE;
        }
        PositionSetpointTripletUpdated = TRUE;
    }
}

void NAV::Rtl()
{
    /* Sets current position item */
    MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_WAYPOINT;
    MissionItem.Lat = CVT.VehicleGlobalPosition.Lat;
    MissionItem.Lon = CVT.VehicleGlobalPosition.Lon;
    MissionItem.AltitudeIsRelative = FALSE;
    MissionItem.Altitude = CVT.VehicleGlobalPosition.Alt;
    MissionItem.Yaw = NAN;
    MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
    MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
    MissionItem.TimeInside = 0.0f;
    MissionItem.AutoContinue = TRUE;
    MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
    ConvertMissionItemToCurrentSetpoint(&PositionSetpointTripletMsg.Current,
            &MissionItem);
    PositionSetpointTripletMsg.Previous.Valid = FALSE;
    PositionSetpointTripletMsg.Next.Valid = FALSE;

    float rtl_altitude =
            (ConfigTblPtr->NAV_RTL_RETURN_ALT < CVT.VehicleLandDetectedMsg.AltMax) ?
                    ConfigTblPtr->NAV_RTL_RETURN_ALT :
                    CVT.VehicleLandDetectedMsg.AltMax;

    /* For safety reasons don't go into RTL if landed */
    if (CVT.VehicleLandDetectedMsg.Landed)
    {
        RtlState = RTLState::RTL_STATE_LANDED;
        (void) CFE_EVS_SendEvent(NAV_RTL_LND_SFGA_EID, CFE_EVS_ERROR,
                "Already landed, not executing RTL");

    }
    else if (CVT.VehicleGlobalPosition.Alt
            < (CVT.HomePositionMsg.Alt + rtl_altitude))
    {
        /* If lower than return altitude, climb up first */
        RtlState = RTLState::RTL_STATE_CLIMB;
    }
    else
    {
        RtlState = RTLState::RTL_STATE_RETURN;
        MissionItem.AltitudeIsRelative = FALSE;
        MissionItem.Altitude = CVT.VehicleGlobalPosition.Alt;
    }

    SetRtlItem();
}

void NAV::SetRtlItem()
{
    CanLoiterAtSetpoint = FALSE;

    switch (RtlState)
    {
        case RTLState::RTL_STATE_CLIMB:
        {
            /* Check if we are pretty close to home already */
            float home_distance = get_distance_to_next_waypoint(
                    CVT.HomePositionMsg.Lat, CVT.HomePositionMsg.Lon,
                    CVT.VehicleGlobalPosition.Lat, CVT.VehicleGlobalPosition.Lon);
            /* If we are close to home we do not climb as high, otherwise we climb to return alt */
            float rtl_altitude =
                    (ConfigTblPtr->NAV_RTL_RETURN_ALT < CVT.VehicleLandDetectedMsg.AltMax) ?
                            ConfigTblPtr->NAV_RTL_RETURN_ALT :
                            CVT.VehicleLandDetectedMsg.AltMax;
            float climb_alt = CVT.HomePositionMsg.Alt + rtl_altitude;
            /* We are close to home, limit climb to min */
            if (home_distance < ConfigTblPtr->NAV_RTL_MIN_DIST)
            {
                climb_alt = CVT.HomePositionMsg.Alt + ConfigTblPtr->NAV_RTL_DESCEND_ALT;
            }
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_WAYPOINT;
            MissionItem.Lat = CVT.VehicleGlobalPosition.Lat;
            MissionItem.Lon = CVT.VehicleGlobalPosition.Lon;
            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.Altitude = climb_alt;
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            (void) CFE_EVS_SendEvent(NAV_RTL_CLIMB_ST_EID, CFE_EVS_INFORMATION,
                    "RTL: Commencing climb to %d m (%d m above home)",
                    (int) climb_alt, (int) (climb_alt - CVT.HomePositionMsg.Alt));
            break;
        }
        case RTLState::RTL_STATE_RETURN:
        {
            MissionItem.Lat = CVT.HomePositionMsg.Lat;
            MissionItem.Lon = CVT.HomePositionMsg.Lon;
            /* Check if we are pretty close to home already */
            float home_distance = get_distance_to_next_waypoint(
                    CVT.HomePositionMsg.Lat, CVT.HomePositionMsg.Lon,
                    CVT.VehicleGlobalPosition.Lat, CVT.VehicleGlobalPosition.Lon);
            if (home_distance < ConfigTblPtr->NAV_RTL_MIN_DIST)
            {
                MissionItem.Yaw = CVT.HomePositionMsg.Yaw;
            }
            else
            {
                /* Use current heading to home */
                MissionItem.Yaw = get_bearing_to_next_waypoint(
                        CVT.VehicleGlobalPosition.Lat,
                        CVT.VehicleGlobalPosition.Lon, CVT.HomePositionMsg.Lat,
                        CVT.HomePositionMsg.Lon);
            }
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_WAYPOINT;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            (void) CFE_EVS_SendEvent(NAV_RTL_RETURN_ST_EID, CFE_EVS_INFORMATION,
                    "RTL: Commencing return at %d m (%d m above home)",
                    (int) MissionItem.Altitude,
                    (int) (MissionItem.Altitude - CVT.HomePositionMsg.Alt));
            break;
        }
        case RTLState::RTL_STATE_DESCEND:
        {
            MissionItem.Lat = CVT.HomePositionMsg.Lat;
            MissionItem.Lon = CVT.HomePositionMsg.Lon;
            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.Altitude = CVT.HomePositionMsg.Alt + ConfigTblPtr->NAV_RTL_DESCEND_ALT;
            /* Check if we are already lower - then we will just stay there */
            if (MissionItem.Altitude > CVT.VehicleGlobalPosition.Alt)
            {
                MissionItem.Altitude = CVT.VehicleGlobalPosition.Alt;
            }
            MissionItem.Yaw = CVT.HomePositionMsg.Yaw;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_WAYPOINT;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;

            /* Disable previous setpoint to prevent drift */
            PositionSetpointTripletMsg.Previous.Valid = FALSE;
            (void) CFE_EVS_SendEvent(NAV_RTL_RETURN_ST_EID, CFE_EVS_INFORMATION,
                    "RTL: Commencing descent to %d m (%d m above home)",
                    (int) MissionItem.Altitude,
                    (int) (MissionItem.Altitude - CVT.HomePositionMsg.Alt));
            break;
        }
        case RTLState::RTL_STATE_LOITER:
        {
            bool autoland = ConfigTblPtr->NAV_RTL_LAND_DELAY > -DELAY_SIGMA;

            MissionItem.Lat = CVT.HomePositionMsg.Lat;
            MissionItem.Lon = CVT.HomePositionMsg.Lon;
            /* Don't change altitude */
            MissionItem.Yaw = CVT.HomePositionMsg.Yaw;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.NavCmd =
                    autoland ?
                            PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TIME :
                            PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside =
                    ConfigTblPtr->NAV_RTL_LAND_DELAY < 0.0f ?
                            0.0f : ConfigTblPtr->NAV_RTL_LAND_DELAY;
            MissionItem.AutoContinue = autoland;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            CanLoiterAtSetpoint = TRUE;

            float TimeInside = GetTimeInside(&MissionItem);
            if (autoland && (TimeInside > FLT_EPSILON))
            {
                (void) CFE_EVS_SendEvent(NAV_RTL_LOITER_ST_EID,
                CFE_EVS_INFORMATION, "RTL: Commencing loiter for %.1fs",
                        TimeInside);
            }
            else
            {
                (void) CFE_EVS_SendEvent(NAV_RTL_RETURN_ST_EID,
                CFE_EVS_INFORMATION, "RTL: Loiter completed");
            }
            break;
        }
        case RTLState::RTL_STATE_LAND:
        {
            /* Set the land item */
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND;
            /* Use current position */
            MissionItem.Lat = CVT.HomePositionMsg.Lat;
            MissionItem.Lon = CVT.HomePositionMsg.Lon;
            MissionItem.Yaw = CVT.HomePositionMsg.Yaw;
            MissionItem.Altitude = 0;
            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            (void) CFE_EVS_SendEvent(NAV_RTL_LAND_ST_EID, CFE_EVS_INFORMATION,
                    "RTL: Commencing land at home");
            break;
        }
        case RTLState::RTL_STATE_LANDED:
        {
            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_CUSTOM_0;
            MissionItem.Lat = CVT.HomePositionMsg.Lat;
            MissionItem.Lon = CVT.HomePositionMsg.Lon;
            MissionItem.AltitudeIsRelative = FALSE;
            MissionItem.Altitude = CVT.HomePositionMsg.Alt;
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            (void) CFE_EVS_SendEvent(NAV_RTL_LAND_EN_EID, CFE_EVS_INFORMATION,
                    "RTL: Land completed");
            break;
        }
        default:
            break;
    }

    WaypointPositionReached = FALSE;
    WaypointYawReached = FALSE;
    TimeFirstInsideOrbit = 0;
    TimeWpReached = 0;

    ConvertMissionItemToCurrentSetpoint(&PositionSetpointTripletMsg.Current,
            &MissionItem);
    PositionSetpointTripletMsg.Next.Valid = FALSE;
    PositionSetpointTripletUpdated = TRUE;
}

void NAV::AdvanceRtl()
{
    switch (RtlState)
    {
        case RTLState::RTL_STATE_CLIMB:
            RtlState = RTLState::RTL_STATE_RETURN;
            break;
        case RTLState::RTL_STATE_RETURN:
            RtlState = RTLState::RTL_STATE_DESCEND;
            break;
        case RTLState::RTL_STATE_TRANSITION_TO_MC:
            RtlState = RTLState::RTL_STATE_RETURN;
            break;
        case RTLState::RTL_STATE_DESCEND:
            /* Only go to land if autoland is enabled */
            if (ConfigTblPtr->NAV_RTL_LAND_DELAY < -DELAY_SIGMA
                    || ConfigTblPtr->NAV_RTL_LAND_DELAY > DELAY_SIGMA)
            {
                RtlState = RTLState::RTL_STATE_LOITER;
            }
            else
            {
                RtlState = RTLState::RTL_STATE_LAND;
            }
            break;
        case RTLState::RTL_STATE_LOITER:
            RtlState = RTLState::RTL_STATE_LAND;
            break;
        case RTLState::RTL_STATE_LAND:
            RtlState = RTLState::RTL_STATE_LANDED;
            break;
        default:
            break;
    }
}

void NAV::RtlActive()
{
    /* Bogus Land */
    if (!CVT.VehicleLandDetectedMsg.Landed
            && !CVT.VehicleLandDetectedMsg.GroundContact
            && CVT.VehicleLocalPositionMsg.DistBottom < 0.1
            && RtlState == RTLState::RTL_STATE_LANDED)
    {
        if (!ForceDescentCompleted && !ForceDescentExecuting)
        {
            ForceDescentExecuting = TRUE;

            MissionItem.NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND;
            MissionItem.Lat = NAN;
            MissionItem.Lon = NAN;
            MissionItem.AltitudeIsRelative = FALSE;
            /* move down 1 m */
            ForceDescentTarget = CVT.VehicleGlobalPosition.Alt - 1;
            MissionItem.Altitude = ForceDescentTarget;
            MissionItem.Yaw = NAN;
            MissionItem.LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
            MissionItem.AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            MissionItem.TimeInside = 0.0f;
            MissionItem.AutoContinue = TRUE;
            MissionItem.Origin = NAV_Origin_t::ORIGIN_ONBOARD;
            ConvertMissionItemToCurrentSetpoint(
                    &PositionSetpointTripletMsg.Current, &MissionItem);
            PositionSetpointTripletUpdated = TRUE;
        }
        else if (!ForceDescentCompleted && ForceDescentExecuting)
        {
            if (fabsf(CVT.VehicleGlobalPosition.Alt - ForceDescentTarget)
                    < 0.1)
            {
                ForceDescentExecuting = FALSE;
                ForceDescentCompleted = FALSE;
                ForceDescentTarget = 0;
            }
        }

    }
    else if (CVT.VehicleLandDetectedMsg.Landed
            || CVT.VehicleLandDetectedMsg.GroundContact)
    {
        ForceDescentExecuting = FALSE;
        ForceDescentCompleted = FALSE;
        ForceDescentTarget = 0;
    }
    else
    {
        osalbool MissionItemReachedFlag = IsMissionItemReached();
        if (MissionItemReachedFlag && RtlState != RTLState::RTL_STATE_LANDED)
        {
            AdvanceRtl();
            SetRtlItem();
        }

    }

    HkTlm.RtlForceDescentExecuting = ForceDescentExecuting;
    HkTlm.RtlForceDescentCompleted = ForceDescentCompleted;
}

osalbool NAV::StateChangeDetect()
{
    /* When there is a change in state reset fail safe flag */
    PX4_VehicleStatusMsg_t CurrentState = CVT.VehicleStatusMsg;
    osalbool StateChange = FALSE;
    
    if (&PreviousState == nullptr)
    {
        MissionResultMsg.StayInFailsafe = FALSE;
        PreviousState = CurrentState;
        StateChange = TRUE;
    }
    else if (PreviousState.NavState != CurrentState.NavState)
    {
        MissionResultMsg.StayInFailsafe = FALSE;
        PreviousState = CurrentState;
        StateChange = TRUE;
    }

    return StateChange;
}

void NAV::CacheCommandEventHist()
{
    /* Holds command history one level deep */
    PX4_VehicleCommandMsg_t CurrentCommand = CVT.VehicleCommandMsg;
    PreviousCommand = CurrentCommand;
}

osalbool NAV::IsMissionItemReached()
{
    osalbool isMissionItemReached = FALSE;
    uint64 Now = 0;
    float Dist= 0.0f;
    float DistXy = 0.0f;
    float DistZ = 0.0f;
    float AltAsml = 0.0f;
    float TakeoffAlt = 0.0f;
    float AltitudeAcceptanceRadius = 0.0f;
    float MissionAcceptanceRadius = 0.0f;
    float Cog = 0.0f;
    float YawErr = 0.0f;
    float TimeInside = 0.0f;
    PX4_PositionSetpoint_t CurrentSetpoint = {0};
    PX4_PositionSetpoint_t NextSetpoint = {0};
    float Range = 0.0f;
    float Bearing = 0.0f;
    float InnerAngle = 0.0f;
    
    switch (MissionItem.NavCmd)
    {
        case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_DO_SET_SERVO:
        {
            isMissionItemReached = TRUE;
            goto MissionItemReached_Exit_Tag;
        }
        case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND:
        {
            isMissionItemReached = CVT.VehicleLandDetectedMsg.Landed;
            goto MissionItemReached_Exit_Tag;
        }
        case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM:
        {
            isMissionItemReached = FALSE;
            goto MissionItemReached_Exit_Tag;
        }
        default:
        {
		    /* do nothing, this is a 3D waypoint */
            break;
        }
    }

    Now = CFE_TIME_GetTimeInMicros();
    if (!CVT.VehicleLandDetectedMsg.Landed && !WaypointPositionReached)
    {
        Dist= -1.0f;
        DistXy = -1.0f;
        DistZ = -1.0f;

        AltAsml = MissionItem.AltitudeIsRelative ?
                            MissionItem.Altitude + CVT.HomePositionMsg.Alt : MissionItem.Altitude;
        Dist= get_distance_to_point_global_wgs84(MissionItem.Lat,
                MissionItem.Lon, AltAsml, CVT.VehicleGlobalPosition.Lat, CVT.VehicleGlobalPosition.Lon, CVT.VehicleGlobalPosition.Alt,
                &DistXy, &DistZ);
        if (MissionItem.NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF
                && CVT.VehicleStatusMsg.IsRotaryWing)
        {
            /* We want to avoid the edge case where the acceptance radius is bigger or equal than
             * the altitude of the takeoff waypoint above home. Otherwise, we do not really follow
             * take-off procedures like leaving the landing gear down. */
            TakeoffAlt = MissionItem.AltitudeIsRelative ?
                                    MissionItem.Altitude :
                                    (MissionItem.Altitude - CVT.HomePositionMsg.Alt);
            AltitudeAcceptanceRadius = GetAltitudeAcceptedRadius();

            /* It should be safe to takeoff using half of the TakeoffAlt as accepted radius */
            if (TakeoffAlt > 0 && TakeoffAlt < AltitudeAcceptanceRadius)
            {
                AltitudeAcceptanceRadius = TakeoffAlt / 2.0f;
            }

            /* Require only altitude for takeoff for mc */
            if (CVT.VehicleGlobalPosition.Alt > AltAsml - AltitudeAcceptanceRadius)
            {
                WaypointPositionReached = TRUE;
            }
        }
        else if (MissionItem.NavCmd
                == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF)
        {
            /* For takeoff mission items use the parameter for the takeoff acceptance radius */
            if (Dist>= 0.0f && Dist<= ConfigTblPtr->NAV_ACC_RAD
                    && DistZ <= ConfigTblPtr->NAV_ACC_RAD)
            {
                WaypointPositionReached = TRUE;
            }
        }
        else
        {
            /* For normal mission items used their acceptance radius */
            MissionAcceptanceRadius = MissionItem.AcceptanceRadius;
            
            /* If set to zero use the default instead */
            if (MissionAcceptanceRadius < NAV_EPSILON_POSITION)
            {
                MissionAcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
            }
            if (Dist>= 0.0f && Dist<= MissionAcceptanceRadius
                    && DistZ <= ConfigTblPtr->NAV_ALT_RAD)
            {
                WaypointPositionReached = TRUE;
            }
        }
        
        if (WaypointPositionReached)
        {
            TimeWpReached = Now;
        }
    }

    if (WaypointPositionReached && !WaypointYawReached)
    {
        /* Added PX4_VEHICLE_CMD_NAV_LOITER_TO_ALT to PX4_VehicleCmd_t */
        if ((CVT.VehicleStatusMsg.IsRotaryWing
                || (MissionItem.NavCmd
                        == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TO_ALT
                        && MissionItem.ForceHeading))
                && isfinite(MissionItem.Yaw))
        {
            /* Check course if defined only for rotary wing except takeoff */
            Cog = CVT.VehicleStatusMsg.IsRotaryWing ? 
                            CVT.VehicleGlobalPosition.Yaw : 
                            atan2f(CVT.VehicleGlobalPosition.VelE, 
                                   CVT.VehicleGlobalPosition.VelN);
            YawErr = _wrap_pi(MissionItem.Yaw - Cog);

            /* Accept yaw if reached or if timeout is set then we dont ignore force headings */
            if (fabsf(YawErr)
                    < math::radians((float) ConfigTblPtr->NAV_MIS_YAW_ERR)
                    || (ConfigTblPtr->NAV_MIS_YAW_TMT >= FLT_EPSILON
                            && !MissionItem.ForceHeading))
            {
                WaypointYawReached = TRUE;
            }

            /* If heading needs to be reached, the timeout is enabled and we don't make it we abort */
            if (!WaypointYawReached && MissionItem.ForceHeading
                    && (ConfigTblPtr->NAV_MIS_YAW_TMT >= FLT_EPSILON)
                    && (Now - TimeWpReached
                            >= (uint64) ConfigTblPtr->NAV_MIS_YAW_TMT * 1e6f))
            {
                SetMissionFaliure("did not reach waypoint before timeout");
            }
        }
        else
        {
            WaypointYawReached = TRUE;
        }
    }

    /* Once the position and yaw waypoint have been set we can start the loiter time countdown */
    if (WaypointPositionReached && WaypointYawReached)
    {
        if (TimeFirstInsideOrbit == 0)
        {
            TimeFirstInsideOrbit = Now;
        }

        /* Check if the MAV was long enough inside the waypoint orbit */
        TimeInside = GetTimeInside(&MissionItem);
        if ((TimeInside < FLT_EPSILON)
                || Now - TimeFirstInsideOrbit >= (uint64) TimeInside * 1e6f)
        {
            /* Exit xtrack location */
            if (MissionItem.LoiterExitXTrack
                    && (MissionItem.NavCmd
                            == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TO_ALT
                            || MissionItem.NavCmd
                                    == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TIME))
            {
                /* Reset lat/lon of loiter waypoint so wehicle follows tangent */
                CurrentSetpoint = PositionSetpointTripletMsg.Current;
                NextSetpoint = PositionSetpointTripletMsg.Next;

                Range = get_distance_to_next_waypoint(CurrentSetpoint.Lat,
                        CurrentSetpoint.Lon, NextSetpoint.Lat, NextSetpoint.Lon);
                Bearing = get_bearing_to_next_waypoint(CurrentSetpoint.Lat,
                        CurrentSetpoint.Lon, NextSetpoint.Lat, NextSetpoint.Lon);
                InnerAngle = M_PI_2_F
                        - asinf(MissionItem.LoiterRadius / Range);

                /* Compute ideal tangent origin */
                if (CurrentSetpoint.LoiterDirection > 0)
                {
                    Bearing -= InnerAngle;

                }
                else
                {
                    Bearing += InnerAngle;
                }

                /* Replace current setpoint Lat/Lon with tangent coordinate */
                waypoint_from_heading_and_distance(CurrentSetpoint.Lat, CurrentSetpoint.Lon,
                        Bearing, CurrentSetpoint.LoiterRadius, &CurrentSetpoint.Lat,
                        &CurrentSetpoint.Lon);
            }
            isMissionItemReached = TRUE;
        }
    }

    /* Copy values to HK */
    HkTlm.MissionItemReached = isMissionItemReached;
    HkTlm.WaypointPositionReached = WaypointPositionReached;
    HkTlm.WaypointYawReached = WaypointYawReached;

    /* All acceptance criteria must be met in the same iteration */
    WaypointPositionReached = FALSE;
    WaypointYawReached = FALSE;

MissionItemReached_Exit_Tag:
    return isMissionItemReached;
}

void NAV::SetMissionFaliure(const char* reason)
{
    if (!MissionResultMsg.Failure)
    {
        MissionResultMsg.Failure = TRUE;
        (void) CFE_EVS_SendEvent(NAV_MSN_FAILED_ERR_EID, CFE_EVS_ERROR,
                "Mission failed (%s)", reason);
    }
}

void NAV::ConvertMissionItemToCurrentSetpoint(PX4_PositionSetpoint_t *PosSetpoint,
        NAV_MissionItem_t *Item)
{
    if (!(!Item->NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_WAYPOINT
            || !Item->NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM
            || !Item->NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TIME
            || !Item->NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND
            || !Item->NavCmd == PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF))
    {
        PosSetpoint->Lat = Item->Lat;
        PosSetpoint->Lon = Item->Lon;
        PosSetpoint->Alt =
                Item->AltitudeIsRelative ?
                        Item->Altitude + CVT.HomePositionMsg.Alt : Item->Altitude;
        PosSetpoint->Yaw = Item->Yaw;
        PosSetpoint->LoiterRadius =
                (fabsf(Item->LoiterRadius) > NAV_EPSILON_POSITION) ?
                        fabsf(Item->LoiterRadius) : ConfigTblPtr->NAV_LOITER_RAD;
        PosSetpoint->LoiterDirection = (Item->LoiterRadius > 0) ? 1 : -1;
        PosSetpoint->AcceptanceRadius = Item->AcceptanceRadius;
        PosSetpoint->DisableMcYawControl = Item->DisableMcYaw;

        PosSetpoint->CruisingSpeed = GetCruisingSpeed();
        PosSetpoint->CruisingThrottle = GetCruisingThrottle();

        switch (Item->NavCmd)
        {
            case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF:
            {
                if (CVT.VehicleStatusMsg.ArmingState == PX4_ArmingState_t::PX4_ARMING_STATE_ARMED
                        && !CVT.VehicleLandDetectedMsg.Landed)
                {
                    PosSetpoint->Type = PX4_SetpointType_t::PX4_SETPOINT_TYPE_POSITION;
                }
                else
                {
                    PosSetpoint->Type = PX4_SetpointType_t::PX4_SETPOINT_TYPE_TAKEOFF;
                    /* Set pitch and ensure that the hold time is zero */
                    PosSetpoint->PitchMin = Item->PitchMin;
                }

                break;
            }
            case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LAND:
            {
                PosSetpoint->Type = PX4_SetpointType_t::PX4_SETPOINT_TYPE_LAND;
                break;
            }
            case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_TIME:
            {
                // Fall through
            }
            case PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM:
            {
                PosSetpoint->Type = PX4_SetpointType_t::PX4_SETPOINT_TYPE_LOITER;
                break;
            }
            default:
            {
                PosSetpoint->Type = PX4_SetpointType_t::PX4_SETPOINT_TYPE_POSITION;
                break;
            }
            
            PosSetpoint->Valid = TRUE;
        }
    }
}

void NAV::SetLoiterItem(NAV_MissionItem_t * Item)
{
    float MinClearance = -1.0f;
    
    if (CVT.VehicleLandDetectedMsg.Landed)
    {
        Item->NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_CUSTOM_0;
    }
    else
    {
        Item->NavCmd = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_LOITER_UNLIM;
        if (CanLoiterAtSetpoint && PositionSetpointTripletMsg.Current.Valid)
        {
            /* Use current position setpoint */
            Item->Lat = PositionSetpointTripletMsg.Current.Lat;
            Item->Lon = PositionSetpointTripletMsg.Current.Lon;
            Item->Altitude = PositionSetpointTripletMsg.Current.Alt;
        }
        else
        {
            /* Use current position and use return altitude as clearance */
            Item->Lat = CVT.VehicleGlobalPosition.Lat;
            Item->Lon = CVT.VehicleGlobalPosition.Lon;
            Item->Altitude = CVT.VehicleGlobalPosition.Alt;
            if (MinClearance > 0.0f
                    && Item->Altitude < CVT.HomePositionMsg.Alt + MinClearance)
            {
                Item->Altitude = CVT.HomePositionMsg.Alt = MinClearance;
            }

        }
        Item->AltitudeIsRelative = FALSE;
        Item->Yaw = NAN;
        Item->LoiterRadius = ConfigTblPtr->NAV_LOITER_RAD;
        Item->AcceptanceRadius = ConfigTblPtr->NAV_ACC_RAD;
        Item->TimeInside = 0.0f;
        Item->AutoContinue = FALSE;
        Item->Origin = NAV_Origin_t::ORIGIN_ONBOARD;
    }
}

float NAV::GetTimeInside(NAV_MissionItem_t * Item)
{
    float time = 0.0f;
    
    if (Item->NavCmd != PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF)
    {
        time = Item->TimeInside;
    }
    
    return time;
}

osalbool NAV::HomePositionValid()
{
    osalbool HomePosValidFlag = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&CVT.HomePositionMsg);
    
    return HomePosValidFlag;
}

float NAV::GetDefaultAcceptedRadius()
{
    return ConfigTblPtr->NAV_ACC_RAD;
}

void NAV::SetAcceptedRadius(float mission_radius)
{
    ConfigTblPtr->NAV_ACC_RAD = mission_radius;
}

float NAV::GetAltitudeAcceptedRadius()
{
    return ConfigTblPtr->NAV_ALT_RAD;
}

float NAV::GetCruisingThrottle()
{
    float msnThrottle = -1.0f;
    
    if (MissionThrottle > FLT_EPSILON)
    {
        msnThrottle = MissionThrottle;
    }

    return msnThrottle;
}

float NAV::GetCruisingSpeed()
{
    osalbool is_planned = IsPlannedMission();
    float msnCruisingSpeed = -1.0f;
    
    if (is_planned && MissionCruisingSpeed > 0.0f)
    {
        msnCruisingSpeed = MissionCruisingSpeed;
    }

    return msnCruisingSpeed;
}

/************************/
/*  End of File Comment */
/************************/
