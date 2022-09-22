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
#include "amc_app.h"
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

#include "amc_test_utils.hpp"
#include "amc_app_test.hpp"

#include <time.h>
#include <float.h>

uint16  ProcessDataHook_MsgId = 0;
int32   WriteToSysLog_HookCalledCnt = 0;
int32   SendEvent_HookCalledCnt = 0;
int32   SendHkHookCalledCount = 0;

/**************************************************************************
 * Tests for AMC InitEvent()
 **************************************************************************/

/**
 * Test AMC_InitEvent() with failed CFE_EVS_Register
 */
void Test_AMC_InitEvent_Fail_Register(void)
{
    AMC oAMC;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for AMC InitPipe()
 **************************************************************************/
/**
 * Test AMC_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_AMC_InitPipe_Fail_CreateSCHPipe(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for AmcUpdateMotors
 */
void Test_AMC_InitPipe_Fail_SubscribeAmcUpdateMotors(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for AmcUpdateMotors");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_AMC_InitPipe_Fail_SubscribeSendHK(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test AMC_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_AMC_InitPipe_Fail_CreateCMDPipe(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for CMD msg
 */
void Test_AMC_InitPipe_Fail_SubscribeCMD(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test AMC_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_AMC_InitPipe_Fail_CreateDATAPipe(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorArmed
 */
void Test_AMC_InitPipe_Fail_SubscribeActuatorArmed(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for ActuatorArmed");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls0
 */
void Test_AMC_InitPipe_Fail_SubscribeActuatorControls0(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls0");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls1
 */
void Test_AMC_InitPipe_Fail_SubscribeActuatorControls1(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls1");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls2
 */
void Test_AMC_InitPipe_Fail_SubscribeActuatorControls2(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls2");
}


/**
 * Test AMC_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls3
 */
void Test_AMC_InitPipe_Fail_SubscribeActuatorControls3(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oAMC.InitPipes();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls3");
}


/**************************************************************************
 * Tests for AMC InitData()
 **************************************************************************/
/**
 * Test AMC InitData_Nominal()
 * InitData() needs that the InitConfigTbl() executed which is private
 */
void Test_AMC_InitData_Nominal(void)
{
    AMC oAMC;
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitData, Nominal");
}


/**************************************************************************
 * Tests for AMC InitApp()
 **************************************************************************/
/**
 * Test AMC InitApp(), fail init event
 */
void Test_AMC_InitApp_Fail_InitEvent(void)
{
    AMC oAMC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test AMC InitApp(), fail init pipe
 */
void Test_AMC_InitApp_Fail_InitPipe(void)
{
    AMC oAMC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test AMC InitApp(), fail init data.
 * NOTE: no current way to fail AMC InitData() in default
 */
void Test_AMC_InitApp_Fail_InitData(void)
{
    AMC oAMC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test AMC InitApp(), fail init config table
 */
void Test_AMC_InitApp_Fail_InitConfigTbl(void)
{
    AMC oAMC;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test AMC InitApp(), Nominal
 */
void Test_AMC_InitApp_Nominal(void)
{
    AMC oAMC;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oAMC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, Nominal");
}


/**************************************************************************
 * Tests for extern AMC_AppMain()
 **************************************************************************/
/**
 * Test AMC GetPSPTimeHook
 */
void Test_AMC_GetPSPTimeHook(OS_time_t *LocalTime)
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
 * Test AMC AppMain(), SendMsgHook
 */
int32 Test_AMC_AppMain_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);                /* DataLenth + 7 */
    printf("###AppMain_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = AMC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case AMC_HK_TLM_MID:
        {
            printf("Send AMC_HK_TLM_MID\n");
            break;
        }
        case PX4_ACTUATOR_OUTPUTS_MID:
        {
            PX4_ActuatorOutputsMsg_t  ActOuts;
            CFE_PSP_MemCpy((void*)&ActOuts, (void*)MsgPtr, sizeof(ActOuts));

            printf("Send PX4_ACTUATOR_OUTPUTS_MID:\n");
            localTime = AMC_Test_GetTimeFromTimestamp(ActOuts.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Count: %lu\n", ActOuts.Count);
            for (i = 0; i < PX4_ACTUATOR_OUTPUTS_MAX; i++)
            {
                printf("Output[%d]: %f\n", i, ActOuts.Output[i]);
            }
            break;
        }
        default:
        {
            printf("Send unknown message.\n");
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test AMC extern AMC_AppMain(), Nominal
 */
void Test_AMC_AMC_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_AMC_AppMain_SendMsgHook);

    AMC_AppMain();
}


/**************************************************************************
 * Tests for oAMC::AppMain()
 **************************************************************************/
/**
 * Test AMC AppMain(), WriteToSysLogHook
 */
int32 Test_AMC_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
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
 * Test AMC AppMain(), SendEventHook
 */
int32 Test_AMC_AppMain_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
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
 * Test AMC AppMain(), Fail RegisterApp
 */
void Test_AMC_AppMain_Fail_RegisterApp(void)
{
    AMC oAMC;

    int32 expected = CFE_ES_ERR_APP_REGISTER;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC_AppMain(), Fail InitApp
 */
void Test_AMC_AppMain_Fail_InitApp(void)
{
    AMC oAMC;

    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_AMC_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True (WriteToSysLog_HookCalledCnt == 2, "AppMain, Fail_InitApp");
}


/**
 * Test AMC_AppMain(), Fail AcquireConfigPtrs
 */
void Test_AMC_AppMain_Fail_AcquireConfigPtrs(void)
{
    AMC oAMC;

    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 3);

    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_AMC_AppMain_SendEventHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True (SendEvent_HookCalledCnt == 2, "AppMain, Fail_AcquireConfigPtrs");
}


/**
 * Test AMC AppMain(), SchPipeError
 */
void Test_AMC_AppMain_SchPipeError(void)
{
    AMC oAMC;

    int32 expected = CFE_SB_PIPE_RD_ERR;

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 20);

    oAMC.AppMain();
}


/**
 * Test AMC AppMain(), SchPipeTimeout
 */
void Test_AMC_AppMain_SchPipeTimeout(void)
{
    AMC oAMC;

    int32 expected = CFE_SB_TIME_OUT;

    /* The following will emulate the behavior of SCH pipe: No message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    oAMC.AppMain();
}


/**
 * Test AMC_AppMain(), Invalid Schedule Message
 */
void Test_AMC_AppMain_InvalidSchMessage(void)
{
    AMC oAMC;

    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, PX4_ACTUATOR_ARMED_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Hook to support: AMC AppMain(), Nominal - SendHK
 */
int32 Test_AMC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    AMC_HkTlm_t        HkMsg;

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
    localTime = AMC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case AMC_HK_TLM_MID:
        {
            SendHkHookCalledCount++;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));

            printf("Sent AMC_HK_TLM_MID:\n");
            printf("CmdCnt: %d\n", HkMsg.usCmdCnt);
            printf("CmdErrCnt: %d\n", HkMsg.usCmdErrCnt);

            localTime = AMC_Test_GetTimeFromTimestamp(HkMsg.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Count: %u\n", (unsigned int)HkMsg.Count);
            for (i = 0; i < PX4_ACTUATOR_OUTPUTS_MAX; i++)
            {
                printf("Output[%d]: %f\n", i, HkMsg.Output[i]);
            }
            for (i = 0; i < AMC_MAX_MOTOR_OUTPUTS; i++)
            {
                printf("PWM[%d]: %u\n", i, HkMsg.PWM[i]);
            }
            printf("DebugArmed: %u\n", HkMsg.DebugArmed);
            printf("DebugEngaged: %u\n", HkMsg.DebugEngaged);
            printf("ArmedTimeout: %d\n", HkMsg.ArmedTimeout);
            break;
        }
        case PX4_ACTUATOR_OUTPUTS_MID:
        {
            printf("Sent PX4_ACTUATOR_OUTPUTS_MID\n");
            break;
        }
        default:
        {
            printf("Sent Unknown message.\n");
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test AMC_AppMain(), Nominal - SendHK
 */
void Test_AMC_AppMain_Nominal_SendHK(void)
{
    AMC oAMC;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHkHookCalledCount = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_AMC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True (SendHkHookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test AMC_AppMain(), Nominal - UpdateMotors
 */
void Test_AMC_AppMain_Nominal_UpdateMotors(void)
{
    AMC oAMC;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, (void*)&Test_AMC_AppMain_SendMsgHook);

    /* Execute the function being tested */
    oAMC.AppMain();

}


/**
 * Test AMC AppMain(), ProcessDataHook
 */
int32 Test_AMC_AppMain_ProcessDataHook(void *dst, void *src, uint32 size)
{
    unsigned char *pBuff = NULL;
    int i = 0;
    CFE_SB_Msg_t*   dataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    pBuff = (unsigned char*)src;
    printf("###ProcessDataHook: ");
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
            ProcessDataHook_MsgId = PX4_ACTUATOR_ARMED_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_0_MID:
            ProcessDataHook_MsgId = PX4_ACTUATOR_CONTROLS_0_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_1_MID:
            ProcessDataHook_MsgId = PX4_ACTUATOR_CONTROLS_1_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_2_MID:
            ProcessDataHook_MsgId = PX4_ACTUATOR_CONTROLS_2_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_3_MID:
            ProcessDataHook_MsgId = PX4_ACTUATOR_CONTROLS_3_MID;
            break;
        default:
            ProcessDataHook_MsgId = 0;
            break;
    }

    return ((int32)ProcessDataHook_MsgId);
}


/**
 * Test AMC AppMain(), ProcessData - DataPipeError
 */
void Test_AMC_AppMain_ProcessData_DataPipeError(void)
{
    AMC oAMC;

    int32              expected = CFE_SB_PIPE_RD_ERR;
    int32              SchPipe;
    AMC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("AMC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, AMC_UPDATE_MOTORS_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC AppMain(), ProcessData - InvalidMsgID
 */
void Test_AMC_AppMain_ProcessData_InvalidMsgID(void)
{
    AMC oAMC;

    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    UtAssert_True((oAMC.HkTlm.usCmdErrCnt == 1) && (ProcessDataHook_MsgId == 0),
                   "Test_AMC_AppMain_ProcessData_InvalidMsgID");
}


/**
 * Test AMC AppMain(), ProcessData - ActuatorArmed
 */
void Test_AMC_AppMain_ProcessData_ActuatorArmed(void)
{
    AMC oAMC;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True(ProcessDataHook_MsgId == PX4_ACTUATOR_ARMED_MID, "AppMain_ProcessData_ActuatorArmed");
}


/**
 * Test AMC AppMain(), ProcessData - ActuatorControls0
 */
void Test_AMC_AppMain_ProcessData_ActuatorControls0(void)
{
    AMC oAMC;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_0_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True(ProcessDataHook_MsgId == PX4_ACTUATOR_CONTROLS_0_MID,
                  "AppMain_ProcessData_ActuatorControls0");
}


/**
 * Test AMC AppMain(), ProcessData - ActuatorControls1
 */
void Test_AMC_AppMain_ProcessData_ActuatorControls1(void)
{
    AMC oAMC;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_1_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True(ProcessDataHook_MsgId == PX4_ACTUATOR_CONTROLS_1_MID,
                  "AppMain_ProcessData_ActuatorControls1");
}


/**
 * Test AMC AppMain(), ProcessData - ActuatorControls2
 */
void Test_AMC_AppMain_ProcessData_ActuatorControls2(void)
{
    AMC oAMC;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_2_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True(ProcessDataHook_MsgId == PX4_ACTUATOR_CONTROLS_2_MID,
                  "AppMain_ProcessData_ActuatorControls2");
}


/**
 * Test AMC AppMain(), ProcessData - ActuatorControls3
 */
void Test_AMC_AppMain_ProcessData_ActuatorControls3(void)
{
    AMC oAMC;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("AMC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_3_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_UPDATE_MOTORS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessDataHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_AMC_AppMain_ProcessDataHook);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    UtAssert_True(ProcessDataHook_MsgId == PX4_ACTUATOR_CONTROLS_3_MID,
                  "AppMain_ProcessData_ActuatorControls3");
}


/**************************************************************************
 * Tests for AMC Config Table Data()
 **************************************************************************/
/**
 * Test AMC Config Table Data, ConfigTbl
 */
void Test_AMC_ConfigTable_ConfigTbl(void)
{
    AMC oAMC;
    AMC *pAMC = &oAMC;

    int32  iStatus = CFE_SUCCESS;
    double expected_checksum = 54508.0;
    double ConfigChecksum = 0.0;

    /* Execute the function being tested */
    iStatus = oAMC.InitApp();

    ConfigChecksum = GetConfigTblChecksum(pAMC);

    /* Verify results */
    if ((iStatus == CFE_SUCCESS) &&
        (fabs(ConfigChecksum - expected_checksum) <= FLT_EPSILON))
    {
        UtAssert_True(TRUE, "AMC Config Table Data, ConfigTbl");
    }
    else
    {
        UtAssert_True(FALSE, "AMC Config Table Data, ConfigTbl");
    }
}


/**
 * Test AMC Config Table Data, MultirotorMixerConfigTbl
 */
void Test_AMC_ConfigTable_MultirotorConfigTbl(void)
{
    AMC oAMC;
    AMC *pAMC = &oAMC;

    int32  iStatus = CFE_SUCCESS;
    double expected_checksum = 0.0;
    double ConfigChecksum = 0.0;

    /* Execute the function being tested */
    iStatus = oAMC.InitApp();

    ConfigChecksum = GetMultirotorMixerConfigTblChecksum(pAMC);

    /* Verify results */
    if ((iStatus == CFE_SUCCESS) &&
        (fabs(ConfigChecksum - expected_checksum) <= FLT_EPSILON))
    {
        UtAssert_True(TRUE, "AMC Config Table Data, MultirotorMixerConfigTbl");
    }
    else
    {
        UtAssert_True(FALSE, "AMC Config Table Data, MultirotorMixerConfigTbl");
    }
}


/**
 * Test AMC Config Table Data, SimpleMixerConfigTbl
 */
void Test_AMC_ConfigTable_SimpleMixerConfigTbl(void)
{
    AMC oAMC;
    AMC *pAMC = &oAMC;

    int32  iStatus = CFE_SUCCESS;
    double expected_checksum = 75.0;
    double ConfigChecksum = 0.0;

    /* Execute the function being tested */
    iStatus = oAMC.InitApp();

    ConfigChecksum = GetSimpleMixerConfigTblChecksum(pAMC);

    /* Verify results */
    if ((iStatus == CFE_SUCCESS) &&
        (fabs(ConfigChecksum - expected_checksum) <= FLT_EPSILON))
    {
        UtAssert_True(TRUE, "AMC Config Table Data, SimpleMixerConfigTbl");
    }
    else
    {
        UtAssert_True(FALSE, "AMC Config Table Data, SimpleMixerConfigTbl");
    }
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void AMC_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_AMC_InitEvent_Fail_Register, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitEvent_Fail_Register");

    UtTest_Add(Test_AMC_InitPipe_Fail_CreateSCHPipe, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeAmcUpdateMotors, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeAmcUpdateMotors");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeSendHK, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_AMC_InitPipe_Fail_CreateCMDPipe, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeCMD, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_AMC_InitPipe_Fail_CreateDATAPipe, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeActuatorArmed, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeActuatorArmed");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeActuatorControls0, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeActuatorControls0");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeActuatorControls1, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeActuatorControls1");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeActuatorControls2, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeActuatorControls2");
    UtTest_Add(Test_AMC_InitPipe_Fail_SubscribeActuatorControls3, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitPipe_Fail_SubscribeActuatorControls3");

    UtTest_Add(Test_AMC_InitData_Nominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitData_Nominal");

    UtTest_Add(Test_AMC_InitApp_Fail_InitEvent, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitApp_Fail_InitEvent");
    UtTest_Add(Test_AMC_InitApp_Fail_InitPipe, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitApp_Fail_InitPipe");
    UtTest_Add(Test_AMC_InitApp_Fail_InitData, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitApp_Fail_InitData");
    UtTest_Add(Test_AMC_InitApp_Fail_InitConfigTbl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_AMC_InitApp_Nominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitApp_Nominal");

    UtTest_Add(Test_AMC_AMC_AppMain_Nominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AMC_AppMain_Nominal");
    UtTest_Add(Test_AMC_AppMain_Fail_RegisterApp, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_AMC_AppMain_Fail_InitApp, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_Fail_InitApp");
#if 0  // core dump
    UtTest_Add(Test_AMC_AppMain_Fail_AcquireConfigPtrs, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_Fail_AcquireConfigPtrs");
#endif
#if 0  // infinite loop
    UtTest_Add(Test_AMC_AppMain_SchPipeError, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_SchPipeError");
#endif
    UtTest_Add(Test_AMC_AppMain_SchPipeTimeout, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_SchPipeTimeout");
    UtTest_Add(Test_AMC_AppMain_InvalidSchMessage, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_InvalidSchMessage");
    UtTest_Add(Test_AMC_AppMain_Nominal_SendHK, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_Nominal_SendHK");
    UtTest_Add(Test_AMC_AppMain_Nominal_UpdateMotors, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_Nominal_UpdateMotors");
#if 0   // infinite loop
    UtTest_Add(Test_AMC_AppMain_ProcessData_DataPipeError, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_DataPipeError");
#endif
    UtTest_Add(Test_AMC_AppMain_ProcessData_InvalidMsgID, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_InvalidMsgID");
    UtTest_Add(Test_AMC_AppMain_ProcessData_ActuatorArmed, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_ActuatorArmed");
    UtTest_Add(Test_AMC_AppMain_ProcessData_ActuatorControls0, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_ActuatorControls0");
    UtTest_Add(Test_AMC_AppMain_ProcessData_ActuatorControls1, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_ActuatorControls1");
    UtTest_Add(Test_AMC_AppMain_ProcessData_ActuatorControls2, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_ActuatorControls2");
    UtTest_Add(Test_AMC_AppMain_ProcessData_ActuatorControls3, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_AppMain_ProcessData_ActuatorControls3");

    UtTest_Add(Test_AMC_ConfigTable_ConfigTbl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ConfigTable_ConfigTbl");
    UtTest_Add(Test_AMC_ConfigTable_MultirotorConfigTbl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ConfigTable_MultirotorConfigTbl");
    UtTest_Add(Test_AMC_ConfigTable_SimpleMixerConfigTbl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ConfigTable_SimpleMixerConfigTbl");

}
