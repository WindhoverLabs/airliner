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

#include "to_app_test.h"
#include "to_app.h"
#include "to_test_utils.h"
#include "to_custom_hooks.h"
#include "ut_to_custom_hooks.h"

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
#include "ut_osapi_hooks.h"
#include "ut_cfe_es_hooks_ex.h"
#include "ut_to_custom_stubs.h"


extern TO_ChannelTbl_t TO_EmptyConfigTable;


/* Global variables for tests */
static int32 hookCalledCount = 0;
static CFE_SB_Msg_t* TO_test_sendMsg_msgPtr = NULL;
static int TO_test_CFE_SB_RcvMsg_successCount = 0;

/* Function declarations for tests */
void TO_Channel_Cleanup(int index);
void TO_Channel_Init(int index);

/* CFE_SB_SendMsg_TO_RcvMsgHook */
int32 CFE_SB_SendMsg_TO_RcvMsgHook(CFE_SB_Msg_t* MsgPtr) {
    TO_test_sendMsg_msgPtr = MsgPtr;
    if (TO_test_CFE_SB_RcvMsg_successCount > 0) {
        TO_test_CFE_SB_RcvMsg_successCount--;
        return CFE_SUCCESS;
    } else {
        return CFE_SB_NO_MESSAGE;
    }
}

/**************************************************************************
 * Tests for TO_InitEvent()
 **************************************************************************/

/**
 * Test TO_InitEvent() with failed CFE_EVS_Register
 */
void Test_TO_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}

void Test_TO_InitEvent_Fail_Custom_Bad_Ind(void)
{
    /* Set fail return for TO_Custom_InitEvent()*/
    Ut_TO_Custom_SetReturnCode(UT_TO_CUSTOM_INITEVENT_INDEX, TO_CUSTOM_INITEVENT_BAD_IND_ERR, 1);

    /* Execute the function being tested*/
    TO_InitEvent();

    UtAssert_True(Ut_CFE_ES_SysLogWritten("TO - Custom Init Event error: event table index out of range. Custom EID filters not set."),
                  "InitEvent, fail TO_Custom_InitEvent - Bad Index Error");

}

void Test_TO_InitEvent_Fail_Custom_Index_Occupied(void)
{
    /* Set fail return for TO_Custom_InitEvent()*/
    Ut_TO_Custom_SetReturnCode(UT_TO_CUSTOM_INITEVENT_INDEX, TO_CUSTOM_INITEVENT_INDEX_OCCUPIED, 1);

    /* Execute the function being tested*/
    TO_InitEvent();

    UtAssert_True(Ut_CFE_ES_SysLogWritten("TO - Custom Init Event error: event table occupied at starting index. Custom EID filters not set."),
                  "InitEvent, fail TO_Custom_InitEvent - Index Occupied Error");

}

void Test_TO_InitEvent_Fail_Custom_Too_Many(void)
{
    /* Set fail return for TO_Custom_InitEvent()*/
    Ut_TO_Custom_SetReturnCode(UT_TO_CUSTOM_INITEVENT_INDEX, TO_CUSTOM_INITEVENT_TOO_MANY_FILTERS, 1);

    /* Execute the function being tested*/
    TO_InitEvent();

    UtAssert_True(Ut_CFE_ES_SysLogWritten("TO - Custom Init Event error: Filter limit reached. Some custom EID filters not set."),
                  "InitEvent, fail TO_Custom_InitEvent - Too Many Filters Error");

}

void Test_TO_InitEvent_Fail_Custom_Unknown(void)
{
    /* Set fail return for TO_Custom_InitEvent()*/
    Ut_TO_Custom_SetReturnCode(UT_TO_CUSTOM_INITEVENT_INDEX, 42, 1);

    /* Execute the function being tested*/
    TO_InitEvent();

    UtAssert_True(Ut_CFE_ES_SysLogWritten("TO - Custom Init Event: Unknown Error."),
                  "InitEvent, fail TO_Custom_InitEvent - Unknown Error");

}



/**************************************************************************
 * Tests for TO_InitPipe()
 **************************************************************************/
/**
 * Test TO_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_TO_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test TO_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_TO_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test TO_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_TO_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = TO_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test TO_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_TO_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = TO_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test TO_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_TO_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**************************************************************************
 * Tests for TO_InitData()
 **************************************************************************/
/**
 * Test TO_InitData()
 */
void Test_TO_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = TO_InitData();

    /* Verify results */
    UtAssert_True (CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)&TO_AppData.HkTlm) == TO_HK_TLM_MID, "InitData success - TO_HK_TLM_MID subscribed");
    UtAssert_True (result == expected, "InitData success - returned success");
}

void Test_TO_InitData_FailAppMutSemCreate(void)
{
    int32 result = -777;
    int32 expectedReturn = OS_ERROR;

    char expectedEvent[100];
    sprintf(expectedEvent, "Init Data: Failed to create TO_AppData mutex: 0x%08lX", (unsigned long)expectedReturn);

    /* Set a fail result */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMCREATE_INDEX, OS_ERROR, 1);

    /* Execute the function being tested */
    result = TO_InitData();

    /* Verify results */
    UtAssert_EventSent(TO_CREATE_APPDATA_MUTEX_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "InitData Fail App SemCreate - error event correct");
    UtAssert_True (result == expectedReturn, "InitData Fail App SemCreate - returned failure");


}


/**************************************************************************
 * Tests for TO_InitApp()
 **************************************************************************/
/**
 * Test TO_InitApp(), fail init event
 */
void Test_TO_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test TO_InitApp(), fail init pipe
 */
void Test_TO_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test TO_InitApp(), fail init data.
 */
void Test_TO_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = OS_ERROR;

    /* Set a fail result for TO_InitData with and OS_MutSemCreate() error */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMCREATE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results, error code should propagate back to initApp*/
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test TO_InitApp(), fail TO_Channel_InitAll
 */
void Test_TO_InitApp_Fail_ChannelInitAll(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = OS_SEM_FAILURE;

    /* Fail OS_MutSemCreate in TO_Channel_Init with OS_SEM_FAILURE
     * i.e. the OS call failed. Needs to fail on the fifth call because
     * OS_MutSemCreate is called in TO_InitData() */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMCREATE_INDEX, OS_SEM_FAILURE, 1);

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail channel init all");
}


/**
 * Test TO_InitApp(), fail TO_Custom_Init()
 */
void Test_TO_InitApp_Fail_CustomInit(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = -1;

    /* Set TO_Custom_Init to fail */
    Ut_TO_Custom_SetReturnCode(UT_TO_CUSTOM_INIT_INDEX, expected, 1);

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail custom init all");
}


/**
 * Test TO_InitApp(), Nominal
 */
void Test_TO_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for TO_AppMain()
 **************************************************************************/
/**
 * Test TO_AppMain(), Fail RegisterApp
 */
void Test_TO_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    TO_AppMain();
}


/**
 * Test TO_AppMain(), Fail InitApp
 */
void Test_TO_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    TO_AppMain();
}


/**
 * Test TO_AppMain(), Invalid Schedule Message
 */
void Test_TO_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

}


/**
 * Hook to support: TO_AppMain(), Nominal - SendHK
 */
int32 Test_TO_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}


/**
 * Test TO_AppMain(), Nominal - SendHK
 */
void Test_TO_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Test_TO_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test TO_AppMain(), Nominal - Wakeup
 */
void Test_TO_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_TLM_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

}


/**
 * Test TO_AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_TO_AppMain_ProcessNewData_InvalidMsgID(void)
{
    TO_SendDiagCmd_t  InMsg;
    int32 CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(TO_SendDiagCmd_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InMsg, CmdPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_TLM_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_MSG_ID_ERR_EID, CFE_EVS_ERROR, "", "Error Event Sent");
}


/**
 * Test TO_AppMain(), Fail RcvMsg
 */
void Test_TO_AppMain_Fail_RcvMsg(void)
{
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();
    
    /* Verify results */
    UtAssert_EventSent(TO_SB_RECEIVE_ERR_EID, CFE_EVS_CRITICAL, "", 
                                          "Main loop error: SB receive");    
}

/**************************************************************************
 * Tests for TO_ProcessNewCmds()
 **************************************************************************/
void Test_TO_ProcessNewCmds_RcvMsgErr(void)
{
    int32 errCodeSet = CFE_SB_MSG_TOO_BIG; //Anything other than CFE_SUCCESS or CFE_SB_NO_MESSAGE

    char expectedEvent[100];
    sprintf(expectedEvent, "CMD pipe read error (0x%08X)", (unsigned int)errCodeSet);

    /* Set a fail result */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, errCodeSet, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);

    /* Execute the function being tested */
    TO_ProcessNewCmds();

    /* Verify results */
    UtAssert_EventSent(TO_PIPE_READ_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "ProcessNewCmds SB_RcvMsg error - error event correct");
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
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "NOOP Cmd Event Sent");
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
    UtAssert_EventSent(TO_CMD_NOOP_INF_EID, CFE_EVS_INFORMATION, "", "NOOP Cmd Event Sent");
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
    //TO_AppData.HkTlm.TotalMsgDropped = 3;

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_MESSAGE_FLOWS; ++j)
        {
            //TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].DroppedMsgCnt = 5;
            TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].QueuedMsgCnt = 6;
        }
    }

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_PRIORITY_QUEUES; ++j)
        {
            //TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].DroppedMsgCnt = 7;
            TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].QueuedMsgCnt = 8;
            TO_AppData.ChannelData[i].DumpTbl.PriorityQueue[j].HighwaterMark = 9;
        }
    }

    for(i=0; i < TO_MAX_CHANNELS; ++i)
    {
        TO_AppData.ChannelData[i].OutputQueue.SentCount = 10;
        TO_AppData.ChannelData[i].OutputQueue.HighwaterMark = 11;
        //TO_AppData.ChannelData[i].PeakMemInUse = 12;
    }

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_RESET_INF_EID, CFE_EVS_INFORMATION, "", "RESET Cmd Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 0, "TO_AppData.HkTlm.CmdErrCnt == 0");
    //UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");

    for(i = 0; i < TO_MAX_CHANNELS; ++i)
    {
        for(j = 0; j < TO_MAX_MESSAGE_FLOWS; ++j)
        {
            //UtAssert_True(TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].DroppedMsgCnt == 0,
            //        "TO_AppData.ChannelData[i].DumpTbl.MessageFlow[j].DroppedMsgCnt == 0");
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
        //UtAssert_True(TO_AppData.ChannelData[i].PeakMemInUse == 0,
        //        "TO_AppData.ChannelData[i].PeakMemInUse == 0");
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
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
}

/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Nominal
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
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                       CFE_EVS_ERROR, "", "Add Message Flow Cmd Event Sent");
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
            "Add Message Flow Cmd Event Sent");
}



/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Nominal
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal(void)
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
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_EID, CFE_EVS_INFORMATION, "", "Add Message Flow Cmd Event Sent");
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
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, TO_ADD_MESSAGE_FLOW_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventNotSent(TO_CMD_ADD_MSG_FLOW_EID, CFE_EVS_INFORMATION, "", "ADD MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
}

///**
// * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Full
// */
//void Test_TO_ProcessNewAppCmds_AddMessageFlow_Full(void)
//{
//    TO_NoArgCmd_t           InSchMsg;
//    TO_AddMessageFlowCmd_t  InCmd;
//    int32                   DataPipe;
//    int32                   CmdPipe;
//    int32                   i = 0;
//    uint8                   channelIdx = 0;
//
//    //CFE_SB_MsgId_t  MsgId = 0x0800;
//    uint16          MsgLimit = 1;
//    uint16          PQueueIdx = 0;
//
//    extern TO_ChannelTbl_t TO_FullConfigTable6;
//
//    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
//       and gives it a command to process. */
//    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
//    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
//    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);
//
//    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
//
//    CFE_SB_InitMsg(&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
//    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
//    InCmd.MsgID = i+1;
//    InCmd.MsgLimit = MsgLimit;
//    InCmd.PQueueIdx = PQueueIdx;
//    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);
//
//    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
//    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
//    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
//
//    /* Set function hook for TO_Custom_Init */
//    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
//
//    UtAssert_True(TO_FullConfigTable6.MessageFlow[0].MsgId == 0x0800, "The 0th entry in the configuration table is not empty.");
//
//    UtAssert_True(TO_FullConfigTable6.MessageFlow[158].MsgId == 0x08D5, "The 158th entry in the configuration table is not empty.");
//
//    UtAssert_True(TO_FullConfigTable6.MessageFlow[159].MsgId == 0, "The 159th entry in the configuration table is empty.");
//
//    /* Execute the function being tested */
//    TO_AppMain();
//
//    /* Verify results */
//    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_EID, CFE_EVS_INFORMATION, "", "Executed ADD_MESSAGE cmd");
//
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId == 0x0800,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[142].MsgId == 0x08C9,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[142].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[143].MsgId == 0x08B0,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[143].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[158].MsgId == 0x08D5,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[158].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[159].MsgId == 1,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[159].MsgId");
//}

//
///**
// * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Over
// */
//void Test_TO_ProcessNewAppCmds_AddMessageFlow_Over(void)
//{
//    TO_NoArgCmd_t           InSchMsg;
//    TO_AddMessageFlowCmd_t  InCmd;
//    int32                   DataPipe;
//    int32                   CmdPipe;
//    int32                   i = 0;
//    uint8                   channelIdx = 0;
//
//    uint16          MsgLimit = 1;
//    uint16          PQueueIdx = 0;
//
//    extern TO_ChannelTbl_t TO_FullConfigTable7;
//
//    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
//       and gives it a command to process. */
//    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
//    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
//    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);
//
//    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
//
//    CFE_SB_InitMsg(&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
//    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
//    InCmd.MsgID = i+1;
//    InCmd.MsgLimit = MsgLimit;
//    InCmd.PQueueIdx = PQueueIdx;
//    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);
//
//    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
//    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
//    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
//
//    /* Set function hook for TO_Custom_Init */
//    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
//
//    UtAssert_True(TO_FullConfigTable7.MessageFlow[159].MsgId == 0x08D6, "The 159th entry in the configuration table is not empty.");
//
//    /* Execute the function being tested */
//    TO_AppMain();
//
//    /* Verify results */
//    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "", "Failed to add message flow. No unused slots available on channel");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId == 0x0800,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[0].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[143].MsgId == 0x08B0,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[143].MsgId");
//    UtAssert_True(TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[159].MsgId == 0x08D6,
//            "Add Message Flow Cmd set TO_AppData.ChannelData[0].ConfigTblPtr->MessageFlow[159].MsgId");
//}


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
    UtAssert_EventNotSent(TO_MSG_FLOW_INFO_EID, CFE_EVS_INFORMATION, "", "REMOVE MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
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

    char expEventText[100];

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

                        
    UtAssert_EventSent(TO_MSG_FLOW_INFO_EID, 
                       CFE_EVS_INFORMATION, "", "Query Message Flow Cmd Event Sent");
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
    UtAssert_EventNotSent(TO_MSG_FLOW_INFO_EID, CFE_EVS_INFORMATION, "", "QUERY MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
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
    Ut_CFE_SB_AddMsgToPipe(&InCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    sprintf(expEventText, "CHANNEL=%d PQI=%u S=%u D=%u QT=%u",
    		0,
            PQueueIdx,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].State,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].Depth,
            TO_AppData.ChannelData[0].ConfigTblPtr->PriorityQueue[PQueueIdx].QType);
            
    UtAssert_EventSent(TO_PQUEUE_INFO_EID, 
                       CFE_EVS_INFORMATION, "", "Query Priority Queue Cmd Event Sent");
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
    UtAssert_EventNotSent(TO_PQUEUE_INFO_EID, CFE_EVS_INFORMATION, "", "REMOVE MSG FLOW Cmd Not Event Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
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

    /* Verify results */
    sprintf(expEventText, "S=%u ML=%u SC=%lu CQC=%u HWM=%u",
            TO_AppData.ChannelData[0].State,
            TO_OUTPUT_QUEUE_DEPTH,
            TO_AppData.ChannelData[0].OutputQueue.SentCount,
            TO_AppData.ChannelData[0].OutputQueue.CurrentlyQueuedCnt,
            TO_AppData.ChannelData[0].OutputQueue.HighwaterMark);

    UtAssert_EventSent(TO_OUT_CH_INFO_EID, 
                       CFE_EVS_INFORMATION, "", "Query Output Queue Cmd Event Sent");
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
    UtAssert_EventNotSent(TO_OUT_CH_INFO_EID,
                       CFE_EVS_INFORMATION, "", "Query Output Queue Cmd Event Not Sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
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
    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 1, "Send Diag Success - Command Accept Count bumped");
    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_TLM_MID), "Send Diag Success - Channel Diag Packet sent");

    int32 numMsgFlowPackets = (int32)ceil((double)TO_MAX_MESSAGE_FLOWS/TO_MSG_FLOW_PKT_LIMIT);
    int32 msgFlowPacketCount = 0;

    for(i=0; i<numMsgFlowPackets; ++i)
    {
        if(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID))
        {
            msgFlowPacketCount++;
        }
    }

    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID), "Send Diag Success - Channel MsgFlow Diag Packets sent");
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
    UtAssert_True(Ut_CFE_SB_PacketSent(TO_DIAG_MSG_FLOW_MID) == FALSE, "Send Diag Reject - Channel MsgFlow Diag Packets Not sent");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "", "Message Length Error Event Sent");

    UtAssert_True(TO_AppData.HkTlm.CmdCnt == 0, "TO_AppData.HkTlm.CmdCnt == 0");
    UtAssert_True(TO_AppData.HkTlm.CmdErrCnt == 1, "TO_AppData.HkTlm.CmdErrCnt == 1");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == 0, "TO_AppData.HkTlm.TotalMsgDropped == 0");
}

/**************************************************************************
 * Tests for Traffic Shaping()
 **************************************************************************/
/**
 * Test TO_ProcessNewAppCmds(), QueryChannelQueue command, Nominal
 */
void Test_TO_AppMain_ProcessTelemetry_PriorityPreemption1(void)
{
    TO_NoArgCmd_t InSchMsg;
    int32         SchPipe;
    int32         DataPipe;
    TO_HkTlm_t    msgCfeEsHk;
    TO_HkTlm_t    msgCfeEvsHk;
    TO_HkTlm_t    msgCfeSbHk;
    TO_HkTlm_t    msgCfeTblHk;
    TO_HkTlm_t    msgCfeTimeHk;
    TO_HkTlm_t    msgCfeTimeDiag;
    TO_HkTlm_t    msgCfeEvsEvent;
    TO_HkTlm_t    msgCfeSbStats;
    TO_HkTlm_t    msgCfeEsApp;
    TO_HkTlm_t    msgCfeTblReg;
    TO_HkTlm_t    msgCfeSbOneSub;
    TO_HkTlm_t    msgCfeEsShell;
    TO_HkTlm_t    msgCfeEsMemStats;
    TO_HkTlm_t    msgCfeHk;
    TO_HkTlm_t    msgCfTrans;
    TO_HkTlm_t    msgCfConfig;
    TO_HkTlm_t    msgCfSpaceToGndPdu;
    TO_HkTlm_t    msgCsHk;
    uint32        chQueue0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and processing a full pipe of telemetry messages. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("TO_UDP");

    /* Initialize a bunch of telemetry messages for downlink. */
    CFE_SB_InitMsg (&msgCfeEsHk, CFE_ES_HK_TLM_MID, sizeof(msgCfeEsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsHk, CFE_EVS_HK_TLM_MID, sizeof(msgCfeEvsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeSbHk, CFE_SB_HK_TLM_MID, sizeof(msgCfeSbHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTblHk, CFE_TBL_HK_TLM_MID, sizeof(msgCfeTblHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeHk, CFE_TIME_HK_TLM_MID, sizeof(msgCfeTimeHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeDiag, CFE_TIME_DIAG_TLM_MID, sizeof(msgCfeTimeDiag), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsEvent, CFE_EVS_EVENT_MSG_MID, sizeof(msgCfeEvsEvent), TRUE);
    CFE_SB_InitMsg (&msgCfeSbStats, CFE_SB_STATS_TLM_MID, sizeof(msgCfeSbStats), TRUE);
    CFE_SB_InitMsg (&msgCfeEsApp, CFE_ES_APP_TLM_MID, sizeof(msgCfeEsApp), TRUE);

    /* Now load up the software bus with all the messages starting with low
     * priority first. */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsEvent, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbStats, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsApp, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTblHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeDiag, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbHk, DataPipe);

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEPUT_INDEX, &Ut_OSAPI_QueuePutHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEGET_INDEX, &Ut_OSAPI_QueueGetHook);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &Ut_CFE_ES_PutPoolBuf);

    /* Execute the function being tested.  We can't just run TO_AppMain because
     * right before it terminates, it clears the output queue.  We have to call
     * TO_InitApp() and then manually step through the main execution loop by
     * calling TO_RcvMsg()*/
    TO_InitApp();
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    /* Verify results */
    Ut_OSAPI_QueueGetIdByName(&chQueue0, "TO_UDP_OUT");
    CFE_SB_MsgPtr_t   msgPtr = 0;
    uint32 sizeCopied = 0;
    int32 iStatus = 0;

    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_ES_HK_TLM_MID, "1-1: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_HK_TLM_MID, "1-2: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TBL_HK_TLM_MID, "1-3: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_HK_TLM_MID, "1-4: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_DIAG_TLM_MID, "1-5: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_EVENT_MSG_MID, "1-6: Low Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_SB_STATS_TLM_MID, "1-7: Low Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_ES_APP_TLM_MID, "1-8: Low Priority");
    iStatus = Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(iStatus == OS_QUEUE_EMPTY, "1-9: EMPTY");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryChannelQueue command, Nominal
 */
void Test_TO_AppMain_ProcessTelemetry_PriorityPreemption2(void)
{
    TO_NoArgCmd_t InSchMsg;
    int32         SchPipe;
    int32         DataPipe;
    TO_HkTlm_t    msgCfeEsHk;
    TO_HkTlm_t    msgCfeEvsHk;
    TO_HkTlm_t    msgCfeSbHk;
    TO_HkTlm_t    msgCfeTblHk;
    TO_HkTlm_t    msgCfeTimeHk;
    TO_HkTlm_t    msgCfeTimeDiag;
    TO_HkTlm_t    msgCfeEvsEvent;
    TO_HkTlm_t    msgCfeSbStats;
    TO_HkTlm_t    msgCfeEsApp;
    TO_HkTlm_t    msgCfeTblReg;
    TO_HkTlm_t    msgCfeSbOneSub;
    TO_HkTlm_t    msgCfeEsShell;
    TO_HkTlm_t    msgCfeEsMemStats;
    TO_HkTlm_t    msgCfeHk;
    TO_HkTlm_t    msgCfTrans;
    TO_HkTlm_t    msgCfConfig;
    TO_HkTlm_t    msgCfSpaceToGndPdu;
    TO_HkTlm_t    msgCsHk;
    uint32        chQueue0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and processing a full pipe of telemetry messages. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("TO_UDP");

    /* Initialize a bunch of telemetry messages for downlink. */
    CFE_SB_InitMsg (&msgCfeEsHk, CFE_ES_HK_TLM_MID, sizeof(msgCfeEsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsHk, CFE_EVS_HK_TLM_MID, sizeof(msgCfeEvsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeSbHk, CFE_SB_HK_TLM_MID, sizeof(msgCfeSbHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTblHk, CFE_TBL_HK_TLM_MID, sizeof(msgCfeTblHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeHk, CFE_TIME_HK_TLM_MID, sizeof(msgCfeTimeHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeDiag, CFE_TIME_DIAG_TLM_MID, sizeof(msgCfeTimeDiag), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsEvent, CFE_EVS_EVENT_MSG_MID, sizeof(msgCfeEvsEvent), TRUE);
    CFE_SB_InitMsg (&msgCfeSbStats, CFE_SB_STATS_TLM_MID, sizeof(msgCfeSbStats), TRUE);
    CFE_SB_InitMsg (&msgCfeEsApp, CFE_ES_APP_TLM_MID, sizeof(msgCfeEsApp), TRUE);

    /* Now load up the software bus with all the messages starting with low
     * priority first. */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsEvent, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbStats, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsApp, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTblHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeDiag, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbHk, DataPipe);

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEPUT_INDEX, &Ut_OSAPI_QueuePutHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEGET_INDEX, &Ut_OSAPI_QueueGetHook);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &Ut_CFE_ES_PutPoolBuf);

    /* Execute the function being tested.  We can't just run TO_AppMain because
     * right before it terminates, it clears the output queue.  We have to call
     * TO_InitApp() and then manually step through the main execution loop by
     * calling TO_RcvMsg()*/
    TO_InitApp();
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    /* Verify results */
    Ut_OSAPI_QueueGetIdByName(&chQueue0, "TO_UDP_OUT");
    CFE_SB_MsgPtr_t   msgPtr = 0;
    uint32 sizeCopied = 0;
    int32 iStatus = 0;

    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_ES_HK_TLM_MID, "1-1: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_HK_TLM_MID, "1-2: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_SB_HK_TLM_MID, "1-3: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TBL_HK_TLM_MID, "1-4: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_HK_TLM_MID, "1-5: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_DIAG_TLM_MID, "1-6: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_EVENT_MSG_MID, "1-7: Low Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_SB_STATS_TLM_MID, "1-8: Low Priority");
    iStatus = Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(iStatus == OS_QUEUE_EMPTY, "1-9: EMPTY");
}


/**
 * Test TO_ProcessNewAppCmds(), QueryChannelQueue command, Nominal
 * Mixed up message input
 */
void Test_TO_AppMain_ProcessTelemetry_PriorityPreemption3(void)
{
    TO_NoArgCmd_t InSchMsg;
    int32         SchPipe;
    int32         DataPipe;
    TO_HkTlm_t    msgCfeEsHk;
    TO_HkTlm_t    msgCfeEvsHk;
    TO_HkTlm_t    msgCfeSbHk;
    TO_HkTlm_t    msgCfeTblHk;
    TO_HkTlm_t    msgCfeTimeHk;
    TO_HkTlm_t    msgCfeTimeDiag;
    TO_HkTlm_t    msgCfeEvsEvent;
    TO_HkTlm_t    msgCfeSbStats;
    TO_HkTlm_t    msgCfeEsApp;
    TO_HkTlm_t    msgCfeTblReg;
    TO_HkTlm_t    msgCfeSbOneSub;
    TO_HkTlm_t    msgCfeEsShell;
    TO_HkTlm_t    msgCfeEsMemStats;
    TO_HkTlm_t    msgCfeHk;
    TO_HkTlm_t    msgCfTrans;
    TO_HkTlm_t    msgCfConfig;
    TO_HkTlm_t    msgCfSpaceToGndPdu;
    TO_HkTlm_t    msgCsHk;
    uint32        chQueue0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and processing a full pipe of telemetry messages. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("TO_UDP");

    /* Initialize a bunch of telemetry messages for downlink. */
    CFE_SB_InitMsg (&msgCfeEsHk, CFE_ES_HK_TLM_MID, sizeof(msgCfeEsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsHk, CFE_EVS_HK_TLM_MID, sizeof(msgCfeEvsHk), TRUE);
    CFE_SB_InitMsg (&msgCfeSbHk, CFE_SB_HK_TLM_MID, sizeof(msgCfeSbHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTblHk, CFE_TBL_HK_TLM_MID, sizeof(msgCfeTblHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeHk, CFE_TIME_HK_TLM_MID, sizeof(msgCfeTimeHk), TRUE);
    CFE_SB_InitMsg (&msgCfeTimeDiag, CFE_TIME_DIAG_TLM_MID, sizeof(msgCfeTimeDiag), TRUE);
    CFE_SB_InitMsg (&msgCfeEvsEvent, CFE_EVS_EVENT_MSG_MID, sizeof(msgCfeEvsEvent), TRUE);
    CFE_SB_InitMsg (&msgCfeSbStats, CFE_SB_STATS_TLM_MID, sizeof(msgCfeSbStats), TRUE);
    CFE_SB_InitMsg (&msgCfeEsApp, CFE_ES_APP_TLM_MID, sizeof(msgCfeEsApp), TRUE);

    /* Now load up the software bus with all the messages in random 
     * order. */

    /* High priority */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbHk, DataPipe);
    /* Low priority */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsEvent, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeSbStats, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEsApp, DataPipe);
    /* Medium priority */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTblHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeHk, DataPipe);
    Ut_CFE_SB_AddMsgToPipe(&msgCfeTimeDiag, DataPipe);

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEPUT_INDEX, &Ut_OSAPI_QueuePutHook);
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUEGET_INDEX, &Ut_OSAPI_QueueGetHook);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX, &Ut_CFE_ES_PutPoolBuf);

    /* Execute the function being tested.  We can't just run TO_AppMain because
     * right before it terminates, it clears the output queue.  We have to call
     * TO_InitApp() and then manually step through the main execution loop by
     * calling TO_RcvMsg()*/
    TO_InitApp();
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);
    TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    /* Verify results */
    Ut_OSAPI_QueueGetIdByName(&chQueue0, "TO_UDP_OUT");
    CFE_SB_MsgPtr_t   msgPtr = 0;
    uint32 sizeCopied = 0;
    int32 iStatus = 0;

    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_ES_HK_TLM_MID, "1-1: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_HK_TLM_MID, "1-2: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_SB_HK_TLM_MID, "1-3: High Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TBL_HK_TLM_MID, "1-4: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_HK_TLM_MID, "1-5: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_TIME_DIAG_TLM_MID, "1-6: Medium Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_EVS_EVENT_MSG_MID, "1-7: Low Priority");
    Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(Ut_CFE_SB_GetMsgIdHook(msgPtr) == CFE_SB_STATS_TLM_MID, "1-8: Low Priority");
    iStatus = Ut_OSAPI_QueueGetHook(chQueue0, &msgPtr, sizeof(msgPtr), &sizeCopied, OS_CHECK);
    UtAssert_True(iStatus == OS_QUEUE_EMPTY, "1-9: EMPTY");
}

/**************************************************************************
 * Tests for NULL Pointer checks
 **************************************************************************/
/**
 * Test Test_TO_NULL_Ptr_check()
 */
void Test_TO_NULL_Ptr_check(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryOutputChannelCmd_t  InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

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

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    iStatus = TO_MessageFlow_Buildup(channel);
    UtAssert_True(iStatus == -3, "TO_MESSAGE_FLOW_NO_TABLE_ERR");
}

/**
 * Test Test_TO_NULL_Ptr_check_PQ_buildup()
 */
void Test_TO_NULL_Ptr_check_PQ_buildup(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryOutputChannelCmd_t  InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

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

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    iStatus = TO_PriorityQueue_BuildupAll(channel);
    UtAssert_True(iStatus == -4, "TO_NO_TABLE_ERR");

}

/**
 * Test Test_TO_NULL_Ptr_check()
 */
void Test_TO_NULL_Ptr_check_PQ_buildup_channel_ptr_null(void)
{
    TO_NoArgCmd_t               InSchMsg;
    TO_QueryOutputChannelCmd_t  InCmd;
    int32                       DataPipe;
    int32                       CmdPipe;
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    /* Get channel data information */
    channel = NULL;

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

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    iStatus = TO_PriorityQueue_BuildupAll(channel);
    UtAssert_True(iStatus == -7, "TO_CHANNEL_PTR_NULL");
}

void Test_TO_NULL_Ptr_check1(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel = NULL;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    iStatus = TO_MessageFlow_Buildup(channel);
    UtAssert_True(iStatus == -2, "TO_MESSAGE_FLOW_BAD_ARG_ERR");

}

void Test_TO_NULL_Ptr_check2(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel = NULL;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    iStatus = TO_MessageFlow_TeardownAll(channel);
    UtAssert_True(iStatus == -2, "TO_MESSAGE_FLOW_BAD_ARG_ERR");

}

/**
 * Test Test_TO_NULL_Ptr_check_Scheduler()
 */
void Test_TO_NULL_Ptr_check_Scheduler(void)
{
    uint16  ChannelIdx = 0;
    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    /* channel->ConfigTblPtr should be NULL.  Code just returns though */
    TO_Scheduler_Run(channel);
}


/**************************************************************************
 * Tests for TO Output Queue
 **************************************************************************/
void Test_TO_Output_Queue_Buildup_QueueCreate_Fail(void)
{
//    uint16  ChannelIdx = 0;
//    int32 iStatus = 0;
//    TO_ChannelData_t* channel;
//
//    char expectedEvent[100];
//
//    /* Set return codes */
//    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
//
//    /* Set function hook for TO_Custom_Init */
//    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
//
//    /* Set function hooks */
//    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);
//
//    TO_InitApp();
//
//    /* Get channel data information */
//    channel = &TO_AppData.ChannelData[ChannelIdx];
//
//    /* Execute the function being tested */
//    iStatus = TO_OutputQueue_Buildup(channel);
//
//    sprintf(expectedEvent, "Failed to create '%s' output channel queue for channel %d. err=%ld",
//            channel->ChannelName,
//            channel->channelIdx,
//            iStatus);
//
//    /* Verify results */
//    UtAssert_EventSent(TO_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
//                       expectedEvent, "Output Queue Buildup OS_QueueCreate Failed - event correct");
}

void Test_TO_OutputQueue_QueueMsg_QueuePut(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    CFE_SB_MsgPtr_t   msgPtr = 0;
       
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_ERROR, 1);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
         
    /* Execute the function being tested */         
    iStatus = TO_OutputQueue_QueueMsg(channel, msgPtr);
    
    sprintf(expectedEvent, "OS_QueuePut failed: channel=%d size=%u error=%i",
            channel->channelIdx,
             4,
            iStatus);       

    /* Verify results */
    UtAssert_EventSent(TO_OSQUEUE_PUT_ERROR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Output Queue Message QueuePut Failed - event correct");                 
}

void Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Fail(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    CFE_SB_MsgPtr_t   msgPtr = 0;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_QUEUE_FULL, 1);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    /* Execute the function being tested */    
    iStatus = TO_OutputQueue_QueueMsg(channel, msgPtr);
    
    sprintf(expectedEvent, "PutPoolBuf: channel=%d, error=%d",
            channel->channelIdx,
            iStatus);       

    /* Verify results */
    UtAssert_EventSent(TO_PUT_POOL_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Output Queue Message QueuePut Queue Full Put Pool Buffer fail - event correct");                 
}

void Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Success(void)
{
    uint16  ChannelIdx = 0;
    int32 result = 0;
    int32 expected = 16;
    TO_ChannelData_t* channel;
    CFE_SB_MsgPtr_t   msgPtr = 0;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, 16, 1);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_QUEUE_FULL, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
          
    /* Execute the function being tested */          
    result = TO_OutputQueue_QueueMsg(channel, msgPtr);
    
    /* Verify results */
    UtAssert_True (result == expected, "Memory In Use as expected.");               
}

void Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Fail(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    channel->OutputQueue.OSALQueueID = 42;

    /* Execute the function being tested */
    iStatus = TO_OutputQueue_Teardown(channel);
    
    sprintf(expectedEvent, "Failed to return message back to memory pool on output queue teardown, channel %d. (%ld)",
            channel->channelIdx,
            -1);       

    /* Verify results */
    UtAssert_EventSent(TO_PUT_POOL_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Output Queue TeardownAll PutPoolBuf Failed - event correct");                 
}

void Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Success(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, 16, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    channel->OutputQueue.OSALQueueID = 42;

    /* Execute the function being tested */
    iStatus = TO_OutputQueue_Teardown(channel);
    
    sprintf(expectedEvent, "Failed to pop all messages from channel '%s' (channel id = %d) output queue. (%ld)",
                                     channel->ChannelName, channel->channelIdx, OS_ERROR); 

    /* Verify results */
    UtAssert_EventSent(TO_OSQUEUE_GET_ERROR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Failed to pop all messages from channel - event correct");                                       
}


void Test_TO_OutputQueue_Teardown_QueueGet_Queue_Empty(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, 16, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 3);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    channel->OutputQueue.OSALQueueID = 42;

    /* Execute the function being tested */
    iStatus = TO_OutputQueue_Teardown(channel);                                    
}


/**************************************************************************
 * Tests for TO Message Flow
 **************************************************************************/
void Test_TO_MessageFlow_TeardownAll_SubscribeEx(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, CFE_SB_BAD_ARGUMENT, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_SUBSCRIBEEX_INDEX);      
    
    iStatus = TO_MessageFlow_Buildup(channel);
    
    sprintf(expectedEvent, "Message flow failed to subscribe to (0x%08X) on channel %d. (%ld)",
        MsgId,
        ChannelIdx,
        CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_EventSent(TO_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Message flow failed to subscribe - event correct");
}
void Test_TO_MessageFlow_TeardownAll_Unsubscribe(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    char expectedEvent[100];    
    
    TO_ChannelData_t* channel;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, -1, 1);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    

    /* Execute the function being tested */
    iStatus = TO_MessageFlow_TeardownAll(channel);
    
    sprintf(expectedEvent, "Message flow failed to unsubscribe from 0x%04x on channel %d. (%ld)",
        CFE_ES_HK_TLM_MID,
        ChannelIdx,
        -1);

    /* Verify results */
    UtAssert_EventSent(TO_UNSUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Message flow failed to unsubscribe - event correct");  
}

void Test_TO_MessageFlow_GetObject_ConfigTblPtr_NULL(void)
{

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow;
    uint32 msgFlowIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t test = {CFE_ES_HK_TLM_MID,  1,  1};
    
    msgFlow = &test;
    
    TO_InitApp();          

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);     

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (msgFlow != NULL, "MessageFlow object is not NULL.");
    
    /* Execute the function being tested */    
    msgFlow = TO_MessageFlow_GetObject(channel, MsgId, &msgFlowIndex);
    
    /* Verify results */
    UtAssert_True (msgFlow == NULL, "MessageFlow object is NULL.");
}

void Test_TO_MessageFlow_GetObject_Channel_Ptr_NULL(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow;
    uint32 msgFlowIndex = 0;
    TO_ChannelData_t* channel = NULL;
    
    TO_MessageFlow_t test = {CFE_ES_HK_TLM_MID,  1, 1};
    msgFlow = &test;    
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (msgFlow != NULL, "MessageFlow object is not NULL.");    
    
    /* Execute the function being tested */    
    msgFlow = TO_MessageFlow_GetObject(channel, MsgId, &msgFlowIndex);
    
    /* Verify results */
    UtAssert_True (msgFlow == NULL, "MessageFlow object is NULL.");
}

void Test_TO_MessageFlow_GetPQueue_MsgFlow_NULL(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_PriorityQueue_t test = {TO_PQUEUE_ENA, TO_PRIORITY_QUEUE_TYPE_FIFO, 10};
    
    pqueue = &test;
    
    TO_InitApp();        

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];          
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);     

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL.");
    
    /* Execute the function being tested */    
    pqueue = TO_MessageFlow_GetPQueue(channel, msgFlow, &pQueueIndex);
    
    /* Verify results */
    UtAssert_True (pqueue == NULL, "Priority Queue object is not NULL.");    
}

void Test_TO_MessageFlow_GetPQueue_ConfigTblPtr_NULL(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};

    TO_PriorityQueue_t test = {TO_PQUEUE_ENA, TO_PRIORITY_QUEUE_TYPE_FIFO, 10};
    
    msgFlow = &msgFlowTest; 
    pqueue = &test; 
    
    TO_InitApp();    

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    /* Set the ConfigTblPtr to NULL to test if condition */
    channel->ConfigTblPtr = NULL;          
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);      

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (channel != NULL, "Channel pointer is not NULL.");
    
    UtAssert_True (msgFlow != NULL, "Message Flow object is not NULL.");
    
    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL.");
    
    /* Execute the function being tested */    
    pqueue = TO_MessageFlow_GetPQueue(channel, msgFlow, &pQueueIndex);
    
    /* Verify results */
    UtAssert_True (pqueue == NULL, "Priority Queue object is not NULL.");    
}

void Test_TO_MessageFlow_GetPQueue_ChannelPtr_NULL(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};

    TO_PriorityQueue_t test = {TO_PQUEUE_ENA, TO_PRIORITY_QUEUE_TYPE_FIFO, 10};
    
    msgFlow = &msgFlowTest; 
    pqueue = &test;
    
    TO_InitApp();    

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel = NULL;           
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);      

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (msgFlow != NULL, "Message Flow object is not NULL.");
    
    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL."); 
    
    /* Execute the function being tested */    
    pqueue = TO_MessageFlow_GetPQueue(channel, msgFlow, &pQueueIndex);
    
    /* Verify results */
    UtAssert_True (pqueue == NULL, "Priority Queue object is not NULL.");    
}

void Test_TO_MessageFlow_GetPQueue_Idx_Max_Priority_Queues(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = TO_MAX_PRIORITY_QUEUES;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};

    TO_PriorityQueue_t test = {TO_PQUEUE_ENA, TO_PRIORITY_QUEUE_TYPE_FIFO, 10};
    
    msgFlow = &msgFlowTest; 
    pqueue = &test;
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    UtAssert_True (msgFlow != NULL, "Message Flow object is not NULL.");
    
    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL.");
    
    TO_InitApp();    

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    msgFlow->PQueueID = PQueueIdx;
    
    pQueueIndex = msgFlow->PQueueID;
    
    /* Execute the function being tested */    
    pqueue = TO_MessageFlow_GetPQueue(channel, msgFlow, &pQueueIndex);
    
    /* Verify results */
    UtAssert_True (pqueue == NULL, "Priority Queue object is not NULL.");    
}

void Test_TO_MessageFlow_GetPQueue_Idx_Null(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = TO_MAX_PRIORITY_QUEUES-1;
    uint16          ChannelIdx = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    TO_PriorityQueue_t *pqueue;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};

    TO_PriorityQueue_t test = {TO_PQUEUE_ENA, TO_PRIORITY_QUEUE_TYPE_FIFO, 10};

    msgFlow = &msgFlowTest;
    pqueue = &test;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    UtAssert_True (msgFlow != NULL, "Message Flow object is not NULL.");

    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL.");

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    msgFlow->PQueueID = PQueueIdx;

    /* Execute the function being tested */
    pqueue = TO_MessageFlow_GetPQueue(channel, msgFlow, NULL);

    /* Verify results */
    UtAssert_True (pqueue != NULL, "Priority Queue object is not NULL.");
}

/**
 * Test Test_TO_MessageFlow_Reset_All_Counts_Channel_NULL(), NULL pointer check
 */
void Test_TO_MessageFlow_Reset_All_Counts_Channel_NULL(void)
{
    TO_NoArgCmd_t InSchMsg;
    
    TO_ChannelData_t* channel = NULL;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_MessageFlow_ResetCountsAll(channel);

    /* Verify results */
    //UtAssert_True (result == expected, "TO_PriorityQueue_IsValid, NULL Pointer Check");
}


/**************************************************************************
 * Tests for TO Priority Queue
 **************************************************************************/
void Test_TO_Priority_Queue_Buildup_QueueCreate_Fail(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);

    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    

    /* Execute the function being tested */            
    iStatus = TO_PriorityQueue_BuildupAll(channel);
    
    sprintf(expectedEvent, "Failed to create '%s' priority queue #%u on channel %d. (%d)",
            channel->ChannelName,
            0,
            channel->channelIdx,
            -15);       

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_CREATE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue Buildup OS_QueueCreate Failed - event correct");                 
}

void Test_TO_Priority_Queue_Buildup_NoPQueues(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    /* Execute the function being tested */
    iStatus = TO_PriorityQueue_BuildupAll(channel);

    sprintf(expectedEvent, "Failed to create '%s' priority queue #%u on channel %d. (%d)",
            channel->ChannelName,
            0,
            channel->channelIdx,
            -15);

    /* Verify results */
    UtAssert_EventNotSent(TO_PQUEUE_CREATE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Priority Queue Buildup OS_QueueCreate Failed - event not sent");
}

void Test_TO_Priority_Queue_Teardown_QueueGet_PutPoolBuf_Fail(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 0);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 4);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);   

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
     
    /* Execute the function being tested */            
    iStatus = TO_PriorityQueue_TeardownAll(channel);
    
    sprintf(expectedEvent, "Failed to return message back to memory pool on tbl load for channel %d. (%ld)",
            channel->channelIdx,
            -1);       

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_TEARDOWN_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue TeardownAll PutPoolBuf Failed - event correct");                 
}

void Test_TO_Priority_Queue_Teardown_QueueDelete_Fail(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEDELETE_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 0);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 4);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEDELETE_INDEX);   

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
            
    /* Execute the function being tested */            
    iStatus = TO_PriorityQueue_TeardownAll(channel);
    
    sprintf(expectedEvent, "Failed to delete priority queue %u for channel %d. (%ld)",
            1,
            channel->channelIdx,
            iStatus);       

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_TEARDOWN_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue TeardownAll PutPoolBuf Failed - event correct");                 
}

void Test_TO_Priority_Queue_Teardown_OSALQueueIDInvalid(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;

    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEDELETE_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 0);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 4);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEDELETE_INDEX);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    /* Execute the function being tested */
    iStatus = TO_PriorityQueue_TeardownAll(channel);

    sprintf(expectedEvent, "Failed to delete priority queue %u for channel %d. (%ld)",
            1,
            channel->channelIdx,
            iStatus);

    /* Verify results */
    UtAssert_EventSent(TO_PQUEUE_TEARDOWN_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Priority Queue TeardownAll PutPoolBuf Failed - event correct");
}

void Test_TO_PriorityQueue_QueueMsg_ConfigTblPtr_NULL(void)
{
    int32  result = CFE_SUCCESS;

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    CFE_SB_MsgPtr_t msgPtr = 0;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    /* Execute the function being tested */    
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);
    
    /* Verify results */
    UtAssert_True(result == -4, "TO_NO_TABLE_ERR");
    UtAssert_EventSent(TO_PQUEUE_MISSING_TBL_ERR_EID, 
                       CFE_EVS_ERROR, "", "Failed to queue message on channel, missing table!");                          
}

void Test_TO_PriorityQueue_QueueMsg_Nominal(void)
{
    int32  result = CFE_SUCCESS;

    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    CFE_EVS_Packet_t  packet;
    CFE_SB_MsgPtr_t  msgPtr = (CFE_SB_MsgPtr_t) &packet;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 0;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    CFE_SB_InitMsg (&packet, CFE_EVS_EVENT_MSG_MID, sizeof(packet), TRUE);

    /* Execute the function being tested */
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);

    /* Verify results */
    UtAssert_True(result == OS_SUCCESS, "Queue message successful");

    /* Do it again for branch coverage purposes. */
//    channel->MemInUse = 0;
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);

    /* Verify results */
    UtAssert_True(result == OS_SUCCESS, "Queue message successful");
}

void Test_TO_PriorityQueue_QueueMsg_Memory_Full_Error(void)
{
    int32  result = CFE_SUCCESS;
    CFE_EVS_Packet_t  packet;

    CFE_SB_MsgPtr_t  msgPtr = (CFE_SB_MsgPtr_t) &packet;
    
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 1;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();

    CFE_SB_InitMsg (&packet, CFE_EVS_EVENT_MSG_MID, sizeof(packet), TRUE);

    /* Pass 1 for branch coverage. */
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX, -1, 1);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);

    /* Verify results */
    UtAssert_True(result == -6, "TO_MEMORY_FULL_ERR");

    /* Pass 2 for branch coverage. */

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX, CFE_SUCCESS, 1);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);      
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);
    
    /* Verify results */
    UtAssert_True(result == -6, "TO_MEMORY_FULL_ERR");
}


void Test_TO_PriorityQueue_QueueMsg_Queue_Full(void)
{
    int32                   DataPipe;    
    int32                   result = CFE_SUCCESS;
    TO_HkTlm_t              msgCfeEvsEvent;
    void *TO_CopyBuffer = NULL;
    int32 status = 0;

    CFE_SB_MsgPtr_t  msgPtr = NULL;
    
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    char            expectedEvent[100];

    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 1;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};
    
    msgPtr = (CFE_SB_MsgPtr_t)&msgCfeEvsEvent;

    DataPipe = Ut_CFE_SB_CreatePipe("TO_UDP");

    /* Initialize a bunch of telemetry messages for downlink. */
    CFE_SB_InitMsg (&msgCfeEvsEvent, CFE_EVS_EVENT_MSG_MID, sizeof(msgCfeEvsEvent), TRUE);
    
    /* Now load up the software bus with all the messages starting with low
     * priority first. */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsEvent, DataPipe); 
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_QUEUE_FULL, 0);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);       
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];    
    
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);
    
    Ut_CFE_ES_GetPoolBuf((uint32 **) &TO_CopyBuffer, channel->MemPoolHandle, 43);
    
    /* Verify results */
    sprintf(expectedEvent, "PutPoolBuf: channel=%d error=%i",
            channel->channelIdx,
            -1);       

    /* Verify results */
    UtAssert_EventSent(TO_PUT_POOL_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue QueueMsg PutPoolBuf Failed - event correct");                         
}

void Test_TO_PriorityQueue_QueueMsg_QueuePut_Error(void)
{
    int32                   DataPipe;    
    int32                   result = CFE_SUCCESS;
    TO_HkTlm_t              msgCfeEvsEvent;
    void *TO_CopyBuffer = NULL;
    int32 status = 0;

    CFE_SB_MsgPtr_t  msgPtr = NULL;
    
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          ChannelIdx = 0;
    char            expectedEvent[100];
    TO_PriorityQueue_t *pqueue;
    uint32 pQueueIndex = 1;
    TO_ChannelData_t* channel = NULL;
    TO_MessageFlow_t msgFlowTest = {CFE_ES_HK_TLM_MID, 1, 1};
    
    msgPtr = (CFE_SB_MsgPtr_t)&msgCfeEvsEvent;

    DataPipe = Ut_CFE_SB_CreatePipe("TO_UDP");

    /* Initialize a bunch of telemetry messages for downlink. */
    CFE_SB_InitMsg (&msgCfeEvsEvent, CFE_EVS_EVENT_MSG_MID, sizeof(msgCfeEvsEvent), TRUE);
    
    /* Now load up the software bus with all the messages starting with low
     * priority first. */
    Ut_CFE_SB_AddMsgToPipe(&msgCfeEvsEvent, DataPipe); 
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_ERROR, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf);       
    
    result = TO_PriorityQueue_QueueMsg(channel, msgPtr, pQueueIndex);
    
    Ut_CFE_ES_GetPoolBuf((uint32 **) &TO_CopyBuffer, channel->MemPoolHandle, 43);      

    /* Verify results */
    UtAssert_EventSent(TO_OSQUEUE_PUT_ERROR_EID, 
                       CFE_EVS_ERROR, "", "OS_QueuePut failed: ");                           
}

/**
 * Test TO_PriorityQueue_IsValid(), NULL pointer check
 */
void Test_TO_PriorityQueue_IsValid_Null_ConfigTblPtr_Check(void)
{
    uint16        PQueueIdx = 0;
    uint16        channelIdx = 0;
    osalbool      result = TRUE;
    osalbool      expected = FALSE;
    
    TO_ChannelData_t* channel = NULL;

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    result = TO_PriorityQueue_IsValid(channel, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "TO_PriorityQueue_IsValid, NULL Pointer Check");
}

/**
 * Test TO_PriorityQueue_IsValid(), NULL pointer check
 */
void Test_TO_PriorityQueue_IsValid_Null_Ptr_Check(void)
{
    uint16        PQueueIdx = 0;
    uint16        channelIdx = 0;
    osalbool      result = TRUE;
    osalbool      expected = FALSE;
    
    TO_ChannelData_t* channel = NULL;

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    result = TO_PriorityQueue_IsValid(channel, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "TO_PriorityQueue_IsValid, NULL Pointer Check");
}

/**
 * Test TO_PriorityQueue_IsValid() State Check
 */
void Test_TO_PriorityQueue_IsValid_PQ_State_Check(void)
{
    TO_NoArgCmd_t InSchMsg;
    TO_QueryPriorityQueueCmd_t InCmd;
    int32         DataPipe;
    int32         CmdPipe;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16        PQueueIdx = 0;
    uint16        channelIdx = 0;
    osalbool      result = TRUE;
    osalbool      expected = FALSE;
    
    TO_ChannelData_t* channel;

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[channelIdx];
    
    channel->State = 2;    
    
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
    
    TO_InitApp();

    channel->ConfigTblPtr->PriorityQueue[0].State = 0;

    /* Execute the function being tested */
    result = TO_PriorityQueue_IsValid(channel, PQueueIdx);

    /* Verify results */
    UtAssert_True (result == expected, "TO_PriorityQueue_IsValid, State not enabled");
}


/**************************************************************************
 * Tests for TO Config Utils (Table validation, Managing Tables)
 **************************************************************************/
void Test_TO_InitTables_Ground_Table_Fail_TBL_Register(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    
    iStatus = TO_InitTables(channel);
    
    sprintf(expectedEvent, "Failed to register Config table (0x%08X) for channel %d",
        (unsigned int)iStatus, ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to register Config table - event correct");
}

void Test_TO_InitTables_Dump_Table_Fail_TBL_Register(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 2);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    
    iStatus = TO_InitTables(channel);
    
    sprintf(expectedEvent, "Failed to register Dump table (0x%08X) for channel %d",
        (unsigned int)iStatus,
        ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to register Dump table - event correct");
}

void Test_TO_InitTables_Dump_Table_Fail_TBL_Load(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 2);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  -1, 1);       
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
    
    iStatus = TO_InitTables(channel);
    
    sprintf(expectedEvent, "Failed to load Dump Table (0x%08X) for channel %d",
        (unsigned int)iStatus,
        ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to load Dump table - event correct");
}

void Test_TO_InitTables_Load_Backup_Table(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  0, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);
    
    TO_LoadBackupConfigTable(channel);
    
    sprintf(expectedEvent, "Loaded built-in emergency backup config table! - channel %u",
        channel->channelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_BACKUP_TABLE_INF_EID, CFE_EVS_INFORMATION, 
                       expectedEvent, "Loaded built-in emergency backup config table - event correct");
}

void Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->ConfigTblPtr = NULL;
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Config table pointer is NULL, table validation ended");

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_NULL_PTR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Test_TO_ValidateConfigTbl NULL table pointer - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_NULL_PTR_ERR, "TO_CONFIG_TABLE_NULL_PTR_ERR");
}

void Test_TO_ValidateConfigTbl_InvalidCCSDSVersion(void)
{
    int32 iStatus = 0;
    char expectedEvent[100];

    TO_ChannelTbl_t TO_ConfigTable =
    {
        /* Table ID */
        0,
        {
            /* Message Flows */
            {0x0800, 1, 0},
            {0x2801, 1, 0}
        },{
            /* Priority Queues */
            {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  2, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  3, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  4, TO_PRIORITY_QUEUE_TYPE_FIFO}
        }
    };

    iStatus = TO_ValidateConfigTbl(&TO_ConfigTable);

    sprintf(expectedEvent, "Invalid CCSDS Version for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
            1,
            TO_ConfigTable.MessageFlow[1].MsgId,
            TO_ConfigTable.TableID);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_CCSDS_VER_INVALID_EID, CFE_EVS_ERROR,
                       expectedEvent, "Invalid CCSDS Version for table index - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_RETURN_INVALID_VERSION, "TO_CONFIG_TABLE_RETURN_INVALID_VERSION");
}

void Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State = 3;
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Priority Queue State is invalid (%d), for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State,
            channel->ConfigTblPtr->TableID,
            PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_STATE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue State is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_STATE_ERR, "TO_CONFIG_TABLE_PQUEUE_STATE_ERR");
}

void Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType = 3;
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Priority Queue Qtype is invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType,
            channel->ConfigTblPtr->TableID,
            PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue Qtype is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR, "TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR");
}

void Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit = 0;
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Priority Queue MsgLimit invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit,
            channel->ConfigTblPtr->TableID,
            PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue MsgLimit is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR, "TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR");
}

void Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    
    /* TO_MAX_QUEUE_SIZE_LIMIT = 100 currently */
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit = TO_MAX_QUEUE_SIZE_LIMIT + 1;
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Priority Queue MsgLimit invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit,
            channel->ConfigTblPtr->TableID,
            PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue MsgLimit is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR, "TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR");
}

void Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information - channel is only opened */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "No Priority Queues or all queue states are unused in the config table with Table ID (%lu)",
            channel->ConfigTblPtr->TableID);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_NO_PQUEUES_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue Qtype is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_NO_PQUEUES_ERR, "TO_CONFIG_TABLE_NO_PQUEUES_ERR");
}

void Test_TO_ValidateConfigTbl_Secondary_Header_Absent(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 MFlowIdx = 0;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    
    channel->ConfigTblPtr->PriorityQueue[0].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[1].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[2].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[3].MsgLimit = 100;
    
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId    = 0x042;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit = 42;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID = 3;        
    
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Secondary Header is absent for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
            MFlowIdx,
            channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId,
            channel->ConfigTblPtr->TableID);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_SHDR_ABSENT_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Secondary Header is absent for table index - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER, "TO_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER");
}

void Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    
    channel->ConfigTblPtr->PriorityQueue[0].MsgLimit = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[1].MsgLimit = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[2].MsgLimit = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[3].MsgLimit = TO_MAX_PQ_MSG_SIZE_LIMIT;
    
    channel->ConfigTblPtr->MessageFlow[0].MsgId    = CFE_ES_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent, "Message Flow MsgLimit invalid (%u) in Table ID (%lu) at table index (%d) ",
           (unsigned int)channel->ConfigTblPtr->MessageFlow[0].MsgLimit,
           channel->ConfigTblPtr->TableID,
           0);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Message Flow MsgLimit invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR, "TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR");

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = CFE_EVS_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = (TO_MAX_PQ_MSG_SIZE_LIMIT+1);
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 3;
            
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent, "Message Flow MsgLimit invalid (%u) in Table ID (%lu) at table index (%d) ",
           (unsigned int)channel->ConfigTblPtr->MessageFlow[0].MsgLimit,
           channel->ConfigTblPtr->TableID,
           0);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Message Flow MsgLimit invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR, "TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR");
}

void Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[0].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[1].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[2].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[3].MsgLimit = 100;

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
}

void Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    uint16 MFlowIdx = 0;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;
    
    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    
    channel->ConfigTblPtr->PriorityQueue[0].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[1].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[2].MsgLimit = 100;
    channel->ConfigTblPtr->PriorityQueue[3].MsgLimit = 100;
    
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId    = CFE_ES_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit = 10;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID = 5;
            
    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
    
    sprintf(expectedEvent, "Message Flow Priority Queue ID is invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID,
            channel->ConfigTblPtr->TableID,
            MFlowIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Message Flow Priority Queue ID is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR, "TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR");
}

void Test_TO_ValidateConfigTbl1(void)
{
    TO_NoArgCmd_t                     InSchMsg;
    int32                             DataPipe;
    int32 result = 0;
    int32 expected = CFE_SUCCESS;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);
    
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    
    /* Execute the function being tested */
    result = TO_Custom_Init();
    
        /* Verify results */
    UtAssert_True (result == expected, 
                "TO_Custom_Init() did return an expected value");      
}

void Test_TO_ValidateConfigTbl2(void)
{
    TO_NoArgCmd_t                     InSchMsg;
    int32                             DataPipe;
    int32 result = 0;
    int32 expected = CFE_SUCCESS;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    result = TO_Custom_Init();

    /* Verify results */
    UtAssert_True (result == expected,
                "TO_Custom_Init() did return an expected value");
}

void Test_TO_ManageChannelTables_GetStatus_Error(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_SEVERITY_ERROR, 1);       
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);    
            
    TO_ManageChannelTables(initialManage, ChannelIdx);
    
    sprintf(expectedEvent, "Channel %u config table handle failure! (0x%08X)",
            ChannelIdx,
            CFE_SEVERITY_ERROR);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Channel config table handle failure - event correct");                 
}

void Test_TO_ManageChannelTables_GetStatus_Validate_Pending(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, -1 , 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_MANAGE_INDEX);

    TO_ManageChannelTables(initialManage, ChannelIdx);

    sprintf(expectedEvent, "Failed to manage Config table for channel %u, (0x%08X)",
           ChannelIdx,
           -1);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_MANAGE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to manage Config table for channel - event correct");
}

void Test_TO_ManageChannelTables_GetStatus_Update_Pending(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, -1 , 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 0);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_MANAGE_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
            
    TO_ManageChannelTables(initialManage, ChannelIdx);
    
    sprintf(expectedEvent, "Failed to manage Config table for channel %u, (0x%08X)",
           ChannelIdx,
           -1);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_MANAGE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Failed to manage Config table for channel - event correct");                 
}

void Test_TO_ManageChannelTables_GetAddress_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, CFE_SUCCESS , 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);    
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_MANAGE_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);    
            
    TO_ManageChannelTables(initialManage, ChannelIdx);
    
    sprintf(expectedEvent, "Failed to get Config table's address for channel %u, (0x%08X)",
            ChannelIdx,
            -1);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_ADDR_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Failed to get Config table's address for channel - event correct");                 
}


void Test_TO_ManageChannelTables_GetStatus_Update_Pending_Fail_Dequeue(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_UPDATE_PENDING, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, CFE_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 0);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, -1, 1);
    
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);   
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_UNSUBSCRIBE_INDEX);  
            
    TO_ManageChannelTables(initialManage, ChannelIdx);
    
    sprintf(expectedEvent, "Failed to clear SB data pipe for channel %u, (0x%08X)",
                                     ChannelIdx,
                                     CFE_TBL_INFO_UPDATE_PENDING);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Failed to clear SB data pipe for channel - event correct");

}

void Test_TO_ManageChannelTables_GetAddress_Process_Config_Tbl_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    osalbool initialManage = TRUE;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    /* Set table pointer to NULL so TO_ProcessNewConfigTbl will return -3 (TO_MESSAGE_FLOW_NO_TABLE_ERR) */
    channel->ConfigTblPtr = NULL;
    
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, CFE_SUCCESS , 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_INFO_UPDATED, 1);
  
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_MANAGE_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);    
            
    TO_ManageChannelTables(initialManage, ChannelIdx);
    
    sprintf(expectedEvent, "Process New Config Table Failed for channel %u, (0x%08X)",
           ChannelIdx,
           -3);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_PROCESS_CONFIG_TBL_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Process New Config Table Failed for channel - event correct");

    /* Pass #2 for branch coverage purposes. */

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, CFE_TBL_INFO_UPDATED , 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_INFO_UPDATED, 1);

    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETSTATUS_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_MANAGE_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    TO_ManageChannelTables(initialManage, ChannelIdx);

    sprintf(expectedEvent, "Process New Config Table Failed for channel %u, (0x%08X)",
           ChannelIdx,
           -3);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_PROCESS_CONFIG_TBL_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Process New Config Table Failed for channel - event correct");
}

/**************************************************************************
 * Tests for TO Channel algorithms
 **************************************************************************/
void Test_TO_Channel_SBPipe_Dequeue_All(void)
{
    osalbool      result = TRUE;
    osalbool      expected = FALSE;
    
    uint16 ChannelID = 5;
    
    /* Execute the function being tested */
    result = TO_Channel_SBPipe_Dequeue_All(ChannelID);

    /* Verify results */
    UtAssert_EventSent(TO_FLUSH_INVALID_CHIDX_ERR_EID, CFE_EVS_ERROR, "",
                       "ChannelID out of range in SB pipe dequeue all.");
                           
    UtAssert_True (result == expected, "ChannelID out of range in SB pipe dequeue all.");    
}

void Test_TO_Channel_SBPipe_Dequeue_All_Continue(void)
{
    osalbool      result = TRUE;
    osalbool      expected = TRUE;
    
    uint16 ChannelID = 0;
    
    TO_InitApp();
    
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);
    
    /* Execute the function being tested */
    result = TO_Channel_SBPipe_Dequeue_All(ChannelID);

    /* Verify results */   
    UtAssert_True (result == expected, "SB Receive Msg Dequeue Success.");    
}

/**
 * Test TO_Channel_State(), Success
 */
void Test_TO_Channel_State(void)
{
    uint8 result = TO_CHANNEL_UNKNOWN;
    uint8 expected = TO_CHANNEL_OPENED;
    
    uint16 ChannelID = 0;

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Execute the function being tested */
    result = TO_Channel_State(ChannelID);
    
    /* Verify results */
    UtAssert_True (result == expected, "Channel 0 is open.");
}

/**
 * Test TO_Channel_State(), Fail ChannelID out of range.
 */
void Test_TO_Channel_State_Fail(void)
{
    uint8 result = TO_CHANNEL_UNKNOWN;
    uint8 expected = TO_CHANNEL_UNKNOWN;
    
    uint16 ChannelID = 5;

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Execute the function being tested */
    result = TO_Channel_State(ChannelID);
    
    /* Verify results */    
    UtAssert_EventSent(TO_CHANNEL_OUT_OF_RANGE_ERR_EID, CFE_EVS_ERROR, "",
                       "Invalid channel index");
                       
    UtAssert_True (result == expected, "Channel 0 is open.");                   
}

/**
 * Test TO_Channel_OpenChannel(), Fail ChannelID out of range.
 */
void TO_Channel_OpenChannel_Fail_Invalid_ChannelID(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = TO_CHANNEL_OUT_OF_RANGE_ERR;
        
    uint16 ChannelID = 5;
    
    /* Execute the function being tested */
    result = TO_Channel_OpenChannel(ChannelID, "GRD", "GRD_CFG", "/boot/apps/to_grd_def.tbl", &TO_EmptyConfigTable, "GRD_DMP", 1, "CFTOSemIdGrd");
    
    /* Verify results */    
    UtAssert_EventSent(TO_INIT_APP_ERR_EID, CFE_EVS_ERROR, "",
                       "Invalid channel index of 5.");
    UtAssert_True (result == expected, "Invalid channel index.");                
}

/**
 * Test TO_Channel_Fail_ChannelAlreadyOpen(), fail, channel is already open.
 */
void TO_Channel_OpenChannel_Fail_ChannelAlreadyOpen(void)
{
    int32 result = CFE_SUCCESS;

    uint16 ChannelID = 0;

    /* Execute the function being tested */
    TO_AppData.ChannelData[ChannelID].State = TO_CHANNEL_OPENED;
    result = TO_Channel_OpenChannel(ChannelID, "GRD", "GRD_CFG", "/boot/apps/to_grd_def.tbl", &TO_EmptyConfigTable, "GRD_DMP", 1, "CFTOSemIdGrd");

    /* Verify results */
    UtAssert_True (result == TO_CHANNEL_OPEN_ERR, "Channel already opened.");
}


void Test_TO_Channel_Open_OS_CountSemCreate_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[1000];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->State = TO_CHANNEL_CLOSED; 
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, OS_SEM_FAILURE, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMCREATE_INDEX);
    
    status = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME,
                                     TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_UDP_DUMP_TABLENAME,
                                     4, "cs_gnd");

    sprintf(expectedEvent, "Failed to create counting semaphore "
                              "for CF channel semaphore:%s for TO channel(%u):%s . "
                              "(OSAL Error:%d)",
                              "cs_gnd", ChannelIdx, "UDP", OS_SEM_FAILURE);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_APP_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to create counting semaphore - event correct");
}

void Test_TO_Channel_Open_OutputQueue_Buildup_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    
    /* Set function hooks */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_QUEUECREATE_INDEX, &Ut_OSAPI_QueueCreateHook);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->State = TO_CHANNEL_CLOSED; 
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMCREATE_INDEX);    
    
    status = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME,
                                     TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_UDP_DUMP_TABLENAME,
                                     4, "cs_gnd");
                                     
    UtAssert_True(status == OS_ERR_NAME_TAKEN, "OutputQueue_Buildup Error");                                 
}

void Test_TO_Channel_Open_CFE_SB_CreatePipe_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->State = TO_CHANNEL_CLOSED; 
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUECREATE_INDEX, OS_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, -1, 1);
    
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMCREATE_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUECREATE_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_CREATEPIPE_INDEX);
    
    status = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME,
                                     TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_UDP_DUMP_TABLENAME,
                                     4, "cs_gnd");
                                     
    sprintf(expectedEvent, "Failed to create channel (%d) '%s' pipe (0x%08X)",
            0,
            "UDP",
            -1);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_DATAPIPE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to create channel - event correct");                                      
                                                                   
}

void Test_TO_Channel_Open_TO_InitTables_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->State = TO_CHANNEL_CLOSED; 
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUECREATE_INDEX, OS_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 1);
    
    
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMCREATE_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUECREATE_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_CREATEPIPE_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    
    status = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME,
                                     TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_UDP_DUMP_TABLENAME,
                                     4, "cs_gnd");
                                     
    sprintf(expectedEvent, "Failed to init config tables (0x%08X) for channel %d",
           status,
           0);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Failed to init config tables - event correct");                                      
                                                                   
}

void Test_TO_Channel_Open_PoolCreateEx_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    channel->State = TO_CHANNEL_CLOSED; 
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUECREATE_INDEX, OS_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_POOLCREATEEX_INDEX, -1, 1);
    
    
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMCREATE_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUECREATE_INDEX);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_CREATEPIPE_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_POOLCREATEEX_INDEX);
    
    status = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME,
                                     TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_UDP_DUMP_TABLENAME,
                                     4, "cs_gnd");
                                     
    sprintf(expectedEvent, "Error creating memory pool (0x%08X) for channel %d", status, 0);

    /* Verify results */
    UtAssert_EventSent(TO_CR_POOL_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Error creating memory pool - event correct");                                      
                                                                   
}

void Test_TO_Channel_LockByIndex_Index_Out_Of_Range(void)
{
    uint16  ChannelIdx = 5;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    TO_Channel_LockByIndex(ChannelIdx);
    
    sprintf(expectedEvent, "Invalid channel index (%u).",
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_OUT_OF_RANGE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Invalid channel index - event correct"); 
}

void Test_TO_Channel_UnlockByIndex_Index_Out_Of_Range(void)
{
    uint16  ChannelIdx = 5;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    TO_Channel_UnlockByIndex(ChannelIdx);
    
    sprintf(expectedEvent, "Invalid channel index (%u).",
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_OUT_OF_RANGE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Invalid channel index - event correct"); 
}

void Test_TO_Channel_LockByRef_NullPtr(void)
{
    TO_Channel_LockByRef(0);
}

void Test_TO_Channel_UnlockByRef_NullPtr(void)
{
    TO_Channel_UnlockByRef(0);
}

void Test_TO_Channel_LockByRef_Mutex_Take_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMTAKE_INDEX, OS_ERROR, 1);    
    
    TO_Channel_LockByRef(channel);
    
    sprintf(expectedEvent, "Channel lock mutex failure (%i) on channel %d.",
            OS_ERROR,
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_LOCK_MUTEX_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Channel lock mutex failure - event correct");
}

void Test_TO_Channel_UnlockByRef_Mutex_Give_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMGIVE_INDEX, OS_ERROR, 1);    
    
    TO_Channel_UnlockByRef(channel);
    
    sprintf(expectedEvent, "Channel unlock mutex failure (%i) on channel %d.",
            OS_ERROR,
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_UNLOCK_MUTEX_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Channel unlock mutex failure - event correct");
}

void Test_TO_Channel_InitAll_Mutex_Create_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->State = TO_CHANNEL_OPENED;
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMCREATE_INDEX, OS_SEM_FAILURE, 1);    
    
    TO_Channel_InitAll();
                      
    UtAssert_True(channel->State == TO_CHANNEL_UNKNOWN, "Channel Cleanup OS Mut Semphore Create Failure");
}

void Test_TO_Channel_CleanupAll_PriorityQueue_TeardownAll_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, CFE_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEDELETE_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_SUCCESS, 0);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 3);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 4);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEDELETE_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);      
    
    TO_Channel_CleanupAll();
    
    /* TO_Channel_CleanupAll does not return a status, TO_PriorityQueue_TeardownAll does and it's OS_ERROR */
    sprintf(expectedEvent, "Priority Queue Teardown failure %ld on channel %u.",
            OS_ERROR, 
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_TEARDOWN_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Priority Queue Teardown failure - event correct");
}

void Test_TO_Channel_CleanupAll_OutputQueue_Teardown_Failure(void)
{
    uint16  ChannelIdx = 0;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, CFE_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEDELETE_INDEX, OS_ERROR, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEDELETE_INDEX);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
    
    Ut_TO_Custom_SetReturnCode(UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX, -1, 1);
    Ut_TO_Custom_ContinueReturnCodeAfterCountZero(UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX);     
    
    TO_Channel_CleanupAll();
    
    /* TO_Channel_CleanupAll does not return a status, TO_OutputQueue_Teardown Custom returns -1 */
    sprintf(expectedEvent, "Output Queue Teardown failure %ld on channel %u.",
            -1, 
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_TEARDOWN_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Output Queue Teardown failure - event correct");
}

void Test_TO_Channel_Cleanup_Index_Out_Of_Range_Failure(void)
{
    uint16  ChannelIdx = 5;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();  
    
    /* Declared function prototype at the top of file because it will
     * generate warning due to the fact the function is not declared
     * in the header file of to_channel.h - compiler warning shown below
     *
     * warning: implicit declaration of function 'TO_Channel_Cleanup'; 
     * did you mean 'TO_Channel_CleanupAll'? [-Wimplicit-function-declaration]
     */    
    TO_Channel_Cleanup(ChannelIdx);
    
    sprintf(expectedEvent, "Invalid channel index (%u).",
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_OUT_OF_RANGE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Invalid channel index - event correct");
}

void Test_TO_Channel_Init_Index_Out_Of_Range_Failure(void)
{
    uint16  ChannelIdx = 5;
    uint32 status = 0;
    TO_ChannelData_t* channel;    
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);    
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();  
    
    /* Declared function prototype at the top of file because it will
     * generate warning due to the fact the function is not declared
     * in the header file of to_channel.h - compiler warning shown below
     *
     * warning: implicit declaration of function 'TO_Channel_Init'; 
     * did you mean 'TO_Channel_InitAll'? [-Wimplicit-function-declaration]
     */
    TO_Channel_Init(ChannelIdx);
    
    sprintf(expectedEvent, "Invalid channel index (%u).",
            ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CHANNEL_OUT_OF_RANGE_ERR_EID, CFE_EVS_ERROR, 
                       expectedEvent, "Invalid channel index - event correct");
}


/**************************************************************************
 * Tests for TO_Classifier_Run
 **************************************************************************/
void Test_TO_Classifier_NoMessage(void)
{
    int32 errCodeSet = CFE_SB_WRONG_MSG_TYPE; //set error code, anything besides CFE_SUCCESS or CFE_SB_NO_MESSAGE
    TO_ChannelData_t* groundChannel;

    /* Set up expected event string*/
    char expectedEvent[100];
    memset(expectedEvent, 0x00, sizeof(expectedEvent));
    sprintf(expectedEvent, "Data pipe read error (0x%08X) on channel 0", (unsigned int)errCodeSet);

    /* Set a fail result */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, errCodeSet, 1);

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);

    /* Verify results */
    UtAssert_EventSent(TO_PIPE_READ_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Test_TO_Classifier No Message - error event correct");

}

void Test_TO_Classifier_MessageTooLong(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;

    int32 messagePipeId = 0;
    int32 expectedAppMsgsDropped = 0;
    int32 expectedChannelMsgsDropped = 0;

    uint16 dummyMID = 0x0800;
    char dummyMessageBuf[TO_MAX_MSG_LENGTH+1];
    memset(dummyMessageBuf, 0x00, sizeof(dummyMessageBuf));

    /* Set up expected event string*/
    char expectedEvent[100];
    memset(expectedEvent, 0x00, sizeof(expectedEvent));
    sprintf(expectedEvent, "Message too long (size = %lu > max = %d) for msgId = (0x%04X) on channel (%u)",
    		TO_MAX_MSG_LENGTH+1, TO_MAX_MSG_LENGTH, (unsigned short)dummyMID, (unsigned int)gndChannelId);


    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];

    /* Load faulty message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 0 pipe");
    Ut_CFE_SB_InitMsgHook(&dummyMessageBuf, dummyMID, sizeof(dummyMessageBuf), TRUE);

    Ut_CFE_SB_AddMsgToPipe(&dummyMessageBuf, messagePipeId);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Setup test counts*/
//    expectedAppMsgsDropped = TO_AppData.HkTlm.TotalMsgDropped + 1;
    expectedChannelMsgsDropped = groundChannel->DropMsgCount + 1;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);

    /* Verify results */
    UtAssert_EventSent(TO_TLM_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Test_TO_Classifier Message Too Long - error event correct");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == expectedAppMsgsDropped,
//            "Test_TO_Classifier Message Too Long - app dropped count correct");
    UtAssert_True(groundChannel->DropMsgCount == expectedChannelMsgsDropped,
                  "Test_TO_Classifier Message Too Long - app dropped count correct");

}

void Test_TO_Classifier_MsgNullPtr(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;

    int32 messagePipeId = 0;

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];

    /* Load faulty message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 1 pipe");

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);
}

void Test_TO_Classifier_MaxMessages(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;

    int32 messagePipeId = 0;

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];

    /* Load faulty message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 1 pipe");

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 0);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);
}

void Test_TO_Classifier_NoMsgFlow(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;

    int32 messagePipeId = 0;
    int32 expectedAppMsgsDropped = 0;
    int32 expectedChannelMsgsDropped = 0;

    int32 msgLength = TO_MAX_MSG_LENGTH-10; //set message length valid, < TO_MAX_MSG_LENGTH
    uint16 dummyMID = 0x7777;
    char dummyMessageBuf[msgLength];
    memset(dummyMessageBuf, 0x00, sizeof(dummyMessageBuf));

    /* Set up expected event string*/
    char expectedEvent[100];
    memset(expectedEvent, 0x00, sizeof(expectedEvent));
    sprintf(expectedEvent, "Classifier Recvd invalid msgId (0x%04X) or message flow was removed on channel (%u)",
            (unsigned short)dummyMID, (unsigned int)gndChannelId);


    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];

    /* Load message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 1 pipe");
    Ut_CFE_SB_InitMsgHook(&dummyMessageBuf, dummyMID, sizeof(dummyMessageBuf), TRUE);

    Ut_CFE_SB_AddMsgToPipe(&dummyMessageBuf, (CFE_SB_PipeId_t)messagePipeId);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Setup test counts*/
//    expectedAppMsgsDropped = TO_AppData.HkTlm.TotalMsgDropped + 1;
    expectedChannelMsgsDropped = groundChannel->DropMsgCount + 1;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);

    /* Verify results */
    UtAssert_EventSent(TO_MF_MSG_ID_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Test_TO_Classifier No MsgFlow - error event correct");
//    UtAssert_True(TO_AppData.HkTlm.TotalMsgDropped == expectedAppMsgsDropped,
//                  "Test_TO_Classifier No MsgFlow - app dropped count correct");
    UtAssert_True(groundChannel->DropMsgCount == expectedChannelMsgsDropped,
                  "Test_TO_Classifier No MsgFlow - app dropped count correct");
}


void Test_TO_Classifier_Message_pqueue_NULL(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;

    int32 messagePipeId = 0;
    int32 expectedAppMsgsDropped = 0;
    int32 expectedChannelMsgsDropped = 0;

    int32 msgLength = TO_MAX_MSG_LENGTH-10; //set message length valid, < TO_MAX_MSG_LENGTH
    uint16 dummyMID = 0x0800;
    char dummyMessageBuf[msgLength];
    memset(dummyMessageBuf, 0x00, sizeof(dummyMessageBuf));

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];
    
    /* Set the PQueueID to be greater than TO_MAX_PRIORITY_QUEUES so
     * call to TO_MessageFlow_GetPQueue returns NULL
     */
    groundChannel->ConfigTblPtr->MessageFlow[0].PQueueID = 5;

    /* Load message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 1 pipe");
    Ut_CFE_SB_InitMsgHook(&dummyMessageBuf, dummyMID, sizeof(dummyMessageBuf), TRUE);

    Ut_CFE_SB_AddMsgToPipe(&dummyMessageBuf, (CFE_SB_PipeId_t)messagePipeId);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);
}


void Test_TO_Classifier_Message_HighwaterMark(void)
{
    TO_ChannelData_t* groundChannel;
    uint32 gndChannelId = 0;
    void *TO_CopyBuffer = NULL;

    int32 messagePipeId = 0;
    int32 expectedAppMsgsDropped = 0;
    int32 expectedChannelMsgsDropped = 0;

    int32 msgLength = TO_MAX_MSG_LENGTH-10; //set message length valid, < TO_MAX_MSG_LENGTH
    uint16 dummyMID = 0x0800;
    char dummyMessageBuf[msgLength];
    memset(dummyMessageBuf, 0x00, sizeof(dummyMessageBuf));

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Initialize app*/
    TO_InitApp();
    
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, OS_SUCCESS, 0);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, 0, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_PUTPOOLBUF_INDEX);
    
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX, &Ut_CFE_ES_GetPoolBuf); 

    /* Get channel data information */
    groundChannel = &TO_AppData.ChannelData[0];
    
    /* Set the HighwaterMark high to test the false part of the if check
     * in to_classifer
     */
    groundChannel->DumpTbl.PriorityQueue[1].HighwaterMark = 20;  
    
    Ut_CFE_ES_GetPoolBuf((uint32 **) &TO_CopyBuffer, groundChannel->MemPoolHandle, 42);    

    /* Load message*/
    messagePipeId = Ut_CFE_SB_CreatePipe("Classifier Ch 1 pipe");
    Ut_CFE_SB_InitMsgHook(&dummyMessageBuf, dummyMID, sizeof(dummyMessageBuf), TRUE);

    Ut_CFE_SB_AddMsgToPipe(&dummyMessageBuf, (CFE_SB_PipeId_t)messagePipeId);

    groundChannel->DataPipeId = (CFE_SB_PipeId_t)messagePipeId;

    /* Execute the function being tested */
    TO_Classifier_Run(groundChannel);
}

void Test_TO_Scheduler_Run_OS_Max_Queues(void)
{
    uint16                  ChannelIdx = 0;
    TO_ChannelData_t*       channel = NULL;
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[0];
    
    channel->DumpTbl.PriorityQueue[0].OSALQueueID = 128;
    
    TO_Scheduler_Run(channel);
}

void Test_TO_Scheduler_Run_QueueMsg_Fail(void)
{
    uint16                  ChannelIdx = 0;
    TO_ChannelData_t*       channel = NULL;
    
    /* Get the call to TO_OutputQueue_QueueMsg return not CFE_SUCCESS */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEPUT_INDEX, -1, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEPUT_INDEX);
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[0];
    
    TO_Scheduler_Run(channel);
}

void Test_TO_Scheduler_Run_HighwaterMark(void)
{
    uint16                  ChannelIdx = 0;
    TO_ChannelData_t*       channel = NULL;
    
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Initialize app*/
    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[0];
    
    channel->OutputQueue.HighwaterMark = 20;
    
    TO_Scheduler_Run(channel);
}


void Test_TO_ReportHousekeeping_Ground(void)
{
    uint16  ChannelIdx = 0;
    TO_ChannelData_t* channel;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    TO_ReportHousekeeping();
}

void Test_TO_ReportHousekeeping_UnknownChannelID(void)
{
    TO_ChannelData_t* channel;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Set an incorrect channel ID */
    TO_AppData.ChannelData[0].channelIdx = 5;

    TO_ReportHousekeeping();
}

void Test_TO_SendDiag_Ground(void)
{
    uint16  ChannelIdx = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];
    
    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
  
    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();
    
    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    /* Set ConfigTblPtr to NULL to test NULL table pointer check */
    channel->ConfigTblPtr = NULL;
    
    TO_SendDiag(ChannelIdx);
    
    sprintf(expectedEvent, "Channel %d table is not available.", ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CMD_SEND_MISSING_TBL_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "Channel table is not available - event correct");      
}


/**************************************************************************
 * Additional tests for TO_ProcessNewAppCmds
 **************************************************************************/ 
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

void Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   DataPipe;
    int32                   CmdPipe;
    uint16                  channelIdx = 5;

    CFE_SB_MsgId_t  MsgId = 0x0a20;
    uint16          MsgLimit = 100;
    uint16          PQueueIdx = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = channelIdx;
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
                                          "Add Message Flow: Invalid channel.");
}

void Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr(void)
{
    TO_NoArgCmd_t           InSchMsg;
    TO_AddMessageFlowCmd_t  InCmd;
    int32                   DataPipe;
    int32                   CmdPipe;
    uint16                  channelIdx = 0;

    CFE_SB_MsgId_t  MsgId = 0x0a20;
    uint16          MsgLimit = 100;
    uint16          PQueueIdx = 0;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("TO_CMD_PIPE");
    CFE_SB_InitMsg (&InCmd, TO_CMD_MID, sizeof(InCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InCmd, TO_ADD_MESSAGE_FLOW_CC);
    InCmd.ChannelIdx = channelIdx;
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
     UtAssert_EventSent(TO_MSG_FLOW_MISSING_TBL_ERR_EID, CFE_EVS_ERROR, "", 
                                          "Channel missing valid table data.");
}

/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, SubscribeEx Failure
 */
void Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure(void)
{
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;
    uint16          MsgLimit = 1;
    uint16          PQueueIdx = 0;
    uint16          channelIdx = 0;
    osalbool        result = TRUE;
    osalbool        expected = FALSE;
    
    TO_ChannelData_t channel;
    TO_ChannelTbl_t table;
    CFE_PSP_MemSet(&table, 0x0, sizeof(table)); 
    
    /* Get channel data information */
    TO_AppData.ChannelData[channelIdx].State = TO_CHANNEL_OPENED;
    TO_AppData.ChannelData[channelIdx].ConfigTblPtr = &table;
    TO_AppData.ChannelData[channelIdx].ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;       

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_EMPTY, 0);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_QUEUEGET_INDEX);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, CFE_SB_BAD_ARGUMENT, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_SUBSCRIBEEX_INDEX);  

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);
    
    TO_InitApp();      

    /* Execute the function being tested */
    result = TO_MessageFlow_Add(channelIdx, MsgId, MsgLimit, PQueueIdx);
    
    /* Verify results */
    UtAssert_True (result == expected, "Message flow failed to subscribe on channel.");
        
    UtAssert_EventSent(TO_CMD_ADD_MSG_FLOW_ERR_EID, CFE_EVS_ERROR, "",
                       "Message flow failed to subscribe"); 
}


/**
 * Test TO_ProcessNewAppCmds(), AddMessageFlow command, Channel Not Open
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
                       "Channel not open."); 
}


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
                                          "Message flow is not defined.");
}

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
                                          "Invalid channel - channel index out of range.");
}

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
                                          "Channel has not been opened..");
}

/**
 * Test TO_ProcessNewAppCmds(), QueryMessageFlow command, Nominal channel invalid
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
                                          "Invalid channel - channel index out of range.");
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
                                          "Channel missing valid table data.");
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
    UtAssert_EventSent(TO_PQUEUE_INFO_ERR_EID, 
                       CFE_EVS_ERROR, "", "Invalid channel index");
}

/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue command, Channel closed
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
    UtAssert_EventSent(TO_PQUEUE_INFO_ERR_EID, 
                       CFE_EVS_ERROR, "", "PQueueIdx exceeds the largest available priority queue index");
}

/**
 * Test TO_ProcessNewAppCmds(), QueryOutputChannel command, ChannelID invalid (out of range)
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
            "Invalid channel index");
}

/**
 * Test TO_ProcessNewAppCmds Send diag command Fail ChannelID invalid (out of range)
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
    UtAssert_EventSent(TO_CMD_SEND_DIAG_ERR_EID,
                                 CFE_EVS_ERROR, "", "Invalid channel index");
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

    UtAssert_EventSent(TO_CMD_SEND_DIAG_ERR_EID,
                                 CFE_EVS_ERROR, "", "Channel has not been opened.");
                                 
}


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
                                          "Channel Not Open");
}

/**
 * Test TO_ProcessNewAppCmds(), QueryPriorityQueue command, Nominal
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
                                          "Channel Not Open");
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
    uint16                      ChannelIdx = 0;
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
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX, -1, 1);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested */
    TO_AppMain();

    /* Verify results */
    UtAssert_EventSent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR, "", "Remove Message Flow Unsubscribe Fail");
}

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
                                          "Channel has not been opened..");
}

/**************************************************************************
 * Tests for TO_RcvMsg
 **************************************************************************/
void Test_TO_RcvMsg_noMsg(void) 
{
    int32 result = -99;
    char outputText[200];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SUCCESS (%ld), actual = %ld",
           CFE_SUCCESS, result);

    UtAssert_True(CFE_SUCCESS == result, outputText);
}

void Test_TO_RcvMsg_recieveTimeout(void) 
{
    int32 result = -99;
    char outputText[200];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_TIME_OUT, 1);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SUCCESS (%ld), actual = %ld",
           CFE_SUCCESS, result);

    UtAssert_True(CFE_SUCCESS == result, outputText);
}


void Test_TO_RcvMsg_receiveError(void) 
{
    char outputText[200];

    /* Implementation doesn't distinguish between error types except on return type.
     * The call to CFE_EVS_SendEvent records the same event type regardless.  Is that sufficient? */
    int32 result = -99;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 1);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SB_BAD_ARGUMENT (%ld), actual = %ld",
           CFE_SB_BAD_ARGUMENT, result);

    UtAssert_True(CFE_SB_BAD_ARGUMENT == result, outputText);


    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SB_PIPE_RD_ERR (%ld), actual = %ld",
            CFE_SB_PIPE_RD_ERR, result);

    UtAssert_True(CFE_SB_PIPE_RD_ERR == result, outputText);
}

void Test_TO_RcvMsg_unknownMsgId(void) 
{
    int32 result = -99;
    char outputText[200];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_CMD_MID, 1);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SUCCESS (%ld), actual = %ld",
           CFE_SUCCESS, result);

    UtAssert_True(CFE_SUCCESS == result, outputText);
}


void Test_TO_RcvMsg_sendTlmMsgId(void) 
{
    int32 result = -99;
    char outputText[200];

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_SendMsg_TO_RcvMsgHook);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_TLM_MID, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SUCCESS (%ld), actual = %ld",
           CFE_SUCCESS, result);

    UtAssert_True(CFE_SUCCESS == result, outputText);
}


void Test_TO_RcvMsg_sendHkMsgId(void) 
{
    int32 result = -99;
    TO_test_sendMsg_msgPtr = NULL;
    char outputText[200];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_HK_MID, 1);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, CFE_SB_SendMsg_TO_RcvMsgHook);

    result = TO_RcvMsg(TO_SCH_PIPE_PEND_TIME);

    sprintf(outputText, "expected = CFE_SUCCESS (%ld), actual = %ld",
           CFE_SUCCESS, result);

    UtAssert_True(CFE_SUCCESS == result, outputText);

    sprintf(outputText, "expected = %p, actual = %p",
            &TO_AppData.HkTlm, TO_test_sendMsg_msgPtr);

    UtAssert_True((void *)TO_test_sendMsg_msgPtr == (void *)&TO_AppData.HkTlm, outputText);
}

void Test_TO_VerifyCmdLength_MsgPtr_Null(void) 
{
    osalbool result = TRUE;
    osalbool expected = FALSE;
    CFE_SB_Msg_t* Cmd_msgPtr = NULL;
    uint16 expectedLen = 42;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, TO_SEND_HK_MID, 1);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, CFE_SB_SendMsg_TO_RcvMsgHook);

    result = TO_VerifyCmdLength(Cmd_msgPtr, expectedLen);
    
    /* Verify results */
    UtAssert_True (result == expected, "Channel has not been opened.");
}





/**
 * Test TO_ProcessNewConfigTbl(), Nominal
 */
void Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}




/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void TO_App_Test_AddTestCases(void)
{
    /* Test for TO_InitEvent() */
    UtTest_Add(Test_TO_InitEvent_Fail_Register, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitEvent_Fail_Register");

    UtTest_Add(Test_TO_InitEvent_Fail_Custom_Bad_Ind, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitEvent_Fail_Custom_Bad_Ind");

    UtTest_Add(Test_TO_InitEvent_Fail_Custom_Index_Occupied, TO_Test_Setup_NoConfig, TO_Test_TearDown,
                "Test_TO_InitEvent_Fail_Custom_Index_Occupied");

    UtTest_Add(Test_TO_InitEvent_Fail_Custom_Too_Many, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitEvent_Fail_Custom_Too_Many");

    UtTest_Add(Test_TO_InitEvent_Fail_Custom_Unknown, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitEvent_Fail_Custom_Unknown");

    /* Test for TO_InitPipe() */
    UtTest_Add(Test_TO_InitPipe_Fail_CreateSCHPipe, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_TO_InitPipe_Fail_SubscribeWakeup, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_TO_InitPipe_Fail_SubscribeSendHK, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_TO_InitPipe_Fail_CreateCMDPipe, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_TO_InitPipe_Fail_SubscribeCMD, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitPipe_Fail_SubscribeCMD");

    /**** Tests for TO_InitData() */
    UtTest_Add(Test_TO_InitData, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitData");

    UtTest_Add(Test_TO_InitData_FailAppMutSemCreate, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitData Fail App MutSemCreate");

    /**** Tests for TO_InitApp() */
    UtTest_Add(Test_TO_InitApp_Fail_InitEvent, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Fail_InitEvent");
    UtTest_Add(Test_TO_InitApp_Fail_InitPipe, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Fail_InitPipe");
    UtTest_Add(Test_TO_InitApp_Fail_InitData, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Fail_InitData");
    UtTest_Add(Test_TO_InitApp_Fail_ChannelInitAll, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Fail_ChannelInitAll");               
    UtTest_Add(Test_TO_InitApp_Fail_CustomInit, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Fail_CustomInit");
    UtTest_Add(Test_TO_InitApp_Nominal, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_InitApp_Nominal");

    /**** Tests for AppMain() */
    UtTest_Add(Test_TO_AppMain_Fail_RegisterApp, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_TO_AppMain_Fail_InitApp, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_Fail_InitApp");
    UtTest_Add(Test_TO_AppMain_InvalidSchMessage, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_InvalidSchMessage");
    UtTest_Add(Test_TO_AppMain_Nominal_SendHK, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_Nominal_SendHK");
    UtTest_Add(Test_TO_AppMain_Nominal_Wakeup, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_TO_AppMain_ProcessNewData_InvalidMsgID, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_ProcessNewData_InvalidMsgID");
    UtTest_Add(Test_TO_AppMain_Fail_RcvMsg, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_AppMain_Fail_RcvMsg");                

    /**** Tests for TO_ProcessNewCmds() */
    UtTest_Add(Test_TO_ProcessNewCmds_RcvMsgErr, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewCmds_RcvMsgErr");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Noop_InvalidSize, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Noop_InvalidSize");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Noop_Nominal, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Noop_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Reset_Nominal, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Reset_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_Reset_InvalidCmdLength, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Reset_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidPQueueIdx, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidPQueueIdx");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_AlreadyDefined, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_AlreadyDefined");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidCmdLength, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_InvalidCmdLength");
//    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Over, TO_Test_Setup_FullConfig7, TO_Test_TearDown,
//               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Over");
//    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Full, TO_Test_Setup_FullConfig6, TO_Test_TearDown,
//               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Full");
    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_InvalidCmdLength, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_InvalidCmdLength, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Nominal, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_InvalidCmdLength, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_Nominal, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_Nominal");
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_InvalidCmdLength, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_InvalidCmdLength");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Success, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Success");
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_InvalidCmdLength, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_InvalidCmdLength");

    /* Traffic shaping algorithm */
    UtTest_Add(Test_TO_AppMain_ProcessTelemetry_PriorityPreemption1, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_AppMain_ProcessTelemetry_PriorityPreemption1");
    UtTest_Add(Test_TO_AppMain_ProcessTelemetry_PriorityPreemption2, TO_Test_Setup_FullConfig2, TO_Test_TearDown,
               "Test_TO_AppMain_ProcessTelemetry_PriorityPreemption2");
    UtTest_Add(Test_TO_AppMain_ProcessTelemetry_PriorityPreemption3, TO_Test_Setup_FullConfig2, TO_Test_TearDown,
               "Test_TO_AppMain_ProcessTelemetry_PriorityPreemption3");
              
    /* Tests for checking TO NULL Pointers */
    UtTest_Add(Test_TO_NULL_Ptr_check, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check");          

    UtTest_Add(Test_TO_NULL_Ptr_check_PQ_buildup, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check_PQ_buildup");

    UtTest_Add(Test_TO_NULL_Ptr_check_PQ_buildup_channel_ptr_null, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check_PQ_buildup_channel_ptr_null");
                             
    UtTest_Add(Test_TO_NULL_Ptr_check1, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check1");
               
    UtTest_Add(Test_TO_NULL_Ptr_check2, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check2");
               
    UtTest_Add(Test_TO_NULL_Ptr_check_Scheduler, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_NULL_Ptr_check_Scheduler");
               
    /**** Tests for TO Output Queue */
    UtTest_Add(Test_TO_Output_Queue_Buildup_QueueCreate_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_Output_Queue_Buildup_QueueCreate_Fail");    
    UtTest_Add(Test_TO_OutputQueue_QueueMsg_QueuePut, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_QueueMsg_QueuePut");
               
    UtTest_Add(Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Fail");
               
    UtTest_Add(Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Success, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_QueueMsg_QueuePut_PutPoolBuf_Success");
               
    UtTest_Add(Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Fail");
               
    UtTest_Add(Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Success, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_Teardown_QueueGet_PutPoolBuf_Success");
               
    UtTest_Add(Test_TO_OutputQueue_Teardown_QueueGet_Queue_Empty, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_OutputQueue_Teardown_QueueGet_Queue_Empty");                 

    /**** Tests for TO Message Flow */
    UtTest_Add(Test_TO_MessageFlow_TeardownAll_SubscribeEx, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_TeardownAll_SubscribeEx");
                   
    UtTest_Add(Test_TO_MessageFlow_TeardownAll_Unsubscribe, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_TeardownAll_Unsubscribe");
                         
    UtTest_Add(Test_TO_MessageFlow_GetObject_ConfigTblPtr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetObject_ConfigTblPtr_NULL");

    UtTest_Add(Test_TO_MessageFlow_GetObject_Channel_Ptr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetObject_Channel_Ptr_NULL");

    UtTest_Add(Test_TO_MessageFlow_GetPQueue_MsgFlow_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetPQueue_MsgFlow_NULL");
               
    UtTest_Add(Test_TO_MessageFlow_GetPQueue_ConfigTblPtr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetPQueue_ConfigTblPtr_NULL");
               
    UtTest_Add(Test_TO_MessageFlow_GetPQueue_ChannelPtr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetPQueue_ChannelPtr_NULL");
               
    UtTest_Add(Test_TO_MessageFlow_GetPQueue_Idx_Max_Priority_Queues, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetPQueue_Idx_Max_Priority_Queues");
               
    UtTest_Add(Test_TO_MessageFlow_GetPQueue_Idx_Null, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_GetPQueue_Idx_Null");

    UtTest_Add(Test_TO_MessageFlow_Reset_All_Counts_Channel_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_MessageFlow_Reset_All_Counts_Channel_NULL");               

    /**** Tests for TO Priority Queue */
    UtTest_Add(Test_TO_Priority_Queue_Buildup_QueueCreate_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_Priority_Queue_Buildup_QueueCreate_Fail");

    UtTest_Add(Test_TO_Priority_Queue_Buildup_NoPQueues, TO_Test_Setup_NoPQueueConfig, TO_Test_TearDown,
               "Test_TO_Priority_Queue_Buildup_NoPQueues");
                
    UtTest_Add(Test_TO_Priority_Queue_Teardown_QueueGet_PutPoolBuf_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_Priority_Queue_Teardown_QueueGet_PutPoolBuf_Fail");

    UtTest_Add(Test_TO_Priority_Queue_Teardown_OSALQueueIDInvalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_Priority_Queue_Teardown_OSALQueueIDInvalid");
               
    UtTest_Add(Test_TO_Priority_Queue_Teardown_QueueDelete_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_Priority_Queue_Teardown_QueueDelete_Fail");
                                 
    UtTest_Add(Test_TO_PriorityQueue_QueueMsg_ConfigTblPtr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_QueueMsg_ConfigTblPtr_NULL");

    UtTest_Add(Test_TO_PriorityQueue_QueueMsg_Nominal, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_QueueMsg_Nominal");

    UtTest_Add(Test_TO_PriorityQueue_QueueMsg_Memory_Full_Error, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_QueueMsg_Memory_Full_Error");
               
    UtTest_Add(Test_TO_PriorityQueue_QueueMsg_Queue_Full, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_QueueMsg_Queue_Full");
               
    UtTest_Add(Test_TO_PriorityQueue_QueueMsg_QueuePut_Error, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_QueueMsg_QueuePut_Error");
               
    UtTest_Add(Test_TO_PriorityQueue_IsValid_Null_ConfigTblPtr_Check, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_IsValid_Null_ConfigTblPtr_Check");               
               
    UtTest_Add(Test_TO_PriorityQueue_IsValid_Null_Ptr_Check, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_IsValid_Null_Ptr_Check");
               
    UtTest_Add(Test_TO_PriorityQueue_IsValid_PQ_State_Check, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_PriorityQueue_IsValid_PQ_State_Check");                

    /**** Tests TO Config Utils (Table validation, Managing Tables */
    UtTest_Add(Test_TO_InitTables_Ground_Table_Fail_TBL_Register, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_InitTables_Ground_Table_Fail_TBL_Register");
              
    UtTest_Add(Test_TO_InitTables_Dump_Table_Fail_TBL_Register, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_InitTables_Dump_Table_Fail_TBL_Register");
              
    UtTest_Add(Test_TO_InitTables_Dump_Table_Fail_TBL_Load, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_InitTables_Dump_Table_Fail_TBL_Load");
              
    UtTest_Add(Test_TO_InitTables_Load_Backup_Table, TO_Test_Setup_FullConfig1, TO_Test_TearDown,                                                        
              "Test_TO_InitTables_Load_Backup_Table");
               
    UtTest_Add(Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL");

    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure");

    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure");

    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure");

    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max");
              
//    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
//              "Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States");
              
    UtTest_Add(Test_TO_ValidateConfigTbl_Secondary_Header_Absent, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_Secondary_Header_Absent");
              
    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range");

    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks");
                           
    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid");

//    UtTest_Add(Test_TO_ValidateConfigTbl_Ground_Tbl_MessageFlow_Queue_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
//              "Test_TO_ValidateConfigTbl_Ground_Tbl_MessageFlow_Queue_Invalid");

//    UtTest_Add(Test_TO_ValidateConfigTbl1, TO_Test_Setup_FullConfig4, TO_Test_TearDown, 
//               "Test_TO_ValidateConfigTbl1");

//    UtTest_Add(Test_TO_ValidateConfigTbl2, TO_Test_Setup_FullConfig5, TO_Test_TearDown, 
//               "Test_TO_ValidateConfigTbl2");

//    UtTest_Add(Test_TO_ValidateConfigTbl_InvalidCCSDSVersion, TO_Test_Setup_FullConfig4, TO_Test_TearDown,
//               "Test_TO_ValidateConfigTbl_InvalidCCSDSVersion");

    UtTest_Add(Test_TO_ManageChannelTables_GetStatus_Error, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetStatus_Error");                               

    UtTest_Add(Test_TO_ManageChannelTables_GetStatus_Validate_Pending, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetStatus_Validate_Pending");

    UtTest_Add(Test_TO_ManageChannelTables_GetStatus_Update_Pending, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetStatus_Update_Pending");

    UtTest_Add(Test_TO_ManageChannelTables_GetAddress_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetAddress_Failure");

    UtTest_Add(Test_TO_ManageChannelTables_GetStatus_Update_Pending_Fail_Dequeue, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetStatus_Update_Pending_Fail_Dequeue");
              
    UtTest_Add(Test_TO_ManageChannelTables_GetAddress_Process_Config_Tbl_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ManageChannelTables_GetAddress_Process_Config_Tbl_Failure");

    /* Additional tests for TO Channel */
    UtTest_Add(Test_TO_Channel_SBPipe_Dequeue_All, TO_Test_Setup_NoConfig, TO_Test_TearDown, 
               "Test_TO_Channel_SBPipe_Dequeue_All");
    
    UtTest_Add(Test_TO_Channel_SBPipe_Dequeue_All_Continue, TO_Test_Setup_NoConfig, TO_Test_TearDown, 
               "Test_TO_Channel_SBPipe_Dequeue_All_Continue");
    
    UtTest_Add(Test_TO_Channel_State, TO_Test_Setup_EmptyConfig, TO_Test_TearDown, 
               "Test_TO_Channel_State");
    
    UtTest_Add(Test_TO_Channel_State_Fail, TO_Test_Setup_EmptyConfig, TO_Test_TearDown, 
               "Test_TO_Channel_State_Fail");
    
    UtTest_Add(TO_Channel_OpenChannel_Fail_Invalid_ChannelID, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "TO_Channel_OpenChannel_Fail_Invalid_ChannelID");

    UtTest_Add(TO_Channel_OpenChannel_Fail_ChannelAlreadyOpen, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "TO_Channel_OpenChannel_Fail_ChannelAlreadyOpen");

    UtTest_Add(Test_TO_Channel_Open_OS_CountSemCreate_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Open_OS_CountSemCreate_Failure");         
              
    UtTest_Add(Test_TO_Channel_Open_OutputQueue_Buildup_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Open_OutputQueue_Buildup_Failure");
              
    UtTest_Add(Test_TO_Channel_Open_CFE_SB_CreatePipe_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Open_CFE_SB_CreatePipe_Failure");
              
    UtTest_Add(Test_TO_Channel_Open_TO_InitTables_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Open_TO_InitTables_Failure");
              
    UtTest_Add(Test_TO_Channel_Open_PoolCreateEx_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Open_PoolCreateEx_Failure");
              
    UtTest_Add(Test_TO_Channel_LockByIndex_Index_Out_Of_Range, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_LockByIndex_Index_Out_Of_Range");
              
    UtTest_Add(Test_TO_Channel_UnlockByIndex_Index_Out_Of_Range, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_UnlockByIndex_Index_Out_Of_Range");
              
    UtTest_Add(Test_TO_Channel_LockByRef_NullPtr, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_LockByRef_NullPtr");

    UtTest_Add(Test_TO_Channel_UnlockByRef_NullPtr, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_UnlockByRef_NullPtr");

    UtTest_Add(Test_TO_Channel_LockByRef_Mutex_Take_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_LockByRef_Mutex_Take_Failure");
              
    UtTest_Add(Test_TO_Channel_UnlockByRef_Mutex_Give_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_UnlockByRef_Mutex_Give_Failure");
              
    UtTest_Add(Test_TO_Channel_InitAll_Mutex_Create_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_InitAll_Mutex_Create_Failure");
              
    UtTest_Add(Test_TO_Channel_CleanupAll_PriorityQueue_TeardownAll_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_CleanupAll_PriorityQueue_TeardownAll_Failure");
              
    UtTest_Add(Test_TO_Channel_CleanupAll_OutputQueue_Teardown_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_CleanupAll_OutputQueue_Teardown_Failure");                         
              
    UtTest_Add(Test_TO_Channel_Cleanup_Index_Out_Of_Range_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Cleanup_Index_Out_Of_Range_Failure");
              
    UtTest_Add(Test_TO_Channel_Init_Index_Out_Of_Range_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Channel_Init_Index_Out_Of_Range_Failure");
              
    /**** Tests for TO_Classifier_Run */
    UtTest_Add(Test_TO_Classifier_NoMessage, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_Classifier_NoMessage");

    UtTest_Add(Test_TO_Classifier_MsgNullPtr, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_Classifier_MsgNullPtr");

    UtTest_Add(Test_TO_Classifier_MaxMessages, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_Classifier_MaxMessages");

    UtTest_Add(Test_TO_Classifier_MessageTooLong, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_Classifier_MessageTooLong");

    UtTest_Add(Test_TO_Classifier_NoMsgFlow, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_Classifier_NoMsgFlow");

    UtTest_Add(Test_TO_Classifier_Message_pqueue_NULL, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Classifier_Message_pqueue_NULL");
              
    UtTest_Add(Test_TO_Classifier_Message_HighwaterMark, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Classifier_Message_HighwaterMark");                           
            
    /**** Tests for TO_Scheduler_Run */        
    UtTest_Add(Test_TO_Scheduler_Run_OS_Max_Queues, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Scheduler_Run_OS_Max_Queues");
                          
    UtTest_Add(Test_TO_Scheduler_Run_QueueMsg_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Scheduler_Run_QueueMsg_Fail");
              
    UtTest_Add(Test_TO_Scheduler_Run_HighwaterMark, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_Scheduler_Run_HighwaterMark"); 
              
    UtTest_Add(Test_TO_ReportHousekeeping_Ground, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ReportHousekeeping_Ground");
              
    UtTest_Add(Test_TO_ReportHousekeeping_UnknownChannelID, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ReportHousekeeping_UnknownChannelID");

    UtTest_Add(Test_TO_SendDiag_Ground, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_SendDiag_Ground");              

    /* Additional tests for TO_ProcessNewCmds */
    UtTest_Add(Test_TO_ProcessNewAppCmds_Default, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ProcessNewAppCmds_Default");

    UtTest_Add(Test_TO_ProcessNewAppCmds_MsgPtr_Null, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
              "Test_TO_ProcessNewAppCmds_MsgPtr_Null");
               
//    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid, TO_Test_Setup_FullConfig3, TO_Test_TearDown,
//               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Invalid");
               
//    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr, TO_Test_Setup_FullConfig3, TO_Test_TearDown,
//               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Channel_Null_ConfigTblPtr");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_SubscribeEx_Failure");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_AddMessageFlow_Fail_Channel_Not_Open, TO_Test_Setup_EmptyConfig, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_AddMessageFlow_Fail_Channel_Not_Open"); 
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_That_Does_Not_Exist, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_That_Does_Not_Exist");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_Channel_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_Channel_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_MessageFlow_Remove_MsgId_Channel_Not_Open");                         
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_MsgFlow_Not_Defined, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_MsgFlow_Not_Defined");
                                                                                                         
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Channel_Index_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Channel_Index_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_PQ_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_PQ_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryOutputChannel_Fail_ChannelID_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryOutputChannel_Fail_ChannelID_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Fail_ChannelID_Invalid, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Fail_ChannelID_Invalid");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_SendDiag_Fail_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_SendDiag_Fail_Channel_Not_Open");
               
//    UtTest_Add(Test_TO_ProcessNewAppCmds_Modify_Message_Flow_Fail_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
//               "Test_TO_ProcessNewAppCmds_Modify_Message_Flow_Fail_Channel_Not_Open");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryMessageFlow_Nominal_Channel_Not_Open");
               
    UtTest_Add(Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Fail_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_QueryPriorityQueue_Fail_Channel_Not_Open");

    UtTest_Add(Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_RemoveMessageFlow_Nominal_Unsubscribe_Fail");

    UtTest_Add(Test_TO_ProcessNewAppCmds_Output_Channel_Query_Channel_Not_Open, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewAppCmds_Output_Channel_Query_Channel_Not_Open");

    UtTest_Add(Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail");

    /* TO_RcvMsg tests */
    UtTest_Add(Test_TO_RcvMsg_noMsg, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_noMsg");
    UtTest_Add(Test_TO_RcvMsg_recieveTimeout, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_receiveTimeout");
    UtTest_Add(Test_TO_RcvMsg_receiveError, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_receiveError");
    UtTest_Add(Test_TO_RcvMsg_unknownMsgId, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_unknownMsgId");
    UtTest_Add(Test_TO_RcvMsg_sendTlmMsgId, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_sendTlmMsgId");
    UtTest_Add(Test_TO_RcvMsg_sendHkMsgId, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_RcvMsg_sendHkMsgId");
               
    /* Test TO_VerifyCmdLength */
    UtTest_Add(Test_TO_VerifyCmdLength_MsgPtr_Null, TO_Test_Setup_NoConfig, TO_Test_TearDown,
               "Test_TO_VerifyCmdLength_MsgPtr_Null");                 
}

