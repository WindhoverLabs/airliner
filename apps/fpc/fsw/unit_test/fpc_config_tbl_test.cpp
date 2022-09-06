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

#include "fpc_config_tbl_test.hpp"
#include "fpc_test_utils.hpp"

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
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NO_ACCESS;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Register");
}

/**
 * Test FPC InitConfigTbl(), fail ValidateConfigTbl
 */
void Test_FPC_InitConfigTbl_Fail_ValidateConfigTbl(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = -1;

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();
}

/**
 * Test FPC InitConfigTbl(), ValidateConfigTbl_Nominal
 */
void Test_FPC_InitConfigTbl_ValidateConfigTbl_Nominal(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected, "InitConfigTbl, ValidateConfigTbl Nominal");
}

/**
 * Test FPC InitConfigTbl(), fail TBL Load
 */
void Test_FPC_InitConfigTbl_Fail_TblLoad(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Load");
}

/**
 * Test FPC InitConfigTbl(), fail TBL Manage
 */
void Test_FPC_InitConfigTbl_Fail_TblManage(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Manage");
}

/**
 * Test FPC InitConfigTbl(), fail TBL GetAddress
 */
void Test_FPC_InitConfigTbl_Fail_TblGetAddress(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL GetAddress");
}

/**
 * Test FPC InitConfigTbl(), fail AcquireConfigPtrs
 */
void Test_FPC_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail AcquireConfigPtrs");
}

/**
 * Test FPC InitConfigTbl(), Nominal
 */
void Test_FPC_InitConfigTbl_Nominal(void)
{
#ifndef FPC_UT_EXTERN_OBJECT
    FPC   oFPC{};
#endif

    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFPC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected, "InitConfigTbl, Nominal");
}


void FPC_Config_Tbl_Test_AddTestCases(void)
{
#ifdef FPC_UT_TEST_WITH_OWN_FPC_OBJECT
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_TblRegister, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_FPC_InitConfigTbl_Fail_ValidateConfigTbl, FPC_Test_Setup_ConfigInvalid,
               FPC_Test_TearDown, "Test_FPC_InitConfigTbl_Fail_ValidateConfigTbl");
    UtTest_Add(Test_FPC_InitConfigTbl_ValidateConfigTbl_Nominal, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_InitConfigTbl_ValidateConfigTbl_Nominal");
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
#endif
}
