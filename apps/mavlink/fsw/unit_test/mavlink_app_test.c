
#include "mavlink_app_test.h"
#include "mavlink_app.h"
#include "mavlink_test_utils.h"

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
 * Tests for MAVLINK_InitEvent()
 **************************************************************************/

/**
 * Test MAVLINK_InitEvent() with failed CFE_EVS_Register
 */
void Test_MAVLINK_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for MAVLINK_InitPipe()
 **************************************************************************/
/**
 * Test MAVLINK_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_MAVLINK_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test MAVLINK_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_MAVLINK_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test MAVLINK_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_MAVLINK_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test MAVLINK_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_MAVLINK_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test MAVLINK_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_MAVLINK_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test MAVLINK_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_MAVLINK_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = MAVLINK_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for MAVLINK_InitData()
 **************************************************************************/
/**
 * Test MAVLINK_InitData()
 */
void Test_MAVLINK_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = MAVLINK_InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for MAVLINK_InitApp()
 **************************************************************************/
/**
 * Test MAVLINK_InitApp(), fail init event
 */
void Test_MAVLINK_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test MAVLINK_InitApp(), fail init pipe
 */
void Test_MAVLINK_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test MAVLINK_InitApp(), fail init data.
 * NOTE: no current way to fail MAVLINK_InitData() in default
 */
void Test_MAVLINK_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test MAVLINK_InitApp(), fail init config table
 */
void Test_MAVLINK_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test MAVLINK_InitApp(), fail init CDS table
 */
void Test_MAVLINK_InitApp_Fail_InitCDSTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_ES_CDS_INVALID_NAME;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCDS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init CDS table");
}


/**
 * Test MAVLINK_InitApp(), Nominal
 */
void Test_MAVLINK_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = MAVLINK_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for MAVLINK_AppMain()
 **************************************************************************/
/**
 * Test MAVLINK_AppMain(), Fail RegisterApp
 */
void Test_MAVLINK_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    MAVLINK_AppMain();
}


/**
 * Test MAVLINK_AppMain(), Fail InitApp
 */
void Test_MAVLINK_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    MAVLINK_AppMain();
}


/**
 * Test MAVLINK_AppMain(), Fail AcquireConfigPtrs
 */
void Test_MAVLINK_AppMain_Fail_AcquireConfigPtrs(void)
{
    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    MAVLINK_AppMain();
}


/**
 * Test MAVLINK_AppMain(), Invalid Schedule Message
 */
void Test_MAVLINK_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    MAVLINK_AppMain();

}


/**
 * Hook to support: MAVLINK_AppMain(), Nominal - SendHK
 */
int32 Test_MAVLINK_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test MAVLINK_AppMain(), Nominal - SendHK
 */
void Test_MAVLINK_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MAVLINK_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Test_MAVLINK_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    MAVLINK_AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test MAVLINK_AppMain(), Nominal - Wakeup
 */
void Test_MAVLINK_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MAVLINK_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    MAVLINK_AppMain();

}


/**
 * Test MAVLINK_AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_MAVLINK_AppMain_ProcessNewData_InvalidMsgID(void)
{
    MAVLINK_InData_t  InMsg;
    int32 DataPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("MAVLINK_DATA_PIPE");
    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(MAVLINK_InData_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InMsg, DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MAVLINK_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    MAVLINK_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
    UtAssert_EventSent(MAVLINK_MSGID_ERR_EID, CFE_EVS_ERROR, "", "Error Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void MAVLINK_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_MAVLINK_InitEvent_Fail_Register, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitEvent_Fail_Register");

    UtTest_Add(Test_MAVLINK_InitPipe_Fail_CreateSCHPipe, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_MAVLINK_InitPipe_Fail_SubscribeWakeup, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_MAVLINK_InitPipe_Fail_SubscribeSendHK, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_MAVLINK_InitPipe_Fail_CreateCMDPipe, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_MAVLINK_InitPipe_Fail_SubscribeCMD, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_MAVLINK_InitPipe_Fail_CreateDATAPipe, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_MAVLINK_InitData, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitData");

    UtTest_Add(Test_MAVLINK_InitApp_Fail_InitEvent, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Fail_InitEvent");
    UtTest_Add(Test_MAVLINK_InitApp_Fail_InitPipe, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Fail_InitPipe");
    UtTest_Add(Test_MAVLINK_InitApp_Fail_InitData, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Fail_InitData");
    UtTest_Add(Test_MAVLINK_InitApp_Fail_InitConfigTbl, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_MAVLINK_InitApp_Fail_InitCDSTbl, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Fail_InitCDSTbl");
    UtTest_Add(Test_MAVLINK_InitApp_Nominal, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_InitApp_Nominal");

    UtTest_Add(Test_MAVLINK_AppMain_Fail_RegisterApp, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_MAVLINK_AppMain_Fail_InitApp, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_Fail_InitApp");
    UtTest_Add(Test_MAVLINK_AppMain_Fail_AcquireConfigPtrs, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_MAVLINK_AppMain_InvalidSchMessage, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_InvalidSchMessage");
    UtTest_Add(Test_MAVLINK_AppMain_Nominal_SendHK, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_Nominal_SendHK");
    UtTest_Add(Test_MAVLINK_AppMain_Nominal_Wakeup, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_MAVLINK_AppMain_ProcessNewData_InvalidMsgID, MAVLINK_Test_Setup, MAVLINK_Test_TearDown,
               "Test_MAVLINK_AppMain_ProcessNewData_InvalidMsgID");

}


