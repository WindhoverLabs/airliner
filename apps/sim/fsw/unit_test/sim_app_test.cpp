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
#include "sim_app.h"
#include "sim_test_utils.h"
#include "sim_app_test.h"
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

int32 hookCalledCount = 0;

/**************************************************************************
 * Tests for SIM_InitEvent()
 **************************************************************************/

/**
 * Test SIM_InitEvent() with failed CFE_EVS_Register
 */
void Test_SIM_InitEvent_Fail_Register(void)
{
    SIM oSIM;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for SIM_InitPipe()
 **************************************************************************/
/**
 * Test SIM_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_SIM_InitPipe_Fail_CreateSCHPipe(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test SIM_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_SIM_InitPipe_Fail_SubscribeWakeup(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test SIM_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_SIM_InitPipe_Fail_SubscribeSendHK(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test SIM_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_SIM_InitPipe_Fail_CreateCMDPipe(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test SIM_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_SIM_InitPipe_Fail_SubscribeCMD(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test SIM_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_SIM_InitPipe_Fail_CreateDATAPipe(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSIM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for SIM_InitData()
 **************************************************************************/
/**
 * Test SIM_InitData()
 */
void Test_SIM_InitData(void)
{
    SIM oSIM;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oSIM.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for SIM_InitApp()
 **************************************************************************/
/**
 * Test SIM_InitApp(), fail init event
 */
void Test_SIM_InitApp_Fail_InitEvent(void)
{
    SIM oSIM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test SIM_InitApp(), fail init pipe
 */
void Test_SIM_InitApp_Fail_InitPipe(void)
{
    SIM oSIM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test SIM_InitApp(), fail init data.
 * NOTE: no current way to fail SIM_InitData() in default
 */
void Test_SIM_InitApp_Fail_InitData(void)
{
    SIM oSIM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSIM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test SIM_InitApp(), fail init config table
 */
void Test_SIM_InitApp_Fail_InitConfigTbl(void)
{
    SIM oSIM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSIM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test SIM_InitApp(), Nominal
 */
void Test_SIM_InitApp_Nominal(void)
{
    SIM oSIM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSIM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for SIM_AppMain()
 **************************************************************************/
/**
 * Test SIM_AppMain(), Fail RegisterApp
 */
void Test_SIM_AppMain_Fail_RegisterApp(void)
{
    SIM oSIM;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oSIM.AppMain();
}


/**
 * Test SIM_AppMain(), Fail InitApp
 */
void Test_SIM_AppMain_Fail_InitApp(void)
{
    SIM oSIM;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oSIM.AppMain();
}


/**
 * Test SIM_AppMain(), Fail AcquireConfigPtrs
 */
void Test_SIM_AppMain_Fail_AcquireConfigPtrs(void)
{
    SIM oSIM;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oSIM.AppMain();
}


/**
 * Test SIM_AppMain(), Invalid Schedule Message
 */
void Test_SIM_AppMain_InvalidSchMessage(void)
{
    SIM oSIM;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSIM.AppMain();

}


/**
 * Hook to support: SIM_AppMain(), Nominal - SendHK
 */
int32 Test_SIM_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test SIM_AppMain(), Nominal - SendHK
 */
void Test_SIM_AppMain_Nominal_SendHK(void)
{
    SIM oSIM;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SIM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_SIM_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oSIM.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test SIM_AppMain(), Nominal - Wakeup
 */
void Test_SIM_AppMain_Nominal_Wakeup(void)
{
    SIM oSIM;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SIM_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oSIM.AppMain();

}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SIM_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_SIM_InitEvent_Fail_Register, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitEvent_Fail_Register");

    UtTest_Add(Test_SIM_InitPipe_Fail_CreateSCHPipe, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_SIM_InitPipe_Fail_SubscribeWakeup, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_SIM_InitPipe_Fail_SubscribeSendHK, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_SIM_InitPipe_Fail_CreateCMDPipe, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_SIM_InitPipe_Fail_SubscribeCMD, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_SIM_InitPipe_Fail_CreateDATAPipe, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_SIM_InitData, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitData");

    UtTest_Add(Test_SIM_InitApp_Fail_InitEvent, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitApp_Fail_InitEvent");
    UtTest_Add(Test_SIM_InitApp_Fail_InitPipe, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitApp_Fail_InitPipe");
    UtTest_Add(Test_SIM_InitApp_Fail_InitData, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitApp_Fail_InitData");
    UtTest_Add(Test_SIM_InitApp_Fail_InitConfigTbl, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_SIM_InitApp_Nominal, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_InitApp_Nominal");

    UtTest_Add(Test_SIM_AppMain_Fail_RegisterApp, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_SIM_AppMain_Fail_InitApp, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_Fail_InitApp");
    UtTest_Add(Test_SIM_AppMain_Fail_AcquireConfigPtrs, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_SIM_AppMain_InvalidSchMessage, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_InvalidSchMessage");
    UtTest_Add(Test_SIM_AppMain_Nominal_SendHK, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_Nominal_SendHK");
    UtTest_Add(Test_SIM_AppMain_Nominal_Wakeup, SIM_Test_Setup, SIM_Test_TearDown,
               "Test_SIM_AppMain_Nominal_Wakeup");

}


