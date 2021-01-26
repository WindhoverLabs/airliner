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

#include <string.h>
#include <stdio.h>
#include "to_app_test.h"
#include "to_app.h"
#include "to_test_utils.h"
#include "to_custom_hooks.h"


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
#include "ut_osapi_hooks.h"
#include "ut_cfe_es_hooks_ex.h"
#include "test_utilities.h"
#include "ut_to_custom_stubs.h"

TO_AppData_t TO_AppData;

int32 TO_MessageFlow_Buildup(TO_ChannelData_t *channel)
{
    return 0;
}

int32 TO_PriorityQueue_BuildupAll(TO_ChannelData_t *channel)
{
    return TO_CHANNEL_PTR_NULL;
}

void TO_Channel_LockByIndex(uint16 index)
{

}

void TO_Channel_UnlockByIndex(uint16 index)
{

}

int32 TO_MessageFlow_TeardownAll(TO_ChannelData_t *channel)
{
    return 0;
}

int32 TO_PriorityQueue_TeardownAll(TO_ChannelData_t *channel)
{
    return 0;
}

osalbool TO_Channel_SBPipe_Dequeue_All(uint16 index)
{
    return TRUE;
}

/**
 * Test TO_ProcessNewConfigTbl(), Nominal
 */
void Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail(void)
{
    int32 result = 0;
    int32 expected = TO_CHANNEL_PTR_NULL;
    TO_ChannelData_t *channel = 0;

    /* Execute the function being tested */
    result = TO_ProcessNewConfigTbl(channel);

    /* Verify results */
    UtAssert_True (result == expected, "Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail");
}




/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void TO_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail, TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail");

}

