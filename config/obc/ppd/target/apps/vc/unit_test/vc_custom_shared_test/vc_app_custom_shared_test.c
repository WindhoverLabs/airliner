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

#include "vc_app_custom_shared_test.h"
#include "vc_custom_shared_test_utils.h"
#include "vc_platform_cfg.h"
#include "vc_transmit_udp.h"
#include "vc_msgids.h"
#include "vc_platform_stubs.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

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

/**************************************************************************
 * Tests VC_ProcessNewCustomCmds()
 **************************************************************************/
/**
 * Test VC_ProcessNewCustomCmds() - InvalidMsgID
 */
void Test_VC_Custom_ProcessNewCustomCmds_InvalidMsgID(void)
{
    VC_StartStreamCmd_t InMsg;

    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(VC_StartStreamCmd_t), TRUE);
    
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);

    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InMsg);

    /* Verify results */
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_EventSent(VC_MSGID_ERR_EID, CFE_EVS_ERROR, "", 
            "VC_ProcessNewCustomCmds() failed to raise an event");
}


/**
 * Test VC_ProcessNewCustomCmds() - Invalid Command Code
 */
void Test_VC_Custom_ProcessNewCustomCmds_InvalidCommand(void)
{
    /* Create an invalid command code */
    uint8 InvalidCommandCode = 100;
    
    VC_StartStreamCmd_t InMsg;
    
    CFE_SB_InitMsg (&InMsg, VC_STARTSTREAMING_CC, sizeof(VC_StartStreamCmd_t), TRUE);
    
    /* Set invalid command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, InvalidCommandCode);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InMsg);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_MSGID_ERR_EID, CFE_EVS_ERROR, "", 
            "Cmd with Invalid Cmd Code Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, Invalid Size
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidSize(void)
{
    /* Command with the wrong type (size) */
    VC_NoArgCmd_t InMsg;
    
    CFE_SB_InitMsg (&InMsg, VC_CMD_MID, sizeof(InMsg), TRUE);
    
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, VC_STARTSTREAMING_CC);

    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InMsg);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_MSGLEN_ERR_EID, CFE_EVS_ERROR, "", 
            "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, Invalid State
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidState(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;

    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);

    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to streaming */
    VC_AppData.AppState = VC_STREAMING;
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_CMD_ERR_EID, CFE_EVS_ERROR, "VC is already streaming", "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, Invalid (Null) Address
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidNullAddress(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;
    
    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_ADDR_NUL_ERR_EID, CFE_EVS_ERROR, 
            "NUL (empty) string specified for address", "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, Invalid Address
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidAddress(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;
    
    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Start streaming needs an address to pass null check */
    strcpy(InStartStreamingCmd.Address, "NOT_NULL");

    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_ADDR_ERR_EID, CFE_EVS_ERROR, 
            "Invalid string specified for address", "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, destination
 * update failure
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_UpdateDestinationFail(void)
{
    /* Currently no way to fail update destination. */
    //VC_StartStreamCmd_t InStartStreamingCmd;

    //CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    //CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    ///* Set get command code function hook */
    //Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    ///* Set app state to initialized */
    //VC_AppData.AppState = VC_INITIALIZED;
    
    ///* Start streaming needs an address to pass null check */
    //strcpy(InStartStreamingCmd.Address, "1.1.1.1");

    ///* Call the function under test */
    //VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);
    
    ///* Verify results */
    //UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    //UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");    
    //UtAssert_EventSent(VC_INIT_ERR_EID, CFE_EVS_ERROR, "Destination update failed", "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, transmit
 * uninit failure
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_TransmitUninitFail(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;

    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Start streaming needs an address to pass null check */
    strcpy(InStartStreamingCmd.Address, "1.1.1.1");
    
    /* Set a channel to enabled to pass VC_Update_Destination */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set close to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Close_Return = -1;

    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");    
}


/**
 * Test VC_ProcessNewCustomCmds(), StartStreaming command, devices
 * start failure
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_DevicesStartFail(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;

    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Start streaming needs an address to pass null check */
    strcpy(InStartStreamingCmd.Address, "1.1.1.1");
    
    /* Set source IP to a valid value to pass transmit init */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "1.1.1.1");
    
    /* Set a channel to enabled to pass VC_Update_Destination */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set status to initialized */
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Set device start child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);

    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_INIT_ERR_EID, CFE_EVS_ERROR, "VC_Devices_Start failed in cmd start streaming", "Device start failure did not raise event");
}


/**
 * Test VC_ProcessNewCustomCmds(), Start Streaming command, Nominal
 */
void Test_VC_ProcessNewCustomCmds_StartStreaming_Nominal(void)
{
    VC_StartStreamCmd_t InStartStreamingCmd;

    CFE_SB_InitMsg (&InStartStreamingCmd, VC_CMD_MID, sizeof(InStartStreamingCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartStreamingCmd, VC_STARTSTREAMING_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to initialized */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Start streaming needs an address to pass null check */
    strcpy(InStartStreamingCmd.Address, "1.1.1.1");
    
    /* Set source IP to a valid value to pass transmit init */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "1.1.1.1");
    
    /* Set a channel to enabled to pass VC_Update_Destination */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set status to initialized */
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;

    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InStartStreamingCmd);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdCnt = 1,"Command counter != 1");
    UtAssert_EventSent(VC_CMD_INF_EID, CFE_EVS_INFORMATION, "", "Start Streaming Cmd Event Sent");
    UtAssert_True(VC_AppData.AppState == VC_STREAMING, "App state != streaming");
}


/**
 * Test VC_ProcessNewCustomCmds(), StopStreaming command, Invalid Size
 */
void Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidSize(void)
{
    /* Command with the wrong type (size) */
    VC_StartStreamCmd_t InMsg;
    
    CFE_SB_InitMsg (&InMsg, VC_CMD_MID, sizeof(InMsg), TRUE);
    
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, VC_STOPSTREAMING_CC);

    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InMsg);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_MSGLEN_ERR_EID, CFE_EVS_ERROR, "", 
            "Start Streaming Cmd Event Sent");
}


/**
 * Test VC_ProcessNewCustomCmds(), StopStreaming command, Invalid State
 */
void Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidState(void)
{
    VC_NoArgCmd_t InMsg;

    CFE_SB_InitMsg (&InMsg, VC_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, VC_STOPSTREAMING_CC);

    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set app state to streaming */
    VC_AppData.AppState = VC_INITIALIZED;
    
    /* Call the function under test */
    VC_ProcessNewCustomCmds((CFE_SB_MsgPtr_t)&InMsg);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.HkTlm.usCmdErrCnt = 1,"Command error counter != 1");
    UtAssert_EventSent(VC_CMD_ERR_EID, CFE_EVS_ERROR, 
            "VC is already not streaming", "Stop Streaming Cmd Event Sent");
}


/**************************************************************************
 * Tests VC_Custom_Max_Events_Not_Reached()
 **************************************************************************/
/**
 * Test Test_VC_Custom_Max_Events_Not_Reached(), Nominal
 * 
 */
void Test_VC_Custom_Max_Events_Not_Reached_Nominal(void)
{
    UtAssert_True(VC_Custom_Max_Events_Not_Reached(VC_MAX_EVENT_FILTERS - 1) == TRUE,
            "Max events not reached did return true");
}


/**
 * Test Test_VC_Custom_Max_Events_Not_Reached(), Fail
 * 
 */
void Test_VC_Custom_Max_Events_Not_Reached_Fail(void)
{
    UtAssert_True(VC_Custom_Max_Events_Not_Reached(VC_MAX_EVENT_FILTERS + 1) == FALSE,
            "Max events not reached did not return false");
}


/**************************************************************************
 * Tests VC_Custom_Init_EventFilters()
 **************************************************************************/
/**
 * Test VC_Custom_Init_EventFilters(), Nominal
 */
void Test_VC_Custom_Init_EventFilters_Nominal(void)
{
    int32 returnCode = 0;
    int32 expected   = 2;

    CFE_EVS_BinFilter_t   EventTbl[VC_MAX_EVENT_FILTERS];
    
    returnCode = VC_Custom_Init_EventFilters(1, EventTbl);
    
    UtAssert_True(returnCode == expected,
            "Test_VC_Custom_Init_EventFilters_Nominal did not return the expected value");
    UtAssert_True(EventTbl[1].EventID == VC_SOCKET_ERR_EID,
            "Event filter array entries not added");
    UtAssert_True(EventTbl[2].EventID == VC_DEVICE_ERR_EID,
            "Event filter array entries not added");
}


/**
 * Test VC_Custom_Init_EventFilters(), Fail
 */
void Test_VC_Custom_Init_EventFilters_Fail1(void)
{
    int32 returnCode = 0;
    int32 expected   = -1;
    CFE_EVS_BinFilter_t   EventTbl[VC_MAX_EVENT_FILTERS];
    
    returnCode = VC_Custom_Init_EventFilters(VC_MAX_EVENT_FILTERS, EventTbl);
    
    UtAssert_True(returnCode == expected,
            "Test_VC_Custom_Init_EventFilters_Fail did not return the expected value");

}


/**
 * Test VC_Custom_Init_EventFilters(), Fail
 */
void Test_VC_Custom_Init_EventFilters_Fail2(void)
{
    int32 returnCode = 0;
    int32 expected   = -1;
    CFE_EVS_BinFilter_t   EventTbl[VC_MAX_EVENT_FILTERS];
    
    returnCode = VC_Custom_Init_EventFilters(VC_MAX_EVENT_FILTERS-1, EventTbl);
    
    UtAssert_True(returnCode == expected,
            "Test_VC_Custom_Init_EventFilters_Fail did not return the expected value");
    UtAssert_True(returnCode == expected,
            "Test_VC_Custom_Init_EventFilters_Nominal did not return the expected value");
    UtAssert_True(EventTbl[VC_MAX_EVENT_FILTERS-1].EventID == VC_SOCKET_ERR_EID,
            "Event filter array entries not added");

}


/**
 * Test VC_Custom_Init_EventFilters(), Fail null check
 */
void Test_VC_Custom_Init_EventFilters_Null(void)
{
    int32 returnCode = 0;
    int32 expected   = -1;
    
    returnCode = VC_Custom_Init_EventFilters(1, (CFE_EVS_BinFilter_t *)0);
    
    UtAssert_True(returnCode == expected,
            "Test_VC_Custom_Init_EventFilters_Fail did not return the expected value");
}

/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void VC_Custom_App_Shared_Test_AddTestCases(void)
{
/**************************************************************************
 * Tests for Custom Shared Layer
 **************************************************************************/
    UtTest_Add(Test_VC_Custom_ProcessNewCustomCmds_InvalidMsgID, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_ProcessNewCustomCmds_InvalidMsgID");
    UtTest_Add(Test_VC_Custom_ProcessNewCustomCmds_InvalidCommand, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_ProcessNewCustomCmds_InvalidCommand");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidSize, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidSize");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidState, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidState");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidNullAddress, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidNullAddress");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidAddress, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_InvalidAddress");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_UpdateDestinationFail, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_UpdateDestinationFail");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_TransmitUninitFail, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_TransmitUninitFail");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_DevicesStartFail, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_DevicesStartFail");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StartStreaming_Nominal, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StartStreaming_Nominal");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidSize, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidSize");
    UtTest_Add(Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidState, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_ProcessNewCustomCmds_StopStreaming_InvalidState");
    UtTest_Add(Test_VC_Custom_Max_Events_Not_Reached_Nominal, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Max_Events_Not_Reached_Nominal");
    UtTest_Add(Test_VC_Custom_Max_Events_Not_Reached_Fail, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Max_Events_Not_Reached_Fail");
    UtTest_Add(Test_VC_Custom_Init_EventFilters_Nominal, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Init_EventFilters_Nominal");
    UtTest_Add(Test_VC_Custom_Init_EventFilters_Fail1, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Init_EventFilters_Fail1");
    UtTest_Add(Test_VC_Custom_Init_EventFilters_Fail2, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Init_EventFilters_Fail2");
    UtTest_Add(Test_VC_Custom_Init_EventFilters_Null, 
            VC_Custom_Shared_Test_Setup, VC_Custom_Shared_Test_TearDown,
            "Test_VC_Custom_Init_EventFilters_Null");
}
