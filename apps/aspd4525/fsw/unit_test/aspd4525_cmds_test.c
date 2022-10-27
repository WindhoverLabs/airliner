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

#include "aspd4525_cmds_test.h"
#include "aspd4525_test_utils.h"
#include "aspd4525_version.h"

#include "aspd4525_msg.h"

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
 * Tests for ASPD4525_ProcessNewCmds()
 **************************************************************************/
/**
 * Test ASPD4525_ProcessNewCmds, InvalidCmd
 */
void Test_ASPD4525_ProcessNewCmds_InvalidCmd(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, 0, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid CMD msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, InvalidCmd");
}


/**
 * Test ASPD4525_ProcessNewCmds, InvalidCmdCode
 */
void Test_ASPD4525_ProcessNewCmds_InvalidCmdCode(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid cmdId (%u)", 100);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, InvalidCmd");
}


/**
 * Test ASPD4525_ProcessNewCmds, PipeError
 */
void Test_ASPD4525_ProcessNewCmds_PipeError(void)
{
    int32                SchPipe;
    int32                expected = CFE_SB_PIPE_RD_ERR;
    ASPD4525_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(ASPD4525_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, ASPD4525_WAKEUP_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ASPD4525_AppMain();

    sprintf(expectedEvent, "CMD pipe read error (0x%08X)",
            (unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_PIPE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, PipeError");
}


/**
 * Test ASPD4525_ProcessNewCmds, Noop
 */
void Test_ASPD4525_ProcessNewCmds_Noop(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)ASPD4525_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                           ASPD4525_NOOP_CC, ASPD4525_MAJOR_VERSION,
                           ASPD4525_MINOR_VERSION, ASPD4525_REVISION,
                           ASPD4525_MISSION_REV);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "ProcessNewCmds, Noop");
}


/**
 * Test ASPD4525_ProcessNewCmds, Reset
 */
void Test_ASPD4525_ProcessNewCmds_Reset(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)ASPD4525_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd RESET cmd (%u)", ASPD4525_RESET_CC);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "ProcessNewCmds, Reset");
}



void ASPD4525_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_InvalidCmd,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_InvalidCmdCode,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_PipeError,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ProcessNewCmds_PipeError");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_Noop,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ProcessNewCmds_Noop");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_Reset,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ProcessNewCmds_Reset");
} /* end ASPD4525_Cmds_Test_AddTestCases */


