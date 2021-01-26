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
#include "rcin_custom.h"
#include "rcin_app.h"
#include "rcin_msg.h"
#include "rcin_version.h"
#include <unistd.h>
#include <limits.h>
#include "px4lib.h"
#include "px4lib_msgids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RCIN oRCIN;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RCIN::RCIN()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RCIN::~RCIN()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RCIN::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    int32 customEventCount = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[RCIN_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet(EventTbl, 0x00, sizeof(EventTbl));
    
    EventTbl[  ind].EventID = RCIN_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_INIT_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_CMD_NOOP_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_SUBSCRIBE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_PIPE_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_RCVMSG_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_MSGID_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_CC_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_MSGLEN_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_PUBLISHING_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = RCIN_NOT_PUBLISHING_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    /* Add custom events to the filter table */
    customEventCount = RCIN_Custom_Init_EventFilters(ind, EventTbl);
    
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
        (void) CFE_ES_WriteToSysLog("RCIN - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RCIN::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId, RCIN_SCH_PIPE_DEPTH,
            RCIN_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(RCIN_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, RCIN_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to RCIN_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto RCIN_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(RCIN_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, RCIN_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to RCIN_SEND_HK_MID. (0x%08X)",
                    (unsigned int)iStatus);
            goto RCIN_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create SCH pipe (0x%08lX)",
            iStatus);
        goto RCIN_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId, RCIN_CMD_PIPE_DEPTH,
            RCIN_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(RCIN_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                "CMD Pipe failed to subscribe to RCIN_CMD_MID. (0x%08lX)",
                iStatus);
            goto RCIN_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
            "Failed to create CMD pipe (0x%08lX)", iStatus);
        goto RCIN_InitPipe_Exit_Tag;
    }

RCIN_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, RCIN_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init output messages */
    CFE_SB_InitMsg(&InputRcMsg, PX4_INPUT_RC_MID, 
        sizeof(PX4_InputRcMsg_t), TRUE);
        
    /* Init custom data */
    RCIN_Custom_InitData();
    
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RCIN initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RCIN::InitApp()
{
    int32  iStatus     = CFE_SUCCESS;
    int8   hasEvents   = 0;
    boolean returnBool = TRUE;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RCIN - Failed to init events (0x%08lX)\n", iStatus);
        goto RCIN_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto RCIN_InitApp_Exit_Tag;
    }

    InitData();
    
    returnBool = RCIN_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = RCIN_ERROR;
        goto RCIN_InitApp_Exit_Tag;
    }

    HkTlm.State = RCIN_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&RCIN_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(RCIN_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto RCIN_InitApp_Exit_Tag;
    }

RCIN_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RCIN_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 RCIN_MAJOR_VERSION,
                                 RCIN_MINOR_VERSION,
                                 RCIN_REVISION,
                                 RCIN_MISSION_REV);
    }
    else
    {
        if (hasEvents != 1)
        {
            (void) CFE_ES_WriteToSysLog("RCIN - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RCIN::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RCIN_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RCIN_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case RCIN_WAKEUP_MID:
            {
                ReadDevice();
                SendInputRcMsg();
                break;
            }
            case RCIN_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(RCIN_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
                break;
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
        /* If there's no incoming message within a specified time (via the
         * iBlocking arg, do nothing here.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(RCIN_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::ProcessCmdPipe()
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
                case RCIN_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(RCIN_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(RCIN_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process RCIN Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case RCIN_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(RCIN_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    RCIN_MAJOR_VERSION,
                    RCIN_MINOR_VERSION,
                    RCIN_REVISION,
                    RCIN_MISSION_REV);
                break;
            }
            case RCIN_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(RCIN_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send RCIN Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::ReportHousekeeping()
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
void RCIN::SendInputRcMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&InputRcMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&InputRcMsg);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean RCIN::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(RCIN_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* Read from the Radio Control Receiver                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::ReadDevice(void)
{
    boolean returnBool = FALSE;

    returnBool = RCIN_Custom_Measure(&InputRcMsg);
    if(FALSE == returnBool)
    {
        /* Rollover */
        if(StrikeCount == UCHAR_MAX)
        {
            StrikeCount = 0;
        }
        /* Increment strike count */
        StrikeCount++;
        /* Measure is returning FALSE set state to not publishing */
        if(StrikeCount == RCIN_STRIKE_COUNT_THRES)
        {
            if (HkTlm.State != RCIN_NOTPUBLISHING)
            {
                HkTlm.State = RCIN_NOTPUBLISHING;
                (void) CFE_EVS_SendEvent(RCIN_NOT_PUBLISHING_ERR_EID, CFE_EVS_ERROR,
                    "RCIN is NOT publishing fresh data");
            }
        }
    }
    else
    {
        StrikeCount = 0;
        if (HkTlm.State != RCIN_PUBLISHING)
        {
            /* Measure is returning TRUE set state to publishing */
            HkTlm.State = RCIN_PUBLISHING;
            (void) CFE_EVS_SendEvent(RCIN_PUBLISHING_INF_EID, CFE_EVS_INFORMATION,
                    "RCIN is publishing fresh data");
        }
    }

    InputRcMsg.LastSignal = InputRcMsg.Timestamp;
    InputRcMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RCIN Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void RCIN_AppMain()
{
    oRCIN.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RCIN Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RCIN - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RCIN_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(RCIN_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(RCIN_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(RCIN_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(RCIN_SCH_PIPE_PEND_TIME);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RCIN_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RCIN_CleanupCallback(void)
{
    RCIN_Custom_Uninit();
}

/************************/
/*  End of File Comment */
/************************/
