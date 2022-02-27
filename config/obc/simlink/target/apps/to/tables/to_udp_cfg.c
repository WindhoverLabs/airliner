/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe_tbl_filedef.h"
#include "to_tbldefs.h"
#include "msg_ids.h"

/**
 ** \brief The cFE TO config table definition.
 **
 ** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
 **    ObjName - variable name of config table, e.g., TO_ConfigDefTbl[]
 **    TblName - app's table name, e.g., TO.CONFIG_TBL, where TO is the same app name
 **              used in cfe_es_startup.scr, and TO_defConfigTbl is the same table
 **              name passed in to CFE_TBL_Register()
 **    Desc - description of table in string format
 **    TgtFileName[20] - table file name, compiled as .tbl file extension
 **    ObjSize - size of the entire table
 **
 */
static CFE_TBL_FileDef_t CFE_TBL_FileDef OS_USED =
{
  "TO_ConfigTbl", "TO.UDP_CFG", "TO ground dev udp table",
  "to_udp_cfg.tbl", (sizeof(TO_ChannelTbl_t))
};


/************************************************************************
 ** Defines
 *************************************************************************/

#define TO_PQUEUE_SINGLE_PASS_IDX               8
#define TO_PQUEUE_HIGH_OPS_RSRVD_IDX            9
#define TO_PQUEUE_HIGH_IDX                     10
#define TO_PQUEUE_EVENTS_IDX                   11
#define TO_PQUEUE_HK_IDX                       13
#define TO_PQUEUE_SHELL_IDX                    14

/**
 **  \brief Default TO config table data
 */
TO_ChannelTbl_t TO_ConfigTbl =
{
    /* Table ID */
    1,
    {
        /* Message Flows */
        {CFE_ES_APP_TLM_MID,                     1, TO_PQUEUE_HK_IDX},
        {CFE_ES_HK_TLM_MID,                      1, TO_PQUEUE_HK_IDX},
        {CFE_ES_MEMSTATS_TLM_MID,                1, TO_PQUEUE_HK_IDX},
        {CFE_ES_SHELL_TLM_MID,                  32, TO_PQUEUE_SHELL_IDX},
        {CFE_EVS_EVENT_MSG_MID,                 32, TO_PQUEUE_EVENTS_IDX},
        {CFE_EVS_HK_TLM_MID,                     1, TO_PQUEUE_HK_IDX},
        {CFE_SB_HK_TLM_MID,                      1, TO_PQUEUE_HK_IDX},
        {CFE_SB_STATS_TLM_MID,                   1, TO_PQUEUE_HK_IDX},
        {CFE_TBL_HK_TLM_MID,                     1, TO_PQUEUE_HK_IDX},
        {CFE_TBL_REG_TLM_MID,                    1, TO_PQUEUE_HK_IDX},
        {CFE_TIME_HK_TLM_MID,                    1, TO_PQUEUE_HK_IDX},
        {CFE_TIME_DIAG_TLM_MID,                  1, TO_PQUEUE_HK_IDX},
        {CI_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {DS_DIAG_TLM_MID,                        1, TO_PQUEUE_HK_IDX},
        {DS_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {HS_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {MD_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {MM_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {SCH_DIAG_TLM_MID,                       1, TO_PQUEUE_HK_IDX},
        {SCH_HK_TLM_MID,                         1, TO_PQUEUE_HK_IDX},
        {TO_DATA_TYPE_MID,                       1, TO_PQUEUE_HK_IDX},
        {TO_DIAG_MSG_FLOW_MID,                   1, TO_PQUEUE_HK_IDX},
        {TO_DIAG_TLM_MID,                        1, TO_PQUEUE_HK_IDX},
        {TO_HK_TLM_MID,                          1, TO_PQUEUE_HK_IDX},
        {UBLOX_M8N_HK_TLM_MID,                   1, TO_PQUEUE_HK_IDX},
        {HMC5883_HK_TLM_MID,                     1, TO_PQUEUE_HK_IDX},
        {ICM20689_HK_TLM_MID,                    1, TO_PQUEUE_HK_IDX},
        {MS5611_HK_TLM_MID,                      1, TO_PQUEUE_HK_IDX},
        {RFD900X_HK_TLM_MID,                     1, TO_PQUEUE_HK_IDX}
  },{
        /* Priority Queues */
        /* TO_PQUEUE_SINGLE_PASS_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_SINGLE},
        /* TO_PQUEUE_HIGH_OPS_RSRVD_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_HIGH_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_EVENTS_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_HK_IDX */
        {TO_PQUEUE_ENA, 50, TO_PRIORITY_QUEUE_TYPE_FIFO},
        /* TO_PQUEUE_SHELL_IDX */
        {TO_PQUEUE_ENA, 64, TO_PRIORITY_QUEUE_TYPE_FIFO}
  }
};


/************************/
/*  End of File Comment */
/************************/
