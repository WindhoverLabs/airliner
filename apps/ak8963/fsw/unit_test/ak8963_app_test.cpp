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
#include "ak8963_app.h"
#include "ak8963_test_utils.h"
#include "ak8963_app_test.h"
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
#include "ak8963_custom_stubs.h"

int32 hookCalledCount = 0;

/**************************************************************************
 * Tests for AK8963_InitEvent()
 **************************************************************************/

/**
 * Test AK8963_InitEvent() with failed CFE_EVS_Register
 */
void Test_AK8963_InitEvent_Fail_Register(void)
{
    AK8963 oAK8963;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for AK8963_InitPipe()
 **************************************************************************/
/**
 * Test AK8963_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_AK8963_InitPipe_Fail_CreateSCHPipe(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test AK8963_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_AK8963_InitPipe_Fail_SubscribeWakeup(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test AK8963_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_AK8963_InitPipe_Fail_SubscribeSendHK(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test AK8963_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_AK8963_InitPipe_Fail_CreateCMDPipe(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test AK8963_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_AK8963_InitPipe_Fail_SubscribeCMD(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test AK8963_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_AK8963_InitPipe_Fail_CreateDATAPipe(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oAK8963.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for AK8963_InitData()
 **************************************************************************/
/**
 * Test AK8963_InitData()
 */
void Test_AK8963_InitData(void)
{
    AK8963 oAK8963;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    AK8963_Custom_Returns.AK8963_WhoAmI_Return = TRUE;
    AK8963_Custom_Returns.AK8963_WhoAmI_Value = AK8963_WAI_ID;
    
    /* Pass install delete handler */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_TASKINSTALLDELETEHANDLER_INDEX, 0, 1);

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for AK8963_InitApp()
 **************************************************************************/
/**
 * Test AK8963_InitApp(), fail init event
 */
void Test_AK8963_InitApp_Fail_InitEvent(void)
{
    AK8963 oAK8963;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test AK8963_InitApp(), fail init pipe
 */
void Test_AK8963_InitApp_Fail_InitPipe(void)
{
    AK8963 oAK8963;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test AK8963_InitApp(), fail init data.
 * NOTE: no current way to fail AK8963_InitData() in default
 */
void Test_AK8963_InitApp_Fail_InitData(void)
{
    AK8963 oAK8963;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    AK8963_Custom_Returns.AK8963_WhoAmI_Return = TRUE;
    AK8963_Custom_Returns.AK8963_WhoAmI_Value = AK8963_WAI_ID;
    
    /* Pass install delete handler */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_TASKINSTALLDELETEHANDLER_INDEX, 0, 1);

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test AK8963_InitApp(), fail init config table
 */
void Test_AK8963_InitApp_Fail_InitConfigTbl(void)
{
    AK8963 oAK8963;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test AK8963_InitApp(), Nominal
 */
void Test_AK8963_InitApp_Nominal(void)
{
    AK8963 oAK8963;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Pass validate device */
    AK8963_Custom_Returns.AK8963_WhoAmI_Return = TRUE;
    AK8963_Custom_Returns.AK8963_WhoAmI_Value = AK8963_WAI_ID;

    /* Execute the function being tested */
    result = oAK8963.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for AK8963_AppMain()
 **************************************************************************/
/**
 * Test AK8963_AppMain(), Fail RegisterApp
 */
void Test_AK8963_AppMain_Fail_RegisterApp(void)
{
    AK8963 oAK8963;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oAK8963.AppMain();
}


/**
 * Test AK8963_AppMain(), Fail InitApp
 */
void Test_AK8963_AppMain_Fail_InitApp(void)
{
    AK8963 oAK8963;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oAK8963.AppMain();
}


/**
 * Test AK8963_AppMain(), Fail AcquireConfigPtrs
 */
void Test_AK8963_AppMain_Fail_AcquireConfigPtrs(void)
{
    AK8963 oAK8963;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oAK8963.AppMain();
}


/**
 * Test AK8963_AppMain(), Invalid Schedule Message
 */
void Test_AK8963_AppMain_InvalidSchMessage(void)
{
    AK8963 oAK8963;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAK8963.AppMain();

}


/**
 * Hook to support: AK8963_AppMain(), Nominal - SendHK
 */
int32 Test_AK8963_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test AK8963_AppMain(), Nominal - SendHK
 */
void Test_AK8963_AppMain_Nominal_SendHK(void)
{
    AK8963 oAK8963;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AK8963_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Pass validate device */
    AK8963_Custom_Returns.AK8963_WhoAmI_Return = TRUE;
    AK8963_Custom_Returns.AK8963_WhoAmI_Value = AK8963_WAI_ID;

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_AK8963_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oAK8963.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test AK8963_AppMain(), Nominal - Wakeup
 */
void Test_AK8963_AppMain_Nominal_Wakeup(void)
{
    AK8963 oAK8963;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AK8963_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oAK8963.AppMain();

}


/**
 * TODO Test AK8963_ReadDevice(), Nominal
 */
void Test_AK8963_ReadDevice_Nominal(void)
{
    int16 expectedX = 1000;
    int16 expectedY = 2000;
    int16 expectedZ = 3000;
    uint64 expectedTime = 10000;

    AK8963 oAK8963;
    
    oAK8963.InitApp();
    oAK8963.InitData();

    /* Set time return value */
    //AK8963_Custom_Returns.AK8963_Get_Time_Return = expectedTime;
    /* Set TODO Mag return values */
    //AK8963_Custom_Returns.AK8963_Read_Mag_Return = TRUE;
    //AK8963_Custom_Returns.AK8963_Read_Mag_X_Value = expectedX;
    //AK8963_Custom_Returns.AK8963_Read_Mag_Y_Value = expectedY;
    //AK8963_Custom_Returns.AK8963_Read_Mag_Z_Value = expectedZ;

    oAK8963.ReadDevice();

    /* Validate time */
    //UtAssert_True(AK8963_Custom_Returns.AK8963_Get_Time_Return == expectedTime, "result != expected");
    /* Validate Mag */
    //UtAssert_True(AK8963_Custom_Returns.AK8963_Read_Mag_X_Value == expectedX, "result != expected");
    //UtAssert_True(AK8963_Custom_Returns.AK8963_Read_Mag_Y_Value == expectedY, "result != expected");
    //UtAssert_True(AK8963_Custom_Returns.AK8963_Read_Mag_Z_Value == expectedZ, "result != expected");
}


/**
 * Test AK8963_ValidateDevice(), Nominal
 */
void Test_AK8963_ValidateDevice_Nominal(void)
{
    boolean returnBool = FALSE;
    boolean expected = TRUE;
    
    AK8963_Custom_Returns.AK8963_WhoAmI_Return = TRUE;
    AK8963_Custom_Returns.AK8963_WhoAmI_Value = AK8963_WAI_ID;

    AK8963 oAK8963;
    
    oAK8963.InitApp();
    
    returnBool = oAK8963.ValidateDevice();

    UtAssert_True(returnBool == expected, "result != expected");
}


/**
 * Test AK8963_ValidateDevice(), Fail
 */
void Test_AK8963_ValidateDevice_Fail(void)
{
    boolean returnBool = TRUE;
    boolean expected = FALSE;

    AK8963 oAK8963;
    
    oAK8963.InitApp();
    
    returnBool = oAK8963.ValidateDevice();

    UtAssert_True(returnBool == expected, "result != expected");
}


/**
 * Test AK8963_CleanupCallback(), Nominal
 * No way to fail cleanupcallback
 */
void Test_AK8963_CleanupCallback_Nominal(void)
{
    AK8963_CleanupCallback();
}


/**
 * Test UpdateParamsFromTable(), Nominal
 */
void Test_AK8963_UpdateParamsFromTable_Nominal(void)
{
    AK8963 oAK8963;
    
    oAK8963.InitApp();
    oAK8963.UpdateParamsFromTable();

    /* Verify results */
    UtAssert_True(oAK8963.m_Params.MagXScale == oAK8963.ConfigTblPtr->MagXScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oAK8963.m_Params.MagYScale == oAK8963.ConfigTblPtr->MagYScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oAK8963.m_Params.MagZScale == oAK8963.ConfigTblPtr->MagZScale, "m_Param != ConfigTblPtr");
    UtAssert_True(oAK8963.m_Params.MagXOffset == oAK8963.ConfigTblPtr->MagXOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oAK8963.m_Params.MagYOffset == oAK8963.ConfigTblPtr->MagYOffset, "m_Param != ConfigTblPtr");
    UtAssert_True(oAK8963.m_Params.MagZOffset == oAK8963.ConfigTblPtr->MagZOffset, "m_Param != ConfigTblPtr");
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void AK8963_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_AK8963_InitEvent_Fail_Register, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitEvent_Fail_Register");

    UtTest_Add(Test_AK8963_InitPipe_Fail_CreateSCHPipe, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_AK8963_InitPipe_Fail_SubscribeWakeup, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_AK8963_InitPipe_Fail_SubscribeSendHK, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_AK8963_InitPipe_Fail_CreateCMDPipe, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_AK8963_InitPipe_Fail_SubscribeCMD, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_AK8963_InitPipe_Fail_CreateDATAPipe, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_AK8963_InitData, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitData");

    UtTest_Add(Test_AK8963_InitApp_Fail_InitEvent, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitApp_Fail_InitEvent");
    UtTest_Add(Test_AK8963_InitApp_Fail_InitPipe, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitApp_Fail_InitPipe");
    UtTest_Add(Test_AK8963_InitApp_Fail_InitData, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitApp_Fail_InitData");
    UtTest_Add(Test_AK8963_InitApp_Fail_InitConfigTbl, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_AK8963_InitApp_Nominal, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_InitApp_Nominal");

    UtTest_Add(Test_AK8963_AppMain_Fail_RegisterApp, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_AK8963_AppMain_Fail_InitApp, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_Fail_InitApp");
    UtTest_Add(Test_AK8963_AppMain_Fail_AcquireConfigPtrs, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_AK8963_AppMain_InvalidSchMessage, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_InvalidSchMessage");
    UtTest_Add(Test_AK8963_AppMain_Nominal_SendHK, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_Nominal_SendHK");
    UtTest_Add(Test_AK8963_AppMain_Nominal_Wakeup, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_AppMain_Nominal_Wakeup");
              
    UtTest_Add(Test_AK8963_ReadDevice_Nominal, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_ReadDevice_Nominal");
    UtTest_Add(Test_AK8963_ValidateDevice_Nominal, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_ValidateDevice_Nominal");
    UtTest_Add(Test_AK8963_ValidateDevice_Fail, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_ValidateDevice_Fail");
    UtTest_Add(Test_AK8963_CleanupCallback_Nominal, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_CleanupCallback_Nominal");
    UtTest_Add(Test_AK8963_UpdateParamsFromTable_Nominal, AK8963_Test_Setup, AK8963_Test_TearDown,
               "Test_AK8963_UpdateParamsFromTable_Nominal");

}


