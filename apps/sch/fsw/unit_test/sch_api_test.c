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

#include "sch_api_test.h"
#include "sch_test_utils.h"

#include "sch_app.h"
#include "sch_apipriv.h"

#include "uttest.h"
#include "utassert.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"


/*
 * SCH_LibInit_Test
 */
void SCH_LibInit_Test(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = SCH_LibInit();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR,
                  "SCH_LibData.ProcessingDisabledCtr == SCH_LIB_DIS_CTR");

    UtAssert_True(Result == OS_SUCCESS, "Result == OS_SUCCESS");
}


/*
 * SCH_EnableProcessing_Test
 */
void SCH_EnableProcessing_Test(void)
{
    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    SCH_EnableProcessing();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == 0,
                  "SCH_LibData.ProcessingDisabledCtr == 0");
}


/*
 * SCH_DisableProcessing_Test
 */
void SCH_DisableProcessing_Test(void)
{
    /* Execute the function being tested */
    SCH_DisableProcessing();
    
    /* Verify results */
    UtAssert_True(SCH_LibData.ProcessingDisabledCtr == 1,
                  "SCH_LibData.ProcessingDisabledCtr == 1");
}


/*
 * SCH_GetProcessingState_Test_True
 */
void SCH_GetProcessingState_Test_True(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 0;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetProcessingState_Test_True */

void SCH_GetProcessingState_Test_False(void)
{
    boolean   Result;

    SCH_LibData.ProcessingDisabledCtr = 1;

    /* Execute the function being tested */
    Result = SCH_GetProcessingState();
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetProcessingState_Test_False */

void SCH_API_Test_AddTestCases(void)
{
    UtTest_Add(SCH_LibInit_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_LibInit_Test");

    UtTest_Add(SCH_EnableProcessing_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableProcessing_Test");

    UtTest_Add(SCH_DisableProcessing_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableProcessing_Test");

    UtTest_Add(SCH_GetProcessingState_Test_True, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetProcessingState_Test_True");
    UtTest_Add(SCH_GetProcessingState_Test_False, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetProcessingState_Test_False");

} /* end SCH_API_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
