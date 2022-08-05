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

#include "fpc_cmds_test.h"
#include "fpc_test_utils.h"

#include "fpc_msg.h"

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
 * Tests for FPC ProcessNewCmds()
 **************************************************************************/
/**
 * Test FPC ProcessNewCmds, InvalidCmd
 */
void Test_FPC_ProcessNewCmds_InvalidCmd(void)
{
}

/**
 * Test FPC ProcessNewCmds, InvalidCmdCode
 */
void Test_FPC_ProcessNewCmds_InvalidCmdCode(void)
{
}

/**
 * Test FPC ProcessNewCmds, CmdPipeError
 */
void Test_FPC_ProcessNewCmds_CmdPipeError(void)
{
}

/**
 * Test FPC ProcessNewCmds, Noop
 */
void Test_FPC_ProcessNewCmds_Noop(void)
{
}

/**
 * Test FPC ProcessNewCmds, Reset
 */
void Test_FPC_ProcessNewCmds_Reset(void)
{
}

/**
 * Test FPC ProcessNewCmds, DoGoAround
 */
void Test_FPC_ProcessNewCmds_DoGoAround(void)
{
}


void FPC_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmd, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmdCode, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_FPC_ProcessNewCmds_CmdPipeError, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_CmdPipeError");
    UtTest_Add(Test_FPC_ProcessNewCmds_Noop, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Noop");
    UtTest_Add(Test_FPC_ProcessNewCmds_Reset, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Reset");
    UtTest_Add(Test_FPC_ProcessNewCmds_DoGoAround, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_DoGoAround");
} /* end FPC_Cmds_Test_AddTestCases */


