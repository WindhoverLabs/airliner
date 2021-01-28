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
    "hs_xct.tbl", (sizeof(HS_XCTEntry_t) * HS_MAX_EXEC_CNT_SLOTS)
};



HS_XCTEntry_t      HS_Default_ExeCount_Tbl[HS_MAX_EXEC_CNT_SLOTS] =
{
/*          ResourceName               NullTerm ResourceType              */

/*   0 */ { "CFE_ES",                  0,       HS_XCT_TYPE_APP_MAIN       },
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
/*  11 */ { "EA",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  12 */ { "FM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  13 */ { "HK",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  14 */ { "HS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  15 */ { "LC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  16 */ { "MD",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  17 */ { "MM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  18 */ { "SC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  19 */ { "SCH",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  20 */ { "TO",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  21 */ { "VC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  22 */ { "MAC",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  23 */ { "AMC",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  24 */ { "PX4BR",                   0,       HS_XCT_TYPE_APP_MAIN       },
/*  25 */ { "SENS",                    0,       HS_XCT_TYPE_APP_MAIN       },
/*  26 */ { "MPU9250",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*  27 */ { "HMC5883",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*  28 */ { "ULR",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  29 */ { "GPS",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  30 */ { "MS5611",                  0,       HS_XCT_TYPE_APP_MAIN       },
/*  31 */ { "SIM",                     0,       HS_XCT_TYPE_APP_MAIN       }

};

/************************/
/*  End of File Comment */
/************************/
