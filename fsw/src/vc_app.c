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
#include "vc_app.h"
#include "vc_cmds.h"
#include "vc_msgids.h"
#include "vc_perfids.h"
#include <string.h>

/************************************************************************
** Global Variables
*************************************************************************/
VC_AppData_t VC_AppData;

/************************************************************************
** Local Function Definitions
*************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VC_InitEvent()
{
    int32 iStatus          = CFE_SUCCESS;
    int32 ind              = 0;
    int32 customEventCount = 0;

    CFE_EVS_BinFilter_t   EventTbl[VC_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet(EventTbl, 0x00, sizeof(EventTbl));

    EventTbl[  ind].EventID = VC_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_INIT_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_NOOP_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_RESET_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_CMD_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_DEV_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_CHA_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_UNINIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_CMD_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_PIPE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_MSGID_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = VC_MSGLEN_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    /* Add custom events to the filter table */
    customEventCount = VC_Custom_Init_EventFilters(ind, EventTbl);
    if(VC_CUSTOM_ERROR_CODE == customEventCount)
    {
        iStatus = CFE_EVS_APP_FILTER_OVERLOAD;
        (void) CFE_ES_WriteToSysLog("VC - Failed to init custom event filters (0x%08X)\n", (unsigned int)iStatus);
        goto VC_InitEvent_Exit_Tag;
    }
    
    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, (ind + customEventCount), CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VC - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

VC_InitEvent_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VC_InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&VC_AppData.SchPipeId,
                                 VC_SCH_PIPE_DEPTH,
                                 VC_SCH_PIPE_NAME);

    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(VC_PROCESS_CMDS_MID, VC_AppData.SchPipeId, CFE_SB_Default_Qos, VC_SCH_PIPE_WAKEUP_RESERVED);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to VC_PROCESS_CMDS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto VC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(VC_SEND_HK_MID, VC_AppData.SchPipeId, CFE_SB_Default_Qos, VC_SCH_PIPE_SEND_HK_RESERVED);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to VC_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto VC_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto VC_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&VC_AppData.CmdPipeId,
                                 VC_CMD_PIPE_DEPTH,
                                 VC_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(VC_CMD_MID, VC_AppData.CmdPipeId);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to VC_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto VC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto VC_InitPipe_Exit_Tag;
    }

VC_InitPipe_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VC_InitData()
{
    int32  iStatus = CFE_SUCCESS;

    /* Setup app state */
    CFE_PSP_MemSet((void*)&VC_AppData, 0x00, sizeof(VC_AppData));
    VC_AppData.AppState = VC_UNINITIALIZED;
    VC_AppData.uiRunStatus = CFE_ES_APP_RUN;
    
    /* Init housekeeping packet */
    CFE_SB_InitMsg(&VC_AppData.HkTlm,
                   VC_HK_TLM_MID, sizeof(VC_AppData.HkTlm), TRUE);
                   
    /* Initialize custom device data */
    iStatus = VC_Devices_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        goto VC_InitData_Exit_Tag;
    }

    /* Initialize custom transmit data */
    iStatus = VC_Transmit_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        goto VC_InitData_Exit_Tag;
    }
    
VC_InitData_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup prior to exit                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_CleanupCallback()
{
    VC_Devices_Critical_Cleanup();
    VC_Transmit_Critical_Cleanup();
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Normal application exit                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_CleanupExit(void)
{
    if(VC_Devices_Stop() != TRUE)
    {
        (void) CFE_EVS_SendEvent(VC_UNINIT_ERR_EID, CFE_EVS_ERROR,"VC_Devices_Stop failed");
    }

    if(VC_Devices_Uninit() != TRUE)
    {
        (void) CFE_EVS_SendEvent(VC_UNINIT_ERR_EID, CFE_EVS_ERROR,"VC_Devices_Uninit failed");
    }

    if(VC_Transmit_Uninit() != TRUE)
    {
        (void) CFE_EVS_SendEvent(VC_UNINIT_ERR_EID, CFE_EVS_ERROR,"VC_Transmit_Uninit failed");
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize the application                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VC_InitApp(void)
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;
    
    iStatus = VC_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VC - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto VC_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }
    
    iStatus = VC_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto VC_InitApp_Exit_Tag;
    }
    
    iStatus = VC_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                "Failed to init pipes (0x%08X)",
                                (unsigned int)iStatus);
        goto VC_InitApp_Exit_Tag;
    }
    
    /* Initialize data transmission resources */
    if (FALSE == VC_Transmit_Init()) 
    {
        /* Data transmit sink failed to initialize for now error out */   
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,"VC_Transmit_Init failed");
        iStatus = -1;
        goto VC_InitApp_Exit_Tag;
    }
    
    /* Initialize devices */
    if (FALSE == VC_Devices_Init()) 
    {
        /* Device failed to initialize for now error out */
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,"VC_Devices_Init failed");
        iStatus = -1;
        goto VC_InitApp_Exit_Tag;
    }
    else
    {
        /* Initialization was successful set state */
        VC_AppData.AppState = VC_INITIALIZED;
    }
    
    /* Start streaming */
    if (FALSE == VC_Devices_Start()) 
    {
        /* Start streaming failed, raise event but for now don't error out */
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,"VC_Devices_Start failed in InitApp");
    }
    else
    {
        /* Streaming is successful set state */
        VC_AppData.AppState = VC_STREAMING;
    }
    
    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&VC_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto VC_InitApp_Exit_Tag;
    }
    
VC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        /* Application startup event message */
        (void) CFE_EVS_SendEvent(VC_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 VC_MAJOR_VERSION,
                                 VC_MINOR_VERSION,
                                 VC_REVISION,
                                 VC_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("VC - Application failed to initialize\n");
        }
    }
    return (iStatus);
} /* End of VC_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VC_RcvMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, VC_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(VC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
    {
            case VC_PROCESS_CMDS_MID:
            {
                VC_ProcessNewCmds();
                break;
            }
            case VC_SEND_HK_MID:
            {
                VC_ReportHousekeeping(MsgPtr);
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(VC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
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
        (void) CFE_EVS_SendEvent(VC_PIPE_ERR_EID, CFE_EVS_ERROR,
            "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        VC_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, VC_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case VC_CMD_MID:
                {
                    VC_ProcessNewAppCmds(CmdMsgPtr);
                    break;
                }
                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    VC_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(VC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(VC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            VC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process VC Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case VC_NOOP_CC:
            {
                VC_NoopCmd(MsgPtr);
                break;
            }
            case VC_RESET_CC:
            {
                VC_ResetCmd(MsgPtr);
                break;
            }
            default:
            {
                VC_ProcessNewCustomCmds(MsgPtr);
                break;
            }
        }
    }
} /* end VC_ProcessNewAppCmds() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VC application entry point and main process loop                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_AppMain(void)
{
    /* Setup the RunStatus variable */
    VC_AppData.uiRunStatus = CFE_ES_APP_RUN;
    
    /* Register the application with Executive Services */
    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VC - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Create the first Performance Log entry */
    CFE_ES_PerfLogEntry(VC_MAIN_TASK_PERF_ID); 
   
    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = VC_InitApp();
    }
    
    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(VC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(VC_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(VC_MAIN_TASK_PERF_ID);
    }
    else
    {
        VC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

   /* Application main loop */
    while(CFE_ES_RunLoop(&VC_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = VC_RcvMsg(VC_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(VC_PIPE_ERR_EID, CFE_EVS_ERROR, \
                              "SB Pipe Read Error, VC App will exit. RC = 0x%08X", (unsigned int)iStatus);

            VC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }

    } /* end CFS_ES_RunLoop while */

    /* Pre-normal exit cleanup */
    VC_CleanupExit();

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VC_MAIN_TASK_PERF_ID);

   /* Exit the application */
   CFE_ES_ExitApp(VC_AppData.uiRunStatus);
} /* end VC_AppMain */

/************************/
/*  End of File Comment */
/************************/
