#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"
#include "msg_ids.h"

SBN_RemapTbl_t SBN_RemapTbl =
{
    SBN_REMAP_DEFAULT_SEND, /* Remap Default */
    0, /* number of entries, initialized at validation time */
    {  /* remap table */
        /* {CPU_ID, from, to} and if to is 0x0000, filter rather than remap */
        {0, 0x0000,       0x0000}
    }
};/* end SBN_RemapTbl */

CFE_TBL_FILEDEF(SBN_RemapTbl, SBN.REMAP_TBL, SBN Remap Table,sbn_remap_tbl.tbl)
