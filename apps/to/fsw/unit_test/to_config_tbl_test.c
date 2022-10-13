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

#include "to_config_tbl_test.h"
#include "to_app.h"
#include "to_test_utils.h"
#include "to_custom_hooks.h"
#include "ut_cfe_es_hooks_ex.h"
#include "ut_osapi_hooks.h"
#include "ut_to_custom_hooks.h"
#include "ut_to_custom_stubs.h"

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


/**************************************************************************
 * Tests for TO Config Utils (Table validation, Managing Tables)
 **************************************************************************/
/**
 * Test TO_InitTables(), Ground_Table_Fail_TBL_Register
 */
void Test_TO_InitTables_Ground_Table_Fail_TBL_Register(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    iStatus = TO_InitTables(channel);

    sprintf(expectedEvent, "Failed to register Config table (0x%08X) for channel %d",
        (unsigned int)iStatus, ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "TO_InitTables, Ground_Table_Fail_TBL_Register");
}


/**
 * Test TO_InitTables(), Dump_Table_Fail_TBL_Register
 */
void Test_TO_InitTables_Dump_Table_Fail_TBL_Register(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 2);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    iStatus = TO_InitTables(channel);

    sprintf(expectedEvent, "Failed to register Dump table (0x%08X) for channel %d",
        (unsigned int)iStatus, ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "TO_InitTables, Dump_Table_Fail_TBL_Register");
}


/**
 * Test TO_InitTables(), Dump_Table_Fail_TBL_Load
 */
void Test_TO_InitTables_Dump_Table_Fail_TBL_Load(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX,  0, 2);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  0, 1);
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  -1, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    iStatus = TO_InitTables(channel);

    sprintf(expectedEvent, "Failed to load Dump Table (0x%08X) for channel %d",
        (unsigned int)iStatus, ChannelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
             "TO_InitTables, Dump_Table_Fail_TBL_Load");
}


/**
 * Test TO_InitTables(), Load_Backup_Table
 */
void Test_TO_InitTables_Load_Backup_Table(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX,  0, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    TO_LoadBackupConfigTable(channel);

    sprintf(expectedEvent,
            "Loaded built-in emergency backup config table! - channel %lu",
            channel->channelIdx);

    /* Verify results */
    UtAssert_EventSent(TO_BACKUP_TABLE_INF_EID, CFE_EVS_INFORMATION,
                       expectedEvent, "TO_InitTables, Load_Backup_Table");
}


/**
 * Test TO_ValidateConfigTbl(), ConfigTblPtr_NULL
 */
void Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->ConfigTblPtr = NULL;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent, "Config table pointer is NULL, table validation ended");

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_NULL_PTR_EID, CFE_EVS_ERROR, expectedEvent,
             "Test_TO_ValidateConfigTbl NULL table pointer - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_NULL_PTR_ERR,
                  "TO_ValidateConfigTbl, ConfigTblPtr_NULL");
}


/**
 * Test TO_ValidateConfigTbl(), InvalidCCSDSVersion
 */
void Test_TO_ValidateConfigTbl_InvalidCCSDSVersion(void)
{
    int32 iStatus = 0;
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    TO_ChannelTbl_t TO_ConfigTable =
    {
        /* Table ID */
        0,
        {
            /* Message Flows */
            {0x0800, 1, 0},
            {0x2801, 1, 0}
        },{
            /* Priority Queues */
            {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  2, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  3, TO_PRIORITY_QUEUE_TYPE_FIFO},
            {TO_PQUEUE_ENA,  4, TO_PRIORITY_QUEUE_TYPE_FIFO}
        }
    };

    iStatus = TO_ValidateConfigTbl(&TO_ConfigTable);

    sprintf(expectedEvent,
            "Invalid CCSDS Version for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
            1, TO_ConfigTable.MessageFlow[1].MsgId, TO_ConfigTable.TableID);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_CCSDS_VER_INVALID_EID, CFE_EVS_ERROR, expectedEvent,
             "Invalid CCSDS Version for table index - event correct");
    UtAssert_True(iStatus == TO_CONFIG_TABLE_RETURN_INVALID_VERSION,
                   "TO_ValidateConfigTbl, InvalidCCSDSVersion");
}


/**
 * Test TO_ValidateConfigTbl(), Priority_Queue_State_Failure
 */
void Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Priority Queue State is invalid (%d), for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State,
            channel->ConfigTblPtr->TableID, PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_STATE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
             "Priority Queue State is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_STATE_ERR,
                  "TO_ValidateConfigTbl, Priority_Queue_State_Failure");
}


/**
 * Test TO_ValidateConfigTbl(), Priority_Queue_Qtype_Failure
 */
void Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint16 PQueueIdx = 0;
    CFE_SB_MsgId_t  MsgId = CFE_ES_HK_TLM_MID;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Priority Queue Qtype is invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType,
            channel->ConfigTblPtr->TableID, PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
             "Priority Queue Qtype is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR,
                  "TO_ValidateConfigTbl, Priority_Queue_Qtype_Failure");
}


/**
 * Test TO_ValidateConfigTbl(), Priority_Queue_MsgLimit_Failure
 */
void Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure(void)
{
    uint16            ChannelIdx = 0;
    uint16            PQueueIdx = 0;
    int32             iStatus = 0;
    TO_ChannelData_t* channel;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].Depth = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Priority Queue Depth invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].Depth,
            channel->ConfigTblPtr->TableID, PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "Priority Queue MsgLimit is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR,
                  "TO_ValidateConfigTbl, Priority_Queue_MsgLimit_Failure");
}


/**
 * Test TO_ValidateConfigTbl(), Priority_Queue_MsgLimit_Failure_Max
 */
void Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max(void)
{
    uint16            ChannelIdx = 0;
    uint16            PQueueIdx = 0;
    int32             iStatus = 0;
    TO_ChannelData_t* channel;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    /* TO_MAX_QUEUE_SIZE_LIMIT = 100 currently */
    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].Depth = TO_MAX_QUEUE_SIZE_LIMIT + 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Priority Queue Depth invalid (%d) for Table ID (%lu) at table index (%d)",
            channel->ConfigTblPtr->PriorityQueue[PQueueIdx].Depth,
            channel->ConfigTblPtr->TableID, PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "Priority Queue MsgLimit is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR,
                  "TO_ValidateConfigTbl, Priority_Queue_MsgLimit_Failure_Max");
}


/**
 * Test TO_ValidateConfigTbl(), Priority_Queue_No_Valid_States
 */
void Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States(void)
{
    uint16            ChannelIdx = 0;
    uint16            PQueueIdx = 0;
    int32             iStatus = 0;
    TO_ChannelData_t* channel;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    /* Execute the function being tested*/
    TO_InitApp();

    /* Get channel data information - channel is only opened */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType = TO_PRIORITY_QUEUE_TYPE_COUNT;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
          "Priority Queue Qtype is invalid (%d) for Table ID (%lu) at table index (%d)",
          channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType,
          channel->ConfigTblPtr->TableID, PQueueIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
              "Priority Queue Qtype is invalid - event correct");
    UtAssert_True(iStatus == TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR,
                  "TO_ValidateConfigTbl, Priority_Queue_No_Valid_States");
}


/**
 * Test TO_ValidateConfigTbl(), Secondary_Header_Absent
 */
void Test_TO_ValidateConfigTbl_Secondary_Header_Absent(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint16 MFlowIdx = 0;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[0].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[1].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[2].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[3].Depth = 100;

    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId    = 0x042;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit = 42;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Secondary Header is absent for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
            MFlowIdx, channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId,
            channel->ConfigTblPtr->TableID);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_SHDR_ABSENT_EID, CFE_EVS_ERROR, expectedEvent,
             "Secondary Header is absent for table index - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER,
                  "TO_ValidateConfigTbl, Secondary_Header_Absent");
}


/**
 * Test TO_ValidateConfigTbl(), MessageFlow_MsgLimit_Not_In_Range
 */
void Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[0].Depth = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[1].Depth = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[2].Depth = TO_MAX_PQ_MSG_SIZE_LIMIT;
    channel->ConfigTblPtr->PriorityQueue[3].Depth = TO_MAX_PQ_MSG_SIZE_LIMIT;

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = CFE_ES_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Message Flow MsgLimit invalid (%u) in Table ID (%lu) at table index (%d) ",
            (unsigned int)channel->ConfigTblPtr->MessageFlow[0].MsgLimit,
            channel->ConfigTblPtr->TableID, 0);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "Message Flow MsgLimit invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR,
                  "TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR");

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = CFE_EVS_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = (TO_MAX_PQ_MSG_SIZE_LIMIT+1);
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 3;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
            "Message Flow MsgLimit invalid (%u) in Table ID (%lu) at table index (%d) ",
            (unsigned int)channel->ConfigTblPtr->MessageFlow[0].MsgLimit,
            channel->ConfigTblPtr->TableID, 0);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "Message Flow MsgLimit invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR,
                  "TO_ValidateConfigTbl, MessageFlow_MsgLimit_Not_In_Range");
}


/**
 * Test TO_ValidateConfigTbl(), MessageFlow_UnusedChecks
 */
void Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[100];

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[0].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[1].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[2].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[3].Depth = 100;

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 0;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 0;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    channel->ConfigTblPtr->MessageFlow[0].MsgId    = 1;
    channel->ConfigTblPtr->MessageFlow[0].MsgLimit = 1;
    channel->ConfigTblPtr->MessageFlow[0].PQueueID = 1;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);
}


/**
 * Test TO_ValidateConfigTbl(), MessageFlow_PQueueId_Invalid
 */
void Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid(void)
{
    uint16  ChannelIdx = 0;
    int32 iStatus = 0;
    TO_ChannelData_t* channel;
    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint16 MFlowIdx = 0;

    /* Set return codes */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, TO_Custom_InitHook);

    TO_InitApp();

    /* Get channel data information */
    channel = &TO_AppData.ChannelData[ChannelIdx];

    channel->ConfigTblPtr->PriorityQueue[0].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[1].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[2].State = TO_PQUEUE_ENA;
    channel->ConfigTblPtr->PriorityQueue[3].State = TO_PQUEUE_ENA;

    channel->ConfigTblPtr->PriorityQueue[0].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[1].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[2].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;
    channel->ConfigTblPtr->PriorityQueue[3].QType = TO_PRIORITY_QUEUE_TYPE_FIFO;

    channel->ConfigTblPtr->PriorityQueue[0].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[1].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[2].Depth = 100;
    channel->ConfigTblPtr->PriorityQueue[3].Depth = 100;

    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgId    = CFE_ES_HK_TLM_MID;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit = 10;
    channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID = 5;

    iStatus = TO_ValidateConfigTbl(channel->ConfigTblPtr);

    sprintf(expectedEvent,
      "Message Flow Priority Queue ID is invalid (%d) for Table ID (%lu) at table index (%d)",
      channel->ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID,
      channel->ConfigTblPtr->TableID, MFlowIdx);

    /* Verify results */
    UtAssert_EventSent(TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR_EID, CFE_EVS_ERROR,
             expectedEvent, "Message Flow Priority Queue ID is invalid - event correct");

    UtAssert_True(iStatus == TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR,
                  "TO_ValidateConfigTbl, MessageFlow_PQueueId_Invalid");
}


/**
 * Test Test_TO_ValidateConfigTbl_Nominal
 */
void Test_TO_ValidateConfigTbl_Nominal(void)
{
    TO_NoArgCmd_t      InSchMsg;
    int32              SchPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("TO_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, TO_SEND_TLM_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, SchPipe);

    /* Set function hook for TO_Custom_Init */
    Ut_TO_Custom_SetFunctionHook(UT_TO_CUSTOM_INIT_INDEX, (void *)&TO_Custom_InitHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    TO_AppMain();
}


/**
 * Test TO_ProcessNewConfigTbl(), PriorityQueueBuildupFail
 */
void Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = TO_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "TO_ProcessNewConfigTbl, PriorityQueueBuildupFail");
}



void TO_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_TO_InitTables_Ground_Table_Fail_TBL_Register,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_InitTables_Ground_Table_Fail_TBL_Register");
    UtTest_Add(Test_TO_InitTables_Dump_Table_Fail_TBL_Register,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_InitTables_Dump_Table_Fail_TBL_Register");
    UtTest_Add(Test_TO_InitTables_Dump_Table_Fail_TBL_Load,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_InitTables_Dump_Table_Fail_TBL_Load");
    UtTest_Add(Test_TO_InitTables_Load_Backup_Table,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_InitTables_Load_Backup_Table");

    UtTest_Add(Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_ConfigTblPtr_NULL");
    UtTest_Add(Test_TO_ValidateConfigTbl_InvalidCCSDSVersion,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_InvalidCCSDSVersion");
    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Priority_Queue_State_Failure");
    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Priority_Queue_Qtype_Failure");
    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure");
    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Priority_Queue_MsgLimit_Failure_Max");
    UtTest_Add(Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Priority_Queue_No_Valid_States");
    UtTest_Add(Test_TO_ValidateConfigTbl_Secondary_Header_Absent,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Secondary_Header_Absent");
    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_MessageFlow_MsgLimit_Not_In_Range");
    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_MessageFlow_UnusedChecks");
    UtTest_Add(Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_MessageFlow_PQueueId_Invalid");
    UtTest_Add(Test_TO_ValidateConfigTbl_Nominal,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ValidateConfigTbl_Nominal");

    UtTest_Add(Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail,
               TO_Test_Setup_FullConfig1, TO_Test_TearDown,
               "Test_TO_ProcessNewConfigTbl_PriorityQueueBuildupFail");
}
