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
#include "ld_app.h"
#include "ld_test_utils.h"
#include "ld_app_test.h"
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
 * Tests for LD_InitEvent()
 **************************************************************************/

/**
 * Test LD_InitEvent() with failed CFE_EVS_Register
 */
void Test_LD_InitEvent_Fail_Register(void)
{
    LD oLD;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for LD_InitPipe()
 **************************************************************************/
/**
 * Test LD_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateSCHPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_LD_InitPipe_Fail_SubscribeWakeup(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_LD_InitPipe_Fail_SubscribeSendHK(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test LD_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateCMDPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_LD_InitPipe_Fail_SubscribeCMD(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test LD_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateDATAPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for LD_InitData()
 **************************************************************************/
/**
 * Test LD_InitData()
 */
void Test_LD_InitData(void)
{
    LD oLD;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oLD.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for LD_InitApp()
 **************************************************************************/
/**
 * Test LD_InitApp(), fail init event
 */
void Test_LD_InitApp_Fail_InitEvent(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test LD_InitApp(), fail init pipe
 */
void Test_LD_InitApp_Fail_InitPipe(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test LD_InitApp(), fail init data.
 * NOTE: no current way to fail LD_InitData() in default
 */
void Test_LD_InitApp_Fail_InitData(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test LD_InitApp(), fail init config table
 */
void Test_LD_InitApp_Fail_InitConfigTbl(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test LD_InitApp(), Nominal
 */
void Test_LD_InitApp_Nominal(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for LD_AppMain()
 **************************************************************************/
/**
 * Test LD_AppMain(), Fail RegisterApp
 */
void Test_LD_AppMain_Fail_RegisterApp(void)
{
    LD oLD;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oLD.AppMain();
}


/**
 * Test LD_AppMain(), Fail InitApp
 */
void Test_LD_AppMain_Fail_InitApp(void)
{
    LD oLD;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oLD.AppMain();
}


/**
 * Test LD_AppMain(), Fail AcquireConfigPtrs
 */
void Test_LD_AppMain_Fail_AcquireConfigPtrs(void)
{
    LD oLD;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oLD.AppMain();
}


/**
 * Test LD_AppMain(), Invalid Schedule Message
 */
void Test_LD_AppMain_InvalidSchMessage(void)
{
    LD oLD;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

}


/**
 * Hook to support: LD_AppMain(), Nominal - SendHK
 */
int32 Test_LD_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test LD_AppMain(), Nominal - SendHK
 */
void Test_LD_AppMain_Nominal_SendHK(void)
{
    LD oLD;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, LD_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_LD_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test LD_AppMain(), Nominal - Wakeup
 */
void Test_LD_AppMain_Nominal_Wakeup(void)
{
    LD oLD;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, LD_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oLD.AppMain();

}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void LD_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_LD_InitEvent_Fail_Register, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitEvent_Fail_Register");

    UtTest_Add(Test_LD_InitPipe_Fail_CreateSCHPipe, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeWakeup, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeSendHK, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateCMDPipe, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeCMD, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateDATAPipe, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_LD_InitData, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitData");

    UtTest_Add(Test_LD_InitApp_Fail_InitEvent, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitEvent");
    UtTest_Add(Test_LD_InitApp_Fail_InitPipe, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitPipe");
    UtTest_Add(Test_LD_InitApp_Fail_InitData, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitData");
    UtTest_Add(Test_LD_InitApp_Fail_InitConfigTbl, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_LD_InitApp_Nominal, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Nominal");

    UtTest_Add(Test_LD_AppMain_Fail_RegisterApp, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_LD_AppMain_Fail_InitApp, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_InitApp");
    UtTest_Add(Test_LD_AppMain_Fail_AcquireConfigPtrs, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_LD_AppMain_InvalidSchMessage, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_InvalidSchMessage");
    UtTest_Add(Test_LD_AppMain_Nominal_SendHK, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_SendHK");
    UtTest_Add(Test_LD_AppMain_Nominal_Wakeup, LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_Wakeup");

}


