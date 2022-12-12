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
#include "rcin_msg.h"
#include "rcin_version.h"

#include "rcin_cmds_test.h"
#include "rcin_test_utils.h"

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
 * Tests for RCIN ProcessCmdPipe()
 **************************************************************************/
/**
 * Test RCIN_ProcessCmdPipe, InvalidCmd
 */
void Test_RCIN_ProcessCmdPipe_InvalidCmd(void)
{
    int32            CmdPipe;
    RCIN_NoArgCmd_t  CmdMsg;
    char             expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(RCIN_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, 0, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    RCIN_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "Recvd invalid CMD msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_True((oRCIN.HkTlm.usCmdCnt == 0) &&
                  (oRCIN.HkTlm.usCmdErrCnt == 1),
                  "RCIN_ProcessCmdPipe, InvalidCmd");

    UtAssert_EventSent(RCIN_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "RCIN_ProcessCmdPipe, InvalidCmd Event Sent");
}


/**
 * Test RCIN_ProcessCmdPipe, InvalidCmdCode
 */
void Test_RCIN_ProcessCmdPipe_InvalidCmdCode(void)
{
    int32            CmdPipe;
    RCIN_NoArgCmd_t  CmdMsg;
    char             expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(RCIN_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, RCIN_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    RCIN_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "Recvd invalid command code (%u)", 100);

    /* Verify results */
    UtAssert_True((oRCIN.HkTlm.usCmdCnt == 0) &&
                  (oRCIN.HkTlm.usCmdErrCnt == 1),
                  "RCIN_ProcessCmdPipe, InvalidCmdCode");

    UtAssert_EventSent(RCIN_CC_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "RCIN_ProcessCmdPipe, InvalidCmdCode Event Sent");
}


/**
 * Test RCIN_ProcessCmdPipe, NoMessage
 */
void Test_RCIN_ProcessCmdPipe_NoMessage(void)
{
    int32            SchPipe;
    RCIN_NoArgCmd_t  SchMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(RCIN_SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&SchMsg, RCIN_SEND_HK_MID, sizeof(SchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&SchMsg, (CFE_SB_PipeId_t)SchPipe);
    RCIN_Test_PrintCmdMsg((void*)&SchMsg, sizeof(SchMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();
}


/**
 * Test RCIN_ProcessCmdPipe, PipeError
 */
void Test_RCIN_ProcessCmdPipe_PipeError(void)
{
    int32            SchPipe;
    RCIN_NoArgCmd_t  SchMsg;
    char             expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(RCIN_SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&SchMsg, RCIN_SEND_HK_MID, sizeof(SchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&SchMsg, (CFE_SB_PipeId_t)SchPipe);
    RCIN_Test_PrintCmdMsg((void*)&SchMsg, sizeof(SchMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "CMD pipe read error (0x%08lX)",
                      (long unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(RCIN_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "RCIN_ProcessCmdPipe, PipeError Event Sent");
}


/**
 * Test RCIN_ProcessCmdPipe, Noop
 */
void Test_RCIN_ProcessCmdPipe_Noop(void)
{
    int32            CmdPipe;
    RCIN_NoArgCmd_t  CmdMsg;
    char             expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(RCIN_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, RCIN_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)RCIN_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    RCIN_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "Recvd NOOP. Version %d.%d.%d.%d",
                      RCIN_MAJOR_VERSION, RCIN_MINOR_VERSION,
                      RCIN_REVISION, RCIN_MISSION_REV);

    /* Verify results */
    UtAssert_True((oRCIN.HkTlm.usCmdCnt == 1) &&
                  (oRCIN.HkTlm.usCmdErrCnt == 0),
                  "RCIN_ProcessCmdPipe, Noop");

    UtAssert_EventSent(RCIN_CMD_NOOP_EID, CFE_EVS_INFORMATION, expEvent,
                       "RCIN_ProcessCmdPipe, Noop Event Sent");
}


/**
 * Test RCIN_ProcessCmdPipe, Reset
 */
void Test_RCIN_ProcessCmdPipe_Reset(void)
{
    int32            CmdPipe;
    RCIN_NoArgCmd_t  CmdMsg;
    char             expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(RCIN_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, RCIN_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)RCIN_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    RCIN_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oRCIN.InitApp();

    oRCIN.HkTlm.usCmdCnt = 3;
    oRCIN.HkTlm.usCmdErrCnt = 1;

    oRCIN.ProcessCmdPipe();

    /* Verify results */
    UtAssert_True((oRCIN.HkTlm.usCmdCnt == 0) &&
                  (oRCIN.HkTlm.usCmdErrCnt == 0),
                  "RCIN_ProcessCmdPipe, Reset");
}


/**
 * Test RCIN VerifyCmdLength(), Fail CmdLength
 */
void Test_RCIN_VerifyCmdLength_Fail_CmdLength(void)
{
    RCIN oRCINut;

    boolean          bResult = TRUE;
    boolean          bExpected = FALSE;
    RCIN_NoArgCmd_t  CmdMsg;
    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, RCIN_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)RCIN_NOOP_CC);
    RCIN_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oRCINut.InitApp();

    bResult = oRCINut.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg,
                                      sizeof(CmdMsg) + 5);

    sprintf(expEvent, "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                      "msgLen=%d, expectedLen=%d",
                      RCIN_CMD_MID, RCIN_NOOP_CC,
                      sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((bResult == bExpected) &&
                  (oRCINut.HkTlm.usCmdCnt == 0) &&
                  (oRCINut.HkTlm.usCmdErrCnt == 1),
                  "VerifyCmdLength(), Fail CmdLength");

    UtAssert_EventSent(RCIN_MSGLEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "VerifyCmdLength(), Fail CmdLength Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void RCIN_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_RCIN_ProcessCmdPipe_InvalidCmd,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_RCIN_ProcessCmdPipe_InvalidCmdCode,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_RCIN_ProcessCmdPipe_NoMessage,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_NoMessage");
    UtTest_Add(Test_RCIN_ProcessCmdPipe_PipeError,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_PipeError");
    UtTest_Add(Test_RCIN_ProcessCmdPipe_Noop,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_Noop");
    UtTest_Add(Test_RCIN_ProcessCmdPipe_Reset,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ProcessCmdPipe_Reset");

    UtTest_Add(Test_RCIN_VerifyCmdLength_Fail_CmdLength,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_VerifyCmdLength_Fail_CmdLength");
}
