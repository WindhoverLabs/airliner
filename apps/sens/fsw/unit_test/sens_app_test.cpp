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

#include "sens_app_test.hpp"
#include "sens_test_utils.hpp"

#include "cfe.h"
#include "sens_app.h"
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

#include <time.h>
#include <inttypes.h>

int32  hookCalledCount = 0;


/**
 * Test SENS GetPSPTimeHook
 */
void Test_SENS_GetPSPTimeHook(OS_time_t *LocalTime)
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
 * Tests for SENS_InitEvent()
 **************************************************************************/

/**
 * Test SENS_InitEvent() with failed CFE_EVS_Register
 */
void Test_SENS_InitEvent_Fail_Register(void)
{
    SENS oSENS;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for SENS_InitPipe()
 **************************************************************************/
/**
 * Test SENS_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_SENS_InitPipe_Fail_CreateSCHPipe(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_SENS_InitPipe_Fail_SubscribeWakeup(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_SENS_InitPipe_Fail_SubscribeSendHK(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for InputRcMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeInputRcMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for InputRcMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for DifferentialPressureMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeDifferentialPressureMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for DifferentialPressureMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for SensorBaroMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeSensorBaroMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for SensorBaroMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for SensorAccelMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeSensorAccelMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for SensorAccelMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for SensorMagMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeSensorMagMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for SensorMagMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for SensorGyroMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeSensorGyroMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for SensorGyroMsg");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_SubscribeEx for VehicleControlModeMsg
 */
void Test_SENS_InitPipe_Fail_SubscribeVehicleControlModeMsg(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for VehicleControlModeMsg");
}


/**
 * Test SENS_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_SENS_InitPipe_Fail_CreateCMDPipe(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test SENS_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_SENS_InitPipe_Fail_SubscribeCMD(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test SENS_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_SENS_InitPipe_Fail_CreateDATAPipe(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oSENS.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for SENS_InitData()
 **************************************************************************/
/**
 * Test SENS_InitData()
 */
void Test_SENS_InitData(void)
{
    SENS oSENS;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for SENS_InitApp()
 **************************************************************************/
/**
 * Test SENS_InitApp(), fail init event
 */
void Test_SENS_InitApp_Fail_InitEvent(void)
{
    SENS oSENS;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test SENS_InitApp(), fail init pipe
 */
void Test_SENS_InitApp_Fail_InitPipe(void)
{
    SENS oSENS;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test SENS_InitApp(), fail init data.
 * NOTE: no current way to fail SENS_InitData() in default
 */
void Test_SENS_InitApp_Fail_InitData(void)
{
    SENS oSENS;

    int32 result = 1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test SENS_InitApp(), fail init config table
 */
void Test_SENS_InitApp_Fail_InitConfigTbl(void)
{
    SENS oSENS;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test SENS_InitApp(), Nominal
 */
void Test_SENS_InitApp_Nominal(void)
{
    SENS oSENS;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for extern SENS_AppMain()
 **************************************************************************/
/**
 * Test SENS extern SENS_AppMain(), Nominal
 */
void Test_SENS_SENS_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    SENS_AppMain();
}


/**************************************************************************
 * Tests for SENS_AppMain()
 **************************************************************************/
/**
 * Test SENS_AppMain(), Fail RegisterApp
 */
void Test_SENS_AppMain_Fail_RegisterApp(void)
{
    SENS oSENS;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oSENS.AppMain();
}


/**
 * Test SENS_AppMain(), Fail InitApp
 */
void Test_SENS_AppMain_Fail_InitApp(void)
{
    SENS oSENS;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    oSENS.AppMain();
}


/**
 * Test SENS_AppMain(), Fail AcquireConfigPtrs
 */
void Test_SENS_AppMain_Fail_AcquireConfigPtrs(void)
{
    SENS oSENS;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();
}


/**
 * Test SENS_AppMain(), Invalid Schedule Message
 */
void Test_SENS_AppMain_InvalidSchMessage(void)
{
    SENS   oSENS;

    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, PX4_AIRSPEED_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "Recvd invalid SCH msgId (0x%04X)", PX4_AIRSPEED_MID);

    /* Verify results */
    UtAssert_EventSent(SENS_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "SENS_AppMain, Invalid Schedule Message");
}


/**
 * Test SENS_AppMain(), Sch Pipe Error
 */
void Test_SENS_AppMain_SchPipeError(void)
{
    SENS   oSENS;

    int32  expected = CFE_SB_PIPE_RD_ERR;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "SCH pipe read error (0x%08lX).", CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(SENS_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "SENS_AppMain, Sch Pipe Error");
}


/**
 * Test SENS_AppMain(), SchData_InputRcMsg
 */
void Test_SENS_AppMain_SchData_InputRcMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_InputRcMsg_t InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_INPUT_RC_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();
    InMsg.LastSignal = InMsg.Timestamp - 1000000;

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.InputRcMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.InputRcMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_DifferentialPressureMsg
 */
void Test_SENS_AppMain_SchData_DifferentialPressureMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_DifferentialPressureMsg_t InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_DIFFERENTIAL_PRESSURE_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.DifferentialPressureMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.DifferentialPressureMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_SensorBaroMsg
 */
void Test_SENS_AppMain_SchData_SensorBaroMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorBaroMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_BARO_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.SensorBaroMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.SensorBaroMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_SensorAccelMsg
 */
void Test_SENS_AppMain_SchData_SensorAccelMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorAccelMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_ACCEL_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.SensorAccelMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.SensorAccelMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_SensorMagMsg
 */
void Test_SENS_AppMain_SchData_SensorMagMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorMagMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_MAG_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.SensorMagMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.SensorMagMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_SensorGyroMsg
 */
void Test_SENS_AppMain_SchData_SensorGyroMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorGyroMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_GYRO_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.SensorGyroMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.SensorGyroMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), SchData_VehicleControlModeMsg
 */
void Test_SENS_AppMain_SchData_VehicleControlModeMsg(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorGyroMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.VehicleControlModeMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.VehicleControlModeMsg.Timestamp: %s", asctime(loc_time));
}


/**
 * Hook to support: SENS_AppMain(), Nominal - SendHK
 */
int32 Test_SENS_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test SENS_AppMain(), Nominal - SendHK
 */
void Test_SENS_AppMain_Nominal_SendHK(void)
{
    SENS oSENS;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_SENS_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oSENS.AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test SENS_AppMain(), Nominal - Wakeup
 */
void Test_SENS_AppMain_Nominal_Wakeup(void)
{
    SENS oSENS;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

}


/**************************************************************************
 * Tests for SENS CyclicProcessing
 **************************************************************************/
/**
 * Test SENS_AppMain(), ProcessRCInput
 */
void Test_SENS_AppMain_ProcessRCInput(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_InputRcMsg_t InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_INPUT_RC_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();
    InMsg.LastSignal = InMsg.Timestamp - 1000000;

    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.InputRcMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.InputRcMsg.Timestamp: %s", asctime(loc_time));
    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.InputRcMsg.LastSignal);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.InputRcMsg.LastSignal: %s", asctime(loc_time));
}


/**
 * Test SENS_AppMain(), CombineSensorInput
 */
void Test_SENS_AppMain_CombineSensorInput(void)
{
    SENS             oSENS;

    int32            SchPipe;
    time_t           localTime;
    struct tm        *loc_time;
    PX4_SensorGyroMsg_t  InMsg;
    PX4_SensorAccelMsg_t SensorAccelMsg;
    PX4_SensorMagMsg_t   SensorMsgMsg;
    PX4_SensorBaroMsg_t  SensorBaroMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&SensorAccelMsg, PX4_SENSOR_ACCEL_MID, sizeof(SensorAccelMsg), TRUE);
    SensorAccelMsg.Timestamp = SENS_Test_GetTimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&SensorAccelMsg, (CFE_SB_PipeId_t)SchPipe);

    CFE_SB_InitMsg ((void*)&SensorMsgMsg, PX4_SENSOR_MAG_MID, sizeof(SensorMsgMsg), TRUE);
    SensorMsgMsg.Timestamp = SENS_Test_GetTimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&SensorMsgMsg, (CFE_SB_PipeId_t)SchPipe);

    CFE_SB_InitMsg ((void*)&SensorBaroMsg, PX4_SENSOR_BARO_MID, sizeof(SensorBaroMsg), TRUE);
    SensorBaroMsg.Timestamp = SENS_Test_GetTimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&SensorBaroMsg, (CFE_SB_PipeId_t)SchPipe);

    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_GYRO_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = SENS_Test_GetTimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.InitApp();
    memset((void *)&oSENS.CVT, 0x00, sizeof(oSENS.CVT));
    oSENS.RcvSchPipeMsg(SENS_SCH_PIPE_PEND_TIME);
    oSENS.RcvSchPipeMsg(SENS_SCH_PIPE_PEND_TIME);
    oSENS.RcvSchPipeMsg(SENS_SCH_PIPE_PEND_TIME);
    oSENS.RcvSchPipeMsg(SENS_SCH_PIPE_PEND_TIME);

    localTime = SENS_Test_GetTimeFromTimestamp(oSENS.CVT.SensorGyroMsg.Timestamp);
    loc_time = localtime(&localTime);
    printf("Received oSENS.CVT.SensorGyroMsg.Timestamp: %s", asctime(loc_time));
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SENS_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_SENS_InitEvent_Fail_Register,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitEvent_Fail_Register");

    UtTest_Add(Test_SENS_InitPipe_Fail_CreateSCHPipe,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeWakeup,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeSendHK,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeInputRcMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeInputRcMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeDifferentialPressureMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeDifferentialPressureMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeSensorBaroMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeSensorBaroMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeSensorAccelMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeSensorAccelMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeSensorMagMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeSensorMagMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeSensorGyroMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeSensorGyroMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeVehicleControlModeMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeVehicleControlModeMsg");
    UtTest_Add(Test_SENS_InitPipe_Fail_CreateCMDPipe,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_SENS_InitPipe_Fail_SubscribeCMD,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_SENS_InitPipe_Fail_CreateDATAPipe,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_SENS_InitData,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitData");

    UtTest_Add(Test_SENS_InitApp_Fail_InitEvent,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitApp_Fail_InitEvent");
    UtTest_Add(Test_SENS_InitApp_Fail_InitPipe,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitApp_Fail_InitPipe");
    UtTest_Add(Test_SENS_InitApp_Fail_InitData,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitApp_Fail_InitData");
    UtTest_Add(Test_SENS_InitApp_Fail_InitConfigTbl,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_SENS_InitApp_Nominal,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitApp_Nominal");

    UtTest_Add(Test_SENS_SENS_AppMain_Nominal,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_SENS_AppMain_Nominal");

    UtTest_Add(Test_SENS_AppMain_Fail_RegisterApp,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_SENS_AppMain_Fail_InitApp,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_Fail_InitApp");
    UtTest_Add(Test_SENS_AppMain_Fail_AcquireConfigPtrs,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_SENS_AppMain_InvalidSchMessage,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_InvalidSchMessage");
#if 1  // ticket #238
    UtTest_Add(Test_SENS_AppMain_SchPipeError,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchPipeError");
#endif
    UtTest_Add(Test_SENS_AppMain_SchData_InputRcMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_InputRcMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_DifferentialPressureMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_DifferentialPressureMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_SensorBaroMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_SensorBaroMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_SensorAccelMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_SensorAccelMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_SensorMagMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_SensorMagMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_SensorGyroMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_SensorGyroMsg");
    UtTest_Add(Test_SENS_AppMain_SchData_VehicleControlModeMsg,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_SchData_VehicleControlModeMsg");
    UtTest_Add(Test_SENS_AppMain_Nominal_SendHK,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_Nominal_SendHK");
    UtTest_Add(Test_SENS_AppMain_Nominal_Wakeup,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_SENS_AppMain_ProcessRCInput,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_ProcessRCInput");
    UtTest_Add(Test_SENS_AppMain_CombineSensorInput,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_AppMain_CombineSensorInput");
}
