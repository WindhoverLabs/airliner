/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "hs_tbl.h"
#include "hs_tbldefs.h"
#include "cfe_tbl_filedef.h"


static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "HS_Default_ExeCount_Tbl", HS_APP_NAME ".ExeCount_Tbl", "HS ExeCount Table",
    "hs_xct_test.tbl", (sizeof(HS_XCTEntry_t) * HS_MAX_EXEC_CNT_SLOTS)
};



HS_XCTEntry_t      HS_Default_ExeCount_Tbl[HS_MAX_EXEC_CNT_SLOTS] =
{
/*          ResourceName               NullTerm ResourceType              */

/*   0 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*   1 */ { "CFE_EVS",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*   2 */ { "CFE_TIME",                0,       HS_XCT_TYPE_APP_MAIN       },
/*   3 */ { "TIME_TONE_TASK",          0,       HS_XCT_TYPE_APP_MAIN       },
/*   4 */ { "TIME_1HZ_TASK",           0,       HS_XCT_TYPE_APP_MAIN       },
/*   5 */ { "CFE_TBL",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*   6 */ { "CFE_SB",                  0,       HS_XCT_TYPE_APP_MAIN       },
/*   7 */ { "CF",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*   8 */ { "CI",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*   9 */ { "CS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  10 */ { "DS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  11 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  12 */ { "FM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  13 */ { "HK",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  14 */ { "HS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  15 */ { "LC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  16 */ { "MD",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  17 */ { "MM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  18 */ { "SC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  19 */ { "SCH",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  20 */ { "TO",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  21 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  22 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  23 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  24 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  25 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  26 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  27 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  28 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  29 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  30 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },
/*  31 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         },

};

/************************/
/*  End of File Comment */
/************************/
