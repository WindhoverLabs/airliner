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

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, PX4_AIRSPEED_MID, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();

    /* Verify results */
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oAMC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, InvalidCmd");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, InvalidCmd");
    }
}


/**
 * Test AMC ProcessCmdPipe, CmdPipeError
 */
void Test_AMC_ProcessCmdPipe_CmdPipeError(void)
{
    AMC  oAMC;

    int32              SchPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("AMC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC ProcessCmdPipe, Noop
 */
void Test_AMC_ProcessCmdPipe_Noop(void)
{
    AMC  oAMC;

    int32              CmdPipe;
    AMC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISARM_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_ENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DISENGAGE_DEBUG_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, AMC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
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
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_DEBUG_CMD_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    oAMC.HkTlm.DebugEngaged = TRUE;
    CmdMsg.Index = 0;        // check this values
    CmdMsg.Cmd = oAMC.ConfigTblPtr->Channel[0].PwmInitial;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    if (oAMC.HkTlm.usCmdCnt == 1)
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, DebugCmd");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, DebugCmd");
    }
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetOutputScaler
 */
void Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler(void)
{
    AMC  oAMC;

    int32                           CmdPipe;
    AMC_SimpleSetOutputScalerCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_OUTPUT_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    CmdMsg.MixerIndex = 0;                  // check this values
    CmdMsg.OutputScaler.NegativeScale = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.NegativeScale + 1.0;
    CmdMsg.OutputScaler.PositiveScale = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.PositiveScale + 1.0;
    CmdMsg.OutputScaler.Offset = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.Offset + 1.0;
    CmdMsg.OutputScaler.MinOutput = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MinOutput + 1.0;
    CmdMsg.OutputScaler.MaxOutput = oAMC.MixerConfigTblPtr->Simple[0].OutputScaler.MaxOutput + 1.0;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    if (oAMC.HkTlm.usCmdCnt == 1)
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, SimpleSetOutputScaler");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, SimpleSetOutputScaler");
    }
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetControl
 */
void Test_AMC_ProcessCmdPipe_SimpleSetControl(void)
{
    AMC  oAMC;

    int32                      CmdPipe;
    AMC_SimpleSetControlCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_CONTROL_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    CmdMsg.MixerIndex = 0;                        // check this value
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

    if (oAMC.HkTlm.usCmdCnt == 1)
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, SimpleSetControl");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, SimpleSetControl");
    }
}


/**
 * Test AMC ProcessCmdPipe, SimpleSetControlScaler
 */
void Test_AMC_ProcessCmdPipe_SimpleSetControlScaler(void)
{
    AMC  oAMC;

    int32                            CmdPipe;
    AMC_SimpleSetControlScalerCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("AMC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, AMC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)AMC_SIMPLE_SET_CONTROL_SCALER_CC);

    AMC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    oAMC.InitApp();

    CmdMsg.MixerIndex = 0;                  // check this value
    CmdMsg.Control = 0;
    CmdMsg.Scaler.NegativeScale = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.NegativeScale + 1;
    CmdMsg.Scaler.PositiveScale = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.PositiveScale + 1;
    CmdMsg.Scaler.Offset = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.Offset + 1;
    CmdMsg.Scaler.MinOutput = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MinOutput + 1;
    CmdMsg.Scaler.MaxOutput = oAMC.MixerConfigTblPtr->Simple[0].Controls[0].Scaler.MaxOutput + 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    oAMC.ProcessCmdPipe();

    if (oAMC.HkTlm.usCmdCnt == 1)
    {
        UtAssert_True(TRUE, "AMC ProcessCmdPipe, SimpleSetControlScaler");
    }
    else
    {
        UtAssert_True(FALSE, "AMC ProcessCmdPipe, SimpleSetControlScaler");
    }
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
    UtTest_Add(Test_AMC_ProcessCmdPipe_CmdPipeError, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_CmdPipeError");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Noop, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Noop");
    UtTest_Add(Test_AMC_ProcessCmdPipe_Reset, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_Reset");
    UtTest_Add(Test_AMC_ProcessCmdPipe_ArmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_ArmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisarmDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisarmDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_EngageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_EngageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DisengageDebug, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DisengageDebug");
    UtTest_Add(Test_AMC_ProcessCmdPipe_DebugCmd, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_DebugCmd");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetOutputScaler");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControl, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControl");
    UtTest_Add(Test_AMC_ProcessCmdPipe_SimpleSetControlScaler, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_ProcessCmdPipe_SimpleSetControlScaler");
    UtTest_Add(Test_AMC_VerifyCmdLength_Fail_CmdLength, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_VerifyCmdLength_Fail_CmdLength");
} /* end AMC_Cmds_Test_AddTestCases */


