/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe_tbl_filedef.h"
#include "pq_structs.h"
#include "msg_ids.h"


/************************************************************************
 ** Defines
 *************************************************************************/

#define PQ_PQUEUE_SINGLE_PASS_IDX               0
#define PQ_PQUEUE_HIGH_OPS_RSRVD_IDX            1
#define PQ_PQUEUE_HIGH_IDX                      2
#define PQ_PQUEUE_MEDIUM_IDX                    3
#define PQ_PQUEUE_DEFAULT_IDX                   4
#define PQ_PQUEUE_LOW_IDX                       5

/**
 **  \brief Default TO config table data
 */
PQ_ChannelTbl_t PQ_BackupConfigTbl =
{
    /* Table ID */
    0,
    {
        /* Message Flows */
        /* Ground Queues */
        {SBN_SUB_MID,                 64, PQ_PQUEUE_MEDIUM_IDX},
        {SBN_UNSUB_MID,               64, PQ_PQUEUE_MEDIUM_IDX},
        {SBN_ALLSUB_MID,               1, PQ_PQUEUE_HIGH_IDX},
        {CFE_ES_HK_TLM_MID,            1, PQ_PQUEUE_LOW_IDX},
        {CFE_EVS_HK_TLM_MID,           1, PQ_PQUEUE_LOW_IDX},
        {CFE_SB_HK_TLM_MID,            1, PQ_PQUEUE_LOW_IDX},
        {CFE_TBL_HK_TLM_MID,           1, PQ_PQUEUE_LOW_IDX},
        {CFE_TIME_HK_TLM_MID,          1, PQ_PQUEUE_LOW_IDX},
        {CFE_TIME_DIAG_TLM_MID,        1, PQ_PQUEUE_LOW_IDX},
        {CFE_EVS_EVENT_MSG_MID,       32, PQ_PQUEUE_LOW_IDX},
        {CFE_SB_STATS_TLM_MID,         1, PQ_PQUEUE_LOW_IDX},
        {CFE_ES_APP_TLM_MID,           1, PQ_PQUEUE_LOW_IDX},
        {CFE_TBL_REG_TLM_MID,          1, PQ_PQUEUE_LOW_IDX},
        {CFE_ES_SHELL_TLM_MID,        32, PQ_PQUEUE_LOW_IDX},
        {CFE_ES_MEMSTATS_TLM_MID,      1, PQ_PQUEUE_LOW_IDX},
        {CF_HK_TLM_MID,                1, PQ_PQUEUE_LOW_IDX},
        {CF_TRANS_TLM_MID,             1, PQ_PQUEUE_LOW_IDX},
        {CF_CONFIG_TLM_MID,            1, PQ_PQUEUE_LOW_IDX},
        {CF_CPD_TO_PPD_PDU_MID,       32, PQ_PQUEUE_LOW_IDX},
        {FM_HK_TLM_MID,                1, PQ_PQUEUE_LOW_IDX},
        {FM_FILE_INFO_TLM_MID,         1, PQ_PQUEUE_LOW_IDX},
        {FM_DIR_LIST_TLM_MID,          1, PQ_PQUEUE_LOW_IDX},
        {FM_OPEN_FILES_TLM_MID,        1, PQ_PQUEUE_LOW_IDX},
        {FM_FREE_SPACE_TLM_MID,        1, PQ_PQUEUE_LOW_IDX},
        {MD_HK_TLM_MID,                1, PQ_PQUEUE_LOW_IDX},
        {MM_HK_TLM_MID,                1, PQ_PQUEUE_LOW_IDX},
  },{
        /* Priority Queues */
        /* PQ_PQUEUE_SINGLE_PASS_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_SINGLE},
        /* PQ_PQUEUE_HIGH_OPS_RSRVD_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_FIFO},
        /* PQ_PQUEUE_HIGH_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_FIFO},
        /* PQ_PQUEUE_MEDIUM_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_FIFO},
        /* PQ_PQUEUE_DEFAULT_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_FIFO},
        /* PQ_PQUEUE_LOW_IDX */
        {PQ_PQUEUE_ENA, 100, PQ_PRIORITY_QUEUE_TYPE_FIFO}
  }
};


/************************/
/*  End of File Comment */
/************************/
