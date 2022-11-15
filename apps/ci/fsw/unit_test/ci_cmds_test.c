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

#include "ci_cmds_test.h"
#include "ci_test_utils.h"

#include "ci_msg.h"
#include "ci_version.h"

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


/**************************************************************************
 * Tests for CI_ProcessNewCmds()
 **************************************************************************/
/**
 * Test CI ProcessNewCmds, InvalidCmd
 */
void Test_CI_ProcessNewCmds_InvalidCmd(void)
{
    int32           CmdPipe;
    CI_NoArgCmd_t   cmdMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&cmdMsg, 0, sizeof(cmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    CI_Test_PrintCmdMsg((void*)&cmdMsg, sizeof(cmdMsg));

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "Recvd invalid CMD msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_True((CI_AppData.HkTlm.usCmdCnt == 0) &&
                  (CI_AppData.HkTlm.usCmdErrCnt == 1),
                  "ProcessNewCmds, InvalidCmd");
    UtAssert_EventSent(CI_MSGID_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "ProcessNewCmds, InvalidCmd Event Sent");
}


/**
 * Test CI ProcessNewCmds, InvalidCmdCode
 */
void Test_CI_ProcessNewCmds_InvalidCmdCode(void)
{
    int32           CmdPipe;
    CI_NoArgCmd_t   cmdMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&cmdMsg, CI_CMD_MID, sizeof(cmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&cmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    CI_Test_PrintCmdMsg((void*)&cmdMsg, sizeof(cmdMsg));

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "Recvd invalid cmdId (%u)", 100);

    /* Verify results */
    UtAssert_True((CI_AppData.HkTlm.usCmdCnt == 0) &&
                  (CI_AppData.HkTlm.usCmdErrCnt == 1),
                  "ProcessNewCmds, InvalidCmdCode");
    UtAssert_EventSent(CI_MSGID_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "ProcessNewCmds, InvalidCmdCode Event Sent");
}


/**
 * Test CI ProcessNewCmds, CmdPipeError
 */
void Test_CI_ProcessNewCmds_CmdPipeError(void)
{
    int32           CmdPipe;
    CI_NoArgCmd_t   cmdMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 1);

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "CMD pipe read error (0x%08X)",
                          (unsigned int)CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_EventSent(CI_PIPE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "ProcessNewCmds, CmdPipeError EventSent");
}


/**
 * Test CI ProcessNewCmds, Noop
 */
void Test_CI_ProcessNewCmds_Noop(void)
{
    int32           CmdPipe;
    CI_NoArgCmd_t   cmdMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&cmdMsg, CI_CMD_MID, sizeof(cmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&cmdMsg, (uint16)CI_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    CI_Test_PrintCmdMsg((void*)&cmdMsg, sizeof(cmdMsg));

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                          CI_NOOP_CC, CI_MAJOR_VERSION, CI_MINOR_VERSION,
                          CI_REVISION, CI_MISSION_REV);

    /* Verify results */
    UtAssert_True(CI_AppData.HkTlm.usCmdCnt == 1,
                  "ProcessNewCmds, Noop");
    UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION,
             expEventText, "ProcessNewCmds, Noop Event Sent");
}


/**
 * Test CI ProcessNewCmds, Reset
 */
void Test_CI_ProcessNewCmds_Reset(void)
{
    int32           CmdPipe;
    CI_NoArgCmd_t   cmdMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&cmdMsg, CI_CMD_MID, sizeof(cmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&cmdMsg, (uint16)CI_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    CI_Test_PrintCmdMsg((void*)&cmdMsg, sizeof(cmdMsg));

    /* Execute the function being tested */
    CI_InitApp();

    CI_AppData.HkTlm.usCmdCnt = 1;
    CI_AppData.HkTlm.usCmdErrCnt = 1;
    CI_AppData.HkTlm.IngestMsgCount = 1;
    CI_AppData.HkTlm.IngestErrorCount = 1;

    CI_ProcessNewCmds();

    sprintf(expEventText, "Recvd RESET cmd (%u)", CI_RESET_CC);

    /* Verify results */
    UtAssert_True((CI_AppData.HkTlm.usCmdCnt == 0) &&
                  (CI_AppData.HkTlm.usCmdErrCnt == 0) &&
                  (CI_AppData.HkTlm.IngestMsgCount == 0) &&
                  (CI_AppData.HkTlm.IngestErrorCount == 0),
                  "ProcessNewCmds, Reset");
    UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION, expEventText,
                       "ProcessNewCmds, Reset Event Sent");
}


/**
 * Test CI ProcessNewCmds, CI_AUTH_CMD_CC
 */
void Test_CI_ProcessNewCmds_CI_AUTH_CMD_CC(void)
{
    int32             CmdPipe;
    CI_CmdRegData_t   cmdRegMsg;
    CI_CmdAuthData_t  cmdAuthMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);

    CFE_SB_InitMsg((void*)&cmdRegMsg, CI_CMD_MID, sizeof(cmdRegMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdRegMsg, (uint16)CI_REG_CMD_CC);
    cmdRegMsg.msgID = FAC_CMD_MID;
    cmdRegMsg.cmdCode = 0;
    cmdRegMsg.step = STEP_2;
    cmdRegMsg.log = LOG;
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdRegMsg, (CFE_SB_PipeId_t)CmdPipe);
    CI_Test_PrintCmdMsg((void*)&cmdRegMsg, sizeof(cmdRegMsg));

    CFE_SB_InitMsg((void*)&cmdAuthMsg, CI_CMD_MID, sizeof(cmdAuthMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdAuthMsg, (uint16)CI_AUTH_CMD_CC);
    cmdAuthMsg.msgID = FAC_CMD_MID;
    cmdAuthMsg.cmdCode = 0;
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdAuthMsg, (CFE_SB_PipeId_t)CmdPipe);
    CI_Test_PrintCmdMsg((void*)&cmdAuthMsg, sizeof(cmdAuthMsg));

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "Cmd (%u) authorized for msgId (0x%04X)",
                          cmdAuthMsg.cmdCode, cmdAuthMsg.msgID);

    /* Verify results */
    UtAssert_True((CI_AppData.HkTlm.usCmdCnt == 2) &&
                  (CI_AppData.HkTlm.usCmdErrCnt == 0),
                  "ProcessNewCmds, CI_AUTH_CMD_CC");
    UtAssert_EventSent(CI_CMD_AUTHORIZED_EID, CFE_EVS_INFORMATION,
                 expEventText, "ProcessNewCmds, CI_AUTH_CMD_CC Event Sent");
}


/**
 * Test CI ProcessNewCmds, CI_DEAUTH_CMD_CC
 */
void Test_CI_ProcessNewCmds_CI_DEAUTH_CMD_CC(void)
{
    int32             CmdPipe;
    CI_CmdRegData_t   cmdRegMsg;
    CI_CmdAuthData_t  cmdAuthMsg;
    CI_CmdAuthData_t  cmdDeauthMsg;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CI_CMD_PIPE_NAME);

    CFE_SB_InitMsg((void*)&cmdRegMsg, CI_CMD_MID, sizeof(cmdRegMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdRegMsg, (uint16)CI_REG_CMD_CC);
    cmdRegMsg.msgID = FAC_CMD_MID;
    cmdRegMsg.cmdCode = 0;
    cmdRegMsg.step = STEP_2;
    cmdRegMsg.log = LOG;
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdRegMsg, (CFE_SB_PipeId_t)CmdPipe);
    CI_Test_PrintCmdMsg((void*)&cmdRegMsg, sizeof(cmdRegMsg));

    CFE_SB_InitMsg((void*)&cmdAuthMsg, CI_CMD_MID, sizeof(cmdAuthMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdAuthMsg, (uint16)CI_AUTH_CMD_CC);
    cmdAuthMsg.msgID = FAC_CMD_MID;
    cmdAuthMsg.cmdCode = 0;
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdAuthMsg, (CFE_SB_PipeId_t)CmdPipe);
    CI_Test_PrintCmdMsg((void*)&cmdAuthMsg, sizeof(cmdAuthMsg));

    CFE_SB_InitMsg((void*)&cmdDeauthMsg, CI_CMD_MID,
                   sizeof(cmdDeauthMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&cmdDeauthMsg,
                      (uint16)CI_DEAUTH_CMD_CC);
    cmdDeauthMsg.msgID = FAC_CMD_MID;
    cmdDeauthMsg.cmdCode = 0;
    Ut_CFE_SB_AddMsgToPipe((void*)&cmdDeauthMsg, (CFE_SB_PipeId_t)CmdPipe);
    CI_Test_PrintCmdMsg((void*)&cmdDeauthMsg, sizeof(cmdDeauthMsg));

    /* Execute the function being tested */
    CI_InitApp();
    CI_ProcessNewCmds();

    sprintf(expEventText, "Cmd (%u) deauthorized for msgId (0x%04X)",
                          cmdDeauthMsg.cmdCode, cmdDeauthMsg.msgID);

    /* Verify results */
    UtAssert_True((CI_AppData.HkTlm.usCmdCnt == 3) &&
                  (CI_AppData.HkTlm.usCmdErrCnt == 0),
                  "ProcessNewCmds, CI_DEAUTH_CMD_CC");
    UtAssert_EventSent(CI_CMD_DEAUTHORIZED_EID, CFE_EVS_INFORMATION,
             expEventText, "ProcessNewCmds, CI_DEAUTH_CMD_CC Event Sent");
}



void CI_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_CI_ProcessNewCmds_InvalidCmd,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_CI_ProcessNewCmds_InvalidCmdCode,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_CI_ProcessNewCmds_CmdPipeError,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_CmdPipeError");
    UtTest_Add(Test_CI_ProcessNewCmds_Noop,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_Noop");
    UtTest_Add(Test_CI_ProcessNewCmds_Reset,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_Reset");
    UtTest_Add(Test_CI_ProcessNewCmds_CI_AUTH_CMD_CC,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_CI_AUTH_CMD_CC");
    UtTest_Add(Test_CI_ProcessNewCmds_CI_DEAUTH_CMD_CC,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ProcessNewCmds_CI_DEAUTH_CMD_CC");
}
