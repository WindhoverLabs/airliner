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

/*   0 */ { "AMC",                     0,       10,            HS_AMT_ACT_EVENT       },
/*   1 */ { "CF",                      0,       10,            HS_AMT_ACT_EVENT       },
/*   2 */ { "CFE_ES",                  0,       10,            HS_AMT_ACT_PROC_RESET  },
/*   3 */ { "CFE_EVS",                 0,       10,            HS_AMT_ACT_PROC_RESET  },
/*   4 */ { "CFE_SB",                  0,       10,            HS_AMT_ACT_PROC_RESET  },
/*   5 */ { "CFE_TBL",                 0,       10,            HS_AMT_ACT_PROC_RESET  },
/*   6 */ { "CFE_TIME",                0,       10,            HS_AMT_ACT_PROC_RESET  },
/*   7 */ { "CI",                      0,       10,            HS_AMT_ACT_APP_RESTART },
/*   8 */ { "CS",                      0,       10,            HS_AMT_ACT_EVENT       },
/*   9 */ { "FM",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  10 */ { "HS",                      0,       10,            HS_AMT_ACT_APP_RESTART },
/*  11 */ { "LD",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  12 */ { "MAC",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  13 */ { "MD",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  14 */ { "MM",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  15 */ { "MPC",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  16 */ { "NAV",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  17 */ { "PE",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  18 */ { "QAE",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  19 */ { "SBN",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  20 */ { "SC",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  21 */ { "SCH",                     0,       10,            HS_AMT_ACT_EVENT       },
/*  22 */ { "SED",                     0,       10,            HS_AMT_ACT_APP_RESTART },
/*  23 */ { "TO",                      0,       10,            HS_AMT_ACT_APP_RESTART },
/*  24 */ { "VM",                      0,       10,            HS_AMT_ACT_EVENT       },
/*  25 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  26 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  27 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  28 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  29 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  30 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       },
/*  31 */ { "",                        0,       10,            HS_AMT_ACT_NOACT       }

};

/************************/
/*  End of File Comment */
/************************/
