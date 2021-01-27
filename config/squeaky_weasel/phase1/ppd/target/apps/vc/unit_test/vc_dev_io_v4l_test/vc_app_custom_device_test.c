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
 * Tests for VC_Ioctl()
 **************************************************************************/
/**
 * Test VC_Ioctl() ioctl returns -1 error
 */
void Test_VC_Custom_Ioctl_Error(void)
{
    int32 result = 0;
    int32 expected = -1;
    
    /* Set a return value for the wrapped ioctl */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* A test argument to pass to VC_Ioctl value is random 
     * the wrapped ioctl will not use the arguments and will return a
     * value */
    int test = 1;
    
    
    result = VC_Ioctl(test, test, &test);

    UtAssert_True(result == expected, "VC_Ioctl did not return an error");
}
/**
 * Test VC_Ioctl() ioctl returns -1 error and errno interrupted
 */
void Test_VC_Custom_Ioctl_Interrupted(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint32 i = 0;
    
    /* Set a return value for the wrapped ioctl (always fail) */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1 = -1;
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return2 = -1;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = EINTR;
    
    /* A test argument to pass to VC_Ioctl value is random 
     * the wrapped ioctl will not use the arguments and will return a
     * value */
    int test = 777;
    
    /* Call VC_Ioctl with errno set to EINTR max retry attempts*/
    result = VC_Ioctl(test, test, &test);
    
    UtAssert_True(result == expected, "VC_Ioctl did not return an error");
}

/**************************************************************************
 * Tests for VC_InitDevice()
 **************************************************************************/

/**
 * Test VC_InitDevice() through VC_Init_CustomDevices already initialized error
 */
void Test_VC_Custom_InitDevice_AlreadyInitialized(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    /* Set the first device file descriptor to a non-zero value */
    VC_AppCustomDevice.Channel[0].DeviceFd = 1;

    result = VC_Init_CustomDevices();
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_InitDevice() did not return an error");
    
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode == VC_DEVICE_DISABLED, 
                         "VC_InitCustomDevices() did not mode to disabled");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, "VC Device test for channel 0 already initialized.", 
                        "VC_InitDevice() failed to raise an event");
}


/**
 * Test VC_InitDevice() null device name
 */
void Test_VC_Custom_InitDevice_NullName(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char *DeviceName = 0;
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;


    result = VC_InitDevice(DeviceID, DeviceName);
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_InitDevice() did not return an error");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, "VC Device name for channel 0 is null.", 
                        "VC_InitDevice() failed to raise an event");
}


/**
 * Test VC_InitDevice() invalid device ID
 */
void Test_VC_Custom_InitDevice_InvalidID(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = VC_MAX_DEVICES;
    char returnString[32];
    snprintf(returnString, 32, "VC DeviceID (%u) invalid.", VC_MAX_DEVICES);
    char DeviceName[] = "test";
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;

    result = VC_InitDevice(DeviceID, DeviceName);
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_InitDevice() did not return an error");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_InitDevice() failed to raise an event");
}


/**
 * Test VC_InitDevice() through VC_InitCustomDevices() open error
 */
void Test_VC_Custom_InitDevice_OpenError(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[32];
    snprintf(returnString, 32, "VC Device open errno: %i on channel %u", 5, 0);
    //char DeviceName[] = "test";
    
    /* Set a return value for the wrapped open */
    VC_Platform_Stubs_Returns.VC_Wrap_Open_Return = -1;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Open_Errno = 1;
    
    /* Set error number to interrupted */
    VC_Platform_Stubs_Returns.VC_Wrap_Open_Errno_Value = 5;
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;

    //result = VC_InitDevice(DeviceID, DeviceName);
    result = VC_Init_CustomDevices();
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode == VC_DEVICE_DISABLED, 
                         "VC_InitCustomDevices() did not mode to disabled");
    UtAssert_True(result == expected,"VC_InitDevice() did not return an error");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_InitDevice() failed to raise an event");
}

/**
 * Test VC_InitDevice() nominal
 */
void Test_VC_Custom_InitDevice_Nominal(void)
{
    int32 result = 0;
    int32 expected = 0;
    uint8 DeviceID = 0;
    char DeviceName[] = "test";
    
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;

    result = VC_InitDevice(DeviceID, DeviceName);
    
    UtAssert_True(result == expected,"VC_InitDevice() did not return success");
}


/**************************************************************************
 * Tests for VC_InitCustomDevices()
 **************************************************************************/

/**
 * Test VC_InitCustomDevices() pass init but fail configure
 * ConfigureDevice will fail on the first v4L struct verification
 */
void Test_VC_Custom_InitCustomDevices_FailConfigure(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Device initialized channel %u from %s", 0, "test");
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;

    /* Set a test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    result = VC_Init_CustomDevices();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_UNINITIALIZED, 
                         "VC_InitCustomDevices() did not set status to uninitialized");
    UtAssert_True(result == expected,"VC_InitDevice() did not return failure");
}

/**
 * Test VC_InitCustomDevices() Nominal
 */
void Test_VC_Custom_InitCustomDevices_Nominal(void)
{
    int32 result = 0;
    int32 expected = 0;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Device initialized channel %u from %s", 0, "test");
    char returnString1[64];
    snprintf(returnString1, 64, "VC Device configured channel %u from %s", 0, "test"); 
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;
    
    /* Set the correct size */
    VC_AppCustomDevice.Channel[0].Buffer_Size = VC_MAX_BUFFER_SIZE;
    
    /* Set the correct buffer number */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;
    
    result = VC_Init_CustomDevices();
    
    UtAssert_True(result == expected,"VC_Init_CustomDevices() did not succeed");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED,
                    "VC_Init_CustomDevices() state not initialized");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString1, 
                        "VC_InitCustomDevices() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_ConfigureDevice()
 **************************************************************************/
 
/**
 * Test VC_ConfigureDevice() fail V4L capabilities query
 */
void Test_VC_Custom_ConfigureDevice_CapabilitiesFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_QUERYCAP returned %i on %s channel %u", 5, "test", 0);
    
    /* Set a return value for the wrapped ioctl */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");

    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
    
}


/**
 * Test VC_ConfigureDevice() fail V4L capabilities fail 
 * check buffer type
 */
void Test_VC_Custom_ConfigureDevice_CapabilitiesBuffer(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Capabilities %u on %s channel %u not found", 
    VC_V4L_BUFFER_TYPE,"test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
    
}


/**
 * Test VC_ConfigureDevice() fail V4L capabilities check fail
 * check streaming capability
 */
void Test_VC_Custom_ConfigureDevice_CapabilitiesStreaming(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Capabilities %u on %s channel %u not found", 
    V4L2_CAP_STREAMING,"test", 0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed once for the first capabilities check */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 1;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
    
}


/**
 * Test VC_ConfigureDevice() fail V4L set format fail
 */
void Test_VC_Custom_ConfigureDevice_FormatFail(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VIDIOC_S_FMT returned %i on %s channel %u", 
    5,"test", 0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    /* Set the second ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1 = -1;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
    
}

/**
 * Test VC_ConfigureDevice() fail V4L set format verification check
 */
void Test_VC_Custom_ConfigureDevice_FormatCheck(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC device %s channel %u didn't accept format %u.", 
    "test",0, 0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
    
}

/**
 * Test VC_ConfigureDevice() fail V4L set format verification check
 * resolution
 */
void Test_VC_Custom_ConfigureDevice_FormatCheckRes(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC device %s channel %u didn't accept resolution instead %d:%d.", 
    "test",0, VC_FRAME_WIDTH, VC_FRAME_HEIGHT);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
}

/**
 * Test VC_ConfigureDevice() fail V4L set format verification check
 * size
 */
void Test_VC_Custom_ConfigureDevice_FormatCheckSize(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC device %s channel %u size image %u > buffer size %lu.", 
    "test",0, VC_MAX_BUFFER_SIZE, 0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
}


/**
 * Test VC_ConfigureDevice() fail V4L requests buffers
 */
void Test_VC_Custom_ConfigureDevice_RequestsBuffers(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_REQBUFS returned %i on %s channel %u.", 
    5,"test",0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;
    
    /* Set the correct size */
    VC_AppCustomDevice.Channel[0].Buffer_Size = VC_MAX_BUFFER_SIZE;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    /* Set the third ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return2 = -1;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
}


/**
 * Test VC_ConfigureDevice() fail V4L requests buffers fail requested 
 * count verification
 */
void Test_VC_Custom_ConfigureDevice_RequestsBuffersCount(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_REQBUFS did not comply. %u buffers on %s channel %u.", 
    4,"test",0);
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;
    
    /* Set the correct size */
    VC_AppCustomDevice.Channel[0].Buffer_Size = VC_MAX_BUFFER_SIZE;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_ConfigureDevices() failed to raise an event");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
}

/**
 * Test VC_ConfigureDevice() nominal 
 */
void Test_VC_Custom_ConfigureDevice_Nominal(void)
{
    int32 result = 0;
    int32 expected = 0;
    uint8 DeviceID = 0;
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;
    
    /* Set the correct size */
    VC_AppCustomDevice.Channel[0].Buffer_Size = VC_MAX_BUFFER_SIZE;
    
    /* Set the correct buffer number */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;

    /* Call the function under test */
    result = VC_ConfigureDevice(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
    UtAssert_True(result == expected,"VC_ConfigureDevice() did not return failure");
}

/**************************************************************************
 * Tests for VC_Devices_Init()
 **************************************************************************/
 
/**
 * Test VC_Devices_Init fail VC_Init_CustomDevices and 
 * VC_InitDevice() already initialized error (see previous already 
 * initialized test)
 */
void Test_VC_Custom_DevicesInit_Fail(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    uint8 DeviceID = 0;
    
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    /* Set the first device file descriptor to a non-zero value */
    VC_AppCustomDevice.Channel[0].DeviceFd = 1;

    result = VC_Devices_Init();
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_InitDevice() did not return an error");
    
    UtAssert_True(VC_AppCustomDevice.Channel[0].Mode == VC_DEVICE_DISABLED, 
                         "VC_InitCustomDevices() did not mode to disabled");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, "VC Device test for channel 0 already initialized.", 
                        "VC_InitDevice() failed to raise an event");
}

/**
 * Test VC_Devices_Init Nominal (See previous test for
 * VC_InitCustomDevices() Nominal)
 */
void Test_VC_Custom_DevicesInit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    uint8 DeviceID = 0;
    char returnString[64];
    snprintf(returnString, 64, "VC Device initialized channel %u from %s", 0, "test");
    char returnString1[64];
    snprintf(returnString1, 64, "VC Device configured channel %u from %s", 0, "test"); 
    /* Set the first device to enabled */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Set to true to emulate ioctl setting struct values */
    /* Need to succeed twice to reach format ioctl call */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set the video format to pass format check */
    VC_AppCustomDevice.Channel[0].VideoFormat = VC_V4L_VIDEO_FORMAT;
    
    /* Set the correct buffer type */
    VC_AppCustomDevice.Channel[0].BufferType = VC_V4L_BUFFER_TYPE;
    
    /* Set the correct resolution */
    VC_AppCustomDevice.Channel[0].FrameWidth = VC_FRAME_WIDTH;
    VC_AppCustomDevice.Channel[0].FrameHeight = VC_FRAME_HEIGHT;
    
    /* Set the correct size */
    VC_AppCustomDevice.Channel[0].Buffer_Size = VC_MAX_BUFFER_SIZE;
    
    /* Set the correct buffer number */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;
    
    result = VC_Devices_Init();
    
    UtAssert_True(result == expected,"VC_Init_CustomDevices() did not succeed");
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED,
                    "VC_Init_CustomDevices() state not initialized");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_InitCustomDevices() failed to raise an event");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString1, 
                        "VC_InitCustomDevices() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_Start_StreamingDevice()
 **************************************************************************/

/**
 * Test VC_Start_StreamingDevice through VC_Start_Streaming()
 * fail queue buffers ioctl
 */
void Test_VC_Custom_StartStreamingDevice_BufferQueue(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_QBUF returned %i on %s channel %u", 5, "test", 0);

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to its correct value */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;

    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Call the function under test */
    result = VC_Start_Streaming();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Start_StreamingDevice() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED, 
                        "VC_StartStreaming changed status with failure");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
}

/**
 * Test VC_Start_StreamingDevice through VC_Start_Streaming() 
 * fail stream on ioctl
 */
void Test_VC_Custom_StartStreamingDevice_StreamOn(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMON returned %i on %s channel %u", 5, "test", 0);

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to one to only call ioctl queue buffer once */
    VC_AppCustomDevice.Channel[0].BufferRequest = 1;
    
    /* Set the second ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1 = -1;

    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Call the function under test */
    result = VC_Start_Streaming();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Start_StreamingDevice() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED, 
                        "VC_StartStreaming changed status with failure");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
}

/**
 * Test VC_Start_StreamingDevice nominal
 */
void Test_VC_Custom_StartStreamingDevice_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMON success on %s channel %u", "test", 0);

    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to one to only call ioctl queue buffer once */
    VC_AppCustomDevice.Channel[0].BufferRequest = 1;
    
    /* Call the function under test */
    result = VC_Start_StreamingDevice(DeviceID);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(result == expected,"VC_Start_StreamingDevice() did not succeed");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_Start_Streaming()
 **************************************************************************/


/**
 * Test VC_Start_Streaming nominal
 */
void Test_VC_Custom_StartStreaming_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMON success on %s channel %u", "test", 0);
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;

    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to one to only call ioctl queue buffer once */
    VC_AppCustomDevice.Channel[0].BufferRequest = 1;
    
    /* Call the function under test */
    result = VC_Start_Streaming();

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_STREAMING,
                        "VC_Start_Streaming() did not set state to streaming");
    UtAssert_True(result == expected,"VC_Start_StreamingDevice() did not succeed");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
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
    VC_AppCustomDevice.Channel[0].DeviceFd = 7;
    
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
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
        /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno_Value = 6;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].DeviceFd = 7;
    
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
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
        /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Errno_Value = EINTR;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].DeviceFd = 7;
    
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

/**
 * Test VC_Stream_Task() fail select timeout max retry attempts
 */
void Test_VC_Custom_StreamTask_TimeoutError(void)
{
    char returnString[64];
    snprintf(returnString, 64, "VC select timed out");
    char returnString1[128];
    snprintf(returnString1, 128, "VC streaming task exited with return code %d task status (0x%08X)", 
                                -1, (unsigned int)CFE_SUCCESS);
  
    /* Set register child task to succeed */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, CFE_SUCCESS, 1);
    
    /* Set the while loop continue flag to true */
    VC_AppCustomDevice.ContinueFlag = TRUE;
    
    /* Set the mode and state for channel 0*/
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
    /* Set a fake device file descriptor */
    VC_AppCustomDevice.Channel[0].DeviceFd = 7;
    
    /* Set select to timeout return value (0) */
    VC_Platform_Stubs_Returns.VC_Wrap_Select_Return = 0;

    /* Call the function under test */
    VC_Stream_Task();
    
    /* Verfity results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==(VC_BUFFER_TIMEOUTS_ALLOWED + 1),
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
 * Test VC_Devices_Start() fail start streaming
 */
void Test_VC_Custom_DevicesStart_StartStreaming(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    
    uint8 DeviceID = 0;
    
    /* Fail start streaming device */
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_QBUF returned %i on %s channel %u", 5, "test", 0);

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 5;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to its correct value */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;

    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Call the function under test */
    result = VC_Devices_Start();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppCustomDevice.ContinueFlag == FALSE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Start() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED, 
                        "VC_StartStreaming changed status with failure");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
}

/**
 * Test VC_Devices_Start() fail child task creation
 */
void Test_VC_Custom_DevicesStart_ChildTaskCreationFail(void)
{
    /* Set create child task to fail */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);
    
    boolean result = TRUE;
    boolean expected = FALSE;
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMON success on %s channel %u", "test", 0);
    
    uint8 DeviceID = 0;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to its correct value */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;

    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Call the function under test */
    result = VC_Devices_Start();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppCustomDevice.ContinueFlag == FALSE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Start() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_STREAMING, 
                        "VC_Custom_Device status is not the correct value");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
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
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMON success on %s channel %u", "test", 0);
    
    uint8 DeviceID = 0;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer request to its correct value */
    VC_AppCustomDevice.Channel[0].BufferRequest = VC_V4L_BUFFER_REQUEST;

    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_INITIALIZED;
    
    /* Call the function under test */
    result = VC_Devices_Start();
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_True(VC_AppCustomDevice.ContinueFlag == TRUE,
                        "Devices start continue flag not correctly set");
    UtAssert_True(result == expected,"VC_Devices_Start() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_STREAMING, 
                        "VC_Custom_Device status is not the correct value");
    UtAssert_EventSent(VC_DEV_INF_EID, CFE_EVS_INFORMATION, returnString, 
                        "VC_Start_StreamingDevice() failed to raise an event");
}

/**************************************************************************
 * Tests for VC_Stop_StreamingDevice()
 **************************************************************************/

/**
 * Test VC_Stop_StreamingDevice() through VC_Stop_Streaming()
 * fail stream off
 */
void Test_VC_Custom_StopStreamingDevices_StreamOff(void)
{
    int32 result = 0;
    int32 expected = -1; 
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF returned %i on %s channel %u", 7, "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Call the function under test */
    result = VC_Stop_Streaming();

    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not fail");
}

/**
 * Test VC_Stop_StreamingDevice() through VC_Stop_Streaming() nominal
 */
void Test_VC_Custom_StopStreamingDevices_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF success on %s channel %u", "test", 0);
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Call the function under test */
    result = VC_Stop_Streaming();

    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not succeed");
}


/**************************************************************************
 * Tests for VC_Stop_Streaming()
 **************************************************************************/

/**************************************************************************
 * Tests for VC_Devices_Stop()
 **************************************************************************/
 
 /**
 * Test VC_Devices_Stop() fail stop streaming
 */
void Test_VC_Custom_DevicesStop_FailStopStreaming(void)
{
    boolean result = TRUE;
    int32 expected = FALSE; 
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF returned %i on %s channel %u", 7, "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Call the function under test */
    result = VC_Devices_Stop();
    
    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not fail");
}

 /**
 * Test VC_Devices_Stop() nominal
 */
void Test_VC_Custom_DevicesStop_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF success on %s channel %u", "test", 0);
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Call the function under test */
    result = VC_Devices_Stop();
    
    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.ContinueFlag==FALSE,"Continue flag wasn't set");
    UtAssert_True(VC_AppData.AppState==VC_INITIALIZED,"App state isn't initialized");
}


/**************************************************************************
 * Tests for VC_DisableDevice()
 **************************************************************************/
 
 /**
 * Test VC_DisableDevice() nothing enabled
 */
void Test_VC_Custom_DisableDevice_NothingEnabled(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC Device for channel %u is not enabled.", 0);
    
    result = VC_DisableDevice(DeviceID);
    UtAssert_True(result == expected,"VC_DisableDevice() did not fail");
}


 /**
 * Test VC_DisableDevice() nominal
 */
void Test_VC_Custom_DisableDevice_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;
    
    /* Set test device file descriptor a random value */
    VC_AppCustomDevice.Channel[0].DeviceFd = 1;
     
    /* Set device mode to enable */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    
    /* Call the function under test */
    result = VC_DisableDevice(DeviceID);
    
    UtAssert_True(VC_AppCustomDevice.Channel[0].DeviceFd == 0,
                            "VC_DisableDevice() did not reset FD");
    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
}

/**************************************************************************
 * Tests for VC_CleanupDevices()
 **************************************************************************/

 /**
 * Test VC_CleanupDevices() stop streaming fail
 * note reuse of code from first stop streaming device fail test
 */
void Test_VC_Custom_CleanupDevices_StopStreaming(void)
{
    int32 result = 0;
    int32 expected = -1; 
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF returned %i on %s channel %u", 7, "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Call the function under test */
    result = VC_CleanupDevices();

    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_STREAMING,
                        "VC_CleanupDevices() did not set status");
}

/**
 * Test VC_CleanupDevices() nominal
 * 
 */
void Test_VC_Custom_CleanupDevices_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF success on %s channel %u", "test", 0);
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Call the function under test */
    result = VC_CleanupDevices();

    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED,
                        "VC_CleanupDevices() did not set correct status");
}


/**************************************************************************
 * Tests for VC_Devices_Uninit()
 **************************************************************************/
 
/**
 * Test VC_Devices_Uninit() nominal
 * 
 */
void Test_VC_Custom_DevicesUninit_Nominal(void)
{
    boolean result = FALSE;
    boolean expected = TRUE;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF success on %s channel %u", "test", 0);
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Call the function under test */
    result = VC_Devices_Uninit();

    UtAssert_True(result == expected,"VC_DisableDevice() did not succeed");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_INITIALIZED,
                        "VC_CleanupDevices() did not set correct status");
}


/**
 * Test VC_Devices_Uninit() cleanup devices fail
 * 
 */
void Test_VC_Custom_DevicesUninit_Cleanup(void)
{
    boolean result = TRUE;
    boolean expected = FALSE;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_STREAMOFF returned %i on %s channel %u", 7, "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    
    /* Set channel 0 to enabled and initialized */
    VC_AppCustomDevice.Channel[0].Mode = VC_DEVICE_ENABLED;
    VC_AppCustomDevice.Channel[0].Status = VC_DEVICE_STREAMING;

    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Call the function under test */
    result = VC_Devices_Uninit();

    UtAssert_True(result == expected,"VC_Stop_StreamingDevice() did not fail");
    UtAssert_True(VC_AppCustomDevice.Channel[0].Status == VC_DEVICE_STREAMING,
                        "VC_CleanupDevices() did not set status");
}

/**************************************************************************
 * Tests for VC_Send_Buffer()
 **************************************************************************/

/**
 * Test VC_Send_Buffer() buffer dequeue fail
 * 
 */
void Test_VC_Custom_SendBuffer_BufferDequeue(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC VIDIOC_DQBUF returned %i on %s channel %u", 7, "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Set the first ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return = -1;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stop_StreamingDevice() failed to raise an event");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not fail");
}

/**
 * Test VC_Send_Buffer() max buffer size fail
 * 
 */
void Test_VC_Custom_SendBuffer_MaxBuffer(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[64];
    snprintf(returnString, 64, "VC Packet on %s channel %u is too large", "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer dequeued validation check to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 2;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stop_StreamingDevice() failed to raise an event");
    /* TODO currently does not fail if max buffer size exceeded. */
    //UtAssert_True(result == expected,"VC_Send_Buffer() did not fail");
}

/**
 * Test VC_Send_Buffer() dequeued buffer address validation fail
 * 
 */
void Test_VC_Custom_SendBuffer_InvalidAddress(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[128];
    snprintf(returnString, 128, 
        "VC VIDIOC_DQBUF returned unknown user pointer on %s channel %u", 
        "test", 0);
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer dequeued validation check to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 1;
    
    /* Set buffer request to a value > 0 */
    VC_AppCustomDevice.Channel[DeviceID].BufferRequest = 1;
    
    /* Set random pointer address */
    VC_AppCustomDevice.Channel[DeviceID].Buffer_Ptrs[0].ptr = (void *)1;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);
    
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Stop_StreamingDevice() failed to raise an event");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not fail");
}

/**
 * Test VC_Send_Buffer() dequeued buffer fail send data
 * 
 */
void Test_VC_Custom_SendBuffer_SendData(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[128];
    snprintf(returnString, 128, 
        "VC send data failed on %s channel %u", 
        "test", 0);
    
    /* Properly initialize custom transmit data */
    VC_CustomTransmit_InitData();
    
    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer dequeued validation check to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 1;
    
    /* Set buffer request to a value > 0 */
    VC_AppCustomDevice.Channel[DeviceID].BufferRequest = 1;
    
    /* set sendto to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_SendTo_Return = -1;
    
    /* Set the second ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1 = -1;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);
    
    /* Two events because send data will fail */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not fail");
}

/**
 * Test VC_Send_Buffer() queue new buffer failure
 */
void Test_VC_Custom_SendBuffer_BufferQueue(void)
{
    int32 result = 0;
    int32 expected = -1;
    uint8 DeviceID = 0;
    
    char returnString[128];
    snprintf(returnString, 128, 
        "VC VIDIOC_QBUF returned %i on %s channel %u", 7, "test", 0);

    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer dequeued validation check to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 1;
    
    /* Set second ioctl call to fail */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Return1 = -1;
    
    /* Set buffer request to a value > 0 */
    VC_AppCustomDevice.Channel[DeviceID].BufferRequest = 1;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
    UtAssert_EventSent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR, returnString, 
                        "VC_Send_Buffer() failed to raise an event");
    UtAssert_True(result == expected,"VC_Send_Buffer() did not fail");
}

/**
 * Test VC_Send_Buffer() nominal
 */
void Test_VC_Custom_SendBuffer_Nominal(void)
{
    int32 result = -1;
    int32 expected = 0;
    uint8 DeviceID = 0;

    /* Set the test device path */
    strcpy(VC_AppCustomDevice.Channel[0].DevName, "test");
    
    /* Set buffer dequeued validation check to pass */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Struct = 1;
    
    /* Set buffer request to a value > 0 */
    VC_AppCustomDevice.Channel[DeviceID].BufferRequest = 1;
    
    /* Set return error number to true */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno = 1;
    
    /* Set error number to interrupted */
    /* Note this will set errno through multiple calls */
    VC_Platform_Stubs_Returns.VC_Wrap_Ioctl_Errno_Value = 7;
    
    /* Call the function under test */
    result = VC_Send_Buffer(DeviceID);

    UtAssert_True(result == expected,"VC_Send_Buffer() did not succeed");
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
    UtTest_Add(Test_VC_Custom_Ioctl_Error, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_Ioctl_Error");
    UtTest_Add(Test_VC_Custom_Ioctl_Interrupted, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_Ioctl_Interrupted");
    UtTest_Add(Test_VC_Custom_InitDevice_AlreadyInitialized, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitDevice_AlreadyInitialized");
    UtTest_Add(Test_VC_Custom_InitDevice_NullName, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitDevice_NullName");
    UtTest_Add(Test_VC_Custom_InitDevice_InvalidID, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitDevice_InvalidID");
    UtTest_Add(Test_VC_Custom_InitDevice_OpenError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitDevice_OpenError");
    UtTest_Add(Test_VC_Custom_InitDevice_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitDevice_Nominal");
    UtTest_Add(Test_VC_Custom_InitCustomDevices_FailConfigure, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitCustomDevices_FailConfigure");
    UtTest_Add(Test_VC_Custom_InitCustomDevices_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_InitCustomDevices_FailInit");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_CapabilitiesFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_CapabilitiesFail");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_CapabilitiesBuffer, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_CapabilitiesBuffer");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_CapabilitiesStreaming, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_CapabilitiesStreaming");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_FormatFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_FormatFail");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_FormatCheck, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_FormatCheck");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_FormatCheckRes, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_FormatCheckRes");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_FormatCheckSize, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_FormatCheckSize");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_RequestsBuffers, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_RequestsBuffers");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_RequestsBuffersCount, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_RequestsBuffersCount");
    UtTest_Add(Test_VC_Custom_ConfigureDevice_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_ConfigureDevice_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesInit_Fail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesInit_Fail");
    UtTest_Add(Test_VC_Custom_DevicesInit_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesInit_Nominal");
    UtTest_Add(Test_VC_Custom_StartStreamingDevice_BufferQueue, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StartStreamingDevice_BufferQueue");
    UtTest_Add(Test_VC_Custom_StartStreamingDevice_StreamOn, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StartStreamingDevice_StreamOn");
    UtTest_Add(Test_VC_Custom_StartStreamingDevice_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StartStreamingDevice_Nominal");
    UtTest_Add(Test_VC_Custom_StartStreaming_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StartStreaming_Nominal");
    UtTest_Add(Test_VC_Custom_StreamTask_RegisterChildTaskFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_RegisterChildTaskFail");
    UtTest_Add(Test_VC_Custom_StreamTask_NothingEnabled, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_NothingEnabled");            
    UtTest_Add(Test_VC_Custom_StreamTask_SelectError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_SelectError");
    UtTest_Add(Test_VC_Custom_StreamTask_InterruptedError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_InterruptedError");
    UtTest_Add(Test_VC_Custom_StreamTask_TimeoutError, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StreamTask_TimeoutError");
    UtTest_Add(Test_VC_Custom_DevicesStart_StartStreaming, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStart_StartStreaming");
    UtTest_Add(Test_VC_Custom_DevicesStart_ChildTaskCreationFail, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStart_ChildTaskCreationFail");
    UtTest_Add(Test_VC_Custom_DevicesStart_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStart_Nominal");
    UtTest_Add(Test_VC_Custom_StopStreamingDevices_StreamOff, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StopStreamingDevices_StreamOff");
    UtTest_Add(Test_VC_Custom_StopStreamingDevices_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_StopStreamingDevices_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesStop_FailStopStreaming, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStop_FailStopStreaming");
    UtTest_Add(Test_VC_Custom_DevicesStop_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesStop_Nominal");
    UtTest_Add(Test_VC_Custom_DisableDevice_NothingEnabled, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DisableDevice_NothingEnabled");
    UtTest_Add(Test_VC_Custom_DisableDevice_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DisableDevice_Nominal");
    UtTest_Add(Test_VC_Custom_CleanupDevices_StopStreaming, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_CleanupDevices_StopStreaming");
    UtTest_Add(Test_VC_Custom_CleanupDevices_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_CleanupDevices_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesUninit_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesUninit_Nominal");
    UtTest_Add(Test_VC_Custom_DevicesUninit_Cleanup, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_DevicesUninit_Cleanup");
    UtTest_Add(Test_VC_Custom_SendBuffer_BufferDequeue, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_BufferDequeue");
    UtTest_Add(Test_VC_Custom_SendBuffer_MaxBuffer, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_MaxBuffer");
    UtTest_Add(Test_VC_Custom_SendBuffer_InvalidAddress, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_InvalidAddress");
    UtTest_Add(Test_VC_Custom_SendBuffer_SendData, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_SendData");
    UtTest_Add(Test_VC_Custom_SendBuffer_BufferQueue, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_BufferQueue");
    UtTest_Add(Test_VC_Custom_SendBuffer_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Custom_SendBuffer_Nominal");
    UtTest_Add(Test_VC_Devices_InitData_Nominal, VC_Custom_Device_Test_Setup, 
            VC_Custom_Device_Test_TearDown, "Test_VC_Devices_InitData_Nominal");
}
