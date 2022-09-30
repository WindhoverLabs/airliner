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

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
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
 * Tests for CI_ProcessNewAppCmds()
 **************************************************************************/
/**
 * Test CI_ProcessNewAppCmds(), Reset
 */
void Test_CI_ProcessNewAppCmds_Reset(void)
{
    boolean         retCode = FALSE;
    CI_NoArgCmd_t   cmd;
    uint32          MsgSize = sizeof(cmd);
    CFE_SB_MsgPtr_t CmdMsgPtr;
    CCSDS_CmdPkt_t  *cmdPkt = 0;
    char            expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Create CFE_SB_Msg_t */
    CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
    CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
    cmdPkt = (CCSDS_CmdPkt_t *)CmdMsgPtr;

    /* Set to cause to reset */
    CCSDS_WR_FC(cmdPkt->SecHdr, 1);

    /* Updates values */
    CI_AppData.HkTlm.usCmdCnt = 1;
    CI_AppData.HkTlm.usCmdErrCnt = 1;
    CI_AppData.HkTlm.IngestMsgCount = 1;
    CI_AppData.HkTlm.IngestErrorCount = 1;

    sprintf(expEventText, "Recvd RESET cmd (%u)", 1);

    /* Execute the function being tested */
    CI_ProcessNewAppCmds(CmdMsgPtr);

    /* Verify results */
    UtAssert_True(CI_AppData.HkTlm.usCmdCnt == 0,"Reset param");
    UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt == 0,"Reset param");
    UtAssert_True(CI_AppData.HkTlm.IngestMsgCount == 0,"Reset param");
    UtAssert_True(CI_AppData.HkTlm.IngestErrorCount == 0,"Reset param");
    UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION, expEventText, expEventText);
}




void CI_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_CI_ProcessNewAppCmds_Reset, CI_Test_Setup, CI_Test_TearDown,
              "Test_CI_ProcessNewAppCmds_Reset");
}
