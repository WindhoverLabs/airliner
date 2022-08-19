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

#include "fpc_app_test.h"
#include "fpc_app.h"
#include "fpc_test_utils.h"

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

#include <time.h>
#include <inttypes.h>
#include <float.h>

uint16   ProcessNewDataHook_MsgId = 0;
int32    WriteToSysLog_HookCalledCnt = 0;
int32    SendEvent_HookCalledCnt = 0;
int32    SendHk_HookCalledCnt = 0;
//uint32   UpdateParams_ValidateStatus = 0x0;

double   UpdateParams_ParamChecksum = 0.0;


/**************************************************************************
 * Tests for FPC InitEvent()
 **************************************************************************/

/**
 * Test FPC InitEvent() with failed CFE_EVS_Register
 */
void Test_FPC_InitEvent_Fail_Register(void)
{
    FPC   oFPC{};

    /* Set a fail result for EVS */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for FPC InitPipe()
 **************************************************************************/
/**
 * Test FPC InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_FPC_InitPipe_Fail_CreateSCHPipe(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_FPC_InitPipe_Fail_SubscribeWakeup(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_FPC_InitPipe_Fail_SubscribeSendHK(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test FPC InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_FPC_InitPipe_Fail_CreateCMDPipe(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test FPC InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FPC_InitPipe_Fail_SubscribeCMD(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test FPC InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_FPC_InitPipe_Fail_CreateDATAPipe(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for ManualControlSp
 */
void Test_FAC_InitPipe_Fail_SubscribeManualControlSp(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_MANUAL_CONTROL_SETPOINT_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleControlMode
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleControlMode(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_CONTROL_MODE_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleGlobalPosition
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleGlobalPosition(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_GLOBAL_POSITION_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleLocalPosition
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleLocalPosition(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_LOCAL_POSITION_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for PositionSetpointTriplet
 */
void Test_FAC_InitPipe_Fail_SubscribePositionSetpointTriplet(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_POSITION_SETPOINT_TRIPLET_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleStatus
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleStatus(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_STATUS_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleLandDetected
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleLandDetected(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_LAND_DETECTED_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for Airspeed
 */
void Test_FAC_InitPipe_Fail_SubscribeAirspeed(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_AIRSPEED_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for VehicleAttitude
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleAttitude(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 11);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_ATTITUDE_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for SensorCombined
 */
void Test_FAC_InitPipe_Fail_SubscribeSensorCombined(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 12);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_SENSOR_COMBINED_MID");
}

/**
 * Test FPC InitPipe(), fail CFE_SB_SubscribeEx for SensorBaro
 */
void Test_FAC_InitPipe_Fail_SubscribeSensorBaro(void)
{
    FPC   oFPC{};

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 13);

    /* Execute the function being tested */
    result = oFPC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_SENSOR_BARO_MID");
}


/**************************************************************************
 * Tests for FPC InitData()
 **************************************************************************/
/**
 * Test FPC InitData()
 */
void Test_FPC_InitDataNominal(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFPC.InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for FPC InitApp()
 **************************************************************************/
/**
 * Test FPC InitApp(), fail init event
 */
void Test_FPC_InitApp_Fail_InitEvent(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test FPC InitApp(), fail init pipe
 */
void Test_FPC_InitApp_Fail_InitPipe(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test FPC InitApp(), fail init data.
 * NOTE: no current way to fail FPC InitData() in default
 */
void Test_FPC_InitApp_Fail_InitData(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test FPC InitApp(), fail init config table
 */
void Test_FPC_InitApp_Fail_InitConfigTbl(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test FPC InitApp(), Nominal
 */
void Test_FPC_InitApp_Nominal(void)
{
    FPC   oFPC{};

    /* Set a nominal result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFPC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for FPC AppMain()
 **************************************************************************/
/**
 * Test FPC AppMain(), WriteToSysLogHook
 */
int32 Test_FPC_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buf[256];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s", Buf);

    return WriteToSysLog_HookCalledCnt;
}

/**
 * Test FPC AppMain(), SendEventHook
 */
int32 Test_FPC_AppMain_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];

    SendEvent_HookCalledCnt++;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###AppMain_SendEventHook:\n");
    printf("%s\n", Buf);

    return SendEvent_HookCalledCnt;
}

/**
 * Test FPC AppMain(), Fail RegisterApp
 */
void Test_FPC_AppMain_Fail_RegisterApp(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 expected = CFE_ES_ERR_APP_REGISTER;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif
}


/**
 * Test FPC AppMain(), Fail InitApp
 */
void Test_FPC_AppMain_Fail_InitApp(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_FPC_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True (WriteToSysLog_HookCalledCnt == 3, "AppMain, Fail_InitApp");
}


/**
 * Test FPC AppMain(), Fail AcquireConfigPtrs
 */
void Test_FPC_AppMain_Fail_AcquireConfigPtrs(void)
{
    FPC   oFPC{};

    /* Set a fail result */
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_FPC_AppMain_SendEventHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtAssert_True (((SendEvent_HookCalledCnt == 2) &&
                   (oFPC.ConfigTblPtr == 0)),
                   "AppMain, Fail_AcquireConfigPtrs");
#endif
}


/**
 * Test FPC AppMain(), Invalid Schedule Message
 */
void Test_FPC_AppMain_InvalidSchMessage(void)
{
    FPC   oFPC{};

    /* The following will emulate the behavior of receiving an invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, PX4_BATTERY_STATUS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif
}

/**
 * Test FPC GetPSPTimeHook
 */
void Test_FPC_GetPSPTimeHook(OS_time_t *LocalTime)
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

/**
 * Hook to support: FPC AppMain(), Nominal - SendHK_SendMsgHook
 */
int32 Test_FPC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    FPC_HkTlm_t        HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);                /* DataLenth + 7 */
    printf("###AppMain_SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = FPC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case FPC_HK_TLM_MID:
        {
            SendHk_HookCalledCnt++;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));

            printf("Sent FPC_HK_TLM_MID:\n");
            printf("CmdCnt: %d\n", HkMsg.usCmdCnt);
            printf("CmdErrCnt: %d\n", HkMsg.usCmdErrCnt);
            printf("ControlModeCurrent: %d\n", HkMsg.ControlModeCurrent);
            printf("_hold_alt: %f\n", HkMsg._hold_alt);
            printf("m_Hold_Alt: %f\n", HkMsg.m_Hold_Alt);
            printf("m_Hdg_Hold_Enabled: %d\n", HkMsg.m_Hdg_Hold_Enabled);
            printf("tecsMode: %d\n", HkMsg.tecsMode);
            printf("_time_started_landing: %" PRIu64"\n", HkMsg._time_started_landing);
            printf("use_tecs_pitch: %d\n", HkMsg.use_tecs_pitch);

            localTime = FPC_Test_GetTimeFromTimestamp(HkMsg._hdg_hold_prev_wp.Timestamp);
            loc_time = localtime(&localTime);
            printf("\n_hdg_hold_prev_wp.Timestamp: %s", asctime(loc_time));
            printf("_hdg_hold_prev_wp.Lat: %f\n", HkMsg._hdg_hold_prev_wp.Lat);
            printf("_hdg_hold_prev_wp.Lon: %f\n", HkMsg._hdg_hold_prev_wp.Lon);
            printf("_hdg_hold_prev_wp.X: %f\n", HkMsg._hdg_hold_prev_wp.X);
            printf("_hdg_hold_prev_wp.Y: %f\n", HkMsg._hdg_hold_prev_wp.Y);
            printf("_hdg_hold_prev_wp.Z: %f\n", HkMsg._hdg_hold_prev_wp.Z);
            printf("_hdg_hold_prev_wp.VX: %f\n", HkMsg._hdg_hold_prev_wp.VX);
            printf("_hdg_hold_prev_wp.VY: %f\n", HkMsg._hdg_hold_prev_wp.VY);
            printf("_hdg_hold_prev_wp.VZ: %f\n", HkMsg._hdg_hold_prev_wp.VZ);
            printf("_hdg_hold_prev_wp.Alt: %f\n", HkMsg._hdg_hold_prev_wp.Alt);
            printf("_hdg_hold_prev_wp.Yaw: %f\n", HkMsg._hdg_hold_prev_wp.Yaw);
            printf("_hdg_hold_prev_wp.Yawspeed: %f\n", HkMsg._hdg_hold_prev_wp.Yawspeed);
            printf("_hdg_hold_prev_wp.LoiterRadius: %f\n", HkMsg._hdg_hold_prev_wp.LoiterRadius);
            printf("_hdg_hold_prev_wp.PitchMin: %f\n", HkMsg._hdg_hold_prev_wp.PitchMin);
            printf("_hdg_hold_prev_wp.AX: %f\n", HkMsg._hdg_hold_prev_wp.AX);
            printf("_hdg_hold_prev_wp.AY: %f\n", HkMsg._hdg_hold_prev_wp.AY);
            printf("_hdg_hold_prev_wp.AZ: %f\n", HkMsg._hdg_hold_prev_wp.AZ);
            printf("_hdg_hold_prev_wp.AcceptanceRadius: %f\n",
                                      HkMsg._hdg_hold_prev_wp.AcceptanceRadius);
            printf("_hdg_hold_prev_wp.CruisingSpeed: %f\n", HkMsg._hdg_hold_prev_wp.CruisingSpeed);
            printf("_hdg_hold_prev_wp.CruisingThrottle: %f\n",
                                      HkMsg._hdg_hold_prev_wp.CruisingThrottle);
            printf("_hdg_hold_prev_wp.Valid: %d\n", HkMsg._hdg_hold_prev_wp.Valid);
            printf("_hdg_hold_prev_wp.Type: %d\n", HkMsg._hdg_hold_prev_wp.Type);
            printf("_hdg_hold_prev_wp.PositionValid: %d\n", HkMsg._hdg_hold_prev_wp.PositionValid);
            printf("_hdg_hold_prev_wp.VelocityValid: %d\n", HkMsg._hdg_hold_prev_wp.VelocityValid);
            printf("_hdg_hold_prev_wp.VelocityFrame: %d\n", HkMsg._hdg_hold_prev_wp.VelocityFrame);
            printf("_hdg_hold_prev_wp.AltValid: %d\n", HkMsg._hdg_hold_prev_wp.AltValid);
            printf("_hdg_hold_prev_wp.YawValid: %d\n", HkMsg._hdg_hold_prev_wp.YawValid);
            printf("_hdg_hold_prev_wp.DisableMcYawControl: %d\n",
                                      HkMsg._hdg_hold_prev_wp.DisableMcYawControl);
            printf("_hdg_hold_prev_wp.YawspeedValid: %d\n", HkMsg._hdg_hold_prev_wp.YawspeedValid);
            printf("_hdg_hold_prev_wp.LoiterDirection: %d\n",
                                      HkMsg._hdg_hold_prev_wp.LoiterDirection);
            printf("_hdg_hold_prev_wp.AccelerationValid: %d\n",
                                      HkMsg._hdg_hold_prev_wp.AccelerationValid);
            printf("_hdg_hold_prev_wp.AccelerationIsForce: %d\n",
                                      HkMsg._hdg_hold_prev_wp.AccelerationIsForce);

            localTime = FPC_Test_GetTimeFromTimestamp(HkMsg._hdg_hold_curr_wp.Timestamp);
            loc_time = localtime(&localTime);
            printf("\n_hdg_hold_curr_wp.Timestamp: %s", asctime(loc_time));
            printf("_hdg_hold_curr_wp.Lat: %f\n", HkMsg._hdg_hold_curr_wp.Lat);
            printf("_hdg_hold_curr_wp.Lon: %f\n", HkMsg._hdg_hold_curr_wp.Lon);
            printf("_hdg_hold_curr_wp.X: %f\n", HkMsg._hdg_hold_curr_wp.X);
            printf("_hdg_hold_curr_wp.Y: %f\n", HkMsg._hdg_hold_curr_wp.Y);
            printf("_hdg_hold_curr_wp.Z: %f\n", HkMsg._hdg_hold_curr_wp.Z);
            printf("_hdg_hold_curr_wp.VX: %f\n", HkMsg._hdg_hold_curr_wp.VX);
            printf("_hdg_hold_curr_wp.VY: %f\n", HkMsg._hdg_hold_curr_wp.VY);
            printf("_hdg_hold_curr_wp.VZ: %f\n", HkMsg._hdg_hold_curr_wp.VZ);
            printf("_hdg_hold_curr_wp.Alt: %f\n", HkMsg._hdg_hold_curr_wp.Alt);
            printf("_hdg_hold_curr_wp.Yaw: %f\n", HkMsg._hdg_hold_curr_wp.Yaw);
            printf("_hdg_hold_curr_wp.Yawspeed: %f\n", HkMsg._hdg_hold_curr_wp.Yawspeed);
            printf("_hdg_hold_curr_wp.LoiterRadius: %f\n", HkMsg._hdg_hold_curr_wp.LoiterRadius);
            printf("_hdg_hold_curr_wp.PitchMin: %f\n", HkMsg._hdg_hold_curr_wp.PitchMin);
            printf("_hdg_hold_curr_wp.AX: %f\n", HkMsg._hdg_hold_curr_wp.AX);
            printf("_hdg_hold_curr_wp.AY: %f\n", HkMsg._hdg_hold_curr_wp.AY);
            printf("_hdg_hold_curr_wp.AZ: %f\n", HkMsg._hdg_hold_curr_wp.AZ);
            printf("_hdg_hold_curr_wp.AcceptanceRadius: %f\n", HkMsg._hdg_hold_curr_wp.AcceptanceRadius);
            printf("_hdg_hold_curr_wp.CruisingSpeed: %f\n", HkMsg._hdg_hold_curr_wp.CruisingSpeed);
            printf("_hdg_hold_curr_wp.CruisingThrottle: %f\n", HkMsg._hdg_hold_curr_wp.CruisingThrottle);
            printf("_hdg_hold_curr_wp.Valid: %d\n", HkMsg._hdg_hold_curr_wp.Valid);
            printf("_hdg_hold_curr_wp.Type: %d\n", HkMsg._hdg_hold_curr_wp.Type);
            printf("_hdg_hold_curr_wp.PositionValid: %d\n", HkMsg._hdg_hold_curr_wp.PositionValid);
            printf("_hdg_hold_curr_wp.VelocityValid: %d\n", HkMsg._hdg_hold_curr_wp.VelocityValid);
            printf("_hdg_hold_curr_wp.VelocityFrame: %d\n", HkMsg._hdg_hold_curr_wp.VelocityFrame);
            printf("_hdg_hold_curr_wp.AltValid: %d\n", HkMsg._hdg_hold_curr_wp.AltValid);
            printf("_hdg_hold_curr_wp.YawValid: %d\n", HkMsg._hdg_hold_curr_wp.YawValid);
            printf("_hdg_hold_curr_wp.DisableMcYawControl: %d\n",
                                      HkMsg._hdg_hold_curr_wp.DisableMcYawControl);
            printf("_hdg_hold_curr_wp.YawspeedValid: %d\n", HkMsg._hdg_hold_curr_wp.YawspeedValid);
            printf("_hdg_hold_curr_wp.LoiterDirection: %d\n", HkMsg._hdg_hold_curr_wp.LoiterDirection);
            printf("_hdg_hold_curr_wp.AccelerationValid: %d\n",
                                      HkMsg._hdg_hold_curr_wp.AccelerationValid);
            printf("_hdg_hold_curr_wp.AccelerationIsForce: %d\n",
                                      HkMsg._hdg_hold_curr_wp.AccelerationIsForce);

            printf("\n_airspeed_valid: %d\n", HkMsg._airspeed_valid);
            printf("_airspeed_last_received: %" PRIu64"\n", HkMsg._airspeed_last_received);
            printf("_airspeed: %f\n", HkMsg._airspeed);
            printf("_eas2tas: %f\n", HkMsg._eas2tas);

            printf("\n_runway_takeoff._state: %d\n", HkMsg._runway_takeoff._state);
            printf("_runway_takeoff._initialized: %d\n", HkMsg._runway_takeoff._initialized);
            printf("_runway_takeoff._initialized_time: %" PRIu64"\n",
                                    HkMsg._runway_takeoff._initialized_time);
            printf("_runway_takeoff._init_yaw: %f\n", HkMsg._runway_takeoff._init_yaw);
            printf("_runway_takeoff._climbout: %d\n", HkMsg._runway_takeoff._climbout);
            printf("_runway_takeoff._throttle_ramp_time: %u\n",
                                    HkMsg._runway_takeoff._throttle_ramp_time);
            printf("_runway_takeoff._runway_takeoff_enabled: %d\n",
                                    HkMsg._runway_takeoff._runway_takeoff_enabled);
            printf("_runway_takeoff._heading_mode: %ld\n", HkMsg._runway_takeoff._heading_mode);
            printf("_runway_takeoff._nav_alt: %f\n", HkMsg._runway_takeoff._nav_alt);
            printf("_runway_takeoff._takeoff_throttle: %f\n",
                                    HkMsg._runway_takeoff._takeoff_throttle);
            printf("_runway_takeoff._runway_pitch_sp: %f\n",
                                    HkMsg._runway_takeoff._runway_pitch_sp);
            printf("_runway_takeoff._max_takeoff_pitch: %f\n",
                                    HkMsg._runway_takeoff._max_takeoff_pitch);
            printf("_runway_takeoff._max_takeoff_roll: %f\n",
                                    HkMsg._runway_takeoff._max_takeoff_roll);
            printf("_runway_takeoff._min_airspeed_scaling: %f\n",
                                    HkMsg._runway_takeoff._min_airspeed_scaling);
            printf("_runway_takeoff._airspeed_min: %f\n",
                                    HkMsg._runway_takeoff._airspeed_min);
            printf("_runway_takeoff._climbout_diff: %f\n",
                                    HkMsg._runway_takeoff._climbout_diff);
            break;
        }
        default:
        {
            printf("Sent Invalid Message\n");
            break;
        }
    }

    return (SendHk_HookCalledCnt);
}

/**
 * Test FPC AppMain(), Nominal - SendHK
 */
void Test_FPC_AppMain_Nominal_SendHK(void)
{
    FPC   oFPC{};

    /* The following will emulate the behavior of receiving FPC_SEND_HK_MID */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FPC_GetPSPTimeHook);
    SendHk_HookCalledCnt = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FPC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True (SendHk_HookCalledCnt == 1, "AppMain_Nominal_SendHK");
}

/**
 * Test FPC SendMsgtHook
 */
int32 Test_FPC_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);             /* DataLenth + 7 */
    printf("###FPC_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = FPC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_POSITION_CONTROL_STATUS_MID:
        {
            PX4_Position_Control_Status_t  PositionControlStatus;
            CFE_PSP_MemCpy((void*)&PositionControlStatus, (void*)MsgPtr,
                           sizeof(PositionControlStatus));

            printf("Sent PX4_POSITION_CONTROL_STATUS_MID:\n");
            localTime = FPC_Test_GetTimeFromTimestamp(PositionControlStatus.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("NAV_ROLL: %f\n", PositionControlStatus.NAV_ROLL);
            printf("NAC_PITCH: %f\n", PositionControlStatus.NAC_PITCH);
            printf("NAV_BEARING: %f\n", PositionControlStatus.NAV_BEARING);
            printf("TARGET_BEARING: %f\n", PositionControlStatus.TARGET_BEARING);
            printf("WP_DIST: %f\n", PositionControlStatus.WP_DIST);
            printf("XTRACK_ERRORP: %f\n", PositionControlStatus.XTRACK_ERRORP);
            printf("TURN_DISTANCE: %f\n", PositionControlStatus.TURN_DISTANCE);
            printf("LANDING_HORIZONTAL_SLOPE_DISPLACEMENT: %f\n",
                    PositionControlStatus.LANDING_HORIZONTAL_SLOPE_DISPLACEMENT);
            printf("LANDING_SLOPE_ANGLE_RAD: %f\n",
                    PositionControlStatus.LANDING_SLOPE_ANGLE_RAD);
            printf("LANDING_FLARE_LENGTH: %f\n",
                    PositionControlStatus.LANDING_FLARE_LENGTH);
            printf("ABORT_LANDING: %d\n", PositionControlStatus.ABORT_LANDING);
            break;
        }
        case PX4_VEHICLE_ATTITUDE_SETPOINT_MID:
        {
            PX4_VehicleAttitudeSetpointMsg_t  VehicleAttitudeSetpoint;
            CFE_PSP_MemCpy((void*)&VehicleAttitudeSetpoint, (void*)MsgPtr,
                           sizeof(VehicleAttitudeSetpoint));

            printf("Sent PX4_VEHICLE_ATTITUDE_SETPOINT_MID\n");
            localTime = FPC_Test_GetTimeFromTimestamp(VehicleAttitudeSetpoint.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("RollBody: %f\n", VehicleAttitudeSetpoint.RollBody);
            printf("PitchBody: %f\n", VehicleAttitudeSetpoint.PitchBody);
            printf("YawBody: %f\n", VehicleAttitudeSetpoint.YawBody);
            printf("YawSpMoveRate: %f\n", VehicleAttitudeSetpoint.YawSpMoveRate);
            printf("Q_D[0]: %f, Q_D[1]: %f, Q_D[2]: %f, Q_D[3]: %f\n",
                   VehicleAttitudeSetpoint.Q_D[0], VehicleAttitudeSetpoint.Q_D[1],
                   VehicleAttitudeSetpoint.Q_D[2], VehicleAttitudeSetpoint.Q_D[3]);
            printf("Q_D_Valid: %d\n", VehicleAttitudeSetpoint.Q_D_Valid);
            printf("Thrust: %f\n", VehicleAttitudeSetpoint.Thrust);
            printf("RollResetIntegral: %d\n", VehicleAttitudeSetpoint.RollResetIntegral);
            printf("PitchResetIntegral: %d\n", VehicleAttitudeSetpoint.PitchResetIntegral);
            printf("YawResetIntegral: %d\n", VehicleAttitudeSetpoint.YawResetIntegral);
            printf("FwControlYaw: %d\n", VehicleAttitudeSetpoint.FwControlYaw);
            printf("DisableMcYawControl: %d\n",
                   VehicleAttitudeSetpoint.DisableMcYawControl);
            printf("ApplyFlaps: %d\n", VehicleAttitudeSetpoint.ApplyFlaps);
            printf("LandingGear: %f\n", VehicleAttitudeSetpoint.LandingGear);
            break;
        }
        case PX4_TECS_STATUS_MID:
        {
            PX4_TecsStatusMsg_t  TecsStatus;
            CFE_PSP_MemCpy((void*)&TecsStatus, (void*)MsgPtr, sizeof(TecsStatus));

            printf("Sent PX4_TECS_STATUS_MID\n");
            printf("Mode: %d\n", TecsStatus.Mode);
            localTime = FPC_Test_GetTimeFromTimestamp(TecsStatus.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("AltitudeSp: %f\n", TecsStatus.AltitudeSp);
            printf("AltitudeFiltered: %f\n", TecsStatus.AltitudeFiltered);
            printf("FlightPathAngleSp: %f\n", TecsStatus.FlightPathAngleSp);
            printf("FlightPathAngle: %f\n", TecsStatus.FlightPathAngle);
            printf("AirspeedSp: %f\n", TecsStatus.AirspeedSp);
            printf("AirspeedFiltered: %f\n", TecsStatus.AirspeedFiltered);
            printf("AirspeedDerivativeSp: %f\n", TecsStatus.AirspeedDerivativeSp);
            printf("AirspeedDerivative: %f\n", TecsStatus.AirspeedDerivative);
            printf("TotalEnergyError: %f\n", TecsStatus.TotalEnergyError);
            printf("EnergyDistributionError: %f\n", TecsStatus.EnergyDistributionError);
            printf("TotalEnergyRateError: %f\n", TecsStatus.TotalEnergyRateError);
            printf("EnergyDistributionRateError: %f\n", TecsStatus.EnergyDistributionRateError);
            printf("ThrottleInteg: %f\n", TecsStatus.ThrottleInteg);
            printf("PitchInteg: %f\n", TecsStatus.PitchInteg);
            break;
        }
        default:
        {
            printf("Sent Invalid Message ID\n");
            break;
        }
    }

    return 0;
}

/**
 * Test FPC AppMain(), Nominal - Wakeup
 */
void Test_FPC_AppMain_Nominal_Wakeup(void)
{
    FPC   oFPC{};

    /* The following will emulate the behavior of receiving FPC_WAKEUP_MID */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FPC_GetPSPTimeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FPC_SendMsgHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif
}


/**
 * Test FPC AppMain(), ProcessNewDataHook
 */
int32 Test_FPC_AppMain_ProcessNewDataHook(void *dst, void *src, uint32 size)
{
    unsigned char *pBuff = NULL;
    int i = 0;
    CFE_SB_Msg_t*   dataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    pBuff = (unsigned char*)src;
    printf("###ProcessNewDataHook: ");
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
        case PX4_MANUAL_CONTROL_SETPOINT_MID:
            ProcessNewDataHook_MsgId = PX4_MANUAL_CONTROL_SETPOINT_MID;
            break;
        case PX4_VEHICLE_CONTROL_MODE_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_CONTROL_MODE_MID;
            break;
        case PX4_POSITION_SETPOINT_TRIPLET_MID:
            ProcessNewDataHook_MsgId = PX4_POSITION_SETPOINT_TRIPLET_MID;
            break;
        case PX4_VEHICLE_STATUS_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_STATUS_MID;
            break;
        case PX4_VEHICLE_LAND_DETECTED_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_LAND_DETECTED_MID;
            break;
        case PX4_VEHICLE_LOCAL_POSITION_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_LOCAL_POSITION_MID;
            break;
        case PX4_VEHICLE_GLOBAL_POSITION_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_GLOBAL_POSITION_MID;
            break;
        case PX4_AIRSPEED_MID:
            ProcessNewDataHook_MsgId = PX4_AIRSPEED_MID;
            break;
        case PX4_VEHICLE_ATTITUDE_MID:
            ProcessNewDataHook_MsgId = PX4_VEHICLE_ATTITUDE_MID;
            break;
        case PX4_SENSOR_COMBINED_MID:
            ProcessNewDataHook_MsgId = PX4_SENSOR_COMBINED_MID;
            break;
        case PX4_SENSOR_BARO_MID:
            ProcessNewDataHook_MsgId = PX4_SENSOR_BARO_MID;
            break;
        default:
            ProcessNewDataHook_MsgId = 0;
            break;
    }

    return ((int32)ProcessNewDataHook_MsgId);
}

/**
 * Test FPC AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_FPC_AppMain_ProcessNewData_InvalidMsgID(void)
{
    FPC   oFPC{};

    int32 DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
#if 0
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2,"Event Count = 2");
    UtAssert_EventSent(FPC_MSGID_ERR_EID, CFE_EVS_ERROR, "", "Error Event Sent");
#endif
}

/**
 * Test FPC AppMain(), ProcessNewData - ManualControlSetpoint
 */
void Test_FAC_AppMain_ProcessNewData_ManualControlSetpoint(void)
{
    FPC   oFPC{};

    int32                           DataPipe;
    PX4_ManualControlSetpointMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_MANUAL_CONTROL_SETPOINT_MID,
                  "AppMain_ProcessNewData_ManualControlSetpoint");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleControlMode
 */
void Test_FAC_AppMain_ProcessNewData_VehicleControlMode(void)
{
    FPC   oFPC{};

    int32                        DataPipe;
    PX4_VehicleControlModeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_CONTROL_MODE_MID,
                  "AppMain_ProcessNewData_VehicleControlMode");
}

/**
 * Test FPC AppMain(), ProcessNewData - PositionSetpointTriplet
 */
void Test_FAC_AppMain_ProcessNewData_PositionSetpointTriplet(void)
{
    FPC   oFPC{};

    int32                             DataPipe;
    PX4_PositionSetpointTripletMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_POSITION_SETPOINT_TRIPLET_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_POSITION_SETPOINT_TRIPLET_MID,
                  "AppMain_ProcessNewData_PositionSetpointTriplet");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleStatus
 */
void Test_FAC_AppMain_ProcessNewData_VehicleStatus(void)
{
    FPC   oFPC{};

    int32                             DataPipe;
    PX4_VehicleStatusMsg_t            InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_STATUS_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_STATUS_MID,
                  "AppMain_ProcessNewData_VehicleStatus");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleLandDetected
 */
void Test_FAC_AppMain_ProcessNewData_VehicleLandDetected(void)
{
    FPC   oFPC{};

    int32                             DataPipe;
    PX4_VehicleLandDetectedMsg_t      InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_LAND_DETECTED_MID,
                  "AppMain_ProcessNewData_VehicleLandDetected");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleLocalPosition
 */
void Test_FAC_AppMain_ProcessNewData_VehicleLocalPosition(void)
{
    FPC   oFPC{};

    int32                             DataPipe;
    PX4_VehicleLocalPositionMsg_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LOCAL_POSITION_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_LOCAL_POSITION_MID,
                  "AppMain_ProcessNewData_VehicleLocalPosition");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleGlobalPosition
 */
void Test_FAC_AppMain_ProcessNewData_VehicleGlobalPosition(void)
{
    FPC   oFPC{};

    int32                             DataPipe;
    PX4_VehicleGlobalPositionMsg_t    InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_GLOBAL_POSITION_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_GLOBAL_POSITION_MID,
                  "AppMain_ProcessNewData_VehicleGlobalPosition");
}

/**
 * Test FPC AppMain(), ProcessNewData - Airspeed
 */
void Test_FAC_AppMain_ProcessNewData_Airspeed(void)
{
    FPC   oFPC{};

    int32                     DataPipe;
    PX4_AirspeedMsg_t         InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_AIRSPEED_MID,
                  "AppMain_ProcessNewData_Airspeed");
}

/**
 * Test FPC AppMain(), ProcessNewData - VehicleAttitude
 */
void Test_FAC_AppMain_ProcessNewData_VehicleAttitude(void)
{
    FPC   oFPC{};

    int32                     DataPipe;
    PX4_VehicleAttitudeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_VEHICLE_ATTITUDE_MID,
                  "AppMain_ProcessNewData_VehicleAttitude");
}

/**
 * Test FPC AppMain(), ProcessNewData - SensorCombined
 */
void Test_FAC_AppMain_ProcessNewData_SensorCombined(void)
{
    FPC   oFPC{};

    int32                     DataPipe;
    PX4_SensorCombinedMsg_t   InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_COMBINED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_SENSOR_COMBINED_MID,
                  "AppMain_ProcessNewData_SensorCombined");
}

/**
 * Test FPC AppMain(), ProcessNewData - SensorBaro
 */
void Test_FAC_AppMain_ProcessNewData_SensorBaro(void)
{
    FPC   oFPC{};

    int32                     DataPipe;
    PX4_SensorBaroMsg_t       InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_BARO_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FPC_AppMain_ProcessNewDataHook);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

    /* Verify results */
    UtAssert_True(ProcessNewDataHook_MsgId == PX4_SENSOR_BARO_MID,
                  "AppMain_ProcessNewData_SensorBaro");
}

/**************************************************************************
 * Tests for FPC UpdateParamsFromTable()
 **************************************************************************/
/**
 * Test FPC UpdateParamsFromTable(), SendMsgHook
 */
int32 Test_FPC_UpdateParamsFromTable_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    return 0;
}

/**
 * Test FPC UpdateParamsFromTable()
 */
void Test_FPC_UpdateParamsFromTable(void)
{
    FPC   oFPC{};

    double expected_checksum = 313.117959422;

    UpdateParams_ParamChecksum = 0.0;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FPC_UpdateParamsFromTable_SendMsgHook);

#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif

#if 0
    if (fabs(UpdateParams_ParamChecksum - expected_checksum) <= FLT_EPSILON) // Fail with DBL_EPSILON
    {
        UtAssert_True(TRUE, "FPC UpdateParamsFromTable");
    }
    else
    {
        UtAssert_True(FALSE, "FPC UpdateParamsFromTable");
    }
#endif
}




/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void FPC_App_Test_AddTestCases(void)
{
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtTest_Add(Test_FPC_InitEvent_Fail_Register, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitEvent_Fail_Register");

    UtTest_Add(Test_FPC_InitPipe_Fail_CreateSCHPipe, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_FPC_InitPipe_Fail_SubscribeWakeup, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_FPC_InitPipe_Fail_SubscribeSendHK, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_FPC_InitPipe_Fail_CreateCMDPipe, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_FPC_InitPipe_Fail_SubscribeCMD, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_FPC_InitPipe_Fail_CreateDATAPipe, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeManualControlSp, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeManualControlSp");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleControlMode, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_InitPipe_Fail_SubscribeVehicleControlMode");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleGlobalPosition, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_InitPipe_Fail_SubscribeVehicleGlobalPosition");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleLocalPosition, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_InitPipe_Fail_SubscribeVehicleLocalPosition");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribePositionSetpointTriplet, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_InitPipe_Fail_SubscribePositionSetpointTriplet");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleStatus, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVehicleStatus");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleLandDetected, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_InitPipe_Fail_SubscribeVehicleLandDetected");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeAirspeed, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeAirspeed");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleAttitude, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVehicleAttitude");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeSensorCombined, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeSensorCombined");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeSensorBaro, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeSensorBaro");

    UtTest_Add(Test_FPC_InitDataNominal, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitDataNominal");

    UtTest_Add(Test_FPC_InitApp_Fail_InitEvent, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitApp_Fail_InitEvent");
    UtTest_Add(Test_FPC_InitApp_Fail_InitPipe, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitApp_Fail_InitPipe");
    UtTest_Add(Test_FPC_InitApp_Fail_InitData, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitApp_Fail_InitData");
    UtTest_Add(Test_FPC_InitApp_Fail_InitConfigTbl, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_FPC_InitApp_Nominal, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitApp_Nominal");
#endif

    UtTest_Add(Test_FPC_AppMain_Fail_RegisterApp, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_FPC_AppMain_Fail_InitApp, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_Fail_InitApp");
    UtTest_Add(Test_FPC_AppMain_Fail_AcquireConfigPtrs, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FPC_AppMain_InvalidSchMessage, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_InvalidSchMessage");
    UtTest_Add(Test_FPC_AppMain_Nominal_SendHK, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_Nominal_SendHK");
    UtTest_Add(Test_FPC_AppMain_Nominal_Wakeup, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_FPC_AppMain_ProcessNewData_InvalidMsgID, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_AppMain_ProcessNewData_InvalidMsgID");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_ManualControlSetpoint, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_ManualControlSetpoint");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleControlMode, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_VehicleControlMode");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_PositionSetpointTriplet, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_PositionSetpointTriplet");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleStatus, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_VehicleStatus");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleLandDetected, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_VehicleLandDetected");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleLocalPosition, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_VehicleLocalPosition");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleGlobalPosition, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FAC_AppMain_ProcessNewData_VehicleGlobalPosition");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_Airspeed, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_Airspeed");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleAttitude, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VehicleAttitude");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_SensorCombined, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_SensorCombined");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_SensorBaro, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_SensorBaro");

    UtTest_Add(Test_FPC_UpdateParamsFromTable, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_UpdateParamsFromTable");
}
