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
#include "rgbled_app.h"
#include "rgbled_msg.h"
#include "rgbled_version.h"
#include "rgbled_custom.h"
#include "px4lib_msgids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RGBLED oRGBLED;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RGBLED::RGBLED()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RGBLED::~RGBLED()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RGBLED::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet(EventTbl, 0x00, sizeof(EventTbl));

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_RESERVED_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_INIT_INF_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_CMD_NOOP_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_SUBSCRIBE_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_PIPE_INIT_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_RCVMSG_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_MSGID_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_CC_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_MSGLEN_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_CMD_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }

    if(ind < CFE_EVS_MAX_EVENT_FILTERS)
    {
        EventTbl[  ind].EventID = RGBLED_INIT_ERR_EID;
        EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    }
    
    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, ind, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RGBLED - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RGBLED::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
                                RGBLED_SCH_PIPE_DEPTH,
                                RGBLED_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(RGBLED_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, RGBLED_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RGBLED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to RGBLED_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto RGBLED_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(RGBLED_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, RGBLED_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RGBLED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to RGBLED_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto RGBLED_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_LED_CONTROL_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RGBLED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to PX4_LED_CONTROL_MID. (0x%08lX)",
                     iStatus);
            goto RGBLED_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(RGBLED_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create SCH pipe (0x%08lX)",
             iStatus);
        goto RGBLED_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
                                RGBLED_CMD_PIPE_DEPTH,
                                RGBLED_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(RGBLED_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RGBLED_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                 "CMD Pipe failed to subscribe to RGBLED_CMD_MID. (0x%08lX)",
                 iStatus);
            goto RGBLED_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(RGBLED_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create CMD pipe (0x%08lX)",
             iStatus);
        goto RGBLED_InitPipe_Exit_Tag;
    }

RGBLED_InitPipe_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, RGBLED_HK_TLM_MID, sizeof(HkTlm), TRUE);
    
    /* Init custom data */
    RGBLED_Custom_InitData();
    
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RGBLED initialization                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RGBLED::InitApp()
{
    int32  iStatus     = CFE_SUCCESS;
    boolean returnBool = FALSE;
    int8   hasEvents   = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RGBLED - Failed to init events (0x%08lX)\n", iStatus);
        goto RGBLED_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto RGBLED_InitApp_Exit_Tag;
    }

    InitData();
    
    returnBool = RGBLED_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        goto RGBLED_InitApp_Exit_Tag;
    }
    HkTlm.State = RGBLED_INITIALIZED;
    
    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&RGBLED_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RGBLED_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto RGBLED_InitApp_Exit_Tag;
    }

RGBLED_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RGBLED_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 RGBLED_MAJOR_VERSION,
                                 RGBLED_MINOR_VERSION,
                                 RGBLED_REVISION,
                                 RGBLED_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            CFE_EVS_SendEvent(RGBLED_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            CFE_ES_WriteToSysLog("RGBLED - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RGBLED::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;
    static uint8 lastColor  = 0;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RGBLED_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RGBLED_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case RGBLED_WAKEUP_MID:
            {
                ProcessCmdPipe();
                
                /* If not in self test mode and the last color is not the 
                 * current value update */
                if(HkTlm.State != RGBLED_SELFTEST && 
                   lastColor != CVT.RGBLEDControl.Color)
                {
                    HkTlm.State = RGBLED_ON;
                    lastColor = CVT.RGBLEDControl.Color;
                    switch (CVT.RGBLEDControl.Color)
                    {
                        case RGBLED_COLOR_RED:
                        {
                            RGBLED_Custom_SetColor(255, 0, 0);
                            HkTlm.Color = RGBLED_COLOR_RED;
                            break;
                        }
    
                        case RGBLED_COLOR_GREEN:
                        {
                            RGBLED_Custom_SetColor(0, 255, 0);
                            HkTlm.Color = RGBLED_COLOR_GREEN;
                            break;
                        }
    
                        case RGBLED_COLOR_BLUE:
                        {
                            RGBLED_Custom_SetColor(0, 0, 255);
                            HkTlm.Color = RGBLED_COLOR_BLUE;
                            break;
                        }
    
                        case RGBLED_COLOR_AMBER:
                        {
                            RGBLED_Custom_SetColor(255, 255, 0);
                            HkTlm.Color = RGBLED_COLOR_AMBER;
                            break;
                        }
                        
                        case RGBLED_COLOR_YELLOW:
                        {
                            RGBLED_Custom_SetColor(255, 255, 0);
                            HkTlm.Color = RGBLED_COLOR_YELLOW;
                            break;
                        }
    
                        case RGBLED_COLOR_PURPLE:
                        {
                            RGBLED_Custom_SetColor(255, 0, 255);
                            HkTlm.Color = RGBLED_COLOR_PURPLE;
                            break;
                        }
    
                        case RGBLED_COLOR_CYAN:
                        {
                            RGBLED_Custom_SetColor(0, 255, 255);
                            HkTlm.Color = RGBLED_COLOR_CYAN;
                            break;
                        }
    
                        case RGBLED_COLOR_WHITE:
                        {
                            RGBLED_Custom_SetColor(255, 255, 255);
                            HkTlm.Color = RGBLED_COLOR_WHITE;
                            break;
                        }
    
                        default: /* COLOR_OFF */
                        {
                            RGBLED_Custom_SetColor(0, 0, 0);
                            HkTlm.Color = RGBLED_COLOR_OFF;
                            HkTlm.State = RGBLED_INITIALIZED;
                            break;
                        }
                    }
                }
                
                break;
            }
            case RGBLED_SEND_HK_MID:
            {
                ReportHousekeeping();
                break;
            }
            case PX4_LED_CONTROL_MID:
            {
                CFE_PSP_MemCpy(&CVT.RGBLEDControl, MsgPtr, sizeof(CVT.RGBLEDControl));
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(RGBLED_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
                break;
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, do nothing here.
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
        (void) CFE_EVS_SendEvent(RGBLED_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED::ProcessCmdPipe()
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
                case RGBLED_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(RGBLED_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(RGBLED_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process RGBLED Commands                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        /* All three commands currently have no arguments */
        uint16 ExpectedLength = sizeof(RGBLED_NoArgCmd_t); 
        /* Length verification is then the same for all three commands */
        if (VerifyCmdLength(MsgPtr, ExpectedLength))
        {
            uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
            switch (uiCmdCode)
            {
                case RGBLED_NOOP_CC:
                {
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(RGBLED_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                        "Recvd NOOP. Version %d.%d.%d.%d",
                        RGBLED_MAJOR_VERSION,
                        RGBLED_MINOR_VERSION,
                        RGBLED_REVISION,
                        RGBLED_MISSION_REV);
                    break;
                }
                case RGBLED_RESET_CC:
                {
                    HkTlm.usCmdCnt = 0;
                    HkTlm.usCmdErrCnt = 0;
                    break;
                }
                case RGBLED_SELFTEST_CC:
                {
                    if(HkTlm.State != RGBLED_SELFTEST)
                    {
                        HkTlm.usCmdCnt++;
                        previousState = HkTlm.State;
                        HkTlm.State = RGBLED_SELFTEST;
                        RGBLED_Custom_SelfTest();
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                        CFE_EVS_SendEvent(RGBLED_CMD_ERR_EID, CFE_EVS_ERROR, 
                                "RGBLED is already running self-test");
                    }
                    break;
                }
                default:
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(RGBLED_CC_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                    break;
                }
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send RGBLED Housekeeping                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean RGBLED::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(RGBLED_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* RGBLED Application C style main entry point.                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void RGBLED_AppMain()
{
    oRGBLED.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RGBLED Application C++ style main entry point.                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RGBLED - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RGBLED_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(RGBLED_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(RGBLED_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(RGBLED_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(RGBLED_SCH_PIPE_PEND_TIME);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RGBLED_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RGBLED Self-Test Complete, Update State                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED_SelfTest_Complete(void)
{
    oRGBLED.HkTlm.State = oRGBLED.previousState;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RGBLED Cleanup prior to exit                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RGBLED_CleanupCallback(void)
{
    RGBLED_Custom_Uninit();
}


/************************/
/*  End of File Comment */
/************************/
