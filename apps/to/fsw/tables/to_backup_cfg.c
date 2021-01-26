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
#define TO_PQUEUE_MEDIUM_IDX                    3
#define TO_PQUEUE_LOW_IDX                       4

/**
 **  \brief Default TO config table data
 */
TO_ChannelTbl_t TO_BackupConfigTbl =
{
    /* Table ID */
    0xffffffff,
    {
        /* Message Flows */
        /* Ground Queues */
        {TO_HK_TLM_MID,                1, TO_PQUEUE_HIGH_IDX},
        {TO_DATA_TYPE_MID,             1, TO_PQUEUE_HIGH_IDX},
        {TO_DIAG_TLM_MID,              1, TO_PQUEUE_HIGH_IDX},
        {CFE_ES_HK_TLM_MID,            1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_EVS_HK_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_SB_HK_TLM_MID,            1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TBL_HK_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_HK_TLM_MID,          1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TIME_DIAG_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_EVS_EVENT_MSG_MID,       32, TO_PQUEUE_MEDIUM_IDX},
        {CFE_SB_STATS_TLM_MID,         1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_APP_TLM_MID,           1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_TBL_REG_TLM_MID,          1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_SB_ONESUB_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_SHELL_TLM_MID,        32, TO_PQUEUE_MEDIUM_IDX},
        {CFE_ES_MEMSTATS_TLM_MID,      1, TO_PQUEUE_MEDIUM_IDX},
        {CF_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
        {CF_TRANS_TLM_MID,             1, TO_PQUEUE_MEDIUM_IDX},
        {CF_CONFIG_TLM_MID,            1, TO_PQUEUE_MEDIUM_IDX},
        {CF_SPACE_TO_GND_PDU_MID,     32, TO_PQUEUE_MEDIUM_IDX},
        {FM_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
        {FM_FILE_INFO_TLM_MID,         1, TO_PQUEUE_MEDIUM_IDX},
        {FM_DIR_LIST_TLM_MID,          1, TO_PQUEUE_MEDIUM_IDX},
        {FM_OPEN_FILES_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {FM_FREE_SPACE_TLM_MID,        1, TO_PQUEUE_MEDIUM_IDX},
        {MD_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
        {MM_HK_TLM_MID,                1, TO_PQUEUE_MEDIUM_IDX},
  },{
        /* Priority Queues */
        /* TO_PQUEUE_SINGLE_PASS_IDX */
        {TO_PQUEUE_ENA, 100, TO_PRIORITY_QUEUE_TYPE_SINGLE},
        /* TO_PQUEUE_HIGH_OPS_RSRVD_IDX */
        {TO_PQUEUE_ENA, 100, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_HIGH_IDX */
        {TO_PQUEUE_ENA, 100, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_MEDIUM_IDX */
        {TO_PQUEUE_ENA, 100, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_LOW_IDX */
        {TO_PQUEUE_ENA, 100, TO_PRIORITY_QUEUE_TYPE_FIFO}
  }
};


/************************/
/*  End of File Comment */
/************************/
