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

#include "sch_app_test.h"
#include "sch_test_utils.h"
#include "sch_custom_stubs.h"
#include "sch_app.h"
#include "sch_apipriv.h"
#include "sch_msg.h"
#include "sch_events.h"
#include "sch_version.h"

#include "uttest.h"
#include "utassert.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"


/*
 * Wrapper variables
 */
int OS_RUNTIME_MODE;
int MINOR_FRAME;

/*
 * Hook functions to support
 */
int32 SCH_APP_TEST_CFE_TIME_RegisterSynchCallbackHook(
                            CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr)
{
    SCH_AppData.IgnoreMajorFrame      = TRUE;
    SCH_AppData.MinorFramesSinceTone  = 1;

    return CFE_SUCCESS;
}


int32 SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook(
                             CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr)
{
    SCH_AppData.AppID = 99;

    return CFE_SUCCESS;
}


int32 SCH_APP_TEST_OS_TimerCreateHook(
              uint32 *timer_id, const char *timer_name,
              uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr)
{
    SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY + 1;

    return CFE_SUCCESS;
}


int32 SCH_APP_TEST_CFE_SB_RcvMsgHook(CFE_SB_MsgPtr_t *BufPtr,
                                CFE_SB_PipeId_t PipeId, int32 TimeOut)
{
    return CFE_SB_NO_MESSAGE;
}

int32 SCH_APP_TEST_TimerSetHook(uint32 timer_id, uint32 start_time,
                                uint32 interval_time)
{
    SCH_AppData.NextSlotNumber = 10;

    return CFE_SUCCESS;
}


/**
 * SCH_AppMain, Test_RegisterAppError
 */
void SCH_AppMain_Test_RegisterAppError(void)
{
    int32  Expected = CFE_ES_ERR_APP_REGISTER;
    char   expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, Expected, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                  (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();

    sprintf(expSysLog, "SCH App terminating, err = 0x%08X\n",
                       (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain_Test_RegisterAppError SysLog Written");
}


/**
 * SCH_AppMain, Test_AppInitError
 */
void SCH_AppMain_Test_AppInitError(void)
{
    int32  Expected = CFE_ES_ERR_APPNAME;
    char   expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETAPPID_INDEX, Expected, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                  (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();

    sprintf(expSysLog, "SCH App: Unable to obtain own AppID, RC=0x%08X\n",
                       (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 2,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 2");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain_Test_AppInitError SysLog Written");
}


/**
 * SCH_AppMain, Test_CustomLateInitError
 */
void SCH_AppMain_Test_CustomLateInitError(void)
{
    int32  Expected = CFE_TIME_TOO_MANY_SYNCH_CALLBACKS;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX,
                              Expected, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                  (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();

    sprintf(expEventText, "Error initializing Timers (RC=0x%08X)",
                          (unsigned int)Expected);
    sprintf(expSysLog, "SCH App terminating, err = 0x%08X\n",
                       (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_MAJOR_FRAME_SUB_ERR_EID, CFE_EVS_ERROR,
              expEventText, "AppMain_Test_CustomLateInitError Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain_Test_CustomLateInitError SysLog Written");
}


/**
 * SCH_AppMain, Test_NoisyMajorFrameError
 */
void SCH_AppMain_Test_NoisyMajorFrameError(void)
{
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.IgnoreMajorFrameMsgSent = FALSE;

    /* Causes SCH_CustomLateInit to set SCH_AppData.IgnoreMajorFrame = TRUE,
       which it would normally do if CFE_TIME_RegisterSynchCallback weren't
       stubbed out. Also sets SCH_AppData.MinorFramesSinceTone = 1. Setting
       these manually in the test setup doesn't work, because they get reset
       at a certain point */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX,
                     (void*)&SCH_APP_TEST_CFE_TIME_RegisterSynchCallbackHook);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of
       the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                   (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    SCH_AppMain();

    sprintf(expEventText,
          "Major Frame Sync too noisy (Slot %d). Disabling synchronization.",
          (int)SCH_AppData.MinorFramesSinceTone);

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == TRUE,
                               "SCH_AppData.IgnoreMajorFrameMsgSent == TRUE");

#if SCH_LIB_PRESENCE == 1
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");
#else
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");
#endif

    UtAssert_EventSent(SCH_NOISY_MAJOR_FRAME_ERR_EID, CFE_EVS_ERROR,
             expEventText, "AppMain_Test_NoisyMajorFrameError Event Sent");
}


/**
 * SCH_AppMain, Test_NominalIgnoreMajorFrameFalse
 */
void SCH_AppMain_Test_NominalIgnoreMajorFrameFalse(void)
{
    SCH_AppData.IgnoreMajorFrameMsgSent = TRUE;

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                 (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    SCH_AppMain();

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE,
                            "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");
}


/**
 * SCH_AppMain, Test_ProcessScheduleTableError
 */
void SCH_AppMain_Test_ProcessScheduleTableError(void)
{
    int32   Expected = CFE_SB_PIPE_RD_ERR;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

#if SCH_LIB_PRESENCE == 1
    /* Set to pass condition "SCH_GetProcessingState() == TRUE" */
    SCH_LibData.ProcessingDisabledCtr = 0;
#endif

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 10;
    SCH_AppData.NextSlotNumber       = SCH_TIME_SYNC_SLOT;
    SCH_AppData.LastProcessCount     = 1;

    /* Sets SCH_AppData.NextSlotNumber = 10, in order to reach
       SCH_ProcessCommands */
    Ut_OSTIMER_SetFunctionHook(UT_OSTIMER_SET_INDEX,
                               (void*)&SCH_APP_TEST_TimerSetHook);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                 (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, Expected, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    SCH_AppMain();

    sprintf(expEventText, "SCH App: terminating, err = 0x%08X",
                          (unsigned int)Expected);
    sprintf(expSysLog, "SCH App terminating, err = 0x%08X\n",
                           (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 5,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 5");

    UtAssert_EventSent(SCH_APP_EXIT_EID, CFE_EVS_CRITICAL, expEventText,
                       "AppMain_Test_ProcessScheduleTableError Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain_Test_ProcessScheduleTableError SysLog Written");
}


/**
 * SCH_AppMain, Test_Nominal
 */
void SCH_AppMain_Test_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end
       of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
                 (void*)&SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();

    /* Verify results */
    UtAssert_True(SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");
}


/**
 * SCH_AppInit, Test_GetAppIDError
 */
void SCH_AppInit_Test_GetAppIDError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_ES_ERR_APPNAME;
    char    expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETAPPID_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expSysLog, "SCH App: Unable to obtain own AppID, RC=0x%08X\n",
                       (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "AppInit_Test_GetAppIDError");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppInit_Test_GetAppIDError SysLog Written");
}


/**
 * SCH_AppInit, Test_EvsInitError
 */
void SCH_AppInit_Test_EvsInitError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_EVS_APP_NOT_REGISTERED;
    char    expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expSysLog,
            "SCH App: Error Registering For Event Services, RC=0x%08X\n",
            (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "AppInit_Test_EvsInitError");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppInit_Test_EvsInitError SysLog Written");
}


/**
 * SCH_AppInit, Test_SbInitError
 */
void SCH_AppInit_Test_SbInitError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SB_BAD_ARGUMENT;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expEventText, "Error Creating SB Pipe, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "AppInit_Test_SbInitError");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_EventSent(SCH_CR_PIPE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "AppInit_Test_SbInitError Event Sent");
}


/**
 * SCH_AppInit, Test_TblInitError
 */
void SCH_AppInit_Test_TblInitError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_INVALID_NAME;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expEventText, "Error Registering SDT, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "AppInit_Test_TblInitError");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_EventSent(SCH_SDT_REG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "AppInit_Test_TblInitError Event Sent");
}


/**
 * SCH_AppInit, Test_TimerInitError
 */
void SCH_AppInit_Test_TimerInitError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_ES_BIN_SEM_DELETE_ERR;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_BINSEMCREATE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expEventText,
            "Error creating Main Loop Timing Semaphore (RC=0x%08X)",
            (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "AppInit_Test_TimerInitError");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");

    UtAssert_EventSent(SCH_SEM_CREATE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "AppInit_Test_TimerInitError Event Sent");
}


/**
 * SCH_AppInit, Test_ChildTaskInitError
 */
void SCH_AppInit_Test_ChildTaskInitError(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_ES_ERR_TASKID;
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, expected, 1);

    /* Execute the function being tested */
    result = SCH_AppInit();

    sprintf(expEventText, "AD child task create failed - err = 0x%08X",
                          (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "AppInit_Test_ChildTaskInitError");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");

    UtAssert_EventSent(SCH_AD_CHILD_TASK_CREATE_ERR_EID, CFE_EVS_ERROR,
             expEventText, "AppInit_Test_ChildTaskInitError Event Sent");
}


/**
 * SCH_AppInit, Test_Nominal
 */
void SCH_AppInit_Test_Nominal(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Execute the function being tested */
    Result = SCH_AppInit();

    sprintf(expEventText, "SCH Initialized. Version %d.%d.%d.%d",
                          SCH_MAJOR_VERSION, SCH_MINOR_VERSION,
                          SCH_REVISION, SCH_MISSION_REV);

    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0,
                  "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount == 0,
                  "SCH_AppData.SkippedSlotsCount == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount == 0,
                  "SCH_AppData.MultipleSlotsCount == 0");
    UtAssert_True(SCH_AppData.SameSlotCount == 0,
                  "SCH_AppData.SameSlotCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0,
                  "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0,
                  "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0,
                  "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0,
                  "SCH_AppData.ErrCounter == 0");

    UtAssert_True(Result == Expected, "AppInit_Test_Nominal");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");

    UtAssert_EventSent(SCH_INITSTATS_INF_EID, CFE_EVS_INFORMATION,
                       expEventText, "AppInit_Test_Nominal Event Sent");
}


/**
 * SCH_EvsInit, Test_RegisterError
 */
void SCH_EvsInit_Test_RegisterError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_EVS_APP_NOT_REGISTERED;
    char    expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_EvsInit();

    sprintf(expSysLog,
            "SCH App: Error Registering For Event Services, RC=0x%08X\n",
            (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "EvsInit_Test_RegisterError");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
        "SCH App: EvsInit_Test_RegisterError, SysLog Written");
}


/**
 * SCH_EvsInit, Test_Nominal
 */
void SCH_EvsInit_Test_Nominal(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    SCH_AppData.EventFilters[0].EventID = 1;
    SCH_AppData.EventFilters[0].Mask    = 2;

    SCH_AppData.EventFilters[1].EventID = 3;
    SCH_AppData.EventFilters[1].Mask    = 4;

    SCH_AppData.EventFilters[2].EventID = 5;
    SCH_AppData.EventFilters[2].Mask    = 6;

    SCH_AppData.EventFilters[3].EventID = 7;
    SCH_AppData.EventFilters[3].Mask    = 8;

    SCH_AppData.EventFilters[4].EventID = 9;
    SCH_AppData.EventFilters[4].Mask    = 10;

    /* Execute the function being tested */
    Result = SCH_EvsInit();

    /* Verify results */
    UtAssert_True(SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID,
                 "SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID");
    UtAssert_True(SCH_AppData.EventFilters[0].Mask == CFE_EVS_FIRST_ONE_STOP,
               "SCH_AppData.EventFilters[0].Mask == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True(SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID,
               "SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID");
    UtAssert_True(SCH_AppData.EventFilters[1].Mask == CFE_EVS_FIRST_ONE_STOP,
               "SCH_AppData.EventFilters[1].Mask == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True(SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID,
             "SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID");
    UtAssert_True(SCH_AppData.EventFilters[2].Mask == CFE_EVS_FIRST_ONE_STOP,
               "SCH_AppData.EventFilters[2].Mask == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True(SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID,
                "SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID");
    UtAssert_True(SCH_AppData.EventFilters[3].Mask == CFE_EVS_FIRST_TWO_STOP,
               "SCH_AppData.EventFilters[3].Mask == CFE_EVS_FIRST_TWO_STOP");
    UtAssert_True(SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID,
               "SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID");
    UtAssert_True(SCH_AppData.EventFilters[4].Mask == CFE_EVS_FIRST_ONE_STOP,
               "SCH_AppData.EventFilters[4].Mask == CFE_EVS_FIRST_ONE_STOP");

    UtAssert_True(Result == Expected, "EvsInit_Test_Nominal");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                                     "Ut_CFE_ES_GetSysLogQueueDepth() == 0");
}


/**
 * SCH_SbInit, Test_CreateCmdPipeError
 */
void SCH_SbInit_Test_CreateCmdPipeError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SB_BAD_ARGUMENT;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_SbInit();

    sprintf(expEventText, "Error Creating SB Pipe, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "SbInit_Test_CreateCmdPipeError");

    UtAssert_EventSent(SCH_CR_PIPE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "SbInit_Test_CreateCmdPipeError Event Sent");
}


/**
 * SCH_SbInit, Test_CreateADPipeError
 */
void SCH_SbInit_Test_CreateADPipeError(void)
{
    /* Set a fail result for SB */
    int32  result = CFE_SUCCESS;
    int32  expected = CFE_SB_BAD_ARGUMENT;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = SCH_SbInit();

    sprintf(expEventText, "Error Creating SB Pipe, RC=0x%08lX", expected);

    /* Verify results */
    UtAssert_True(result == expected, "SbInit_Test_CreateADPipeError");

    UtAssert_EventSent(SCH_CR_PIPE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "SbInit_Test_CreateADPipeError Event Sent");
}


/**
 * SCH_SbInit, Test_SubscribeHKError
 */
void SCH_SbInit_Test_SubscribeHKError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SB_BAD_ARGUMENT;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_SbInit();

    sprintf(expEventText,
            "Error Subscribing to HK Request(MID=0x%04X), RC=0x%08X",
            SCH_SEND_HK_MID, (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "SbInit_Test_SubscribeHKError");

    UtAssert_EventSent(SCH_SUB_HK_REQ_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "SbInit_Test_SubscribeHKError Even Sent");
}


/**
 * SCH_SbInit, Test_SubscribeGNDError
 */
void SCH_SbInit_Test_SubscribeGNDError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SB_BAD_ARGUMENT;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, Expected, 2);

    /* Execute the function being tested */
    Result = SCH_SbInit();

    sprintf(expEventText,
            "Error Subscribing to GND CMD(MID=0x%04X), RC=0x%08X",
            SCH_CMD_MID, (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "SbInit_Test_SubscribeGNDError");

    UtAssert_EventSent(SCH_SUB_GND_CMD_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "SbInit_Test_SubscribeGNDError Event Sent");
}


/**
 * SCH_SbInit, Test_SubscribeActivityDoneError
 */
void SCH_SbInit_Test_SubscribeActivityDoneError(void)
{
    /* Set a fail result for SB */
    int32  result = CFE_SUCCESS;
    int32  expected = CFE_SB_BAD_ARGUMENT;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = SCH_SbInit();

    sprintf(expEventText,
            "Error Subscribing to activity done msg(MID=0x%04X), RC=0x%08X",
            SCH_ACTIVITY_DONE_MID, (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "SbInit_Test_SubscribeActivityDoneError");

    UtAssert_EventSent(SCH_SUB_GND_CMD_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "SbInit_Test_SubscribeActivityDoneError Event Sent");
}


/**
 * SCH_SbInit, Test_Nominal
 */
void SCH_SbInit_Test_Nominal(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) 1;
    SCH_AppData.CmdPipe = 1;

    /* Execute the function being tested */
    Result = SCH_SbInit();

    /* Verify results */
    UtAssert_True(SCH_AppData.MsgPtr == (CFE_SB_MsgPtr_t) 0,
                               "SCH_AppData.MsgPtr == (CFE_SB_MsgPtr_t) 0");
    UtAssert_True(SCH_AppData.CmdPipe == 0,
                               "SCH_AppData.CmdPipe == 0");

    UtAssert_True(Result == Expected, "SbInit_Test_Nominal");
}


/**
 * SCH_TimerInit, Test_CustomEarlyInitError
 */
void SCH_TimerInit_Test_CustomEarlyInitError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = -1;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_OSTIMER_SetReturnCode(UT_OSTIMER_CREATE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_TimerInit();

    sprintf(expEventText, "Error creating Timer (RC=0x%08X)",
                          (unsigned int)-1);

    /* Verify results */
    UtAssert_True(Result == Expected, "TimerInit_Test_CustomEarlyInitError");

    UtAssert_EventSent(SCH_MINOR_FRAME_TIMER_CREATE_ERR_EID, CFE_EVS_ERROR,
             expEventText, "TimerInit_Test_CustomEarlyInitError Event Sent");
}


/**
 * SCH_TimerInit, Test_TimerAccuracyWarning
 */
void SCH_TimerInit_Test_TimerAccuracyWarning(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.SyncToMET = 4;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Satisfies condition
       "SCH_AppData.ClockAccuracy > SCH_WORST_CLOCK_ACCURACY" */
    Ut_OSTIMER_SetFunctionHook(UT_OSTIMER_CREATE_INDEX,
                               (void*)&SCH_APP_TEST_OS_TimerCreateHook);

    /* Execute the function being tested */
    Result = SCH_TimerInit();

    sprintf(expEventText,
      "OS Timer Accuracy (%d > reqd %d usec) requires Minor Frame MET sync",
      (int)SCH_AppData.ClockAccuracy, SCH_WORST_CLOCK_ACCURACY);

    /* Verify results */
    UtAssert_True(SCH_AppData.ClockAccuracy == SCH_WORST_CLOCK_ACCURACY + 1,
               "SCH_AppData.ClockAccuracy == SCH_WORST_CLOCK_ACCURACY + 1");

    UtAssert_True(SCH_AppData.SyncToMET == SCH_MINOR_SYNCHRONIZED,
                  "SCH_AppData.SyncToMET == SCH_MINOR_SYNCHRONIZED");

    UtAssert_True
        (SCH_AppData.WorstCaseSlotsPerMinorFrame ==
         ((SCH_AppData.ClockAccuracy * 2) / SCH_NORMAL_SLOT_PERIOD) + 1,
         "SCH_AppData.WorstCaseSlotsPerMinorFrame == "
         "((SCH_AppData.ClockAccuracy * 2) / SCH_NORMAL_SLOT_PERIOD) + 1");

    UtAssert_True(Result == Expected, "TimerInit_Test_TimerAccuracyWarning");

    UtAssert_EventSent(SCH_MINOR_FRAME_TIMER_ACC_WARN_EID, CFE_EVS_INFORMATION,
             expEventText, "TimerInit_Test_TimerAccuracyWarning Event Sent");
}


/**
 * SCH_TimerInit, Test_BinSemCreateError
 */
void SCH_TimerInit_Test_BinSemCreateError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_ES_BIN_SEM_DELETE_ERR;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_BINSEMCREATE_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_TimerInit();

    sprintf(expEventText,
            "Error creating Main Loop Timing Semaphore (RC=0x%08X)",
            (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "TimerInit_Test_BinSemCreateError");

    UtAssert_EventSent(SCH_SEM_CREATE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TimerInit_Test_BinSemCreateError Event Sent");
}


/**
 * SCH_TimerInit, Test_Nominal
 */
void SCH_TimerInit_Test_Nominal(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    SCH_AppData.IgnoreMajorFrame     = 1;
    SCH_AppData.IgnoreMajorFrameMsgSent = 2;
    SCH_AppData.UnexpectedMajorFrame = 3;
    SCH_AppData.SyncToMET            = 4;
    SCH_AppData.MajorFrameSource     = 5;
    SCH_AppData.NextSlotNumber       = 6;
    SCH_AppData.MinorFramesSinceTone = 7;
    SCH_AppData.LastSyncMETSlot      = 8;
    SCH_AppData.SyncAttemptsLeft     = 9;
    SCH_AppData.UnexpectedMajorFrameCount   = 10;
    SCH_AppData.MissedMajorFrameCount       = 11;
    SCH_AppData.ValidMajorFrameCount        = 12;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Execute the function being tested */
    Result = SCH_TimerInit();

    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame == FALSE,
                                    "SCH_AppData.IgnoreMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE,
                             "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE,
                                "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.SyncToMET == SCH_NOT_SYNCHRONIZED,
                            "SCH_AppData.SyncToMET == SCH_NOT_SYNCHRONIZED");
    UtAssert_True(SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_NONE,
                        "SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_NONE");
    UtAssert_True(SCH_AppData.NextSlotNumber == 0,
                                          "SCH_AppData.NextSlotNumber == 0");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT,
                   "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");
    UtAssert_True(SCH_AppData.LastSyncMETSlot == 0,
                                         "SCH_AppData.LastSyncMETSlot == 0");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft == 0,
                                        "SCH_AppData.SyncAttemptsLeft == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount == 0,
                               "SCH_AppData.UnexpectedMajorFrameCount == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount == 0,
                                   "SCH_AppData.MissedMajorFrameCount == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount == 0,
                                    "SCH_AppData.ValidMajorFrameCount == 0");
    UtAssert_True(SCH_AppData.WorstCaseSlotsPerMinorFrame == 1,
                             "SCH_AppData.WorstCaseSlotsPerMinorFrame == 1");
    UtAssert_True(SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY,
                     "SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY");

    UtAssert_True (Result == Expected, "TimerInit_Test_Nominal");
}


/**
 * SCH_ProcessScheduleTable, Test_ProcessCount2LastProcessCount1
 */
void SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 2;
    SCH_AppData.NextSlotNumber       = 1;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == 2,
                  "SCH_AppData.LastProcessCount == 2");

    UtAssert_True(Result == Expected,
             "ProcessScheduleTable_Test_ProcessCount2LastProcessCount1");
}


/**
 * SCH_ProcessScheduleTable, Test_ProcessCountTotalSlotsLastProcessCountNotSame
 */
void SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_TOTAL_SLOTS - 1;
    SCH_AppData.NextSlotNumber       = 0;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS,
                  "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS");

    UtAssert_True(Result == Expected,
      "ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame");
}


/**
 * SCH_ProcessScheduleTable, Test_ProcessCountTotalSlotsLastProcessCountSame
 */
void SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_TOTAL_SLOTS - 1;
    SCH_AppData.NextSlotNumber       = 0;
    SCH_AppData.LastProcessCount     = SCH_TOTAL_SLOTS;

    sprintf(expEventText, "Slot did not increment: slot = %d",
			  SCH_AppData.MinorFramesSinceTone);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS,
                  "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS");

    UtAssert_True(SCH_AppData.SameSlotCount == 1,
                  "SCH_AppData.SameSlotCount == 1");

    UtAssert_True(Result == Expected, "ProcessScheduleTable_Test_"
                           "ProcessCountTotalSlotsLastProcessCountSame");

    UtAssert_EventSent(SCH_SAME_SLOT_EID, CFE_EVS_DEBUG, expEventText,
      "ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame"
      " Event Sent");
}


/**
 * SCH_ProcessScheduleTable, Test_ProcessCountOtherAndNoRollover
 */
void SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.ProcessCountOtherAndNoRollover */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 0;
    SCH_AppData.NextSlotNumber       = SCH_TOTAL_SLOTS;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == 1,
                  "SCH_AppData.LastProcessCount == 1");

    UtAssert_True(Result == Expected,
             "ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover");
}


/**
 * SCH_ProcessScheduleTable, Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands
 */
void SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Cause SCH_CustomGetCurrentSlotNumber to return
       SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_MAX_LAG_COUNT + 1;
    SCH_AppData.NextSlotNumber       = SCH_MAX_LAG_COUNT + 3;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    sprintf(expEventText, "Slots skipped: slot = %d, count = %d",
                    (int)SCH_AppData.NextSlotNumber, SCH_TOTAL_SLOTS - 2);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS - 1,
                   "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS - 1");

    UtAssert_True(SCH_AppData.SkippedSlotsCount == 1,
                  "SCH_AppData.SkippedSlotsCount == 1");

    UtAssert_True(SCH_AppData.TablePassCount == 1,
                  "SCH_AppData.TablePassCount == 1");

    /* Note: SCH_AppData.NextSlotNumber is incremented by 1 in
       SCH_ProcessNextSlot */
    UtAssert_True(SCH_AppData.NextSlotNumber == SCH_MAX_LAG_COUNT + 2,
                  "SCH_AppData.NextSlotNumber == SCH_MAX_LAG_COUNT + 2");

    UtAssert_True(Result == Expected, "ProcessScheduleTable_Test_Skipped"
              "SlotsErrorIncrementTablePassCountAndCallProcessCommands");

    UtAssert_EventSent(SCH_SKIPPED_SLOTS_EID, CFE_EVS_ERROR,
             expEventText, "ProcessScheduleTable_Test_SkippedSlotsError"
             "IncrementTablePassCountAndCallProcessCommands Event Sent");
}


/**
 * SCH_ProcessScheduleTable, Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup
 */
void SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.SyncToMET = SCH_TOTAL_SLOTS - 1;

    SCH_AppData.NextSlotNumber = SCH_TOTAL_SLOTS - SCH_MAX_LAG_COUNT + 1;

    SCH_AppData.WorstCaseSlotsPerMinorFrame = 1;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    sprintf(expEventText, "Multiple slots processed: slot = %d, count = %d",
                 (int)SCH_AppData.NextSlotNumber, SCH_MAX_SLOTS_PER_WAKEUP);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT,
                  "SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT");

    UtAssert_True(SCH_AppData.MultipleSlotsCount == 1,
                  "SCH_AppData.MultipleSlotsCount == 1");

    UtAssert_True(Result == Expected, "ProcessScheduleTable_Test_MultiSlots"
                       "ProcessCountTooLargeSynchronizedProcessCountGreater"
                                                   "ThanMaxSlotsPerWakeup");

    UtAssert_EventSent(SCH_MULTI_SLOTS_EID, CFE_EVS_INFORMATION,
             expEventText, "ProcessScheduleTable_Test_MultiSlotsProcessCount"
               "TooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup"
               " Event Sent");
}


/**
 * SCH_ProcessScheduleTable, Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup
 */
void SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_MAX_LAG_COUNT - 1;
    SCH_AppData.NextSlotNumber       = SCH_TOTAL_SLOTS;

    SCH_AppData.WorstCaseSlotsPerMinorFrame = SCH_TOTAL_SLOTS;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    sprintf(expEventText, "Multiple slots processed: slot = %d, count = %d",
                          SCH_TOTAL_SLOTS, SCH_MAX_SLOTS_PER_WAKEUP);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();

    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT,
                  "SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT");

    UtAssert_True(SCH_AppData.MultipleSlotsCount == 1,
                  "SCH_AppData.MultipleSlotsCount == 1");

    UtAssert_True(Result == Expected, "ProcessScheduleTable_Test_MultiSlots"
                 "NotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup");

    UtAssert_EventSent(SCH_MULTI_SLOTS_EID, CFE_EVS_INFORMATION,
             expEventText, "ProcessScheduleTable_Test_MultiSlotsNot"
             "SynchronizedProcessCountGreaterThanMaxSlotsPerWakeup"
             " Event Sent");
}


/**
 * SCH_ProcessNextSlot, Test_ProcessCommandsRollover
 */
void SCH_ProcessNextSlot_Test_ProcessCommandsRollover(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    int32   SlotIndex;

    SCH_AppData.NextSlotNumber = SCH_TIME_SYNC_SLOT;
    SlotIndex = SCH_AppData.NextSlotNumber * SCH_ENTRIES_PER_SLOT;
    SCH_AppData.ScheduleTable[SlotIndex].EnableState = SCH_ENABLED;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessNextSlot();

    /* Verify results */
    UtAssert_True(SCH_AppData.NextSlotNumber == 0,
                  "SCH_AppData.NextSlotNumber == 0");
    UtAssert_True(SCH_AppData.TablePassCount == 1,
                  "SCH_AppData.TablePassCount == 1");
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 1,
                  "SCH_AppData.SlotsProcessedCount == 1");

    UtAssert_True(Result == Expected,
                  "ProcessNextSlot_Test_ProcessCommandsRollover");

    /* Generates 2 event message we don't care about in this test */
}


/**
 * SCH_ProcessNextSlot, Test_DoNotProcessCommandsNoRollover
 */
void SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    int32   SlotIndex;

    SCH_AppData.NextSlotNumber = 1;
    SlotIndex = SCH_AppData.NextSlotNumber * SCH_ENTRIES_PER_SLOT;
    SCH_AppData.ScheduleTable[SlotIndex].EnableState = SCH_ENABLED;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessNextSlot();

    /* Verify results */
    UtAssert_True(SCH_AppData.NextSlotNumber == 2,
                  "SCH_AppData.NextSlotNumber == 2");

    UtAssert_True(SCH_AppData.SlotsProcessedCount == 1,
                  "SCH_AppData.SlotsProcessedCount == 1");

    UtAssert_True(Result == Expected,
                  "ProcessNextSlot_Test_DoNotProcessCommandsNoRollover");

    /* Generates 2 event message we don't care about in this test */
}


/**
 * SCH_ProcessNextEntry, Test_CorruptMessageIndex
 */
void SCH_ProcessNextEntry_Test_CorruptMessageIndex(void)
{
    int32 EntryNumber = 1;
    char  expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES;

    SCH_AppData.ScheduleTable[EntryNumber].Frequency = 99;
    SCH_AppData.ScheduleTable[EntryNumber].Type = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 1;

    SCH_AppData.NextSlotNumber = 2;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    sprintf(expEventText1, "Corrupt data error (1): slot = %d, entry = %d",
                          (int)SCH_AppData.NextSlotNumber, (int)EntryNumber);
    sprintf(expEventText2,
          "Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d",
          SCH_AppData.ScheduleTable[EntryNumber].MessageIndex,
          SCH_AppData.ScheduleTable[EntryNumber].Frequency,
          SCH_AppData.ScheduleTable[EntryNumber].Type,
          SCH_AppData.ScheduleTable[EntryNumber].Remainder);

    /* Verify results */
    UtAssert_True(SCH_AppData.BadTableDataCount == 1,
                  "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True(
          SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED,
          "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText1,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Slot)");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText2,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Entry)");
}


/**
 * SCH_ProcessNextEntry, Test_CorruptFrequency
 */
void SCH_ProcessNextEntry_Test_CorruptFrequency(void)
{
    int32 EntryNumber = 1;
    char  expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;

    /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Frequency = SCH_UNUSED;

    SCH_AppData.ScheduleTable[EntryNumber].Type = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 1;

    SCH_AppData.NextSlotNumber = 2;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    sprintf(expEventText1, "Corrupt data error (1): slot = %d, entry = %d",
                          (int)SCH_AppData.NextSlotNumber, (int)EntryNumber);
    sprintf(expEventText2,
          "Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d",
          SCH_AppData.ScheduleTable[EntryNumber].MessageIndex,
          SCH_AppData.ScheduleTable[EntryNumber].Frequency,
          SCH_AppData.ScheduleTable[EntryNumber].Type,
          SCH_AppData.ScheduleTable[EntryNumber].Remainder);

    /* Verify results */
    UtAssert_True(SCH_AppData.BadTableDataCount == 1,
                  "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True(
      SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED,
      "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText1,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Slot)");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText2,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Entry)");
}


/**
 * SCH_ProcessNextEntry, Test_CorruptType
 */
void SCH_ProcessNextEntry_Test_CorruptType(void)
{
    int32 EntryNumber = 1;
    char  expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency = 99;

    /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Type = 99;

    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 1;

    SCH_AppData.NextSlotNumber = 2;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    sprintf(expEventText1, "Corrupt data error (1): slot = %d, entry = %d",
                          (int)SCH_AppData.NextSlotNumber, (int)EntryNumber);
    sprintf(expEventText2,
          "Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d",
          SCH_AppData.ScheduleTable[EntryNumber].MessageIndex,
          SCH_AppData.ScheduleTable[EntryNumber].Frequency,
          SCH_AppData.ScheduleTable[EntryNumber].Type,
          SCH_AppData.ScheduleTable[EntryNumber].Remainder);

    /* Verify results */
    UtAssert_True(SCH_AppData.BadTableDataCount == 1,
                  "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True(
         SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED,
         "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText1,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Slot)");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText2,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Entry)");
}


/**
 * SCH_ProcessNextEntry, Test_CorruptRemainder
 */
void SCH_ProcessNextEntry_Test_CorruptRemainder(void)
{
    int32 EntryNumber = 1;
    char  expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency = 99;
    SCH_AppData.ScheduleTable[EntryNumber].Type = SCH_ACTIVITY_SEND_MSG;

    /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 99;

    SCH_AppData.NextSlotNumber = 2;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    sprintf(expEventText1, "Corrupt data error (1): slot = %d, entry = %d",
                          (int)SCH_AppData.NextSlotNumber, (int)EntryNumber);
    sprintf(expEventText2,
          "Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d",
          SCH_AppData.ScheduleTable[EntryNumber].MessageIndex,
          SCH_AppData.ScheduleTable[EntryNumber].Frequency,
          SCH_AppData.ScheduleTable[EntryNumber].Type,
          SCH_AppData.ScheduleTable[EntryNumber].Remainder);

    /* Verify results */
    UtAssert_True(SCH_AppData.BadTableDataCount == 1,
                  "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True(
       SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED,
       "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText1,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Slot)");

    UtAssert_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, expEventText2,
               "ProcessNextEntry_Test_CorruptMessageIndex Event Sent(Entry)");
}


/**
 * SCH_ProcessNextEntry, Test_Success
 */
void SCH_ProcessNextEntry_Test_Success(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency = 2;
    SCH_AppData.ScheduleTable[EntryNumber].Type = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 1;

    SCH_AppData.NextSlotNumber = 2;
    SCH_AppData.TablePassCount = 3;

    /* Set to satisfy condition "Status == CFE_SUCCESS", and to prevent
       possible segmentation fault  */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SENDMSG_INDEX, CFE_SUCCESS, 1);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 1,
                  "SCH_AppData.ScheduleActivitySuccessCount == 1");
}


/**
 * SCH_ProcessNextEntry, Test_PacketSendError
 */
void SCH_ProcessNextEntry_Test_PacketSendError(void)
{
    int32 EntryNumber = 1;
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency = 2;
    SCH_AppData.ScheduleTable[EntryNumber].Type = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder = 1;

    SCH_AppData.NextSlotNumber = 2;
    SCH_AppData.TablePassCount = 3;

    /* Set to fail condition "Status == CFE_SUCCESS", and to prevent
       possible segmentation fault */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SENDMSG_INDEX, CFE_SB_MSG_TOO_BIG, 1);

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber],
                         EntryNumber);

    sprintf(expEventText, "Activity error: slot = %d, entry = %d, err = 0x%08X",
                          (int)SCH_AppData.NextSlotNumber, (int)EntryNumber,
                          (unsigned int)CFE_SB_MSG_TOO_BIG);

    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 1,
                  "SCH_AppData.ScheduleActivityFailureCount == 1");

    UtAssert_EventSent(SCH_PACKET_SEND_EID, CFE_EVS_ERROR, expEventText,
                   "ProcessNextEntry_Test_PacketSendError Event Sent");
}

/**
 * SCH_ValidateScheduleData, Test_GarbageFrequency
 */
void SCH_ValidateScheduleData_Test_GarbageFrequency(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_GARBAGE_ENTRY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_GarbageFrequency");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                       EventVerifyErr,
                       "ValidateScheduleData_Test_GarbageFrequency:"
                       " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_GarbageFrequency:"
                       " Verify Results EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_GarbageRemainder
 */
void SCH_ValidateScheduleData_Test_GarbageRemainder(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_GARBAGE_ENTRY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 99;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_GarbageRemainder");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                       EventVerifyErr,
                       "ValidateScheduleData_Test_GarbageRemainder:"
                       " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_GarbageRemainder:"
                       " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_GarbageGroupData
 */
void SCH_ValidateScheduleData_Test_GarbageGroupData(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_GARBAGE_ENTRY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].GroupData = 0x0000000F;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_GarbageGroupData");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                       EventVerifyErr,
                       "ValidateScheduleData_Test_GarbageGroupData:"
                       " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_GarbageGroupData:"
                       " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_GarbageType
 */
void SCH_ValidateScheduleData_Test_GarbageType(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_GARBAGE_ENTRY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].Type = 99;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_GarbageType");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                       EventVerifyErr,
                       "ValidateScheduleData_Test_GarbageType:"
                       " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_GarbageType:"
                       " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_GarbageMessageIndex
 */
void SCH_ValidateScheduleData_Test_GarbageMessageIndex(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_GARBAGE_ENTRY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 99;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_GarbageMessageIndex");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                       EventVerifyErr,
                       "ValidateScheduleData_Test_GarbageMessageIndex:"
                       " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_GarbageMessageIndex:"
                       " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_EnableStateUnusedAllFieldsUnused
 */
void SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    int32   TableIndex;
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, 0, SCH_TABLE_ENTRIES);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 1,
                  "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True(Result == Expected, "ValidateScheduleData_Test_Enable"
                                      "StateUnusedAllFieldsUnused");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                       EventVerifyResult,
                       "ValidateScheduleData_Test_EnableStateUnused"
                       "AllFieldsUnused EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_EnableStateEnabledFrequencyUnused
 */
void SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_NO_FREQUENCY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_ENABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
            "ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_EnableStateDisabledFrequencyUnused
 */
void SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_NO_FREQUENCY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
            "ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_BadRemainder
 */
void SCH_ValidateScheduleData_Test_BadRemainder(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_BAD_REMAINDER;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 100;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_BadRemainder");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_BadRemainder:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_BadRemainder:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_BadActivity
 */
void SCH_ValidateScheduleData_Test_BadActivity(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_BAD_ACTIVITY;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = 99;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_BadActivity");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_BadActivity:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_BadActivity:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_MsgIndexZero
 */
void SCH_ValidateScheduleData_Test_MsgIndexZero(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_BAD_MSG_INDEX;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 0;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_MsgIndexZero");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_MsgIndexZero:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_MsgIndexZero:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_MsgIndexTooHigh
 */
void SCH_ValidateScheduleData_Test_MsgIndexTooHigh(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = SCH_SDT_BAD_MSG_INDEX;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_MAX_MESSAGES;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_MsgIndexTooHigh");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_MsgIndexTooHigh:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_MsgIndexTooHigh:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_ValidEntryResult
 */
void SCH_ValidateScheduleData_Test_ValidEntryResult(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;
    int32   TableIndex;
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_MAX_MESSAGES - 1;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               SCH_TABLE_ENTRIES, 0, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 1,
                  "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_ValidEntryResult");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_ValidEntryResult:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateScheduleData, Test_EnableStateOther
 */
void SCH_ValidateScheduleData_Test_EnableStateOther(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected  = SCH_SDT_BAD_ENABLE_STATE;
    int32   TableIndex;
    char    EventVerifyErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    EventVerifyResult[CFE_EVS_MAX_MESSAGE_LENGTH];

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = 99;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 0;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    sprintf(EventVerifyErr, "Schedule tbl verify error - idx[%d] ena[%d]"
                    " typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X] dl[%d]",
                    0, SCH_AppData.ScheduleTable[0].EnableState,
                    SCH_AppData.ScheduleTable[0].Type,
                    SCH_AppData.ScheduleTable[0].Frequency,
                    SCH_AppData.ScheduleTable[0].Remainder,
                    SCH_AppData.ScheduleTable[0].MessageIndex,
                    (unsigned int)SCH_AppData.ScheduleTable[0].GroupData,
                    (int)SCH_AppData.ScheduleTable[0].Deadline);
    sprintf(EventVerifyResult, "Schedule table verify results -- "
                               "good[%d] bad[%d] unused[%d]",
                               0, SCH_TABLE_ENTRIES, 0);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 1,
                  "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True(Result == Expected,
                  "ValidateScheduleData_Test_EnableStateOther");

    UtAssert_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
             EventVerifyErr,
             "ValidateScheduleData_Test_EnableStateOther:"
             " Verify Error EventSent");

    UtAssert_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
             EventVerifyResult,
             "ValidateScheduleData_Test_EnableStateOther:"
             " Verify Result EventSent");
}

/**
 * SCH_ValidateMessageData, Test_MessageIdUnusedGarbageEntry
 */
void SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry(void)
{
    int32   Result;

    SCH_AppData.MessageTable[0].MessageBuffer[0] = 99;

    /* Set to satisfy condition "MessageID == SCH_UNUSED_MID" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SCH_UNUSED_MID, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set message length to a constant value */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Set to pass condition "*UserDataPtr != SCH_UNUSED" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETUSERDATA_INDEX, (int32)(&SCH_AppData.MessageTable[0].MessageBuffer[0]), 1);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    if (SCH_UNUSED_MID == 0x0)
    {
        UtAssert_True
            (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR,
                "Message tbl verify err - idx[0] mid[0x0] len[10] buf[0]"),
            "Message tbl verify err - idx[0] mid[0x0] len[10] buf[0]");
    }
    else
    {
        UtAssert_True
            (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR,
                "Message tbl verify err - idx[0] mid[0x1897] len[10] buf[0]"),
            "Message tbl verify err - idx[0] mid[0x1897] len[10] buf[0]");
    }

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[1] unused[127]"),
        "Message tbl verify results - good[0] bad[1] unused[127]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_GARBAGE_ENTRY, "Result == SCH_MDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry */

void SCH_ValidateMessageData_Test_MessageIdUnusedValid(void)
{
    int32   Result;

    /* Set to satisfy condition "MessageID == SCH_UNUSED_MID" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SCH_UNUSED_MID, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set message length to a constant value */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[0] unused[128]"),
        "Message tbl verify results - good[0] bad[0] unused[128]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateMessageData_Test_MessageIdUnusedValid */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength > (SCH_MAX_MSG_WORDS * 2))" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MAX_MSG_WORDS * 2) + 2, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[130] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[130] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength < (SCH_MIN_MSG_WORDS * 2))" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) - 2, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[6] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[6] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength & 1) != 0)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 11, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[19] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[19] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to fail all condtions in the if-statement for MessageLength */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[128] bad[0] unused[0]"),
        "Message tbl verify results - good[128] bad[0] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid */

void SCH_ValidateMessageData_Test_MessageIdOther(void)
{
    int32   Result;

    /* Set to fail all if-condtions on MessageID */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID + 1), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength & 1) != 0)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x2000] len[18] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x2000] len[18] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_BAD_MSG_ID, "Result == SCH_MDT_BAD_MSG_ID");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdOther */

void SCH_ValidateScheduleDeadlines_Test_AllZeroes(void)
{
    int32   Result;
    int32   TableIndex;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */
    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 1;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 0;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 1;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = 0;
    }

    sprintf(expEventText, "Schedule table verify results -- good[%u] bad[0] unused[0]", SCH_TABLE_ENTRIES);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText),
        		expEventText);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleDeadlines_Test_AllZeroes */

void SCH_ValidateScheduleDeadlines_Test_AllOnes(void)
{
    int32   Result;
    int32   TableIndex;
    char    expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested. Note: A table of all ones will pass 1/5 of the time due to
     * the last item in a slot specifying a deadline of 1 being a valid deadline */
    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 1;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 0;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 1;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = 1;
    }

    sprintf(expEventText1, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[1] rem[0] msg[1] grp[0x00000000] dl[1]");
    sprintf(expEventText2, "Schedule table verify results -- good[%u] bad[%u] unused[0]", SCH_TABLE_ENTRIES/5, SCH_TABLE_ENTRIES-(SCH_TABLE_ENTRIES/5));

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, expEventText1),
        		expEventText1);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText2),
        		expEventText2);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleDeadlines_Test_AllOnes */

void SCH_ValidateScheduleDeadlines_Test_AllTwos(void)
{
    int32   Result;
    int32   TableIndex;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */
    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 1;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 0;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 1;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Deadline = 2;
    }

    sprintf(expEventText1, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[1] rem[0] msg[1] grp[0x00000000] dl[2]");
    sprintf(expEventText2, "Schedule table verify results -- good[0] bad[%u] unused[0]", SCH_TABLE_ENTRIES);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, expEventText1),
        		expEventText1);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText2),
        		expEventText2);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True (Result == SCH_SDT_BAD_DEADLINE, "Result == SCH_SDT_BAD_DEADLINE");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 501");

} /* end SCH_ValidateScheduleDeadlines_Test_AllTwos */

void SCH_ValidateScheduleDeadlines_Test_Valid1(void)
{
    int32   Result;
    int32   TestIdx1 = 0; 	// Frame 0
    int32   TestIdx2 = 100; // Frame 20
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in two locations with valid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 20;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 20;

    sprintf(expEventText, "Schedule table verify results -- good[2] bad[0] unused[%u]", SCH_TABLE_ENTRIES-2);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText),
        		expEventText);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleDeadlines_Test_Valid1 */

void SCH_ValidateScheduleDeadlines_Test_Valid2(void)
{
    int32   Result;
    int32   TestIdx1 = 50;	// Frame 10
    int32   TestIdx2 = 100; // Frame 20
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in two locations with valid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 10;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 90;

    sprintf(expEventText, "Schedule table verify results -- good[2] bad[0] unused[%u]", SCH_TABLE_ENTRIES-2);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText),
        		expEventText);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleDeadlines_Test_Valid2 */

void SCH_ValidateScheduleDeadlines_Test_Valid3(void)
{
    int32   Result;
    int32   TestMsgIdx = 7;
    int32   TestIdx1 = 0;	// Frame 0
    int32   TestIdx2 = 200; // Frame 40
    int32   TestIdx3 = 50;  // Frame 10
    int32   TestIdx4 = 125; // Frame 25
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in two locations with valid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 40;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 60;

	SCH_AppData.ScheduleTable[TestIdx3].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx3].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx3].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx3].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx3].MessageIndex = TestMsgIdx;
	SCH_AppData.ScheduleTable[TestIdx3].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx3].Deadline = 3;

	SCH_AppData.ScheduleTable[TestIdx4].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx4].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx4].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx4].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx4].MessageIndex = TestMsgIdx;
	SCH_AppData.ScheduleTable[TestIdx4].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx4].Deadline = 85;

    sprintf(expEventText, "Schedule table verify results -- good[4] bad[0] unused[%u]", SCH_TABLE_ENTRIES-4);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText),
        expEventText);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleDeadlines_Test_Valid3 */

void SCH_ValidateScheduleDeadlines_Test_Invalid1(void)
{
    int32   Result;
    int32   TestIdx1 = 50;	 // Frame 10
    int32   TestIdx2 = 100;	 // Frame 20
    char    expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in two locations with valid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 50;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 100;

    sprintf(expEventText1, "Schedule table verify results -- good[0] bad[2] unused[%u]", SCH_TABLE_ENTRIES-2);
    sprintf(expEventText2, "Schedule tbl verify error - idx[50] ena[2] typ[1] fre[1] rem[0] msg[1] grp[0x00000000] dl[50]");

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText1),
        expEventText1);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, expEventText2),
        expEventText2);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True (Result == SCH_SDT_BAD_DEADLINE, "Result == SCH_SDT_BAD_DEADLINE");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleDeadlines_Test_Invalid1 */

void SCH_ValidateScheduleDeadlines_Test_Invalid2(void)
{
    int32   Result;
    int32   TestIdx1 = 0;	// Frame 0
    int32   TestIdx2 = 25;  // Frame 5
    char    expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in two locations with invalid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 1;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 7;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 1;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 100;

    sprintf(expEventText1, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[1] rem[1] msg[1] grp[0x00000000] dl[7]");
    sprintf(expEventText2, "Schedule table verify results -- good[0] bad[2] unused[%u]", SCH_TABLE_ENTRIES-2);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, expEventText1),
        expEventText1);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText2),
        expEventText2);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True (Result == SCH_SDT_BAD_REMAINDER, "Result == SCH_SDT_BAD_REMAINDER");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleDeadlines_Test_Invalid2 */

void SCH_ValidateScheduleDeadlines_Test_Invalid3(void)
{
    int32   Result;
    int32   TestMsgIdx = 7;
    int32   TestIdx1 = 0; 	// Frame 0
    int32   TestIdx2 = 10;  // Frame 2
    int32   TestIdx3 = 5;	// Frame 1
    int32   TestIdx4 = 50;  // Frame 10
    char    expEventText1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventText2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build a table to be tested */

    /* Place messages in locations with invalid deadlines */
	SCH_AppData.ScheduleTable[TestIdx1].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx1].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx1].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx1].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx1].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx1].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx1].Deadline = 100;

	SCH_AppData.ScheduleTable[TestIdx2].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx2].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx2].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx2].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx2].MessageIndex = 1;
	SCH_AppData.ScheduleTable[TestIdx2].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx2].Deadline = 100;

	SCH_AppData.ScheduleTable[TestIdx3].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx3].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx3].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx3].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx3].MessageIndex = TestMsgIdx;
	SCH_AppData.ScheduleTable[TestIdx3].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx3].Deadline = 50;

	SCH_AppData.ScheduleTable[TestIdx4].EnableState = SCH_DISABLED;
	SCH_AppData.ScheduleTable[TestIdx4].Type = SCH_ACTIVITY_SEND_MSG;
	SCH_AppData.ScheduleTable[TestIdx4].Frequency = 1;
	SCH_AppData.ScheduleTable[TestIdx4].Remainder = 0;
	SCH_AppData.ScheduleTable[TestIdx4].MessageIndex = TestMsgIdx;
	SCH_AppData.ScheduleTable[TestIdx4].GroupData = SCH_UNUSED;
	SCH_AppData.ScheduleTable[TestIdx4].Deadline = 92;

    sprintf(expEventText1, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[1] rem[0] msg[1] grp[0x00000000] dl[100]");
    sprintf(expEventText2, "Schedule table verify results -- good[0] bad[4] unused[%u]", SCH_TABLE_ENTRIES-4);

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);

    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, expEventText1),
        expEventText1);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, expEventText2),
        expEventText2);

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");
    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True (Result == SCH_SDT_BAD_DEADLINE, "Result == SCH_SDT_BAD_DEADLINE");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleDeadlines_Test_Invalid3 */


void SCH_App_Test_AddTestCases(void)
{
    UtTest_Add(SCH_AppMain_Test_RegisterAppError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_RegisterAppError");
    UtTest_Add(SCH_AppMain_Test_AppInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_AppInitError");
    UtTest_Add(SCH_AppMain_Test_CustomLateInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_CustomLateInitError");
    UtTest_Add(SCH_AppMain_Test_NoisyMajorFrameError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_NoisyMajorFrameError");
    UtTest_Add(SCH_AppMain_Test_NominalIgnoreMajorFrameFalse, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_NominalIgnoreMajorFrameFalse");
    UtTest_Add(SCH_AppMain_Test_ProcessScheduleTableError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_ProcessScheduleTableError");
    UtTest_Add(SCH_AppMain_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_Nominal");

    UtTest_Add(SCH_AppInit_Test_GetAppIDError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_GetAppIDError");
    UtTest_Add(SCH_AppInit_Test_EvsInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_EvsInitError");
    UtTest_Add(SCH_AppInit_Test_SbInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_SbInitError");
    UtTest_Add(SCH_AppInit_Test_TblInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_TblInitError");
    UtTest_Add(SCH_AppInit_Test_TimerInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_TimerInitError");
    UtTest_Add(SCH_AppInit_Test_ChildTaskInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_ChildTaskInitError");
    UtTest_Add(SCH_AppInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_Nominal");

    UtTest_Add(SCH_EvsInit_Test_RegisterError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EvsInit_Test_RegisterError");
    UtTest_Add(SCH_EvsInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EvsInit_Test_Nominal");

    UtTest_Add(SCH_SbInit_Test_CreateCmdPipeError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_CreateCmdPipeError");
    UtTest_Add(SCH_SbInit_Test_CreateADPipeError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_CreateADPipeError");
    UtTest_Add(SCH_SbInit_Test_SubscribeHKError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_SubscribeHKError");
    UtTest_Add(SCH_SbInit_Test_SubscribeGNDError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_SubscribeGNDError");
    UtTest_Add(SCH_SbInit_Test_SubscribeActivityDoneError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_SubscribeActivityDoneError");
    UtTest_Add(SCH_SbInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_Nominal");

    UtTest_Add(SCH_TimerInit_Test_CustomEarlyInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_CustomEarlyInitError");
    UtTest_Add(SCH_TimerInit_Test_TimerAccuracyWarning, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_TimerAccuracyWarning");
    UtTest_Add(SCH_TimerInit_Test_BinSemCreateError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_BinSemCreateError");
    UtTest_Add(SCH_TimerInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_Nominal");

    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands, SCH_Test_SetupUnitTest, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup, SCH_Test_SetupUnitTest, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup, SCH_Test_SetupUnitTest, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup");

    UtTest_Add(SCH_ProcessNextSlot_Test_ProcessCommandsRollover, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextSlot_Test_ProcessCommandsRollover");
    UtTest_Add(SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover");

    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptMessageIndex, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptMessageIndex");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptFrequency, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptFrequency");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptType, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptType");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptRemainder, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptRemainder");
    UtTest_Add(SCH_ProcessNextEntry_Test_Success, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_Success");
    UtTest_Add(SCH_ProcessNextEntry_Test_PacketSendError, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_PacketSendError");

    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageFrequency, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageFrequency");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageRemainder, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageRemainder");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageGroupData, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageGroupData");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageType, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageType");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageMessageIndex, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageMessageIndex");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_BadRemainder, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_BadRemainder");
    UtTest_Add(SCH_ValidateScheduleData_Test_BadActivity, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_BadActivity");
    UtTest_Add(SCH_ValidateScheduleData_Test_MsgIndexZero, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_MsgIndexZero");
    UtTest_Add(SCH_ValidateScheduleData_Test_MsgIndexTooHigh, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_MsgIndexTooHigh");
    UtTest_Add(SCH_ValidateScheduleData_Test_ValidEntryResult, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_ValidEntryResult");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateOther, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateOther");

    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdUnusedValid, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdUnusedValid");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdOther, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdOther");

    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_AllZeroes, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_AllZeroes");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_AllOnes, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_AllOnes");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_AllTwos, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_AllTwos");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Valid1, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Valid1");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Valid2, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Valid2");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Valid3, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Valid3");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Invalid1, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Invalid1");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Invalid2, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Invalid2");
    UtTest_Add(SCH_ValidateScheduleDeadlines_Test_Invalid3, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_ValidateScheduleDeadlines_Test_Invalid3");

} /* end SCH_App_Test_AddTestCases */
/************************/
/*  End of File Comment */
/************************/
