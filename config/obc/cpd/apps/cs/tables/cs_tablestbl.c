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
    /*   4 */ { CS_STATE_ENABLED,   "FM.FreeSpace"    },
    /*   5 */ { CS_STATE_ENABLED,   "SCH.MSG_DEFS"    },
    /*   6 */ { CS_STATE_ENABLED,   "SCH.SCHED_DEF"   },
    /*   7 */ { CS_STATE_ENABLED,   "TO.UDP_CFG"      },
    /*   8 */ { CS_STATE_EMPTY,     ""                },
    /*   9 */ { CS_STATE_EMPTY,     ""                },
    /*  10 */ { CS_STATE_EMPTY,     ""                },
    /*  11 */ { CS_STATE_EMPTY,     ""                },
    /*  12 */ { CS_STATE_EMPTY,     ""                },
    /*  13 */ { CS_STATE_EMPTY,     ""                },
    /*  14 */ { CS_STATE_EMPTY,     ""                },
    /*  15 */ { CS_STATE_EMPTY,     ""                }

};

/*
** Table file header
*/
CFE_TBL_FILEDEF(CS_TablesTable, CS.DefTablesTbl, CS Tables Tbl, cs_tablestbl.tbl)

/************************/
/*  End of File Comment */
/************************/
