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

#include "sens_cmds_test.hpp"
#include "sens_test_utils.hpp"

#include "cfe.h"
#include "sens_msg.h"
#include "sens_version.h"
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
 * Tests for SENS ProcessCmdPipe()
 **************************************************************************/
/**
 * Test SENS ProcessCmdPipe(), InvalidCmd
 */
void Test_SENS_ProcessCmdPipe_InvalidCmd(void)
{
    SENS            oSENS;

    int32           CmdPipe;
    SENS_NoArgCmd_t InMsg;
    char            expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("SENS_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    SENS_Test_PrintCmdMsg((void*)&InMsg, sizeof(SENS_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "Recvd invalid CMD msgId (0x%04X)", PX4_AIRSPEED_MID);

    /* Verify results */
    UtAssert_EventSent(SENS_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmd Event Sent");
    UtAssert_True(oSENS.HkTlm.usCmdErrCnt == 1, "ProcessCmdPipe, InvalidCmd");
}


/**
 * Test SENS ProcessCmdPipe, InvalidCmdCode
 */
void Test_SENS_ProcessCmdPipe_InvalidCmdCode(void)
{
    SENS             oSENS;

    int32            CmdPipe;
    SENS_NoArgCmd_t  InMsg;
    char             expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("SENS_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, SENS_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    SENS_Test_PrintCmdMsg((void*)&InMsg, sizeof(SENS_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "Recvd invalid command code (%u)", 100);

    UtAssert_EventSent(SENS_CC_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmd Event Sent");
    UtAssert_True(oSENS.HkTlm.usCmdErrCnt == 1, "ProcessCmdPipe, InvalidCmd");
}


/**
 * Test SENS ProcessCmdPipe, CmdPipeError
 */
void Test_SENS_ProcessCmdPipe_CmdPipeError(void)
{
    SENS              oSENS;

    int32             SchPipe;
    SENS_NoArgCmd_t   InMsg;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("SENS_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, SENS_SEND_HK_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "CMD pipe read error (0x%08lX)", CFE_SB_BAD_ARGUMENT);

    UtAssert_EventSent(SENS_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, CmdPipeError");
}


/**
 * Test SENS ProcessCmdPipe, Noop
 */
void Test_SENS_ProcessCmdPipe_Noop(void)
{
    SENS              oSENS;

    int32             CmdPipe;
    SENS_NoArgCmd_t   InMsg;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("SENS_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, SENS_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)SENS_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    SENS_Test_PrintCmdMsg((void*)&InMsg, sizeof(SENS_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();

    sprintf(expectedEvent, "Recvd NOOP. Version %d.%d.%d.%d",
                            SENS_MAJOR_VERSION, SENS_MINOR_VERSION,
                            SENS_REVISION, SENS_MISSION_REV);

    UtAssert_EventSent(SENS_CMD_NOOP_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "ProcessCmdPipe, Noop Event Sent");
    UtAssert_True(oSENS.HkTlm.usCmdCnt == 1, "ProcessCmdPipe, Noop");
}


/**
 * Test SENS ProcessCmdPipe, Reset
 */
void Test_SENS_ProcessCmdPipe_Reset(void)
{
    SENS              oSENS;

    int32             CmdPipe;
    SENS_NoArgCmd_t   InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("SENS_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, SENS_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)SENS_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    SENS_Test_PrintCmdMsg((void*)&InMsg, sizeof(SENS_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SENS_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oSENS.AppMain();
}


/**************************************************************************
 * Tests for SENS VerifyCmdLength()
 **************************************************************************/
/**
 * Test SENS VerifyCmdLength(), Fail CmdLength
 */
void Test_SENS_VerifyCmdLength_Fail_CmdLength(void)
{
    SENS              oSENS;

    bool              bResult = TRUE;
    bool              bExpected = FALSE;
    SENS_NoArgCmd_t   CmdMsg;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg ((void*)&CmdMsg, SENS_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SENS_RESET_CC);

    SENS_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(SENS_NoArgCmd_t));

    /* Execute the function being tested */
    oSENS.InitData();
    bResult = oSENS.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg, sizeof(CmdMsg) + 5);

    /* Verify results */
    sprintf(expectedEvent, "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                           "msgLen=%d, expectedLen=%d", SENS_CMD_MID, SENS_RESET_CC,
                           sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    UtAssert_EventSent(SENS_MSGLEN_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "VerifyCmdLength(), Fail CmdLength Event Sent");
    UtAssert_True (((bResult == bExpected) && (oSENS.HkTlm.usCmdErrCnt == 1)),
                   "VerifyCmdLength, Fail CmdLength");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SENS_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_SENS_ProcessCmdPipe_InvalidCmd,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_SENS_ProcessCmdPipe_InvalidCmdCode,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_SENS_ProcessCmdPipe_CmdPipeError,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_ProcessCmdPipe_CmdPipeError");
    UtTest_Add(Test_SENS_ProcessCmdPipe_Noop,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_ProcessCmdPipe_Noop");
    UtTest_Add(Test_SENS_ProcessCmdPipe_Reset,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_ProcessCmdPipe_Reset");
    UtTest_Add(Test_SENS_VerifyCmdLength_Fail_CmdLength,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_VerifyCmdLength_Fail_CmdLength");
} /* end SENS_Cmds_Test_AddTestCases */
