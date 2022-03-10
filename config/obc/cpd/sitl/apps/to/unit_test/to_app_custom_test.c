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

#include "to_app_custom_test.h"
#include "to_custom_test_utils.h"
#include "to_platform_stubs.h"

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
#include "to_custom.h"


/**************************************************************************
 * Tests for TO_Custom_Init()
 **************************************************************************/
/**
 * Test TO_Custom_Init() fail TO_Channel_OpenChannel()
 */
void TO_Custom_Init_OpenChannelFail(void)
{
    int32 result = 0;
    int32 expected = 1;
    
    TO_App_Return.TO_Channel_OpenChannel_Return = 1;
    
    /* Execute the function being tested */
    result = TO_Custom_Init();
    
    /* Verify results */
    UtAssert_True (result == expected, 
                "TO_Custom_Init() did not return an expected value");
}


/**
 * Test TO_Custom_Init() fail TO_OutputChannel_Enable()
 */
void TO_Custom_Init_EnableChannelFail(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 ChannelID = 0;
    
    /* Set channel 0 to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set socket call in TO_OutputChannel_Enable to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = -1;
    
    /* Execute the function being tested */
    result = TO_Custom_Init();
    
    /* Verify results */
    UtAssert_True (result == expected, 
                "TO_Custom_Init() did not return an expected value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_DISABLED, 
                "TO_Custom_Init() did not set correct mode");
}


/**
 * Test TO_Custom_Init() nothing enabled
 */
void TO_Custom_Init_NothingEnabled(void)
{
    int32 result = -1;
    int32 expected = 0;

    /* Execute the function being tested */
    result = TO_Custom_Init();
    
    /* Verify results */
    UtAssert_True (result == expected, 
                "TO_Custom_Init() did not return an expected value");
}


/**
 * Test TO_Custom_Init() nominal
 */
void TO_Custom_Init_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 ChannelID = 0;
    uint16 testPort = TO_UDP_CHANNEL_PORT;
    char *testIP = TO_UDP_CHANNEL_ADDRESS;
    
    char returnString[128];
    snprintf(returnString, 128, "UDP telemetry output enabled channel %u to %s:%u", 
            ChannelID, testIP, testPort);
    
    /* Execute the function being tested */
    result = TO_Custom_Init();
    
    /* Verify results */
    UtAssert_True (result == expected, 
                "TO_Custom_Init() did not return an expected value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_ENABLED, 
                "TO_Custom_Init() did not set correct mode");
    //UtAssert_EventSent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION, returnString, 
    //            "TO_Custom_Init() failed to raise an event");
}


/**************************************************************************
 * Tests for TO_OutputChannel_CustomBuildupAll()
 **************************************************************************/
/**
 * Test TO_OutputChannel_CustomBuildupAll() nominal
 * NOTE: Function is not currently implemented.
 */
void Test_TO_OutputChannel_CustomBuildupAll_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint32 index = 0;
    
    /* Execute the function being tested */
    result = TO_OutputChannel_CustomBuildupAll(index);
    
    /* Verify results */
    UtAssert_True (result == expected, 
                "TO_OutputChannel_CustomBuildupAll() failed nominal");
}


/**************************************************************************
 * Tests for TO_OutputChannel_Enable()
 **************************************************************************/
/**
 * Test TO_OutputChannel_Enable() null destination address
 */
void Test_TO_OutputChannel_Enable_NullDestinationAddress(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 ChannelID = 0;
    /* Null destination address */
    char *DestinationAddress = 0;
    uint16 DestinationPort = 0;
    
    char returnString[128];
    snprintf(returnString, 128, "Destination address for channel %u is null.", 0);
    
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_EventSent(TO_TLMOUTENA_ERR_EID, CFE_EVS_ERROR, returnString, 
                "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Enable() invalid channel id
 */
void Test_TO_OutputChannel_Enable_InvalidID(void)
{
    int32 result = 0;
    int32 expected = -1;
    /* Set invalid channel ID */
    uint8 ChannelID = (TO_MAX_CHANNELS);
    /* Set test destination address */
    char *DestinationAddress = "test";
    uint16 DestinationPort = 0;
    
    char returnString[128];
    snprintf(returnString, 128, "ChannelID (%u) invalid.", (TO_MAX_CHANNELS));
    
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_EventSent(TO_TLMOUTENA_ERR_EID, CFE_EVS_ERROR, returnString, 
                "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Enable() fail socket creation
 */
void Test_TO_OutputChannel_Enable_SocketFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    int32 expectedErrno = 5;
    uint8 ChannelID = 0;
    /* Set test destination address */
    char *DestinationAddress = "test";
    uint16 DestinationPort = 0;
    
    char returnString[128];
    snprintf(returnString, 128, "TLM socket errno: %i on channel %u", expectedErrno, ChannelID);
    
    /* Set socket call to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = -1;
    
    /* Set errno for socket call */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Errno = 1;
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Errno_Value = expectedErrno;
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Socket == -1,
                "TO_OutputChannel_Enable() failed to return an error");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_DISABLED, 
                "TO_OutputChannel_Enable() did not set correct mode");
    UtAssert_EventSent(TO_TLMOUTSOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Enable() fail bind
 */
void Test_TO_OutputChannel_Enable_BindFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    int32 expectedErrno = 5;
    uint8 ChannelID = 0;
    /* Set test destination address */
    char *DestinationAddress = "test";
    uint16 DestinationPort = 0;
    
    char returnString[128];
    snprintf(returnString, 128, "TLM bind errno: %i on channel %u", expectedErrno, ChannelID);
    
    /* Set socket call to pass */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = 1;
    
    /* Set bind call to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_Bind_Return = -1;
    
    /* Set errno for bind call */
    TO_Platform_Stubs_Returns.TO_Wrap_Bind_Errno = 1;
    TO_Platform_Stubs_Returns.TO_Wrap_Bind_Errno_Value = expectedErrno;
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_DISABLED, 
                "TO_OutputChannel_Enable() did not set correct mode");
    UtAssert_EventSent(TO_TLMOUTSOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Enable() fail create child task
 */
void Test_TO_OutputChannel_Enable_CreateChildTaskFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 ChannelID = 0;
    /* Set test destination address */
    char *DestinationAddress = "test";
    uint16 DestinationPort = 0;
    
    /* Set create child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);
    
    /* Set socket call to pass */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = 1;
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_ENABLED, 
                "TO_OutputChannel_Enable() did not set correct mode");
}


/**
 * Test TO_OutputChannel_Enable() nominal
 */
void Test_TO_OutputChannel_Enable_Nominal(void)
{
    int32 result = -1;
    int32 expected = CFE_SUCCESS;
    uint8 ChannelID = 0;
    /* Set test destination address */
    char *DestinationAddress = "test";
    uint16 DestinationPort = 0;
    
    /* Set create child task to return success */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    /* Set socket call to pass */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = 1;
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Enable(ChannelID, DestinationAddress, DestinationPort);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_ENABLED, 
                "TO_OutputChannel_Enable() did not set correct mode");
}


/**************************************************************************
 * Tests for TO_OutputChannel_Send()
 **************************************************************************/
/**
 * Test TO_OutputChannel_Send() invalid channel id
 */
void Test_TO_OutputChannel_Send_InvalidID(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 ChannelID = (TO_MAX_CHANNELS);
    char *testBuffer = "test";
    
    /* Execute the function being tested */
    result = TO_OutputChannel_Send(ChannelID, testBuffer, sizeof(testBuffer));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
    UtAssert_True (result == expected, 
                "TO_OutputChannel_Enable() did not return the correct value");
}


/**
 * Test TO_OutputChannel_Send() sendto fail
 */
void Test_TO_OutputChannel_Send_SendToFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 ChannelID = 0;
    char *testBuffer = "test";
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set sendto call to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Return = -1;
    /* Set errno for sendto call */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno = 1;
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno_Value = 8;
    
    /* Call the function under test */
    result = TO_OutputChannel_Send(ChannelID, testBuffer, sizeof(testBuffer));

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_DISABLED, 
                "TO_OutputChannel_Send() did not set mode to disabled");
    UtAssert_True(result == expected,
                "TO_OutputChannel_Send() did not return the correct value");
    UtAssert_EventSent(TO_TLMOUTSTOP_ERR_EID, CFE_EVS_ERROR, "", 
                "TO_OutputChannel_Send() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Send() sendto fail message too long
 */
void Test_TO_OutputChannel_Send_SendToTooLong(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 ChannelID = 0;
    char *testBuffer = "test";
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set sendto call to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Return = -1;
    /* Set errno for sendto call */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno = 1;
    /* Set errno message too long */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Errno_Value = 90;
    
    /* Call the function under test */
    result = TO_OutputChannel_Send(ChannelID, testBuffer, sizeof(testBuffer));

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,
            "TO_OutputChannel_Send() did not return the correct value");
    UtAssert_EventSent(TO_TLMOUTSTOP_ERR_EID, CFE_EVS_ERROR, "", 
            "TO_OutputChannel_Send() failed to raise an event");
}


/**
 * Test TO_OutputChannel_Send() nominal
 */
void Test_TO_OutputChannel_Send_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 ChannelID = 0;
    char *testBuffer = "test";
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Call the function under test */
    result = TO_OutputChannel_Send(ChannelID, testBuffer, sizeof(testBuffer));

    UtAssert_True(result == expected,
            "TO_OutputChannel_Send() did not return the correct value");
}


 /**************************************************************************
 * Tests for TO_OutputChannel_ChannelHandler()
 **************************************************************************/
/**
 * Test TO_OutputChannel_ChannelHandler() fail OS_QueueGet
 */
void TO_OutputChannel_ChannelHandle_QueueGetFail(void)
{
    uint8 ChannelID = 0;
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set all status returns after the first call to disabled 
     * NOTE: calls to TO_Channel_State sets the channel mode to
     * TO_CHANNEL_DISABLED disabled after the first call
     */
    TO_App_Return.TO_Channel_State_Return = TO_CHANNEL_OPENED;
    TO_App_Return.TO_Channel_State_Return1 = TO_CHANNEL_CLOSED;
    
    /* Set OS_QueueGet to fail with OS_ERROR */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_ERROR, 1);
    
    /* Call the function under test */
    TO_OutputChannel_ChannelHandler(ChannelID);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_OSQUEUE_GET_ERROR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_Send() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ChannelHandler() fail sendto
 */
void TO_OutputChannel_ChannelHandle_SendToFail(void)
{
    uint8 ChannelID = 0;
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set sendto call to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_SendTo_Return = -1;
    
    /* Set all status returns after the first call to disabled 
     * NOTE: calls to TO_Channel_State sets the channel mode to
     * TO_CHANNEL_DISABLED disabled after the first call
     */
    TO_App_Return.TO_Channel_State_Return = TO_CHANNEL_OPENED;
    TO_App_Return.TO_Channel_State_Return1 = TO_CHANNEL_CLOSED;

    /* Call the function under test */
    TO_OutputChannel_ChannelHandler(ChannelID);
    
    /* Verify results */
    /* Events sendto error and disable channel info message */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(TO_TLMOUTSTOP_ERR_EID, CFE_EVS_ERROR, "", 
            "TO_OutputChannel_Send() failed to raise an event");
    UtAssert_EventSent(TO_CMD_INF_EID, CFE_EVS_INFORMATION, "", 
            "TO_OutputChannel_Send() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ChannelHandler() fail CFE_ES_PutPoolBuf()
 */
void TO_OutputChannel_ChannelHandle_PutPoolBufFail(void)
{
    uint8 ChannelID = 0;
    
    /* Set CFE_ES_PutPoolBuf to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX, -1, 1);
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set all status returns after the first call to disabled 
     * NOTE: calls to TO_Channel_State sets the channel mode to
     * TO_CHANNEL_DISABLED disabled after the first call
     */
    TO_App_Return.TO_Channel_State_Return = TO_CHANNEL_OPENED;
    TO_App_Return.TO_Channel_State_Return1 = TO_CHANNEL_CLOSED;

    /* Call the function under test */
    TO_OutputChannel_ChannelHandler(ChannelID);
    
    /* Verify results */
    /* Events sendto error and disable channel info message */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_PUT_POOL_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_Send() failed to raise an event");
}

/**
 * Test TO_OutputChannel_ChannelHandler() OS_QUEUE_TIMEOUT
 */
void TO_OutputChannel_ChannelHandle_OSQueueTimeout(void)
{
    uint8 ChannelID = 0;
    
    /* Set OS_QueueGet to fail with OS_QUEUE_TIMEOUT */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_QUEUEGET_INDEX, OS_QUEUE_TIMEOUT, 1);
    
    /* Set test channel to enabled */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Set all status returns after the first call to disabled 
     * NOTE: calls to TO_Channel_State sets the channel mode to
     * TO_CHANNEL_DISABLED disabled after the first call
     */
    TO_App_Return.TO_Channel_State_Return = TO_CHANNEL_OPENED;
    TO_App_Return.TO_Channel_State_Return1 = TO_CHANNEL_CLOSED;

    /* Call the function under test */
    TO_OutputChannel_ChannelHandler(ChannelID);
    
    /* Verify results */
    /* Events sendto error and disable channel info message */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
}


 /**************************************************************************
 * Tests for TO_OutputChannel_ProcessNewCustomCmds()
 **************************************************************************/
/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() invalid message length
 * enable channel command code
 */
void TO_OutputChannel_ProcessNewCustomCmds_EnableInvalidMessageLength(void)
{
    /* Wrong type to create incorrect size condition */
    TO_DisableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_ENABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(TO_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_ProcessNewCustomCmds() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() invalid message length
 * disable channel command code
 */
void TO_OutputChannel_ProcessNewCustomCmds_DisableInvalidMessageLength(void)
{
    /* Wrong type to create incorrect size condition */
    TO_EnableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_DISABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(TO_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_EventSent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_ProcessNewCustomCmds() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() invalid command code
 */
void TO_OutputChannel_ProcessNewCustomCmds_InvalidCommandCode(void)
{
    /* Create an invalid command code */
    uint8 InvalidCommandCode = 100;

    /* Wrong type to create incorrect size condition */
    TO_EnableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    /* Set invalid command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, InvalidCommandCode);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(TO_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_EventSent(TO_CC_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_ProcessNewCustomCmds() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() enable fail
 */
void TO_OutputChannel_ProcessNewCustomCmds_EnableFail(void)
{
    TO_EnableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    /* Set command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_ENABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Set socket create to fail */
    TO_Platform_Stubs_Returns.TO_Wrap_Socket_Return = -1;
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);
    
    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTSOCKET_ERR_EID, CFE_EVS_ERROR, "", 
            "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() disable fail
 */
void TO_OutputChannel_ProcessNewCustomCmds_DisableFail(void)
{
    TO_DisableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    /* Set command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_DISABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);
    
    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.usCmdErrCnt == 1,"CmdErrCnt not incremented");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTDIS_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_Disable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() enable nominal
 */
void TO_OutputChannel_ProcessNewCustomCmds_EnableNominal(void)
{
    TO_EnableChannelCmd_t InMsg;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    /* Set command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_ENABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);

    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.usCmdCnt == 1,"usCmdCnt not incremented");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION, "",
            "TO_OutputChannel_Enable() failed to raise an event");
}


/**
 * Test TO_OutputChannel_ProcessNewCustomCmds() disable nominal
 */
void TO_OutputChannel_ProcessNewCustomCmds_DisableNominal(void)
{
    uint8 ChannelID = 0;
    TO_DisableChannelCmd_t InMsg;
    
    /* Set to enabled for TO_OutputChannel_Disable can set to 
     * disabled 
     */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    CFE_SB_InitMsg(&InMsg, TO_CMD_MID, sizeof(InMsg), TRUE);

    /* Set command code */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InMsg, TO_DISABLE_CHANNEL_CC);
    
    /* Set get command code function hook */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    
    
    /* Call the function under test */
    TO_OutputChannel_ProcessNewCustomCmds((CFE_SB_Msg_t*)&InMsg);

    /* Verify results */
    UtAssert_True(TO_AppData.HkTlm.usCmdCnt == 1,"usCmdCnt not incremented");
    UtAssert_True(TO_AppCustomData.Channel[ChannelID].Mode == TO_CHANNEL_DISABLED,
            "TO_OutputChannel_Disable did not set correct mode");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTDIS_INF_EID, CFE_EVS_INFORMATION, "",
            "TO_OutputChannel_Disable() failed to raise an event");
}

 /**************************************************************************
 * Tests for TO_OutputChannel_UDPChannelTask()
 **************************************************************************/
/**
 * Test TO_OutputChannel_UDPChannelTask()
 * Note: currently there is no way to fail this function
 */
void TO_OutputChannel_UDPChannelTask_Nominal(void)
{
    TO_OutputChannel_UDPChannelTask();
}


 /**************************************************************************
 * Tests for TO_OutputChannel_CustomCleanupAll()
 **************************************************************************/
/**
 * Test TO_OutputChannel_CustomCleanupAll() nominal
 * Note: currently there is no way to fail this function
 */
void TO_OutputChannel_CustomCleanupAll_Nominal(void)
{
    uint8 ChannelID = 0;
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;

    /* Call the function under test */
    TO_OutputChannel_CustomCleanupAll();
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTDIS_INF_EID, CFE_EVS_INFORMATION, "",
            "TO_OutputChannel_Disable() failed to raise an event");
}


 /**************************************************************************
 * Tests for TO_OutputChannel_Disable()
 **************************************************************************/
 
/**
 * Test TO_OutputChannel_Disable() nominal
 */
void TO_OutputChannel_Disable_Fail(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 ChannelID = 0;
    
    /* Call the function under test */
    result = TO_OutputChannel_Disable(ChannelID);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTDIS_ERR_EID, CFE_EVS_ERROR, "",
            "TO_OutputChannel_Disable() failed to raise an event");
    UtAssert_True(result == expected,
            "TO_OutputChannel_Disable() did not return the correct value");
    
}


/**
 * Test TO_OutputChannel_Disable() nominal
 */
void TO_OutputChannel_Disable_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 ChannelID = 0;
    
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;
    
    /* Call the function under test */
    result = TO_OutputChannel_Disable(ChannelID);
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(TO_TLMOUTDIS_INF_EID, CFE_EVS_INFORMATION, "",
            "TO_OutputChannel_Disable() failed to raise an event");
    UtAssert_True(result == expected,
            "TO_OutputChannel_Disable() did not return the correct value");
    
}


 /**************************************************************************
 * Test Rollup
 **************************************************************************/
void TO_Custom_App_Test_AddTestCases(void)
{
    UtTest_Add(TO_Custom_Init_OpenChannelFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_Custom_Init_OpenChannelFail");
    UtTest_Add(TO_Custom_Init_EnableChannelFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_Custom_Init_EnableChannelFail");               
    UtTest_Add(TO_Custom_Init_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_Custom_Init_Nominal"); 
    UtTest_Add(TO_Custom_Init_NothingEnabled, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_Custom_Init_NothingEnabled"); 
    UtTest_Add(Test_TO_OutputChannel_CustomBuildupAll_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_CustomBuildupAll_Nominal");
    UtTest_Add(Test_TO_OutputChannel_Enable_NullDestinationAddress, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_NullDestinationAddress");
    UtTest_Add(Test_TO_OutputChannel_Enable_InvalidID, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_InvalidID");
    UtTest_Add(Test_TO_OutputChannel_Enable_SocketFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_SocketFail");
    UtTest_Add(Test_TO_OutputChannel_Enable_BindFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_BindFail");
    UtTest_Add(Test_TO_OutputChannel_Enable_CreateChildTaskFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_CreateChildTaskFail");
    UtTest_Add(Test_TO_OutputChannel_Enable_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Enable_Nominal");
    UtTest_Add(Test_TO_OutputChannel_Send_InvalidID, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Send_InvalidID");
    UtTest_Add(Test_TO_OutputChannel_Send_SendToFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Send_SendToFail");
    UtTest_Add(Test_TO_OutputChannel_Send_SendToTooLong, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Send_SendToTooLong");
    UtTest_Add(Test_TO_OutputChannel_Send_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "Test_TO_OutputChannel_Send_Nominal");
    UtTest_Add(TO_OutputChannel_ChannelHandle_QueueGetFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ChannelHandle_QueueGetFail");
    //UtTest_Add(TO_OutputChannel_ChannelHandle_SendToFail,
    //            TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
    //           "TO_OutputChannel_ChannelHandle_SendToFail");
    //UtTest_Add(TO_OutputChannel_ChannelHandle_PutPoolBufFail,
    //            TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
    //           "TO_OutputChannel_ChannelHandle_PutPoolBufFail");
    UtTest_Add(TO_OutputChannel_ChannelHandle_OSQueueTimeout, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ChannelHandle_OSQueueTimeout");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_EnableInvalidMessageLength, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_EnableInvalidMessageLength");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_DisableInvalidMessageLength, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_DisableInvalidMessageLength");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_InvalidCommandCode, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_InvalidCommandCode");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_EnableFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_EnableFail");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_DisableFail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_DisableFail");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_EnableNominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_EnableNominal");
    UtTest_Add(TO_OutputChannel_ProcessNewCustomCmds_DisableNominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_ProcessNewCustomCmds_DisableNominal");
    UtTest_Add(TO_OutputChannel_UDPChannelTask_Nominal,
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_UDPChannelTask_Nominal");
    UtTest_Add(TO_OutputChannel_CustomCleanupAll_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_CustomCleanupAll_Nominal");
    UtTest_Add(TO_OutputChannel_Disable_Fail, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_Disable_Fail");
    UtTest_Add(TO_OutputChannel_Disable_Nominal, 
                TO_Custom_Test_Setup, TO_Custom_Test_TearDown,
               "TO_OutputChannel_Disable_Nominal");
}
