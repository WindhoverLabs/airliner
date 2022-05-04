/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe_tbl_filedef.h"
#include "to_tbldefs.h"
#include "msg_ids.h"

/************************************************************************
 ** Defines
 *************************************************************************/

#define TO_PQUEUE_SINGLE_PASS_IDX               0
#define TO_PQUEUE_HIGH_OPS_RSRVD_IDX            1
#define TO_PQUEUE_HIGH_IDX                      2
#define TO_PQUEUE_EVENTS_IDX                    3
#define TO_PQUEUE_CFDP_IDX                      4
#define TO_PQUEUE_MEDIUM_IDX                    5
#define TO_PQUEUE_SHELL_IDX                     6
#define TO_PQUEUE_LOW_IDX                       7

/**
 **  \brief Default TO config table data
 */
TO_ChannelTbl_t TO_BackupConfigTbl =
{
    /* Table ID */
    1,
    {
        /* Message Flows */
        {TO_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
        {TO_DIAG_TLM_MID,              1, TO_PQUEUE_MEDIUM_IDX},
        {TO_DIAG_MSG_FLOW_MID,         1, TO_PQUEUE_MEDIUM_IDX},
        {CI_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_HK_TLM_MID,            1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_EVS_HK_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_SB_HK_TLM_MID,            1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TBL_HK_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_HK_TLM_MID,          1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_DIAG_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_EVS_EVENT_MSG_MID,       32, TO_PQUEUE_EVENTS_IDX},
        {CFE_SB_STATS_TLM_MID,         1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_APP_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TBL_REG_TLM_MID,          1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_SB_ONESUB_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_SHELL_TLM_MID,        32, TO_PQUEUE_SHELL_IDX},
        {CFE_ES_MEMSTATS_TLM_MID,      1, TO_PQUEUE_MEDIUM_IDX},
  },{
        /* Priority Queues */
        /* TO_PQUEUE_SINGLE_PASS_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_SINGLE},
        /* TO_PQUEUE_HIGH_OPS_RSRVD_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_HIGH_IDX */
        {TO_PQUEUE_ENA, 4, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_EVENTS_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_CFDP_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_MEDIUM_IDX */
        {TO_PQUEUE_ENA, 16, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_SHELL_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_LOW_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_FIFO}
  }
};


/************************/
/*  End of File Comment */
/************************/
