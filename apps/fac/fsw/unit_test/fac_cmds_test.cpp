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

#include "fac_cmds_test.hpp"
#include "fac_test_utils.hpp"

#include "fac_msg.h"

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


/**************************************************************************
 * Tests for FAC ProcessNewCmds()
 **************************************************************************/
/**
 * Test FAC ProcessNewCmds, InvalidCmd
 */
void Test_FAC_ProcessNewCmds_InvalidCmd(void)
{
    int32           CmdPipe;
    FAC_NoArgCmd_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FAC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FAC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFAC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, InvalidCmd");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, InvalidCmd");
    }
}


/**
 * Test FAC ProcessAppCmds, InvalidCmdCode
 */
void Test_FAC_ProcessAppCmds_InvalidCmdCode(void)
{
    int32              CmdPipe;
    FAC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FAC_CMD_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)20);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FAC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FAC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFAC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessAppCmds, InvalidCmdCode");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessAppCmds, InvalidCmdCode");
    }
}


/**
 * Test FAC ProcessAppCmds, CmdPipeError
 */
void Test_FAC_ProcessAppCmds_CmdPipeError(void)
{
    int32              SchPipe;
    FAC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FAC_SEND_HK_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC ProcessAppCmds, Noop
 */
void Test_FAC_ProcessAppCmds_Noop(void)
{
    int32              CmdPipe;
    FAC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FAC_CMD_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FAC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FAC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FAC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.usCmdCnt == 1, "ProcessAppCmds, Noop");
}


/**
 * Test FAC ProcessAppCmds, Reset
 */
void Test_FAC_ProcessAppCmds_Reset(void)
{
    int32              CmdPipe;
    FAC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FAC_CMD_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FAC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FAC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FAC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(((oFAC.HkTlm.SendHkMsgRcvCnt == 0) && (oFAC.HkTlm.HkMsgSndCnt == 0)
                  && (oFAC.HkTlm.usCmdCnt == 0) && (oFAC.HkTlm.usCmdErrCnt == 0)),
				  "ProcessAppCmds, Reset");
}


/**
 * Test FAC VerifyCmdLength(), Fail CmdLength
 */
void Test_FAC_VerifyCmdLength_Fail_CmdLength(void)
{
    bool              bResult = TRUE;
    bool              bExpected = FALSE;
    FAC_NoArgCmd_t    CmdMsg;

    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_NOOP_CC);

    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    bResult = oFAC.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg, 16);

    /* Verify results */
    UtAssert_True (((bResult == bExpected) && (oFAC.HkTlm.usCmdErrCnt == 1)),
                   "VerifyCmdLength, Fail CmdLength");
}



void FAC_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_FAC_ProcessNewCmds_InvalidCmd, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_FAC_ProcessAppCmds_InvalidCmdCode, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_InvalidCmdCode");
    UtTest_Add(Test_FAC_ProcessAppCmds_CmdPipeError, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_CmdPipeError");
    UtTest_Add(Test_FAC_ProcessAppCmds_Noop, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Noop");
    UtTest_Add(Test_FAC_ProcessAppCmds_Reset, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Reset");
    UtTest_Add(Test_FAC_VerifyCmdLength_Fail_CmdLength, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_VerifyCmdLength_Fail_CmdLength");
} /* end FAC_Cmds_Test_AddTestCases */
