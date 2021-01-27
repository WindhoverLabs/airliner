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



HS_EMTEntry_t      HS_Default_EventMon_Tbl[HS_MAX_MONITORED_EVENTS] =
{
/*          AppName                    NullTerm EventID        ActionType */
                                                
/*   0 */ { "CFE_ES",                  0,       10,            HS_EMT_ACT_NOACT },
/*   1 */ { "CFE_EVS",                 0,       10,            HS_EMT_ACT_NOACT },
/*   2 */ { "CFE_TIME",                0,       10,            HS_EMT_ACT_NOACT },
/*   3 */ { "CFE_TBL",                 0,       10,            HS_EMT_ACT_NOACT },
/*   4 */ { "CFE_SB",                  0,       10,            HS_EMT_ACT_NOACT },
/*   5 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*   6 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*   7 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*   8 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*   9 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  10 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  11 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  12 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  13 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  14 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
/*  15 */ { "",                        0,       10,            HS_EMT_ACT_NOACT },
                                 
};                               
                                 
/************************/       
/*  End of File Comment */       
/************************/       
