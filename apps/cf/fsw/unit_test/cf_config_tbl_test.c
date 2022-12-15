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

#include "cf_config_tbl_test.h"
#include "cf_test_utils.h"

#include "cf_events.h"

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
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"


/**************************************************************************
 * Tests for CF_TableInit()
 **************************************************************************/
/**
 * Test CF_TableInit(), fail TBL Register
 */
void Test_CF_TableInit_Fail_TblRegister(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_TableInit();

    sprintf(expEvent, "Error Registering Config Table,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "TableInit(), fail TBL Register");

    UtAssert_EventSent(CF_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "TableInit(), fail TBL Register: Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValFlightEntityId
 */
void Test_CF_TableInit_Fail_TblValFlightEntityId(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    strcpy(CF_AppData.Tbl->FlightEntityId, "1234567890");

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Cannot set FlightEntityId to %s, "
            "must be 2 byte, dotted decimal fmt like 0.24",
            CF_AppData.Tbl->FlightEntityId);
    sprintf(expEventVal, "Total Validation Errors - %d for "
            "CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "TableInit(), fail TblValFlightEntityId");

    UtAssert_EventSent(CF_TBL_VAL_ERR1_EID, CFE_EVS_ERROR, expEventErr,
           "TableInit(), fail TblValFlightEntityId: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
         "TableInit(), fail TblValFlightEntityId: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValIncomingMsgId
 */
void Test_CF_TableInit_Fail_TblValIncomingMsgId(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->InCh[0].IncomingPDUMsgId = 0xFFFF;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Cannot set IncomingPDUMsgId 0x%X > "
                         "CFE_SB_HIGHEST_VALID_MSGID 0x%X",
                         CF_AppData.Tbl->InCh[0].IncomingPDUMsgId,
                         CFE_SB_HIGHEST_VALID_MSGID);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValIncomingMsgId");

    UtAssert_EventSent(CF_TBL_VAL_ERR2_EID, CFE_EVS_ERROR, expEventErr,
         "CF TableInit(), fail TblValIncomingMsgId: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
         "CF TableInit(), fail TblValIncomingMsgId: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValOutgoingFileChunk
 */
void Test_CF_TableInit_Fail_TblValOutgoingFileChunk(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    strcpy(CF_AppData.Tbl->OutgoingFileChunkSize, "1234567890");

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Cannot set OUTGOING_FILE_CHUNK_SIZE(%d) > "
                         "CF_MAX_OUTGOING_CHUNK_SIZE (%d)",
                         atoi(CF_AppData.Tbl->OutgoingFileChunkSize),
                         CF_MAX_OUTGOING_CHUNK_SIZE);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValOutgoingFileChunk");

    UtAssert_EventSent(CF_TBL_VAL_ERR3_EID, CFE_EVS_ERROR, expEventErr,
        "CF TableInit(), fail TblValOutgoingFileChunk: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
        "CF TableInit(), fail TblValOutgoingFileChunk: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValChanInUse
 */
void Test_CF_TableInit_Fail_TblValChanInUse(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].EntryInUse = 3;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Ch%d EntryInUse %d must be 0-unused or 1-in use",
            0, CF_AppData.Tbl->OuCh[0].EntryInUse);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValChanInUse");

    UtAssert_EventSent(CF_TBL_VAL_ERR4_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValChanInUse: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValChanInUse: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValDequeEnable
 */
void Test_CF_TableInit_Fail_TblValDequeEnable(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].DequeueEnable = 2;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d DequeueEnable %d must be 0-disabled or 1-enabled",
            0, CF_AppData.Tbl->OuCh[0].DequeueEnable);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValDequeEnable");

    UtAssert_EventSent(CF_TBL_VAL_ERR5_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValDequeEnable: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValDequeEnable: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValOutgoingMsgId
 */
void Test_CF_TableInit_Fail_TblValOutgoingMsgId(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].OutgoingPduMsgId = 0xFFFF;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Cannot set Ch%d OutgoingPduMsgId 0x%X > "
                         "CFE_SB_HIGHEST_VALID_MSGID 0x%X",
                         0,
                         CF_AppData.Tbl->OuCh[0].OutgoingPduMsgId,
                         CFE_SB_HIGHEST_VALID_MSGID);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValOutgoingMsgId");

    UtAssert_EventSent(CF_TBL_VAL_ERR7_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValOutgoingMsgId: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
          "CF TableInit(), fail TblValOutgoingMsgId: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollDirInUse
 */
void Test_CF_TableInit_Fail_TblValPollDirInUse(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].PollDir[0].EntryInUse = 3;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d,PollDir%d EntryInUse %d must be 0-unused or 1-in use",
            0, 0, CF_AppData.Tbl->OuCh[0].PollDir[0].EntryInUse);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollDirInUse");

    UtAssert_EventSent(CF_TBL_VAL_ERR8_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollDirInUse: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollDirInUse: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollEnable
 */
void Test_CF_TableInit_Fail_TblValPollEnable(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].PollDir[0].EnableState = 2;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d,PollDir%d EnableState %d must be 0-disabled or 1-enabled",
            0, 0, CF_AppData.Tbl->OuCh[0].PollDir[0].EnableState);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollEnable");

    UtAssert_EventSent(CF_TBL_VAL_ERR9_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollEnable: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollEnable: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollClass
 */
void Test_CF_TableInit_Fail_TblValPollClass(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].PollDir[0].Class = 0;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Chan %d,PollDir%d Class %d must be 1-unreliable or 2-reliable",
            0, 0, CF_AppData.Tbl->OuCh[0].PollDir[0].Class);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollClass");

    UtAssert_EventSent(CF_TBL_VAL_ERR10_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollClass: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollClass: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollPreserve
 */
void Test_CF_TableInit_Fail_TblValPollPreserve(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    CF_AppData.Tbl->OuCh[0].PollDir[0].Preserve = 3;

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d,PollDir%d Preserve %d must be 0-delete or 1-preserve",
            0, 0, CF_AppData.Tbl->OuCh[0].PollDir[0].Preserve);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollPreserve");

    UtAssert_EventSent(CF_TBL_VAL_ERR11_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollPreserve: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollPreserve: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollSrcPath
 */
void Test_CF_TableInit_Fail_TblValPollSrcPath(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    strcpy(CF_AppData.Tbl->OuCh[0].PollDir[0].SrcPath,"/NoSlashAtEnd");

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d,PollSrcPath must be terminated,have no spaces,slash at end",
            0);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollSrcPath");

    UtAssert_EventSent(CF_TBL_VAL_ERR12_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollSrcPath: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollSrcPath: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPollDstPath
 */
void Test_CF_TableInit_Fail_TblValPollDstPath(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    strcpy(CF_AppData.Tbl->OuCh[0].PollDir[0].DstPath,"/No Spaces Allowed");

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr,
            "Ch%d,PollDstPath must be terminated and have no spaces", 0);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPollDstPath");

    UtAssert_EventSent(CF_TBL_VAL_ERR13_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPollDstPath: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPollDstPath: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblValPeerEntityId
 */
void Test_CF_TableInit_Fail_TblValPeerEntityId(void)
{
    /* Set a fail result */
    int32           result = CFE_SUCCESS;
    int32           expected = CF_ERROR;
    char  expEventErr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventVal[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    CF_TableInit();

    /* Corrupt table param */
    strcpy(CF_AppData.Tbl->OuCh[0].PollDir[0].PeerEntityId, "12");

    result = CF_ValidateCFConfigTable(CF_AppData.Tbl);

    sprintf(expEventErr, "Cannot set Ch%d Poll %d PeerEntityId to %s,"
                         "must be 2 byte, dotted decimal fmt like 0.24",
                         0, 0,
                         CF_AppData.Tbl->OuCh[0].PollDir[0].PeerEntityId);
    sprintf(expEventVal,
            "Total Validation Errors - %d for CF Configuration Table", 1);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF TableInit(), fail TblValPeerEntityId");

    UtAssert_EventSent(CF_TBL_VAL_ERR6_EID, CFE_EVS_ERROR, expEventErr,
             "CF TableInit(), fail TblValPeerEntityId: Error Event Sent");

    UtAssert_EventSent(CF_TBL_VAL_ERR14_EID, CFE_EVS_ERROR, expEventVal,
             "CF TableInit(), fail TblValPeerEntityId: Validate Event Sent");
}


/**
 * Test CF_TableInit(), fail TblLoad
 */
void Test_CF_TableInit_Fail_TblLoad(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_TableInit();

    sprintf(expEvent, "Error Loading Config Table,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "TableInit(), fail TblLoad");

    UtAssert_EventSent(CF_CFGTBL_LD_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "TableInit(), fail TblLoad: Event Sent");
}


/**
 * Test CF_TableInit(), fail TblManage
 */
void Test_CF_TableInit_Fail_TblManage(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NO_ACCESS;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_TableInit();

    sprintf(expEvent,
            "Error from TBL Manage call for Config Table,RC=0x%08X",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF TableInit(), fail TblManage");

    UtAssert_EventSent(CF_CFGTBL_MNG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF TableInit(), fail TblManage: Event Sent");
}


/**
 * Test CF_TableInit(), fail TblGetAdr
 */
void Test_CF_TableInit_Fail_TblGetAdr(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_TableInit();

    sprintf(expEvent, "Error Getting Adr for Config Tbl,RC=0x%08X",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF TableInit(), fail TblGetAdr");

    UtAssert_EventSent(CF_CFGTBL_GADR_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF TableInit(), fail TblGetAdr: Event Sent");
}


/**
 * Test CF_TableInit(), Nominal
 */
void Test_CF_TableInit_Nominal(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = CF_TableInit();

    /* Verify results */
    UtAssert_True(result == expected, "CF TableInit(), Nominal");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CF_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_CF_TableInit_Fail_TblRegister,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblRegister");

    UtTest_Add(Test_CF_TableInit_Fail_TblValFlightEntityId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValFlightEntityId");
    UtTest_Add(Test_CF_TableInit_Fail_TblValIncomingMsgId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValIncomingMsgId");
    UtTest_Add(Test_CF_TableInit_Fail_TblValOutgoingFileChunk,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValOutgoingFileChunk");
    UtTest_Add(Test_CF_TableInit_Fail_TblValChanInUse,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValChanInUse");
    UtTest_Add(Test_CF_TableInit_Fail_TblValDequeEnable,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValDequeEnable");
    UtTest_Add(Test_CF_TableInit_Fail_TblValOutgoingMsgId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValOutgoingMsgId");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollDirInUse,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollDirInUse");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollEnable,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollEnable");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollClass,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollClass");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollPreserve,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollPreserve");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollSrcPath,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollSrcPath");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPollDstPath,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPollDstPath");
    UtTest_Add(Test_CF_TableInit_Fail_TblValPeerEntityId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblValPeerEntityId");

    UtTest_Add(Test_CF_TableInit_Fail_TblLoad,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblLoad");
    UtTest_Add(Test_CF_TableInit_Fail_TblManage,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblManage");
    UtTest_Add(Test_CF_TableInit_Fail_TblGetAdr,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Fail_TblGetAdr");
    UtTest_Add(Test_CF_TableInit_Nominal,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_TableInit_Nominal");
}
