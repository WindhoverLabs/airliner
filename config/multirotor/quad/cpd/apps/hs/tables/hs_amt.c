/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "hs_tbl.h"
#include "hs_tbldefs.h"
#include "cfe_tbl_filedef.h"


static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "HS_Default_AppMon_Tbl", HS_APP_NAME ".AppMon_Tbl", "HS AppMon Table",
    "hs_amt.tbl", (sizeof(HS_AMTEntry_t) * HS_MAX_MONITORED_APPS)
};

/*
 * ActionType:
 *   HS_AMT_ACT_NOACT:        No action is taken.
 *   HS_AMT_ACT_PROC_RESET:   Generates Processor Reset on failure.
 *   HS_AMT_ACT_APP_RESTART:  Attempts to restart application on failure.
 *   HS_AMT_ACT_EVENT:        Generates event message on failure.
 *   HS_AMT_ACT_LAST_NONMSG:  Index for finding end of non-message actions.
 */

HS_AMTEntry_t      HS_Default_AppMon_Tbl[HS_MAX_MONITORED_APPS] =
{
/*          AppName                    NullTerm CycleCount     ActionType */

/*   0 */ { "AMC",                     0,      251,            HS_AMT_ACT_EVENT       },
/*   1 */ { "CF",                      0,        2,            HS_AMT_ACT_EVENT       },
/*   2 */ { "CFE_ES",                  0,        2,            HS_AMT_ACT_PROC_RESET  },
/*   3 */ { "CFE_EVS",                 0,        2,            HS_AMT_ACT_PROC_RESET  },
/*   4 */ { "CFE_SB",                  0,        1,            HS_AMT_ACT_PROC_RESET  },
/*   5 */ { "CFE_TBL",                 0,        1,            HS_AMT_ACT_PROC_RESET  },
/*   6 */ { "CFE_TIME",                0,        1,            HS_AMT_ACT_PROC_RESET  },
/*   7 */ { "CI",                      0,      252,            HS_AMT_ACT_APP_RESTART },
/*   8 */ { "CS",                      0,        1,            HS_AMT_ACT_EVENT       },
/*   9 */ { "MAC",                     0,      251,            HS_AMT_ACT_EVENT       },
/*  10 */ { "FM",                      0,        2,            HS_AMT_ACT_EVENT       },
/*  11 */ { "MPC",                     0,      251,            HS_AMT_ACT_EVENT       },
/*  12 */ { "HS",                      0,        2,            HS_AMT_ACT_EVENT       },
/*  13 */ { "ICM20689",                0,      251,            HS_AMT_ACT_APP_RESTART },
/*  14 */ { "LD",                      0,      251,            HS_AMT_ACT_EVENT       },
/*  15 */ { "MD",                      0,        2,            HS_AMT_ACT_EVENT       },
/*  16 */ { "MM",                      0,        2,            HS_AMT_ACT_EVENT       },
/*  17 */ { "MS5611",                  0,      251,            HS_AMT_ACT_APP_RESTART },
/*  18 */ { "NAV",                     0,      251,            HS_AMT_ACT_EVENT       },
/*  19 */ { "PE",                      0,      126,            HS_AMT_ACT_EVENT       },
/*  20 */ { "QAE",                     0,      251,            HS_AMT_ACT_EVENT       },
/*  21 */ { "RCIN",                    0,       51,            HS_AMT_ACT_APP_RESTART },
/*  22 */ { "SBND",                    0,      251,            HS_AMT_ACT_EVENT       },
/*  23 */ { "SC",                      0,        2,            HS_AMT_ACT_EVENT       },
/*  24 */ { "SCH",                     0,      251,            HS_AMT_ACT_EVENT       },
/*  25 */ { "SENS",                    0,      251,            HS_AMT_ACT_APP_RESTART },
/*  26 */ { "TO",                      0,      251,            HS_AMT_ACT_APP_RESTART },
/*  27 */ { "VM",                      0,      251,            HS_AMT_ACT_EVENT       },
/*  28 */ { "ZUSUP",                   0,      251,            HS_AMT_ACT_EVENT       },
/*  29 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  30 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  31 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       }

};

/************************/
/*  End of File Comment */
/************************/
