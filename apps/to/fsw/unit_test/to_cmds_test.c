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

#include "to_cmds_test.h"
#include "to_app.h"
#include "to_test_utils.h"
#include "to_custom_hooks.h"
#include "ut_cfe_es_hooks_ex.h"
#include "ut_osapi_hooks.h"
#include "ut_to_custom_hooks.h"
#include "ut_to_custom_stubs.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"


static CFE_SB_Msg_t* TO_test_CmdSendMsg_msgPtr = NULL;
static int TO_test_CFE_SB_CmdRcvMsg_successCount = 0;

/* CFE_SB_SendMsg_TO_RcvMsgHook */
int32 CFE_SB_SendMsg_TO_CmdRcvMsgHook(CFE_SB_Msg_t* MsgPtr) {
    TO_test_CmdSendMsg_msgPtr = MsgPtr;
    if (TO_test_CFE_SB_CmdRcvMsg_successCount > 0) {
        TO_test_CFE_SB_CmdRcvMsg_successCount--;
        return CFE_SUCCESS;
    } else {
        return CFE_SB_NO_MESSAGE;
    }
}

/**************************************************************************
 * Tests for TO_ProcessNewCmds()
 **************************************************************************/
/**
 * Test TO_ProcessNewAppCmds(), Default
 */
void Test_TO_ProcessNewAppCmds_Default(void)
{
    uint16  ChannelIdx = 0;
    TO_ChannelData_t* channel;

    CFE_SB_Msg_t *msgPtr;

    CFE_PSP_MemSet(&msgPtr, 0x01, sizeof(msgPtr));

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETCMDCODE_INDEX, 42, 1);

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    TO_ProcessNewAppCmds(msgPtr);
}


/**
 * Test TO_ProcessNewAppCmds(), MsgPtr_Null
 */
void Test_TO_ProcessNewAppCmds_MsgPtr_Null(void)
{
    uint16  ChannelIdx = 0;
    TO_ChannelData_t* channel;

    CFE_SB_Msg_t *msgPtr;

    CFE_PSP_MemSet(&msgPtr, 0x00, sizeof(msgPtr));

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETCMDCODE_INDEX, 42, 1);

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    TO_ProcessNewAppCmds(msgPtr);
}


/**
 * Test TO_ProcessNewCmds(), RcvMsgErr
 */
void Test_TO_ProcessNewCmds_RcvMsgErr(void)
{
    int32 errCodeSet = CFE_SB_MSG_TOO_BIG; //Anything other than CFE_SUCCESS or CFE_SB_NO_MESSAGE

    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    sprintf(expectedEvent, "CMD pipe read error (0x%08X)", (unsigned int)errCodeSet);

    /* Set a fail result */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, errCodeSet, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);

    /* Execute the function being tested */
    TO_ProcessNewCmds();

    /* Verify results */
    UtAssert_EventSent(TO_PIPE_READ_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "ProcessNewCmds, RcvMsgErr");
}


/**
 * Test TO_ProcessNewAppCmds(), NOOP command, Invalid Size
 */
void Test_TO_ProcessNewAppCmds_Noop_InvalidSize(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_AddMessageFlowCmd_t InNoopCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InNoopCmd, TO_CMD_MID, sizeof(InNoopCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InNoopCmd, TO_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe(&InNoopCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
                       "TO_ProcessNewAppCmds, NOOP command, Invalid Size");
}


/**
 * Test TO_ProcessNewAppCmds(), NOOP command, Nominal
 */
void Test_TO_ProcessNewAppCmds_Noop_Nominal(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_NoArgCmd_t InNoopCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InNoopCmd, TO_CMD_MID, sizeof(InNoopCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InNoopCmd, TO_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe(&InNoopCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_NOOP_INF_EID, CFE_EVS_INFORMATION, "",
                       "TO_ProcessNewAppCmds, NOOP command, Nominal");
}


/**
 * Test TO_ProcessNewAppCmds(), Reset command, Nominal
 */
void Test_TO_ProcessNewAppCmds_Reset_Nominal(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_NoArgCmd_t InResetCmd;
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Now give all the counters we're going to clear a value to ensure that
     * the reset command actually clears them.
     */
    TO_AppData.HkTlm.CmdCnt = 1;
    TO_AppData.HkTlm.CmdErrCnt = 2;

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_MESSAGE_FLOWS; ++j)
        {
            TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].QueuedMsgCnt = 6;
        }
    }

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_PRIORITY_QUEUES; ++j)
        {
            TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].QueuedMsgCnt = 8;
            TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].HighwaterMark = 9;
        }
    }

    for(i=0; i < TO_MAX_CHANNELS; ++i)
    {
        TO_AppData.ChannelData[i].OutputQueue.SentCount = 10;
        TO_AppData.ChannelData[i].OutputQueue.HighwaterMark = 11;
    }

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_RESET_INF_EID, CFE_EVS_INFORMATION, "", "RESET Cmd Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 0, "TO_AppData.HkTlm.CmdErrCnt == 0");

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_MESSAGE_FLOWS; ++j)
        {
            UtAssert_True(TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].QueuedMsgCnt == 0,
                    "TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].QueuedMsgCnt == 0");
        }
    }

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_PRIORITY_QUEUES; ++j)
        {
            UtAssert_True(TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].QueuedMsgCnt == 0,
                    "TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].QueuedMsgCnt == 0");
            UtAssert_True(TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].HighwaterMark == 0,
                    "TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].HighwaterMark == 0");
        }
    }

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        UtAssert_True(TO_AppData.ChannelData[i].OutputQueue.SentCount == 0,
                "TO_AppData.ChannelData[i].OutputQueue.SentCount == 0");
        UtAssert_True(TO_AppData.ChannelData[i].OutputQueue.HighwaterMark == 0,
                "TO_AppData.ChannelData[i].OutputQueue.HighwaterMark == 0");
    }
}


/**
 * Test TO_ProcessNewAppCmds(), Reset command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_Reset_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_CMD_RESET_INF_EID, CFE_EVS_INFORMATION, "", "RESET Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow_InvalidPQueueIdx
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidPQueueIdx(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_AddMessageFlowCmd_t InCmd;
    int32         DataPipe;
    int32         CmdPipe;

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = TO_MAX_PRIORITY_QUEUES;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
                       "TO_ProcessNewAppCmds, AddMessageFlow_InvalidPQueueIdx");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, AlreadyDefined
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_AlreadyDefined(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   DataPipe;
    int32                   CmdPipe;

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = 0;
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
            "TO_ProcessNewAppCmds, AddMessageFlow command, AlreadyDefined");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Nominal
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   SchPipe;
    int32                   CmdPipe;

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    char            expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg ((void *)&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InSchMsg, (CFE_SB_PipeId_t)SchPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg ((void *)&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = 0;
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe((void *)&InCmd, (CFE_SB_PipeId_t)CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    sprintf(expectedEvent, "Executed ADD_MESSAGE cmd (%u) on channel %d",
             TO_ADD_MESSAGE_FLOW_CC, InCmd.ChannelIdx);
    /* Verify results */
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_EID, CFE_EVS_INFORMATION, expectedEvent,
                      "Add Message Flow Cmd Event Sent");
    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId == MsgId,
        "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->.MessageFlow[0].MsgId");
    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgLimit == MsgLimit,
        "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgLimit");
    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].PQueueID == PQueueIdx,
        "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].PQueueID");
}


/**
 * Test TO_ProcessNewAppCmds(), Add Message Flow command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         SchPipe;
    int32         CmdPipe;
    char          expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg ((void *)&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InSchMsg, (CFE_SB_PipeId_t)SchPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg ((void *)&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_ADD_MESSAGE_FLOW_CC);
    Ut_CFE_SB_AddMsgToPipe((void *)&InResetCmd, (CFE_SB_PipeId_t)CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    sprintf(expectedEvent,
             "Rcvd invalid msgLen: msgId=0x%04X, cmdCode=%d, msgLen=%d, expectedLen=%d",
             TO_CMD_MID, TO_ADD_MESSAGE_FLOW_CC, 100, sizeof(TO_AddMessageFlowCmd_t));

    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
    UtAssert_EventNotSent(TO_CMD_ADD_MSG_FLOW_EID, CFE_EVS_INFORMATION, "",
                          "ADD MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "TO_ProcessNewAppCmds, Add Message Flow, invalid command length");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Full
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Full(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   SchPipe;
    int32                   CmdPipe;
    int32                   i = 0;
    uint8                   channelIdx = 0;

    CFE_SB_MsgId_t     MsgId = 0x1fff;
    uint16             MsgLimit = 1;
    uint16             PQueueIdx = 0;
    TO_ChannelData_t*  channel = NULL;
    char               expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a SCH message,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg ((void *)&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InSchMsg, (CFE_SB_PipeId_t)SchPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg((void *)&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe((void *)&InCmd, (CFE_SB_PipeId_t)CmdPipe);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_InitApp();

    channel = &TO_AppData.ChannelData[channelIdx];
    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; i++)
    {
        channel->ConfigTblPtr->MessageFlow[i].MsgId = i + 1;
    }

    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent,
             "Failed to add message flow. No unused slots available on channel %d.",
             InCmd.ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                  "TO_ProcessNewAppCmds, AddMessageFlow, Full");

    TO_Cleanup();
}


/**
 * Test TO_ProcessNewAppCmds(), RemoveMessageFlow command, Nominal
 */
void Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_RemoveMessageFlowCmd_t   InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_REMOVE_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_REMOVE_MSG_FLOW_EID, CFE_EVS_INFORMATION, "",
        "Remove Message Flow Cmd Event Sent");
    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId == 0,
        "Remove Message Flow Cmd clear TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId");
}


/**
 * Test TO_ProcessNewAppCmds(), Remove Message Flow command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_RemoveMessageFlow_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_REMOVE_MESSAGE_FLOW_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_MSG_FLOW_INFO_EID, CFE_EVS_INFORMATION, "",
             "REMOVE MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
             "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryMessageFlow command, Nominal
 */
void Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryMessageFlowCmd_t    InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    char expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    sprintf(expEventText, "MID=0x%04x ML=%u PQI=%u Q=%lu SB=%lu",
            TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId,
            TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgLimit,
            TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].PQueueID,
            TO_AppData.ChannelData[0].DumpTbl.MessageFlow[0].QueuedMsgCnt,
            TO_AppData.ChannelData[0].DumpTbl.MessageFlow[0].SBMsgCnt);

    UtAssert_EventSent(TO_MSG_FLOW_INFO_EID, CFE_EVS_INFORMATION, "",
             "TO_ProcessNewAppCmds, QueryMessageFlow Nominal");
}


/**
 * Test TO_ProcessNewAppCmds(), Query Message Flow command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_QueryMessageFlow_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_QUERY_MESSAGE_FLOW_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_MSG_FLOW_INFO_EID, CFE_EVS_INFORMATION, "",
             "QUERY MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
             "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue command, Nominal
 */
void Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Nominal(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_QueryPriorityQueueCmd_t InCmd;
    int32         DataPipe;
    int32         CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16        PQueueIdx = 0;
    char expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_PRIORITY_QUEUE_CC);
    InCmd.PQueueIndex = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    sprintf(expEventText, "CHANNEL=%d PQI=%u S=%u D=%u QT=%u", 0, PQueueIdx,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].State,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].Depth,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].QType);

    UtAssert_EventSent(TO_PQUEUE_INFO_EID, CFE_EVS_INFORMATION, "",
             "TO_ProcessNewAppCmds, QueryPriorityQueue Nominal");
}


/**
 * Test TO_ProcessNewAppCmds(), Query Priority Queue command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_QueryPriorityQueue_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_QUERY_PRIORITY_QUEUE_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_PQUEUE_INFO_EID, CFE_EVS_INFORMATION, "",
             "REMOVE MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
             "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryOutputChannel command, Nominal
 */
void Test_TO_ProcessNewAppCmds_QueryOutputChannel_Nominal(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryOutputChannelCmd_t  InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16  ChannelIdx = 0;
    char expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_OUTPUT_CHANNEL_CC);
    InCmd.ChannelIdx = ChannelIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    sprintf(expEventText, "S=%u ML=%u SC=%lu CQC=%u HWM=%u",
            TO_AppData.ChannelData[0].State, TO_OUTPUT_QUEUE_DEPTH,
            TO_AppData.ChannelData[0].OutputQueue.SentCount,
            TO_AppData.ChannelData[0].OutputQueue.CurrentlyQueuedCnt,
            TO_AppData.ChannelData[0].OutputQueue.HighwaterMark);

    UtAssert_EventSent(TO_OUT_CH_INFO_EID, CFE_EVS_INFORMATION, "",
             "TO_ProcessNewAppCmds, QueryOutputChannel Nominal");
}


/**
 * Test TO_ProcessNewAppCmds(), Query Output Channel command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_QueryOutputChannel_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_QUERY_OUTPUT_CHANNEL_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_OUT_CH_INFO_EID, CFE_EVS_INFORMATION, "",
             "Query Output Queue Cmd Event Not Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
             "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**
 * Test TO_ProcessNewAppCmds Send diag command
 */
void Test_TO_ProcessNewAppCmds_SendDiag_Success(void) {

    TO_NoArgCmd_t    InSchMsg;
    TO_SendDiagCmd_t InCmd;
    int32            DataPipe;
    int32            CmdPipe;
    uint8            ChannelIdx = 0;
    int32            i=0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_SEND_DIAG_CC);
    InCmd.ChannelIdx = ChannelIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 1,
                  "Send Diag Success - Command Accept Count bumped");
    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_TLM_MID),
                  "Send Diag Success - Channel Diag Packet sent");

    int32 numMsgFlowPackets = (int32)ceil((double)TO_MAX_MESSAGE_FLOWS/TO_MSG_FLOW_PKT_LIMIT);
    int32 msgFlowPacketCount = 0;

    for(i=0; i<numMsgFlowPackets; ++i)
    {
        if(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID))
        {
            msgFlowPacketCount++;
        }
    }

    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID),
                  "TO_ProcessNewAppCmds, SendDiag_Success");
}


/**
 * Test TO_ProcessNewAppCmds(), Send Diag command, invalid command length
 */
void Test_TO_ProcessNewAppCmds_SendDiag_InvalidCmdLength(void)
{
    TO_NoArgCmd_t InSchMsg;
    char          InResetCmd[100];
    int32         DataPipe;
    int32         CmdPipe;
    uint32        i = 0;
    uint32        j = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, TO_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_SEND_DIAG_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID) == FALSE,
                  "Send Diag Reject - Channel MsgFlow Diag Packets Not sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
                       "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
}


/**************************************************************************
 * Additional tests for TO_ProcessNewAppCmds
 **************************************************************************/
/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow_Channel_Invalid
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   SchPipe;
    int32                   CmdPipe;
    uint16                  channelIdx = 5;

    CFE_SB_MsgId_t  MsgId = 0x0a20;
    uint16          MsgLimit = 100;
    uint16          PQueueIdx = 0;
    char            expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a SCH message,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg ((void *)&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InSchMsg, (CFE_SB_PipeId_t)SchPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg ((void *)&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = channelIdx;
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe((void *)&InCmd, (CFE_SB_PipeId_t)CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    sprintf(expectedEvent, "Invalid channel (channel = %d, but max = %d).",
             channelIdx, TO_MAX_CHANNELS);

    /* Verify results */
     UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                        "TO_ProcessNewAppCmds, AddMessageFlow_Channel_Invalid");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow_Channel_Null_ConfigTblPtr
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   SchPipe;
    int32                   CmdPipe;
    uint16                  channelIdx = 0;

    CFE_SB_MsgId_t     MsgId = 0x0a20;
    uint16             MsgLimit = 100;
    uint16             PQueueIdx = 0;
    TO_ChannelData_t*  channel = NULL;
    char               expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg ((void *)&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InSchMsg, (CFE_SB_PipeId_t)SchPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg ((void *)&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = channelIdx;
    InCmd.MsgID = MsgId;
    InCmd.MsgLimit = MsgLimit;
    InCmd.PQueueIdx = PQueueIdx;
    Ut_CFE_SB_AddMsgToPipe((void *)&InCmd, (CFE_SB_PipeId_t)CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_InitApp();

    channel = &TO_AppData.ChannelData[channelIdx];
    channel->ConfigTblPtr = NULL;

    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "Channel missing valid table data (channel = %d).",
             channelIdx);

    /* Verify results */
     UtAssert_EventSent(TO_MSG_FLOW_MISSING_TBL_ERR_EID, CFE_EVS_ERROR, expectedEvent,
              "TO_ProcessNewAppCmds, AddMessageFlow_Channel_Null_ConfigTblPtr");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, SubscribeEx Failure
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure(void)
{
    CFE_SB_MsgId_t  MsgId = FAC_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          channelIdx = 0;
    osalbool        result = TRUE;
    osalbool        expected = FALSE;

    TO_ChannelTbl_t table;
    CFE_PSP_MemSet(&table, 0x0, sizeof(table));

    /* Get channel data information */
    TO_AppData.ChannelData[channelIdx].State = TO_CHANNEL_OPENED;
    TO_AppData.ChannelData[channelIdx].ConfigTblPtr = &table;
    TO_AppData.ChannelData[channelIdx].ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, CFE_SB_BAD_ARGUMENT, 3);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_SUBSCRIBEEX_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Execute the function being tested */
    result = TO_MessageFlow_Add(channelIdx, MsgId, MsgLimit, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "Message flow failed to subscribe on channel.");

    UtAssert_EventSent(TO_UNSUBSCRIBE_ERR_EID, CFE_EVS_ERROR, "",
                       "TO_ProcessNewAppCmds(), AddMessageFlow SubscribeEx Failure");
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow fail, Channel Not Open
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Fail_Channel_Not_Open(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          channelIdx = 0;
    osalbool        result = TRUE;
    osalbool        expected = FALSE;

    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];
    channel->State = TO_CHANNEL_CLOSED;

    /* Execute the function being tested */
    result = TO_MessageFlow_Add(channelIdx, MsgId, MsgLimit, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");

    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds(), AddMessageFlow fail, Channel Not Open");
}


/**
 * Test TO_ProcessNewAppCmds(), MessageFlow_Remove_MsgId_That_Does_Not_Exist
 */
void Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_That_Does_Not_Exist(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_RemoveMessageFlowCmd_t   InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16                      channelIdx = 0;
    CFE_SB_MsgId_t  MsgId = 0x0a20;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_REMOVE_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds(), MessageFlow_Remove_MsgId_That_Does_Not_Exist");
}


/**
 * Test TO_ProcessNewAppCmds(), MessageFlow_Remove_Channel_Invalid
 */
void Test_TO_ProcessNewAppCmds_MessageFlow_Remove_Channel_Invalid(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_RemoveMessageFlowCmd_t   InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16                      channelIdx = 5;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_REMOVE_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;
    InCmd.ChannelIdx = channelIdx;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, MessageFlow_Remove_Channel_Invalid");
}


/**
 * Test TO_ProcessNewAppCmds(), MessageFlow_Remove_MsgId_Channel_Not_Open
 */
void Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_Channel_Not_Open(void)
{
    uint16                      channelIdx = 0;
    CFE_SB_MsgId_t              MsgId = CFE_ES_HK_TLM_MID;
    osalbool                    result = TRUE;
    osalbool                    expected = FALSE;

    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];
    channel->State = TO_CHANNEL_CLOSED;

    /* Execute the function being tested */
    result = TO_MessageFlow_Remove(channelIdx, MsgId);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");

    UtAssert_EventSent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, MessageFlow_Remove_MsgId_Channel_Not_Open");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryMessageFlow Nominal channel invalid
 */
void Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Invalid(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryMessageFlowCmd_t    InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;
    InCmd.ChannelIdx = 5;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_MSG_FLOW_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryMessageFlow Nominal channel invalid");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryMessageFlow command, Message Flow Not Defined
 */
void Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_MsgFlow_Not_Defined(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryMessageFlowCmd_t    InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16                      channelIdx = 0;
    CFE_SB_MsgId_t  MsgId = 0x0a20;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;
    InCmd.ChannelIdx = channelIdx;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_MSG_FLOW_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryMessageFlow Message Flow Not Defined");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue command, Channel Index Invalid
 */
void Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Channel_Index_Invalid(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_QueryPriorityQueueCmd_t InCmd;
    int32         DataPipe;
    int32         CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16            PQueueIdx = 0;
    char expEventText[100];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_PRIORITY_QUEUE_CC);
    InCmd.PQueueIndex = PQueueIdx;
    InCmd.ChannelIdx = 4;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryPriorityQueue Channel Index Invalid");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue_PQ_Invalid
 */
void Test_TO_ProcessNewAppCmds_QueryPriorityQueue_PQ_Invalid(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_QueryPriorityQueueCmd_t InCmd;
    int32         DataPipe;
    int32         CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16            PQueueIdx = TO_MAX_PRIORITY_QUEUES;
    uint16          channelIdx = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_PRIORITY_QUEUE_CC);
    InCmd.PQueueIndex = PQueueIdx;
    InCmd.ChannelIdx = channelIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryPriorityQueue_PQ_Invalid");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryOutputChannel Fail ChannelID invalid (out of range)
 */
void Test_TO_ProcessNewAppCmds_QueryOutputChannel_Fail_ChannelID_Invalid(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryOutputChannelCmd_t  InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16  ChannelIdx = 5;
    char expEventText[100];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_QUERY_OUTPUT_CHANNEL_CC);
    InCmd.ChannelIdx = ChannelIdx;
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    UtAssert_EventSent(TO_OUT_CH_INFO_ERR_EID, CFE_EVS_ERROR, "",
            "TO_ProcessNewAppCmds, QueryOutputChannel Fail ChannelID invalid");
}


/**
 * Test TO_ProcessNewAppCmds Send diag Fail ChannelID invalid (out of range)
 */
void Test_TO_ProcessNewAppCmds_SendDiag_Fail_ChannelID_Invalid(void) {
    TO_NoArgCmd_t           InSchMsg;
    TO_SendDiagCmd_t        InCmd;
    int32                   DataPipe;
    int32                   CmdPipe;
    uint16                  ChannelIdx = 5;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_SEND_DIAG_CC);
    InCmd.ChannelIdx = ChannelIdx;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_SEND_DIAG_ERR_EID, CFE_EVS_ERROR, "",
                       "TO_ProcessNewAppCmds, Send diag Fail ChannelID invalid");
}


/**
 * Test TO_ProcessNewAppCmds Send diag command Fail Channel not open
 */
void Test_TO_ProcessNewAppCmds_SendDiag_Fail_Channel_Not_Open(void)
{
    uint16                  ChannelIdx = 0;
    osalbool                result = TRUE;
    osalbool                expected = FALSE;

    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->State = TO_CHANNEL_CLOSED;

    /* Execute the function being tested */
    result = TO_SendDiag(ChannelIdx);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");

    UtAssert_EventSent(TO_CMD_SEND_DIAG_ERR_EID, CFE_EVS_ERROR, "",
                       "TO_ProcessNewAppCmds Send diag Fail Channel not open");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryMessageFlow_Nominal_Channel_Not_Open
 */
void Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Not_Open(void)
{
    uint16                      channelIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    osalbool                    result = TRUE;
    osalbool                    expected = FALSE;

    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];
    channel->State = TO_CHANNEL_CLOSED;

    /* Execute the function being tested */
    result = TO_MessageFlow_Query(channelIdx, MsgId);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");

    UtAssert_EventSent(TO_MSG_FLOW_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryMessageFlow_Nominal_Channel_Not_Open");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue Fail_Channel_Not_Open
 */
void Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Fail_Channel_Not_Open(void)
{
    uint16          channelIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          PQueueIdx = 0;
    osalbool        result = TRUE;
    osalbool        expected = FALSE;
    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];
    channel->State = TO_CHANNEL_CLOSED;

    /* Execute the function being tested */
    result = TO_PriorityQueue_Query(channelIdx, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");

    UtAssert_EventSent(TO_PQUEUE_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, QueryPriorityQueue Fail_Channel_Not_Open");
}


/**
 * Test TO_ProcessNewAppCmds(), RemoveMessageFlow command, Unsubscribe_Fail
 */
void Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_RemoveMessageFlowCmd_t   InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    CFE_SB_MsgId_t              MsgId = CFE_ES_HK_TLM_MID;
    char                        expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_REMOVE_MESSAGE_FLOW_CC);
    InCmd.MsgID = MsgId;

    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, CFE_SB_BAD_ARGUMENT, 1);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    sprintf(expectedEvent,
            "Message flow (0x%08X) failed to unsubscribe on channel %d. (%ld)",
            MsgId, 0, CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_EventSent(TO_UNSUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
             "ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail");
}


/**
 * Test TO_ProcessNewAppCmds(), Output_Channel_Query_Channel_Not_Open
 */
void Test_TO_ProcessNewAppCmds_Output_Channel_Query_Channel_Not_Open(void)
{
    uint16                      channelIdx = 0;
    CFE_SB_MsgId_t              MsgId = CFE_ES_HK_TLM_MID;
    osalbool                    result = TRUE;
    osalbool                    expected = FALSE;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    result = TO_OutputChannel_Query(channelIdx);

    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");
    UtAssert_EventSent(TO_OUT_CH_INFO_ERR_EID, CFE_EVS_ERROR, "",
             "TO_ProcessNewAppCmds, Output_Channel_Query_Channel_Not_Open");
}


/**
 * Test TO_VerifyCmdLength(), MsgPtr_Null
 */
void Test_TO_VerifyCmdLength_MsgPtr_Null(void)
{
    osalbool result = TRUE;
    osalbool expected = FALSE;
    CFE_SB_Msg_t* Cmd_msgPtr = NULL;
    uint16 expectedLen = 42;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_HK_MID, 1);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, CFE_SB_SendMsg_TO_CmdRcvMsgHook);

    result = TO_VerifyCmdLength(Cmd_msgPtr, expectedLen);

    /* Verify results */
    UtAssert_True (result == expected, "TO_VerifyCmdLength(), MsgPtr_Null");
}


void TO_Cmds_Test_AddTestCases(void)
{
    /* Tests for TO_ProcessNewCmds() */
    UtTest_Add(Test_TO_ProcessNewAppCmds_Default,
              TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ProcessNewAppCmds_Default");
    UtTest_Add(Test_TO_ProcessNewAppCmds_MsgPtr_Null,
              TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ProcessNewAppCmds_MsgPtr_Null");
    UtTest_Add(Test_TO_ProcessNewCmds_RcvMsgErr,
              TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
              "Test_TO_ProcessNewCmds_RcvMsgErr");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Noop_InvalidSize,
               TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Noop_InvalidSize");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Noop_Nominal,
               TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Noop_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Reset_Nominal,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Reset_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Reset_InvalidCmdLength,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Reset_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidPQueueIdx,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidPQueueIdx");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_AlreadyDefined,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_AlreadyDefined");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidCmdLength,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Full,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Full");
    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_InvalidCmdLength,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_InvalidCmdLength,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Nominal,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_InvalidCmdLength,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_Nominal,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_InvalidCmdLength,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Success,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Success");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_InvalidCmdLength,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_InvalidCmdLength");

    /* Additional tests for TO_ProcessNewAppCmds */
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Fail_Channel_Not_Open,
               TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Fail_Channel_Not_Open");
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_That_Does_Not_Exist,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_That_Does_Not_Exist");
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_Channel_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_Channel_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_Channel_Not_Open,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_Channel_Not_Open");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_MsgFlow_Not_Defined,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_MsgFlow_Not_Defined");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Channel_Index_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Channel_Index_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_PQ_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_PQ_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_Fail_ChannelID_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_Fail_ChannelID_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Fail_ChannelID_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Fail_ChannelID_Invalid");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Fail_Channel_Not_Open,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Fail_Channel_Not_Open");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Not_Open,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Not_Open");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Fail_Channel_Not_Open,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Fail_Channel_Not_Open");
    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Output_Channel_Query_Channel_Not_Open,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Output_Channel_Query_Channel_Not_Open");

    UtTest_Add(Test_TO_VerifyCmdLength_MsgPtr_Null,
               TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_VerifyCmdLength_MsgPtr_Null");
}
