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

#include "fpc_cmds_test.h"
#include "fpc_test_utils.h"

#include "fpc_msg.h"

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
 * Tests for FPC ProcessNewCmds()
 **************************************************************************/
/**
 * Test FPC ProcessNewCmds, InvalidCmd
 */
void Test_FPC_ProcessNewCmds_InvalidCmd(void)
{
    FPC   oFPC{};

    int32           CmdPipe;
    FPC_NoArgCmd_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

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
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFPC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, InvalidCmd");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, InvalidCmd");
    }
#endif
}

/**
 * Test FPC ProcessNewCmds, InvalidCmdCode
 */
void Test_FPC_ProcessNewCmds_InvalidCmdCode(void)
{
    FPC   oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)20);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

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
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFPC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, InvalidCmdCode");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, InvalidCmdCode");
    }
#endif
}

/**
 * Test FPC ProcessNewCmds, CmdPipeError
 */
void Test_FPC_ProcessNewCmds_CmdPipeError(void)
{
    FPC   oFPC{};

    int32              SchPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("FPC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_WAKEUP_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    oFPC.AppMain();
#else
    FPC_AppMain();
#endif
}

/**
 * Test FPC ProcessNewCmds, Noop
 */
void Test_FPC_ProcessNewCmds_Noop(void)
{
    FPC   oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FPC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

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
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtAssert_True(oFPC.HkTlm.usCmdCnt == 1, "ProcessNewCmds, Noop");
#endif
}

/**
 * Test FPC ProcessNewCmds, Reset
 */
void Test_FPC_ProcessNewCmds_Reset(void)
{
    FPC   oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FPC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

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
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtAssert_True(((oFPC.HkTlm.usCmdCnt == 0) && (oFPC.HkTlm.usCmdErrCnt == 0)),
                                  "ProcessNewCmds, Reset");
#endif
}

/**
 * Test FPC ProcessNewCmds, DoGoAround
 */
void Test_FPC_ProcessNewCmds_DoGoAround(void)
{
    FPC   oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FPC_DO_GO_AROUND_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

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
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtAssert_True(oFPC.HkTlm.usCmdCnt == 1, "ProcessNewCmds, DoGoAround");
#endif
}


void FPC_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmd, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmdCode, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_FPC_ProcessNewCmds_CmdPipeError, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_CmdPipeError");
    UtTest_Add(Test_FPC_ProcessNewCmds_Noop, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Noop");
    UtTest_Add(Test_FPC_ProcessNewCmds_Reset, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Reset");
    UtTest_Add(Test_FPC_ProcessNewCmds_DoGoAround, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_DoGoAround");
} /* end FPC_Cmds_Test_AddTestCases */
