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

#include "sens_config_tbl_test.hpp"
#include "sens_test_utils.hpp"

#include "cfe.h"
#include "sens_msg.h"
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
 * Tests for SENS InitConfigTbl()
 **************************************************************************/
/**
 * Test SENS InitConfigTbl(), fail TBL Register
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_TblRegister(void)
{
    SENS  oSENS;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NO_ACCESS;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Register");
}


/**
 * Test SENS InitConfigTbl(), fail ValidateConfigTbl
 * Can not call private function, InitConfigTbl directly
 * Currently there are no way to make this fail
 */
void Test_SENS_InitConfigTbl_Fail_ValidateConfigTbl(void)
{
    SENS  oSENS;
    int32 result = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSENS.InitApp();
}


/**
 * Test SENS InitConfigTbl(), fail TBL Load
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_TblLoad(void)
{
    SENS  oSENS;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Load");
}


/**
 * Test SENS InitConfigTbl(), fail TBL Manage
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_TblManage(void)
{
    SENS  oSENS;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Manage");
}


/**
 * Test SENS InitConfigTbl(), fail TBL GetAddress
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_TblGetAddress(void)
{
    SENS  oSENS;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL GetAddress");
}


/**
 * Test SENS InitConfigTbl(), fail AcquireConfigPointers
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_AcquireConfigPointers(void)
{
    SENS  oSENS;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail AcquireConfigPointers");
}


/**
 * Test SENS InitConfigTbl(), Nominal
 * Can not call private function, InitConfigTbl directly
 */
void Test_SENS_InitConfigTbl_Fail_Nominal(void)
{
    SENS  oSENS;

    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oSENS.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, Nominal");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SENS_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_TblRegister,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_ValidateConfigTbl,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_ValidateConfigTbl");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_TblLoad,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_TblManage,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_TblManage");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_TblGetAddress,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_AcquireConfigPointers,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_AcquireConfigPointers");
    UtTest_Add(Test_SENS_InitConfigTbl_Fail_Nominal,
               SENS_Test_Setup, SENS_Test_TearDown,
               "Test_SENS_InitConfigTbl_Fail_Nominal");
}
