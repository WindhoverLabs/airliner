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

#include "to_test_utils.h"
#include "to_custom_hooks.h"
#include "to_platform_cfg.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "to_table_names.h"
#include "ut_osapi_hooks.h"

#include <time.h>
#include "ut_to_custom_stubs.h"


/*
 * Config table for testing
 */
#define TO_PQUEUE_SINGLE_PASS_IDX       0
#define TO_PQUEUE_HIGH_OPS_RSRVD_IDX    1
#define TO_PQUEUE_HIGH_IDX              2
#define TO_PQUEUE_MEDIUM_IDX            3
#define TO_PQUEUE_LOW_IDX               4


TO_ChannelTbl_t TO_EmptyConfigTable =
{
    /* Table ID */
    0,
    {
        /* Message Flows */
        {0, 0, 0}
    },{
        /* Priority Queues */
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_SINGLE},
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO}
    }
};


TO_ChannelTbl_t TO_NoPQueueConfigTable =
{
    /* Table ID */
    0,
    {
        /* Message Flows */
        {0, 0, 0}
    },{
        /* Priority Queues */
    }
};


TO_ChannelTbl_t TO_FullConfigTable1 =
{
    /* Table ID */
    1,
    {
        /* Message Flows */
        {CFE_ES_HK_TLM_MID,         1,  TO_PQUEUE_HIGH_IDX},
        {CFE_EVS_HK_TLM_MID,        1,  TO_PQUEUE_HIGH_IDX},
        {CFE_SB_HK_TLM_MID,         1,  TO_PQUEUE_HIGH_IDX},
        {CFE_TBL_HK_TLM_MID,        1,  TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_HK_TLM_MID,       1,  TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_DIAG_TLM_MID,     1,  TO_PQUEUE_MEDIUM_IDX},
        {CFE_EVS_EVENT_MSG_MID,     32, TO_PQUEUE_LOW_IDX},
        {CFE_SB_STATS_TLM_MID,      1,  TO_PQUEUE_LOW_IDX},
        {CFE_ES_APP_TLM_MID,        1,  TO_PQUEUE_LOW_IDX}
    },{
        /* Priority Queues */
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_SINGLE},
        {TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA,  2, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA,  3, TO_PRIORITY_QUEUE_TYPE_FIFO},
        {TO_PQUEUE_ENA,  4, TO_PRIORITY_QUEUE_TYPE_FIFO}
    }
};


TO_ChannelTbl_t TO_FullConfigTable2 =
{
	/* Table ID */
	1,
	{
		/* Message Flows */
		{CFE_ES_HK_TLM_MID,			1,	TO_PQUEUE_HIGH_IDX},
		{CFE_EVS_HK_TLM_MID,		1,	TO_PQUEUE_HIGH_IDX},
		{CFE_SB_HK_TLM_MID,			1,	TO_PQUEUE_HIGH_IDX},
		{CFE_TBL_HK_TLM_MID,		1,	TO_PQUEUE_MEDIUM_IDX},
		{CFE_TIME_HK_TLM_MID,		1,	TO_PQUEUE_MEDIUM_IDX},
		{CFE_TIME_DIAG_TLM_MID,		1,	TO_PQUEUE_MEDIUM_IDX},
		{CFE_EVS_EVENT_MSG_MID,		32, TO_PQUEUE_LOW_IDX},
		{CFE_SB_STATS_TLM_MID,		1,	TO_PQUEUE_LOW_IDX},
		{CFE_ES_APP_TLM_MID,		1,	TO_PQUEUE_LOW_IDX}
	},{
		/* Priority Queues */
		{TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_SINGLE},
		{TO_PQUEUE_ENA, 10, TO_PRIORITY_QUEUE_TYPE_FIFO},
		{TO_PQUEUE_ENA,  4, TO_PRIORITY_QUEUE_TYPE_FIFO},
		{TO_PQUEUE_ENA,  3, TO_PRIORITY_QUEUE_TYPE_FIFO},
		{TO_PQUEUE_ENA,  2, TO_PRIORITY_QUEUE_TYPE_FIFO}
	}
};



/*
 * Function Definitions
 */

void TO_Test_Setup_NoConfig(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));

    Ut_TO_Custom_Reset();

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    
    /* Clear queues, semaphores, etc and call init */
    Ut_OS_API_Clear();
}


void TO_Test_Setup_EmptyConfig(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));

    Ut_TO_Custom_Reset();

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    
    /* Clear queues, semaphores, etc and call init */
    Ut_OS_API_Clear();

    Ut_CFE_TBL_AddTable(TO_CONFIG_TABLE_FILENAME, (void *) &TO_EmptyConfigTable);
}


void TO_Test_Setup_NoPQueueConfig(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));

    Ut_TO_Custom_Reset();

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(TO_CONFIG_TABLE_FILENAME, (void *) &TO_NoPQueueConfigTable);
}


void TO_Test_Setup_FullConfig1(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));

    Ut_TO_Custom_Reset();
    
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    
    /* Clear queues, semaphores, etc and call init */
    Ut_OS_API_Clear();

    Ut_CFE_TBL_AddTable(TO_CONFIG_TABLE_FILENAME, (void *) &TO_FullConfigTable1);
}


void TO_Test_Setup_FullConfig2(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));

    Ut_TO_Custom_Reset();

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    
    /* Clear queues, semaphores, etc and call init */
    Ut_OS_API_Clear();

    Ut_CFE_TBL_AddTable(TO_CONFIG_TABLE_FILENAME, (void *) &TO_FullConfigTable2);
}



void TO_Test_TearDown(void) 
{
    CFE_PSP_MemSet(&TO_AppData, 0x00, sizeof(TO_AppData_t));
}
