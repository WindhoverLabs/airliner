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
#define CFE_TIME_TONE_CMD_MIDX         7
#define CFE_TIME_1HZ_CMD_MIDX          8

#define SIMLINK_SEND_HK_MIDX          10
#define AMC_SEND_HK_MIDX              11

#define SIMLINK_WAKEUP_MIDX           13

#define SCH_SEND_HK_MIDX              15
#define TO_SEND_HK_MIDX               16

#define TO_SEND_TLM_MIDX              19

#define UBLOX_M8N_SEND_HK_MIDX        22
#define UBLOX_M8N_WAKEUP_MIDX         23

#define HMC5883_SEND_HK_MIDX          25
#define HMC5883_WAKEUP_MIDX           26

#define HS_WAKEUP_MIDX                32
#define HS_SEND_HK_MIDX               33
#define MD_SEND_HK_MIDX               34
#define MD_WAKEUP_MIDX                35

#define DS_SEND_HK_MIDX               37
#define SBUS_WAKEUP_MIDX              38

#define ICM20689_SEND_HK_MIDX         41
#define ICM20689_MEASURE_MIDX         42

#define MS5611_SEND_HK_MIDX           44
#define MS5611_WAKEUP_MIDX            45

#define RFD900X_SEND_HK_MIDX          47
#define RFD900X_WAKEUP_MIDX           48

#define CI_1HZ_PROC_TIMEOUTS_MIDX     51
#define CI_SEND_HK_MIDX               52
#define CI_INGEST_COMMANDS_MIDX       53

#define CVT_SEND_HK_MIDX              56

#define SEDPWM_SEND_HK_MIDX           58
#define SEDPWM_WAKEUP_MIDX            59

#define ASPD4525_SEND_HK_MIDX         61
#define ASPD4525_WAKEUP_MIDX          62


#define MM_SEND_HK_MIDX               70


/* 250HZ Group  - 22 Activities  */
#define SCH_250HZ_GROUP_1         \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	ICM20689_MEASURE_MIDX,    SCH_GROUP_NONE }, \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	MS5611_WAKEUP_MIDX,       SCH_GROUP_NONE }, \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	RFD900X_WAKEUP_MIDX,      SCH_GROUP_NONE }, \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SEDPWM_WAKEUP_MIDX,       SCH_GROUP_NONE }, \
	 { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SBUS_WAKEUP_MIDX,         SCH_GROUP_NONE }, \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SIMLINK_WAKEUP_MIDX,      SCH_GROUP_NONE }, \
	 { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	UBLOX_M8N_WAKEUP_MIDX,    SCH_GROUP_NONE }, \
	 { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	HMC5883_WAKEUP_MIDX,      SCH_GROUP_NONE }, \
     { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	ASPD4525_WAKEUP_MIDX,     SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }, \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }


/* 125HZ Group  - 1 Activity  */
#define SCH_125HZ_GROUP_1        \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_125HZ_GROUP_2        \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }


/* 50HZ Group   - 1 Activity  */
#define SCH_50HZ_GROUP_1         \
     { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_50HZ_GROUP_2         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_50HZ_GROUP_3         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_50HZ_GROUP_4         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_50HZ_GROUP_5         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }


/* 25HZ Group   - 1 Activity  */
#define SCH_25HZ_GROUP_1         \
	 { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CI_INGEST_COMMANDS_MIDX,  SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_2         \
	 { SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	TO_SEND_TLM_MIDX,         SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_3         \
     { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_4         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_5         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_6         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_7         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_8         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_9         \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }

#define SCH_25HZ_GROUP_10        \
	 { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE }



/* Default schedule table */
SCH_ScheduleEntry_t SCH_DefaultScheduleTable[SCH_TABLE_ENTRIES] =
{

		/* Slot #0*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	UBLOX_M8N_SEND_HK_MIDX,   SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #1*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	HMC5883_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #2*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
        { SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #3*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	ICM20689_SEND_HK_MIDX,    SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #4*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	MS5611_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #5*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CFE_ES_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #6*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CFE_EVS_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #7*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CFE_SB_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #8*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CFE_TIME_SEND_HK_MIDX,    SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #9*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CFE_TBL_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #10*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	RFD900X_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #11*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SEDPWM_SEND_HK_MIDX,      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #12*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CI_1HZ_PROC_TIMEOUTS_MIDX, SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #13*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	CI_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #14*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SIMLINK_SEND_HK_MIDX,     SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #15*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	AMC_SEND_HK_MIDX,         SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #16*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	DS_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #17*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	ASPD4525_SEND_HK_MIDX,    SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #18*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #19*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #20*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #21*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	HS_WAKEUP_MIDX,           SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #22*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	HS_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #23*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #24*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #25*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #26*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #27*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	MD_WAKEUP_MIDX,           SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #28*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	MD_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #29*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	MM_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #30*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #31*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #32*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #33*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #34*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #35*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #36*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #37*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #38*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #39*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #40*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #41*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	TO_SEND_HK_MIDX,          SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #42*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #43*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #44*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #45*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #46*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #47*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #48*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #49*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #50*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #51*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #52*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #53*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #54*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #55*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #56*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #57*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #58*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #59*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #60*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #61*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #62*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #63*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #64*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #65*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #66*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #67*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #68*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #69*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #70*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #71*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #72*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #73*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #74*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #75*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #76*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #77*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #78*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #79*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #80*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #81*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #82*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #83*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #84*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #85*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #86*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #87*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #88*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #89*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #90*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #91*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #92*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #93*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #94*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #95*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #96*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #97*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #98*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #99*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #100*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #101*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #102*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #103*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #104*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #105*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #106*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #107*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #108*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #109*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #110*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #111*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #112*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #113*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #114*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #115*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #116*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #117*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #118*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #119*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #120*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #121*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #122*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #123*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #124*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #125*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #126*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #127*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #128*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #129*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #130*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #131*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #132*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #133*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #134*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #135*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #136*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #137*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #138*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #139*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #140*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #141*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #142*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #143*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #144*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #145*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #146*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #147*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #148*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #149*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #150*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #151*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #152*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #153*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #154*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #155*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #156*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #157*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #158*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #159*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #160*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #161*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #162*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #163*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #164*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #165*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #166*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #167*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #168*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #169*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #170*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #171*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #172*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #173*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #174*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #175*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #176*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #177*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #178*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #179*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #180*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #181*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #182*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #183*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #184*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #185*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #186*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #187*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #188*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #189*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #190*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #191*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #192*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #193*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #194*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #195*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #196*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #197*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #198*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #199*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #200*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #201*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #202*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #203*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #204*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #205*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #206*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #207*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #208*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #209*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #210*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #211*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #212*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #213*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #214*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #215*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #216*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #217*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #218*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #219*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #220*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #221*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #222*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #223*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #224*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #225*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #226*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #227*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #228*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #229*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #230*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #231*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #232*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #233*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #234*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #235*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #236*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #237*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #238*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #239*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #240*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_1,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #241*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_2,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #242*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_3,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #243*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_4,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #244*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_5,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #245*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_1,
	    SCH_25HZ_GROUP_6,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #246*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_2,
	    SCH_25HZ_GROUP_7,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #247*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_3,
	    SCH_25HZ_GROUP_8,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #248*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_1,
	    SCH_50HZ_GROUP_4,
	    SCH_25HZ_GROUP_9,
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },
		{ SCH_UNUSED,	0,	                    0,	0,	0,	                      SCH_GROUP_NONE },

		/* Slot #249*/
	    SCH_250HZ_GROUP_1,
	    SCH_125HZ_GROUP_2,
	    SCH_50HZ_GROUP_5,
	    SCH_25HZ_GROUP_10,
		{ SCH_ENABLED,	SCH_ACTIVITY_SEND_MSG,	1,	0,	SCH_SEND_HK_MIDX,         SCH_GROUP_NONE },
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
    
