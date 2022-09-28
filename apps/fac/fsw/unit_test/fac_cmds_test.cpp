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
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)100);
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
 * Test FAC ProcessAppCmds, Update_FW_R_TC_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_R_TC_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_R_TC_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.3f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_R_TC == 0.9f), "ProcessAppCmds, Update_FW_R_TC_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_P_TC_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_P_TC_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_P_TC_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.1f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_P_TC == 0.9f), "ProcessAppCmds, Update_FW_P_TC_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_PR_P_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_PR_P_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_PR_P_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_PR_P == 0.9f), "ProcessAppCmds, Update_FW_PR_P_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_PR_I_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_PR_I_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_PR_I_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.4f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_PR_I == 0.4f), "ProcessAppCmds, Update_FW_PR_I_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_P_RMAX_POS_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_POS_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_P_RMAX_POS_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_P_RMAX_POS == 89.0f), "ProcessAppCmds, Update_FW_P_RMAX_POS_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_P_RMAX_NEG_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_NEG_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_P_RMAX_NEG_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_P_RMAX_NEG == 89.0f), "ProcessAppCmds, Update_FW_P_RMAX_NEG_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_PR_IMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_PR_IMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_PR_IMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_PR_IMAX == 0.9f), "ProcessAppCmds, Update_FW_PR_IMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RR_P_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RR_P_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RR_P_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RR_P == 0.9f), "ProcessAppCmds, Update_FW_RR_P_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RR_I_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RR_I_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RR_I_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.1f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RR_I == 0.1f), "ProcessAppCmds, Update_FW_RR_I_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RR_IMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RR_IMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RR_IMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RR_IMAX == 0.9f), "ProcessAppCmds, Update_FW_RR_IMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_R_RMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_R_RMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_R_RMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_R_RMAX == 89.0f), "ProcessAppCmds, Update_FW_R_RMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YR_P_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YR_P_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YR_P_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YR_P == 0.9f), "ProcessAppCmds, Update_FW_YR_P_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YR_I_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YR_I_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YR_I_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 49.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YR_I == 49.0f), "ProcessAppCmds, Update_FW_YR_I_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YR_IMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YR_IMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YR_IMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YR_IMAX == 0.9f), "ProcessAppCmds, Update_FW_YR_IMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_Y_RMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_Y_RMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_Y_RMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_Y_RMAX == 89.0f), "ProcessAppCmds, Update_FW_Y_RMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RLL_TO_YAW_FF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RLL_TO_YAW_FF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RLL_TO_YAW_FF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
          (oFAC.ParamTblPtr->FW_RLL_TO_YAW_FF == 0.9f), "ProcessAppCmds, Update_FW_RLL_TO_YAW_FF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_W_EN_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_W_EN_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamInt32Cmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_W_EN_CC);

    /* No Invalid parameter value */

    /* Valid parameter value */
    CmdMsg.param = 2;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.ParamTblPtr->FW_W_EN == 2),
                   "ProcessAppCmds, Update_FW_W_EN_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_WR_P_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_WR_P_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_WR_P_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_WR_P == 0.9f), "ProcessAppCmds, Update_FW_WR_P_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_WR_I_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_WR_I_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_WR_I_CC);

    /* Invalid parameter value */
    CmdMsg.param = 0.004f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.4f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_WR_I == 0.4f), "ProcessAppCmds, Update_FW_WR_I_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_WR_IMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_WR_IMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_WR_IMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_WR_IMAX == 0.9f), "ProcessAppCmds, Update_FW_WR_IMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_W_RMAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_W_RMAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_W_RMAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_W_RMAX == 89.0f), "ProcessAppCmds, Update_FW_W_RMAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RR_FF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RR_FF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RR_FF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 9.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RR_FF == 9.0f), "ProcessAppCmds, Update_FW_RR_FF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_PR_FF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_PR_FF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_PR_FF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 9.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_PR_FF == 9.0f), "ProcessAppCmds, Update_FW_PR_FF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YR_FF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YR_FF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YR_FF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 9.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YR_FF == 9.0f), "ProcessAppCmds, Update_FW_YR_FF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_WR_FF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_WR_FF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_WR_FF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 9.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_WR_FF == 9.0f), "ProcessAppCmds, Update_FW_WR_FF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YCO_VMIN_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YCO_VMIN_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YCO_VMIN_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 999.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YCO_VMIN == 999.0f), "ProcessAppCmds, Update_FW_YCO_VMIN_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_YCO_METHOD_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_YCO_METHOD_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamInt32Cmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_YCO_METHOD_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_YCO_METHOD == 1), "ProcessAppCmds, Update_FW_YCO_METHOD_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RSP_OFF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RSP_OFF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RSP_OFF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -100.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RSP_OFF == 89.0f), "ProcessAppCmds, Update_FW_RSP_OFF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_PSP_OFF_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_PSP_OFF_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_PSP_OFF_CC);

    /* Invalid parameter value */
    CmdMsg.param = -100.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_PSP_OFF == 89.0f), "ProcessAppCmds, Update_FW_PSP_OFF_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_MAN_R_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_MAN_R_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_MAN_R_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_MAN_R_MAX == 89.0f), "ProcessAppCmds, Update_FW_MAN_R_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_MAN_P_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_MAN_P_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_MAN_P_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 89.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_MAN_P_MAX == 89.0f), "ProcessAppCmds, Update_FW_MAN_P_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_FLAPS_SCL_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_FLAPS_SCL_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_FLAPS_SCL_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_FLAPS_SCL == 0.9f), "ProcessAppCmds, Update_FW_FLAPS_SCL_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_FLAPERON_SCL_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_FLAPERON_SCL_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_FLAPERON_SCL_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_FLAPERON_SCL == 0.9f), "ProcessAppCmds, Update_FW_FLAPERON_SCL_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_ARSP_MODE_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_ARSP_MODE_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamInt32Cmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_ARSP_MODE_CC);

    /* No Invalid parameter value */

    /* Valid parameter value */
    CmdMsg.param = 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.ParamTblPtr->FW_ARSP_MODE == 1),
                   "ProcessAppCmds, Update_FW_ARSP_MODE_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_MAN_R_SC_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_MAN_R_SC_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_MAN_R_SC_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_MAN_R_SC == 0.9f), "ProcessAppCmds, Update_FW_MAN_R_SC_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_MAN_P_SC_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_MAN_P_SC_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_MAN_P_SC_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_MAN_P_SC == 0.9f), "ProcessAppCmds, Update_FW_MAN_P_SC_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_MAN_Y_SC_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_MAN_Y_SC_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_MAN_Y_SC_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 2.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_MAN_Y_SC == 2.0f), "ProcessAppCmds, Update_FW_MAN_Y_SC_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_BAT_SCALE_EN_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_BAT_SCALE_EN_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamInt32Cmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_BAT_SCALE_EN_CC);

    /* No Invalid parameter value */

    /* Valid parameter value */
    CmdMsg.param = 2;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.ParamTblPtr->FW_BAT_SCALE_EN == 2),
                   "ProcessAppCmds, Update_FW_BAT_SCALE_EN_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_ACRO_X_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_ACRO_X_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_ACRO_X_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = 40.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 710.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_ACRO_X_MAX == 710.0f), "ProcessAppCmds, Update_FW_ACRO_X_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_ACRO_Y_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_ACRO_Y_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_ACRO_Y_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = 40.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 710.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_ACRO_Y_MAX == 710.0f), "ProcessAppCmds, Update_FW_ACRO_Y_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_ACRO_Z_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_ACRO_Z_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_ACRO_Z_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = 9.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 170.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_ACRO_Z_MAX == 170.0f), "ProcessAppCmds, Update_FW_ACRO_Z_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_RATT_TH_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_RATT_TH_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_RATT_TH_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.9f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_RATT_TH == 0.9f), "ProcessAppCmds, Update_FW_RATT_TH_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_AIRSPD_MIN_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MIN_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_AIRSPD_MIN_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 39.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_AIRSPD_MIN == 39.0f), "ProcessAppCmds, Update_FW_AIRSPD_MIN_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_AIRSPD_MAX_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MAX_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_AIRSPD_MAX_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 39.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_AIRSPD_MAX == 39.0f), "ProcessAppCmds, Update_FW_AIRSPD_MAX_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_FW_AIRSPD_TRIM_CC
 */
void Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_TRIM_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_FW_AIRSPD_TRIM_CC);

    /* Invalid parameter value */
    CmdMsg.param = -1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 39.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->FW_AIRSPD_TRIM == 39.0f), "ProcessAppCmds, Update_FW_AIRSPD_TRIM_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_TRIM_ROLL_CC
 */
void Test_FAC_ProcessAppCmds_Update_TRIM_ROLL_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_TRIM_ROLL_CC);

    /* Invalid parameter value */
    CmdMsg.param = -0.26f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.24f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->TRIM_ROLL == 0.24f), "ProcessAppCmds, Update_TRIM_ROLL_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_TRIM_PITCH_CC
 */
void Test_FAC_ProcessAppCmds_Update_TRIM_PITCH_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_TRIM_PITCH_CC);

    /* Invalid parameter value */
    CmdMsg.param = -0.26f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.24f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->TRIM_PITCH == 0.24f), "ProcessAppCmds, Update_TRIM_PITCH_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_TRIM_YAW_CC
 */
void Test_FAC_ProcessAppCmds_Update_TRIM_YAW_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamFloatCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_TRIM_YAW_CC);

    /* Invalid parameter value */
    CmdMsg.param = -0.26f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 0.24f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->TRIM_YAW == 0.24f), "ProcessAppCmds, Update_TRIM_YAW_CC");
}


/**
 * Test FAC ProcessAppCmds, Update_VT_TYPE_CC
 */
void Test_FAC_ProcessAppCmds_Update_VT_TYPE_CC(void)
{
    int32                      CmdPipe;
    FAC_UpdateParamUint32Cmd_t CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FAC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FAC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FAC_UPDATE_VT_TYPE_CC);

    /* Invalid parameter value */
    CmdMsg.param = 9;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Valid parameter value */
    CmdMsg.param = 1;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    FAC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True((oFAC.HkTlm.usCmdCnt == 1) && (oFAC.HkTlm.usCmdErrCnt == 1) &&
            (oFAC.ParamTblPtr->VT_TYPE == 1), "ProcessAppCmds, Update_VT_TYPE_CC");
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
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_R_TC_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_R_TC_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_P_TC_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_P_TC_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_PR_P_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_PR_P_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_PR_I_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_PR_I_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_POS_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_POS_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_NEG_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_P_RMAX_NEG_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_PR_IMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_PR_IMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RR_P_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RR_P_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RR_I_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RR_I_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RR_IMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RR_IMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_R_RMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_R_RMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YR_P_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YR_P_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YR_I_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YR_I_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YR_IMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YR_IMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_Y_RMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_Y_RMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RLL_TO_YAW_FF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RLL_TO_YAW_FF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_W_EN_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_W_EN_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_WR_P_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_WR_P_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_WR_I_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_WR_I_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_WR_IMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_WR_IMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_W_RMAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_W_RMAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RR_FF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RR_FF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_PR_FF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_PR_FF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YR_FF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YR_FF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_WR_FF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_WR_FF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YCO_VMIN_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YCO_VMIN_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_YCO_METHOD_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_YCO_METHOD_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RSP_OFF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RSP_OFF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_PSP_OFF_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_PSP_OFF_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_MAN_R_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_MAN_R_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_MAN_P_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_MAN_P_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_FLAPS_SCL_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_FLAPS_SCL_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_FLAPERON_SCL_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_FLAPERON_SCL_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_ARSP_MODE_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_ARSP_MODE_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_MAN_R_SC_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_MAN_R_SC_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_MAN_P_SC_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_MAN_P_SC_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_MAN_Y_SC_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_MAN_Y_SC_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_BAT_SCALE_EN_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_BAT_SCALE_EN_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_ACRO_X_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_ACRO_X_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_ACRO_Y_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_ACRO_Y_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_ACRO_Z_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_ACRO_Z_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_RATT_TH_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_RATT_TH_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MIN_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MIN_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MAX_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_MAX_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_TRIM_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_FW_AIRSPD_TRIM_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_TRIM_ROLL_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_TRIM_ROLL_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_TRIM_PITCH_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_TRIM_PITCH_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_TRIM_YAW_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_TRIM_YAW_CC");
    UtTest_Add(Test_FAC_ProcessAppCmds_Update_VT_TYPE_CC, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_ProcessAppCmds_Update_VT_TYPE_CC");

    UtTest_Add(Test_FAC_VerifyCmdLength_Fail_CmdLength, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_VerifyCmdLength_Fail_CmdLength");
} /* end FAC_Cmds_Test_AddTestCases */
