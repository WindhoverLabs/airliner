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

#include <iostream>

int32   WriteToSysLog_HookCalledCnt = 0;
int32   SendEvent_HookCalledCnt = 0;
uint32  UpdateParams_ValidateStatus = 0x0;

double  UpdateParams_ParamChecksum = 0.0;

/**************************************************************************
 * Tests for FAC InitEvent()
 **************************************************************************/

/**
 * Test FAC InitEvent() with failed CFE_EVS_Register
 */
void Test_FAC_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for FAC InitPipe()
 **************************************************************************/
/**
 * Test FAC InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for SendHK
 */
void Test_FAC_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for FAC_SEND_HK");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for RunController
 */
void Test_FAC_InitPipe_Fail_SubscribeRunController(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for FAC_RUN_CONTROLLER");
}


/**
 * Test FAC InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FAC_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test FAC InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for BatteryStatus
 */
void Test_FAC_InitPipe_Fail_SubscribeBatteryStatus(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_BATTERY_STATUS_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for ManualControlSp
 */
void Test_FAC_InitPipe_Fail_SubscribeManualControlSp(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_MANUAL_CONTROL_SETPOINT_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VAttSp
 */
void Test_FAC_InitPipe_Fail_SubscribeVAttSp(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_ATTITUDE_SETPOINT_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VAtt
 */
void Test_FAC_InitPipe_Fail_SubscribeVAtt(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_ATTITUDE_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VControlMode
 */
void Test_FAC_InitPipe_Fail_SubscribeVControlMode(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_CONTROL_MODE_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VehicleStatus
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleStatus(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_STATUS_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VGlobalPosition
 */
void Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_GLOBAL_POSITION_MID");
}

/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VLandDetected
 */
void Test_FAC_InitPipe_Fail_SubscribeVLandDetected(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_LAND_DETECTED_MID");
}


/**************************************************************************
 * Tests for FAC InitData()
 **************************************************************************/
/**
 * Test FAC InitData()
 */
void Test_FAC_InitDataNominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 conf_result = -1;
    int32 expected = CFE_SUCCESS;

    /* Initialize the table so the table pointer is set. */
    conf_result = oFAC.InitConfigTbl();

    /* Execute the function being tested */
    if (conf_result == CFE_SUCCESS)
    {
        result = oFAC.InitData();
    }

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for FAC InitApp()
 **************************************************************************/
/**
 * Test FAC InitApp(), fail init event
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
 * Test FAC InitApp(), fail init pipe
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
 * Test FAC InitApp(), fail init data.
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
 * Test FAC InitApp(), fail init config table
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
 * Test FAC InitApp(), Nominal
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
 * Test FAC AppMain(), WriteToSysLogHook
 */
int32 Test_FAC_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buf[256];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s\n", Buf);

    return WriteToSysLog_HookCalledCnt;
}

/**
 * Test FAC AppMain(), SendEventHook
 */
int32 Test_FAC_AppMain_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
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
 * Test FAC AppMain(), Fail RegisterApp
 */
void Test_FAC_AppMain_Fail_RegisterApp(void)
{
    int32 expected = CFE_ES_ERR_APP_REGISTER;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC AppMain(), Fail InitApp
 */
void Test_FAC_AppMain_Fail_InitApp(void)
{
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_FAC_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (WriteToSysLog_HookCalledCnt == 3, "AppMain, Fail_InitApp");
}


/**
 * Test FAC AppMain(), Fail AcquireConfigPtrs
 */
void Test_FAC_AppMain_Fail_AcquireConfigPtrs(void)
{
    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX, (void*)Test_FAC_AppMain_SendEventHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (SendEvent_HookCalledCnt == 3, "AppMain, Fail_AcquireConfigPtrs");
}


/**
 * Test FAC AppMain(), Invalid Schedule Message
 */
void Test_FAC_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate the behavior of receiving a invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, PX4_BATTERY_STATUS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Hook to support: FAC AppMain(), Nominal - SendHK
 */
int32 Test_FAC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */
    unsigned char* pMsg = NULL;
    uint16 len = sizeof(FAC_HkTlm_t);
    int i = 0;

    pMsg = (unsigned char*)MsgPtr;

    printf("###SendHK_SendMsgHook:\n");
    for (i = 0; i < len; i++)
    {
        printf("0x%x ", *pMsg);
        pMsg++;
    }
    printf("\n");

    return CFE_SUCCESS;
}

/**
 * Test FAC AppMain(), Nominal - SendHK
 */
void Test_FAC_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a FAC_SEND_HK_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (((oFAC.HkTlm.SendHkMsgRcvCnt == 1) && (oFAC.HkTlm.HkMsgSndCnt == 1)),
                    "AppMain_Nominal_SendHK");
}


/**
 * Test FAC AppMain(), Nominal - RunController
 */
void Test_FAC_AppMain_Nominal_RunController(void)
{
    /* The following will emulate behavior of receiving a FAC_RUN_CONTROLLER_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Test FAC AppMain(), ProcessNewData IncomingDataHook
 */
int32 Test_FAC_AppMain_ProcessNewData_IncomingDataHook(void *dst, void *src, uint32 size)
{
    unsigned char *pMsg = NULL;
    int i = 0;

    pMsg = (unsigned char*)src;
    printf("###IncomingDataHook: ");
    for (i = 0; i < size; i++)
    {
        printf("0x%x ", *pMsg);
        pMsg ++;
    }
    printf("\n");

    if (oFAC.HkTlm.usDataErrCnt > 0)
    {
        return oFAC.HkTlm.usDataErrCnt;
    }
    else
    {
        return 0;
    }
}

/**
 * Test FAC AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_FAC_AppMain_ProcessNewData_InvalidMsgID(void)
{
    int32 DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(PX4_ActuatorArmedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
#if 1
    UtAssert_True(oFAC.HkTlm.usDataErrCnt == 1, "AppMain_ProcessNewData_InvalidMsgID");
#else
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR, "Recvd invalid DATA msgId (0x0000)", "Error Event Sent");
#endif
}

/**
 * Test FAC AppMain(), ProcessNewData - Airspeed
 */
void Test_FAC_AppMain_ProcessNewData_Airspeed(void)
{
    int32              DataPipe;
    PX4_AirspeedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(PX4_AirspeedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.AirSpeedMsgRcvCnt == 1, "AppMain_ProcessNewData_Airspeed");
}

/**
 * Test FAC AppMain(), ProcessNewData - BatteryStatus
 */
void Test_FAC_AppMain_ProcessNewData_BatteryStatus(void)
{
    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID,
                     sizeof(PX4_BatteryStatusMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.BatteryStatusMsgRcvCnt == 1, "AppMain_ProcessNewData_BatteryStatus");
}

/**
 * Test FAC AppMain(), ProcessNewData - ManualControlSp
 */
void Test_FAC_AppMain_ProcessNewData_ManualControlSp(void)
{
    int32              DataPipe;
    PX4_ManualControlSetpointMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_MANUAL_CONTROL_SETPOINT_MID,
                     sizeof(PX4_ManualControlSetpointMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.ManualControlSpMsgRcvCnt == 1,
                   "AppMain_ProcessNewData_ManualControlSp");
}

/**
 * Test FAC AppMain(), ProcessNewData - VAtt
 */
void Test_FAC_AppMain_ProcessNewData_VAtt(void)
{
    int32              DataPipe;
    PX4_VehicleAttitudeMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_MID,
                     sizeof(PX4_VehicleAttitudeMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VAttMsgRcvCnt == 1, "AppMain_ProcessNewData_VAtt");
}

/**
 * Test FAC AppMain(), ProcessNewData - VAttSp
 */
void Test_FAC_AppMain_ProcessNewData_VAttSp(void)
{
    int32              DataPipe;
    PX4_VehicleAttitudeSetpointMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                     sizeof(PX4_VehicleAttitudeSetpointMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VAttSpMsgRcvCnt == 1, "AppMain_ProcessNewData_VAttSp");
}

/**
 * Test FAC AppMain(), ProcessNewData - VControlMode
 */
void Test_FAC_AppMain_ProcessNewData_VControlMode(void)
{
    int32              DataPipe;
    PX4_VehicleControlModeMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID,
                     sizeof(PX4_VehicleControlModeMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VControlModeMsgRcvCnt == 1,
                   "AppMain_ProcessNewData_VControlMode");
}

/**
 * Test FAC AppMain(), ProcessNewData - VGlobalPosition
 */
void Test_FAC_AppMain_ProcessNewData_VGlobalPosition(void)
{
    int32              DataPipe;
    PX4_VehicleGlobalPositionMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_GLOBAL_POSITION_MID,
                     sizeof(PX4_VehicleGlobalPositionMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VGlobalPositionMsgRcvCnt == 1,
                  "AppMain_ProcessNewData_VGlobalPosition");
}

/**
 * Test FAC AppMain(), ProcessNewData - VLandDetected
 */
void Test_FAC_AppMain_ProcessNewData_VLandDetected(void)
{
    int32              DataPipe;
    PX4_VehicleLandDetectedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LAND_DETECTED_MID,
                     sizeof(PX4_VehicleLandDetectedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VLandDetectedMsgRcvCnt == 1,
                   "AppMain_ProcessNewData_VLandDetected");
}

/**
 * Test FAC AppMain(), ProcessNewData - VehicleStatus
 */
void Test_FAC_AppMain_ProcessNewData_VehicleStatus(void)
{
    int32              DataPipe;
    PX4_VehicleStatusMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_STATUS_MID, sizeof(PX4_VehicleStatusMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessNewData_IncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VehicleStatusMsgRcvCnt == 1,
                  "AppMain_ProcessNewData_VehicleStatus");
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


/**
 * Test FAC UpdateParams(), SendEventHook
 */
int32 Test_FAC_UpdateParams_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];
    FAC_ParamTbl_t *pTbl;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###UpdateParams_SendEventHook:\n");
    printf("%s\n", Buf);

    pTbl = oFAC.ParamTblPtr;

    UpdateParams_ParamChecksum = (
          (double)pTbl->FW_R_TC + (double)pTbl->FW_P_TC +
          (double)pTbl->FW_PR_P + (double)pTbl->FW_PR_I +
          (double)pTbl->FW_P_RMAX_POS + (double)pTbl->FW_P_RMAX_NEG +
          (double)pTbl->FW_PR_IMAX +
          (double)pTbl->FW_RR_P + (double)pTbl->FW_RR_I +
          (double)pTbl->FW_RR_IMAX + (double)pTbl->FW_R_RMAX +
          (double)pTbl->FW_YR_P + (double)pTbl->FW_YR_I +
          (double)pTbl->FW_YR_IMAX + (double)pTbl->FW_Y_RMAX +
          (double)pTbl->FW_RLL_TO_YAW_FF +
          (double)pTbl->FW_W_EN + (double)pTbl->FW_WR_P +
          (double)pTbl->FW_WR_I + (double)pTbl->FW_WR_IMAX + (double)pTbl->FW_W_RMAX +
          (double)pTbl->FW_RR_FF + (double)pTbl->FW_PR_FF +
          (double)pTbl->FW_YR_FF + (double)pTbl->FW_WR_FF +
          (double)pTbl->FW_YCO_VMIN + (double)pTbl->FW_YCO_METHOD +
          (double)pTbl->FW_RSP_OFF + (double)pTbl->FW_PSP_OFF +
          (double)pTbl->FW_MAN_R_MAX + (double)pTbl->FW_MAN_P_MAX +
          (double)pTbl->FW_FLAPS_SCL + (double)pTbl->FW_FLAPERON_SCL +
          (double)pTbl->FW_ARSP_MODE +
          (double)pTbl->FW_MAN_R_SC + (double)pTbl->FW_MAN_P_SC + (double)pTbl->FW_MAN_Y_SC +
          (double)pTbl->FW_BAT_SCALE_EN +
          (double)pTbl->FW_ACRO_X_MAX + (double)pTbl->FW_ACRO_Y_MAX + (double)pTbl->FW_ACRO_Z_MAX +
          (double)pTbl->FW_RATT_TH +
          (double)pTbl->FW_AIRSPD_MIN + (double)pTbl->FW_AIRSPD_MAX + (double)pTbl->FW_AIRSPD_TRIM +
          (double)pTbl->TRIM_ROLL + (double)pTbl->TRIM_PITCH + (double)pTbl->TRIM_YAW +
          (double)pTbl->VT_TYPE);
    std::cout.precision(17);
    std::cout << std::endl << "Params Checksum(with Max precision): "
              << UpdateParams_ParamChecksum << std::endl;

    std::cout.precision(7);
    std::cout << std::endl << "Param Table Values:" << std::endl;
    std::cout << "FW_R_TC: " << pTbl->FW_R_TC << ", FW_P_TC: " << pTbl->FW_P_TC << std::endl;
    std::cout << "FW_PR_P: " << pTbl->FW_PR_P << ", FW_PR_I: " << pTbl->FW_PR_I << std::endl;
    std::cout << "FW_P_RMAX_POS: " << pTbl->FW_P_RMAX_POS << ", FW_P_RMAX_NEG: "
              << pTbl->FW_P_RMAX_NEG << std::endl;
    std::cout << "FW_PR_IMAX: " << pTbl->FW_PR_IMAX << std::endl;
    std::cout << "FW_RR_P: " << pTbl->FW_RR_P << ", FW_RR_I: " << pTbl->FW_RR_I << std::endl;
    std::cout << "FW_RR_IMAX: " << pTbl->FW_RR_IMAX << ", FW_R_RMAX: " << pTbl->FW_R_RMAX
              << std::endl;
    std::cout << "FW_YR_P: " << pTbl->FW_YR_P << ", FW_YR_I: " << pTbl->FW_YR_I << std::endl;
    std::cout << "FW_YR_IMAX: " << pTbl->FW_YR_IMAX << ", FW_Y_RMAX: " << pTbl->FW_Y_RMAX
              << std::endl;
    std::cout << "FW_RLL_TO_YAW_FF: " << pTbl->FW_RLL_TO_YAW_FF << std::endl;
    std::cout << "FW_W_EN: " << pTbl->FW_W_EN << ", FW_WR_P: " << pTbl->FW_WR_P << std::endl;
    std::cout << "FW_WR_I: " << pTbl->FW_WR_I << ", FW_WR_IMAX: " << pTbl->FW_WR_IMAX
              << ", FW_W_RMAX: " << pTbl->FW_W_RMAX << std::endl;
    std::cout << "FW_RR_FF: " << pTbl->FW_RR_FF << ", FW_PR_FF: " << pTbl->FW_PR_FF
              << std::endl;
    std::cout << "FW_YR_FF: " << pTbl->FW_YR_FF << ", FW_WR_FF: " << pTbl->FW_WR_FF
              << std::endl;
    std::cout << "FW_YCO_VMIN: " << pTbl->FW_YCO_VMIN << ", FW_YCO_METHOD: "
              << pTbl->FW_YCO_METHOD << std::endl;
    std::cout << "FW_RSP_OFF: " << pTbl->FW_RSP_OFF << ", FW_PSP_OFF: " << pTbl->FW_PSP_OFF
              << std::endl;
    std::cout << "FW_MAN_R_MAX: " << pTbl->FW_MAN_R_MAX << ", FW_MAN_P_MAX: "
              << pTbl->FW_MAN_P_MAX << std::endl;
    std::cout << "FW_FLAPS_SCL: " << pTbl->FW_FLAPS_SCL << ", FW_FLAPERON_SCL: "
              << pTbl->FW_FLAPERON_SCL << std::endl;
    std::cout << "FW_ARSP_MODE: " << pTbl->FW_ARSP_MODE << std::endl;
    std::cout << "FW_MAN_R_SC: " << pTbl->FW_MAN_R_SC << ", FW_MAN_P_SC: "
              << pTbl->FW_MAN_P_SC << ", FW_MAN_Y_SC: " << pTbl->FW_MAN_Y_SC << std::endl;
    std::cout << "FW_BAT_SCALE_EN: " << pTbl->FW_BAT_SCALE_EN << std::endl;
    std::cout << "FW_ACRO_X_MAX: " << pTbl->FW_ACRO_X_MAX << ", FW_ACRO_Y_MAX: "
              << pTbl->FW_ACRO_Y_MAX << ", FW_ACRO_Z_MAX: " << pTbl->FW_ACRO_Z_MAX
              << std::endl;
    std::cout << "FW_RATT_TH: " << pTbl->FW_RATT_TH << std::endl;
    std::cout << "FW_AIRSPD_MIN: " << pTbl->FW_AIRSPD_MIN << ", FW_AIRSPD_MAX: "
              << pTbl->FW_AIRSPD_MAX << ", FW_AIRSPD_TRIM: " << pTbl->FW_AIRSPD_TRIM
              << std::endl;
    std::cout << "TRIM_ROLL: " << pTbl->TRIM_ROLL << ", TRIM_PITCH: " << pTbl->TRIM_PITCH
              << ", TRIM_YAW: " << pTbl->TRIM_YAW << std::endl;
    std::cout << "VT_TYPE: " << pTbl->VT_TYPE << std::endl;

    printf("\n###Updated ECL Values:\n");
    FAC_Test_PrintEclValues();

    UpdateParams_ValidateStatus = FAC_Test_ValidateEclValues();
}

/**
 * Test FAC UpdateParams(), UpdateParams
 */
void Test_FAC_UpdateParams(void)
{
    UpdateParams_ParamChecksum = 0.0;
    UpdateParams_ValidateStatus = 0x0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_FAC_UpdateParams_SendEventHook);

    printf("###Initial ECL Values:\n");
    FAC_Test_PrintEclValues();

    /* Execute the function being tested */
    oFAC.InitApp();

    /* Verify results */
    if ((UpdateParams_ValidateStatus == 0x0) &&
        (fabs(UpdateParams_ParamChecksum - (double)1369.37) <= FLT_EPSILON)) // Fail with DBL_EPSILON
    {
        UtAssert_True(TRUE, "FAC UpdateParams");
    }
    else
    {
        UtAssert_True(FALSE, "FAC UpdateParams");
    }
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
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeSendHK, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeRunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeRunController");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateCMDPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeCMD, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateDATAPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeBatteryStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeBatteryStatus");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeManualControlSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeManualControlSp");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVAttSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVAttSp");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVAtt, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVAtt");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVControlMode, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVControlMode");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVehicleStatus");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVLandDetected, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVLandDetected");

    UtTest_Add(Test_FAC_InitDataNominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitDataNominal");

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
    UtTest_Add(Test_FAC_AppMain_Nominal_RunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Nominal_RunController");

    UtTest_Add(Test_FAC_AppMain_ProcessNewData_InvalidMsgID, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_InvalidMsgID");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_Airspeed, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_Airspeed");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_BatteryStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_BatteryStatus");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_ManualControlSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_ManualControlSp");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VAtt, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VAtt");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VAttSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VAttSp");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VControlMode, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VControlMode");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VGlobalPosition, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VGlobalPosition");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VLandDetected, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VLandDetected");
    UtTest_Add(Test_FAC_AppMain_ProcessNewData_VehicleStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_ProcessNewData_VehicleStatus");

    UtTest_Add(Test_FAC_RunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_RunController");
    UtTest_Add(Test_FAC_ControlAttitude, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ControlAttitude");
    UtTest_Add(Test_FAC_UpdateParams, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_UpdateParams");

}


