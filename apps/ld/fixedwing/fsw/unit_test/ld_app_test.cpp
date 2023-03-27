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
#include "ld_version.h"

#include "ld_custom_stubs.hpp"
#include "ld_test_utils.hpp"
#include "ld_app_test.hpp"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include <stdarg.h>
#include <float.h>
#include <time.h>


uint16   RcvDataPipeMsgHook_MsgId = 0;
uint16   SendHKSendMsgHook_MsgId = 0;
uint16   DiagTlmSendMsgHook_MsgId = 0;

uint32   WriteToSysLog_HookCalledCnt = 0;
uint32   WriteToSysLog_HookFlag = 0;

uint32   Execute_VLandDetRcvdCnt = 0;
uint32   Execute_DiagTlmRcvdCnt = 0;
PX4_VehicleLandDetectedMsg_t  HookVLndDetect[100];
LD_Diag_t                     HookDiag;

double   ConfigData_Checksum = 0.0;



/**************************************************************************
 * Tests for LD InitEvent()
 **************************************************************************/
/**
 * Test LD InitEvent() with failed CFE_EVS_Register
 */
void Test_LD_InitEvent_Fail_Register(void)
{
    LD oLDut;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitEvent();

    sprintf(expSysLog, "LD - Failed to register with EVS (0x%08lX)\n",
            expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitEvent, failed EVS Register");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog) == TRUE,
                  "InitEvent, failed EVS Register: Sys Log Written");
}


/**************************************************************************
 * Tests for LD InitPipe()
 **************************************************************************/
/**
 * Test LD InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateSCHPipe(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Failed to create SCH pipe (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SB create SCH pipe");

    UtAssert_EventSent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SB create SCH pipe: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_LD_InitPipe_Fail_SubscribeWakeup(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to LD_WAKEUP_MID. (0x%08lX)",
            expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for wakeup");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
             "InitPipe, fail CFE_SB_SubscribeEx for wakeup: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_LD_InitPipe_Fail_SubscribeSendHK(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to LD_SEND_HK_MID. (0x%08X)",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for sendhk");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
             "InitPipe, fail CFE_SB_SubscribeEx for sendhk: Event Sent");
}


/**
 * Test LD InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateCMDPipe(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Failed to create CMD pipe (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SB create CMD pipe");

    UtAssert_EventSent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SB create CMD pipe: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_LD_InitPipe_Fail_SubscribeCMD(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent,
            "CMD Pipe failed to subscribe to LD_CMD_MID. (0x%08lX)",
            expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_Subscribe for CMD");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "InitPipe, fail CFE_SB_Subscribe for CMD: Event Sent");
}


/**
 * Test LD InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateDATAPipe(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Failed to create Data pipe (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SB create DATA pipe");

    UtAssert_EventSent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SB create DATA pipe: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for ActuatorArmed
 */
void Test_LD_InitPipe_Fail_SubscribeExActuatorArmed(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_ACTUATOR_ARMED_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for ActuatorArmed");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
       "InitPipe, fail CFE_SB_SubscribeEx for ActuatorArmed: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for Airspeed
 */
void Test_LD_InitPipe_Fail_SubscribeExAirspeed(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_AIRSPEED_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for Airspeed");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
             "InitPipe, fail CFE_SB_SubscribeEx for Airspeed: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls0
 */
void Test_LD_InitPipe_Fail_SubscribeExActuatorControls0(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_ACTUATOR_CONTROLS_0_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
             "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls0");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CFE_SB_SubscribeEx for "
                       "ActuatorControls0: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for ControlState
 */
void Test_LD_InitPipe_Fail_SubscribeExControlState(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_CONTROL_STATE_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for ControlState");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
       "InitPipe, fail CFE_SB_SubscribeEx for ControlState: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for BatteryStatus
 */
void Test_LD_InitPipe_Fail_SubscribeExBatteryStatus(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_BATTERY_STATUS_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for BatteryStatus");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
       "InitPipe, fail CFE_SB_SubscribeEx for BatteryStatus: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for VehicleAttitude
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for VehicleAttitude");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
      "InitPipe, fail CFE_SB_SubscribeEx for VehicleAttitude: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for ManualControlSetpoint
 */
void Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
            "InitPipe, fail CFE_SB_SubscribeEx for ManualControlSetpoint");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CFE_SB_SubscribeEx for "
                       "ManualControlSetpoint: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for VehicleLocalPosition
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
             "InitPipe, fail CFE_SB_SubscribeEx for VehicleLocalPosition");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CFE_SB_SubscribeEx for "
                       "VehicleLocalPosition: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for VehicleSensorCombined
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 11);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_SENSOR_COMBINED_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
           "InitPipe, fail CFE_SB_SubscribeEx for VehicleSensorCombined");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CFE_SB_SubscribeEx for "
                       "VehicleSensorCombined: Event Sent");
}


/**
 * Test LD InitPipe(), fail CFE_SB_SubscribeEx for VehicleControlMode
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 12);

    /* Execute the function being tested */
    result = oLDut.InitPipe();

    sprintf(expEvent, "Data Pipe failed to subscribe to "
            "PX4_VEHICLE_CONTROL_MODE_MID. (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for VehicleControlMode");

    UtAssert_EventSent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CFE_SB_SubscribeEx for "
                       "VehicleControlMode: Event Sent");
}



/**************************************************************************
 * Tests for LD InitData()
 **************************************************************************/
/**
 * Test LD InitData()
 */
void Test_LD_InitData(void)
{
    LD oLDut;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oLDut.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for LD InitApp()
 **************************************************************************/
/**
 * Test LD InitApp(), fail init event
 */
void Test_LD_InitApp_Fail_InitEvent(void)
{
    LD oLDut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expSysLog2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expSysLog3[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    sprintf(expSysLog1, "LD - Failed to register with EVS (0x%08lX)\n",
            expected);
    sprintf(expSysLog2, "LD - Failed to init events (0x%08lX)\n",
            expected);
    sprintf(expSysLog3, "%s", "LD - Application failed to initialize\n");

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init event");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog1) == TRUE,
                  "InitApp, fail init event: SysLog1 Written");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog2) == TRUE,
                  "InitApp, fail init event: SysLog2 Written");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog3) == TRUE,
                  "InitApp, fail init event: SysLog3 Written");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 3,
                  "InitApp, fail init event: SysLog QueueDepth");
}


/**
 * Test LD InitApp(), fail init pipe
 */
void Test_LD_InitApp_Fail_InitPipe(void)
{
    LD oLDut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    sprintf(expEvent, "Failed to create SCH pipe (0x%08lX)", expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init pipe");

    UtAssert_EventSent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitApp, fail init pipe: Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "InitApp, fail init pipe: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "InitApp, fail init pipe: Event QueueDepth");
}


/**
 * Test LD InitApp(), fail init data.
 * NOTE: no current way to fail LD_InitData() in default
 */
void Test_LD_InitApp_Fail_InitData(void)
{
    LD oLDut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init data");
}


/**
 * Test LD InitApp(), fail init config table
 */
void Test_LD_InitApp_Fail_InitConfigTbl(void)
{
    LD oLDut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    sprintf(expEvent, "Failed to register config table (0x%08lX)",
            expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init config table");

    UtAssert_EventSent(LD_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitApp, fail init config table: Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "InitApp, fail init config table: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "InitApp, fail init config table: Event QueueDepth");
}


/**
 * Test LD InitApp(), Nominal
 */
void Test_LD_InitApp_Nominal(void)
{
    LD oLDut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventOp[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    result = oLDut.InitApp();

    if (oLDut.ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_AUTO)
    {
        sprintf(expEventOp, "%s", "Operational mode auto");
    }
    else if (oLDut.ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL)
    {
        sprintf(expEventOp, "%s", "Operational mode manual");
    }

    sprintf(expEvent, "Initialized.  Version %d.%d.%d.%d",
            LD_MAJOR_VERSION, LD_MINOR_VERSION,
            LD_REVISION, LD_MISSION_REV);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, nominal");

    UtAssert_EventSent(LD_STARTUP_MODE_EID, CFE_EVS_INFORMATION,
                       expEventOp,
                       "InitApp, nominal: OpMode Event Sent");

    UtAssert_EventSent(LD_INIT_INF_EID, CFE_EVS_INFORMATION, expEvent,
                       "InitApp, nominal: Initialized Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "InitApp, nominal: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
                  "InitApp, nominal: Event QueueDepth");
}


/**************************************************************************
 * Tests for extern LD_AppMain()
 **************************************************************************/
/**
 * Test LD extern LD_AppMain(), Nominal
 */
void Test_LD_LD_AppMain_Nominal(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventOp[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    LD_AppMain();

    if (oLD.ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_AUTO)
    {
        sprintf(expEventOp, "%s", "Operational mode auto");
    }
    else if (oLD.ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL)
    {
        sprintf(expEventOp, "%s", "Operational mode manual");
    }

    sprintf(expEvent, "Initialized.  Version %d.%d.%d.%d",
            LD_MAJOR_VERSION, LD_MINOR_VERSION,
            LD_REVISION, LD_MISSION_REV);

    /* Verify results */
    UtAssert_EventSent(LD_STARTUP_MODE_EID, CFE_EVS_INFORMATION,
                       expEventOp,
                       "extern LD_AppMain, Nominal: OpMode Event Sent");

    UtAssert_EventSent(LD_INIT_INF_EID, CFE_EVS_INFORMATION, expEvent,
                  "extern LD_AppMain, Nominal: Initialized Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "extern LD_AppMain, Nominal: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
                  "extern LD_AppMain, Nominal: Event QueueDepth");
}

/**************************************************************************
 * Tests for LD AppMain()
 **************************************************************************/
/**
 * Test LD AppMain(), Fail RegisterApp
 */
void Test_LD_AppMain_Fail_RegisterApp(void)
{
    LD oLDut;

    int32 expected = CFE_ES_ERR_APP_REGISTER;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expSysLog, "LD - Failed to register the app (0x%08lX)\n",
            expected);

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog) == TRUE,
                  "AppMain, Fail RegisterApp: SysLog Written");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "AppMain, Fail RegisterApp: SysLog QueueDepth");
}


/**
 * Test LD AppMain(), WriteToSysLogHook
 */
int32 Test_LD_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buff[CFE_ES_MAX_SYSLOG_MSG_SIZE];
    char      expSysLog1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char      expSysLog2[CFE_EVS_MAX_MESSAGE_LENGTH];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buff, (size_t)CFE_ES_MAX_SYSLOG_MSG_SIZE, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s", Buff);

    snprintf(expSysLog1, CFE_EVS_MAX_MESSAGE_LENGTH,
             "LD - Failed to register with EVS (0x%08lX)\n",
             CFE_EVS_APP_NOT_REGISTERED);

    snprintf(expSysLog2, CFE_EVS_MAX_MESSAGE_LENGTH,
             "LD - Failed to init events (0x%08lX)\n",
             CFE_EVS_APP_NOT_REGISTERED);

    if (strncmp(Buff, expSysLog1, CFE_ES_MAX_SYSLOG_MSG_SIZE) == 0)
    {
        WriteToSysLog_HookFlag |= 0x00000001;
    }

    if (strncmp(Buff, expSysLog2, CFE_ES_MAX_SYSLOG_MSG_SIZE) == 0)
    {
        WriteToSysLog_HookFlag |= 0x00000002;
    }

    if (strncmp(Buff, "LD - Application failed to initialize\n",
                CFE_ES_MAX_SYSLOG_MSG_SIZE) == 0)
    {
        WriteToSysLog_HookFlag |= 0x00000004;
    }

    return CFE_SUCCESS;
}


/**
 * Test LD AppMain(), Fail InitApp
 */
void Test_LD_AppMain_Fail_InitApp(void)
{
    LD oLDut;

    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    WriteToSysLog_HookFlag = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
                              (void*)&Test_LD_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(WriteToSysLog_HookCalledCnt == 3,
                  "AppMain, Fail_InitApp: WriteToSysLog_HookCalledCnt");

    UtAssert_True(WriteToSysLog_HookFlag == 0x00000007,
                  "AppMain, Fail_InitApp: WriteToSysLog_HookFlag");
}


/**
 * Test LD AppMain(), Fail AcquireConfigPtrs
 */
void Test_LD_AppMain_Fail_AcquireConfigPtrs(void)
{
    LD oLDut;

    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expectedEvent,
            "Failed to get Config table's address (0x%08lX)",
            CFE_TBL_ERR_INVALID_HANDLE);

    /* Verify results */
    UtAssert_EventSent(LD_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent,
                       "AppMain, Fail AcquireConfigPtrs: Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "AppMain, Fail AcquireConfigPtrs: Event QueueDepth");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "AppMain, Fail AcquireConfigPtrs: SysLog QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsgHook
 */
int32 Test_LD_AppMain_RcvDataPipeMsgHook
                    (void *dst, void *src, uint32 size)
{
    unsigned char   *pBuff = NULL;
    int             i = 0;
    CFE_SB_Msg_t*   dataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    pBuff = (unsigned char*)src;
    printf("###RcvDataPipeMsgHook: ");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff ++;
    }
    printf("\n");

    dataMsgPtr = (CFE_SB_Msg_t*)src;
    DataMsgId = CFE_SB_GetMsgId(dataMsgPtr);
    switch(DataMsgId)
    {
        case PX4_ACTUATOR_ARMED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_ACTUATOR_ARMED_MID;
            break;
        case PX4_AIRSPEED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_AIRSPEED_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_0_MID:
            RcvDataPipeMsgHook_MsgId = PX4_ACTUATOR_CONTROLS_0_MID;
            break;
        case PX4_CONTROL_STATE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_CONTROL_STATE_MID;
            break;
        case PX4_BATTERY_STATUS_MID:
            RcvDataPipeMsgHook_MsgId = PX4_BATTERY_STATUS_MID;
            break;
        case PX4_VEHICLE_ATTITUDE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_ATTITUDE_MID;
            break;
        case PX4_MANUAL_CONTROL_SETPOINT_MID:
            RcvDataPipeMsgHook_MsgId = PX4_MANUAL_CONTROL_SETPOINT_MID;
            break;
        case PX4_VEHICLE_LOCAL_POSITION_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_LOCAL_POSITION_MID;
            break;
        case PX4_SENSOR_COMBINED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_SENSOR_COMBINED_MID;
            break;
        case PX4_VEHICLE_CONTROL_MODE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_CONTROL_MODE_MID;
            break;
        default:
            RcvDataPipeMsgHook_MsgId = 0;
            break;
    }

    return CFE_SUCCESS;
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - InvalidMsgID
 */
void Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID(void)
{
    LD oLDut;

    int32          DataPipe;
    LD_NoArgCmd_t  InMsg;
    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, 0, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expEvent, "Recvd invalid Data msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(LD_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppMain, RcvDataPipeMsg - InvalidMsgID");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
             "AppMain, RcvDataPipeMsg - InvalidMsgID: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - DataPipeError
 */
void Test_LD_AppMain_RcvDataPipeMsg_DataPipeError(void)
{
    LD oLDut;

    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expEvent, "Data pipe read error (0x%08lX).", CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppMain, RcvDataPipeMsg - DataPipeError");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                "AppMain, RcvDataPipeMsg - DataPipeError: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ActuatorArmed
 */
void Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed(void)
{
    LD oLDut;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_ACTUATOR_ARMED_MID,
             "AppMain, RcvDataPipeMsg - ActuatorArmed: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
             "AppMain, RcvDataPipeMsg - ActuatorArmed: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - Airspeed
 */
void Test_LD_AppMain_RcvDataPipeMsg_Airspeed(void)
{
    LD oLDut;

    int32              DataPipe;
    PX4_AirspeedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID,
                     sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_AIRSPEED_MID,
                  "AppMain, RcvDataPipeMsg - Airspeed: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
                  "AppMain, RcvDataPipeMsg - Airspeed: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ActuatorControls0
 */
void Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0(void)
{
    LD oLDut;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_0_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_ACTUATOR_CONTROLS_0_MID,
             "AppMain, RcvDataPipeMsg - ActuatorControls0: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
            "AppMain, RcvDataPipeMsg - ActuatorControls0: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ControlState
 */
void Test_LD_AppMain_RcvDataPipeMsg_ControlState(void)
{
    LD oLDut;

    int32                  DataPipe;
    PX4_ControlStateMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_CONTROL_STATE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_CONTROL_STATE_MID,
                  "AppMain, RcvDataPipeMsg - ControlState: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
                 "AppMain, RcvDataPipeMsg - ControlState: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - BatteryStatus
 */
void Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus(void)
{
    LD oLDut;

    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_BATTERY_STATUS_MID,
                  "AppMain, RcvDataPipeMsg - BatteryStatus: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
                "AppMain, RcvDataPipeMsg - BatteryStatus: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleAttitude
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude(void)
{
    LD oLDut;

    int32                     DataPipe;
    PX4_VehicleAttitudeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_ATTITUDE_MID,
             "AppMain, RcvDataPipeMsg - VehicleAttitude: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
             "AppMain, RcvDataPipeMsg - VehicleAttitude: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ManualControlSetpoint
 */
void Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint(void)
{
    LD oLDut;

    int32                           DataPipe;
    PX4_ManualControlSetpointMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_MANUAL_CONTROL_SETPOINT_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_MANUAL_CONTROL_SETPOINT_MID,
           "AppMain, RcvDataPipeMsg - ManualControlSetpoint: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
         "AppMain, RcvDataPipeMsg - ManualControlSetpoint: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleLocalPosition
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition(void)
{
    LD oLDut;

    int32                          DataPipe;
    PX4_VehicleLocalPositionMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LOCAL_POSITION_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_LOCAL_POSITION_MID,
           "AppMain, RcvDataPipeMsg - VehicleLocalPosition: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
         "AppMain, RcvDataPipeMsg - VehicleLocalPosition: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleSensorCombined
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined(void)
{
    LD oLDut;

    int32 DataPipe;
    PX4_SensorCombinedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_COMBINED_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_SENSOR_COMBINED_MID,
        "AppMain, RcvDataPipeMsg - VehicleSensorCombined: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
        "AppMain, RcvDataPipeMsg - VehicleSensorCombined: Event QueueDepth");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleControlMode
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode(void)
{
    LD oLDut;

    int32                        DataPipe;
    PX4_VehicleControlModeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                               (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_CONTROL_MODE_MID,
           "AppMain, RcvDataPipeMsg - VehicleControlMode: Received MsgId");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,
           "AppMain, RcvDataPipeMsg - VehicleControlMode: Event QueueDepth");
}


/**
 * Test LD AppMain(), Invalid Schedule Message
 */
void Test_LD_AppMain_InvalidSchMessage(void)
{
    LD oLDut;

    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving an
       invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(LD_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppMain, Invalid Schedule Message: Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "AppMain, Invalid Schedule Message: Event QueueDepth");
}


/**
 * Test LD AppMain(), SchPipeError
 */
void Test_LD_AppMain_SchPipeError(void)
{
    LD oLDut;

    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLDut.AppMain();

    sprintf(expEvent, "SCH pipe read error (0x%08lX).", CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppMain, SchPipeError: Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "AppMain, SchPipeError: Event QueueDepth");
}


/**
 * Hook to support: LD AppMain(), Nominal_SendHK - SendMsg
 */
int32 Test_LD_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    LD_HkTlm_t         HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);    /* DataLenth + 7 */
    printf("###AppMain_SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) +
             ((MsgPtr->Hdr).StreamId[1]));
    switch (MsgId)
    {
        case LD_HK_TLM_MID:
        {
            SendHKSendMsgHook_MsgId = LD_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr,
                           sizeof(HkMsg));

            printf("Sent LD_HK_TLM_MID:\n");
            printf("CmdCnt: %d\n", HkMsg.usCmdCnt);
            printf("CmdErrCnt: %d\n", HkMsg.usCmdErrCnt);
            printf("state: %d\n", HkMsg.state);
            printf("velocity_xy_filtered_f: %f\n",
                                     HkMsg.velocity_xy_filtered_f);
            printf("velocity_xy_unfiltered_f: %f\n",
                                   HkMsg.velocity_xy_unfiltered_f);
            printf("velocity_z_filtered_f: %f\n",
                                      HkMsg.velocity_z_filtered_f);
            printf("velocity_z_unfiltered_f: %f\n",
                                    HkMsg.velocity_z_unfiltered_f);
            printf("airspeed_filtered_f: %f\n",
                                        HkMsg.airspeed_filtered_f);
            printf("airspeed_unfiltered_f: %f\n",
                                      HkMsg.airspeed_unfiltered_f);
            printf("accel_xy_filtered_f: %f\n",
                                        HkMsg.accel_xy_filtered_f);
            printf("accel_xy_unfiltered_f: %f\n",
                                      HkMsg.accel_xy_unfiltered_f);
            printf("static_xy_b: %d\n", HkMsg.static_xy_b);
            printf("static_z_b: %d\n", HkMsg.static_z_b);
            printf("static_air_b: %d\n", HkMsg.static_air_b);
            printf("landDetected_b: %d\n", HkMsg.landDetected_b);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}

/**
 * Test LD AppMain(), Nominal - SendHK
 */
void Test_LD_AppMain_Nominal_SendHK(void)
{
    LD oLDut;

    int32            SchPipe;
    LD_NoArgCmd_t    InMsg;

    /* LD_SEND_HK_MID */
    SchPipe = Ut_CFE_SB_CreatePipe(LD_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, LD_SEND_HK_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of receiving
       LD_WAKEUP_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, LD_WAKEUP_MID, 1);

    /* Used to verify HK was transmitted correctly. */
    SendHKSendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                 (void*)&Test_LD_AppMain_Nominal_SendHK_SendMsgHook);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* Execute the function being tested */
    oLDut.InitApp();

    /* Fix LocalPosition data */
    oLDut.CVT.VehicleLocalPositionMsg.Timestamp = LD_Test_GetTimeUs();
    oLDut.CVT.VehicleLocalPositionMsg.RefTimestamp = LD_Test_GetTimeUs();
    oLDut.CVT.VehicleLocalPositionMsg.RefLat = 47.397741928975;
    oLDut.CVT.VehicleLocalPositionMsg.RefLon = 8.545593979817;
    oLDut.CVT.VehicleLocalPositionMsg.SurfaceBottomTimestamp =
                                             LD_Test_GetTimeUs();
    oLDut.CVT.VehicleLocalPositionMsg.X = -1.995731f;
    oLDut.CVT.VehicleLocalPositionMsg.Y = 1.565559f;
    oLDut.CVT.VehicleLocalPositionMsg.Z = -0.826584f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_XY[0] = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_XY[1] = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_Z = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.VX = -0.027511f;
    oLDut.CVT.VehicleLocalPositionMsg.VY = 0.006788f;
    oLDut.CVT.VehicleLocalPositionMsg.VZ = -0.051438f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_VXY[0] = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_VXY[1] = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.Delta_VZ = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.AX = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.AY = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.AZ = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.Yaw = 1.547718f;
    oLDut.CVT.VehicleLocalPositionMsg.RefAlt = 490.7512f;
    oLDut.CVT.VehicleLocalPositionMsg.DistBottom = 1.155246f;
    oLDut.CVT.VehicleLocalPositionMsg.DistBottomRate = 0.051438f;
    oLDut.CVT.VehicleLocalPositionMsg.EpH = 0.369742f;
    oLDut.CVT.VehicleLocalPositionMsg.EpV = 0.216528f;
    oLDut.CVT.VehicleLocalPositionMsg.EvH = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.EvV = 0.0f;
    oLDut.CVT.VehicleLocalPositionMsg.EstimatorType = 0;
    oLDut.CVT.VehicleLocalPositionMsg.XY_Valid = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.Z_Valid = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.V_XY_Valid = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.V_Z_Valid = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oLDut.CVT.VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oLDut.CVT.VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oLDut.CVT.VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oLDut.CVT.VehicleLocalPositionMsg.XY_Global = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.Z_Global = TRUE;
    oLDut.CVT.VehicleLocalPositionMsg.DistBottomValid = TRUE;

    /* Fix Airspeed data */
    oLDut.CVT.AirspeedMsg.Timestamp = LD_Test_GetTimeUs();
    oLDut.CVT.AirspeedMsg.IndicatedAirspeed = 1.0f;
    oLDut.CVT.AirspeedMsg.TrueAirspeed = 1.5f;
    oLDut.CVT.AirspeedMsg.TrueAirspeedUnfiltered = 0.0f;
    oLDut.CVT.AirspeedMsg.AirTemperature = 0.0f;
    oLDut.CVT.AirspeedMsg.Confidence = 0.0f;

    oLDut.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);
    oLDut.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);

    /* Verify results */
    UtAssert_True (SendHKSendMsgHook_MsgId == LD_HK_TLM_MID,
                   "AppMain_Nominal_SendHK");

}


/**
 * Hook to support: LD AppMain(), Nominal_Wakeup SendMsg
 */
int32 Test_LD_AppMain_Nominal_Wakeup_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###Wakeup_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_VEHICLE_LAND_DETECTED_MID:
        {
            PX4_VehicleLandDetectedMsg_t   VehicleLandDetected;
            CFE_PSP_MemCpy((void*)&VehicleLandDetected,
                         (void*)MsgPtr, sizeof(VehicleLandDetected));

            printf("Sent PX4_VEHICLE_LAND_DETECTED_MID:\n");
            localTime = LD_Test_GetTimeFromTimestamp(
                                      VehicleLandDetected.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("AltMax: %f\n", VehicleLandDetected.AltMax);
            printf("Landed: %d\n", VehicleLandDetected.Landed);
            printf("Freefall: %d\n", VehicleLandDetected.Freefall);
            printf("GroundContact: %d\n",
                    VehicleLandDetected.GroundContact);
            break;
        }
        case LD_DIAG_TLM_MID:
        {
            LD_Diag_t   Diag;
            CFE_PSP_MemCpy((void*)&Diag, (void*)MsgPtr,
                           sizeof(Diag));

            printf("Sent LD_DIAG_TLM_MID:\n");
            printf("GC_MinThrust: %d\n", Diag.GC_MinThrust);
            printf("GC_AltitudeLock: %d\n", Diag.GC_AltitudeLock);
            printf("GC_PositionLock: %d\n", Diag.GC_PositionLock);
            printf("GC_InDescent: %d\n", Diag.GC_InDescent);
            printf("GC_HitGround: %d\n", Diag.GC_HitGround);
            printf("GC_HorMovement: %d\n", Diag.GC_HorMovement);
            printf("GC_VertMovement: %d\n", Diag.GC_VertMovement);
            printf("GC_ManualControlIdlingOrAutomatic: %d\n",
                     Diag.GC_ManualControlIdlingOrAutomatic);
            printf("GroundContact: %d\n", Diag.GroundContact);
            printf("LD_GC_history_state: %d\n",
                    Diag.LD_GC_history_state);
            printf("LD_MinThrust: %d\n", Diag.LD_MinThrust);
            printf("LD_Rotation: %d\n", Diag.LD_Rotation);
            printf("LD_HorMovement: %d\n", Diag.LD_HorMovement);
            printf("LD_PositionLock: %d\n", Diag.LD_PositionLock);
            printf("Landed: %d\n", Diag.Landed);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}
/**
 * Test LD AppMain(), Nominal - Wakeup
 */
void Test_LD_AppMain_Nominal_Wakeup(void)
{
    LD oLDut;

    /* The following will emulate behavior of receiving a
       SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 3);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, LD_WAKEUP_MID, 1);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void*)&Test_LD_AppMain_Nominal_Wakeup_SendMsgHook);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 12);

    /* Execute the function being tested */
    oLDut.AppMain();

    /* Verify results */
}


/**************************************************************************
 * Tests for LD Execute()
 **************************************************************************/
/**
 * Test LD Execute, SendMsgtHook
 */
int32 Test_LD_Execute_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###Execute_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_VEHICLE_LAND_DETECTED_MID:
        {
            PX4_VehicleLandDetectedMsg_t *pVldm;

            pVldm = &HookVLndDetect[Execute_VLandDetRcvdCnt];
            CFE_PSP_MemCpy((void*)pVldm, (void*)MsgPtr,
                           sizeof(PX4_VehicleLandDetectedMsg_t));

            printf("Sent PX4_VEHICLE_LAND_DETECTED_MID:\n");
            localTime = LD_Test_GetTimeFromTimestamp(
                                           pVldm->Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("AltMax: %f\n", pVldm->AltMax);
            printf("Landed: %d\n", pVldm->Landed);
            printf("Freefall: %d\n", pVldm->Freefall);
            printf("GroundContact: %d\n", pVldm->GroundContact);

            Execute_VLandDetRcvdCnt ++;
            break;
        }
        case LD_DIAG_TLM_MID:
        {
            Execute_DiagTlmRcvdCnt ++;
            CFE_PSP_MemCpy((void*)&HookDiag, (void*)MsgPtr,
                           sizeof(HookDiag));

            printf("Sent LD_DIAG_TLM_MID:\n");
            printf("GC_MinThrust: %d\n", HookDiag.GC_MinThrust);
            printf("GC_AltitudeLock: %d\n", HookDiag.GC_AltitudeLock);
            printf("GC_PositionLock: %d\n", HookDiag.GC_PositionLock);
            printf("GC_InDescent: %d\n", HookDiag.GC_InDescent);
            printf("GC_HitGround: %d\n", HookDiag.GC_HitGround);
            printf("GC_HorMovement: %d\n", HookDiag.GC_HorMovement);
            printf("GC_VertMovement: %d\n", HookDiag.GC_VertMovement);
            printf("GC_ManualControlIdlingOrAutomatic: %d\n",
                     HookDiag.GC_ManualControlIdlingOrAutomatic);
            printf("GroundContact: %d\n", HookDiag.GroundContact);
            printf("LD_GC_history_state: %d\n",
                    HookDiag.LD_GC_history_state);
            printf("LD_MinThrust: %d\n", HookDiag.LD_MinThrust);
            printf("LD_Rotation: %d\n", HookDiag.LD_Rotation);
            printf("LD_HorMovement: %d\n", HookDiag.LD_HorMovement);
            printf("LD_PositionLock: %d\n", HookDiag.LD_PositionLock);
            printf("Landed: %d\n", HookDiag.Landed);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test LD Execute(), AutoLanded
 */
void Test_LD_Execute_AutoLanded(void)
{
#if 0
    LD oLDut;
#endif

    int32                         SchPipe;
    int32                         DataPipe;
    float                         AltMax;
    LD_NoArgCmd_t                 WakeupMsg;
    PX4_ActuatorArmedMsg_t        ActArmedMsg;
    PX4_VehicleLocalPositionMsg_t VLocalPosMsg;
    PX4_AirspeedMsg_t             AirSpeedMsg;
    PX4_BatteryStatusMsg_t        BatStatMsg;
    PX4_VehicleLandDetectedMsg_t  *pVldm;
    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    SchPipe = Ut_CFE_SB_CreatePipe(LD_SCH_PIPE_NAME);
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);

    /* Send ActuatorArmedMsg */
    CFE_SB_InitMsg((void*)&ActArmedMsg, PX4_ACTUATOR_ARMED_MID,
                    sizeof(ActArmedMsg), TRUE);
    GetActuatorArmedMsg(&ActArmedMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&ActArmedMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send VehicleLocalPositionMsg */
    CFE_SB_InitMsg((void*)&VLocalPosMsg, PX4_VEHICLE_LOCAL_POSITION_MID,
                    sizeof(VLocalPosMsg), TRUE);
    GetVehicleLocalPositionMsg(&VLocalPosMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLocalPosMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send AirspeedMsg */
    CFE_SB_InitMsg((void*)&AirSpeedMsg, PX4_AIRSPEED_MID,
                   sizeof(AirSpeedMsg), TRUE);
    GetAirspeedMsg(&AirSpeedMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&AirSpeedMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send BatteryStatusMsg */
    CFE_SB_InitMsg((void*)&BatStatMsg, PX4_BATTERY_STATUS_MID,
                   sizeof(BatStatMsg), TRUE);
    GetBatteryStatusMsg(&BatStatMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStatMsg, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg((void*)&WakeupMsg, LD_WAKEUP_MID,
                   sizeof(WakeupMsg), TRUE);
    /* Wakeup #1 with No Data */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #2 after receiving ActuatorArmedMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #3 after receiving VehicleLocalPositionMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #4 after receiving AirspeedMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #5 after receiving BatteryStatusMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);

    Execute_VLandDetRcvdCnt = 0;
    Execute_DiagTlmRcvdCnt = 0;
    memset(&HookVLndDetect[0], 0x00, sizeof(HookVLndDetect));
    memset(&HookDiag, 0x00, sizeof(HookDiag));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_LD_Execute_SendMsgHook);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 10);

    /* Execute the function being tested */
printf("###AutoFlying: Initial previous.AltMax(%f)\n", oLD.PreviousLandDetectedMsg.AltMax);
printf("###AutoFlying: Landed(%u), Freefall(%u), GroundContact(%u)\n",
        oLD.PreviousLandDetectedMsg.Landed, oLD.PreviousLandDetectedMsg.Freefall, oLD.PreviousLandDetectedMsg.GroundContact);
    oLD.InitApp();

    oLD.uiRunStatus = CFE_ES_APP_RUN;
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;

    while (CFE_ES_RunLoop(&oLD.uiRunStatus) == TRUE)
    {
        oLD.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);
        oLD.RcvDataPipeMsg();
    }

    if (BatStatMsg.Warning == PX4_BATTERY_WARNING_LOW)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_75_PERCENT;
    }
    else if (BatStatMsg.Warning == PX4_BATTERY_WARNING_CRITICAL)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_50_PERCENT;
    }
    else if (BatStatMsg.Warning == PX4_BATTERY_WARNING_EMERGENCY)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_25_PERCENT;
    }
    else
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX;
    }

    sprintf(expEvent, "%s", "Land detected");

    pVldm = &HookVLndDetect[Execute_VLandDetRcvdCnt - 1];

    /* Verify results */
    UtAssert_DoubleCmpAbs(pVldm->AltMax, AltMax, FLT_EPSILON,
                          "Execute, AutoLanded: AltitudeMax");

    UtAssert_True((pVldm->Landed == TRUE) &&
                  (pVldm->Freefall == FALSE) &&
                  (pVldm->GroundContact == FALSE),
                  "Execute, AutoLanded: VehicleLandDetected Status");

    UtAssert_EventSent(LD_LAND_DETECTED_EID, CFE_EVS_INFORMATION, expEvent,
                       "Execute, AutoLanded: Event Sent");
}


/**
 * Test LD Execute(), AutoFlying
 */
void Test_LD_Execute_AutoFlying(void)
{
#if 0
    LD oLDut;
#endif

    int32                         SchPipe;
    int32                         DataPipe;
    float                         AltMax;
    LD_NoArgCmd_t                 WakeupMsg;
    PX4_ActuatorArmedMsg_t        ActArmedMsg;
    PX4_VehicleLocalPositionMsg_t VLocalPosMsg;
    PX4_AirspeedMsg_t             AirSpeedMsg;
    PX4_BatteryStatusMsg_t        BatStatMsg;
    PX4_VehicleLandDetectedMsg_t  *pVldm;
    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    SchPipe = Ut_CFE_SB_CreatePipe(LD_SCH_PIPE_NAME);
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);

    /* Send ActuatorArmedMsg */
    CFE_SB_InitMsg((void*)&ActArmedMsg, PX4_ACTUATOR_ARMED_MID,
                    sizeof(ActArmedMsg), TRUE);
    GetActuatorArmedMsg(&ActArmedMsg);
    ActArmedMsg.Armed = TRUE;
    ActArmedMsg.ReadyToArm = FALSE;
    Ut_CFE_SB_AddMsgToPipe((void*)&ActArmedMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send VehicleLocalPositionMsg */
    CFE_SB_InitMsg((void*)&VLocalPosMsg, PX4_VEHICLE_LOCAL_POSITION_MID,
                    sizeof(VLocalPosMsg), TRUE);
    GetVehicleLocalPositionMsg(&VLocalPosMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLocalPosMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send AirspeedMsg */
    CFE_SB_InitMsg((void*)&AirSpeedMsg, PX4_AIRSPEED_MID,
                   sizeof(AirSpeedMsg), TRUE);
    GetAirspeedMsg(&AirSpeedMsg);
    AirSpeedMsg.TrueAirspeedUnfiltered = 100.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&AirSpeedMsg, (CFE_SB_PipeId_t)DataPipe);

    /* Send BatteryStatusMsg */
    CFE_SB_InitMsg((void*)&BatStatMsg, PX4_BATTERY_STATUS_MID,
                   sizeof(BatStatMsg), TRUE);
    GetBatteryStatusMsg(&BatStatMsg);
    BatStatMsg.Warning = PX4_BATTERY_WARNING_CRITICAL;
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStatMsg, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg((void*)&WakeupMsg, LD_WAKEUP_MID,
                   sizeof(WakeupMsg), TRUE);
    /* Wakeup #1 with No Data */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #2 after receiving ActuatorArmedMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #3 after receiving VehicleLocalPositionMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #4 after receiving AirspeedMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);
    /* Wakeup #5 after receiving BatteryStatusMsg */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupMsg, (CFE_SB_PipeId_t)SchPipe);

    Execute_VLandDetRcvdCnt = 0;
    Execute_DiagTlmRcvdCnt = 0;
    memset(&HookVLndDetect[0], 0x00, sizeof(HookVLndDetect));
    memset(&HookDiag, 0x00, sizeof(HookDiag));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_LD_Execute_SendMsgHook);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 10);

    /* Execute the function being tested */
#if 0
    oLDut.AppMain();
#else
printf("###AutoFlying: Initial previous.AltMax(%f)\n", oLD.PreviousLandDetectedMsg.AltMax);
printf("###AutoFlying: Landed(%u), Freefall(%u), GroundContact(%u)\n",
        oLD.PreviousLandDetectedMsg.Landed, oLD.PreviousLandDetectedMsg.Freefall, oLD.PreviousLandDetectedMsg.GroundContact);
    oLD.InitApp();

    oLD.uiRunStatus = CFE_ES_APP_RUN;
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;

    while (CFE_ES_RunLoop(&oLD.uiRunStatus) == TRUE)
    {
        oLD.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);
        oLD.RcvDataPipeMsg();
    }
#endif

    if (BatStatMsg.Warning == PX4_BATTERY_WARNING_LOW)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_75_PERCENT;
    }
    else if (BatStatMsg.Warning == PX4_BATTERY_WARNING_CRITICAL)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_50_PERCENT;
    }
    else if (BatStatMsg.Warning == PX4_BATTERY_WARNING_EMERGENCY)
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX * LD_25_PERCENT;
    }
    else
    {
        AltMax = oLD.ConfigTblPtr->LD_ALT_MAX;
    }

    sprintf(expEvent, "%s", "Vehicle in flight");

    pVldm = &HookVLndDetect[Execute_VLandDetRcvdCnt - 1];

    /* Verify results */
    UtAssert_DoubleCmpAbs(pVldm->AltMax, AltMax, FLT_EPSILON,
                          "Execute, AutoFlying: AltitudeMax");

    UtAssert_True((pVldm->Landed == FALSE) &&
                  (pVldm->Freefall == FALSE) &&
                  (pVldm->GroundContact == FALSE),
                  "Execute, AutoFlying: VehicleLandDetected Status");

    UtAssert_EventSent(LD_FLIGHT_DETECTED_EID, CFE_EVS_INFORMATION, expEvent,
                       "Execute, AutoFlying: Event Sent");
}


/**
 * Test LD Execute(), ManualFlying
 */
void Test_LD_Execute_ManualFlying(void)
{
}



/*********************************************************************
 * Tests for LD ConfigData()
 *********************************************************************/
/**
 * Test LD ConfigData()
 */
void Test_LD_ConfigData(void)
{
    LD oLDut;

    double expected_checksum = 8010021.73;

    /* Execute the function being tested */
    oLDut.InitApp();

    ConfigData_Checksum = 0.0;
    ConfigData_Checksum = LD_Test_GetConfigDataChecksum(
                                        oLDut.ConfigTblPtr);

    /* Verify results */
    UtAssert_True(
        fabs(ConfigData_Checksum - expected_checksum) <= FLT_EPSILON,
        "LD ConfigData");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void LD_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_LD_InitEvent_Fail_Register,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitEvent_Fail_Register");

    UtTest_Add(Test_LD_InitPipe_Fail_CreateSCHPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeWakeup,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeSendHK,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateCMDPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeCMD,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateDATAPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExActuatorArmed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExActuatorArmed");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExAirspeed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExAirspeed");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExActuatorControls0,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExActuatorControls0");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExControlState,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExControlState");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExBatteryStatus,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExBatteryStatus");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode");

    UtTest_Add(Test_LD_InitData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitData");

    UtTest_Add(Test_LD_InitApp_Fail_InitEvent,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitEvent");
    UtTest_Add(Test_LD_InitApp_Fail_InitPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitPipe");
    UtTest_Add(Test_LD_InitApp_Fail_InitData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitData");
    UtTest_Add(Test_LD_InitApp_Fail_InitConfigTbl,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_LD_InitApp_Nominal,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Nominal");

    UtTest_Add(Test_LD_LD_AppMain_Nominal,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_LD_AppMain_Nominal");

    UtTest_Add(Test_LD_AppMain_Fail_RegisterApp,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_LD_AppMain_Fail_InitApp,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_InitApp");
    UtTest_Add(Test_LD_AppMain_Fail_AcquireConfigPtrs,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_LD_AppMain_InvalidSchMessage,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_InvalidSchMessage");
    UtTest_Add(Test_LD_AppMain_SchPipeError,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_SchPipeError");
    UtTest_Add(Test_LD_AppMain_Nominal_SendHK,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_SendHK");
    UtTest_Add(Test_LD_AppMain_Nominal_Wakeup,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_DataPipeError,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_DataPipeError");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_Airspeed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_Airspeed");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ControlState,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ControlState");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode");

    UtTest_Add(Test_LD_Execute_AutoLanded,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_Execute_AutoLanded");
    UtTest_Add(Test_LD_Execute_AutoFlying,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_Execute_AutoFlying");

    UtTest_Add(Test_LD_ConfigData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ConfigData");
}
