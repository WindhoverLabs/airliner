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

#include "vc_app_custom_device_test.h"
#include "vc_custom_device_test_utils.h"
#include "vc_transmit_udp.h"
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
 * Tests for VC_InitCustomDevices()
 **************************************************************************/
/**
 * Test VC_InitCustomDevices() fail socket creation
 */
void Test_VC_Custom_InitCustomDevices_FailSocket(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "Socket errno: %i on channel %u", 13, 0);
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set socket creation to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno_Value = 13;
    
    result = VC_Init_CustomDevices();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_UNINITIALIZED, 
                         "VC_InitCustomDevices() status NOT uninitialized");
    UtAssert_True(result == expected,"VC_InitDevice() did not return failure");
}

/**
 * Test VC_InitCustomDevices() fail bind
 */
void Test_VC_Custom_InitCustomDevices_FailBind(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "Bind errno: %i on channel %u", 13, 0);
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set socket creation to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return = 1;
    
    /* Set bind to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Return = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Errno_Value = 13;
    
    result = VC_Init_CustomDevices();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_UNINITIALIZED, 
                         "VC_InitCustomDevices() status NOT uninitialized");
    UtAssert_True(result == expected,"VC_InitDevice() did not return failure");
}

/**
 * Test VC_InitCustomDevices() nominal 
 */
void Test_VC_Custom_InitCustomDevices_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Device initialized channel %u", 0);
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set socket creation to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return = 1;
    
    /* Set bind to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Bind_Return = 1;
    
    result = VC_Init_CustomDevices();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED, 
                         "VC_InitCustomDevices() status NOT initialized");
    UtAssert_True(result == expected,"VC_InitDevice() did not return failure");
}

/**************************************************************************
 * Tests for VC_Stream_Task()
 **************************************************************************/

/**
 * Test VC_Stream_Task() fail register child task
 */
void Test_VC_Custom_StreamTask_RegisterChildTaskFail(void)
{
    /* Set register child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, -1, 1);

    /* Call the function under test */
    VC_Stream_Task();
    
    /* Verfity results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "VC_Stream_Task() did not set app state");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, 
                        "VC streaming task exited with return code 0 task status (0xFFFFFFFF)", 
                        "VC_Stream_Task() failed to raise an event");
}


/**
 * Test VC_Stream_Task() fail nothing enabled and streaming
 */
void Test_VC_Custom_StreamTask_NothingEnabled(void)
{
    
    char returnString[64];
    snprintf(returnString, 64, "VC can't start streaming no devices are streaming enabled");
    char returnString1[128];
    snprintf(returnString1, 128, "VC streaming task exited with return code %d task status (0x%08X)", 
                                -1, (unsigned int)CFE_SUCCESS);
  
    /* Set register child task to succeed */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    /* Set the while loop continue flag to true */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].Socket = 7;
    
    /* Set select to fail with an error (-1) */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Return = -1;

    /* Call the function under test */
    VC_Stream_Task();
    
    /* Verfity results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "VC_Stream_Task() did not set app state");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stream_Task() failed to raise an event");                        
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString1, 
                        "VC_Stream_Task() failed to raise an event");
}


/**
 * Test VC_Stream_Task() fail select error
 */
void Test_VC_Custom_StreamTask_SelectError(void)
{
    
    char returnString[64];
    snprintf(returnString, 64, "VC start streaming failed select() returned %i", 6);
    char returnString1[128];
    snprintf(returnString1, 128, "VC streaming task exited with return code %d task status (0x%08X)", 
                                -1, (unsigned int)CFE_SUCCESS);
  
    /* Set register child task to succeed */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    /* Set the while loop continue flag to true */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Set the mode and state for channel 0*/
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
        /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno_Value = 6;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].Socket = 7;
    
    /* Set select to fail with an error (-1) */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Return = -1;

    /* Call the function under test */
    VC_Stream_Task();
    
    /* Verfity results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "VC_Stream_Task() did not set app state");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stream_Task() failed to raise an event");                        
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString1, 
                        "VC_Stream_Task() failed to raise an event");
}


/**
 * Test VC_Stream_Task() fail select interrupted max retry attempts
 */
void Test_VC_Custom_StreamTask_InterruptedError(void)
{
    char returnString[64];
    snprintf(returnString, 64, "VC select was interrupted");
    char returnString1[128];
    snprintf(returnString1, 128, "VC streaming task exited with return code %d task status (0x%08X)", 
                                -1, (unsigned int)CFE_SUCCESS);
  
    /* Set register child task to succeed */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    /* Set the while loop continue flag to true */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Set the mode and state for channel 0*/
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
        /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno_Value = EINTR;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].Socket = 7;
    
    /* Set select to fail with an error (-1) */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Return = -1;

    /* Call the function under test */
    VC_Stream_Task();
    
    /* Verfity results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==(VC_MAX_RETRY_ATTEMPTS + 1),
                        "Event Count is not correct");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "VC_Stream_Task() did not set app state");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stream_Task() failed to raise an event");                        
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString1, 
                        "VC_Stream_Task() failed to raise an event");
}


/**************************************************************************
 * Tests for VC_Devices_Start()
 **************************************************************************/
 /**
 * Test VC_Devices_Start() fail child task creation
 */
void Test_VC_Custom_DevicesStart_ChildTaskCreationFail(void)
{
    /* Set create child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);
    
    boolean result = TRUE;
    boolean expected = FALSE;
    
    /* Call the function under test */
    result = VC_Devices_Start();

    UtAssert_True(VC_AppCustomDevice.ContinueFlag == FALSE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Start() did not fail");
}


/**
 * Test VC_Devices_Start() nominal
 */
void Test_VC_Custom_DevicesStart_Nominal(void)
{
    /* Set create child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    boolean result = FALSE;
    boolean expected = TRUE;

    /* Call the function under test */
    result = VC_Devices_Start();
    
    UtAssert_True(VC_AppCustomDevice.ContinueFlag == TRUE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Start() did not fail");
}


/**************************************************************************
 * Tests for VC_Devices_Stop()
 **************************************************************************/
 /**
 * Test VC_Devices_Stop() fail delete child task
 */
void Test_VC_Custom_DevicesStop_ChildTaskDeleteFail(void)
{
    /* Set delete child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_DELETECHILDTASK_INDEX, -1, 1);

    boolean result = TRUE;
    boolean expected = FALSE;
    
    /* Call the function under test */
    result = VC_Devices_Stop();

    UtAssert_True(VC_AppCustomDevice.ContinueFlag == FALSE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "AppState not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Stop() did not fail");
}


 /**
 * Test VC_Devices_Stop() nominal
 */
void Test_VC_Custom_DevicesStop_Nominal(void)
{
    /* Set delete child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_DELETECHILDTASK_INDEX, CFE_SUCCESS, 1);

    boolean result = FALSE;
    boolean expected = TRUE;
    
    /* Call the function under test */
    result = VC_Devices_Stop();

    UtAssert_True(VC_AppCustomDevice.ContinueFlag == FALSE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(VC_AppData.AppState == VC_INITIALIZED,
                        "AppState not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Stop() did not fail");
}


/**************************************************************************
 * Tests for VC_CleanupDevices()
 **************************************************************************/
 /**
 * Test VC_CleanupDevices() close fail
 * 
 */
void Test_VC_Custom_CleanupDevices_CloseFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    /* Set channel 0 to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set close to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Close_Return = -1;
    
    /* Call the function under test */
    result = VC_CleanupDevices();
       
    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode == VC_DEVICE_ENABLED,
                        "VC_CleanupDevices() did not set correct status");
}
 
/**
 * Test VC_CleanupDevices() nominal
 * 
 */
void Test_VC_Custom_CleanupDevices_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    /* Set channel 0 to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    
    /* Call the function under test */
    result = VC_CleanupDevices();
       
    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode == VC_DEVICE_DISABLED,
                        "VC_CleanupDevices() did not set correct status");
}

/**************************************************************************
 * Tests for VC_Devices_Init()
 **************************************************************************/
 /**
 * Test VC_Devices_Init fail VC_Init_CustomDevices see previous test
 * VC_InitCustomDevices() fail socket creation
 */
void Test_VC_Custom_DevicesInit_Fail(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set socket creation to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Return = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Socket_Errno_Value = 13;

    result = VC_Devices_Init();
    
    UtAssert_True(result == expected,"VC_Devices_Init() did not return an error");
}


 /**
 * Test VC_Devices_Init nominal see previous test
 * VC_InitCustomDevices() nominal
 */
void Test_VC_Custom_DevicesInit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;

    result = VC_Devices_Init();
    
    UtAssert_True(result == expected,"VC_Devices_Init() did not return an error");
}
 
 
/**************************************************************************
 * Tests for VC_Devices_Uninit()
 **************************************************************************/
/**
 * Test VC_Devices_Uninit() cleanup fail see previous test
 * VC_CleanupDevices() close fail
 */
void Test_VC_Custom_DevicesUninit_CleanupFail(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;

    /* Set channel 0 to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set close to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Close_Return = -1;

    /* Call the function under test */
    result = VC_Devices_Uninit();
     
    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_DISABLED,
                        "VC_CleanupDevices() did not set correct status");
}


/**
 * Test VC_Devices_Uninit() nominal
 * 
 */
void Test_VC_Custom_DevicesUninit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;

    /* Call the function under test */
    result = VC_Devices_Uninit();
     
    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_DISABLED,
                        "VC_CleanupDevices() did not set correct status");
}

/**************************************************************************
 * Tests for VC_Send_Buffer()
 **************************************************************************/
/**
 * Test VC_Send_Buffer() recv error
 * 
 */
void Test_VC_Custom_SendBuffer_RecvError(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[64];
    snprintf(returnString, 64, "VC recv errno: %i on channel %u", 1, 0);
    
    /* Set recv to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Errno = 1;
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Errno_Value = 1;

    /* Call the function under test */
    result = VC_Send_Buffer(0);
     
    UtAssert_True(result == expected,"VC_Send_Buffer() did not return the correct value");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Send_Buffer() failed to raise an event");  
}


/**
 * Test VC_Send_Buffer() size error
 * 
 */
void Test_VC_Custom_SendBuffer_SizeError(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[64];
    snprintf(returnString, 64, "VC recv size error on channel %u", 0);
    
    /* Set recv to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return = VC_MPARTMUX_HEADER_SIZE;
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return1 = -1;

    /* Call the function under test */
    result = VC_Send_Buffer(0);
     
    UtAssert_True(result == expected,"VC_Send_Buffer() did not return the correct value");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Send_Buffer() failed to raise an event");  
}


/**
 * Test VC_Send_Buffer() send data fail
 * 
 */
void Test_VC_Custom_SendBuffer_SendError(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    char returnString[64];
    snprintf(returnString, 64, "VC send data failed on channel %u", 0);
    
    /* Set recv to pass/fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return = VC_MPARTMUX_HEADER_SIZE;
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return1 = 0;
    
    /* set sendto to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Return = -1;
    
    /* Properly initialize custom transmit data */
    VC_CustomTransmit_InitData();

    /* Call the function under test */
    result = VC_Send_Buffer(0);
     
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not return the correct value");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Send_Buffer() failed to raise an event");  
}


/**
 * Test VC_Send_Buffer() nominal
 * 
 */
void Test_VC_Custom_SendBuffer_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC send data failed on channel %u", 0);
    
    /* Set recv to pass/fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return = VC_MPARTMUX_HEADER_SIZE;
    VC_Platform_Stubs_Returns.VC_Wrap_Recv_Return1 = 0;
    
    /* Properly initialize custom transmit data */
    VC_CustomTransmit_InitData();

    /* Call the function under test */
    result = VC_Send_Buffer(0);
     
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not return the correct value");
}


/**************************************************************************
 * Tests for VC_Devices_InitData()
 **************************************************************************/

/**
 * Test VC_Devices_InitData() nominal
 * Note: currently no way to fail this function
 */
void Test_VC_Devices_InitData_Nominal(void)
{
    int32 result = -1;
    int32 expected = CFE_SUCCESS;
    
    /* Call the function under test */
    result = VC_Devices_InitData();
    
    /* Verify the results */
    UtAssert_True(result == expected,"VC_Custom_InitData() did not succeed");
}

/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void VC_Custom_App_Device_Test_AddTestCases(void)
{
/**************************************************************************
 * Tests for Custom Device Layer
 **************************************************************************/
    UtTest_Add(Test_VC_Custom_InitCustomDevices_FailSocket, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitCustomDevices_FailSocket");
    UtTest_Add(Test_VC_Custom_InitCustomDevices_FailBind, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitCustomDevices_FailBind");
    UtTest_Add(Test_VC_Custom_InitCustomDevices_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitCustomDevices_Nominal");
    UtTest_Add(Test_VC_Custom_StreamTask_RegisterChildTaskFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_RegisterChildTaskFail");
    UtTest_Add(Test_VC_Custom_StreamTask_NothingEnabled, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_NothingEnabled");
    UtTest_Add(Test_VC_Custom_StreamTask_SelectError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_SelectError");
    UtTest_Add(Test_VC_Custom_StreamTask_InterruptedError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_InterruptedError");
    UtTest_Add(Test_VC_Custom_DevicesStart_ChildTaskCreationFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStart_ChildTaskCreationFail");
    UtTest_Add(Test_VC_Custom_DevicesStart_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStart_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesStop_ChildTaskDeleteFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStop_ChildTaskDeleteFail");
    UtTest_Add(Test_VC_Custom_DevicesStop_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStop_Nominal");
    UtTest_Add(Test_VC_Custom_CleanupDevices_CloseFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_CleanupDevices_CloseFail");
    UtTest_Add(Test_VC_Custom_CleanupDevices_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_CleanupDevices_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesInit_Fail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesInit_Fail");
    UtTest_Add(Test_VC_Custom_DevicesInit_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesInit_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesUninit_CleanupFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesUninit_CleanupFail");
    UtTest_Add(Test_VC_Custom_DevicesUninit_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesUninit_Nominal");
    UtTest_Add(Test_VC_Custom_SendBuffer_RecvError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_RecvError");
    UtTest_Add(Test_VC_Custom_SendBuffer_SizeError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_SizeError");
    UtTest_Add(Test_VC_Custom_SendBuffer_SendError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_SendError");
    UtTest_Add(Test_VC_Custom_SendBuffer_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_Nominal");
    UtTest_Add(Test_VC_Devices_InitData_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Devices_InitData_Nominal");
}
