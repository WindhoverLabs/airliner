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


#include "sch_config_tbl_test.h"
#include "sch_test_utils.h"

#include "sch_app.h"
#include "sch_cmds.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
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
 * Tests for SCH_AcquirePointers()
 **************************************************************************/
/**
 * SCH_AcquirePointers_Test_Fail_TblManage
 */
void SCH_AcquirePointers_Test_Fail_TblManage(void)
{
}


/**
 * SCH_AcquirePointers_Test_Fail_TblGetAddress
 */
void SCH_AcquirePointers_Test_Fail_TblGetAddress(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_NEVER_LOADED;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, Expected, 1);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();
   
    /* Verify results */
    UtAssert_True(Result == Expected,
                  "AcquirePointers_Test_Fail_TblGetAddress");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
}


void SCH_AcquirePointers_Test_AllResultsSuccess(void)
{
    int32   Result;

    /* Set to fail condition "Result > CFE_SUCCESS", both times */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

}


void SCH_AcquirePointers_Test_Result1LessThanSuccess(void)
{
    int32   Result;

    /* Set to fail first instance of condition "Result > CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();

    /* Verify results */
    UtAssert_True(Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

}



void SCH_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(SCH_AcquirePointers_Test_Fail_TblGetAddress,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_AcquirePointers_Test_Fail_TblGetAddress");
    UtTest_Add(SCH_AcquirePointers_Test_AllResultsSuccess,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_AcquirePointers_Test_AllResultsSuccess");
    UtTest_Add(SCH_AcquirePointers_Test_Result1LessThanSuccess,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_AcquirePointers_Test_Result1LessThanSuccess");
}
