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

#include "sch_api_test.h"
#include "sch_test_utils.h"

#include "uttest.h"
#include "utassert.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"

#include <time.h>


CFE_SB_MsgId_t  ActivityComplete_SendMsgHook_MsgId = 0;


/*
 * SCH_LibInit_Test
 */
void SCH_LibInit_Test(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = SCH_LibInit();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR,
                  "SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR");

    UtAssert_True(Result == OS_SUCCESS, "Result == OS_SUCCESS");
}


/*
 * SCH_EnableProcessing_Test
 */
void SCH_EnableProcessing_Test(void)
{
    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    SCH_EnableProcessing();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == 0,
                  "SCH_LibData.ProcessingDisabledCtr == 0");
}


/*
 * SCH_DisableProcessing_Test
 */
void SCH_DisableProcessing_Test(void)
{
    /* Execute the function being tested */
    SCH_DisableProcessing();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == 1,
                  "SCH_LibData.ProcessingDisabledCtr == 1");
}


/*
 * SCH_GetProcessingState_Test_True
 */
void SCH_GetProcessingState_Test_True(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 0;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True(Result == TRUE, "GetProcessingState_Test_True");
}


/*
 * SCH_GetProcessingState_Test_False
 */
void SCH_GetProcessingState_Test_False(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True(Result == FALSE, "GetProcessingState_Test_False");
}


/*
 * SCH_ActivityComplete_Test_Nominal_SendMsgHook
 */
int32 SCH_ActivityComplete_Test_Nominal_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*        pBuff = NULL;
    uint16                msgLen = 0;
    int                   i = 0;
    CFE_SB_MsgId_t        MsgId;
    time_t                localTime;
    struct tm             *loc_time;
    CFE_TIME_SysTime_t    TimeFromMsg;
    SCH_ActivityDoneMsg_t DoneMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###ActivityComplete_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = SCH_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    if (MsgId == SCH_ACTIVITY_DONE_MID)
    {
        ActivityComplete_SendMsgHook_MsgId = MsgId;
        CFE_PSP_MemCpy((void*)&DoneMsg, (void*)MsgPtr, sizeof(DoneMsg));

        printf("Sent SCH_ACTIVITY_DONE_MID:\n");
        printf("MsgID: 0x%04X\n", DoneMsg.MsgID);
    }
    else
    {
        printf("Sent MID(0x%04X)\n", MsgId);
    }

    return CFE_SUCCESS;
}


/*
 * SCH_ActivityComplete_Test_Nominal
 */
void SCH_ActivityComplete_Test_Nominal(void)
{
    int32  SlotIdx = 0;
    int32  ActIdx = 2;

    ActivityComplete_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                 (void *)&SCH_ActivityComplete_Test_Nominal_SendMsgHook);

    /* Execute the function being tested */
    SCH_AppInit();
    SCH_ActivityComplete(SCH_TEST_MID);

    /* Verify results */
    UtAssert_True(ActivityComplete_SendMsgHook_MsgId == SCH_ACTIVITY_DONE_MID,
                  "SCH_ActivityComplete_Test_Nominal");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SCH_API_Test_AddTestCases(void)
{
    UtTest_Add(SCH_LibInit_Test,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_LibInit_Test");

    UtTest_Add(SCH_EnableProcessing_Test,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_EnableProcessing_Test");

    UtTest_Add(SCH_DisableProcessing_Test,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_DisableProcessing_Test");

    UtTest_Add(SCH_GetProcessingState_Test_True,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_GetProcessingState_Test_True");
    UtTest_Add(SCH_GetProcessingState_Test_False,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_GetProcessingState_Test_False");

    UtTest_Add(SCH_ActivityComplete_Test_Nominal,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_ActivityComplete_Test_Nominal");
}
