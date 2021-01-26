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
#include "qae_app.hpp"
#include "qae_test_utils.h"
#include "qae_app_test.h"
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

#include <math/Quaternion.hpp>
#include <math/Vector3F.hpp>
#include <math/Matrix3F3.hpp>
#include <math/Matrix4F4.hpp>
#include <float.h>

int32 hookCalledCount = 0;

/**************************************************************************
 * Tests for QAE_InitEvent()
 **************************************************************************/

/**
 * Test QAE_InitEvent() with failed CFE_EVS_Register
 */
void Test_QAE_InitEvent_Fail_Register(void)
{
    QAE oQAE;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for QAE_InitPipe()
 **************************************************************************/
/**
 * Test QAE_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_QAE_InitPipe_Fail_CreateSCHPipe(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test QAE_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_QAE_InitPipe_Fail_SubscribeWakeup(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test QAE_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_QAE_InitPipe_Fail_SubscribeSendHK(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test QAE_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_QAE_InitPipe_Fail_CreateCMDPipe(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test QAE_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_QAE_InitPipe_Fail_SubscribeCMD(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test QAE_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_QAE_InitPipe_Fail_CreateDATAPipe(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oQAE.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for QAE_InitData()
 **************************************************************************/
/**
 * Test QAE_InitData()
 */
void Test_QAE_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    QAE oQAE;

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for QAE_InitApp()
 **************************************************************************/
/**
 * Test QAE_InitApp(), fail init event
 */
void Test_QAE_InitApp_Fail_InitEvent(void)
{
    QAE oQAE;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test QAE_InitApp(), fail init pipe
 */
void Test_QAE_InitApp_Fail_InitPipe(void)
{
    QAE oQAE;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test QAE_InitApp(), fail init data.
 * NOTE: no current way to fail QAE_InitData() in default
 */
void Test_QAE_InitApp_Fail_InitData(void)
{
    QAE oQAE;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test QAE_InitApp(), fail init config table
 */
void Test_QAE_InitApp_Fail_InitConfigTbl(void)
{
    QAE oQAE;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test QAE_InitApp(), Nominal
 */
void Test_QAE_InitApp_Nominal(void)
{
    QAE oQAE;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oQAE.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for QAE_AppMain()
 **************************************************************************/
/**
 * Test QAE_AppMain(), Fail RegisterApp
 */
void Test_QAE_AppMain_Fail_RegisterApp(void)
{
    QAE oQAE;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oQAE.AppMain();
}


/**
 * Test QAE_AppMain(), Fail InitApp
 */
void Test_QAE_AppMain_Fail_InitApp(void)
{
    QAE oQAE;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oQAE.AppMain();
}


/**
 * Test QAE_AppMain(), Fail AcquireConfigPtrs
 */
void Test_QAE_AppMain_Fail_AcquireConfigPtrs(void)
{
    QAE oQAE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oQAE.AppMain();
}


/**
 * Test QAE_AppMain(), Invalid Schedule Message
 */
void Test_QAE_AppMain_InvalidSchMessage(void)
{
    QAE oQAE;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oQAE.AppMain();

}


/**
 * Hook to support: QAE_AppMain(), Nominal - SendHK
 */
int32 Test_QAE_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test QAE_AppMain(), Nominal - SendHK
 */
void Test_QAE_AppMain_Nominal_SendHK(void)
{
    QAE oQAE;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, QAE_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_QAE_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oQAE.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test QAE_AppMain(), Nominal - Wakeup
 */
void Test_QAE_AppMain_Nominal_Wakeup(void)
{
    QAE oQAE;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, QAE_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oQAE.AppMain();

}


/**
 * Test QAE::InitEstimateAttitude Nominal
 */
void Test_QAE_InitEstimateAttitude_Nominal(void)
{
    boolean return_bool = FALSE;
    /* Data taken from PX4 */
    math::Vector3F accelInput(-0.001376f, 0.017657f, -0.116889f);
    math::Vector3F magInput(-0.005498f,  -0.169592f, -0.450869f);
    math::Quaternion expectedQ(0.704510f, -0.048778f, -0.057120, 0.705708);

    QAE oQAE;
    oQAE.InitApp();
    oQAE.m_Accel = accelInput;
    oQAE.m_Mag = magInput;
    
    return_bool = oQAE.InitEstimateAttitude();

    //printf("oQAE.m_Quaternion[0] %f\n", oQAE.m_Quaternion[0]);
    //printf("oQAE.m_Quaternion[1] %f\n", oQAE.m_Quaternion[1]);
    //printf("oQAE.m_Quaternion[2] %f\n", oQAE.m_Quaternion[2]);
    //printf("oQAE.m_Quaternion[3] %f\n", oQAE.m_Quaternion[3]);

    UtAssert_True(fabs(oQAE.m_Quaternion[0] - expectedQ[0]) < 0.00001f, "InitEstimateAttitude Nominal m_Quaternion[0] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[1] - expectedQ[1]) < 0.00001f, "InitEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[2] - expectedQ[2]) < 0.00001f, "InitEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[3] - expectedQ[3]) < 0.00001f, "InitEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(return_bool == TRUE, "InitEstimateAttitude return_bool == TRUE");
}


/**
 * Test QAE::UpdateMagDeclination initial update Nominal
 */
void Test_QAE_UpdateMagDeclination_InitialNominal(void)
{
    float old_declination;
    float new_declination;

    QAE oQAE;
    oQAE.InitApp();

    new_declination = oQAE.ConfigTblPtr->ATT_MAG_DECL + 1.0f;

    /* Set estimator state to unintialized */
    oQAE.HkTlm.EstimatorState = QAE_EST_UNINITIALIZED;
    
    oQAE.UpdateMagDeclination(new_declination);
    
    UtAssert_DoubleCmpAbs(oQAE.m_MagDeclination, new_declination, FLT_EPSILON, "oQAE.m_MagDeclination == expected_declination");
    
    /* Set estimator state to intialized */
    oQAE.HkTlm.EstimatorState = QAE_EST_INITIALIZED;
    
    oQAE.UpdateMagDeclination(new_declination);
    
    UtAssert_True(oQAE.m_MagDeclination == new_declination, "oQAE.m_MagDeclination == expected_declination");
    
}

/* TODO */
/**
 * Test QAE::UpdateMagDeclination immediate update Nominal
 */
void Test_QAE_UpdateMagDeclination_ImmediateNominal(void)
{
    
}


/**
 * Test QAE::UpdateEstimateAttitude Nominal
 */
void Test_QAE_UpdateEstimateAttitude_Nominal(void)
{
    math::Quaternion existingQ(0.5584248900f, -0.0916596875f, -0.1673399657f, 0.8073150516f);
    math::Vector3F gyroInput(-0.0014175611f, -0.0097235963f, -0.0023968932f);
    math::Vector3F gyroBiasInput(0.0004477855f, 0.0004819505f, 0.0005668105f);
    math::Vector3F ratesInput(0.0012337452f, -0.0073956200f, -0.0015565471f);
    math::Vector3F accelInput(-0.1689540148f, 3.6650042534f , -9.3518390656f);
    math::Vector3F magInput(-0.0778940916f, -0.0357517637f,  -0.4682809114f);
    math::Vector3F posAccInput(-0.0186713543f, 0.0805401132f,  -0.0707371980f);

    float dtInput = 0.0038930001f;
    math::Quaternion expectedQ(0.5584318042f, -0.0916518793f, -0.1673415601f, 0.8073107004f);
    math::Vector3F expectedR(-0.0009688014f, -0.0092391772f, -0.0018312578f);
    math::Vector3F expectedGB(0.0004487596f, 0.0004844195f, 0.0005656354f);
    boolean returnBool = FALSE;

    QAE oQAE;
    oQAE.InitApp();
    oQAE.m_Accel = accelInput;
    oQAE.m_Mag = magInput;
    oQAE.m_Gyro = gyroInput;
    oQAE.m_GyroBias = gyroBiasInput;
    oQAE.m_Rates = ratesInput;
    oQAE.m_PositionAcc = posAccInput;
    oQAE.m_Quaternion = existingQ;
    oQAE.HkTlm.EstimatorState = QAE_EST_INITIALIZED;
    oQAE.HkTlm.State = QAE_SENSOR_DATA_RCVD;
    
    returnBool = oQAE.UpdateEstimateAttitude(dtInput);
    
    //printf("oQAE.m_Quaternion[0] %0.10f\n", oQAE.m_Quaternion[0]);
    //printf("oQAE.m_Quaternion[1] %0.10f\n", oQAE.m_Quaternion[1]);
    //printf("oQAE.m_Quaternion[2] %0.10f\n", oQAE.m_Quaternion[2]);
    //printf("oQAE.m_Quaternion[3] %0.10f\n", oQAE.m_Quaternion[3]);
    //printf("oQAE.m_Rates[0] %0.10f\n", oQAE.m_Rates[0]);
    //printf("oQAE.m_Rates[1] %0.10f\n", oQAE.m_Rates[1]);
    //printf("oQAE.m_Rates[2] %0.10f\n", oQAE.m_Rates[2]);
    //printf("oQAE.m_GyroBias[0] %0.10f\n", oQAE.m_GyroBias[0]);
    //printf("oQAE.m_GyroBias[1] %0.10f\n", oQAE.m_GyroBias[1]);
    //printf("oQAE.m_GyroBias[2] %0.10f\n", oQAE.m_GyroBias[2]);
    
    UtAssert_True(fabs(oQAE.m_Quaternion[0] - expectedQ[0]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Quaternion[0] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[1] - expectedQ[1]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[2] - expectedQ[2]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(fabs(oQAE.m_Quaternion[3] - expectedQ[3]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Quaternion[1] failed");
    UtAssert_True(fabs(oQAE.m_Rates[0] - expectedR[0]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Rates[0] failed");
    UtAssert_True(fabs(oQAE.m_Rates[1] - expectedR[1]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Rates[1] failed");
    UtAssert_True(fabs(oQAE.m_Rates[2] - expectedR[2]) < 0.00001f, "UpdateEstimateAttitude Nominal m_Rates[1] failed");
    UtAssert_True(fabs(oQAE.m_GyroBias[0] - expectedGB[0]) < 0.00001f, "UpdateEstimateAttitude Nominal m_GyroBias[0] failed");
    UtAssert_True(fabs(oQAE.m_GyroBias[1] - expectedGB[1]) < 0.00001f, "UpdateEstimateAttitude Nominal m_GyroBias[1] failed");
    UtAssert_True(fabs(oQAE.m_GyroBias[2] - expectedGB[2]) < 0.00001f, "UpdateEstimateAttitude Nominal m_GyroBias[1] failed");
    
    UtAssert_True(returnBool == TRUE, "UpdateEstimateAttitude return_bool == TRUE");
}


/* TODO */
/**
 * Test QAE::EstimateAttitude Nominal
 */
void Test_QAE_EstimateAttitude_Nominal(void)
{
    //math::Vector3F gyroInput(0.000244f, 0.001586f, 0.007006f);
    //math::Vector3F accelInput(0.702764f, 3.002803f , -9.637285f);
    //math::Vector3F magInput(0.054984f, -0.103588f, -0.463699f);
    //math::Quaternion existingQ(0.753631f, -0.127809f, -0.071840f, 0.640737f);
    //float dtInput = 0.003960f;
    
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void QAE_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_QAE_InitEvent_Fail_Register, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitEvent_Fail_Register");

    UtTest_Add(Test_QAE_InitPipe_Fail_CreateSCHPipe, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_QAE_InitPipe_Fail_SubscribeWakeup, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_QAE_InitPipe_Fail_SubscribeSendHK, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_QAE_InitPipe_Fail_CreateCMDPipe, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_QAE_InitPipe_Fail_SubscribeCMD, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_QAE_InitPipe_Fail_CreateDATAPipe, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_QAE_InitData, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitData");

    UtTest_Add(Test_QAE_InitApp_Fail_InitEvent, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitApp_Fail_InitEvent");
    UtTest_Add(Test_QAE_InitApp_Fail_InitPipe, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitApp_Fail_InitPipe");
    UtTest_Add(Test_QAE_InitApp_Fail_InitData, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitApp_Fail_InitData");
    UtTest_Add(Test_QAE_InitApp_Fail_InitConfigTbl, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_QAE_InitApp_Nominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitApp_Nominal");

    UtTest_Add(Test_QAE_AppMain_Fail_RegisterApp, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_QAE_AppMain_Fail_InitApp, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_Fail_InitApp");
    UtTest_Add(Test_QAE_AppMain_Fail_AcquireConfigPtrs, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_QAE_AppMain_InvalidSchMessage, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_InvalidSchMessage");
    UtTest_Add(Test_QAE_AppMain_Nominal_SendHK, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_Nominal_SendHK");
    UtTest_Add(Test_QAE_AppMain_Nominal_Wakeup, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_QAE_InitEstimateAttitude_Nominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_InitEstimateAttitude_Nominal");
    UtTest_Add(Test_QAE_UpdateMagDeclination_InitialNominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_UpdateMagDeclination_InitialNominal");
    UtTest_Add(Test_QAE_UpdateMagDeclination_ImmediateNominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_UpdateMagDeclination_ImmediateNominal");
    UtTest_Add(Test_QAE_UpdateEstimateAttitude_Nominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_UpdateEstimateAttitude_Nominal");
    UtTest_Add(Test_QAE_EstimateAttitude_Nominal, QAE_Test_Setup, QAE_Test_TearDown,
               "Test_QAE_EstimateAttitude_Nominal");
}


