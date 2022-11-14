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
#include "amc_msg.h"
#include "amc_version.h"
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

#include "amc_cmds_test.hpp"
#include "amc_test_utils.hpp"
#include "ut_amc_custom_stubs.h"
#include "ut_amc_custom_hooks.h"


/**************************************************************************
 * Tests for AMC ProcessCmdPipe()
 **************************************************************************/
/**
 * Test AMC ProcessCmdPipe, InvalidCmd
 */
void Test_AMC_ProcessCmdPipe_InvalidCmd(void)
{
    AMC  oAMC;

    int32           CmdPipe;
    AMC_NoArgCmd_t  CmdMsg;
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, PX4_AIRSPEED_MID, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();

    sprintf(expectedEvent, "Recvd invalid CMD msgId (0x%04X)", PX4_AIRSPEED_MID);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, InvalidCmd");
    UtAssert_EventSent(AMC_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmd Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, InvalidCmdCode
 */
void Test_AMC_ProcessCmdPipe_InvalidCmdCode(void)
{
    AMC  oAMC;

    int32           CmdPipe;
    AMC_NoArgCmd_t  CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();

    sprintf(expectedEvent, "Recvd invalid command code (%u)", 100);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, InvalidCmdCode");
    UtAssert_EventSent(AMC_CC_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmdCode Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, CmdPipeError
 */
void Test_AMC_ProcessCmdPipe_CmdPipeError(void)
{
    AMC  oAMC;

    int32              SchPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char    expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(AMC_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();

    sprintf(expectedEvent, "CMD pipe read error (0x%08X)",
                           (unsigned int)CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_EventSent(AMC_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, CmdPipeError Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Noop
 */
void Test_AMC_ProcessCmdPipe_Noop(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();

    sprintf(expectedEvent, "Recvd NOOP. Version %d.%d.%d.%d",
                           AMC_MAJOR_VERSION, AMC_MINOR_VERSION,
                           AMC_REVISION, AMC_MISSION_REV);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, Noop");
    UtAssert_EventSent(AMC_CMD_NOOP_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "ProcessCmdPipe, Noop Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Reset
 */
void Test_AMC_ProcessCmdPipe_Reset(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();
    oAMC.ProcessCmdPipe();
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, Reset: Noop cmd count");

    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    oAMC.ProcessCmdPipe();
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Reset: Invalid CmdCode count");

    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    oAMC.ProcessCmdPipe();
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, Reset");
}


/**
 * Test AMC ProcessCmdPipe, ArmDebug
 */
void Test_AMC_ProcessCmdPipe_ArmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = FALSE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Debug mode armed.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0) &&
                  (oAMC.HkTlm.DebugArmed == TRUE) &&
                  (oAMC.HkTlm.ArmedTimeout == AMC_ARM_DEBUG_TIMEOUT),
                  "ProcessCmdPipe, ArmDebug");
    UtAssert_EventSent(AMC_ARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, ArmDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail ArmDebug
 */
void Test_AMC_ProcessCmdPipe_FailArmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s",
            "Failed to arm debug mode. Debug mode is already armed.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail ArmDebug");
    UtAssert_EventSent(AMC_ARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, Fail ArmDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, DisarmDebug
 */
void Test_AMC_ProcessCmdPipe_DisarmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Debug mode disarmed.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0) &&
                  (oAMC.HkTlm.DebugArmed == FALSE) &&
                  (oAMC.HkTlm.ArmedTimeout == 0),
                  "ProcessCmdPipe, DisarmDebug");
    UtAssert_EventSent(AMC_DISARM_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, DisarmDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail DisarmDebug
 */
void Test_AMC_ProcessCmdPipe_FailDisarmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = FALSE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s",
            "Failed to disarm debug mode. Debug mode is not armed.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail DisarmDebug");
    UtAssert_EventSent(AMC_DISARM_DEBUG_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "ProcessCmdPipe, Fail DisarmDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, EngageDebug
 */
void Test_AMC_ProcessCmdPipe_EngageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    oAMC.HkTlm.DebugEngaged = FALSE;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Debug mode engaged.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0) &&
                  (oAMC.HkTlm.DebugEngaged == TRUE) &&
                  (oAMC.HkTlm.DebugArmed == FALSE),
                  "ProcessCmdPipe, EngageDebug");
    UtAssert_EventSent(AMC_ENGAGE_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, EngageDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail EngageDebug
 */
void Test_AMC_ProcessCmdPipe_FailEngageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = FALSE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s",
            "Failed to engage debug, debug mode is not armed.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail EngageDebug");
    UtAssert_EventSent(AMC_ENGAGE_DEBUG_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "ProcessCmdPipe, Fail EngageDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, DisengageDebug
 */
void Test_AMC_ProcessCmdPipe_DisengageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Debug mode disengaged.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0) &&
                  (oAMC.HkTlm.DebugEngaged == FALSE),
                  "ProcessCmdPipe, DisengageDebug");
    UtAssert_EventSent(AMC_DISENGAGE_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, DisengageDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail DisengageDebug
 */
void Test_AMC_ProcessCmdPipe_FailDisengageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = FALSE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s",
            "Failed to disengage debug mode. Debug is not engaged.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail DisengageDebug");
    UtAssert_EventSent(AMC_DISENGAGE_DEBUG_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "ProcessCmdPipe, Fail DisengageDebug Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, DebugCmd
 */
void Test_AMC_ProcessCmdPipe_DebugCmd(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_DebugCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_AMC_Custom_SetFunctionHook(UT_AMC_CUSTOM_INITDEVICE_INDEX,
                                  (void*)&UT_InitDevice);
    Ut_AMC_Custom_SetFunctionHook(UT_AMC_CUSTOM_SETMOTOROUTPUTS_INDEX,
                                  (void*)&UT_SetMotorOutputs);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    SetMotorOutputs_CalledCnt = 0;
    CmdMsg.Index = 0;
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmMax;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "Received debug command index %hu value %hu",
                           CmdMsg.Index, CmdMsg.Cmd);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0) &&
                  (SetMotorOutputs_CalledCnt == 1),
                  "ProcessCmdPipe, DebugCmd");
    UtAssert_EventSent(AMC_CMD_DEBUG_INF_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, DebugCmd Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail DebugCmdInvalidIndex
 */
void Test_AMC_ProcessCmdPipe_FailDebugCmdInvalidIndex(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_DebugCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.Index = AMC_MAX_MOTOR_OUTPUTS;
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmMax;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Invalid index received");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail DebugCmdInvalidIndex");
    UtAssert_EventSent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
      expectedEvent, "ProcessCmdPipe, Fail DebugCmdInvalidIndex Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, Fail DebugCmdDisengaged
 */
void Test_AMC_ProcessCmdPipe_FailDebugCmdDisengaged(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_DebugCmd_t     CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = FALSE;
    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.Index = 0;
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmMax;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    sprintf(expectedEvent, "%s",
            "Debug command received with debug mode disengaged");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail DebugCmdDisengaged");
    UtAssert_EventSent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR,
       expectedEvent, "ProcessCmdPipe, Fail DebugCmdDisengaged Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetOutputScaler
 */
void Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                           CmdPipe;
    AMC_SimpleSetOutputScalerCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg,
                       (uint16)AMC_SIMPLE_SET_OUTPUT_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = 0;
    CmdMsg.OutputScaler.NegativeScale =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.NegativeScale + 1.0;
    CmdMsg.OutputScaler.PositiveScale =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.PositiveScale + 1.0;
    CmdMsg.OutputScaler.Offset =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.Offset + 1.0;
    CmdMsg.OutputScaler.MinOutput =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MinOutput + 1.0;
    CmdMsg.OutputScaler.MaxOutput =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MaxOutput + 1.0;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    printf("SimpleMixerOutputScaler, Before:\n");
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);

    oAMC.ProcessCmdPipe();

    printf("SimpleMixerOutputScaler, After:\n");
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, SimpleSetOutputScaler");
}


/**
 * Test AMC ProcessCmdPipe, Fail SimpleSetOutputScalerInvalidMixerIndex
 */
void Test_AMC_ProcessCmdPipe_Fail_SimpleSetOutputScalerInvalidMixerIndex(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                           CmdPipe;
    AMC_SimpleSetOutputScalerCmd_t  CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg,
                       (uint16)AMC_SIMPLE_SET_OUTPUT_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = AMC_SIMPLE_MIXER_MAX_MIXERS;
    CmdMsg.OutputScaler.NegativeScale =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.NegativeScale + 1.0;
    CmdMsg.OutputScaler.PositiveScale =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.PositiveScale + 1.0;
    CmdMsg.OutputScaler.Offset =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.Offset + 1.0;
    CmdMsg.OutputScaler.MinOutput =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MinOutput + 1.0;
    CmdMsg.OutputScaler.MaxOutput =
          oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MaxOutput + 1.0;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    printf("SimpleMixerOutputScaler, Before:\n");
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);

    oAMC.ProcessCmdPipe();

    printf("SimpleMixerOutputScaler, After:\n");
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);
    sprintf(expectedEvent, "%s", "Invalid Mixer index.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
              (oAMC.HkTlm.usCmdErrCnt == 1),
              "ProcessCmdPipe, Fail SimpleSetOutputScalerInvalidMixerIndex");
    UtAssert_EventSent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
      "ProcessCmdPipe, Fail SimpleSetOutputScalerInvalidMixerIndex Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetControl
 */
void Test_AMC_ProcessCmdPipe_SimpleSetControl(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                      CmdPipe;
    AMC_SimpleSetControlCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_CONTROL_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = 0;
    CmdMsg.Control = 0;
    CmdMsg.Group = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlGroup;
    if (oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex <
                                PX4_ACTUATOR_CONTROL_LANDING_GEAR)
    {
        CmdMsg.Index = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex + 1;
    }
    else
    {
        CmdMsg.Index = 0;
    }
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    printf("SimpleMixerControls, Before\n");
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    oAMC.ProcessCmdPipe();
    printf("SimpleMixerControls, After\n");
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, SimpleSetControl");
}


/**
 * Test AMC ProcessCmdPipe, Fail SimpleSetControlInvalidMixerIndex
 */
void Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlInvalidMixerIndex(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                      CmdPipe;
    AMC_SimpleSetControlCmd_t  CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_CONTROL_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = AMC_SIMPLE_MIXER_MAX_MIXERS;
    CmdMsg.Control = 0;
    CmdMsg.Group = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlGroup;
    if (oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex <
                                PX4_ACTUATOR_CONTROL_LANDING_GEAR)
    {
        CmdMsg.Index = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex + 1;
    }
    else
    {
        CmdMsg.Index = 0;
    }
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    printf("SimpleMixerControls, Before\n");
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    oAMC.ProcessCmdPipe();
    printf("SimpleMixerControls, After\n");
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);
    sprintf(expectedEvent, "%s", "Invalid Mixer index.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail SimpleSetControlInvalidMixerIndex");
    UtAssert_EventSent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
          "ProcessCmdPipe, Fail SimpleSetControlInvalidMixerIndex Event Sent");
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetControlScaler
 */
void Test_AMC_ProcessCmdPipe_SimpleSetControlScaler(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                            CmdPipe;
    AMC_SimpleSetControlScalerCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)AMC_SIMPLE_SET_CONTROL_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = 0;
    CmdMsg.Control = 0;
    CmdMsg.Scaler.NegativeScale =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.NegativeScale + 1;
    CmdMsg.Scaler.PositiveScale =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.PositiveScale + 1;
    CmdMsg.Scaler.Offset =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.Offset + 1;
    CmdMsg.Scaler.MinOutput =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MinOutput + 1;
    CmdMsg.Scaler.MaxOutput =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MaxOutput + 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    printf("SimpleMixerControlsScaler, Before\n");
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    oAMC.ProcessCmdPipe();
    printf("SimpleMixerControlsScaler, After\n");
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 1) &&
                  (oAMC.HkTlm.usCmdErrCnt == 0),
                  "ProcessCmdPipe, SimpleSetControlScaler");
}


/**
 * Test AMC ProcessCmdPipe, Fail SimpleSetControlScalerInvalidMixerIndex
 */
void Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlScalerInvalidMixerIndex(void)
{
    AMC  oAMC;
    AMC  *pAMC = &oAMC;

    int32                            CmdPipe;
    AMC_SimpleSetControlScalerCmd_t  CmdMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)AMC_SIMPLE_SET_CONTROL_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.usCmdCnt = 0;
    oAMC.HkTlm.usCmdErrCnt = 0;
    CmdMsg.MixerIndex = AMC_SIMPLE_MIXER_MAX_MIXERS;
    CmdMsg.Control = 0;
    CmdMsg.Scaler.NegativeScale =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.NegativeScale + 1;
    CmdMsg.Scaler.PositiveScale =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.PositiveScale + 1;
    CmdMsg.Scaler.Offset =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.Offset + 1;
    CmdMsg.Scaler.MinOutput =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MinOutput + 1;
    CmdMsg.Scaler.MaxOutput =
       oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MaxOutput + 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    printf("SimpleMixerControlsScaler, Before\n");
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    oAMC.ProcessCmdPipe();
    printf("SimpleMixerControlsScaler, After\n");
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);
    sprintf(expectedEvent, "%s", "Invalid Mixer index.");

    /* Verify results */
    UtAssert_True((oAMC.HkTlm.usCmdCnt == 0) &&
                  (oAMC.HkTlm.usCmdErrCnt == 1),
                  "ProcessCmdPipe, Fail SimpleSetControlScalerInvalidMixerIndex");
    UtAssert_EventSent(AMC_CMD_DEBUG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
         "ProcessCmdPipe, Fail SimpleSetControlScalerInvalidMixerIndex Event Sent");
}


/**
 * Test AMC VerifyCmdLength(), Fail CmdLength
 */
void Test_AMC_VerifyCmdLength_Fail_CmdLength(void)
{
    AMC  oAMC;

    bool              bResult = TRUE;
    bool              bExpected = FALSE;
    AMC_NoArgCmd_t    CmdMsg;

    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_NOOP_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    bResult = oAMC.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg, 16);

    /* Verify results */
    UtAssert_True (((bResult == bExpected) && (oAMC.HkTlm.usCmdErrCnt == 1)),
                   "VerifyCmdLength, Fail CmdLength");
}



void AMC_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_AMC_ProcessCmdPipe_InvalidCmd,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_InvalidCmdCode,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_AMC_ProcessCmdPipe_CmdPipeError,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_CmdPipeError");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Noop,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Noop");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Reset,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Reset");
    UtTest_Add(Test_AMC_ProcessCmdPipe_ArmDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_ArmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailArmDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailArmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisarmDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisarmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDisarmDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDisarmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_EngageDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_EngageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailEngageDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailEngageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisengageDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisengageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDisengageDebug,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDisengageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DebugCmd,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DebugCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDebugCmdInvalidIndex,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDebugCmdInvalidIndex");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDebugCmdDisengaged,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDebugCmdDisengaged");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Fail_SimpleSetOutputScalerInvalidMixerIndex,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Fail_SimpleSetOutputScalerInvalidMixerIndex");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControl,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControl");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlInvalidMixerIndex,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlInvalidMixerIndex");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControlScaler,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControlScaler");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlScalerInvalidMixerIndex,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Fail_SimpleSetControlScalerInvalidMixerIndex");

    UtTest_Add(Test_AMC_VerifyCmdLength_Fail_CmdLength,
               AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_VerifyCmdLength_Fail_CmdLength");
} /* end AMC_Cmds_Test_AddTestCases */
