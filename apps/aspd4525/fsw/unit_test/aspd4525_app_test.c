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

#include "aspd4525_app_test.h"
#include "aspd4525_app.h"
#include "aspd4525_test_utils.h"
#include "ut_aspd4525_custom_stubs.h"
#include "aspd4525_custom_hooks.h"

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
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include <time.h>


CFE_SB_MsgId_t SendHkHook_MsgId = 0;
CFE_SB_MsgId_t WakeupHook_MsgId = 0;


/**
 * Test ASPD4525 GetPSPTimeHook
 */
void Test_ASPD4525_GetPSPTimeHook(OS_time_t *LocalTime)
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
 * Tests for ASPD4525_InitEvent()
 **************************************************************************/
/**
 * Test ASPD4525_InitEvent() with failed CFE_EVS_Register
 */
void Test_ASPD4525_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for ASPD4525_InitPipe()
 **************************************************************************/
/**
 * Test ASPD4525_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_ASPD4525_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test ASPD4525_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_ASPD4525_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test ASPD4525_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_ASPD4525_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test ASPD4525_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_ASPD4525_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test ASPD4525_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_ASPD4525_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test ASPD4525_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_ASPD4525_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = ASPD4525_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**************************************************************************
 * Tests for ASPD4525_InitData()
 **************************************************************************/
/**
 * Test ASPD4525_InitData()
 */
void Test_ASPD4525_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = ASPD4525_InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for ASPD4525_InitApp()
 **************************************************************************/
/**
 * Test ASPD4525_InitApp(), fail init event
 */
void Test_ASPD4525_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test ASPD4525_InitApp(), fail init pipe
 */
void Test_ASPD4525_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test ASPD4525_InitApp(), fail init data.
 * NOTE: no current way to fail ASPD4525_InitData() in default
 */
void Test_ASPD4525_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test ASPD4525_InitApp(), fail init config table
 */
void Test_ASPD4525_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test ASPD4525_InitApp(), fail init CDS table
 */
void Test_ASPD4525_InitApp_Fail_InitCDSTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_ES_CDS_INVALID_NAME;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCDS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init CDS table");
}


/**
 * Test ASPD4525_InitApp(), Nominal
 */
void Test_ASPD4525_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = ASPD4525_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for ASPD4525_AppMain()
 **************************************************************************/
/**
 * Test ASPD4525_AppMain(), Fail RegisterApp
 */
void Test_ASPD4525_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    ASPD4525_AppMain();
}


/**
 * Test ASPD4525_AppMain(), Fail InitApp
 */
void Test_ASPD4525_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    ASPD4525_AppMain();
}


/**
 * Test ASPD4525_AppMain(), Fail AcquireConfigPtrs
 */
void Test_ASPD4525_AppMain_Fail_AcquireConfigPtrs(void)
{
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Failed to get Config table's address (0x%04X)",
            (unsigned int)CFE_TBL_ERR_INVALID_HANDLE);

    UtAssert_EventSent(ASPD4525_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), Fail AcquireConfigPtrs");
}


/**
 * Test ASPD4525_AppMain(), Invalid Schedule Message
 */
void Test_ASPD4525_AppMain_InvalidSchMessage(void)
{
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid SCH msgId (0x%04X)",
            (unsigned int)0);

    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), Invalid Schedule Message");
}


/**
 * Hook to support: ASPD4525_AppMain(), Nominal - SendHK
 */
int32 Test_ASPD4525_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;
    ASPD4525_HkTlm_t    HkTlm;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) + ((MsgPtr->Hdr).StreamId[1]));

    pBuff = (unsigned char *)MsgPtr;
    printf("###SendHK_SendMsgHook(msgLen %u)", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = ASPD4525_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case ASPD4525_HK_TLM_MID:
        {
            SendHkHook_MsgId = ASPD4525_HK_TLM_MID;
            printf("Sent ASPD4525_HK_TLM_MID\n");
            memcpy((void *)&HkTlm, (void *)MsgPtr, sizeof(HkTlm));

            printf("usCmdCnt: %u\n", HkTlm.usCmdCnt);
            printf("usCmdErrCnt: %u\n", HkTlm.usCmdErrCnt);
            printf("uPressureCount: %lu\n", HkTlm.uPressureCount);
            printf("uTemperatureCount: %lu\n", HkTlm.uTemperatureCount);
            printf("uStatus: %lu\n", HkTlm.uStatus);
            printf("fTemperature: %f\n", HkTlm.fTemperature);
            printf("fTrueAirSpeedUnfiltered: %f\n", HkTlm.fTrueAirSpeedUnfiltered);
            printf("fIndicatedAirSpeed: %f\n", HkTlm.fIndicatedAirSpeed);
            printf("fTrueAirSpeed: %f\n", HkTlm.fTrueAirSpeed);
            printf("fPressureMinimum_PSI: %f\n", HkTlm.fPressureMinimum_PSI);
            printf("fPressureMaximum_PSI: %f\n", HkTlm.fPressureMaximum_PSI);
            printf("fTemperatureMinimum_Celcius: %f\n", HkTlm.fTemperatureMinimum_Celcius);
            printf("fTemperatureMaximum_Celcius: %f\n", HkTlm.fTemperatureMaximum_Celcius);
            printf("uAirDensityCalculationMode: %d\n", HkTlm.uAirDensityCalculationMode);
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
 * Test ASPD4525_AppMain(), Nominal - SendHK
 */
void Test_ASPD4525_AppMain_Nominal_SendHK(void)
{
    ASPD4525_NoArgCmd_t  InMsg;
    int32                SchPipe;

    SchPipe = Ut_CFE_SB_CreatePipe(ASPD4525_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void *)&InMsg, ASPD4525_SEND_HK_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void *)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate behavior of receiving a SCH message
       to Wakeup */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INITDATA_INDEX,
                                    (void *)&ASPD4525_Custom_InitDataHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INIT_INDEX,
                                    (void *)&ASPD4525_Custom_InitHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_MEASURE_INDEX,
                                    (void *)&ASPD4525_Custom_MeasureHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 4);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_ASPD4525_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    /* Verify results */
    UtAssert_True (SendHkHook_MsgId == ASPD4525_HK_TLM_MID,
                   "AppMain_Nominal_SendHK");

}


/**
 * Hook to support: ASPD4525_AppMain(), Nominal - Wakeup
 */
int32 Test_ASPD4525_AppMain_Nominal_WakeupHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;
    PX4_AirspeedMsg_t   AirSpeedMsg;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) + ((MsgPtr->Hdr).StreamId[1]));

    pBuff = (unsigned char *)MsgPtr;
    printf("###WakeupHook(msgLen %u)", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = ASPD4525_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case PX4_AIRSPEED_MID:
        {
            WakeupHook_MsgId = PX4_AIRSPEED_MID;
            printf("Sent PX4_AIRSPEED_MID\n");
            memcpy((void *)&AirSpeedMsg, (void *)MsgPtr,
                   sizeof(AirSpeedMsg));

            localTime = ASPD4525_Test_GetTimeFromTimestamp(AirSpeedMsg.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));

            printf("IndicatedAirspeed: %f\n", AirSpeedMsg.IndicatedAirspeed);
            printf("TrueAirspeed: %f\n", AirSpeedMsg.TrueAirspeed);
            printf("TrueAirspeedUnfiltered: %f\n",
                    AirSpeedMsg.TrueAirspeedUnfiltered);
            printf("AirTemperature: %f\n", AirSpeedMsg.AirTemperature);
            printf("Confidence: %f\n", AirSpeedMsg.Confidence);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }
}


/**
 * Test ASPD4525_AppMain(), Nominal - Wakeup
 */
void Test_ASPD4525_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message
       to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INITDATA_INDEX,
                                    (void *)&ASPD4525_Custom_InitDataHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INIT_INDEX,
                                    (void *)&ASPD4525_Custom_InitHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_MEASURE_INDEX,
                                    (void *)&ASPD4525_Custom_MeasureHook);

    WakeupHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_ASPD4525_AppMain_Nominal_WakeupHook);
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                              (void *)&Test_ASPD4525_GetPSPTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    /* Verify results */
    UtAssert_True (WakeupHook_MsgId == PX4_AIRSPEED_MID,
                   "AppMain_Nominal_Wakeup");
}


/**
 * Test ASPD4525_AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_ASPD4525_AppMain_ProcessNewData_InvalidMsgID(void)
{
    ASPD4525_InData_t  InMsg;
    int32              DataPipe;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(ASPD4525_DATA_PIPE_NAME);
    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(ASPD4525_InData_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InMsg, DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid data msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), ProcessNewData - InvalidMsgID EventSent");
}


/**
 * Test ASPD4525_ReadDevice()
 */
void Test_ASPD4525_ReadDevice(void)
{
    boolean bResult = TRUE;
    int32   iStatus = CFE_SUCCESS;

    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INITDATA_INDEX,
                                    (void *)&ASPD4525_Custom_InitDataHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_INIT_INDEX,
                                    (void *)&ASPD4525_Custom_InitHook);
    Ut_ASPD4525_Custom_SetFunctionHook(UT_ASPD4525_CUSTOM_MEASURE_INDEX,
                                    (void *)&ASPD4525_Custom_MeasureHook);

    /* Execute the function being tested */
    ASPD4525_InitApp();

    ASPD4525_ReadDevice();

    printf("ASPD4525_AppData.sPressureDiffCount: 0x%x\n",
                                    ASPD4525_AppData.sPressureDiffCount);
    printf("ASPD4525_AppData.sIemperatureCount: 0x%x\n",
                                    ASPD4525_AppData.sIemperatureCount);
    printf("ASPD4525_AppData.ucStatus: %u\n",
                                    ASPD4525_AppData.ucStatus);
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void ASPD4525_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_InitEvent_Fail_Register,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitEvent_Fail_Register");

    UtTest_Add(Test_ASPD4525_InitPipe_Fail_CreateSCHPipe,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_ASPD4525_InitPipe_Fail_SubscribeWakeup,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_ASPD4525_InitPipe_Fail_SubscribeSendHK,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_ASPD4525_InitPipe_Fail_CreateCMDPipe,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_ASPD4525_InitPipe_Fail_SubscribeCMD,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_ASPD4525_InitPipe_Fail_CreateDATAPipe,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitPipe_Fail_CreateDATAPipe");

    UtTest_Add(Test_ASPD4525_InitData,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitData");

    UtTest_Add(Test_ASPD4525_InitApp_Fail_InitEvent,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Fail_InitEvent");
    UtTest_Add(Test_ASPD4525_InitApp_Fail_InitPipe,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Fail_InitPipe");
    UtTest_Add(Test_ASPD4525_InitApp_Fail_InitData,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Fail_InitData");
    UtTest_Add(Test_ASPD4525_InitApp_Fail_InitConfigTbl,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_ASPD4525_InitApp_Fail_InitCDSTbl,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Fail_InitCDSTbl");
    UtTest_Add(Test_ASPD4525_InitApp_Nominal,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitApp_Nominal");

    UtTest_Add(Test_ASPD4525_AppMain_Fail_RegisterApp,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_ASPD4525_AppMain_Fail_InitApp,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_Fail_InitApp");
    UtTest_Add(Test_ASPD4525_AppMain_Fail_AcquireConfigPtrs,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_ASPD4525_AppMain_InvalidSchMessage,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_InvalidSchMessage");
    UtTest_Add(Test_ASPD4525_AppMain_Nominal_SendHK,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_Nominal_SendHK");
    UtTest_Add(Test_ASPD4525_AppMain_Nominal_Wakeup,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_ASPD4525_AppMain_ProcessNewData_InvalidMsgID,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_AppMain_ProcessNewData_InvalidMsgID");

    UtTest_Add(Test_ASPD4525_ReadDevice,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ReadDevice");
}
