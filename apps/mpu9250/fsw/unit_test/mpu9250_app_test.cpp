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

#include "cfe.h"
#include "mpu9250_app.h"
#include "mpu9250_test_utils.h"
#include "mpu9250_app_test.h"
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
#include "mpu9250_custom_stubs.h"

int32 hookCalledCount = 0;

/**************************************************************************
 * Tests for MPU9250_InitEvent()
 **************************************************************************/

/**
 * Test MPU9250_InitEvent() with failed CFE_EVS_Register
 */
void Test_MPU9250_InitEvent_Fail_Register(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for MPU9250_InitPipe()
 **************************************************************************/
/**
 * Test MPU9250_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_MPU9250_InitPipe_Fail_CreateSCHPipe(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test MPU9250_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_MPU9250_InitPipe_Fail_SubscribeWakeup(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test MPU9250_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_MPU9250_InitPipe_Fail_SubscribeSendHK(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test MPU9250_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_MPU9250_InitPipe_Fail_CreateCMDPipe(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test MPU9250_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_MPU9250_InitPipe_Fail_SubscribeCMD(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test MPU9250_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_MPU9250_InitPipe_Fail_CreateDATAPipe(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPU9250.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for MPU9250_InitData()
 **************************************************************************/
/**
 * Test MPU9250_InitData()
 */
void Test_MPU9250_InitData(void)
{
    MPU9250 oMPU9250;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Value = MPU9250_DEVICE_ID;
    
    /* Pass install delete handler */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_TASKINSTALLDELETEHANDLER_INDEX, 0, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for MPU9250_InitApp()
 **************************************************************************/
/**
 * Test MPU9250_InitApp(), fail init event
 */
void Test_MPU9250_InitApp_Fail_InitEvent(void)
{
    MPU9250 oMPU9250;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test MPU9250_InitApp(), fail init pipe
 */
void Test_MPU9250_InitApp_Fail_InitPipe(void)
{
    MPU9250 oMPU9250;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test MPU9250_InitApp(), fail init data.
 * NOTE: no current way to fail MPU9250_InitData() in default
 */
void Test_MPU9250_InitApp_Fail_InitData(void)
{
    MPU9250 oMPU9250;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Value = MPU9250_DEVICE_ID;
    
    /* Pass install delete handler */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_TASKINSTALLDELETEHANDLER_INDEX, 0, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test MPU9250_InitApp(), fail init config table
 */
void Test_MPU9250_InitApp_Fail_InitConfigTbl(void)
{
    MPU9250 oMPU9250;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test MPU9250_InitApp(), Nominal
 */
void Test_MPU9250_InitApp_Nominal(void)
{
    MPU9250 oMPU9250;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Value = MPU9250_DEVICE_ID;

    /* Execute the function being tested */
    result = oMPU9250.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for MPU9250_AppMain()
 **************************************************************************/
/**
 * Test MPU9250_AppMain(), Fail RegisterApp
 */
void Test_MPU9250_AppMain_Fail_RegisterApp(void)
{
    MPU9250 oMPU9250;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oMPU9250.AppMain();
}


/**
 * Test MPU9250_AppMain(), Fail InitApp
 */
void Test_MPU9250_AppMain_Fail_InitApp(void)
{
    MPU9250 oMPU9250;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oMPU9250.AppMain();
}


/**
 * Test MPU9250_AppMain(), Fail AcquireConfigPtrs
 */
void Test_MPU9250_AppMain_Fail_AcquireConfigPtrs(void)
{
    MPU9250 oMPU9250;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oMPU9250.AppMain();
}


/**
 * Test MPU9250_AppMain(), Invalid Schedule Message
 */
void Test_MPU9250_AppMain_InvalidSchMessage(void)
{
    MPU9250 oMPU9250;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oMPU9250.AppMain();

}


/**
 * Hook to support: MPU9250_AppMain(), Nominal - SendHK
 */
int32 Test_MPU9250_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test MPU9250_AppMain(), Nominal - SendHK
 */
void Test_MPU9250_AppMain_Nominal_SendHK(void)
{
    MPU9250 oMPU9250;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MPU9250_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Pass validate device */
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Value = MPU9250_DEVICE_ID;

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_MPU9250_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oMPU9250.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test MPU9250_AppMain(), Nominal - Wakeup
 */
void Test_MPU9250_AppMain_Nominal_Wakeup(void)
{
    MPU9250 oMPU9250;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MPU9250_MEASURE_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oMPU9250.AppMain();

}


/**
 * Test MPU9250_ReadDevice(), Nominal
 */
void Test_MPU9250_ReadDevice_Nominal(void)
{
    uint16 expected = 10000;
    float expectedCal = 48.0f;
    int16 expectedX = 1000;
    int16 expectedY = 2000;
    int16 expectedZ = 3000;
    uint64 expectedTime = 10000;

    MPU9250 oMPU9250;
    
    oMPU9250.InitApp();
    oMPU9250.InitData();

    /* Set time return value */
    MPU9250_Custom_Returns.MPU9250_Get_Time_Return = expectedTime;
    /* Set accel return values */
    MPU9250_Custom_Returns.MPU9250_Read_Accel_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_Read_Accel_X_Value = expectedX;
    MPU9250_Custom_Returns.MPU9250_Read_Accel_Y_Value = expectedY;
    MPU9250_Custom_Returns.MPU9250_Read_Accel_Z_Value = expectedZ;
    /* Set gyro return values */
    MPU9250_Custom_Returns.MPU9250_Read_Gyro_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_Read_Gyro_X_Value = expectedX;
    MPU9250_Custom_Returns.MPU9250_Read_Gyro_Y_Value = expectedY;
    MPU9250_Custom_Returns.MPU9250_Read_Gyro_Z_Value = expectedZ;
    /* Set temperature return values */
    MPU9250_Custom_Returns.MPU9250_Read_Temp_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_Read_Temp_Value = expected;

    oMPU9250.ReadDevice();
    
    /* Validate time */
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Get_Time_Return == expectedTime, "result != expected");
    /* Validate accel */
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Accel_X_Value == expectedX, "result != expected");
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Accel_Y_Value == expectedY, "result != expected");
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Accel_Z_Value == expectedZ, "result != expected");
    /* Validate gyro */
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Gyro_X_Value == expectedX, "result != expected");
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Gyro_Y_Value == expectedY, "result != expected");
    UtAssert_True(MPU9250_Custom_Returns.MPU9250_Read_Gyro_Z_Value == expectedZ, "result != expected");
    /* Validate temperature */
    UtAssert_True(oMPU9250.SensorAccel.TemperatureRaw == expected, "result != expected");
    UtAssert_True(oMPU9250.SensorGyro.TemperatureRaw == expected, "result != expected");
    UtAssert_True(oMPU9250.SensorAccel.Temperature == expectedCal, "result != expected");
    UtAssert_True(oMPU9250.SensorGyro.Temperature == expectedCal, "result != expected");
}


/**
 * Test MPU9250_ValidateDevice(), Nominal
 */
void Test_MPU9250_ValidateDevice_Nominal(void)
{
    boolean returnBool = FALSE;
    boolean expected = TRUE;
    
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Return = TRUE;
    MPU9250_Custom_Returns.MPU9250_WhoAmI_Value = MPU9250_DEVICE_ID;

    MPU9250 oMPU9250;
    
    oMPU9250.InitApp();
    
    returnBool = oMPU9250.ValidateDevice();

    UtAssert_True(returnBool == expected, "result != expected");
}


/**
 * Test MPU9250_ValidateDevice(), Fail
 */
void Test_MPU9250_ValidateDevice_Fail(void)
{
    boolean returnBool = TRUE;
    boolean expected = FALSE;

    MPU9250 oMPU9250;
    
    oMPU9250.InitApp();
    
    returnBool = oMPU9250.ValidateDevice();

    UtAssert_True(returnBool == expected, "result != expected");
}


/**
 * Test MPU9250_CleanupCallback(), Nominal
 * No way to fail cleanupcallback
 */
void Test_MPU9250_CleanupCallback_Nominal(void)
{
    MPU9250_CleanupCallback();
}


/**
 * Test UpdateParamsFromTable(), Nominal
 */
void Test_MPU9250_UpdateParamsFromTable_Nominal(void)
{
    MPU9250 oMPU9250;
    
    oMPU9250.InitApp();
    oMPU9250.UpdateParamsFromTable();

    /* Verify results */
    UtAssert_True(oMPU9250.m_Params.AccXScale == oMPU9250.ConfigTblPtr->AccXScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.AccYScale == oMPU9250.ConfigTblPtr->AccYScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.AccZScale == oMPU9250.ConfigTblPtr->AccZScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.AccXOffset == oMPU9250.ConfigTblPtr->AccXOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.AccYOffset == oMPU9250.ConfigTblPtr->AccYOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.AccZOffset == oMPU9250.ConfigTblPtr->AccZOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroXScale == oMPU9250.ConfigTblPtr->GyroXScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroYScale == oMPU9250.ConfigTblPtr->GyroYScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroZScale == oMPU9250.ConfigTblPtr->GyroZScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroXOffset == oMPU9250.ConfigTblPtr->GyroXOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroYOffset == oMPU9250.ConfigTblPtr->GyroYOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oMPU9250.m_Params.GyroZOffset == oMPU9250.ConfigTblPtr->GyroZOffset, "m_Param != ConfigTblPtr");
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void MPU9250_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_MPU9250_InitEvent_Fail_Register, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitEvent_Fail_Register");

    UtTest_Add(Test_MPU9250_InitPipe_Fail_CreateSCHPipe, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_MPU9250_InitPipe_Fail_SubscribeWakeup, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_MPU9250_InitPipe_Fail_SubscribeSendHK, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_MPU9250_InitPipe_Fail_CreateCMDPipe, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_MPU9250_InitPipe_Fail_SubscribeCMD, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_MPU9250_InitPipe_Fail_CreateDATAPipe, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_MPU9250_InitData, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitData");

    UtTest_Add(Test_MPU9250_InitApp_Fail_InitEvent, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitApp_Fail_InitEvent");
    UtTest_Add(Test_MPU9250_InitApp_Fail_InitPipe, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitApp_Fail_InitPipe");
    UtTest_Add(Test_MPU9250_InitApp_Fail_InitData, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitApp_Fail_InitData");
    UtTest_Add(Test_MPU9250_InitApp_Fail_InitConfigTbl, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_MPU9250_InitApp_Nominal, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_InitApp_Nominal");

    UtTest_Add(Test_MPU9250_AppMain_Fail_RegisterApp, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_MPU9250_AppMain_Fail_InitApp, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_Fail_InitApp");
    UtTest_Add(Test_MPU9250_AppMain_Fail_AcquireConfigPtrs, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_MPU9250_AppMain_InvalidSchMessage, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_InvalidSchMessage");
    UtTest_Add(Test_MPU9250_AppMain_Nominal_SendHK, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_Nominal_SendHK");
    UtTest_Add(Test_MPU9250_AppMain_Nominal_Wakeup, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_AppMain_Nominal_Wakeup");
              
    UtTest_Add(Test_MPU9250_ReadDevice_Nominal, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_ReadDevice_Nominal");
    UtTest_Add(Test_MPU9250_ValidateDevice_Nominal, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_ValidateDevice_Nominal");
    UtTest_Add(Test_MPU9250_ValidateDevice_Fail, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_ValidateDevice_Fail");
    UtTest_Add(Test_MPU9250_CleanupCallback_Nominal, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_CleanupCallback_Nominal");
    UtTest_Add(Test_MPU9250_UpdateParamsFromTable_Nominal, MPU9250_Test_Setup, MPU9250_Test_TearDown,
               "Test_MPU9250_UpdateParamsFromTable_Nominal");

}


