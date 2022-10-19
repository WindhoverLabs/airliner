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

#include "vm_test_utils.hpp"
#include "vm_app_test.hpp"

#include "cfe.h"
#include "vm_app.h"
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
CFE_SB_MsgId_t ProcessNewDataHook_MsgId = 0;


/**
 * Test VM GetPSPTimeHook
 */
void Test_VM_GetPSPTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


/**************************************************************************
 * Tests for VM_InitEvent()
 **************************************************************************/
/**
 * Test VM_InitEvent() with failed CFE_EVS_Register
 */
void Test_VM_InitEvent_Fail_Register(void)
{
    VM oVM;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for VM_InitPipe()
 **************************************************************************/
/**
 * Test VM_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_VM_InitPipe_Fail_CreateSCHPipe(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test VM_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_VM_InitPipe_Fail_SubscribeWakeup(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test VM_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_VM_InitPipe_Fail_SubscribeSendHK(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test VM_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_VM_InitPipe_Fail_CreateCMDPipe(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test VM_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_VM_InitPipe_Fail_SubscribeCMD(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test VM_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_VM_InitPipe_Fail_CreateDATAPipe(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test VM_InitPipe(), fail Subscribe for BatteryStatus
 */
void Test_VM_InitPipe_Fail_Subscribe_BatteryStatus(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for BatteryStatus");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleCommand
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleCommand(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleCommand");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleControlMode
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleControlMode(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleControlMode");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleGlobalPosition
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleGlobalPosition(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleGlobalPosition");
}


/**
 * Test VM_InitPipe(), fail Subscribe for TelemetryStatus
 */
void Test_VM_InitPipe_Fail_Subscribe_TelemetryStatus(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for TelemetryStatus");
}


/**
 * Test VM_InitPipe(), fail Subscribe for SubsystemInfo
 */
void Test_VM_InitPipe_Fail_Subscribe_SubsystemInfo(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for SubsystemInfo");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleGpsPosition
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleGpsPosition(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleGpsPosition");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleAttitude
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleAttitude(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleAttitude");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleLocalPosition
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleLocalPosition(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 11);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleLocalPosition");
}


/**
 * Test VM_InitPipe(), fail Subscribe for VehicleLandDetected
 */
void Test_VM_InitPipe_Fail_Subscribe_VehicleLandDetected(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 12);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for VehicleLandDetected");
}


/**
 * Test VM_InitPipe(), fail Subscribe for GeofenceResult
 */
void Test_VM_InitPipe_Fail_Subscribe_GeofenceResult(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 13);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for GeofenceResult");
}


/**
 * Test VM_InitPipe(), fail Subscribe for MissionResult
 */
void Test_VM_InitPipe_Fail_Subscribe_MissionResult(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 14);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for MissionResult");
}


/**
 * Test VM_InitPipe(), fail Subscribe for ManualControlSetpoint
 */
void Test_VM_InitPipe_Fail_Subscribe_ManualControlSetpoint(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 15);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for ManualControlSetpoint");
}


/**
 * Test VM_InitPipe(), fail Subscribe for PositionSetpointTriplet
 */
void Test_VM_InitPipe_Fail_Subscribe_PositionSetpointTriplet(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 16);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for PositionSetpointTriplet");
}


/**
 * Test VM_InitPipe(), fail Subscribe for OffboardControlMode
 */
void Test_VM_InitPipe_Fail_Subscribe_OffboardControlMode(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 17);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for OffboardControlMode");
}


/**
 * Test VM_InitPipe(), fail Subscribe for Safety
 */
void Test_VM_InitPipe_Fail_Subscribe_Safety(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 18);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for Safety");
}


/**
 * Test VM_InitPipe(), fail Subscribe for SensorCorrection
 */
void Test_VM_InitPipe_Fail_Subscribe_SensorCorrection(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 19);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for SensorCorrection");
}


/**
 * Test VM_InitPipe(), fail Subscribe for SensorCombined
 */
void Test_VM_InitPipe_Fail_Subscribe_SensorCombined(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 20);

    /* Execute the function being tested */
    result = oVM.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail Subscribe for SensorCombined");
}


/**************************************************************************
 * Tests for VM_InitData()
 **************************************************************************/
/**
 * Test VM_InitData()
 */
void Test_VM_InitData(void)
{
    VM oVM;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oVM.InitData();
}


/**************************************************************************
 * Tests for VM_InitApp()
 **************************************************************************/
/**
 * Test VM_InitApp(), fail init event
 */
void Test_VM_InitApp_Fail_InitEvent(void)
{
    VM oVM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test VM_InitApp(), fail init pipe
 */
void Test_VM_InitApp_Fail_InitPipe(void)
{
    VM oVM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test VM_InitApp(), fail init data.
 * NOTE: no current way to fail VM_InitData() in default
 */
void Test_VM_InitApp_Fail_InitData(void)
{
    VM oVM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oVM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test VM_InitApp(), fail init config table
 */
void Test_VM_InitApp_Fail_InitConfigTbl(void)
{
    VM oVM;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oVM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test VM_InitApp(), Nominal
 */
void Test_VM_InitApp_Nominal(void)
{
    VM oVM;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oVM.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for extern VM_AppMain()
 **************************************************************************/
/**
 * Test VM extern VM_AppMain(), Nominal
 */
void Test_VM_VM_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    VM_AppMain();
}


/**************************************************************************
 * Tests for VM_AppMain()
 **************************************************************************/
/**
 * Test VM_AppMain(), Fail RegisterApp
 */
void Test_VM_AppMain_Fail_RegisterApp(void)
{
    VM oVM;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM_AppMain(), Fail InitApp
 */
void Test_VM_AppMain_Fail_InitApp(void)
{
    VM oVM;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM_AppMain(), Fail AcquireConfigPtrs
 */
void Test_VM_AppMain_Fail_AcquireConfigPtrs(void)
{
    VM oVM;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM_AppMain(), Invalid Schedule Message
 */
void Test_VM_AppMain_InvalidSchMessage(void)
{
    VM oVM;

    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(VM_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
             "AppMain(), Invalid Schedule Message");
}


/**
 * Test VM_AppMain(), No Schedule Message
 */
void Test_VM_AppMain_NoSchMessage(void)
{
    VM oVM;

    int32  expected = CFE_SB_NO_MESSAGE;

    /* The following will emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM_AppMain(), Sch Msg Read timeout
 */
void Test_VM_AppMain_SchMsgReadTimeout(void)
{
    VM oVM;

    int32  expected = CFE_SB_TIME_OUT;

    /* The following will emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM_AppMain(), Sch Pipe read error
 */
void Test_VM_AppMain_SchPipeReadError(void)
{
    VM oVM;

    int32  expected = CFE_SB_PIPE_RD_ERR;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "SCH pipe read error (0x%08lX).", CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), Sch Pipe read error");
}


/**
 * Hook to support: VM_AppMain(), Nominal - SendHK
 */
int32 Test_VM_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test VM_AppMain(), Nominal - SendHK
 */
void Test_VM_AppMain_Nominal_SendHK(void)
{
    VM oVM;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_VM_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oVM.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test VM_AppMain(), Nominal - Wakeup
 */
void Test_VM_AppMain_Nominal_Wakeup(void)
{
    VM oVM;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM AppMain(), ProcessDataPipeHook
 */
int32 Test_VM_AppMain_ProcessDataPipeHook(void *dst, void *src, uint32 size)
{
    return 0;
}


/**
 * Test VM AppMain(), ProcessDataPipe - DataPipeNoMsg
 */
void Test_VM_AppMain_ProcessDataPipe_DataPipeNoMsg(void)
{
    VM oVM;

    int32           expected = CFE_SB_NO_MESSAGE;
    int32           SchPipe;
    VM_NoArgCmd_t   InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("VM_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, VM_WAKEUP_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_RUNLOOP_INDEX);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM AppMain(), ProcessDataPipe - InvalidDataMsg
 */
void Test_VM_AppMain_ProcessDataPipe_InvalidDataMsg(void)
{
    VM oVM;

    int32           DataPipe;
    VM_NoArgCmd_t   InMsg;
    char            expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("VM_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, 0, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_RUNLOOP_INDEX);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "Recvd invalid DATA msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(VM_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                "AppMain(), ProcessDataPipe - InvalidDataMsg");
}


/**
 * Test VM AppMain(), ProcessDataPipe - DataPipeError
 */
void Test_VM_AppMain_ProcessDataPipe_DataPipeError(void)
{
    VM oVM;

    int32           expected = CFE_SB_PIPE_RD_ERR;
    int32           SchPipe;
    VM_NoArgCmd_t   InMsg;
    char            expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("VM_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, VM_WAKEUP_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_RUNLOOP_INDEX);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "DATA pipe read error (0x%08lX)", CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), ProcessDataPipe - DataPipeError");
}


/**
 * Test VM AppMain(), ProcessDataPipe - BatteryStatus
 */
void Test_VM_AppMain_ProcessDataPipe_BatteryStatus(void)
{
    VM oVM;

    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("VM_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = VM_Test_GetTimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_WAKEUP_MID, 1);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_VM_AppMain_ProcessDataPipeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);
    Ut_CFE_ES_ContinueReturnCodeAfterCountZero(UT_CFE_ES_RUNLOOP_INDEX);

    /* Execute the function being tested */
    oVM.AppMain();
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void VM_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_VM_InitEvent_Fail_Register, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitEvent_Fail_Register");

    UtTest_Add(Test_VM_InitPipe_Fail_CreateSCHPipe, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_VM_InitPipe_Fail_SubscribeWakeup, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_VM_InitPipe_Fail_SubscribeSendHK, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_VM_InitPipe_Fail_CreateCMDPipe, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_VM_InitPipe_Fail_SubscribeCMD, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_VM_InitPipe_Fail_CreateDATAPipe, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_BatteryStatus, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_BatteryStatus");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleCommand, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleCommand");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleControlMode, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleControlMode");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleGlobalPosition, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleGlobalPosition");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_TelemetryStatus, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_TelemetryStatus");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_SubsystemInfo, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_SubsystemInfo");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleGpsPosition, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleGpsPosition");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleAttitude, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleAttitude");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleLocalPosition, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleLocalPosition");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_VehicleLandDetected, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_VehicleLandDetected");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_GeofenceResult, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_GeofenceResult");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_MissionResult, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_MissionResult");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_ManualControlSetpoint, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_ManualControlSetpoint");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_PositionSetpointTriplet, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_PositionSetpointTriplet");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_OffboardControlMode, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_OffboardControlMode");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_Safety, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_Safety");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_SensorCorrection, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_SensorCorrection");
    UtTest_Add(Test_VM_InitPipe_Fail_Subscribe_SensorCombined, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitPipe_Fail_Subscribe_SensorCombined");

    UtTest_Add(Test_VM_InitData, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitData");

    UtTest_Add(Test_VM_InitApp_Fail_InitEvent, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitApp_Fail_InitEvent");
    UtTest_Add(Test_VM_InitApp_Fail_InitPipe, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitApp_Fail_InitPipe");
    UtTest_Add(Test_VM_InitApp_Fail_InitData, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitApp_Fail_InitData");
    UtTest_Add(Test_VM_InitApp_Fail_InitConfigTbl, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_VM_InitApp_Nominal, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_InitApp_Nominal");

    UtTest_Add(Test_VM_VM_AppMain_Nominal, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_VM_AppMain_Nominal");
    UtTest_Add(Test_VM_AppMain_Fail_RegisterApp, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_VM_AppMain_Fail_InitApp, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_Fail_InitApp");
    UtTest_Add(Test_VM_AppMain_Fail_AcquireConfigPtrs, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_VM_AppMain_InvalidSchMessage, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_InvalidSchMessage");
    UtTest_Add(Test_VM_AppMain_NoSchMessage, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_NoSchMessage");
    UtTest_Add(Test_VM_AppMain_SchMsgReadTimeout, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_SchMsgReadTimeout");
    UtTest_Add(Test_VM_AppMain_SchPipeReadError, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_SchPipeReadError");
    UtTest_Add(Test_VM_AppMain_Nominal_SendHK, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_Nominal_SendHK");
    UtTest_Add(Test_VM_AppMain_Nominal_Wakeup, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_VM_AppMain_ProcessDataPipe_DataPipeNoMsg, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_ProcessDataPipe_DataPipeNoMsg");
    UtTest_Add(Test_VM_AppMain_ProcessDataPipe_InvalidDataMsg, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_ProcessDataPipe_InvalidDataMsg");
    UtTest_Add(Test_VM_AppMain_ProcessDataPipe_DataPipeError, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_ProcessDataPipe_DataPipeError");
    UtTest_Add(Test_VM_AppMain_ProcessDataPipe_BatteryStatus, VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_AppMain_ProcessDataPipe_BatteryStatus");
}
