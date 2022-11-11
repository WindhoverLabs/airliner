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


boolean   ProcessCmdPipe_Result = FALSE;
char      ProcessCmdPipe_Str[64];


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
 * Test AMC ProcessCmdPipe(), SendEventHook
 */
int32 Test_AMC_ProcessCmdPipe_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];
    char     *pCmdStr = NULL;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###ProcessCmdPipe_SendEventHook:\n");
    printf("%s\n", Buf);

    pCmdStr = strstr(Buf, ProcessCmdPipe_Str);

    if (pCmdStr != NULL)
    {
        ProcessCmdPipe_Result = TRUE;
    }

    return 0;
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

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Debug mode armed");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();
    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0)
         && (oAMC.HkTlm.DebugArmed == TRUE) && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, ArmDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, ArmDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, Fail ArmDebug
 */
void Test_AMC_ProcessCmdPipe_FailArmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Failed to arm debug mode");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;

    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1)
         && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, Fail ArmDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, Fail ArmDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, DisarmDebug
 */
void Test_AMC_ProcessCmdPipe_DisarmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Debug mode disarmed");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;

    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0)
         && (oAMC.HkTlm.DebugArmed == FALSE) && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, DisarmDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, DisarmDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, Fail DisarmDebug
 */
void Test_AMC_ProcessCmdPipe_FailDisarmDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Failed to disarm debug mode");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();
    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1)
         && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, Fail DisarmDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, Fail DisarmDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, EngageDebug
 */
void Test_AMC_ProcessCmdPipe_EngageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Debug mode engaged");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugArmed = TRUE;

    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0)
         && (oAMC.HkTlm.DebugEngaged == TRUE) && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, EngageDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, EngageDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, Fail EngageDebug
 */
void Test_AMC_ProcessCmdPipe_FailEngageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Failed to engage debug");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();
    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1)
         && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, Fail EngageDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, Fail EngageDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, DisengageDebug
 */
void Test_AMC_ProcessCmdPipe_DisengageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Debug mode disengaged");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;

    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0)
         && (oAMC.HkTlm.DebugEngaged == FALSE) && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, DisengageDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, DisengageDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, Fail DisengageDebug
 */
void Test_AMC_ProcessCmdPipe_FailDisengageDebug(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Failed to disengage debug mode");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();
    oAMC.ProcessCmdPipe();

    /* Verify results */
    if ((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1)
         && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, Fail DisengageDebug");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, Fail DisengageDebug");
    }
}


/**
 * Test AMC ProcessCmdPipe, DebugCmd
 */
void Test_AMC_ProcessCmdPipe_DebugCmd(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_DebugCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Received debug command");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;
    CmdMsg.Index = 0;
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmMax;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    if ((oAMC.HkTlm.usCmdCnt == 1) && (oAMC.HkTlm.usCmdErrCnt == 0)
        && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, DebugCmd");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, DebugCmd");
    }
}


/**
 * Test AMC ProcessCmdPipe, Fail DebugCmd
 */
void Test_AMC_ProcessCmdPipe_FailDebugCmd(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_DebugCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(AMC_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessCmdPipe_Result = FALSE;
    strcpy(ProcessCmdPipe_Str, "Debug command received with debug mode disengaged");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_AMC_ProcessCmdPipe_SendEventHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    CmdMsg.Index = 0;
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmMax;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    if ((oAMC.HkTlm.usCmdCnt == 0) && (oAMC.HkTlm.usCmdErrCnt == 1)
        && (ProcessCmdPipe_Result == TRUE))
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, Fail DebugCmd");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, Fail DebugCmd");
    }
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
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_OUTPUT_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);

    CmdMsg.MixerIndex = 0;
    CmdMsg.OutputScaler.NegativeScale = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.NegativeScale + 1.0;
    CmdMsg.OutputScaler.PositiveScale = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.PositiveScale + 1.0;
    CmdMsg.OutputScaler.Offset = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.Offset + 1.0;
    CmdMsg.OutputScaler.MinOutput = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MinOutput + 1.0;
    CmdMsg.OutputScaler.MaxOutput = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MaxOutput + 1.0;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();
    AMC_Test_PrintSimpleMixerOutputScaler(pAMC, CmdMsg.MixerIndex);
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
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    CmdMsg.MixerIndex = 0;
    CmdMsg.Control = 0;
    CmdMsg.Group = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlGroup;
    if (oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex < PX4_ACTUATOR_CONTROL_LANDING_GEAR)
    {
        CmdMsg.Index = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].ControlIndex + 1;
    }
    else
    {
        CmdMsg.Index = 0;
    }
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();
    AMC_Test_PrintSimpleMixerControls(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);
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
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_CONTROL_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);

    CmdMsg.MixerIndex = 0;
    CmdMsg.Control = 0;
    CmdMsg.Scaler.NegativeScale = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.NegativeScale + 1;
    CmdMsg.Scaler.PositiveScale = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.PositiveScale + 1;
    CmdMsg.Scaler.Offset = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.Offset + 1;
    CmdMsg.Scaler.MinOutput = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MinOutput + 1;
    CmdMsg.Scaler.MaxOutput = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MaxOutput + 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();
    AMC_Test_PrintSimpleMixerControlsScaler(pAMC, CmdMsg.MixerIndex, CmdMsg.Control);
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
    UtTest_Add(Test_AMC_ProcessCmdPipe_InvalidCmd, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_InvalidCmdCode, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_AMC_ProcessCmdPipe_CmdPipeError, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_CmdPipeError");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Noop, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Noop");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Reset, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Reset");
    UtTest_Add(Test_AMC_ProcessCmdPipe_ArmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_ArmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailArmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailArmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisarmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisarmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDisarmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDisarmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_EngageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_EngageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailEngageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailEngageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisengageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisengageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDisengageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDisengageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DebugCmd, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DebugCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_FailDebugCmd, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_FailDebugCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControl");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControlScaler, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControlScaler");
    UtTest_Add(Test_AMC_VerifyCmdLength_Fail_CmdLength, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_VerifyCmdLength_Fail_CmdLength");
} /* end AMC_Cmds_Test_AddTestCases */
