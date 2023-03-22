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

#include "ld_msg.h"
#include "ld_version.h"

#include "ld_config_tbl_test.hpp"
#include "ld_test_utils.hpp"

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
 * Tests for LD InitConfigTbl()
 **************************************************************************/
/**
 * Test LD InitConfigTbl(), fail TBL Register
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_TblRegister(void)
{
    LD       oLDut;

    /* Set a fail result */
    int32    result = CFE_SUCCESS;
    int32    expected = CFE_TBL_ERR_NO_ACCESS;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Register");
}


/**
 * Test LD InitConfigTbl(), fail TBL Load
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_TblLoad(void)
{
    LD       oLDut;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Load");
}


/**
 * Test LD InitConfigTbl(), fail TBL Manage
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_TblManage(void)
{
    LD       oLDut;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Manage");
}


/**
 * Test LD InitConfigTbl(), fail TBL GetAddress
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_TblGetAddress(void)
{
    LD       oLDut;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL GetAddress");
}


/**
 * Test LD InitConfigTbl(), fail AcquireConfigPtrs
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
    LD       oLDut;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLDut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail AcquireConfigPtrs");
}


/**
 * Test LD InitConfigTbl(), fail ValidateConfigTbl
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Fail_ValidateConfigTbl(void)
{
    LD       oLDut;

    int32           result = CFE_SUCCESS;
    int32           expected = -1;
    float           tmp_val;
    LD_ConfigTbl_t* pConfigTbl = NULL;

    /* Execute the function being tested */
    result = oLDut.InitApp();

    if(result == CFE_SUCCESS)
    {
        pConfigTbl = oLDut.ConfigTblPtr;

        tmp_val = pConfigTbl->LD_Z_VEL_MAX;
        pConfigTbl->LD_Z_VEL_MAX = LD_Z_VEL_MAX_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_Z_VEL_MAX)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_Z_VEL_MAX = tmp_val;
        tmp_val = pConfigTbl->LD_XY_VEL_MAX;
        pConfigTbl->LD_XY_VEL_MAX = LD_XY_VEL_MAX_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_XY_VEL_MAX)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_XY_VEL_MAX = tmp_val;
        tmp_val = pConfigTbl->LD_ALT_MAX;
        pConfigTbl->LD_ALT_MAX = LD_ALT_MAX_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_ALT_MAX)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_ALT_MAX = tmp_val;
        tmp_val = pConfigTbl->LD_LOW_T_THR;
        pConfigTbl->LD_LOW_T_THR = LD_LOW_T_THR_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_LOW_T_THR)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_LOW_T_THR = tmp_val;
        tmp_val = pConfigTbl->LD_MAN_MIN_THR;
        pConfigTbl->LD_MAN_MIN_THR = LD_MAN_MIN_THR_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_MAN_MIN_THR)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_MAN_MIN_THR = tmp_val;
        tmp_val = pConfigTbl->LD_POS_STK_UP_THRES;
        pConfigTbl->LD_POS_STK_UP_THRES = LD_POS_STK_UP_THRES_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
            "InitConfigTbl(), fail ValidateConfigTbl(LD_POS_STK_UP_THRES)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_POS_STK_UP_THRES = tmp_val;
        tmp_val = pConfigTbl->LD_POS_STK_DW_THRES;
        pConfigTbl->LD_POS_STK_DW_THRES = LD_POS_STK_DW_THRES_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
            "InitConfigTbl(), fail ValidateConfigTbl(LD_POS_STK_DW_THRES)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_POS_STK_DW_THRES = tmp_val;
        tmp_val = pConfigTbl->LD_LANDSPEED;
        pConfigTbl->LD_LANDSPEED = LD_LANDSPEED_MAX + 1.0f;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                 "InitConfigTbl(), fail ValidateConfigTbl(LD_LANDSPEED)");

        result = CFE_SUCCESS;
        pConfigTbl->LD_LANDSPEED = tmp_val;
        tmp_val = pConfigTbl->LD_MIN_THR_NO_ALT_TIMEOUT;
        pConfigTbl->LD_MIN_THR_NO_ALT_TIMEOUT =
                                    LD_MIN_THR_NO_ALT_TIMEOUT_MAX + 1;
        result = oLDut.ValidateConfigTbl(pConfigTbl);
        UtAssert_True(result == expected,
                      "InitConfigTbl(), "
                      "fail ValidateConfigTbl(LD_MIN_THR_NO_ALT_TIMEOUT)");

        pConfigTbl->LD_MIN_THR_NO_ALT_TIMEOUT = tmp_val;

        CFE_TBL_Update(oLDut.ConfigTblHdl);
    }
    else
    {
        UtAssert_True(FALSE, "InitConfigTbl(), fail ValidateConfigTbl");
    }
}


/**
 * Test LD InitConfigTbl(), Nominal
 *   Called InitApp() instead of calling private function InitConfigTbl()
 */
void Test_LD_InitConfigTbl_Nominal(void)
{
    LD       oLDut;

    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLDut.InitApp();

    sprintf(expectedEvent, "Initialized.  Version %d.%d.%d.%d",
            LD_MAJOR_VERSION, LD_MINOR_VERSION,
            LD_REVISION, LD_MISSION_REV);

    /* Verify results */
    UtAssert_True (result == expected, "InitConfigTbl, Nominal");

    UtAssert_EventSent(LD_INIT_INF_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "InitConfigTbl, Nominal: Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void LD_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_LD_InitConfigTbl_Fail_TblRegister,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_LD_InitConfigTbl_Fail_TblLoad,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_LD_InitConfigTbl_Fail_TblManage,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_TblManage");
    UtTest_Add(Test_LD_InitConfigTbl_Fail_TblGetAddress,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_LD_InitConfigTbl_Fail_AcquireConfigPtrs,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_LD_InitConfigTbl_Fail_ValidateConfigTbl,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Fail_ValidateConfigTbl");
    UtTest_Add(Test_LD_InitConfigTbl_Nominal,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitConfigTbl_Nominal");
}
