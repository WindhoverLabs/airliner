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

#include "vc_app_custom_transmit_test.h"
#include "vc_custom_transmit_test_utils.h"
#include "vc_platform_stubs.h"
#include "vc_platform_cfg.h"
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
 * Tests for Custom Transmit Layer
 **************************************************************************/

/**************************************************************************
 * Tests for VC_EnableChannel()
 **************************************************************************/

/**
 * Test VC_EnableChannel() through VC_Init_CustomTransmitters
 * fail already initialized error
 */
void Test_VC_Custom_InitTransmit_AlreadyInitialized(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[128];
    snprintf(returnString, 128, "VC UDP for channel %u already enabled.", 0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set a socket file descriptor to a non 0 value */
    VC_AppCustomData.Channel[0].SocketFd = 1;
    
    /* Call the function under test */
    result = VC_Init_CustomTransmitters();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**
 * Test VC_EnableChannel() invalid channel id
 */
void Test_VC_Custom_InitTransmit_ChannelId(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[128];
    snprintf(returnString, 128, "VC ChannelID (%u) invalid.", (VC_MAX_OUTPUT_CHANNELS + 1));
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Call the function under test */
    result = VC_EnableChannel((VC_MAX_OUTPUT_CHANNELS + 1));

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**
 * Test VC_EnableChannel() through VC_Init_CustomTransmitters
 * fail socket creation
 */
void Test_VC_Custom_InitTransmit_Socket(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[128];
    snprintf(returnString, 128, "VC socket errno: %i on channel %u", 8, 0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set socket call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return = -1;
    
    /* Set errno for socket call */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno_Value = 8;
    
    /* Call the function under test */
    result = VC_Init_CustomTransmitters();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    UtAssert_True(VC_AppCustomData.Channel[0].SocketFd == -1,
                        "VC_EnableChannel() failed to return an error");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**
 * Test VC_EnableChannel() through VC_Init_CustomTransmitters
 * fail invalid address
 */
void Test_VC_Custom_InitTransmit_InvalidAddress(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[128];
    snprintf(returnString, 128, "VC inet_aton errno: %i on channel %u", 88, 0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set an invalid address for inet_aton */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "test");
    
    /* Call the function under test */
    result = VC_Init_CustomTransmitters();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**
 * Test VC_EnableChannel() through VC_Init_CustomTransmitters
 * fail bind
 */
void Test_VC_Custom_InitTransmit_Bind(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[128];
    snprintf(returnString, 128, "VC bind errno: %i on channel %u", 8, 0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set an valid address for inet_aton */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "1.1.1.1");
    
    /* Set socket call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Return = -1;
    
    /* Set errno for socket call */
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno_Value = 8;
    
    /* Call the function under test */
    result = VC_Init_CustomTransmitters();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**
 * Test VC_EnableChannel() through VC_Init_CustomTransmitters nominal
 */
void Test_VC_Custom_InitTransmit_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    char returnString[128];
    snprintf(returnString, 128, "VC UDP output enabled channel %u to %s:%u", 0, "test",0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set an valid address for inet_aton */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "1.1.1.1");
    
    strcpy(VC_AppCustomData.Channel[0].DestIP, "test");
    VC_AppCustomData.Channel[0].DestPort = 0;
    
    /* Call the function under test */
    result = VC_Init_CustomTransmitters();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not succeed");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_ENABLED, 
                         "VC_Init_CustomTransmitters() did not mode to disabled");
    UtAssert_EventSent(VC_CHA_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_Transmit_InitData()
 **************************************************************************/

/**
 * Test VC_Transmit_InitData() nominal
 * Note: currently no way to fail this function
 */
void Test_VC_Transmit_InitData_Nominal(void)
{
    int32 result = -1;
    int32 expected = CFE_SUCCESS;
    
    /* Call the function under test */
    result = VC_Transmit_InitData();
    
    /* Verify the results */
    UtAssert_True(result == expected,"VC_Transmit_InitData() did not succeed");
}


/**************************************************************************
 * Tests for VC_Init_CustomTransmitters()
 **************************************************************************/

/**************************************************************************
 * Tests for VC_Transmit_Init()
 **************************************************************************/
 
 /**
 * Test VC_Transmit_Init() fail VC_Init_CustomTransmitters
 * see reuse (VC_EnableChannel() invalid channel id)
 */
void Test_VC_Custom_TransmitInit_Fail(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    
    char returnString[128];
    snprintf(returnString, 128, "VC UDP for channel %u already enabled.", 0);
    char returnString1[128];
    snprintf(returnString1, 128, "VC_Transmit_Init Failed");
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set a socket file descriptor to a non 0 value */
    VC_AppCustomData.Channel[0].SocketFd = 1;
    
    /* Call the function under test */
    result = VC_Transmit_Init();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not return an error");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_EnableChannel() failed to raise an event");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, returnString1, 
                        "VC_EnableChannel() failed to raise an event");
    
}

/**
 * Test VC_Transmit_Init() nominal
 */
void Test_VC_Custom_TransmitInit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    char returnString[128];
    snprintf(returnString, 128, "VC UDP output enabled channel %u to %s:%u", 0, "test",0);
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set an valid address for inet_aton */
    strcpy(VC_AppCustomData.Channel[0].MyIP, "1.1.1.1");
    
    strcpy(VC_AppCustomData.Channel[0].DestIP, "test");
    VC_AppCustomData.Channel[0].DestPort = 0;
    
    /* Call the function under test */
    result = VC_Transmit_Init();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Init_CustomTransmitters() did not succeed");
    UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_ENABLED, 
                         "VC_Init_CustomTransmitters() did not set mode to disabled");
    UtAssert_EventSent(VC_CHA_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_EnableChannel() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_SendData()
 **************************************************************************/
 
/**
 * Test VC_SendData() invalid channel id
 */
void Test_VC_Custom_SendData_ChannelId(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    /* Call the function under test */
    result = VC_SendData((VC_MAX_OUTPUT_CHANNELS + 1), "buffer", 77);

    UtAssert_True(result == expected,"VC_SendData() did not return the correct value");
}

/**
 * Test VC_SendData() sendto fail
 */
void Test_VC_Custom_SendData_SendTo(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set sendto call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Return = -1;
    
    /* Set errno for sendto call */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno_Value = 8;
    
    /* Call the function under test */
    result = VC_SendData(0, "buffer", 77);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    /* TODO currently failure does not disable channel. */
    //UtAssert_True(VC_AppCustomData.Channel[0].Mode == VC_CHANNEL_DISABLED, 
                         //"VC_SendData() did not set mode to disabled");
    //UtAssert_True(result == expected,"VC_SendData() did not return the correct value");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, "", 
                        "VC_SendData() failed to raise an event");
}

/**
 * Test VC_SendData() sendto fail message too long
 */
void Test_VC_Custom_SendData_SendToTooLong(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Set sendto call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Return = -1;
    
    /* Set errno for sendto call */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Errno_Value = 90;
    
    /* Call the function under test */
    result = VC_SendData(0, "buffer", 77);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    /* TODO currently failure does not disable channel. */
    //UtAssert_True(result == expected,"VC_SendData() did not return the correct value");
    UtAssert_EventSent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, "", 
                        "VC_SendData() failed to raise an event");
}

/**
 * Test VC_SendData() nominal
 */
void Test_VC_Custom_SendData_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    /* Call the function under test */
    result = VC_SendData(0, "buffer", 77);

    UtAssert_True(result == expected,"VC_SendData() did not return the correct value");
}

/**************************************************************************
 * Tests for VC_Address_Verification()
 **************************************************************************/
 
/**
 * Test VC_Address_Verification() nominal
 */
void Test_VC_Custom_AddressVerification_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    result = VC_Address_Verification("1.1.1.1");
    
    UtAssert_True(result == expected,"VC_Address_Verification() did not return the correct value");
}

/**
 * Test VC_Address_Verification() fail
 */
void Test_VC_Custom_AddressVerification_Fail(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    
    result = VC_Address_Verification("test");
    
    UtAssert_True(result == expected,"VC_Address_Verification() did not return the correct value");
}

/**************************************************************************
 * Tests for VC_Update_Destination()
 **************************************************************************/
 
/**
 * Test VC_Update_Destination() nominal
 */
void Test_VC_Custom_UpdateDestination_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    /* Set test channel 0 to enabled */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    result = VC_Update_Destination("test", 5000);
    
    UtAssert_True(result == expected,"VC_Update_Destination() did not return the correct value");
    UtAssert_StrCmp(VC_AppCustomData.Channel[0].DestIP, "test", 
                        "VC_Update_Destination() did not set a correct value");
    UtAssert_True(VC_AppCustomData.Channel[0].DestPort == 5000,
                        "VC_Update_Destination() did not set a correct value");
    
}

/**
 * Test VC_Update_Destination() fail
 */
void Test_VC_Custom_UpdateDestination_Fail(void)
{
    /* No way to fail strncpy in update destination at this time. */
    //boolean result = TRUE;
    //boolean expected = FALSE;

    //result = VC_Update_Destination("test", 5000);

    //UtAssert_True(result == expected,"VC_Update_Destination() did not return the correct value");
}

/**************************************************************************
 * Tests for VC_DisableChannel()
 **************************************************************************/
 
/**
 * Test VC_DisableChannel() fail
 */
void Test_VC_Custom_DisableChannel_Fail(void)
{
    int32 result = 0;
    int32 expected = -1;

    result = VC_DisableChannel(0);

    UtAssert_True(result == expected,"VC_DisableChannel() did not return the correct value");
}

/**
 * Test VC_DisableChannel() through VC_CleanupCustom nominal
 */
void Test_VC_Custom_DisableChannel_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    result = VC_CleanupCustom();
    
    UtAssert_True(result == expected,"VC_DisableChannel() did not return the correct value");
    
}

/**************************************************************************
 * Tests for VC_CleanupCustom()
 **************************************************************************/
 
/**
 * Test VC_CleanupCustom() fail
 * No current way to fail cleanup custom since mode check is the only
 * error checking inside of disable channel. Error checking close return
 * value will enable a fail condition for cleanup custom 
 */


/**************************************************************************
 * Tests for VC_Transmit_Uninit()
 **************************************************************************/
 
 /**
 * Test VC_Transmit_Uninit() fail
 * No current way to fail cleanup custom since mode check is the only
 * error checking inside of disable channel. Error checking close return
 * value will enable a fail condition for cleanup custom 
 */
 
/**
 * Test VC_Transmit_Uninit() nominal
 */
void Test_VC_Custom_TransmitUninit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    
    result = VC_Transmit_Uninit();
    
    UtAssert_True(result == expected,"VC_Transmit_Uninit() did not return the correct value");
    
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void VC_Custom_App_Transmit_Test_AddTestCases(void)
{
/**************************************************************************
 * Tests for Custom Transmit Layer
 **************************************************************************/
    UtTest_Add(Test_VC_Custom_InitTransmit_AlreadyInitialized, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_AlreadyInitialized");
    UtTest_Add(Test_VC_Custom_InitTransmit_ChannelId, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_ChannelId");
    UtTest_Add(Test_VC_Custom_InitTransmit_Socket, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_Socket");
    UtTest_Add(Test_VC_Custom_InitTransmit_InvalidAddress, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_InvalidAddress");
    UtTest_Add(Test_VC_Custom_InitTransmit_Bind, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_Bind");
    UtTest_Add(Test_VC_Custom_InitTransmit_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_InitTransmit_Nominal");
        UtTest_Add(Test_VC_Transmit_InitData_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Transmit_InitData_Nominal");
    UtTest_Add(Test_VC_Custom_TransmitInit_Fail, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_TransmitInit_Fail");
    UtTest_Add(Test_VC_Custom_TransmitInit_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_TransmitInit_Nominal");
    UtTest_Add(Test_VC_Custom_SendData_ChannelId, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_SendData_ChannelId");
    UtTest_Add(Test_VC_Custom_SendData_SendTo, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_SendData_SendTo");
    UtTest_Add(Test_VC_Custom_SendData_SendToTooLong, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_SendData_SendToTooLong");
    UtTest_Add(Test_VC_Custom_SendData_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_SendData_Nominal");
    UtTest_Add(Test_VC_Custom_AddressVerification_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_AddressVerification_Nominal");
    UtTest_Add(Test_VC_Custom_AddressVerification_Fail, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_AddressVerification_Fail");
    UtTest_Add(Test_VC_Custom_UpdateDestination_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_UpdateDestination_Nominal");
    UtTest_Add(Test_VC_Custom_UpdateDestination_Fail, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_UpdateDestination_Fail");
    UtTest_Add(Test_VC_Custom_DisableChannel_Fail, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_DisableChannel_Fail");
    UtTest_Add(Test_VC_Custom_DisableChannel_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_DisableChannel_Nominal");
    UtTest_Add(Test_VC_Custom_TransmitUninit_Nominal, VC_Custom_Transmit_Test_Setup, 
            VC_Custom_Transmit_Test_TearDown, "Test_VC_Custom_TransmitUninit_Nominal");
}
