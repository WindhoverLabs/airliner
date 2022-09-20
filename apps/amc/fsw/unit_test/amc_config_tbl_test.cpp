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
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include "amc_config_tbl_test.hpp"
#include "amc_test_utils.hpp"

/**************************************************************************
 * Tests for AMC InitConfigTbl()
 **************************************************************************/
/**
 * Test AMC InitConfigTbl(), fail TBL Register
 */
void Test_AMC_InitConfigTbl_Fail_TblRegister(void)
{
    AMC  oAMC;

    int32 expected = CFE_TBL_ERR_NO_ACCESS;

    /* fail TBL Register */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), Fail ValidateCfgTbl()
 */
void Test_AMC_InitConfigTbl_Fail_ValidateCfgTbl(void)
{
    AMC  oAMC;

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), ValidateCfgTbl_Nominal
 */
void Test_AMC_InitConfigTbl_ValidateCfgTbl_Nominal(void)
{
    AMC  oAMC;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), fail TBL Load
 */
void Test_AMC_InitConfigTbl_Fail_TblLoad(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    /* fail TBL Load */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}

/**
 * Test AMC InitConfigTbl(), fail TBL Manage
 */
void Test_AMC_InitConfigTbl_Fail_TblManage(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}

/**
 * Test AMC InitConfigTbl(), fail TBL GetAddress
 */
void Test_AMC_InitConfigTbl_Fail_TblGetAddress(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}

/**
 * Test AMC InitConfigTbl(), fail AcquireConfigPtrs
 */
void Test_AMC_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    oAMC.AppMain();
}

/**
 * Test AMC InitConfigTbl(), Nominal
 */
void Test_AMC_InitConfigTbl_Nominal(void)
{
    AMC  oAMC;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}



void AMC_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_TblRegister, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_ValidateCfgTbl, AMC_Test_Setup_CfgTblInvalid, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_ValidateCfgTbl");
    UtTest_Add(Test_AMC_InitConfigTbl_ValidateCfgTbl_Nominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_ValidateCfgTbl_Nominal");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_TblLoad, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_TblManage, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_TblManage");
#if 0   // core dump(same issue with AcquireConfigPointers
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_TblGetAddress, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_AcquireConfigPtrs, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_AcquireConfigPtrs");
#endif
    UtTest_Add(Test_AMC_InitConfigTbl_Nominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Nominal");
}
