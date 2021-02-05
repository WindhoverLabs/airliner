#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"
#include "msg_ids.h"

SBN_RemapTbl_t SBN_RemapTbl =
{ 
    SBN_REMAP_DEFAULT_IGNORE, /* Remap Default */
    0, /* number of entries, initialized at validation time */
    {  /* remap table */
        /* {CPU_ID, from, to} and if to is 0x0000, filter rather than remap */
        {1, CFE_ES_HK_TLM_MID,       CFE_ES_HK_TLM_MID},
        {1, CFE_EVS_HK_TLM_MID,      CFE_EVS_HK_TLM_MID},
        {1, CFE_SB_HK_TLM_MID,       CFE_SB_HK_TLM_MID},
        {1, CFE_TBL_HK_TLM_MID,      CFE_TBL_HK_TLM_MID},
        {1, CFE_TIME_HK_TLM_MID,     CFE_TIME_HK_TLM_MID},
        {1, CFE_TIME_DIAG_TLM_MID,   CFE_TIME_DIAG_TLM_MID},
        {1, CFE_EVS_EVENT_MSG_MID,   CFE_EVS_EVENT_MSG_MID},
        {1, CFE_SB_STATS_TLM_MID,    CFE_SB_STATS_TLM_MID},
        {1, CFE_ES_APP_TLM_MID,      CFE_ES_APP_TLM_MID},
        {1, CFE_TBL_REG_TLM_MID,     CFE_TBL_REG_TLM_MID},
        {1, CFE_ES_SHELL_TLM_MID,    CFE_ES_SHELL_TLM_MID},
        {1, CFE_ES_MEMSTATS_TLM_MID, CFE_ES_MEMSTATS_TLM_MID}
    }
};/* end SBN_RemapTbl */

CFE_TBL_FILEDEF(SBN_RemapTbl, SBN.REMAP_TBL, SBN Remap Table,sbn_remap_tbl.tbl)
