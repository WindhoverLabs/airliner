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

#include "fac_app_test.hpp"
#include "fac_app.hpp"
#include "fac_test_utils.hpp"
#include <float.h>

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
 * Tests for FAC_InitEvent()
 **************************************************************************/

/**
 * Test FAC_InitEvent() with failed CFE_EVS_Register
 */
void Test_FAC_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for FAC_InitPipe()
 **************************************************************************/
/**
 * Test FAC_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test FAC_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_FAC_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test FAC_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_FAC_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test FAC_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test FAC_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FAC_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test FAC_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for FAC_InitData()
 **************************************************************************/
/**
 * Test FAC_InitData()
 */
void Test_FAC_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Initialize the table so the table pointer is set. */
    oFAC.InitConfigTbl();

    /* Execute the function being tested */
    result = oFAC.InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for FAC_InitApp()
 **************************************************************************/
/**
 * Test FAC_InitApp(), fail init event
 */
void Test_FAC_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test FAC_InitApp(), fail init pipe
 */
void Test_FAC_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test FAC_InitApp(), fail init data.
 * NOTE: no current way to fail FAC_InitData() in default
 */
void Test_FAC_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test FAC_InitApp(), fail init config table
 */
void Test_FAC_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test FAC_InitApp(), Nominal
 */
void Test_FAC_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for FAC_AppMain()
 **************************************************************************/
/**
 * Test FAC_AppMain(), Fail RegisterApp
 */
void Test_FAC_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC_AppMain(), Fail InitApp
 */
void Test_FAC_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC_AppMain(), Fail AcquireConfigPtrs
 */
void Test_FAC_AppMain_Fail_AcquireConfigPtrs(void)
{
    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC_AppMain(), Invalid Schedule Message
 */
void Test_FAC_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Hook to support: FAC_AppMain(), Nominal - SendHK
 */
int32 Test_FAC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test FAC_AppMain(), Nominal - SendHK
 */
void Test_FAC_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_FAC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test FAC_AppMain(), Nominal - Wakeup
 */
void Test_FAC_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Test FAC_AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_FAC_AppMain_ProcessNewData_InvalidMsgID(void)
{
	PX4_ActuatorArmedMsg_t  InMsg;
    int32 DataPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(PX4_ActuatorArmedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InMsg, DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR, "Recvd invalid DATA msgId (0x0000)", "Error Event Sent");
}


void Test_FAC_RunController(void)
{
	/* Set inputs */
#if 0
    oFAC.CVT.VAttSp.Thrust = 0.3333882987f;
#endif
}



void Test_FAC_ControlAttitude(void)
{
	/* Set inputs */
#if 0
    oFAC.CVT.VAttSp.Thrust = 0.3333882987f;
    oFAC.CVT.VAttSp.Q_D[0] = 0.7084835768f;
    oFAC.CVT.VAttSp.Q_D[1] = -0.0144501235f;
    oFAC.CVT.VAttSp.Q_D[2] = -0.0355133303f;
    oFAC.CVT.VAttSp.Q_D[3] = 0.7046850324f;
    oFAC.CVT.ControlState.Q[0] = 0.7083791494f;
    oFAC.CVT.ControlState.Q[1] = -0.0311437733f;
    oFAC.CVT.ControlState.Q[2] = -0.0508509092f;
    oFAC.CVT.ControlState.Q[3] = 0.7033087611f;
    oFAC.m_Params.att_p[0] = 6.0000000000f;
    oFAC.m_Params.att_p[1] = 6.0000000000f;
    oFAC.m_Params.att_p[2] = 2.7999999523f;
    oFAC.CVT.VControlMode.ControlVelocityEnabled = TRUE;
    oFAC.CVT.VControlMode.ControlAutoEnabled = TRUE;
    oFAC.CVT.VControlMode.ControlManualEnabled = FALSE;
    oFAC.CVT.VAttSp.YawSpMoveRate = 0.0000000000f;
    oFAC.m_Params.yaw_ff = 0.5000000000f;
    oFAC.CVT.VAttSp.DisableMcYawControl = FALSE;
    oFAC.m_Params.auto_rate_max[0] = 3.8397247791f;
    oFAC.m_Params.auto_rate_max[1] = 3.8397247791f;
    oFAC.m_Params.auto_rate_max[2] = 0.7853982449f;
    oFAC.m_AngularRatesSetpoint[0] = 0.5145305991f;
    oFAC.m_AngularRatesSetpoint[1] = -0.0270411316f;
    oFAC.m_AngularRatesSetpoint[2] = 0.0020858147f;
    oFAC.m_Params.mc_rate_max[0] = 3.8397247791f;
    oFAC.m_Params.mc_rate_max[1] = 3.8397247791f;
    oFAC.m_Params.mc_rate_max[2] = 3.4906587601f;
    oFAC.m_AngularRatesIntegralError[0] = 0.000000f;
    oFAC.m_AngularRatesIntegralError[1] = 0.000000f;
    oFAC.m_AngularRatesIntegralError[2] = -0.0000685215f;

    /* Run the function */
    oFAC.ControlAttitude(0.0199999996f);

	/* Check outputs */
    UtAssert_True(oFAC.CVT.VAttSp.DisableMcYawControl == FALSE,"oFAC.CVT.VAttSp.DisableMcYawControl = FALSE");
    UtAssert_DoubleCmpAbs(oFAC.m_ThrustSp, 0.33338829875, FLT_EPSILON, "oFAC.m_ThrustSp = 0.33338829875");
    UtAssert_DoubleCmpAbs(oFAC.m_AngularRatesSetpoint[0], 0.27225777507, FLT_EPSILON,"oFAC.m_AngularRatesSetpoint[0] = 0.27225777507");
    UtAssert_DoubleCmpAbs(oFAC.m_AngularRatesSetpoint[1], -0.01081879158, FLT_EPSILON,"oFAC.m_AngularRatesSetpoint[1] = -0.01081879158");
    UtAssert_DoubleCmpAbs(oFAC.m_AngularRatesSetpoint[2], 0.00295280106, FLT_EPSILON,"oFAC.m_AngularRatesSetpoint[2] = 0.00295280106");
    UtAssert_DoubleCmpAbs(oFAC.m_AngularRatesIntegralError[2], -0.00006852150, FLT_EPSILON,"oFAC.m_AngularRatesIntegralError[2] = -0.00006852150");
#endif
}



void Test_FAC_UpdateParams(void)
{

}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void FAC_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_FAC_InitEvent_Fail_Register, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitEvent_Fail_Register");

    UtTest_Add(Test_FAC_InitPipe_Fail_CreateSCHPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeWakeup, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeSendHK, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateCMDPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeCMD, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateDATAPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_FAC_InitData, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitData");

    UtTest_Add(Test_FAC_InitApp_Fail_InitEvent, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitEvent");
    UtTest_Add(Test_FAC_InitApp_Fail_InitPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitPipe");
    UtTest_Add(Test_FAC_InitApp_Fail_InitData, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitData");
    UtTest_Add(Test_FAC_InitApp_Fail_InitConfigTbl, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_FAC_InitApp_Nominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Nominal");

    UtTest_Add(Test_FAC_AppMain_Fail_RegisterApp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_FAC_AppMain_Fail_InitApp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_InitApp");
    UtTest_Add(Test_FAC_AppMain_Fail_AcquireConfigPtrs, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FAC_AppMain_InvalidSchMessage, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_InvalidSchMessage");
    UtTest_Add(Test_FAC_AppMain_Nominal_SendHK, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Nominal_SendHK");
    UtTest_Add(Test_FAC_AppMain_Nominal_Wakeup, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_FAC_RunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_RunController");

    UtTest_Add(Test_FAC_ControlAttitude, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ControlAttitude");

    UtTest_Add(Test_FAC_UpdateParams, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_UpdateParams");


}


