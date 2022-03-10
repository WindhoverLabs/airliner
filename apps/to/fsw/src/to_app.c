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

#include "to_app.h"
#include "to_msg.h"
#include "to_version.h"
#include "to_platform_cfg.h"
#include "to_message_flow.h"
#include "to_priority_queue.h"
#include "to_scheduler.h"
#include "to_output_queue.h"
#include "to_custom.h"
#include "to_channel.h"
#include "to_tbldefs.h"
#include "to_verify.h"

/************************************************************************
** Global Variables
*************************************************************************/
TO_AppData_t  TO_AppData;

/************************************************************************
** Local Variables
*************************************************************************/
static TO_ChannelDiagTlm_t diagMsg;
static TO_MsgFlowListPkt_t msgFlowDiagPkt;

/************************************************************************
** Local Function Definitions
*************************************************************************/

/**
 * \brief Take individual initialization function's results returns overall TO application
 *        initialization result.
 *
 * \par This function supports the command, and therefore is only called in the context
 * of the main TO task, not one of the TO output tasks.
 *
 * @param status TO individual initialization function results
 *
 * @param events Result of call to TO_InitEvent, if events is T then TO has initialized
 *               event tables (has events).
 *
 * @return CFE_SUCCESS = TO application initialized without error, Not CFE_SUCCESS = TO application failed initialization
 */
static int32 TO_InitAppResult(int32 status, osalbool events);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_InitEvent(void)
{
    int32  regStatus = CFE_SUCCESS;
    int32  customStatus = CFE_SUCCESS;
    int32  ind = 0;
    uint32 channelID = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    (void) CFE_PSP_MemSet((void*)TO_AppData.EventTbl, 0x00, sizeof(TO_AppData.EventTbl));

    /* Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER
     * (the default) has been provided as an example. */

    TO_AppData.EventTbl[  ind].EventID = TO_MSG_ID_ERR_EID;
    TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

    TO_AppData.EventTbl[  ind].EventID = TO_PIPE_READ_ERR_EID;
    TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

    TO_AppData.EventTbl[  ind].EventID = TO_MSG_LEN_ERR_EID;
    TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;
    
    TO_AppData.EventTbl[  ind].EventID = TO_TLM_MSG_LEN_ERR_EID;
    TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;
        
    TO_AppData.EventTbl[  ind].EventID = TO_SB_RECEIVE_ERR_EID;
    TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;    

    for (channelID = 0; channelID < TO_MAX_CHANNELS; channelID++)
    {
        TO_AppData.EventTbl[  ind].EventID = TO_MSG_ID_ERR_EID;
        TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

        TO_AppData.EventTbl[  ind].EventID = TO_PUT_POOL_ERR_EID;
        TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

        TO_AppData.EventTbl[  ind].EventID = TO_PIPE_READ_ERR_EID;
        TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

        TO_AppData.EventTbl[  ind].EventID = TO_TLM_MSG_LEN_ERR_EID;
        TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_4_STOP;

        TO_AppData.EventTbl[  ind].EventID = TO_MF_MSG_ID_ERR_EID;
        TO_AppData.EventTbl[ind++].Mask    = CFE_EVS_FIRST_ONE_STOP;
    }

    customStatus = TO_Custom_InitEvent(&ind);

    if(TO_CUSTOM_INITEVENT_BAD_IND_ERR == customStatus)
    {
        (void) CFE_ES_WriteToSysLog("TO - Custom Init Event error: event table index out of range. Custom EID filters not set.");
    }
    else if(TO_CUSTOM_INITEVENT_INDEX_OCCUPIED == customStatus)
    {
        (void) CFE_ES_WriteToSysLog("TO - Custom Init Event error: event table occupied at starting index. Custom EID filters not set.");
    }
    else if(TO_CUSTOM_INITEVENT_TOO_MANY_FILTERS == customStatus)
    {
        (void) CFE_ES_WriteToSysLog("TO - Custom Init Event error: Filter limit reached. Some custom EID filters not set.");
    }
    else if(CFE_SUCCESS == customStatus)
    {
        (void) CFE_ES_WriteToSysLog("TO - Custom Init Event: Successfully set custom EID filters.");
    }
    else
    {
        (void) CFE_ES_WriteToSysLog("TO - Custom Init Event: Unknown Error.");
    }

    /* Register the table with CFE */
    regStatus = CFE_EVS_Register(TO_AppData.EventTbl,
                                 (uint16)ind, CFE_EVS_BINARY_FILTER);
    if (regStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("TO - Failed to register with EVS (0x%08X)\n", (unsigned int)regStatus);
    }
    else
    {
        /* If there were custom event init errors, send an EVS message*/
        switch (customStatus)
        {
            case TO_CUSTOM_INITEVENT_BAD_IND_ERR:
                (void) CFE_EVS_SendEvent(TO_EVT_CUSTOM_FILTERS_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Custom Init Event - Event table index out of range. Custom EID filters not set.");
                break;

            case TO_CUSTOM_INITEVENT_INDEX_OCCUPIED:
                (void) CFE_EVS_SendEvent(TO_EVT_CUSTOM_FILTERS_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Custom Init Event - Event table occupied at starting index. Custom EID filters not set.");
                break;

            case TO_CUSTOM_INITEVENT_TOO_MANY_FILTERS:
                (void) CFE_EVS_SendEvent(TO_EVT_CUSTOM_FILTERS_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Custom Init Event - Filter limit reached. Some custom EID filters not set.");
                break;

            case CFE_SUCCESS:
                break;

            default:
                (void) CFE_EVS_SendEvent(TO_EVT_CUSTOM_FILTERS_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Custom Init Event - Unknown error.");
                break;
        }
    }
    return (regStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_InitPipe(void)
{
    int32 status = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    status = CFE_SB_CreatePipe(&TO_AppData.SchPipeId,
                                 TO_SCH_PIPE_DEPTH,
                                 TO_SCH_PIPE_NAME);
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_SubscribeEx(TO_SEND_TLM_MID, TO_AppData.SchPipeId, CFE_SB_Default_Qos, TO_SCH_PIPE_WAKEUP_RESERVED);
        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(TO_SUBSCRIBE_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to TO_SEND_TLM_MID. (0x%08X)",
                                     (unsigned int)status);
            goto TO_InitPipe_Exit_Tag;
        }

        status = CFE_SB_SubscribeEx(TO_SEND_HK_MID, TO_AppData.SchPipeId, CFE_SB_Default_Qos, TO_SCH_PIPE_SEND_HK_RESERVED);
        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(TO_SUBSCRIBE_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to TO_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)status);
            goto TO_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(TO_INIT_SCHPIPE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)status);
            goto TO_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    status = CFE_SB_CreatePipe(&TO_AppData.CmdPipeId,
                                 TO_CMD_PIPE_DEPTH,
                                 TO_CMD_PIPE_NAME);
    if (status == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        status = CFE_SB_Subscribe(TO_CMD_MID, TO_AppData.CmdPipeId);
        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(TO_SUBSCRIBE_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to TO_CMD_MID. (0x%08X)",
                                     (unsigned int)status);
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(TO_INIT_CMDPIPE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)status);
    }

TO_InitPipe_Exit_Tag:
    return (status);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_InitData(void)
{
    int32 status = CFE_SUCCESS;

    /* Init housekeeping packet */
    CFE_SB_InitMsg(&TO_AppData.HkTlm,
                   TO_HK_TLM_MID, sizeof(TO_AppData.HkTlm), TRUE);

    status = OS_MutSemCreate(&TO_AppData.MutexID, "TO_APP", 0);
    if (status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(TO_CREATE_APPDATA_MUTEX_ERR_EID,
                CFE_EVS_ERROR,
                "Init Data: Failed to create TO_AppData mutex: 0x%08lX",
                (unsigned long)status);

        return status;
    }

    TO_AppData.HkTlm.ChannelMaxMem = TO_NUM_BYTES_IN_MEM_POOL;
    
    return CFE_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* TO initialization                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_InitApp(void)
{
    int32     status   = CFE_SUCCESS;
    osalbool  hasEvents = FALSE;

    status = TO_InitEvent();
    if (status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("TO - Failed to init events (0x%08X)\n", (unsigned int)status);
        goto TO_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = TRUE;
    }

    status = TO_InitPipe();
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_PIPE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)status);
        goto TO_InitApp_Exit_Tag;
    }

    status = TO_InitData();
    if (status != CFE_SUCCESS)
    {
        /* An error has already been sent in TO_InitData function */
        return status;
    }

    TO_Channel_InitAll();

    status = TO_Custom_Init();
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID,
                                 CFE_EVS_ERROR,
                                "Failed to init custom layer (0x%08X)",
            (unsigned int)status);
    }

TO_InitApp_Exit_Tag:
    return TO_InitAppResult(status, hasEvents);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* TO initialization result                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_InitAppResult(int32 status, osalbool events)
{
    if (CFE_SUCCESS == status)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_APP_INF_EID,
                                 CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 TO_MAJOR_VERSION,
                                 TO_MINOR_VERSION,
                                 TO_REVISION,
                                 TO_MISSION_REV);
        TO_PrintCustomVersion();
    }
    else
    {
        if (TRUE == events)
        {
            (void) CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID,
                                     CFE_EVS_ERROR, 
                                     "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("TO - Application failed to initialize\n");
        }
    }

    return (status);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* TO Cleanup                                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_Cleanup(void)
{
    TO_Channel_CleanupAll();
    OS_MutSemDelete(TO_AppData.MutexID);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_RcvMsg(int32 blocking)
{
    int32           status=CFE_SUCCESS;
    CFE_SB_Msg_t   *msgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    CFE_ES_PerfLogExit(TO_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    status = CFE_SB_RcvMsg(&msgPtr, TO_AppData.SchPipeId, blocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TO_MAIN_TASK_PERF_ID);

    if (CFE_SUCCESS == status)
    {
        MsgId = CFE_SB_GetMsgId(msgPtr);
        switch (MsgId)
        {
            case TO_SEND_TLM_MID:
                TO_ProcessNewCmds();
                TO_Channel_ProcessTelemetryAll();
                break;

            case TO_SEND_HK_MID:
                TO_ReportHousekeeping();
                break;

        default:
            (void) CFE_EVS_SendEvent(TO_MSG_ID_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
        }
    }
    else if (CFE_SB_NO_MESSAGE == status)
    {
        /* If there's no incoming message, you can do something here, or
         * nothing.  Note, this section is dead code only if the blocking arg
         * is CFE_SB_PEND_FOREVER. */
        status = CFE_SUCCESS;
    }
    else if (CFE_SB_TIME_OUT == status)
    {
        /* If there's no incoming message within a specified time (via the
         * blocking arg, you can do something here, or nothing.
         * Note, this section is dead code only if the blocking arg
         * is CFE_SB_PEND_FOREVER. */
        status = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(TO_PIPE_READ_ERR_EID,
                                 CFE_EVS_ERROR,
                                "SB pipe read error (0x%08X)", 
                                (unsigned int)status);
    }

    TO_ManageAllAppTables();

    return (status);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_ProcessNewCmds(void)
{
    int32 status = CFE_SUCCESS;
    CFE_SB_Msg_t   *cmdMsgPtr=NULL;
    CFE_SB_MsgId_t  cmdMsgId;
    uint32 i;

    /* Process command messages till the pipe is empty */
    for (i = 0; i < TO_CMD_PIPE_DEPTH; ++i)
    {
        status = CFE_SB_RcvMsg(&cmdMsgPtr, TO_AppData.CmdPipeId, CFE_SB_POLL);
        if (status == CFE_SUCCESS)
        {
            cmdMsgId = CFE_SB_GetMsgId(cmdMsgPtr);
            switch (cmdMsgId)
            {
                case TO_CMD_MID:
                    TO_ProcessNewAppCmds(cmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    TO_AppData.HkTlm.CmdErrCnt++;
                    (void) CFE_EVS_SendEvent(TO_MSG_ID_ERR_EID,
                                             CFE_EVS_ERROR,
                                             "Recvd invalid CMD msgId (0x%04X)",
                                             (unsigned short)cmdMsgId);
                    break;
            }
        }
        else if (CFE_SB_NO_MESSAGE == status)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(TO_PIPE_READ_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "CMD pipe read error (0x%08X)", (unsigned int)status);
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process TO Commands                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_ProcessNewAppCmds(CFE_SB_Msg_t *msgPtr)
{
    uint32  cmdCode = 0;
    osalbool sizeOk   = FALSE;
    osalbool isCmdOk  = FALSE;

    if (msgPtr != NULL)
    {
        cmdCode = CFE_SB_GetCmdCode(msgPtr);
        switch (cmdCode)
        {
            case TO_NOOP_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_NoArgCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_AppData.HkTlm.CmdCnt++;
                    (void) CFE_EVS_SendEvent(TO_CMD_NOOP_INF_EID, CFE_EVS_INFORMATION,
                                      "Executed NOOP cmd (%u), Version %d.%d.%d.%d",
                                      (unsigned int)cmdCode,
                                      TO_MAJOR_VERSION,
                                      TO_MINOR_VERSION,
                                      TO_REVISION,
                                      TO_MISSION_REV);
                    TO_PrintCustomVersion();
                }
                break;
            }

            case TO_RESET_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_NoArgCmd_t));

                if (TRUE == sizeOk)
                {
                    (void) OS_MutSemTake(TO_AppData.MutexID);
                    TO_AppData.HkTlm.CmdCnt = 0;
                    TO_AppData.HkTlm.CmdErrCnt = 0;
                    TO_AppData.HkTlm.SentBytes = 0;

                    (void) OS_MutSemGive(TO_AppData.MutexID);

                    TO_Channel_ResetCountsAll();

                    (void) CFE_EVS_SendEvent(TO_CMD_RESET_INF_EID, CFE_EVS_INFORMATION,
                                      "Executed RESET cmd (%u)", (unsigned int)cmdCode);
                }
                break;
            }

            case TO_ADD_MESSAGE_FLOW_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_AddMessageFlowCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_AddMessageFlowCmd_t *cmd = (TO_AddMessageFlowCmd_t*)msgPtr;

                    isCmdOk = TO_MessageFlow_Add(cmd->ChannelIdx, cmd->MsgID, cmd->MsgLimit, cmd->PQueueIdx);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                        (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_EID,
                                                 CFE_EVS_INFORMATION,
                                                 "Executed ADD_MESSAGE cmd (%u) on channel %d", (unsigned int)cmdCode,
                                                 cmd->ChannelIdx);
                    }
                }
                break;
            }

            case TO_REMOVE_MESSAGE_FLOW_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_RemoveMessageFlowCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_RemoveMessageFlowCmd_t *cmd = (TO_RemoveMessageFlowCmd_t*)msgPtr;

                    isCmdOk = TO_MessageFlow_Remove(cmd->ChannelIdx, cmd->MsgID);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                        (void) CFE_EVS_SendEvent(TO_CMD_REMOVE_MSG_FLOW_EID,
                                                 CFE_EVS_INFORMATION,
                                                 "Executed remove message flow cmd (%u) on channel %d", (unsigned int)cmdCode,
                                                 cmd->ChannelIdx);
                    }
                }
                break;
            }

            case TO_QUERY_MESSAGE_FLOW_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_QueryMessageFlowCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_QueryMessageFlowCmd_t *cmd = (TO_QueryMessageFlowCmd_t*)msgPtr;

                    isCmdOk = TO_MessageFlow_Query(cmd->ChannelIdx, cmd->MsgID);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                    }
                }
                break;
            }

            case TO_QUERY_PRIORITY_QUEUE_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_QueryPriorityQueueCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_QueryPriorityQueueCmd_t *cmd = (TO_QueryPriorityQueueCmd_t*)msgPtr;

                    isCmdOk = TO_PriorityQueue_Query(cmd->ChannelIdx, cmd->PQueueIndex);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                    }
                }
                break;
            }

            case TO_QUERY_OUTPUT_CHANNEL_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_QueryOutputChannelCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_QueryOutputChannelCmd_t *cmd = (TO_QueryOutputChannelCmd_t*)msgPtr;

                    isCmdOk = TO_OutputChannel_Query(cmd->ChannelIdx);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                    }
                }
                break;
            }

            case TO_SEND_DIAG_CC:
            {
                sizeOk = TO_VerifyCmdLength(msgPtr, sizeof(TO_SendDiagCmd_t));

                if (TRUE == sizeOk)
                {
                    TO_SendDiagCmd_t *cmd = (TO_SendDiagCmd_t*)msgPtr;

                    isCmdOk = TO_SendDiag(cmd->ChannelIdx);
                    if (FALSE == isCmdOk)
                    {
                        TO_AppData.HkTlm.CmdErrCnt++;
                    }
                    else
                    {
                        TO_AppData.HkTlm.CmdCnt++;
                        (void) CFE_EVS_SendEvent(TO_CMD_SEND_DIAG_EID, CFE_EVS_INFORMATION,
                                          "Successfully executed send diagnostics (%u) on channel %d", (unsigned int)cmdCode,
                                          cmd->ChannelIdx);
                    }
                }
                break;
            }

            default:
            {
                TO_OutputChannel_ProcessNewCustomCmds(msgPtr);
                break;
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send TO Housekeeping                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_ReportHousekeeping(void)
{
    uint32 i;
    TO_ChannelData_t *channel = NULL;

    (void) OS_MutSemTake(TO_AppData.MutexID);
    TO_AppData.HkTlm.SentBytes = 0;

    for (i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        channel = &TO_AppData.ChannelData[i];

        TO_AppData.HkTlm.ChannelInfo[i].CurrentlyQueuedCnt = channel->OutputQueue.CurrentlyQueuedCnt;
        TO_AppData.HkTlm.ChannelInfo[i].MessagesSent = channel->SentMsgCount;
        TO_AppData.HkTlm.ChannelInfo[i].TotalQueued = channel->OutputQueue.QueuedMsgCount;
        TO_AppData.HkTlm.ChannelInfo[i].SentBytes = channel->BytesSent;
        TO_AppData.HkTlm.ChannelInfo[i].MemInUse = channel->OutputQueue.MemInUse;
        TO_AppData.HkTlm.ChannelInfo[i].PeakMemInUse = channel->OutputQueue.PeakMemInUse;

        TO_AppData.HkTlm.SentBytes += channel->OutputQueue.SentBytes;

    }

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&TO_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&TO_AppData.HkTlm);
    (void) OS_MutSemGive(TO_AppData.MutexID);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send TO Diagnostic Message                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_SendDiag(uint16 ChannelIdx)
{
    uint32 messageFlow = 0;
    uint32 pQueue = 0;
    uint32 msgFlowPkt = 0;

    TO_ChannelData_t *channel = NULL;

    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_SEND_DIAG_ERR_EID,
                                 CFE_EVS_ERROR,
                                "Invalid channel index of %d.", ChannelIdx);
        return FALSE;
    }

    channel = &TO_AppData.ChannelData[ChannelIdx];

    /* Check if channel has been opened */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_SEND_DIAG_ERR_EID,
                                 CFE_EVS_ERROR,
                                "Channel %d has not been opened.", ChannelIdx);
        return FALSE;
    }

    /* This channel's config table pointer may be NULL, so we can't send diag
     * data for this channel. */
    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_SEND_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %d table is not available.", 
                                 (unsigned int)channel->channelIdx);
        return FALSE;
    }

    /* Message prep, msg flow diag data is sent out as separate messages from the main diag message */
    CFE_SB_InitMsg(&diagMsg, TO_DIAG_TLM_MID, sizeof(diagMsg), TRUE);
    CFE_SB_InitMsg(&msgFlowDiagPkt, TO_DIAG_MSG_FLOW_MID, sizeof(msgFlowDiagPkt), TRUE);

    diagMsg.ChannelIndex = ChannelIdx;
    msgFlowDiagPkt.ChannelIndex = ChannelIdx;

    TO_Channel_LockByRef(channel);

    diagMsg.MemPoolHandle = channel->OutputQueue.MemPoolHandle;
    diagMsg.MemFullCount = channel->OutputQueue.MemFullCount;
    diagMsg.State = channel->State;

    strncpy(diagMsg.ConfigTableName, channel->ConfigTableName,
            sizeof(diagMsg.ConfigTableName));
    diagMsg.ConfigTableName[CFE_TBL_MAX_NAME_LENGTH - 1] = '\0';

    strncpy(diagMsg.ConfigTableFileName, channel->ConfigTableFileName,
            sizeof(diagMsg.ConfigTableFileName));
    diagMsg.ConfigTableFileName[OS_MAX_PATH_LEN - 1] = '\0';
     
    strncpy(diagMsg.DumpTableName, channel->DumpTableName,
            sizeof(diagMsg.DumpTableName));
    diagMsg.DumpTableName[CFE_TBL_MAX_NAME_LENGTH - 1] = '\0';
       
    strncpy(diagMsg.ChannelName, channel->ChannelName,
            sizeof(diagMsg.ChannelName));
    diagMsg.ChannelName[OS_MAX_API_NAME - 1] = '\0';     

    diagMsg.TableID = channel->ConfigTblPtr->TableID;

    for (messageFlow = 0; messageFlow < TO_MAX_MESSAGE_FLOWS; ++messageFlow)
    {
        /* Circle packet index from 0 -> TO_MSG_FLOW_PKT_LIMIT*/
        msgFlowPkt = messageFlow % TO_MSG_FLOW_PKT_LIMIT;

        /* If first in packet, set offset of message flow in the table*/
        if (msgFlowPkt == 0)
        {
            msgFlowDiagPkt.Offset = messageFlow;
        }

        /* Fill message flow diagnostics until defined number of flows is met*/
        msgFlowDiagPkt.MessageFlow[msgFlowPkt].MsgId
            = channel->ConfigTblPtr->MessageFlow[messageFlow].MsgId;
        msgFlowDiagPkt.MessageFlow[msgFlowPkt].MsgLimit
            = channel->ConfigTblPtr->MessageFlow[messageFlow].MsgLimit;
        msgFlowDiagPkt.MessageFlow[msgFlowPkt].PQueueID
            = channel->ConfigTblPtr->MessageFlow[messageFlow].PQueueID;
        msgFlowDiagPkt.MessageFlow[msgFlowPkt].QueuedMsgCnt
            = channel->DumpTbl.MessageFlow[messageFlow].QueuedMsgCnt;

        /* If full send out as message.*/
        if (msgFlowPkt == TO_MSG_FLOW_PKT_LIMIT-1)
        {
            CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&msgFlowDiagPkt);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&msgFlowDiagPkt);

#ifdef TO_DEBUG_MODE
            OS_printf("TO: DIAG diagMsg.MessageFlow: ChannelID: %u Offset: %lu\n", msgFlowDiagPkt.ChannelIndex, msgFlowDiagPkt.Offset);
            OS_printf("TO: DIAG MsgId, MsgLimit, PQueueID, QueuedCnt\n");
            for (uint32 i = 0; i <= msgFlowPkt; ++i)
            {
                OS_printf("TO: DIAG %d: %0xX, %u, %u, %u\n",
                   (int)i,
                    msgFlowDiagPkt.MessageFlow[i].MsgId,
                    msgFlowDiagPkt.MessageFlow[i].MsgLimit,
                    msgFlowDiagPkt.MessageFlow[i].PQueueID,
                    msgFlowDiagPkt.MessageFlow[i].QueuedMsgCnt);
            }
#endif
            /*Reset struct, should zero out data*/
            CFE_SB_InitMsg(&msgFlowDiagPkt, TO_DIAG_MSG_FLOW_MID, sizeof(msgFlowDiagPkt), TRUE);
            msgFlowDiagPkt.ChannelIndex = ChannelIdx;
        }
    }
    
    /* If there is a partial message to send*/
    if (msgFlowPkt != TO_MSG_FLOW_PKT_LIMIT-1)
    {
        /* Send remaining msgFlow data*/
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&msgFlowDiagPkt);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&msgFlowDiagPkt);

#ifdef TO_DEBUG_MODE
        for (uint32 k = 0; k <= msgFlowPkt; ++k)
        {
          OS_printf("TO: DIAG %d: %0xX, %u, %u, %lu\n",
            (int)k,
            msgFlowDiagPkt.MessageFlow[k].MsgId,
            msgFlowDiagPkt.MessageFlow[k].MsgLimit,
            msgFlowDiagPkt.MessageFlow[k].PQueueID,
            msgFlowDiagPkt.MessageFlow[k].QueuedMsgCnt);
        }
#endif
    }

    for (pQueue = 0; pQueue < TO_MAX_PRIORITY_QUEUES; ++pQueue)
    {
        diagMsg.PQueue[pQueue].State
            = channel->ConfigTblPtr->PriorityQueue[pQueue].State;
        diagMsg.PQueue[pQueue].Depth
            = channel->ConfigTblPtr->PriorityQueue[pQueue].Depth;
        diagMsg.PQueue[pQueue].QType
            = channel->ConfigTblPtr->PriorityQueue[pQueue].QType;
        diagMsg.PQueue[pQueue].CurrentlyQueuedCnt
            = channel->DumpTbl.PriorityQueue[pQueue].CurrentlyQueuedCnt;
    }

    diagMsg.OQueue.SentCount
        = channel->OutputQueue.SentCount;
    diagMsg.OQueue.CurrentlyQueuedCnt
        = channel->OutputQueue.CurrentlyQueuedCnt;
    diagMsg.OQueue.HighwaterMark
        = channel->OutputQueue.HighwaterMark;
    diagMsg.OQueue.SentBytes
            = channel->OutputQueue.SentBytes;

    TO_Channel_UnlockByRef(channel);

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&diagMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&diagMsg);

#ifdef TO_DEBUG_MODE
    OS_printf("TO: DIAG ChannelIdx = %d\n", (int)diagMsg.ChannelIndex);
    OS_printf("TO: DIAG State = %d\n", diagMsg.State);
    OS_printf("TO: DIAG ChannelName = %s\n", diagMsg.ChannelName);
    OS_printf("TO: DIAG TableID = %lu\n", diagMsg.TableID);
    OS_printf("TO: DIAG diagMsg.PQueue:\n");
    OS_printf("TO: DIAG State, MsgLimit, QType, QueuedCnt, CurrQCnt, High, PassedCnt\n");
    for(pQueue = 0; pQueue < TO_MAX_PRIORITY_QUEUES; ++pQueue) {
        OS_printf("TO: DIAG %d: %d, %d, %d, %lu, %lu, %lu, %lu, %lu\n", (int)pQueue,
                  diagMsg.PQueue[pQueue].State,
                  diagMsg.PQueue[pQueue].MsgLimit,
                  diagMsg.PQueue[pQueue].QType,
                  diagMsg.PQueue[pQueue].QueuedMsgCnt,
                  diagMsg.PQueue[pQueue].CurrentlyQueuedCnt,
                  diagMsg.PQueue[pQueue].HighwaterMark);
    }
    OS_printf("TO: DIAG OQueue.SentCount = %lu\n", diagMsg.OQueue.SentCount);
    OS_printf("TO: DIAG OQueue.CurrentlyQueuedCnt = %u\n", diagMsg.OQueue.CurrentlyQueuedCnt);
    OS_printf("TO: DIAG OQueue.HighwaterMark = %u\n", diagMsg.OQueue.HighwaterMark);
#endif

    return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_VerifyCmdLength(CFE_SB_Msg_t* msgPtr, uint16 expectedLen)
{
    uint16  msgLen = 0;
    CFE_SB_MsgId_t MsgId = 0;
    uint16 cmdCode = 0;

    if (msgPtr != NULL)
    {
        msgLen = CFE_SB_GetTotalMsgLength(msgPtr);

        if (expectedLen != msgLen)
        {
            MsgId = CFE_SB_GetMsgId(msgPtr);
            cmdCode = CFE_SB_GetCmdCode(msgPtr);

            (void) CFE_EVS_SendEvent(TO_MSG_LEN_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Rcvd invalid msgLen: msgId=0x%04X, cmdCode=%d, "
                                     "msgLen=%d, expectedLen=%d",
                                     MsgId, cmdCode, msgLen, expectedLen);
                              
            (void) OS_MutSemTake(TO_AppData.MutexID);
            TO_AppData.HkTlm.CmdErrCnt++;
            (void) OS_MutSemGive(TO_AppData.MutexID);
            return FALSE;
        }
    }
    else
    {
        /* msgPtr is NULL */
        return FALSE;
    }

    return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* TO application entry point and main process loop                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_AppMain(void)
{
    int32 status = CFE_SUCCESS;

    /* Clear the AppData structure.  Global data appears to retain its value when an application is
     * restarted.
     */
    (void) CFE_PSP_MemSet(&TO_AppData, 0, sizeof(TO_AppData));

    /* Register the application with Executive Services */
    TO_AppData.runStatus = CFE_ES_APP_RUN;

    status = CFE_ES_RegisterApp();
    if (status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("TO - Failed to register the app (0x%08X)\n", (unsigned int)status);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TO_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (CFE_SUCCESS == status)
    {
        status = TO_InitApp();
    }

    if (CFE_SUCCESS == status)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(TO_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(TO_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(TO_MAIN_TASK_PERF_ID);
    }
    else
    {
        TO_AppData.runStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&TO_AppData.runStatus) == TRUE)
    {
        status = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);
        if (status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(TO_SB_RECEIVE_ERR_EID,
                                     CFE_EVS_CRITICAL,
                                    "Main loop error: SB receive: result = 0x%08X", (unsigned int)status);
        }
    }

    TO_Cleanup();

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TO_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(TO_AppData.runStatus);
}


/************************/
/*  End of File Comment */
/************************/
