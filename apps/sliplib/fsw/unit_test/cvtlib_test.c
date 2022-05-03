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

#include "slipliblib_test.h"
#include "sliplib_lib.h"
#include "slipliblib_test_utils.h"

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



void Test_SLIPLIBLIB_GetContainer_Nominal(void)
{
	int32 status = 0;
	uint32 size1 = 100;
	uint32 size2 = 200;
	uint32 size3 = 300;
	SLIPLIB_ContainerID_t id1;
	SLIPLIB_ContainerID_t id2;
	SLIPLIB_ContainerID_t id3;
	SLIPLIB_ContainerID_t id4;

	status = SLIPLIB_GetContainer(1, size1, &id1);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(2, size2, &id2);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(3, size3, &id3);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(1, size1, &id4);
    UtAssert_True(status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");
    UtAssert_True(id4 == id1, "SLIPLIB_GetContainer did not return the correct ID");
}


void Test_SLIPLIBLIB_GetContainer_ExhaustEntries(void)
{
	int32 status = 0;
	uint32 size = 100;
	SLIPLIB_ContainerID_t id;
	uint32 i = 0;

	for(i = 1; i <= SLIPLIB_MAX_REGISTRATIONS; ++i)
	{
		status = SLIPLIB_GetContainer(i, size, &id);
	    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");
	}

	status = SLIPLIB_GetContainer(i, size, &id);
    UtAssert_True (status == SLIPLIB_REGISTRY_FULL, "SLIPLIB_GetContainer did not return SLIPLIB_REGISTRY_FULL");
}


void Test_SLIPLIBLIB_GetContainer_ExhaustDatastore(void)
{
	int32 status = 0;
	uint32 size1 = SLIPLIB_DATA_STORE_SIZE - 1;
	uint32 size2 = 1;
	uint32 size3 = 1;
	uint32 i = 0;
	SLIPLIB_ContainerID_t id1;
	SLIPLIB_ContainerID_t id2;
	SLIPLIB_ContainerID_t id3;

	status = SLIPLIB_GetContainer(1, size1, &id1);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(2, size2, &id2);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(3, size3, &id3);
    UtAssert_True (status == SLIPLIB_DATASTORE_FULL, "SLIPLIB_GetContainer did not return SLIPLIB_DATASTORE_FULL");
}


void Test_SLIPLIBLIB_GetContainer_IncorrectSize(void)
{
	int32 status = 0;
	uint32 size1 = 100;
	uint32 size2 = 200;
	uint32 size3 = 300;
	SLIPLIB_ContainerID_t id1;
	SLIPLIB_ContainerID_t id2;
	SLIPLIB_ContainerID_t id3;
	SLIPLIB_ContainerID_t id4;

	status = SLIPLIB_GetContainer(1, size1, &id1);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(2, size2, &id2);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(3, size3, &id3);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_GetContainer(1, size2, &id4);
    UtAssert_True(status == SLIPLIB_INCORRECT_SIZE, "SLIPLIB_GetContainer did not return SLIPLIB_INCORRECT_SIZE");
}


void Test_SLIPLIBLIB_SetContent_Nominal(void)
{
	int32 status = 0;
	char bufferExpected[] = "A quick brown fox jumped over the lazy dog.\n";
	uint32 size = sizeof(bufferExpected);
	char actualBuffer[sizeof(bufferExpected)];
	SLIPLIB_ContainerID_t id;
	uint32 updateCount = 0;

	status = SLIPLIB_GetContainer(1, size, &id);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_SetContent(id, (void*) &bufferExpected[0], size);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_SetContainer did not return success");
}


void Test_SLIPLIBLIB_GetContent_Nominal(void)
{
	int32 status = 0;
	char bufferExpected[] = "A quick brown fox jumped over the lazy dog.\n";
	uint32 size = sizeof(bufferExpected);
	char actualBuffer[sizeof(bufferExpected)];
	SLIPLIB_ContainerID_t id;
	uint32 updateCount = 0;

	memset(actualBuffer, 0, sizeof(actualBuffer));

	status = SLIPLIB_GetContainer(1, size, &id);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");

	status = SLIPLIB_SetContent(id, (void*) &bufferExpected[0], size);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_SetContainer did not return success");

	status = SLIPLIB_GetContent(id, &updateCount, (void*) &actualBuffer[0], &size);
    UtAssert_True (status == SLIPLIB_SUCCESS, "SLIPLIB_GetContainer did not return success");
    UtAssert_MemCmp(actualBuffer, bufferExpected, sizeof(actualBuffer), "Contents of actual buffer do not match expected.");
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SLIPLIBLib_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_SLIPLIBLIB_GetContainer_Nominal, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_GetContainer_Nominal");

    UtTest_Add(Test_SLIPLIBLIB_GetContainer_ExhaustEntries, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_GetContainer_ExhaustEntries");

    UtTest_Add(Test_SLIPLIBLIB_GetContainer_IncorrectSize, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_GetContainer_IncorrectSize");

    UtTest_Add(Test_SLIPLIBLIB_GetContainer_ExhaustDatastore, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_GetContainer_ExhaustDatastore");

    UtTest_Add(Test_SLIPLIBLIB_SetContent_Nominal, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_SetContent_Nominal");

    UtTest_Add(Test_SLIPLIBLIB_GetContent_Nominal, SLIPLIBLIB_Test_Setup, SLIPLIBLIB_Test_TearDown,
               "Test_SLIPLIBLIB_GetContent_Nominal");

}


