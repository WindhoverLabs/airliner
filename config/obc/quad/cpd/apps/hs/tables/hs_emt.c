/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "hs_tbl.h"
#include "hs_tbldefs.h"
#include "cfe_tbl_filedef.h"


static CFE_TBL_FileDef_t CFE_TBL_FileDef __attribute__((__used__)) =
{
    "HS_Default_EventMon_Tbl", HS_APP_NAME ".EventMon_Tbl", "HS EventMon Table",
    "hs_emt.tbl", (sizeof(HS_EMTEntry_t) * HS_MAX_MONITORED_EVENTS)
};

/*
 * ActionType:
 *   HS_EMT_ACT_NOACT:        No action is taken.
 *   HS_EMT_ACT_PROC_RESET:   Generates Processor Reset on detection.
 *   HS_EMT_ACT_APP_RESTART:  Attempts to restart application on detection.
 *   HS_EMT_ACT_APP_DELETE:   Deletes application on detection.
 *   HS_EMT_ACT_LAST_NONMSG:  Index for finding end of non-message actions.
 */

HS_EMTEntry_t      HS_Default_EventMon_Tbl[HS_MAX_MONITORED_EVENTS] =
{
/*          AppName                    NullTerm EventID        ActionType */
                                                
/*   0 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   1 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   2 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   3 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   4 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   5 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   6 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   7 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   8 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*   9 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  10 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  11 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  12 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  13 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  14 */ { "",                        0,        0,            HS_EMT_ACT_NOACT },
/*  15 */ { "",                        0,        0,            HS_EMT_ACT_NOACT }

};                               
                                 
/************************/       
/*  End of File Comment */       
/************************/       
