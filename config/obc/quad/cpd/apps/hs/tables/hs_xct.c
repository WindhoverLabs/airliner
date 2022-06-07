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

/*
 * ActionType:
 *   HS_XCT_TYPE_NOTYPE:     No type.
 *   HS_XCT_TYPE_APP_MAIN:   Counter for Application Main task.
 *   HS_XCT_TYPE_APP_CHILD:  Counter for Application Child task.
 *   HS_XCT_TYPE_DEVICE:     Counter for Device Driver.
 *   HS_XCT_TYPE_ISR:        Counter for Interrupt Service Routine.
 */

HS_XCTEntry_t      HS_Default_ExeCount_Tbl[HS_MAX_EXEC_CNT_SLOTS] =
{
/*          ResourceName               NullTerm ResourceType              */
/*   0 */ { "AMC",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*   1 */ { "CF",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*   2 */ { "CFE_ES",                  0,       HS_XCT_TYPE_APP_MAIN       },
/*   3 */ { "ES_PerfFileWriter",       0,       HS_XCT_TYPE_APP_CHILD      },
/*   4 */ { "CFE_EVS",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*   5 */ { "CFE_SB",                  0,       HS_XCT_TYPE_APP_MAIN       },
/*   6 */ { "CFE_TBL",                 0,       HS_XCT_TYPE_APP_MAIN       },
/*   7 */ { "CFE_TIME",                0,       HS_XCT_TYPE_APP_MAIN       },
/*   8 */ { "TIME_TONE_TASK",          0,       HS_XCT_TYPE_APP_MAIN       },
/*   9 */ { "TIME_1HZ_TASK",           0,       HS_XCT_TYPE_APP_MAIN       },
/*  10 */ { "CI",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  11 */ { "CI_LISTENER",             0,       HS_XCT_TYPE_APP_CHILD      },
/*  12 */ { "CS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  13 */ { "CS_RecmpOSTsk",           0,       HS_XCT_TYPE_APP_CHILD      },
/*  14 */ { "CS_RecmpCfeCoreTsk",      0,       HS_XCT_TYPE_APP_CHILD      },
/*  15 */ { "CS_RecmpMemoryTsk",       0,       HS_XCT_TYPE_APP_CHILD      },
/*  16 */ { "CS_RecmpEepromTsk",       0,       HS_XCT_TYPE_APP_CHILD      },
/*  17 */ { "CS_RecmpAppTsk",          0,       HS_XCT_TYPE_APP_CHILD      },
/*  18 */ { "CS_RecmpTableTsk",        0,       HS_XCT_TYPE_APP_CHILD      },
/*  19 */ { "CS_OneShotTask",          0,       HS_XCT_TYPE_APP_CHILD      },
/*  20 */ { "FM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  21 */ { "FM_CHILD_TASK",           0,       HS_XCT_TYPE_APP_CHILD      },
/*  22 */ { "HS",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  23 */ { "HS_IDLE_TASK",            0,       HS_XCT_TYPE_APP_CHILD      },
/*  24 */ { "LD",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  25 */ { "MAC",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  26 */ { "MD",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  27 */ { "MM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  28 */ { "MPC",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  29 */ { "NAV",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  30 */ { "PE",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  31 */ { "QAE",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  32 */ { "SBN",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  33 */ { "sbn_recvs_0",             0,       HS_XCT_TYPE_APP_CHILD      },
/*  34 */ { "sbn_recv_0",              0,       HS_XCT_TYPE_APP_CHILD      },
/*  35 */ { "PQ_OUTCH_0",              0,       HS_XCT_TYPE_APP_CHILD      },
/*  36 */ { "SC",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  37 */ { "SCH",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  38 */ { "SCH_AD_TASK",             0,       HS_XCT_TYPE_APP_CHILD      },
/*  39 */ { "SED",                     0,       HS_XCT_TYPE_APP_MAIN       },
/*  40 */ { "SED_EVENTS",              0,       HS_XCT_TYPE_APP_CHILD      },
/*  41 */ { "TO",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  42 */ { "TO_OUTCH_0",              0,       HS_XCT_TYPE_APP_CHILD      },
/*  43 */ { "VM",                      0,       HS_XCT_TYPE_APP_MAIN       },
/*  44 */ { "",                        0,       HS_XCT_TYPE_NOTYPE         }

};

/************************/
/*  End of File Comment */
/************************/
