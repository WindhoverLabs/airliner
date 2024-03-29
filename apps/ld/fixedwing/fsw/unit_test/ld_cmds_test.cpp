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
#include "ld_msg.h"
#include "ld_version.h"
#include "ld_cmds_test.hpp"
#include "ld_test_utils.hpp"
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
 * Tests for LD ProcessCmdPipe()
 **************************************************************************/
/**
 * Test LD ProcessCmdPipe, InvalidCmd
 */
void Test_LD_ProcessCmdPipe_InvalidCmd(void)
{
    int32           CmdPipe;
    LD_NoArgCmd_t   CmdMsg;
    char    expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, 0, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "Recvd invalid CMD msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_True(oLD.HkTlm.usCmdErrCnt == 1, "ProcessCmdPipe, InvalidCmd");

    UtAssert_EventSent(LD_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "ProcessCmdPipe, InvalidCmd: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, InvalidCmdCode
 */
void Test_LD_ProcessCmdPipe_InvalidCmdCode(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "Recvd invalid command code (%u)", 100);

    /* Verify results */
    UtAssert_True(oLD.HkTlm.usCmdErrCnt == 1,
                  "ProcessCmdPipe, InvalidCmdCode");

    UtAssert_EventSent(LD_CC_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "ProcessCmdPipe, InvalidCmdCode: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, CmdPipeError
 */
void Test_LD_ProcessCmdPipe_CmdPipeError(void)
{
    char   expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 1);

     /* Execute the function being tested */
    oLD.InitApp();
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "CMD pipe read error (0x%08lX)",
                            CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_EventSent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "ProcessCmdPipe, CmdPipeError: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, Noop
 */
void Test_LD_ProcessCmdPipe_Noop(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "Recvd NOOP. Version %d.%d.%d.%d",
                           LD_MAJOR_VERSION, LD_MINOR_VERSION,
                           LD_REVISION, LD_MISSION_REV);

     /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 1) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, Noop");

    UtAssert_EventSent(LD_CMD_NOOP_EID, CFE_EVS_INFORMATION, expEvent,
                       "ProcessCmdPipe, Noop: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, Reset
 */
void Test_LD_ProcessCmdPipe_Reset(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.HkTlm.usCmdCnt = 5;
    oLD.HkTlm.usCmdErrCnt = 2;

    oLD.ProcessCmdPipe();

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 0) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, Reset");
}


/**
 * Test LD ProcessCmdPipe, LD_MODE_AUTO_CC
 */
void Test_LD_ProcessCmdPipe_LD_MODE_AUTO_CC(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_AUTO_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_MANUAL;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "%s", "Operational mode changed to auto.");

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 1) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, LD_MODE_AUTO_CC");

    UtAssert_EventSent(LD_MODE_CHANGED_EID, CFE_EVS_INFORMATION, expEvent,
                       "ProcessCmdPipe, LD_MODE_AUTO_CC: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, fail LD_MODE_AUTO_CC_AlreadyAuto
 */
void Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_AlreadyAuto(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_AUTO_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "%s", "Command error LD already in auto mode.");

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 0) &&
                  (oLD.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, fail LD_MODE_AUTO_CC_AlreadyAuto");

    UtAssert_EventSent(LD_MODE_CHANGE_ERROR_EID, CFE_EVS_ERROR, expEvent,
            "ProcessCmdPipe, fail LD_MODE_AUTO_CC_AlreadyAuto: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, fail LD_MODE_AUTO_CC_TblModify
 */
void Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_TblModify(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_AUTO_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 1);

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_MANUAL;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "CFE_TBL_Modified error (%d)",
                           (int)CFE_TBL_ERR_INVALID_HANDLE);

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 1) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, fail LD_MODE_AUTO_CC_TblModify");

    UtAssert_EventSent(LD_TBL_MODIFIED_ERROR_EID, CFE_EVS_ERROR, expEvent,
            "ProcessCmdPipe, fail LD_MODE_AUTO_CC_TblModify: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, LD_MODE_MANUAL_CC
 */
void Test_LD_ProcessCmdPipe_LD_MODE_MANUAL_CC(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_MANUAL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "%s", "Operational mode changed to manual.");

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 1) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, LD_MODE_MANUAL_CC");

    UtAssert_EventSent(LD_MODE_CHANGED_EID, CFE_EVS_INFORMATION, expEvent,
                       "ProcessCmdPipe, LD_MODE_MANUAL_CC: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, fail LD_MODE_MANUAL_CC_AlreadyManual
 */
void Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_AlreadyManual(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_MANUAL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_MANUAL;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "%s",
            "Command error LD already in manual mode.");

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 0) &&
                  (oLD.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, fail LD_MODE_MANUAL_CC_AlreadyManual");

    UtAssert_EventSent(LD_MODE_CHANGE_ERROR_EID, CFE_EVS_ERROR, expEvent,
        "ProcessCmdPipe, fail LD_MODE_MANUAL_CC_AlreadyManual: Event Sent");
}


/**
 * Test LD ProcessCmdPipe, fail LD_MODE_MANUAL_CC_TblModify
 */
void Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_TblModify(void)
{
    int32              CmdPipe;
    LD_NoArgCmd_t      CmdMsg;
    char     expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(LD_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_MODE_MANUAL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MODIFIED_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 1);

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;
    oLD.ProcessCmdPipe();

    sprintf(expEvent, "CFE_TBL_Modified error (%d)",
                           (int)CFE_TBL_ERR_INVALID_HANDLE);

    /* Verify results */
    UtAssert_True((oLD.HkTlm.usCmdCnt == 1) &&
                  (oLD.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, fail LD_MODE_MANUAL_CC_TblModify");

    UtAssert_EventSent(LD_TBL_MODIFIED_ERROR_EID, CFE_EVS_ERROR, expEvent,
            "ProcessCmdPipe, fail LD_MODE_MANUAL_CC_TblModify: Event Sent");
}


/**
 * Test LD VerifyCmdLength(), Fail CmdLength
 */
void Test_LD_VerifyCmdLength_Fail_CmdLength(void)
{
    bool             bResult = TRUE;
    bool             bExpected = FALSE;
    LD_NoArgCmd_t    CmdMsg;
    char         expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, LD_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)LD_NOOP_CC);

    LD_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oLD.InitApp();
    bResult = oLD.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg,
                                  sizeof(LD_NoArgCmd_t));

    sprintf(expEvent,
            "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
            "msgLen=%d, expectedLen=%d", LD_CMD_MID, LD_NOOP_CC,
            sizeof(CmdMsg) + 5, sizeof(CmdMsg));

    /* Verify results */
    UtAssert_True((bResult == bExpected) && (oLD.HkTlm.usCmdErrCnt == 1),
                   "VerifyCmdLength, Fail CmdLength");

    UtAssert_EventSent(LD_MSGLEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "VerifyCmdLength(), Fail CmdLength: Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void LD_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_LD_ProcessCmdPipe_InvalidCmd,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_LD_ProcessCmdPipe_InvalidCmdCode,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_LD_ProcessCmdPipe_CmdPipeError,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_CmdPipeError");
    UtTest_Add(Test_LD_ProcessCmdPipe_Noop,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Noop");
    UtTest_Add(Test_LD_ProcessCmdPipe_Reset,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Reset");
    UtTest_Add(Test_LD_ProcessCmdPipe_LD_MODE_AUTO_CC,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_LD_MODE_AUTO_CC");
    UtTest_Add(Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_AlreadyAuto,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_AlreadyAuto");
    UtTest_Add(Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_TblModify,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Fail_LD_MODE_AUTO_CC_TblModify");
    UtTest_Add(Test_LD_ProcessCmdPipe_LD_MODE_MANUAL_CC,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_LD_MODE_MANUAL_CC");
    UtTest_Add(Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_AlreadyManual,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_AlreadyManual");
    UtTest_Add(Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_TblModify,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ProcessCmdPipe_Fail_LD_MODE_MANUAL_CC_TblModify");

    UtTest_Add(Test_LD_VerifyCmdLength_Fail_CmdLength,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_VerifyCmdLength_Fail_CmdLength");
}
