#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"
#include "msg_ids.h"

SBN_RemapTbl_t SBN_RemapTbl =
{ 
    SBN_REMAP_DEFAULT_IGNORE, /* Remap Default */
    0, /* number of entries, initialized at validation time */
    {  /* remap table */
        /* {CPU_ID, from, to} and if to is 0x0000, filter rather than remap */
        {2, CFE_CMD_MID_BASE_CPU2 + CFE_EVS_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_EVS_CMD_MSG},
        {2, CFE_CMD_MID_BASE_CPU2 + CFE_SB_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_SB_CMD_MSG},
        {2, CFE_CMD_MID_BASE_CPU2 + CFE_TBL_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_TBL_CMD_MSG},
	{2, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_CMD_MSG},
	{2, CFE_CMD_MID_BASE_CPU2 + CFE_ES_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_ES_CMD_MSG},
	{2, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_TONE_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_TONE_CMD_MSG},
	{2, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_1HZ_CMD_MSG, CFE_CMD_MID_BASE_CPU2 + CFE_TIME_1HZ_CMD_MSG},
	/* Time commands */
	{1, CFE_TIME_DATA_CMD_MID, CFE_TIME_DATA_CMD_MID},
	{1, CFE_TIME_FAKE_CMD_MID, CFE_TIME_FAKE_CMD_MID},
	{1, CFE_TIME_SEND_CMD_MID, CFE_TIME_SEND_CMD_MID}
    }
};/* end SBN_RemapTbl */

CFE_TBL_FILEDEF(SBN_RemapTbl, SBN.REMAP_TBL, SBN Remap Table,sbn_remap_tbl.tbl)
