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
#include "mpc_app.h"
#include "mpc_test_utils.h"
#include "mpc_app_test.h"
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
#include "math/Quaternion.hpp"
#include "math/Matrix3F3.hpp"
#include <float.h>
#include "stubs/ut_mpc_custom_stubs.h"

int32 hookCalledCount = 0;


/**************************************************************************
 * Tests for MPC_InitEvent()
 **************************************************************************/

/**
 * Test MPC_InitEvent() with failed CFE_EVS_Register
 */
void Test_MPC_InitEvent_Fail_Register(void)
{
    MPC oMPC;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for MPC_InitPipe()
 **************************************************************************/
/**
 * Test MPC_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_MPC_InitPipe_Fail_CreateSCHPipe(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test MPC_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_MPC_InitPipe_Fail_SubscribeWakeup(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test MPC_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_MPC_InitPipe_Fail_SubscribeSendHK(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test MPC_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_MPC_InitPipe_Fail_CreateCMDPipe(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test MPC_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_MPC_InitPipe_Fail_SubscribeCMD(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test MPC_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_MPC_InitPipe_Fail_CreateDATAPipe(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oMPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for MPC_InitData()
 **************************************************************************/
/**
 * Test MPC_InitData()
 */
void Test_MPC_InitData(void)
{
    MPC oMPC;

    oMPC.InitConfigTbl();

    /* Execute the function being tested */
    oMPC.InitData();
}


/**************************************************************************
 * Tests for MPC_InitApp()
 **************************************************************************/
/**
 * Test MPC_InitApp(), fail init event
 */
void Test_MPC_InitApp_Fail_InitEvent(void)
{
    MPC oMPC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test MPC_InitApp(), fail init pipe
 */
void Test_MPC_InitApp_Fail_InitPipe(void)
{
    MPC oMPC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test MPC_InitApp(), fail init data.
 * NOTE: no current way to fail MPC_InitData() in default
 */
void Test_MPC_InitApp_Fail_InitData(void)
{
    MPC oMPC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oMPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test MPC_InitApp(), fail init config table
 */
void Test_MPC_InitApp_Fail_InitConfigTbl(void)
{
    MPC oMPC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oMPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test MPC_InitApp(), Nominal
 */
void Test_MPC_InitApp_Nominal(void)
{
    MPC oMPC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oMPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for MPC_AppMain()
 **************************************************************************/
/**
 * Test MPC_AppMain(), Fail RegisterApp
 */
void Test_MPC_AppMain_Fail_RegisterApp(void)
{
    MPC oMPC;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oMPC.AppMain();
}


/**
 * Test MPC_AppMain(), Fail InitApp
 */
void Test_MPC_AppMain_Fail_InitApp(void)
{
    MPC oMPC;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oMPC.AppMain();
}


/**
 * Test MPC_AppMain(), Fail AcquireConfigPtrs
 */
void Test_MPC_AppMain_Fail_AcquireConfigPtrs(void)
{
    MPC oMPC;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oMPC.AppMain();
}


/**
 * Test MPC_AppMain(), Invalid Schedule Message
 */
void Test_MPC_AppMain_InvalidSchMessage(void)
{
    MPC oMPC;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oMPC.AppMain();

}


/**
 * Hook to support: MPC_AppMain(), Nominal - SendHK
 */
int32 Test_MPC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test MPC_AppMain(), Nominal - SendHK
 */
void Test_MPC_AppMain_Nominal_SendHK(void)
{
    MPC oMPC;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MPC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_MPC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oMPC.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test MPC_AppMain(), Nominal - Wakeup
 */
void Test_MPC_AppMain_Nominal_Wakeup(void)
{
    MPC oMPC;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, MPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    //oMPC.AppMain();

}


/**
 * Test MPC_AppMain(), Nominal - ProcessControlStateMsg
 */
void Test_MPC_AppMain_Nominal_ProcessControlStateMsg(void)
{
    MPC oMPC;
    PX4_ControlStateMsg_t controlState = {};
    int32 schPipe;

    schPipe = Ut_CFE_SB_CreatePipe("MPC_SCH_PIPE");

    CFE_SB_InitMsg (&controlState, 0x0000, sizeof(controlState), TRUE);

    controlState.Timestamp = 86440828023;
    controlState.AccX = -0.236284539;
    controlState.AccY = 0.101594232;
    controlState.AccZ = -9.76688671;
    controlState.VelX = 0.000539890432;
    controlState.VelY = 0.0182482861;
    controlState.VelZ = 0.00519393012;
    controlState.PosX = -0.00807033759;
    controlState.PosY = -0.000570089556;
    controlState.PosZ = 0.0179239418;
    controlState.Airspeed = 0;
    controlState.VelVariance[0] = 0.0f;
    controlState.VelVariance[1] = 0.0f;
    controlState.VelVariance[2] = 0.0f;
    controlState.PosVariance[0] = 0.0f;
    controlState.PosVariance[1] = 0.0f;
    controlState.PosVariance[2] = 0.0f;
    controlState.Q[0] = 0.712293863;
    controlState.Q[1] = 0.00346049992;
    controlState.Q[2] = -0.0159233119;
    controlState.Q[3] = 0.701692224;
    controlState.DeltaQReset[0] = 0.999934673;
    controlState.DeltaQReset[1] = 0.000154913403;
    controlState.DeltaQReset[2] = -0.00022063707;
    controlState.DeltaQReset[3] = 0.0114225745;
    controlState.RollRate = 0.00590410549;
    controlState.PitchRate = 0.00479567284;
    controlState.YawRate = 0.000454715308;
    controlState.HorzAccMag = 0.158787385;
    controlState.RollRateBias = -0.00138732162;
    controlState.PitchRateBias = -0.00102514029;
    controlState.YawRateBias = -0.00102514029;
    controlState.AirspeedValid = false;
    controlState.QuatResetCounter = 2;

    Ut_CFE_SB_AddMsgToPipe(&controlState, schPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oMPC.AppMain();

    /* Verify results */
    UtAssert_True(oMPC.m_HeadingResetCounter == 0,"HeadingResetCounter == 0");

//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[0][0], 0.0147489905, FLT_EPSILON, "oMPC.m_Rotation[0][0] == 0.0147489905");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[0][1], -0.999732316, FLT_EPSILON, "oMPC.m_Rotation[0][1] == -0.999732316");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[0][2], -0.0178277437, FLT_EPSILON, "oMPC.m_Rotation[0][2] == -0.0178277437");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[1][0], 0.999511898, FLT_EPSILON, "oMPC.m_Rotation[1][0] == 0.999511898");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[1][1], 0.0152321458, FLT_EPSILON, "oMPC.m_Rotation[1][1] == 0.0152321458");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[1][2], -0.0272763148, FLT_EPSILON, "oMPC.m_Rotation[1][2] == -0.0272763148");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[2][0], 0.0275405664, FLT_EPSILON, "oMPC.m_Rotation[2][0] == 0.0275405664");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[2][1], -0.0174167417, FLT_EPSILON, "oMPC.m_Rotation[2][1] == -0.0174167417");
//    UtAssert_DoubleCmpAbs(oMPC.m_Rotation[2][2], 0.999468982, FLT_EPSILON, "oMPC.m_Rotation[2][1] == 0.999468982");
//    UtAssert_DoubleCmpAbs(oMPC.m_Yaw, 1.55604124, FLT_EPSILON, "oMPC.m_Yaw == 1.55604124");
}


/**
 * Test MPC_AppMain(), Nominal - ProcessControlStateMsg
 */
void Test_MPC_AppMain_Nominal_ProcessVehicleLocalPositionMsg(void)
{
    MPC oMPC;
    PX4_VehicleLocalPositionMsg_t vehicleLocalPosition = {};
    PX4_ControlStateMsg_t controlState = {};
    int32 schPipe;

    schPipe = Ut_CFE_SB_CreatePipe("MPC_SCH_PIPE");

    CFE_SB_InitMsg(&vehicleLocalPosition, 0x0000, sizeof(vehicleLocalPosition), TRUE);
    CFE_SB_InitMsg(&controlState, 0x0001, sizeof(controlState), TRUE);


    vehicleLocalPosition.Timestamp = 88516625760;
    vehicleLocalPosition.RefTimestamp = 86416035453;
    vehicleLocalPosition.RefLat = 47.397742000000001;
    vehicleLocalPosition.RefLon = 8.5455939000000001;
    vehicleLocalPosition.SurfaceBottomTimestamp = 88516625760;
    vehicleLocalPosition.X = -0.0382288173;
    vehicleLocalPosition.Y = 0.0131685678;
    vehicleLocalPosition.Z = 0.405019075;
    vehicleLocalPosition.Delta_XY[0] = -0.00702110073;
    vehicleLocalPosition.Delta_XY[1] = -0.00230869721;
    vehicleLocalPosition.Delta_Z = 0;
    vehicleLocalPosition.VX = -0.035448242;
    vehicleLocalPosition.VY = 0.00551212905;
    vehicleLocalPosition.VZ = 0.20808062;
    vehicleLocalPosition.Delta_VXY[0] = -0.0166048259;
    vehicleLocalPosition.Delta_VXY[1] = -0.00429295097;
    vehicleLocalPosition.Delta_VZ = 0.0100669134;
    vehicleLocalPosition.AX = 0;
    vehicleLocalPosition.AY = 0;
    vehicleLocalPosition.AZ = 0;
    vehicleLocalPosition.Yaw = 1.51540768;
    vehicleLocalPosition.RefAlt = 488.201019;
    vehicleLocalPosition.DistBottom = 0.995466709;
    vehicleLocalPosition.DistBottomRate = -0.20808062;
    vehicleLocalPosition.EpH = 0.412961185;
    vehicleLocalPosition.EpV = 0.23175697;
    vehicleLocalPosition.EvH = 0.188474447;
    vehicleLocalPosition.EvV = 0.0783097148;
    vehicleLocalPosition.EstimatorType = 0;
    vehicleLocalPosition.XY_Valid = true;
    vehicleLocalPosition.Z_Valid = true;
    vehicleLocalPosition.V_XY_Valid = true;
    vehicleLocalPosition.V_Z_Valid = true;
    vehicleLocalPosition.XY_ResetCounter = 1;
    vehicleLocalPosition.Z_ResetCounter = 0;
    vehicleLocalPosition.VXY_ResetCounter = 1;
    vehicleLocalPosition.VZ_ResetCounter = 1;
    vehicleLocalPosition.XY_Global = true;
    vehicleLocalPosition.Z_Global = true;
    vehicleLocalPosition.DistBottomValid = true;

    controlState.Timestamp = 86440828023;
    controlState.AccX = -0.236284539;
    controlState.AccY = 0.101594232;
    controlState.AccZ = -9.76688671;
    controlState.VelX = 0.000539890432;
    controlState.VelY = 0.0182482861;
    controlState.VelZ = 0.00519393012;
    controlState.PosX = -0.00807033759;
    controlState.PosY = -0.000570089556;
    controlState.PosZ = 0.0179239418;
    controlState.Airspeed = 0;
    controlState.VelVariance[0] = 0.0f;
    controlState.VelVariance[1] = 0.0f;
    controlState.VelVariance[2] = 0.0f;
    controlState.PosVariance[0] = 0.0f;
    controlState.PosVariance[1] = 0.0f;
    controlState.PosVariance[2] = 0.0f;
    controlState.Q[0] = 0.712293863;
    controlState.Q[1] = 0.00346049992;
    controlState.Q[2] = -0.0159233119;
    controlState.Q[3] = 0.701692224;
    controlState.DeltaQReset[0] = 0.999934673;
    controlState.DeltaQReset[1] = 0.000154913403;
    controlState.DeltaQReset[2] = -0.00022063707;
    controlState.DeltaQReset[3] = 0.0114225745;
    controlState.RollRate = 0.00590410549;
    controlState.PitchRate = 0.00479567284;
    controlState.YawRate = 0.000454715308;
    controlState.HorzAccMag = 0.158787385;
    controlState.RollRateBias = -0.00138732162;
    controlState.PitchRateBias = -0.00102514029;
    controlState.YawRateBias = -0.00102514029;
    controlState.AirspeedValid = false;
    controlState.QuatResetCounter = 2;

    Ut_CFE_SB_AddMsgToPipe(&controlState, schPipe);
    Ut_CFE_SB_AddMsgToPipe(&vehicleLocalPosition, schPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 3);

    /* Execute the function being tested */
    oMPC.AppMain();

    /* Verify results */
    UtAssert_True(oMPC.m_HeadingResetCounter == 0,"HeadingResetCounter == 0");
    UtAssert_True(oMPC.m_ResetCounterXy == 0, "oMPC.XZ_ResetCounter == 0");
    UtAssert_True(oMPC.m_ResetCounterZ == 0, "oMPC.m_ResetCounterZ == 0");
}


/**
 * Test MPC_AppMain(), Nominal - ProcessControlStateMsg
 */
void Test_MPC_AppMain_Nominal_UpdateParamsFromTable(void)
{
    MPC oMPC;
    int32 schPipe;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 1);

    /* Execute the function being tested */
    oMPC.AppMain();

    /* Verify results */
    UtAssert_DoubleCmpAbs(oMPC.m_PosP[0], 0.950000, FLT_EPSILON, "oMPC.m_PosP[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PosP[1], 0.950000, FLT_EPSILON, "oMPC.m_PosP[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PosP[2], 1.000000, FLT_EPSILON, "oMPC.m_PosP[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelP[0], 0.150000, FLT_EPSILON, "oMPC.m_VelP[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelP[1], 0.150000, FLT_EPSILON, "oMPC.m_VelP[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelP[2], 0.600000, FLT_EPSILON, "oMPC.m_VelP[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelI[0], 0.200000, FLT_EPSILON, "oMPC.m_VelI[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelI[1], 0.200000, FLT_EPSILON, "oMPC.m_VelI[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelI[2], 0.150000, FLT_EPSILON, "oMPC.m_VelI[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelD[0], 0.010000, FLT_EPSILON, "oMPC.m_VelI[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelD[1], 0.010000, FLT_EPSILON, "oMPC.m_VelI[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelD[2], 0.000000, FLT_EPSILON, "oMPC.m_VelI[2]");
}



/**
 * Test MPC_AppMain(), Nominal - UpdateVelocityDerivative
 * Currently commented out. Segfaults on target. Config table pointer
 * uninitialized.
 */
//void Test_MPC_AppMain_Nominal_UpdateVelocityDerivative(void)
//{
    //MPC oMPC;

    ///* Run 2 passes so we can verify the Velocity Error Derivative
     //* calculation.
     //*
     //* Pass 1
     //*/
    //oMPC.m_VehicleLocalPositionMsg.Timestamp = 66561590577;
    //oMPC.m_VehicleLocalPositionMsg.X = 0.000000;
    //oMPC.m_VehicleLocalPositionMsg.Y = 0.000000;
    //oMPC.m_VehicleLocalPositionMsg.Z = -0.000002;
    //oMPC.m_VehicleLocalPositionMsg.DistBottom = 1.029581;
    //oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.000493;
    //oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    //oMPC.m_VehicleLocalPositionMsg.VX = 0.000422;
    //oMPC.m_VehicleLocalPositionMsg.VY = 0.001771;
    //oMPC.m_VehicleLocalPositionMsg.VZ = -0.000493;
    //oMPC.m_Velocity[0] = 0.000000;
    //oMPC.m_Velocity[1] = 0.000000;
    //oMPC.m_Velocity[2] = 0.000000;
    //oMPC.m_Position[0] = 0.000000;
    //oMPC.m_Position[1] = 0.000000;
    //oMPC.m_Position[2] = 0.000000;

    //oMPC.UpdateVelocityDerivative(0.016292);

    //UtAssert_DoubleCmpAbs(oMPC.m_Position[0],  0.000000, FLT_EPSILON, "oMPC.m_Position[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Position[1],  0.000000, FLT_EPSILON, "oMPC.m_Position[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Position[2], -0.000002, FLT_EPSILON, "oMPC.m_Position[2]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[0],  0.000422, FLT_EPSILON, "oMPC.m_Velocity[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[1],  0.001771, FLT_EPSILON, "oMPC.m_Velocity[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[2], -0.000493, FLT_EPSILON, "oMPC.m_Velocity[2]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[0],  0.000000, FLT_EPSILON, "oMPC.m_VelocityErrD[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[1],  0.000000, FLT_EPSILON, "oMPC.m_VelocityErrD[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[2],  0.000000, FLT_EPSILON, "oMPC.m_VelocityErrD[2]");

    ///*
     //* Pass 2
     //*/
    //oMPC.m_VehicleLocalPositionMsg.Timestamp = 66561595349;
    //oMPC.m_VehicleLocalPositionMsg.X = 0.000000;
    //oMPC.m_VehicleLocalPositionMsg.Y = 0.000000;
    //oMPC.m_VehicleLocalPositionMsg.Z = -0.000005;
    //oMPC.m_VehicleLocalPositionMsg.DistBottom = 1.029584;
    //oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.000810;
    //oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    //oMPC.m_VehicleLocalPositionMsg.VX = 0.000195;
    //oMPC.m_VehicleLocalPositionMsg.VY = 0.003512;
    //oMPC.m_VehicleLocalPositionMsg.VZ = -0.000810;

    //oMPC.UpdateVelocityDerivative(0.004689);

    //UtAssert_DoubleCmpAbs(oMPC.m_Position[0],  0.000000, FLT_EPSILON, "oMPC.m_Position[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Position[1],  0.000000, FLT_EPSILON, "oMPC.m_Position[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Position[2], -0.000005, FLT_EPSILON, "oMPC.m_Position[2]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[0],  0.000195, FLT_EPSILON, "oMPC.m_Velocity[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[1],  0.003512, FLT_EPSILON, "oMPC.m_Velocity[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_Velocity[2], -0.000810, FLT_EPSILON, "oMPC.m_Velocity[2]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[0],  0.00023751, FLT_EPSILON, "oMPC.m_VelocityErrD[0]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[1], -0.00182158, FLT_EPSILON, "oMPC.m_VelocityErrD[1]");
    //UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[2],  0.00033167, FLT_EPSILON, "oMPC.m_VelocityErrD[2]");
//}



/**
 * Test MPC_AppMain(), Nominal - LimitAltitude
 */
void Test_MPC_AppMain_Nominal_LimitAltitude(void)
{
    MPC oMPC;

    oMPC.InitConfigTbl();

    /* PASS 1 - Landed and idle. */
    oMPC.m_RunAltControl = 0;
    oMPC.m_PositionSetpoint[2] = 0.005629;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000;
    oMPC.m_Velocity[2] = 0.010930;
    oMPC.m_Position[2] = 0.005629;

    oMPC.LimitAltitude();

    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2],  0.005629, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_True(oMPC.m_RunAltControl == false,  "oMPC.m_RunAltControl");

    /* PASS 2 - Landed and idle. */
    oMPC.m_RunAltControl = 0;
    oMPC.m_PositionSetpoint[2] = 0.005629;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000;
    oMPC.m_Velocity[2] = 0.010245;
    oMPC.m_Position[2] = 0.005350;

    oMPC.LimitAltitude();

    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2],  0.005629, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_True(oMPC.m_RunAltControl == false,  "oMPC.m_RunAltControl");

    /* PASS 3 - Takeoff. */
    oMPC.m_RunAltControl = 1;
    oMPC.m_PositionSetpoint[2] = -2.729340;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    oMPC.m_VelocitySetpoint[2] = -0.687779;
    oMPC.m_Velocity[2] = -0.752254;
    oMPC.m_Position[2] = -0.287284;

    oMPC.LimitAltitude();

    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2],  -2.729340, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_True(oMPC.m_RunAltControl == true,  "oMPC.m_RunAltControl");
}



/**
 * Test MPC_AppMain(), Nominal - ApplyVelocitySetpointSlewRate
 */
void Test_MPC_AppMain_Nominal_ApplyVelocitySetpointSlewRate(void)
{
    MPC oMPC;

    /* PASS 1 - Landed and idle. */
    oMPC.m_VelocitySetpoint[0] = 0.000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000;
    oMPC.m_VelocitySetpointPrevious[0] = -0.004032;
    oMPC.m_VelocitySetpointPrevious[1] = -0.010259;
    oMPC.m_VelocitySetpointPrevious[2] = 0.016675;

    oMPC.ApplyVelocitySetpointSlewRate(0.005820);

//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");

    /* PASS 2 - Takeoff. */
    oMPC.m_VelocitySetpoint[0] = 0.000719;
    oMPC.m_VelocitySetpoint[1] = -0.001065;
    oMPC.m_VelocitySetpoint[2] = -1.500000;
    oMPC.m_VelocitySetpointPrevious[0] = 0.000306;
    oMPC.m_VelocitySetpointPrevious[1] = -0.000646;
    oMPC.m_VelocitySetpointPrevious[2] = -0.727830;

    oMPC.ApplyVelocitySetpointSlewRate(0.068840);

//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000719, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.001065, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -1.072030, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
}



/**
 * Test MPC_AppMain(), Nominal - CalculateVelocitySetpoint
 */
void Test_MPC_AppMain_Nominal_CalculateVelocitySetpoint(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* PASS 1 - Landed and idle. */
    oMPC.m_RunPosControl = 0;
    oMPC.m_RunAltControl = 0;
    oMPC.m_Position[0] = 0.005878;
    oMPC.m_Position[1] = -0.002398;
    oMPC.m_Position[2] = 0.027536;
    oMPC.m_PositionSetpoint[0] = 0.005878;
    oMPC.m_PositionSetpoint[1] = -0.002398;
    oMPC.m_PositionSetpoint[2] = 0.027536;
    oMPC.m_Velocity[0] = 0.005205;
    oMPC.m_Velocity[1] = -0.006782;
    oMPC.m_Velocity[2] = 0.011264;
    oMPC.m_VelocitySetpoint[0] = 0.000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_POSITION;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.000000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateVelocitySetpoint(0.009890);

    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[2]");
    UtAssert_True(oMPC.m_ResetPositionSetpoint == 0, "oMPC.m_ResetPositionSetpoint");
    UtAssert_True(oMPC.m_ResetAltitudeSetpoint == 0, "oMPC.m_ResetAltitudeSetpoint");
    UtAssert_True(oMPC.m_InTakeoff == 0, "oMPC.m_InTakeoff");
    UtAssert_DoubleCmpAbs(oMPC.m_TakeoffVelLimit, 0.000000, FLT_EPSILON, "oMPC.m_TakeoffVelLimit");

    /* PASS 2 - Landed and idle. */
    oMPC.m_RunPosControl = 0;
    oMPC.m_RunAltControl = 0;
    oMPC.m_Position[0] = 0.005625;
    oMPC.m_Position[1] = -0.002856;
    oMPC.m_Position[2] = 0.027540;
    oMPC.m_PositionSetpoint[0] = 0.005878;
    oMPC.m_PositionSetpoint[1] = -0.002398;
    oMPC.m_PositionSetpoint[2] = 0.027536;
    oMPC.m_Velocity[0] = 0.004707;
    oMPC.m_Velocity[1] = -0.005651;
    oMPC.m_Velocity[2] = 0.010354;
    oMPC.m_VelocitySetpoint[0] = 0.000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_TAKEOFF;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.000000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateVelocitySetpoint(0.025312);

    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[2]");
    UtAssert_True(oMPC.m_ResetPositionSetpoint == 0, "oMPC.m_ResetPositionSetpoint");
    UtAssert_True(oMPC.m_ResetAltitudeSetpoint == 0, "oMPC.m_ResetAltitudeSetpoint");
    UtAssert_True(oMPC.m_InTakeoff == 0, "oMPC.m_InTakeoff");
    UtAssert_DoubleCmpAbs(oMPC.m_TakeoffVelLimit, 0.000000, FLT_EPSILON, "oMPC.m_TakeoffVelLimit");

    /* PASS 3 - Taking off */
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_Position[0] =  0.008560;
    oMPC.m_Position[1] = -0.021263;
    oMPC.m_Position[2] =  0.166312;
    oMPC.m_PositionSetpoint[0] =  0.008954;
    oMPC.m_PositionSetpoint[1] = -0.021870;
    oMPC.m_PositionSetpoint[2] = -2.334442;
    oMPC.m_Velocity[0] = -0.008084;
    oMPC.m_Velocity[1] = -0.017554;
    oMPC.m_Velocity[2] = -0.042712;
    oMPC.m_VelocitySetpoint[0] =  0.000038;
    oMPC.m_VelocitySetpoint[1] =  0.000105;
    oMPC.m_VelocitySetpoint[2] = -0.049970;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_TAKEOFF;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 1;
    oMPC.m_TakeoffVelLimit = -0.500000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;
    /* For LimitAltitude() */
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    /* For SlowLandGradualVelocityLimit() */
    oMPC.m_HomePositionMsg.Z = 0.165064;
    /* For LimitVelXYGradually() */
    oMPC.m_CurrentPositionSetpoint[0] =  0.008954;
    oMPC.m_CurrentPositionSetpoint[1] = -0.021870;
    oMPC.m_CurrentPositionSetpoint[2] = -2.334442;
    oMPC.m_Position[0] =  0.008560;
    oMPC.m_Position[1] = -0.021263;
    oMPC.m_Position[2] =  0.166312;
    /* For ApplyVelocitySetpointSlewRate() */
    oMPC.m_VelocitySetpointPrevious[0] =  0.000038;
    oMPC.m_VelocitySetpointPrevious[1] =  0.000105;
    oMPC.m_VelocitySetpointPrevious[2] = -0.049970;
    oMPC.m_Velocity[0] =  -0.008084;
    oMPC.m_Velocity[1] =  -0.017554;
    oMPC.m_Velocity[2] =  -0.042712;
    /* For GetCruisingSpeedXY() */
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.0;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateVelocitySetpoint(0.065570);

//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0],  0.000374399736, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.000576650607, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2],  0.254112512, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[0],  0.000374299735, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[1], -0.000576650607, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[2], -0.377819985, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[2]");
    UtAssert_True(oMPC.m_ResetPositionSetpoint == 0, "oMPC.m_ResetPositionSetpoint");
    UtAssert_True(oMPC.m_ResetAltitudeSetpoint == 0, "oMPC.m_ResetAltitudeSetpoint");
    UtAssert_True(oMPC.m_InTakeoff == 1, "oMPC.m_InTakeoff");
    UtAssert_DoubleCmpAbs(oMPC.m_TakeoffVelLimit, -0.254112512, FLT_EPSILON, "oMPC.m_TakeoffVelLimit");

}



/**
 * Test MPC_AppMain(), Nominal - CalculateThrustSetpoint
 */
void Test_MPC_AppMain_Nominal_CalculateThrustSetpoint(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* PASS 1 - Landed and idle. */
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_ResetIntXY = 1;
    oMPC.m_ResetIntZ = 1;
    oMPC.m_ThrustInt[0] = 0.0;
    oMPC.m_ThrustInt[1] = 0.0;
    oMPC.m_ThrustInt[2] = 0.0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.0;
    oMPC.m_VelocityErrD[0] = 0.01287147;
    oMPC.m_VelocityErrD[1] = 0.22019185;
    oMPC.m_VelocityErrD[2] = -0.33453843;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_Rotation[0][0] = 0.03877932;
    oMPC.m_Rotation[0][1] = -0.99902272;
    oMPC.m_Rotation[0][2] = -0.02121111;
    oMPC.m_Rotation[1][0] = 0.99905360;
    oMPC.m_Rotation[1][1] = 0.03918153;
    oMPC.m_Rotation[1][2] = -0.01888950;
    oMPC.m_Rotation[2][0] = 0.01970213;
    oMPC.m_Rotation[2][1] = -0.02045851;
    oMPC.m_Rotation[2][2] = 0.99959666;
    oMPC.m_VehicleLandDetectedMsg.Landed = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 0.03080329;
    oMPC.m_RSetpoint[0][0] = 1.00000000;
    oMPC.m_RSetpoint[0][1] = 0.00000000;
    oMPC.m_RSetpoint[0][2] = 0.00000000;
    oMPC.m_RSetpoint[1][0] = 0.00000000;
    oMPC.m_RSetpoint[1][1] = 1.00000000;
    oMPC.m_RSetpoint[1][2] = 0.00000000;
    oMPC.m_RSetpoint[2][0] = 0.00000000;
    oMPC.m_RSetpoint[2][1] = 0.00000000;
    oMPC.m_RSetpoint[2][2] = 1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.99988139;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.01540103;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.00000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.00000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -0.00000000;
    /* For InAutoTakeoff() */
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    /* For CalculateVelocitySetpoint() */
    oMPC.m_RunPosControl = 0;
    oMPC.m_RunAltControl = 0;
    oMPC.m_Position[0] = -0.00207413;
    oMPC.m_Position[1] = -0.00082683;
    oMPC.m_Position[2] = -0.02151199;
    oMPC.m_PositionSetpoint[0] = -0.00207413;
    oMPC.m_PositionSetpoint[1] = -0.00082683;
    oMPC.m_PositionSetpoint[2] = -0.02151199;
    oMPC.m_Velocity[0] = -0.00085380;
    oMPC.m_Velocity[1] = -0.00069751;
    oMPC.m_Velocity[2] = 0.00590463;
    oMPC.m_VelocitySetpoint[0] = 0.00000000;
    oMPC.m_VelocitySetpoint[1] = 0.00000000;
    oMPC.m_VelocitySetpoint[2] = 0.00000000;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_POSITION;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.00000000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;
    /* For LimitAltitude() */
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    /* For SlowLandGradualVelocityLimit() */
    oMPC.m_HomePositionMsg.Z = -0.02179659;
    /* For LimitVelXYGradually() */
    oMPC.m_CurrentPositionSetpoint[0] = 0.00000000;
    oMPC.m_CurrentPositionSetpoint[1] = 0.00000000;
    oMPC.m_CurrentPositionSetpoint[2] = 0.00000000;
    oMPC.m_Position[0] = -0.00207413;
    oMPC.m_Position[1] = -0.00082683;
    oMPC.m_Position[2] = -0.02151199;
    /* For ApplyVelocitySetpointSlewRate() */
    oMPC.m_VelocityPrevious[0] = -0.00135698;
    oMPC.m_VelocityPrevious[1] = -0.00031078;
    oMPC.m_VelocityPrevious[2] = 0.00620359;
    oMPC.m_Velocity[0] = -0.00085380;
    oMPC.m_Velocity[1] = -0.00069751;
    oMPC.m_Velocity[2] = 0.00590463;
    /* For GetCruisingSpeedXY() */
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = 0.00000000;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateThrustSetpoint(0.00549900);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], 0.000000, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], 0.000000, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], 0.000000, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], 0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -0.00207413, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1], -0.00082683, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -0.02151199, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.12000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  0.99952561, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.03079842, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.00000000, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.03079842, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  0.99952561, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.00000000, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.00000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.00000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  1.00000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, 0.000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, 0.000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, 0.000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");

    /* PASS 2 - Transition to takeoff. */
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_ResetIntXY = 1;
    oMPC.m_ResetIntZ = 1;
    oMPC.m_ThrustInt[0] = 0.0;
    oMPC.m_ThrustInt[1] = 0.0;
    oMPC.m_ThrustInt[2] = 0.0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.0;
    oMPC.m_VelocityErrD[0] =  0.00865284;
    oMPC.m_VelocityErrD[1] = -0.01698322;
    oMPC.m_VelocityErrD[2] =  0.01355007;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_Rotation[0][0] = -0.01877549;
    oMPC.m_Rotation[0][1] = -0.99964637;
    oMPC.m_Rotation[0][2] = -0.01883845;
    oMPC.m_Rotation[1][0] = 0.99973601;
    oMPC.m_Rotation[1][1] = -0.01852074;
    oMPC.m_Rotation[1][2] = -0.01360697;
    oMPC.m_Rotation[2][0] = 0.01325325;
    oMPC.m_Rotation[2][1] = -0.01908895;
    oMPC.m_Rotation[2][2] = 0.99973011;
    oMPC.m_VehicleLandDetectedMsg.Landed = 1;
    oMPC.m_RSetpoint[0][0] = 1.00000000;
    oMPC.m_RSetpoint[0][1] = 0.00000000;
    oMPC.m_RSetpoint[0][2] = 0.00000000;
    oMPC.m_RSetpoint[1][0] = 0.00000000;
    oMPC.m_RSetpoint[1][1] = 1.00000000;
    oMPC.m_RSetpoint[1][2] = 0.00000000;
    oMPC.m_RSetpoint[2][0] = 0.00000000;
    oMPC.m_RSetpoint[2][1] = 0.00000000;
    oMPC.m_RSetpoint[2][2] = 1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.58957458;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.00000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.00000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -0.00000000;
    /* For InAutoTakeoff() */
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    /* For CalculateVelocitySetpoint() */
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_Position[0] = 0.01195339;
    oMPC.m_Position[1] = -0.01815947;
    oMPC.m_Position[2] = 0.14472280;
    oMPC.m_PositionSetpoint[0] =  0.01195339;
    oMPC.m_PositionSetpoint[1] = -0.01815947;
    oMPC.m_PositionSetpoint[2] = -2.35528564;
    oMPC.m_Velocity[0] = -0.00035331;
    oMPC.m_Velocity[1] = -0.03173279;
    oMPC.m_Velocity[2] =  0.12474953;
    oMPC.m_VelocitySetpoint[0] = -0.00000000;
    oMPC.m_VelocitySetpoint[1] =  0.00000000;
    oMPC.m_VelocitySetpoint[2] = -0.06405000;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_TAKEOFF;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.00000000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;
    /* For LimitAltitude() */
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    /* For SlowLandGradualVelocityLimit() */
    oMPC.m_HomePositionMsg.Z = 0.14212561;
    /* For LimitVelXYGradually() */
    oMPC.m_CurrentPositionSetpoint[0] = 0.01195339;
    oMPC.m_CurrentPositionSetpoint[1] = -0.01815947;
    oMPC.m_CurrentPositionSetpoint[2] = -2.35528564;
    oMPC.m_Position[0] = 0.01195339;
    oMPC.m_Position[1] = -0.01815947;
    oMPC.m_Position[2] = 0.14472280;
    /* For ApplyVelocitySetpointSlewRate() */
    oMPC.m_VelocityPrevious[0] = -0.00012040;
    oMPC.m_VelocityPrevious[1] = -0.03237132;
    oMPC.m_VelocityPrevious[2] = 0.12617928;
    oMPC.m_Velocity[0] = -0.00035331;
    oMPC.m_Velocity[1] = -0.03173279;
    oMPC.m_Velocity[2] = 0.12474953;
    /* For GetCruisingSpeedXY() */
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.00000000;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateThrustSetpoint(0.01281000);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0],  0.00000091, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1],  0.00008130, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], -0.00036278, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1],  0.000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.06405000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], 0.01195339, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1], -0.01815947, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.35528564, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.61317927, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0], -0.01877662, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.99982363, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2], -0.00022750, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.99979573, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1], -0.01877439, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2], -0.00748428, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0],  0.00747869, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1], -0.00036798, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.99997193, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, -0.00036799, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, -0.00747876, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX,  0.00136826, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY,  0.04501338, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -6.01421928, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");

    /* PASS 3 - Taking off. */
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_ResetIntXY = 0;
    oMPC.m_ResetIntZ = 0;
    oMPC.m_ThrustInt[0] = -0.000015456446;
    oMPC.m_ThrustInt[1] = -0.000006746221;
    oMPC.m_ThrustInt[2] = 0.000065119777;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.0;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.0;
    oMPC.m_VelocityErrD[0] = -0.037555422634;
    oMPC.m_VelocityErrD[1] = -0.007769315038;
    oMPC.m_VelocityErrD[2] = 0.097660362720;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_Rotation[0][0] = 0.793960154057;
    oMPC.m_Rotation[0][1] = -0.606549143791;
    oMPC.m_Rotation[0][2] = -0.041537456214;
    oMPC.m_Rotation[1][0] = 0.606583714485;
    oMPC.m_Rotation[1][1] = 0.794909775257;
    oMPC.m_Rotation[1][2] = -0.013206949458;
    oMPC.m_Rotation[2][0] = 0.041029199958;
    oMPC.m_Rotation[2][1] = -0.014710154384;
    oMPC.m_Rotation[2][2] = 0.999049603939;
    oMPC.m_VehicleLandDetectedMsg.Landed = 0;
    oMPC.m_RSetpoint[0][0] =  0.794545173645;
    oMPC.m_RSetpoint[0][1] = -0.607062637806;
    oMPC.m_RSetpoint[0][2] =  0.013155251741;
    oMPC.m_RSetpoint[1][0] =  0.607051432133;
    oMPC.m_RSetpoint[1][1] =  0.794645130634;
    oMPC.m_RSetpoint[1][2] =  0.005279467907;
    oMPC.m_RSetpoint[2][0] = -0.013658720069;
    oMPC.m_RSetpoint[2][1] =  0.003791138530;
    oMPC.m_RSetpoint[2][2] =  0.999899625778;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.94724464;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00039280;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00707683;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.32043308;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00379150;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.01365914;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 0.65241611;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = -0.050282537937;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = -0.020179396495;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -3.821856975555;
    /* For InAutoTakeoff() */
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    /* For CalculateVelocitySetpoint() */
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_Position[0] = 0.008880654350;
    oMPC.m_Position[1] = 0.006610852201;
    oMPC.m_Position[2] = -0.043556511402;
    oMPC.m_PositionSetpoint[0] = 0.008687019348;
    oMPC.m_PositionSetpoint[1] = 0.006540667731;
    oMPC.m_PositionSetpoint[2] = -2.542480468750;
    oMPC.m_Velocity[0] = 0.033547148108;
    oMPC.m_Velocity[1] = 0.014865242876;
    oMPC.m_Velocity[2] = -0.198812872171;
    oMPC.m_VelocitySetpoint[0] = -0.000183953249;
    oMPC.m_VelocitySetpoint[1] = -0.000066675246;
    oMPC.m_VelocitySetpoint[2] = -0.055955003947;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_TAKEOFF;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_InTakeoff = 1;
    oMPC.m_TakeoffVelLimit = 0.00000000;
    oMPC.m_ResetPositionSetpoint = false;
    oMPC.m_ResetAltitudeSetpoint = false;
    /* For LimitAltitude() */
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000;
    /* For SlowLandGradualVelocityLimit() */
    oMPC.m_HomePositionMsg.Z = -0.044796019793;
    /* For LimitVelXYGradually() */
    oMPC.m_CurrentPositionSetpoint[0] = 0.008687019348;
    oMPC.m_CurrentPositionSetpoint[1] = 0.006540667731;
    oMPC.m_CurrentPositionSetpoint[2] = -2.542480468750;
    /* For ApplyVelocitySetpointSlewRate() */
    oMPC.m_VelocityPrevious[0] = 0.032718978822;
    oMPC.m_VelocityPrevious[1] = 0.014280737378;
    oMPC.m_VelocityPrevious[2] = -0.195608407259;
    /* For GetCruisingSpeedXY() */
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.00000000;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.CalculateThrustSetpoint(0.008829000406);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], -0.000075018834, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], -0.000033113003, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2],  0.000254313607, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.000183953249, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.000066675246, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.055955003947, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0],  0.008687019348, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  0.006540667731, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.542480468750, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.413569390774, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  0.794542908669, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.607065439224, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.013157540932, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.607049763203, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  0.794644057751, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.005610517692, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.013861512765, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.003529485315, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.999897718430, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.003529831767, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.013861957006, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, -0.053452875465, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, -0.022792883217, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -4.062112331390, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");
}



/**
 * Test MPC_AppMain(), Nominal - ControlAuto
 */
void Test_MPC_AppMain_Nominal_ControlAuto(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* PASS 1 - Initializing. */
    oMPC.m_VehicleStatusMsg.Timestamp = 20280576066;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)4;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)1;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)1;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 20278631900;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 1;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 20280585087;
    oMPC.m_ControlStateMsg.AccX = -0.174654871225;
    oMPC.m_ControlStateMsg.AccY = 0.096246533096;
    oMPC.m_ControlStateMsg.AccZ = -9.875363349915;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.694662868977;
    oMPC.m_ControlStateMsg.Q[1] = 0.012822493911;
    oMPC.m_ControlStateMsg.Q[2] = -0.013447887264;
    oMPC.m_ControlStateMsg.Q[3] = 0.719095528126;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = -0.001191451796;
    oMPC.m_ControlStateMsg.PitchRate = -0.004856707528;
    oMPC.m_ControlStateMsg.YawRate = 0.000788278761;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 20280574950;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.57048452;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.70721704;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.70699656;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 20280576066;
    oMPC.m_VehicleControlModeMsg.Armed = 0;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 20280585123;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 20280585123;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397743983469;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593796482;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 20280585123;
    oMPC.m_VehicleLocalPositionMsg.X = 0.001803947147;
    oMPC.m_VehicleLocalPositionMsg.Y = 0.000300874759;
    oMPC.m_VehicleLocalPositionMsg.Z = -0.000000002900;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = -0.013946888968;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.019273743033;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.225940167904;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.605385303497;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.269012451172;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = -0.000000000137;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.225940167904;
    oMPC.m_VehicleLocalPositionMsg.EpH = 3.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EpV = 2.000649929047;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 0;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 0;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 0;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 0;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = 0.000000000000;
    oMPC.m_HomePositionMsg.Timestamp = 20280565980;
    oMPC.m_HomePositionMsg.Lat = 47.397744000959;
    oMPC.m_HomePositionMsg.Lon = 8.545593797628;
    oMPC.m_HomePositionMsg.Alt = 488.269012451172;
    oMPC.m_HomePositionMsg.X = 0.001944838790;
    oMPC.m_HomePositionMsg.Y = 0.000086301727;
    oMPC.m_HomePositionMsg.Z = -0.000000002846;
    oMPC.m_HomePositionMsg.Yaw = 1.575607180595;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.269012451172;
    oMPC.m_RefPos.timestamp = 20280585168;
    oMPC.m_RefPos.lat_rad = 0.827246690529;
    oMPC.m_RefPos.lon_rad = 0.149148748287;
    oMPC.m_RefPos.sin_lat = 0.736070434614;
    oMPC.m_RefPos.cos_lat = 0.676904952919;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 20280585123;
    oMPC.m_ResetPositionSetpoint = 1;
    oMPC.m_ResetAltitudeSetpoint = 1;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 0;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 1;
    oMPC.m_ResetIntXY = 1;
    oMPC.m_ResetYawSetpoint = 0;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.000000000000;
    oMPC.m_ThrustInt[1] = 0.000000000000;
    oMPC.m_ThrustInt[2] = 0.000000000000;
    oMPC.m_Position[0] = 0.001803947147;
    oMPC.m_Position[1] = 0.000300874759;
    oMPC.m_Position[2] = -0.000000002900;
    oMPC.m_PositionSetpoint[0] = 0.000000000000;
    oMPC.m_PositionSetpoint[1] = 0.000000000000;
    oMPC.m_PositionSetpoint[2] = 0.000000000000;
    oMPC.m_Velocity[0] = -0.013946888968;
    oMPC.m_Velocity[1] = 0.019273743033;
    oMPC.m_Velocity[2] = -0.225940167904;
    oMPC.m_VelocitySetpoint[0] = 0.000000000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000000000;
    oMPC.m_VelocityPrevious[0] = -0.013365665451;
    oMPC.m_VelocityPrevious[1] = 0.018462687731;
    oMPC.m_VelocityPrevious[2] = -0.226975008845;
    oMPC.m_VelocitySetpointPrevious[0] = -0.013365665451;
    oMPC.m_VelocitySetpointPrevious[1] = 0.018462687731;
    oMPC.m_VelocitySetpointPrevious[2] = -0.226975008845;
    oMPC.m_VelocityErrD[0] = 0.019212024286;
    oMPC.m_VelocityErrD[1] = -0.031484425068;
    oMPC.m_VelocityErrD[2] = -0.046475097537;
    oMPC.m_CurrentPositionSetpoint[0] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[1] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[2] = 0.000000000000;
    oMPC.m_Rotation[0][0] = -0.034558296204;
    oMPC.m_Rotation[0][1] = -0.999402821064;
    oMPC.m_Rotation[0][2] = -0.000242298469;
    oMPC.m_Rotation[1][0] = 0.998713076115;
    oMPC.m_Rotation[1][1] = -0.034525454044;
    oMPC.m_Rotation[1][2] = -0.037155251950;
    oMPC.m_Rotation[2][0] = 0.037124693394;
    oMPC.m_Rotation[2][1] = -0.001526009291;
    oMPC.m_Rotation[2][2] = 0.999309599400;
    oMPC.m_Yaw = 1.605385303497;
    oMPC.m_YawTakeoff = 0.000000000000;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.000000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = 1.000000000000;
    oMPC.m_RSetpoint[0][1] = 0.000000000000;
    oMPC.m_RSetpoint[0][2] = 0.000000000000;
    oMPC.m_RSetpoint[1][0] = 0.000000000000;
    oMPC.m_RSetpoint[1][1] = 1.000000000000;
    oMPC.m_RSetpoint[1][2] = 0.000000000000;
    oMPC.m_RSetpoint[2][0] = 0.000000000000;
    oMPC.m_RSetpoint[2][1] = 0.000000000000;
    oMPC.m_RSetpoint[2][2] = 1.000000000000;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.ControlAuto(0.010219999589);

    UtAssert_True(oMPC.m_ResetIntXY == true, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == true, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], 0.000000000000, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], 0.000000000000, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], 0.000000000000, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], 0.000000000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], 0.000000000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], 0.000000000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], 0.001803947147, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1], 0.000300874759, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -0.000000002900, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  1.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  1.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  1.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, -0.000000000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.570484519005, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");

    /* PASS 2 - Transition to takeoff. */
    oMPC.m_VehicleStatusMsg.Timestamp = 22718352358;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)17;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)2;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)1;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 22718419546;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 0;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 22718419636;
    oMPC.m_ControlStateMsg.AccX = -0.178399145603;
    oMPC.m_ControlStateMsg.AccY = 0.143566161394;
    oMPC.m_ControlStateMsg.AccZ = -9.878698348999;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.715221941471;
    oMPC.m_ControlStateMsg.Q[1] = 0.004807707854;
    oMPC.m_ControlStateMsg.Q[2] = -0.020368587226;
    oMPC.m_ControlStateMsg.Q[3] = 0.698583960533;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = 0.004654823337;
    oMPC.m_ControlStateMsg.PitchRate = 0.000240786583;
    oMPC.m_ControlStateMsg.YawRate = -0.001242246712;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 22718395507;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 47.397741928975;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 8.545593979817;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 490.751251220703;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 1.547718405724;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)3;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 22718406228;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00286550;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00668981;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.54771841;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.71521747;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00131303;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00339367;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.69889259;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.55761039;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 22718352358;
    oMPC.m_VehicleControlModeMsg.Armed = 1;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 22718420325;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 22718420325;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397741905620;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593899402;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 22718420325;
    oMPC.m_VehicleLocalPositionMsg.X = 0.002443513833;
    oMPC.m_VehicleLocalPositionMsg.Y = 0.005807545502;
    oMPC.m_VehicleLocalPositionMsg.Z = 0.003159755142;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = 0.012711226940;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.027060005814;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.027509212494;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.547648310661;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.253997802734;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = 0.002536314772;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.027509212494;
    oMPC.m_VehicleLocalPositionMsg.EpH = 0.364578098059;
    oMPC.m_VehicleLocalPositionMsg.EpV = 0.234133124352;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 22718406228;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = 0.002596978797;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 0.006052741315;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = -2.497253417969;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 1.547718405724;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000100608748;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = 0.000175435256;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = -0.123394995928;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = -0.016529330984;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = -0.036253951490;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -5.474771976471;
    oMPC.m_HomePositionMsg.Timestamp = 22718352358;
    oMPC.m_HomePositionMsg.Lat = 47.397741928428;
    oMPC.m_HomePositionMsg.Lon = 8.545593976476;
    oMPC.m_HomePositionMsg.Alt = 488.252136230469;
    oMPC.m_HomePositionMsg.X = 0.002536065644;
    oMPC.m_HomePositionMsg.Y = 0.005801218562;
    oMPC.m_HomePositionMsg.Z = 0.001871463843;
    oMPC.m_HomePositionMsg.Yaw = 1.547679662704;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.253997802734;
    oMPC.m_RefPos.timestamp = 22718420398;
    oMPC.m_RefPos.lat_rad = 0.827246654264;
    oMPC.m_RefPos.lon_rad = 0.149148750083;
    oMPC.m_RefPos.sin_lat = 0.736070410066;
    oMPC.m_RefPos.cos_lat = 0.676904979613;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 22718420325;
    oMPC.m_ResetPositionSetpoint = 0;
    oMPC.m_ResetAltitudeSetpoint = 0;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 1;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 0;
    oMPC.m_ResetIntXY = 0;
    oMPC.m_ResetYawSetpoint = 1;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = -0.000066571869;
    oMPC.m_ThrustInt[1] = -0.000139073905;
    oMPC.m_ThrustInt[2] = -0.000246876385;
    oMPC.m_Position[0] = 0.002443513833;
    oMPC.m_Position[1] = 0.005807545502;
    oMPC.m_Position[2] = 0.003159755142;
    oMPC.m_PositionSetpoint[0] = 0.002596978797;
    oMPC.m_PositionSetpoint[1] = 0.006052741315;
    oMPC.m_PositionSetpoint[2] = -2.497253417969;
    oMPC.m_Velocity[0] = 0.012711226940;
    oMPC.m_Velocity[1] = 0.027060005814;
    oMPC.m_Velocity[2] = -0.027509212494;
    oMPC.m_VelocitySetpoint[0] = 0.000100608748;
    oMPC.m_VelocitySetpoint[1] = 0.000175435256;
    oMPC.m_VelocitySetpoint[2] = -0.123394995928;
    oMPC.m_VelocityPrevious[0] = 0.013109490275;
    oMPC.m_VelocityPrevious[1] = 0.027552505955;
    oMPC.m_VelocityPrevious[2] = -0.026428043842;
    oMPC.m_VelocitySetpointPrevious[0] = 0.000100608748;
    oMPC.m_VelocitySetpointPrevious[1] = 0.000175435256;
    oMPC.m_VelocitySetpointPrevious[2] = -0.123394995928;
    oMPC.m_VelocityErrD[0] = 0.029724396765;
    oMPC.m_VelocityErrD[1] = 0.044614251703;
    oMPC.m_VelocityErrD[2] = 0.014642260969;
    oMPC.m_CurrentPositionSetpoint[0] = 0.002596978797;
    oMPC.m_CurrentPositionSetpoint[1] = 0.006052741315;
    oMPC.m_CurrentPositionSetpoint[2] = -2.497253417969;
    oMPC.m_Rotation[0][0] = 0.023131102324;
    oMPC.m_Rotation[0][1] = -0.999481022358;
    oMPC.m_Rotation[0][2] = -0.022418946028;
    oMPC.m_Rotation[1][0] = 0.999089300632;
    oMPC.m_Rotation[1][1] = 0.023914664984;
    oMPC.m_Rotation[1][2] = -0.035335492343;
    oMPC.m_Rotation[2][0] = 0.035853296518;
    oMPC.m_Rotation[2][1] = -0.021581180394;
    oMPC.m_Rotation[2][2] = 0.999123930931;
    oMPC.m_Yaw = 1.547648310661;
    oMPC.m_YawTakeoff = 1.547740340233;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 1;
    oMPC.m_TakeoffVelLimit = -0.500000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = 0.023075357080;
    oMPC.m_RSetpoint[0][1] = -0.999729216099;
    oMPC.m_RSetpoint[0][2] = 0.003019101685;
    oMPC.m_RSetpoint[1][0] = 0.999711394310;
    oMPC.m_RSetpoint[1][1] = 0.023094942793;
    oMPC.m_RSetpoint[1][2] = 0.006621827371;
    oMPC.m_RSetpoint[2][0] = -0.006689760368;
    oMPC.m_RSetpoint[2][1] = 0.002865429502;
    oMPC.m_RSetpoint[2][2] = 0.999973535538;

    Ut_MPC_Custom_SetReturnCode(UT_MPC_PX4LIB_GETPX4TIMEUS_INDEX, 1, 1);
    oMPC.ControlAuto(0.014178999700);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], -0.000066571869, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], -0.000139073905, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], -0.000246876385, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0],  0.000100608748, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1],  0.000175435256, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.123394995928, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -0.0382288098, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  0.00106973771, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.49725342, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.557610392570, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  0.023075357080, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.999729216099, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.003019101685, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.999711394310, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  0.023094942793, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.006621827371, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.006689760368, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.002865429502, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.999973535538, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.002865497489, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.006689810194, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.547718405724, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, -0.016529330984, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, -0.036253951490, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -5.474771976471, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");
}



/**
 * Test MPC_AppMain(), Nominal - ControlNonManual
 */
void Test_MPC_AppMain_Nominal_ControlNonManual(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* PASS 1 - Initializing. */
    oMPC.m_VehicleStatusMsg.Timestamp = 32043154862;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)4;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)1;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)1;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 32041122164;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 1;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 32043154215;
    oMPC.m_ControlStateMsg.AccX = -0.234742432833;
    oMPC.m_ControlStateMsg.AccY = 0.068961113691;
    oMPC.m_ControlStateMsg.AccZ = -10.363160133362;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.689731538296;
    oMPC.m_ControlStateMsg.Q[1] = 0.006344274618;
    oMPC.m_ControlStateMsg.Q[2] = -0.012429238297;
    oMPC.m_ControlStateMsg.Q[3] = 0.723930716515;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = 0.003042250173;
    oMPC.m_ControlStateMsg.PitchRate = -0.004442838021;
    oMPC.m_ControlStateMsg.YawRate = -0.004229306709;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 32043147103;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = -0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.58806074;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.70097661;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.71318424;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 32043154862;
    oMPC.m_VehicleControlModeMsg.Armed = 0;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 32043159089;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 32043159089;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397743297475;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593786945;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 32043159089;
    oMPC.m_VehicleLocalPositionMsg.X = 0.000201690331;
    oMPC.m_VehicleLocalPositionMsg.Y = 0.000447960949;
    oMPC.m_VehicleLocalPositionMsg.Z = -0.000000001442;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = 0.001371126156;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.009506213479;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.488032341003;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.619292497635;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.269012451172;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = -0.000000000128;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.488032341003;
    oMPC.m_VehicleLocalPositionMsg.EpH = 3.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EpV = 2.000424146652;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 0;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 0;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 0;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 0;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = 0.000000000000;
    oMPC.m_HomePositionMsg.Timestamp = 32043143528;
    oMPC.m_HomePositionMsg.Lat = 47.397743299551;
    oMPC.m_HomePositionMsg.Lon = 8.545593796525;
    oMPC.m_HomePositionMsg.Alt = 488.269012451172;
    oMPC.m_HomePositionMsg.X = 0.000230828955;
    oMPC.m_HomePositionMsg.Y = 0.000721091288;
    oMPC.m_HomePositionMsg.Z = -0.000000001134;
    oMPC.m_HomePositionMsg.Yaw = 1.619436144829;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.269012451172;
    oMPC.m_RefPos.timestamp = 32043159139;
    oMPC.m_RefPos.lat_rad = 0.827246678556;
    oMPC.m_RefPos.lon_rad = 0.149148748120;
    oMPC.m_RefPos.sin_lat = 0.736070426510;
    oMPC.m_RefPos.cos_lat = 0.676904961732;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 32043159089;
    oMPC.m_ResetPositionSetpoint = 1;
    oMPC.m_ResetAltitudeSetpoint = 1;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 0;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 1;
    oMPC.m_ResetIntXY = 1;
    oMPC.m_ResetYawSetpoint = 0;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.000000000000;
    oMPC.m_ThrustInt[1] = 0.000000000000;
    oMPC.m_ThrustInt[2] = 0.000000000000;
    oMPC.m_Position[0] = 0.000201690331;
    oMPC.m_Position[1] = 0.000447960949;
    oMPC.m_Position[2] = -0.000000001442;
    oMPC.m_PositionSetpoint[0] = 0.000000000000;
    oMPC.m_PositionSetpoint[1] = 0.000000000000;
    oMPC.m_PositionSetpoint[2] = 0.000000000000;
    oMPC.m_Velocity[0] = 0.001371126156;
    oMPC.m_Velocity[1] = 0.009506213479;
    oMPC.m_Velocity[2] = -0.488032341003;
    oMPC.m_VelocitySetpoint[0] = 0.000000000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000000000;
    oMPC.m_VelocityPrevious[0] = 0.001327236881;
    oMPC.m_VelocityPrevious[1] = 0.009677568451;
    oMPC.m_VelocityPrevious[2] = -0.482442229986;
    oMPC.m_VelocitySetpointPrevious[0] = 0.001327236881;
    oMPC.m_VelocitySetpointPrevious[1] = 0.009677568451;
    oMPC.m_VelocitySetpointPrevious[2] = -0.482442229986;
    oMPC.m_VelocityErrD[0] = -0.001915354049;
    oMPC.m_VelocityErrD[1] = 0.015473826788;
    oMPC.m_VelocityErrD[2] = 0.239046335220;
    oMPC.m_CurrentPositionSetpoint[0] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[1] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[2] = 0.000000000000;
    oMPC.m_Rotation[0][0] = -0.048460334539;
    oMPC.m_Rotation[0][1] = -0.998793423176;
    oMPC.m_Rotation[0][2] = -0.007960043848;
    oMPC.m_Rotation[1][0] = 0.998477995396;
    oMPC.m_Rotation[1][1] = -0.048231869936;
    oMPC.m_Rotation[1][2] = -0.026747506112;
    oMPC.m_Rotation[2][0] = 0.026331305504;
    oMPC.m_Rotation[2][1] = -0.009244121611;
    oMPC.m_Rotation[2][2] = 0.999610543251;
    oMPC.m_Yaw = 1.619292497635;
    oMPC.m_YawTakeoff = 0.000000000000;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.000000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = 1.000000000000;
    oMPC.m_RSetpoint[0][1] = 0.000000000000;
    oMPC.m_RSetpoint[0][2] = 0.000000000000;
    oMPC.m_RSetpoint[1][0] = 0.000000000000;
    oMPC.m_RSetpoint[1][1] = 1.000000000000;
    oMPC.m_RSetpoint[1][2] = 0.000000000000;
    oMPC.m_RSetpoint[2][0] = 0.000000000000;
    oMPC.m_RSetpoint[2][1] = 0.000000000000;
    oMPC.m_RSetpoint[2][2] = 1.000000000000;

    oMPC.ControlNonManual(0.012037999928);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], 0.000000000000, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], 0.000000000000, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], 0.000118779142, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0],  0.000000000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1],  0.000000000000, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.422252237797, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0],  0.000201690331, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  0.000447960949, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -0.000000001442, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.119999997318, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0], -0.017263552174, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.999850988388, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.999850988388, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1], -0.017263552174, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  1.000000000000, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.000000000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, -0.000000000000, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.588060736656, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX,  0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY,  0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -0.000000000000, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    //UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");

    /* PASS 2 - Transition to takeoff. */
    oMPC.m_VehicleStatusMsg.Timestamp = 33671229639;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)17;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)2;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)1;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 33671297774;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 0;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 33671300139;
    oMPC.m_ControlStateMsg.AccX = -0.023082222790;
    oMPC.m_ControlStateMsg.AccY = 0.250855356455;
    oMPC.m_ControlStateMsg.AccZ = -10.073497772217;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.617908656597;
    oMPC.m_ControlStateMsg.Q[1] = -0.000546063937;
    oMPC.m_ControlStateMsg.Q[2] = -0.016150537878;
    oMPC.m_ControlStateMsg.Q[3] = 0.786083877087;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = -0.005168430042;
    oMPC.m_ControlStateMsg.PitchRate = -0.006302529015;
    oMPC.m_ControlStateMsg.YawRate = -0.001021062024;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 33671277064;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 47.397743504482;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 8.545593681335;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 490.789276123047;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 1.809637427330;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)3;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 33671288749;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00116860;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.01192364;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.80963743;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.61781979;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00432684;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00414279;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.78629696;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.43548700;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 33671229639;
    oMPC.m_VehicleControlModeMsg.Armed = 1;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397743509334;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593597490;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.X = -0.000593128381;
    oMPC.m_VehicleLocalPositionMsg.Y = 0.006940861698;
    oMPC.m_VehicleLocalPositionMsg.Z = -0.023656614125;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = -0.003156758146;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.031119752675;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.166829451919;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.809476375580;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.269012451172;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = 0.024133512750;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.166829451919;
    oMPC.m_VehicleLocalPositionMsg.EpH = 0.411934226751;
    oMPC.m_VehicleLocalPositionMsg.EpV = 0.275059789419;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 33671288750;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = -0.000539467554;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 0.006310853176;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = -2.520263671875;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 1.809637427330;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000020811351;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = -0.000289084419;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = -0.058515001088;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.007203754503;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = -0.050698895007;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -4.273990154266;
    oMPC.m_HomePositionMsg.Timestamp = 33671219483;
    oMPC.m_HomePositionMsg.Lat = 47.397743504982;
    oMPC.m_HomePositionMsg.Lon = 8.545593667673;
    oMPC.m_HomePositionMsg.Alt = 488.283905029297;
    oMPC.m_HomePositionMsg.X = -0.000483906595;
    oMPC.m_HomePositionMsg.Y = 0.005282537080;
    oMPC.m_HomePositionMsg.Z = -0.014878900722;
    oMPC.m_HomePositionMsg.Yaw = 1.809811234474;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.269012451172;
    oMPC.m_RefPos.timestamp = 33671300423;
    oMPC.m_RefPos.lat_rad = 0.827246682254;
    oMPC.m_RefPos.lon_rad = 0.149148744814;
    oMPC.m_RefPos.sin_lat = 0.736070429013;
    oMPC.m_RefPos.cos_lat = 0.676904959010;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 33671300317;
    oMPC.m_ResetPositionSetpoint = 0;
    oMPC.m_ResetAltitudeSetpoint = 0;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 1;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 0;
    oMPC.m_ResetIntXY = 0;
    oMPC.m_ResetYawSetpoint = 1;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.000005650265;
    oMPC.m_ThrustInt[1] = -0.000070955997;
    oMPC.m_ThrustInt[2] = 0.000187757949;
    oMPC.m_Position[0] = -0.000593128381;
    oMPC.m_Position[1] = 0.006940861698;
    oMPC.m_Position[2] = -0.023656614125;
    oMPC.m_PositionSetpoint[0] = -0.000539467554;
    oMPC.m_PositionSetpoint[1] = 0.006310853176;
    oMPC.m_PositionSetpoint[2] = -2.520263671875;
    oMPC.m_Velocity[0] = -0.003156758146;
    oMPC.m_Velocity[1] = 0.031119752675;
    oMPC.m_Velocity[2] = -0.166829451919;
    oMPC.m_VelocitySetpoint[0] = 0.000020811351;
    oMPC.m_VelocitySetpoint[1] = -0.000289084419;
    oMPC.m_VelocitySetpoint[2] = -0.058515001088;
    oMPC.m_VelocityPrevious[0] = -0.002393212635;
    oMPC.m_VelocityPrevious[1] = 0.030026217923;
    oMPC.m_VelocityPrevious[2] = -0.165472149849;
    oMPC.m_VelocitySetpointPrevious[0] = 0.000020811351;
    oMPC.m_VelocitySetpointPrevious[1] = -0.000289084419;
    oMPC.m_VelocitySetpointPrevious[2] = -0.058515001088;
    oMPC.m_VelocityErrD[0] = 0.044798217714;
    oMPC.m_VelocityErrD[1] = -0.070677615702;
    oMPC.m_VelocityErrD[2] = 0.132893085480;
    oMPC.m_CurrentPositionSetpoint[0] = -0.000539467554;
    oMPC.m_CurrentPositionSetpoint[1] = 0.006310853176;
    oMPC.m_CurrentPositionSetpoint[2] = -2.520263671875;
    oMPC.m_Rotation[0][0] = -0.236377269030;
    oMPC.m_Rotation[0][1] = -0.971438407898;
    oMPC.m_Rotation[0][2] = -0.020817618817;
    oMPC.m_Rotation[1][0] = 0.971473693848;
    oMPC.m_Rotation[1][1] = -0.235856205225;
    oMPC.m_Rotation[1][2] = -0.024716520682;
    oMPC.m_Rotation[2][0] = 0.019100610167;
    oMPC.m_Rotation[2][1] = -0.026066189632;
    oMPC.m_Rotation[2][2] = 0.999477863312;
    oMPC.m_Yaw = 1.809476375580;
    oMPC.m_YawTakeoff = 1.809818625450;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 1;
    oMPC.m_TakeoffVelLimit = -0.500000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = -0.236559972167;
    oMPC.m_RSetpoint[0][1] = -0.971615433693;
    oMPC.m_RSetpoint[0][2] = -0.001685365918;
    oMPC.m_RSetpoint[1][0] = 0.971543788910;
    oMPC.m_RSetpoint[1][1] = -0.236563086510;
    oMPC.m_RSetpoint[1][2] = 0.011861341074;
    oMPC.m_RSetpoint[2][0] = -0.011923358776;
    oMPC.m_RSetpoint[2][1] = 0.001168511692;
    oMPC.m_RSetpoint[2][2] = 0.999928176403;

    oMPC.ControlNonManual(0.011679000221);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], -0.0000417999763, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], -0.000151194414, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2],  0.00127921882, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.0234711003, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.00323182391, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2],  0.456203759, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -0.0413683914, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  0.00132636249, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.52026367, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.12572594, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0], -0.236379489, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.971443236, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.0205576271, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.970802546, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1], -0.235228792, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.0470080934, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.0408299528, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.0310691465, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.998682916, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0],  0.617874324, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1], -0.00644910522, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2],  0.0248382147, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3],  0.785857916, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.0311000906, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.0408413075, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.80963743, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, -0.0254337303, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, -0.0581580326, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -1.23556244, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    //UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");
}



/**
 * Test MPC_AppMain(), Nominal - DoControl
 */
void Test_MPC_AppMain_Nominal_DoControl(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* PASS 1 - Initializing. */
    oMPC.m_VehicleStatusMsg.Timestamp = 33671229639;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)17;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)2;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)1;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 33671297774;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 0;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 33671300139;
    oMPC.m_ControlStateMsg.AccX = -0.023082222790;
    oMPC.m_ControlStateMsg.AccY = 0.250855356455;
    oMPC.m_ControlStateMsg.AccZ = -10.073497772217;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.617908656597;
    oMPC.m_ControlStateMsg.Q[1] = -0.000546063937;
    oMPC.m_ControlStateMsg.Q[2] = -0.016150537878;
    oMPC.m_ControlStateMsg.Q[3] = 0.786083877087;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = -0.005168430042;
    oMPC.m_ControlStateMsg.PitchRate = -0.006302529015;
    oMPC.m_ControlStateMsg.YawRate = -0.001021062024;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 33671277064;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 47.397743504482;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 8.545593681335;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 490.789276123047;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 1.809637427330;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)3;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 33671288749;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00116860;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.01192364;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.80963743;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.61781979;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = -0.00432684;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00414279;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.78629696;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.43548700;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 33671229639;
    oMPC.m_VehicleControlModeMsg.Armed = 1;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397743509334;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593597490;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 33671300317;
    oMPC.m_VehicleLocalPositionMsg.X = -0.000593128381;
    oMPC.m_VehicleLocalPositionMsg.Y = 0.006940861698;
    oMPC.m_VehicleLocalPositionMsg.Z = -0.023656614125;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = -0.003156758146;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.031119752675;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.166829451919;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.809476375580;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.269012451172;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = 0.024133512750;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.166829451919;
    oMPC.m_VehicleLocalPositionMsg.EpH = 0.411934226751;
    oMPC.m_VehicleLocalPositionMsg.EpV = 0.275059789419;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 33671288750;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = -0.000539467554;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 0.006310853176;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = -2.520263671875;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 1.809637427330;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000020811351;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = -0.000289084419;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = -0.058515001088;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.007203754503;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = -0.050698895007;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -4.273990154266;
    oMPC.m_HomePositionMsg.Timestamp = 33671219483;
    oMPC.m_HomePositionMsg.Lat = 47.397743504982;
    oMPC.m_HomePositionMsg.Lon = 8.545593667673;
    oMPC.m_HomePositionMsg.Alt = 488.283905029297;
    oMPC.m_HomePositionMsg.X = -0.000483906595;
    oMPC.m_HomePositionMsg.Y = 0.005282537080;
    oMPC.m_HomePositionMsg.Z = -0.014878900722;
    oMPC.m_HomePositionMsg.Yaw = 1.809811234474;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.269012451172;
    oMPC.m_RefPos.timestamp = 33671300423;
    oMPC.m_RefPos.lat_rad = 0.827246682254;
    oMPC.m_RefPos.lon_rad = 0.149148744814;
    oMPC.m_RefPos.sin_lat = 0.736070429013;
    oMPC.m_RefPos.cos_lat = 0.676904959010;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 33671300317;
    oMPC.m_ResetPositionSetpoint = 0;
    oMPC.m_ResetAltitudeSetpoint = 0;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 1;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 0;
    oMPC.m_ResetIntXY = 0;
    oMPC.m_ResetYawSetpoint = 1;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.000005650265;
    oMPC.m_ThrustInt[1] = -0.000070955997;
    oMPC.m_ThrustInt[2] = 0.000187757949;
    oMPC.m_Position[0] = -0.000593128381;
    oMPC.m_Position[1] = 0.006940861698;
    oMPC.m_Position[2] = -0.023656614125;
    oMPC.m_PositionSetpoint[0] = -0.000539467554;
    oMPC.m_PositionSetpoint[1] = 0.006310853176;
    oMPC.m_PositionSetpoint[2] = -2.520263671875;
    oMPC.m_Velocity[0] = -0.003156758146;
    oMPC.m_Velocity[1] = 0.031119752675;
    oMPC.m_Velocity[2] = -0.166829451919;
    oMPC.m_VelocitySetpoint[0] = 0.000020811351;
    oMPC.m_VelocitySetpoint[1] = -0.000289084419;
    oMPC.m_VelocitySetpoint[2] = -0.058515001088;
    oMPC.m_VelocityPrevious[0] = -0.002393212635;
    oMPC.m_VelocityPrevious[1] = 0.030026217923;
    oMPC.m_VelocityPrevious[2] = -0.165472149849;
    oMPC.m_VelocitySetpointPrevious[0] = 0.000020811351;
    oMPC.m_VelocitySetpointPrevious[1] = -0.000289084419;
    oMPC.m_VelocitySetpointPrevious[2] = -0.058515001088;
    oMPC.m_VelocityErrD[0] = 0.044798217714;
    oMPC.m_VelocityErrD[1] = -0.070677615702;
    oMPC.m_VelocityErrD[2] = 0.132893085480;
    oMPC.m_CurrentPositionSetpoint[0] = -0.000539467554;
    oMPC.m_CurrentPositionSetpoint[1] = 0.006310853176;
    oMPC.m_CurrentPositionSetpoint[2] = -2.520263671875;
    oMPC.m_Rotation[0][0] = -0.236377269030;
    oMPC.m_Rotation[0][1] = -0.971438407898;
    oMPC.m_Rotation[0][2] = -0.020817618817;
    oMPC.m_Rotation[1][0] = 0.971473693848;
    oMPC.m_Rotation[1][1] = -0.235856205225;
    oMPC.m_Rotation[1][2] = -0.024716520682;
    oMPC.m_Rotation[2][0] = 0.019100610167;
    oMPC.m_Rotation[2][1] = -0.026066189632;
    oMPC.m_Rotation[2][2] = 0.999477863312;
    oMPC.m_Yaw = 1.809476375580;
    oMPC.m_YawTakeoff = 1.809818625450;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 1;
    oMPC.m_TakeoffVelLimit = -0.500000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = -0.236559972167;
    oMPC.m_RSetpoint[0][1] = -0.971615433693;
    oMPC.m_RSetpoint[0][2] = -0.001685365918;
    oMPC.m_RSetpoint[1][0] = 0.971543788910;
    oMPC.m_RSetpoint[1][1] = -0.236563086510;
    oMPC.m_RSetpoint[1][2] = 0.011861341074;
    oMPC.m_RSetpoint[2][0] = -0.011923358776;
    oMPC.m_RSetpoint[2][1] = 0.001168511692;
    oMPC.m_RSetpoint[2][2] = 0.999928176403;

    oMPC.DoControl(0.011679000221);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], -0.0000417999763, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], -0.000151194414, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2],  0.00127921882, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.0234711003, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.00323182391, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2],  0.456203759, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -0.0413683914, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  0.00132636249, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.52026367, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.12572594, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0], -0.236379489, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.971443236, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.0205576271, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.970802546, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1], -0.235228792, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.0470080934, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.0408299528, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.0310691465, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.998682916, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.0311000906, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.0408413075, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.80963743, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, -0.0254337303, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, -0.0581580326, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -1.23556244, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    //UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");

    /* PASS 2 - Received triplet to transition to takeoff. */
    oMPC.m_VehicleStatusMsg.Timestamp = 24032670706;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)17;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)2;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)2;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 24027002927;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 1;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 24032731497;
    oMPC.m_ControlStateMsg.AccX = 0.309893906116;
    oMPC.m_ControlStateMsg.AccY = 2.341282367706;
    oMPC.m_ControlStateMsg.AccZ = -9.519306182861;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.710987925529;
    oMPC.m_ControlStateMsg.Q[1] = -0.096762053668;
    oMPC.m_ControlStateMsg.Q[2] = -0.078669473529;
    oMPC.m_ControlStateMsg.Q[3] = 0.692058146000;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = -0.006228174083;
    oMPC.m_ControlStateMsg.PitchRate = -0.005193498451;
    oMPC.m_ControlStateMsg.YawRate = 0.003723319387;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 0;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)0;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)0;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 24032719721;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 47.397742018286;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 8.545593861439;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 491.345642089844;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 1.541023015976;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)3;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 24032720693;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.54103744;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 24032670706;
    oMPC.m_VehicleControlModeMsg.Armed = 1;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 24032731662;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 24032731662;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397741988665;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593887673;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 24032731662;
    oMPC.m_VehicleLocalPositionMsg.X = 0.003216056153;
    oMPC.m_VehicleLocalPositionMsg.Y = -0.002135221381;
    oMPC.m_VehicleLocalPositionMsg.Z = 0.037966083735;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = 0.010284845717;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.018262712285;
    oMPC.m_VehicleLocalPositionMsg.VZ = 0.002778425580;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.541051983833;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 488.884002685547;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = -0.001209149254;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = -0.002778425580;
    oMPC.m_VehicleLocalPositionMsg.EpH = 0.426289767027;
    oMPC.m_VehicleLocalPositionMsg.EpV = 0.174988523126;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 24032720694;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = 0.003091249382;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = -0.002206754638;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = 0.038970947266;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 1.541037440300;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.000000000000;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -0.000000000000;
    oMPC.m_HomePositionMsg.Timestamp = 24032660299;
    oMPC.m_HomePositionMsg.Lat = 47.397742015757;
    oMPC.m_HomePositionMsg.Lon = 8.545593857299;
    oMPC.m_HomePositionMsg.Alt = 488.844818115234;
    oMPC.m_HomePositionMsg.X = 0.003012487665;
    oMPC.m_HomePositionMsg.Y = -0.002286192495;
    oMPC.m_HomePositionMsg.Z = 0.039181273431;
    oMPC.m_HomePositionMsg.Yaw = 1.541115880013;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 488.884002685547;
    oMPC.m_RefPos.timestamp = 24032731790;
    oMPC.m_RefPos.lat_rad = 0.827246655713;
    oMPC.m_RefPos.lon_rad = 0.149148749878;
    oMPC.m_RefPos.sin_lat = 0.736070411047;
    oMPC.m_RefPos.cos_lat = 0.676904978546;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 24032731662;
    oMPC.m_ResetPositionSetpoint = 0;
    oMPC.m_ResetAltitudeSetpoint = 0;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 1;
    oMPC.m_PositionHoldEngaged = 0;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 0;
    oMPC.m_RunAltControl = 0;
    oMPC.m_ResetIntZ = 1;
    oMPC.m_ResetIntXY = 1;
    oMPC.m_ResetYawSetpoint = 1;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.000000000000;
    oMPC.m_ThrustInt[1] = 0.000000000000;
    oMPC.m_ThrustInt[2] = 0.000535357744;
    oMPC.m_Position[0] = 0.003216056153;
    oMPC.m_Position[1] = -0.002135221381;
    oMPC.m_Position[2] = 0.037966083735;
    oMPC.m_PositionSetpoint[0] = 0.003091249382;
    oMPC.m_PositionSetpoint[1] = -0.002206754638;
    oMPC.m_PositionSetpoint[2] = 0.038970947266;
    oMPC.m_Velocity[0] = 0.010284845717;
    oMPC.m_Velocity[1] = 0.018262712285;
    oMPC.m_Velocity[2] = 0.002778425580;
    oMPC.m_VelocitySetpoint[0] = 0.000000000000;
    oMPC.m_VelocitySetpoint[1] = 0.000000000000;
    oMPC.m_VelocitySetpoint[2] = 0.000000000000;
    oMPC.m_VelocityPrevious[0] = 0.011032714508;
    oMPC.m_VelocityPrevious[1] = 0.019521865994;
    oMPC.m_VelocityPrevious[2] = 0.003654700937;
    oMPC.m_VelocitySetpointPrevious[0] = 0.000000000000;
    oMPC.m_VelocitySetpointPrevious[1] = 0.000000000000;
    oMPC.m_VelocitySetpointPrevious[2] = 0.000000000000;
    oMPC.m_VelocityErrD[0] = 0.019077949226;
    oMPC.m_VelocityErrD[1] = 0.028009349480;
    oMPC.m_VelocityErrD[2] = 0.106526166201;
    oMPC.m_CurrentPositionSetpoint[0] = -5227320.500000000000;
    oMPC.m_CurrentPositionSetpoint[1] = -1067128.125000000000;
    oMPC.m_CurrentPositionSetpoint[2] = 488.884002685547;
    oMPC.m_Rotation[0][0] = 0.029733359814;
    oMPC.m_Rotation[0][1] = -0.968865513802;
    oMPC.m_Rotation[0][2] = -0.245796024799;
    oMPC.m_Rotation[1][0] = 0.999314427376;
    oMPC.m_Rotation[1][1] = 0.023385345936;
    oMPC.m_Rotation[1][2] = 0.028705596924;
    oMPC.m_Rotation[2][0] = -0.022063843906;
    oMPC.m_Rotation[2][1] = -0.246481001377;
    oMPC.m_Rotation[2][2] = 0.968896508217;
    oMPC.m_Yaw = 1.541051387787;
    oMPC.m_YawTakeoff = 1.541065335274;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 0.000000000000;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = 1.000000000000;
    oMPC.m_RSetpoint[0][1] = 0.000000000000;
    oMPC.m_RSetpoint[0][2] = 0.000000000000;
    oMPC.m_RSetpoint[1][0] = 0.000000000000;
    oMPC.m_RSetpoint[1][1] = 1.000000000000;
    oMPC.m_RSetpoint[1][2] = 0.000000000000;
    oMPC.m_RSetpoint[2][0] = 0.000000000000;
    oMPC.m_RSetpoint[2][1] = 0.000000000000;
    oMPC.m_RSetpoint[2][2] = 1.000000000000;

    oMPC.DoControl(0.012089000084);

    UtAssert_True(oMPC.m_ResetIntXY == false, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetIntZ == false, "oMPC.m_ResetIntZ");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], -0.0000816519387, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], -0.0000508740704, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], -0.000114646195, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.0234863255, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1], -0.00277875992, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.0604449995, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -0.0375322029, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1], -0.00695630955, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -2.4616394, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust, 0.520086765, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.Thrust");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  0.0297684446, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.999515772, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2],  0.00906175375, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.999541104, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  0.0298176687, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2],  0.00534632057, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0], -0.00561393239, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1],  0.00889844354, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
//    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.999944627, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == true, "oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, 0.00889870152, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.RollBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, 0.00561396172, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.54102302, FLT_EPSILON, "oMPC.m_VehicleAttitudeSetpointMsg.YawBody");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX, -0.0478064008, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccX");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY, -0.028205175, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccY");
//    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -5.27533102, FLT_EPSILON, "oMPC.m_VehicleLocalPositionSetpointMsg.AccZ");
    //UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Timestamp != 0, "oMPC.m_VehicleAttitudeSetpointMsg.Timestamp");
}






/**
 * Test MPC_AppMain(), Nominal - ControlManual
 */
void Test_MPC_AppMain_Nominal_ControlManual(void)
{
    MPC oMPC;

    oMPC.InitData();
    oMPC.InitConfigTbl();

    /* Hovering stationary */
    oMPC.m_VehicleStatusMsg.Timestamp = 71387147216;
    oMPC.m_VehicleStatusMsg.SystemID = 1;
    oMPC.m_VehicleStatusMsg.ComponentID = 1;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    oMPC.m_VehicleStatusMsg.OnboardControlSensorsHealth = 0;
    oMPC.m_VehicleStatusMsg.NavState = (PX4_NavigationState_t)2;
    oMPC.m_VehicleStatusMsg.ArmingState = (PX4_ArmingState_t)2;
    oMPC.m_VehicleStatusMsg.HilState = (PX4_HilState_t)0;
    oMPC.m_VehicleStatusMsg.Failsafe = 0;
    oMPC.m_VehicleStatusMsg.SystemType = (PX4_SystemType_t)13;
    oMPC.m_VehicleStatusMsg.IsRotaryWing = 1;
    oMPC.m_VehicleStatusMsg.IsVtol = 0;
    oMPC.m_VehicleStatusMsg.VtolFwPermanentStab = 0;
    oMPC.m_VehicleStatusMsg.InTransitionMode = 0;
    oMPC.m_VehicleStatusMsg.RcInputMode = (PX4_RcInMode_t)2;
    oMPC.m_VehicleStatusMsg.DataLinkLost = 1;
    oMPC.m_VehicleStatusMsg.DataLinkLostCounter = 0;
    oMPC.m_VehicleStatusMsg.EngineFailure = 0;
    oMPC.m_VehicleStatusMsg.EngineFailureCmd = 0;
    oMPC.m_VehicleStatusMsg.MissionFailure = 0;
    oMPC.m_VehicleLandDetectedMsg.Timestamp = 71352650214;
    oMPC.m_VehicleLandDetectedMsg.AltMax = 10000.000000000000;
    oMPC.m_VehicleLandDetectedMsg.Landed = 0;
    oMPC.m_VehicleLandDetectedMsg.Freefall = 0;
    oMPC.m_VehicleLandDetectedMsg.GroundContact = 0;
    oMPC.m_ControlStateMsg.Timestamp = 71387312526;
    oMPC.m_ControlStateMsg.AccX = -0.039567112923;
    oMPC.m_ControlStateMsg.AccY = 0.473200112581;
    oMPC.m_ControlStateMsg.AccZ = -10.268331527710;
    oMPC.m_ControlStateMsg.VelX = 0.000000000000;
    oMPC.m_ControlStateMsg.VelY = 0.000000000000;
    oMPC.m_ControlStateMsg.VelZ = 0.000000000000;
    oMPC.m_ControlStateMsg.PosX = 0.000000000000;
    oMPC.m_ControlStateMsg.PosY = 0.000000000000;
    oMPC.m_ControlStateMsg.PosZ = 0.000000000000;
    oMPC.m_ControlStateMsg.Airspeed = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.VelVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.PosVariance[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.Q[0] = 0.708361566067;
    oMPC.m_ControlStateMsg.Q[1] = 0.003428169293;
    oMPC.m_ControlStateMsg.Q[2] = -0.012681787834;
    oMPC.m_ControlStateMsg.Q[3] = 0.705727636814;
    oMPC.m_ControlStateMsg.DeltaQReset[0] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[1] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[2] = 0.000000000000;
    oMPC.m_ControlStateMsg.DeltaQReset[3] = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRate = -0.144795238972;
    oMPC.m_ControlStateMsg.PitchRate = -0.034734643996;
    oMPC.m_ControlStateMsg.YawRate = 0.042580589652;
    oMPC.m_ControlStateMsg.HorzAccMag = 0.000000000000;
    oMPC.m_ControlStateMsg.RollRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.PitchRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.YawRateBias = 0.000000000000;
    oMPC.m_ControlStateMsg.AirspeedValid = 1;
    oMPC.m_ControlStateMsg.QuatResetCounter = 1;
    oMPC.m_ManualControlSetpointMsg.Timestamp = 71387286482;
    oMPC.m_ManualControlSetpointMsg.X = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Y = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Z = 0.557142853737;
    oMPC.m_ManualControlSetpointMsg.R = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Flaps = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux1 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux2 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux3 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux4 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.Aux5 = 0.000000000000;
    oMPC.m_ManualControlSetpointMsg.ModeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ReturnSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.RattitudeSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.PosctlSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.LoiterSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.AcroSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.OffboardSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.KillSwitch = (PX4_SwitchPos_t)3;
    oMPC.m_ManualControlSetpointMsg.ArmSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.TransitionSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.GearSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)-1;
    oMPC.m_ManualControlSetpointMsg.DataSource = (PX4_ManualControlDataSource_t)1;
    oMPC.m_ManualControlSetpointMsg.StabSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_ManualControlSetpointMsg.ManSwitch = (PX4_SwitchPos_t)0;
    oMPC.m_PositionSetpointTripletMsg.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Previous.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Previous.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Current.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Current.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Timestamp = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Lat = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Lon = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.X = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Y = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Z = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.VZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Alt = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yaw = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Yawspeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.PitchMin = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AX = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AY = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AZ = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.AcceptanceRadius = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingSpeed = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.CruisingThrottle = 0.000000000000;
    oMPC.m_PositionSetpointTripletMsg.Next.Valid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.Type = (PX4_SetpointType_t)0;
    oMPC.m_PositionSetpointTripletMsg.Next.PositionValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.VelocityValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.DisableMcYawControl = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.YawspeedValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.LoiterDirection = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationValid = 0;
    oMPC.m_PositionSetpointTripletMsg.Next.AccelerationIsForce = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.Timestamp = 71387315002;
    oMPC.m_VehicleAttitudeSetpointMsg.RollBody = -0.01284447;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchBody = -0.02443473;
    oMPC.m_VehicleAttitudeSetpointMsg.YawBody = 1.56718373;
    oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0] = 0.70837075;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1] = 0.00407394;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2] = -0.01318680;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3] = 0.70570564;
    oMPC.m_VehicleAttitudeSetpointMsg.Thrust = 0.43972412;
    oMPC.m_VehicleAttitudeSetpointMsg.LandingGear = -1.00000000;
    oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid = 1;
    oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl = 0;
    oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps = 0;
    oMPC.m_VehicleControlModeMsg.Timestamp = 71387147216;
    oMPC.m_VehicleControlModeMsg.Armed = 1;
    oMPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = 0;
    oMPC.m_VehicleControlModeMsg.SystemHilEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlManualEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAutoEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlOffboardEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlRatesEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlForceEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlVelocityEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlPositionEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = 1;
    oMPC.m_VehicleControlModeMsg.ControlTerminationEnabled = 0;
    oMPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = 0;
    oMPC.m_VehicleLocalPositionMsg.Timestamp = 71387313201;
    oMPC.m_VehicleLocalPositionMsg.RefTimestamp = 71387313201;
    oMPC.m_VehicleLocalPositionMsg.RefLat = 47.397741972655;
    oMPC.m_VehicleLocalPositionMsg.RefLon = 8.545593900131;
    oMPC.m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = 71387313201;
    oMPC.m_VehicleLocalPositionMsg.X = -1.995731592178;
    oMPC.m_VehicleLocalPositionMsg.Y = 1.565559029579;
    oMPC.m_VehicleLocalPositionMsg.Z = -0.826584398746;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_XY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_Z = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.VX = -0.027511324733;
    oMPC.m_VehicleLocalPositionMsg.VY = 0.006788865663;
    oMPC.m_VehicleLocalPositionMsg.VZ = -0.051438082010;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[0] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VXY[1] = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Delta_VZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AX = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AY = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.AZ = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.Yaw = 1.566952824593;
    oMPC.m_VehicleLocalPositionMsg.RefAlt = 504.262969970703;
    oMPC.m_VehicleLocalPositionMsg.DistBottom = 1.155246853828;
    oMPC.m_VehicleLocalPositionMsg.DistBottomRate = 0.051438082010;
    oMPC.m_VehicleLocalPositionMsg.EpH = 0.369742006063;
    oMPC.m_VehicleLocalPositionMsg.EpV = 0.216528818011;
    oMPC.m_VehicleLocalPositionMsg.EvV = 0.000000000000;
    oMPC.m_VehicleLocalPositionMsg.EstimatorType = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_XY_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.V_Z_Valid = 1;
    oMPC.m_VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oMPC.m_VehicleLocalPositionMsg.XY_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.Z_Global = 1;
    oMPC.m_VehicleLocalPositionMsg.DistBottomValid = 1;
    oMPC.m_VehicleLocalPositionSetpointMsg.Timestamp = 71387315000;
    oMPC.m_VehicleLocalPositionSetpointMsg.X = -2.018968582153;
    oMPC.m_VehicleLocalPositionSetpointMsg.Y = 1.573781371117;
    oMPC.m_VehicleLocalPositionSetpointMsg.Z = -0.799546360970;
    oMPC.m_VehicleLocalPositionSetpointMsg.Yaw = 1.567183732986;
    oMPC.m_VehicleLocalPositionSetpointMsg.VX = -0.025572603568;
    oMPC.m_VehicleLocalPositionSetpointMsg.VY = 0.008065467700;
    oMPC.m_VehicleLocalPositionSetpointMsg.VZ = -0.047619022429;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccX = 0.055766887963;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccY = 0.105148039758;
    oMPC.m_VehicleLocalPositionSetpointMsg.AccZ = -4.310579776764;
    oMPC.m_HomePositionMsg.Timestamp = 71342533449;
    oMPC.m_HomePositionMsg.Lat = 47.397741945098;
    oMPC.m_HomePositionMsg.Lon = 8.545593960621;
    oMPC.m_HomePositionMsg.Alt = 504.170379638672;
    oMPC.m_HomePositionMsg.X = -0.003064178396;
    oMPC.m_HomePositionMsg.Y = 0.004552976228;
    oMPC.m_HomePositionMsg.Z = 0.092577040195;
    oMPC.m_HomePositionMsg.Yaw = 1.543434739113;
    oMPC.m_HomePositionMsg.DirectionX = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionY = 0.000000000000;
    oMPC.m_HomePositionMsg.DirectionZ = 0.000000000000;
    oMPC.m_RefAlt = 504.262969970703;
    oMPC.m_RefPos.timestamp = 71387315018;
    oMPC.m_RefPos.lat_rad = 0.827246655434;
    oMPC.m_RefPos.lon_rad = 0.149148750096;
    oMPC.m_RefPos.sin_lat = 0.736070410858;
    oMPC.m_RefPos.cos_lat = 0.676904978752;
    oMPC.m_RefPos.init_done = 1;
    oMPC.m_RefTimestamp = 71387313201;
    oMPC.m_ResetPositionSetpoint = 0;
    oMPC.m_ResetAltitudeSetpoint = 0;
    oMPC.m_DoResetAltPos = 1;
    oMPC.m_ModeAuto = 0;
    oMPC.m_PositionHoldEngaged = 1;
    oMPC.m_AltitudeHoldEngaged = 0;
    oMPC.m_RunPosControl = 1;
    oMPC.m_RunAltControl = 1;
    oMPC.m_ResetIntZ = 0;
    oMPC.m_ResetIntXY = 0;
    oMPC.m_ResetYawSetpoint = 0;
    oMPC.m_HoldOffboardXY = 0;
    oMPC.m_HoldOffboardZ = 0;
    oMPC.m_ThrustInt[0] = 0.005063694902;
    oMPC.m_ThrustInt[1] = 0.010720678605;
    oMPC.m_ThrustInt[2] = 0.077658712864;
    oMPC.m_Position[0] = -1.995731592178;
    oMPC.m_Position[1] = 1.565559029579;
    oMPC.m_Position[2] = -0.826584398746;
    oMPC.m_PositionSetpoint[0] = -2.018968582153;
    oMPC.m_PositionSetpoint[1] = 1.573781371117;
    oMPC.m_PositionSetpoint[2] = -0.799560546875;
    oMPC.m_Velocity[0] = -0.027511324733;
    oMPC.m_Velocity[1] = 0.006788865663;
    oMPC.m_Velocity[2] = -0.051438082010;
    oMPC.m_VelocitySetpoint[0] = -0.025572603568;
    oMPC.m_VelocitySetpoint[1] = 0.008065467700;
    oMPC.m_VelocitySetpoint[2] = -0.047619022429;
    oMPC.m_VelocityPrevious[0] = -0.030311975628;
    oMPC.m_VelocityPrevious[1] = 0.008536919951;
    oMPC.m_VelocityPrevious[2] = -0.017990816385;
    oMPC.m_VelocitySetpointPrevious[0] = -0.025572603568;
    oMPC.m_VelocitySetpointPrevious[1] = 0.008065467700;
    oMPC.m_VelocitySetpointPrevious[2] = -0.047619022429;
    oMPC.m_VelocityErrD[0] = -0.014930430800;
    oMPC.m_VelocityErrD[1] = 0.010728724301;
    oMPC.m_VelocityErrD[2] = 0.159829705954;
    oMPC.m_CurrentPositionSetpoint[0] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[1] = 0.000000000000;
    oMPC.m_CurrentPositionSetpoint[2] = 0.000000000000;
    oMPC.m_Rotation[0][0] = 0.003575533628;
    oMPC.m_Rotation[0][1] = -0.999907612801;
    oMPC.m_Rotation[0][2] = -0.013127873652;
    oMPC.m_Rotation[1][0] = 0.999733686447;
    oMPC.m_Rotation[1][1] = 0.003873676062;
    oMPC.m_Rotation[1][2] = -0.022756543010;
    oMPC.m_Rotation[2][0] = 0.022805288434;
    oMPC.m_Rotation[2][1] = -0.013043008745;
    oMPC.m_Rotation[2][2] = 0.999655008316;
    oMPC.m_Yaw = 1.567219853401;
    oMPC.m_YawTakeoff = 1.543403744698;
    oMPC.m_VelMaxXy = 8.000000000000;
    oMPC.m_InTakeoff = 0;
    oMPC.m_TakeoffVelLimit = 1.180821299553;
    oMPC.m_ResetCounterZ = 0;
    oMPC.m_ResetCounterXy = 0;
    oMPC.m_HeadingResetCounter = 0;
    oMPC.m_RSetpoint[0][0] = 0.003611507826;
    oMPC.m_RSetpoint[0][1] = -0.999909877777;
    oMPC.m_RSetpoint[0][2] = -0.012932283804;
    oMPC.m_RSetpoint[1][0] = 0.999695003033;
    oMPC.m_RSetpoint[1][1] = 0.003926097415;
    oMPC.m_RSetpoint[1][2] = -0.024383721873;
    oMPC.m_RSetpoint[2][0] = 0.024432297796;
    oMPC.m_RSetpoint[2][1] = -0.012840277515;
    oMPC.m_RSetpoint[2][2] = 0.999619007111;

    oMPC.ControlManual(0.148974999785);

    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.RollBody, -0.0069380007, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.RollBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.PitchBody, -0.0134153878, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.PitchBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawBody, 1.567183732986, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.YawBody");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.YawSpMoveRate, 0.000000000000, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.YawSpMoveRate");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[0],  0.7083790898, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.Q_D[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[1],  0.0022770965, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.Q_D[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[2], -0.0072000101, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.Q_D[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Q_D[3],  0.7057917714, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.Q_D[3]");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.Thrust,  0.8200920820, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.Thrust");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleAttitudeSetpointMsg.LandingGear, -1.000000000000, FLT_EPSILON, "oMPC.VehicleAttitudeSetpoint.LandingGear");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.Q_D_Valid == 1, "oMPC.VehicleAttitudeSetpoint.Q_D_Valid");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.RollResetIntegral == 0, "oMPC.VehicleAttitudeSetpoint.RollResetIntegral");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.PitchResetIntegral == 0, "oMPC.VehicleAttitudeSetpoint.PitchResetIntegral");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.YawResetIntegral == 0, "oMPC.VehicleAttitudeSetpoint.YawResetIntegral");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.FwControlYaw == 0, "oMPC.VehicleAttitudeSetpoint.FwControlYaw");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.DisableMcYawControl == 0, "oMPC.VehicleAttitudeSetpoint.DisableMcYawControl");
    UtAssert_True(oMPC.m_VehicleAttitudeSetpointMsg.ApplyFlaps == 0, "oMPC.VehicleAttitudeSetpoint.ApplyFlaps");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.X, -2.018968582153, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.X");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.Y,  1.573781371117, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.Y");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.Z, -0.799546360970, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.Z");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.Yaw, 1.567183732986, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.Yaw");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.VX, -0.025572603568, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.VX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.VY,  0.008065467700, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.VY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.VZ, -0.047619022429, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.VZ");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccX,  0.056190323085, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.AccX");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccY,  0.107689961791, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.AccY");
    UtAssert_DoubleCmpAbs(oMPC.m_VehicleLocalPositionSetpointMsg.AccZ, -8.0419425964, FLT_EPSILON, "MPC.VehicleLocalPositionSetpointMsg.AccZ");
    UtAssert_DoubleCmpAbs(oMPC.m_RefAlt, 504.262969970703, FLT_EPSILON, "oMPC.m_RefAlt");
    UtAssert_DoubleCmpAbs(oMPC.m_RefPos.lat_rad, 0.827246655434, FLT_EPSILON, "oMPC.LatRad");
    UtAssert_DoubleCmpAbs(oMPC.m_RefPos.lon_rad, 0.149148750096, FLT_EPSILON, "oMPC.LonRad");
    UtAssert_DoubleCmpAbs(oMPC.m_RefPos.sin_lat, 0.736070410858, FLT_EPSILON, "oMPC.SinLat");
    UtAssert_DoubleCmpAbs(oMPC.m_RefPos.cos_lat, 0.676904978752, FLT_EPSILON, "oMPC.CosLat");
    UtAssert_True(oMPC.m_RefPos.init_done == 1, "oMPC.init_done");
    UtAssert_True(oMPC.m_ResetPositionSetpoint == 0, "oMPC.m_ResetPositionSetpoint");
    UtAssert_True(oMPC.m_ResetAltitudeSetpoint == 0, "oMPC.m_ResetAltitudeSetpoint");
    UtAssert_True(oMPC.m_DoResetAltPos == 1, "oMPC.m_DoResetAltPos");
    UtAssert_True(oMPC.m_ModeAuto == 0, "oMPC.m_ModeAuto");
    UtAssert_True(oMPC.m_PositionHoldEngaged == 1, "oMPC.m_PositionHoldEngaged");
    UtAssert_True(oMPC.m_AltitudeHoldEngaged == 0, "oMPC.m_AltitudeHoldEngaged");
    UtAssert_True(oMPC.m_RunPosControl == 1, "oMPC.m_RunPosControl");
    UtAssert_True(oMPC.m_RunAltControl == 0, "oMPC.m_RunAltControl");
    UtAssert_True(oMPC.m_ResetIntZ == 0, "oMPC.m_ResetIntZ");
    UtAssert_True(oMPC.m_ResetIntXY == 0, "oMPC.m_ResetIntXY");
    UtAssert_True(oMPC.m_ResetYawSetpoint == 0, "oMPC.m_ResetYawSetpoint");
    UtAssert_True(oMPC.m_HoldOffboardXY == 0, "oMPC.m_HoldOffboardXY");
    UtAssert_True(oMPC.m_HoldOffboardZ == 0, "oMPC.m_HoldOffboardZ");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[0], 0.005225665867, FLT_EPSILON, "oMPC.m_ThrustInt[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[1], 0.010751139373, FLT_EPSILON, "oMPC.m_ThrustInt[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_ThrustInt[2], 0.0628465563, FLT_EPSILON, "oMPC.m_ThrustInt[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_Position[0], -1.995731592178, FLT_EPSILON, "oMPC.m_Position[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_Position[1],  1.565559029579, FLT_EPSILON, "oMPC.m_Position[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_Position[2], -0.826584398746, FLT_EPSILON, "oMPC.m_Position[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[0], -2.0189685821534, FLT_EPSILON, "oMPC.m_PositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[1],  1.573781371117, FLT_EPSILON, "oMPC.m_PositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_PositionSetpoint[2], -0.826584398746, FLT_EPSILON, "oMPC.m_PositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_Velocity[0], -0.027511324733, FLT_EPSILON, "oMPC.m_Velocity[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_Velocity[1],  0.006788865663, FLT_EPSILON, "oMPC.m_Velocity[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_Velocity[2], -0.051438082010, FLT_EPSILON, "oMPC.m_Velocity[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[0], -0.022075140849, FLT_EPSILON, "oMPC.m_VelocitySetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[1],  0.007811224554, FLT_EPSILON, "oMPC.m_VelocitySetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpoint[2], -0.7142857313, FLT_EPSILON, "oMPC.m_VelocitySetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityPrevious[0], -0.030311975628, FLT_EPSILON, "oMPC.m_VelocityPrevious[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityPrevious[1],  0.008536919951, FLT_EPSILON, "oMPC.m_VelocityPrevious[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityPrevious[2], -0.017990816385, FLT_EPSILON, "oMPC.m_VelocityPrevious[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[0], -0.022075140849, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[1],  0.007811224554, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocitySetpointPrevious[2], -0.7142857313, FLT_EPSILON, "oMPC.m_VelocitySetpointPrevious[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[0], -0.014930430800, FLT_EPSILON, "oMPC.m_VelocityErrD[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[1],  0.010728724301, FLT_EPSILON, "oMPC.m_VelocityErrD[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_VelocityErrD[2],  0.159829705954, FLT_EPSILON, "oMPC.m_VelocityErrD[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_CurrentPositionSetpoint[0], 0.000000000000, FLT_EPSILON, "oMPC.m_CurrentPositionSetpoint[0]");
    UtAssert_DoubleCmpAbs(oMPC.m_CurrentPositionSetpoint[1], 0.000000000000, FLT_EPSILON, "oMPC.m_CurrentPositionSetpoint[1]");
    UtAssert_DoubleCmpAbs(oMPC.m_CurrentPositionSetpoint[2], 0.000000000000, FLT_EPSILON, "oMPC.m_CurrentPositionSetpoint[2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][0],  0.0036122610, FLT_EPSILON, "oMPC.m_RSetpoint[0][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][1], -0.9999690652, FLT_EPSILON, "oMPC.m_RSetpoint[0][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[0][2], -0.0069863610, FLT_EPSILON, "oMPC.m_RSetpoint[0][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][0],  0.9999035001, FLT_EPSILON, "oMPC.m_RSetpoint[1][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][1],  0.0037055709, FLT_EPSILON, "oMPC.m_RSetpoint[1][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[1][2], -0.0133895101, FLT_EPSILON, "oMPC.m_RSetpoint[1][2]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][0],  0.0134149855, FLT_EPSILON, "oMPC.m_RSetpoint[2][0]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][1], -0.0069373203, FLT_EPSILON, "oMPC.m_RSetpoint[2][1]");
    UtAssert_DoubleCmpAbs(oMPC.m_RSetpoint[2][2],  0.9998859167, FLT_EPSILON, "oMPC.m_RSetpoint[2][2]");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void MPC_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_MPC_InitEvent_Fail_Register, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitEvent_Fail_Register");

    UtTest_Add(Test_MPC_InitPipe_Fail_CreateSCHPipe, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_MPC_InitPipe_Fail_SubscribeWakeup, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_MPC_InitPipe_Fail_SubscribeSendHK, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_MPC_InitPipe_Fail_CreateCMDPipe, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_MPC_InitPipe_Fail_SubscribeCMD, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_MPC_InitPipe_Fail_CreateDATAPipe, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_MPC_InitData, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitData");

    UtTest_Add(Test_MPC_InitApp_Fail_InitEvent, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitApp_Fail_InitEvent");
    UtTest_Add(Test_MPC_InitApp_Fail_InitPipe, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitApp_Fail_InitPipe");
    UtTest_Add(Test_MPC_InitApp_Fail_InitData, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitApp_Fail_InitData");
    UtTest_Add(Test_MPC_InitApp_Fail_InitConfigTbl, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_MPC_InitApp_Nominal, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_InitApp_Nominal");

    UtTest_Add(Test_MPC_AppMain_Fail_RegisterApp, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_MPC_AppMain_Fail_InitApp, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Fail_InitApp");
    UtTest_Add(Test_MPC_AppMain_Fail_AcquireConfigPtrs, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_MPC_AppMain_InvalidSchMessage, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_InvalidSchMessage");
    UtTest_Add(Test_MPC_AppMain_Nominal_SendHK, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_SendHK");
    UtTest_Add(Test_MPC_AppMain_Nominal_Wakeup, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_MPC_AppMain_Nominal_ProcessControlStateMsg, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ProcessControlStateMsg");

    UtTest_Add(Test_MPC_AppMain_Nominal_ProcessVehicleLocalPositionMsg, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ProcessVehicleLocalPositionMsg");

    UtTest_Add(Test_MPC_AppMain_Nominal_UpdateParamsFromTable, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_UpdateParamsFromTable");

    //UtTest_Add(Test_MPC_AppMain_Nominal_UpdateVelocityDerivative, MPC_Test_Setup, MPC_Test_TearDown,
               //"Test_MPC_AppMain_Nominal_UpdateVelocityDerivative");

    UtTest_Add(Test_MPC_AppMain_Nominal_LimitAltitude, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_LimitAltitude");

    UtTest_Add(Test_MPC_AppMain_Nominal_ApplyVelocitySetpointSlewRate, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ApplyVelocitySetpointSlewRate");

    UtTest_Add(Test_MPC_AppMain_Nominal_CalculateVelocitySetpoint, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_CalculateVelocitySetpoint");

    UtTest_Add(Test_MPC_AppMain_Nominal_CalculateThrustSetpoint, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_CalculateThrustSetpoint");

    UtTest_Add(Test_MPC_AppMain_Nominal_ControlAuto, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ControlAuto");

    UtTest_Add(Test_MPC_AppMain_Nominal_ControlNonManual, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ControlNonManual");

    UtTest_Add(Test_MPC_AppMain_Nominal_DoControl, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_DoControl");

    UtTest_Add(Test_MPC_AppMain_Nominal_ControlManual, MPC_Test_Setup, MPC_Test_TearDown,
               "Test_MPC_AppMain_Nominal_ControlManual");
}


