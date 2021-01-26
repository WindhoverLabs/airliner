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

#include "cvtlib_test.h"
#include "cvt_lib.h"
#include "cvtlib_test_utils.h"

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



void Test_CVTLIB_GetContainer_Nominal(void)
{
	int32 status = 0;
	uint32 size1 = 100;
	uint32 size2 = 200;
	uint32 size3 = 300;
	CVT_ContainerID_t id1;
	CVT_ContainerID_t id2;
	CVT_ContainerID_t id3;
	CVT_ContainerID_t id4;

	status = CVT_GetContainer(1, size1, &id1);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(2, size2, &id2);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(3, size3, &id3);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(1, size1, &id4);
    UtAssert_True(status == CVT_SUCCESS, "CVT_GetContainer did not return success");
    UtAssert_True(id4 == id1, "CVT_GetContainer did not return the correct ID");
}


void Test_CVTLIB_GetContainer_ExhaustEntries(void)
{
	int32 status = 0;
	char name[CVT_CONTAINER_NAME_LENGTH];
	uint32 size = 100;
	CVT_ContainerID_t id;
	uint32 i = 0;

	for(i = 1; i <= CVT_MAX_REGISTRATIONS; ++i)
	{
		status = CVT_GetContainer(i, size, &id);
	    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");
	}

	status = CVT_GetContainer(i, size, &id);
    UtAssert_True (status == CVT_REGISTRY_FULL, "CVT_GetContainer did not return CVT_REGISTRY_FULL");
}


void Test_CVTLIB_GetContainer_ExhaustDatastore(void)
{
	int32 status = 0;
	uint32 size1 = CVT_DATA_STORE_SIZE - 1;
	uint32 size2 = 1;
	uint32 size3 = 1;
	uint32 i = 0;
	CVT_ContainerID_t id1;
	CVT_ContainerID_t id2;
	CVT_ContainerID_t id3;

	status = CVT_GetContainer(1, size1, &id1);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(2, size2, &id2);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(3, size3, &id3);
    UtAssert_True (status == CVT_DATASTORE_FULL, "CVT_GetContainer did not return CVT_DATASTORE_FULL");
}


void Test_CVTLIB_GetContainer_IncorrectSize(void)
{
	int32 status = 0;
	uint32 size1 = 100;
	uint32 size2 = 200;
	uint32 size3 = 300;
	CVT_ContainerID_t id1;
	CVT_ContainerID_t id2;
	CVT_ContainerID_t id3;
	CVT_ContainerID_t id4;

	status = CVT_GetContainer(1, size1, &id1);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(2, size2, &id2);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(3, size3, &id3);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_GetContainer(1, size2, &id4);
    UtAssert_True(status == CVT_INCORRECT_SIZE, "CVT_GetContainer did not return CVT_INCORRECT_SIZE");
}


void Test_CVTLIB_SetContent_Nominal(void)
{
	int32 status = 0;
	char bufferExpected[] = "A quick brown fox jumped over the lazy dog.\n";
	uint32 size = sizeof(bufferExpected);
	char actualBuffer[sizeof(bufferExpected)];
	CVT_ContainerID_t id;
	uint32 updateCount = 0;

	status = CVT_GetContainer(1, size, &id);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_SetContent(id, (void*) &bufferExpected[0], size);
    UtAssert_True (status == CVT_SUCCESS, "CVT_SetContainer did not return success");
}


void Test_CVTLIB_GetContent_Nominal(void)
{
	int32 status = 0;
	char bufferExpected[] = "A quick brown fox jumped over the lazy dog.\n";
	uint32 size = sizeof(bufferExpected);
	char actualBuffer[sizeof(bufferExpected)];
	CVT_ContainerID_t id;
	uint32 updateCount = 0;

	memset(actualBuffer, 0, sizeof(actualBuffer));

	status = CVT_GetContainer(1, size, &id);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");

	status = CVT_SetContent(id, (void*) &bufferExpected[0], size);
    UtAssert_True (status == CVT_SUCCESS, "CVT_SetContainer did not return success");

	status = CVT_GetContent(id, &updateCount, (void*) &actualBuffer[0], &size);
    UtAssert_True (status == CVT_SUCCESS, "CVT_GetContainer did not return success");
    UtAssert_MemCmp(actualBuffer, bufferExpected, sizeof(actualBuffer), "Contents of actual buffer do not match expected.");
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CVTLib_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_CVTLIB_GetContainer_Nominal, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_GetContainer_Nominal");

    UtTest_Add(Test_CVTLIB_GetContainer_ExhaustEntries, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_GetContainer_ExhaustEntries");

    UtTest_Add(Test_CVTLIB_GetContainer_IncorrectSize, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_GetContainer_IncorrectSize");

    UtTest_Add(Test_CVTLIB_GetContainer_ExhaustDatastore, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_GetContainer_ExhaustDatastore");

    UtTest_Add(Test_CVTLIB_SetContent_Nominal, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_SetContent_Nominal");

    UtTest_Add(Test_CVTLIB_GetContent_Nominal, CVTLIB_Test_Setup, CVTLIB_Test_TearDown,
               "Test_CVTLIB_GetContent_Nominal");

}


