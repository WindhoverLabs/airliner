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

#include "fpc_config_tbl_test.h"
#include "fpc_test_utils.h"

#include "fpc_msg.h"

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
 * Tests for FPC InitConfigTbl()
 **************************************************************************/
/**
 * Test FPC InitConfigTbl(), fail TBL Register
 */
void Test_FPC_InitConfigTbl_Fail_TblRegister(void)
{
}

/**
 * Test FPC InitConfigTbl(), fail TBL Load
 */
void Test_FPC_InitConfigTbl_Fail_TblLoad(void)
{
}

/**
 * Test FPC InitConfigTbl(), fail TBL Manage
 */
void Test_FPC_InitConfigTbl_Fail_TblManage(void)
{
}

/**
 * Test FPC InitConfigTbl(), fail TBL GetAddress
 */
void Test_FPC_InitConfigTbl_Fail_TblGetAddress(void)
{
}

/**
 * Test FPC InitConfigTbl(), fail AcquireConfigPtrs
 */
void Test_FPC_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
}

/**
 * Test FPC InitConfigTbl(), Nominal
 */
void Test_FPC_InitConfigTbl_Nominal(void)
{
}


void FPC_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_TblRegister, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_TblLoad, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_TblManage, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_TblManage");
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_TblGetAddress, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_AcquireConfigPtrs, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FPC_InitConfigTbl_Nominal, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Nominal");
}


