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

#include "amc_mixer_config_tbl_test.hpp"
#include "amc_test_utils.hpp"


/**************************************************************************
 * Tests for AMC InitConfigTbl()
 **************************************************************************/
/**
 * Test AMC InitConfigTbl(), fail Mixer TBL Register
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_Fail_MixerTblRegister(void)
{
    AMC  oAMC;

    int32 expected = CFE_TBL_ERR_REGISTRY_FULL;

    /* fail TBL Register */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), fail Mixer TBL Load
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_Fail_MixerTblLoad(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_LOAD_IN_PROGRESS;

    /* fail TBL Load */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), fail Mixer TBL Manage
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_Fail_MixerTblManage(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_INFO_DUMP_PENDING;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), fail Mixer TBL GetAddress
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_Fail_MixerTblGetAddress(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_WARN_DUPLICATE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), fail Mixer AcquireConfigPtrs
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_Fail_MixerAcquireConfigPtrs(void)
{
    AMC  oAMC;

    /* Set a fail result */
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}


/**
 * Test AMC InitConfigTbl(), Mixer Nominal
 * Can not call private function:InitConfigTbl()
 */
void Test_AMC_InitConfigTbl_MixerNominal(void)
{
    AMC  oAMC;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oAMC.AppMain();
}



void AMC_Mixer_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_MixerTblRegister, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_MixerTblRegister");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_MixerTblLoad, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_MixerTblLoad");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_MixerTblManage, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_MixerTblManage");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_MixerTblGetAddress, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_MixerTblGetAddress");
    UtTest_Add(Test_AMC_InitConfigTbl_Fail_MixerAcquireConfigPtrs, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_Fail_MixerAcquireConfigPtrs");
    UtTest_Add(Test_AMC_InitConfigTbl_MixerNominal, AMC_Test_Setup, AMC_Test_TearDown,
               "Test_AMC_InitConfigTbl_MixerNominal");
}


