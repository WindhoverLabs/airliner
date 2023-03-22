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

#include "aspd4525_config_tbl_test.h"
#include "aspd4525_test_utils.h"

#include "aspd4525_msg.h"

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
 * Tests for ASPD4525 InitConfigTbl()
 **************************************************************************/
/**
 * Test ASPD4525 InitConfigTbl(), fail TBL Register
 */
void Test_ASPD4525_InitConfigTbl_Fail_TblRegister(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NO_ACCESS;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Register");
}


/**
 * Test ASPD4525 InitConfigTbl(), ValidateConfigTbl_Nominal
 */
void Test_ASPD4525_InitConfigTbl_ValidateConfigTbl_Nominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, ValidateConfigTbl_Nominal");
}


/**
 * Test ASPD4525 InitConfigTbl(), fail TBL Load
 */
void Test_ASPD4525_InitConfigTbl_Fail_TblLoad(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Load");
}


/**
 * Test ASPD4525 InitConfigTbl(), fail TBL Manage
 */
void Test_ASPD4525_InitConfigTbl_Fail_TblManage(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Manage");
}


/**
 * Test ASPD4525 InitConfigTbl(), fail TBL GetAddress
 */
void Test_ASPD4525_InitConfigTbl_Fail_TblGetAddress(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL GetAddress");
}


/**
 * Test ASPD4525 InitConfigTbl(), fail AcquireConfigPtrs
 */
void Test_ASPD4525_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail AcquireConfigPtrs");
}


/**
 * Test ASPD4525 InitConfigTbl(), Nominal
 */
void Test_ASPD4525_InitConfigTbl_Nominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = ASPD4525_InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, Nominal");
}


void ASPD4525_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Fail_TblRegister,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_ValidateConfigTbl_Nominal,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_ValidateConfigTbl_Nominal");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Fail_TblLoad,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Fail_TblManage,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Fail_TblManage");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Fail_TblGetAddress,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Fail_AcquireConfigPtrs,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_ASPD4525_InitConfigTbl_Nominal,
               ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_InitConfigTbl_Nominal");
}
