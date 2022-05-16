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


/*
** Local Defines
*/

#define CFE_ES_SEND_HK_MIDX            1
#define CFE_EVS_SEND_HK_MIDX           2
#define CFE_SB_SEND_HK_MIDX            3
#define CFE_TIME_SEND_HK_MIDX          4
#define CFE_TBL_SEND_HK_MIDX           5
#define CFE_TIME_FAKE_CMD_MIDX         6

#define SCH_SEND_HK_MIDX              15

#define TO_SEND_HK_MIDX               20
#define TO_SEND_TLM_MIDX              21

#define CI_INGEST_COMMANDS_MIDX       26
#define CI_PROCESS_TIMEOUTS_MIDX      27
#define CI_SEND_HK_MIDX               28
#define CI_WAKEUP_MIDX                29

#define MFA_WAKEUP_MIDX               35



/* Default schedule table */
SCH_ScheduleEntry_t SCH_DefaultScheduleTable[SCH_TABLE_ENTRIES] =
{
		/* Minor Frame 0 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CFE_ES_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	TO_SEND_TLM_MIDX,         SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	MFA_WAKEUP_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 1 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CFE_EVS_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CI_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 2 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CFE_SB_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CI_WAKEUP_MIDX,           SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 3 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CFE_TIME_SEND_HK_MIDX,    SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CI_INGEST_COMMANDS_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 4 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CFE_TBL_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	CI_PROCESS_TIMEOUTS_MIDX, SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 5 */
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 6 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	SCH_SEND_HK_MIDX,         SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 7 */
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,  1,	0,	TO_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 8 */
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Minor Frame 9 */
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }
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
    
