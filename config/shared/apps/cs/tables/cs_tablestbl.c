/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "cs_platform_cfg.h"
#include "cs_msgdefs.h"
#include "cs_tbldefs.h"
#include "cfe_tbl_filedef.h"

CS_Def_Tables_Table_Entry_t      CS_TablesTable[CS_MAX_NUM_TABLES_TABLE_ENTRIES] =
{
    /*         State             Name   */
    /*   0 */ { CS_STATE_ENABLED,   "CF.ConfigTable"  },
    /*   1 */ { CS_STATE_ENABLED,   "CI.CONFIG_TBL"   },
    /*   2 */ { CS_STATE_ENABLED,   "CS.DefAppTbl"    },
    /*   3 */ { CS_STATE_ENABLED,   "CS.DefMemoryTbl" },
    /*   4 */ { CS_STATE_ENABLED,   "DS.FILE_TBL"     },
    /*   5 */ { CS_STATE_ENABLED,   "DS.FILTER_TBL"   },
    /*   6 */ { CS_STATE_ENABLED,   "FM.FreeSpace"    },
    /*   7 */ { CS_STATE_ENABLED,   "HK.CopyTable"    },
    /*   8 */ { CS_STATE_ENABLED,   "HS.AppMon_Tbl"   },
    /*   9 */ { CS_STATE_ENABLED,   "HS.EventMon_Tbl" },
    /*  10 */ { CS_STATE_ENABLED,   "HS.MsgActs_Tbl"  },
    /*  11 */ { CS_STATE_ENABLED,   "HS.ExeCount_Tbl" },
    /*  12 */ { CS_STATE_ENABLED,   "LC.LC_ADT"       },
    /*  13 */ { CS_STATE_ENABLED,   "LC.LC_WDT"       },
    /*  14 */ { CS_STATE_ENABLED,   "SCH.MSG_DEFS"    },
    /*  15 */ { CS_STATE_ENABLED,   "SCH.SCHED_DEF"   },
    /*  16 */ { CS_STATE_ENABLED,   "TO.UDP_CFG"      },
    /*  17 */ { CS_STATE_EMPTY,     ""                },
    /*  18 */ { CS_STATE_EMPTY,     ""                }

};

/*
** Table file header
*/
CFE_TBL_FILEDEF(CS_TablesTable, CS.DefTablesTbl, CS Tables Tbl, cs_tablestbl.tbl)

/************************/
/*  End of File Comment */
/************************/
