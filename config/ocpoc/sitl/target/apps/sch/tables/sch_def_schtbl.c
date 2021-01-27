#ifdef __cplusplus
extern "C" {
#endif

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "sch_platform_cfg.h"
#include "sch_msgdefs.h"
#include "sch_tbldefs.h"

#include "sch_grpids.h"
#include "sch_tblidx.h"

/*
 * Rate Groups for 1Hz Major frame and 8Hz minor frame (slots)
 *

Comments/rationale:
- Distribute the "send KH" commands throughout
- Send the CFE HK together (not required)
- TO sends telemetry late into the major frame at 2Hz rate

                                rate, slot(s)

CFE_ES_SEND_HK_MIDX             1Hz, 0
CFE_EVS_SEND_HK_MIDX            1Hz, 0
CFE_SB_SEND_HK_MIDX             1Hz, 0
CFE_TIME_SEND_HK_MIDX           1Hz, 0
CFE_TBL_SEND_HK_MIDX            1Hz, 0
CFE_TIME_FAKE_CMD_MIDX          ---, ---
CFE_TIME_TONE_CMD_MIDX          ---, ---
CFE_TIME_1HZ_CMD_MIDX           ---, ---

CF_SEND_HK_MIDX                 1Hz, 6
CF_WAKE_UP_REQ_CMD_MIDX         1Hz, 1
CI_SEND_HK_MIDX                 1Hz, 2
CI_1HZ_PROC_TIMEOUTS_MIDX       1Hz, 0 (decrement the timeouts at top of major frame, not required)
CS_BACKGROUND_CYCLE_MIDX        1Hz, 2
CS_SEND_HK_MIDX                 1Hz, 1
DS_SEND_HK_MIDX                 1Hz, 1
HK_SEND_COMBINED_PKT1_MIDX      ---, ---
HK_SEND_COMBINED_PKT2_MIDX      ---, ---
HK_SEND_COMBINED_PKT3_MIDX      ---, ---
HK_SEND_COMBINED_PKT4_MIDX      ---, ---
HK_SEND_COMBINED_PKT5_MIDX      ---, ---
HK_SEND_COMBINED_PKT6_MIDX      ---, ---
HK_SEND_COMBINED_PKT7_MIDX      ---, ---
HK_SEND_COMBINED_PKT8_MIDX      ---, ---
HK_SEND_COMBINED_PKT9_MIDX      ---, ---
HK_SEND_COMBINED_PKT10_MIDX     ---, ---
HK_SEND_HK_MIDX                 1Hz, 2
HS_SEND_HK_MIDX                 1Hz, 6
HS_WAKEUP_MIDX                  1Hz, 3
FM_SEND_HK_MIDX                 1Hz, 3
LC_SEND_HK_MIDX                 1Hz, 3
MD_SEND_HK_MIDX                 1Hz, 4
MD_WAKEUP_MIDX                  1Hz, 5
MM_SEND_HK_MIDX                 1Hz, 4
SC_1HZ_WAKEUP_MIDX              1Hz, 0 (wakeup at top of major frame, not required)
SC_SEND_HK_MIDX                 1Hz, 4
SCH_SEND_HK_MIDX                1Hz, 5
TO_SEND_TLM_MIDX                2Hz, 3, 7
TO_SEND_HK_MIDX                 1Hz, 6 (send HK before TO sends out)
*/


/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/


/* Default schedule table */
SCH_ScheduleEntry_t SCH_DefaultScheduleTable[SCH_TABLE_ENTRIES] = 
{
    /*Enable State, Type, Frequency, Remainder, Message Index, GroupData, Deadline (unused) */

    /* slot #0*/
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CFE_ES_SEND_HK_MIDX,   SCH_GROUP_CFEHK },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CFE_EVS_SEND_HK_MIDX,  SCH_GROUP_CFEHK },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CFE_SB_SEND_HK_MIDX,   SCH_GROUP_CFEHK },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CFE_TIME_SEND_HK_MIDX, SCH_GROUP_CFEHK },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CFE_TBL_SEND_HK_MIDX,  SCH_GROUP_CFEHK },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CI_1HZ_PROC_TIMEOUTS_MIDX,  SCH_GROUP_CI },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  SC_1HZ_WAKEUP_MIDX,    SCH_GROUP_CS },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #1*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CF_WAKE_UP_REQ_CMD_MIDX,  SCH_GROUP_CF },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CS_SEND_HK_MIDX,          SCH_GROUP_CS },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  DS_SEND_HK_MIDX,          SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #2*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CI_SEND_HK_MIDX,           SCH_GROUP_CI },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CS_BACKGROUND_CYCLE_MIDX,  SCH_GROUP_CS },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  HK_SEND_HK_MIDX,           SCH_GROUP_HK },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #3*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  HS_WAKEUP_MIDX,    SCH_GROUP_HS },
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  FM_SEND_HK_MIDX,   SCH_GROUP_NONE },
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  LC_SEND_HK_MIDX,   SCH_GROUP_NONE },
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  TO_SEND_TLM_MIDX,  SCH_GROUP_TO },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #4*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  MM_SEND_HK_MIDX,    SCH_GROUP_NONE },
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  MD_SEND_HK_MIDX,    SCH_GROUP_MD },
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  SC_SEND_HK_MIDX,    SCH_GROUP_SC },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #5*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  MD_WAKEUP_MIDX,    SCH_GROUP_MD },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  SCH_SEND_HK_MIDX,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #6*/
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  CF_SEND_HK_MIDX,  SCH_GROUP_CF },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  HS_SEND_HK_MIDX,  SCH_GROUP_HS },
    { SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  TO_SEND_HK_MIDX,  SCH_GROUP_TO },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },

    /* slot #7*/
	{ SCH_ENABLED,  SCH_ACTIVITY_SEND_MSG,  1,  0,  TO_SEND_TLM_MIDX,  SCH_GROUP_TO },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE },
    { SCH_UNUSED,   0,  0,  0,  0,  SCH_GROUP_NONE }

};

/*
** Local Variables
*/

/*
** Function Prototypes
*/

/*
** Function Definitions
*/



/* Table file header */
CFE_TBL_FILEDEF(SCH_DefaultScheduleTable, SCH.SCHED_DEF, SCH schedule table, sch_def_schtbl.tbl )



#ifdef __cplusplus
}
#endif

/*=======================================================================================
** End of file sch_def_schtbl.c
**=====================================================================================*/
    
