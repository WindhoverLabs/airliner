#ifndef CFE_MSGIDS_H
#define CFE_MSGIDS_H

/*
** Includes
*/

#include "cfe_mission_cfg.h"

/*
** cFE Command Message Id's
*/
#define CFE_EVS_CMD_MID         CFE_CMD_MID_BASE_CPU2 + CFE_EVS_CMD_MSG         /* 0x1801 */
                                                       /* Message ID 0x1802 is available  */
#define CFE_SB_CMD_MID          CFE_CMD_MID_BASE_CPU2 + CFE_SB_CMD_MSG          /* 0x1803 */
#define CFE_TBL_CMD_MID         CFE_CMD_MID_BASE_CPU2 + CFE_TBL_CMD_MSG         /* 0x1804 */
#define CFE_TIME_CMD_MID        CFE_CMD_MID_BASE_CPU2 + CFE_TIME_CMD_MSG        /* 0x1805 */
#define CFE_ES_CMD_MID          CFE_CMD_MID_BASE_CPU2 + CFE_ES_CMD_MSG          /* 0x1806 */

#define CFE_ES_SEND_HK_MID      CFE_CMD_MID_BASE_CPU2 + CFE_ES_SEND_HK_MSG      /* 0x1808 */
#define CFE_EVS_SEND_HK_MID     CFE_CMD_MID_BASE_CPU2 + CFE_EVS_SEND_HK_MSG     /* 0x1809 */
                                                       /* Message ID 0x180A is available  */
#define CFE_SB_SEND_HK_MID      CFE_CMD_MID_BASE_CPU2 + CFE_SB_SEND_HK_MSG      /* 0x180B */
#define CFE_TBL_SEND_HK_MID     CFE_CMD_MID_BASE_CPU2 + CFE_TBL_SEND_HK_MSG     /* 0x180C */
#define CFE_TIME_SEND_HK_MID    CFE_CMD_MID_BASE_CPU2 + CFE_TIME_SEND_HK_MSG    /* 0x180D */

#define CFE_TIME_TONE_CMD_MID   CFE_CMD_MID_BASE_CPU2 + CFE_TIME_TONE_CMD_MSG   /* 0x1810 */
#define CFE_TIME_1HZ_CMD_MID    CFE_CMD_MID_BASE_CPU2 + CFE_TIME_1HZ_CMD_MSG    /* 0x1811 */


/*
** cFE Global Command Message Id's
*/
#define CFE_TIME_DATA_CMD_MID   CFE_CMD_MID_BASE_GLOB + CFE_TIME_DATA_CMD_MSG   /* 0x1860 */
#define CFE_TIME_FAKE_CMD_MID   CFE_CMD_MID_BASE_GLOB + CFE_TIME_FAKE_CMD_MSG   /* 0x1861 */
#define CFE_TIME_SEND_CMD_MID   CFE_CMD_MID_BASE_GLOB + CFE_TIME_SEND_CMD_MSG   /* 0x1862 */


/*
** CFE Telemetry Message Id's
*/
#define CFE_ES_HK_TLM_MID       CFE_TLM_MID_BASE_CPU2 + CFE_ES_HK_TLM_MSG       /* 0x0800 */
#define CFE_EVS_HK_TLM_MID      CFE_TLM_MID_BASE_CPU2 + CFE_EVS_HK_TLM_MSG      /* 0x0801 */
                                                       /* Message ID 0x0802 is available  */
#define CFE_SB_HK_TLM_MID       CFE_TLM_MID_BASE_CPU2 + CFE_SB_HK_TLM_MSG       /* 0x0803 */
#define CFE_TBL_HK_TLM_MID      CFE_TLM_MID_BASE_CPU2 + CFE_TBL_HK_TLM_MSG      /* 0x0804 */
#define CFE_TIME_HK_TLM_MID     CFE_TLM_MID_BASE_CPU2 + CFE_TIME_HK_TLM_MSG     /* 0x0805 */
#define CFE_TIME_DIAG_TLM_MID   CFE_TLM_MID_BASE_CPU2 + CFE_TIME_DIAG_TLM_MSG   /* 0x0806 */
#define CFE_EVS_EVENT_MSG_MID   CFE_TLM_MID_BASE_CPU2 + CFE_EVS_EVENT_MSG_MSG   /* 0x0808 */
#define CFE_SB_STATS_TLM_MID    CFE_TLM_MID_BASE_CPU2 + CFE_SB_STATS_TLM_MSG    /* 0x080A */
#define CFE_ES_APP_TLM_MID      CFE_TLM_MID_BASE_CPU2 + CFE_ES_APP_TLM_MSG      /* 0x080B */
#define CFE_TBL_REG_TLM_MID     CFE_TLM_MID_BASE_CPU2 + CFE_TBL_REG_TLM_MSG     /* 0x080C */
#define CFE_SB_ALLSUBS_TLM_MID  CFE_TLM_MID_BASE_CPU2 + CFE_SB_ALLSUBS_TLM_MSG  /* 0x080D */
#define CFE_SB_ONESUB_TLM_MID   CFE_TLM_MID_BASE_CPU2 + CFE_SB_ONESUB_TLM_MSG   /* 0x080E */
#define CFE_ES_SHELL_TLM_MID    CFE_TLM_MID_BASE_CPU2 + CFE_ES_SHELL_TLM_MSG    /* 0x080F */
#define CFE_ES_MEMSTATS_TLM_MID CFE_TLM_MID_BASE_CPU2 + CFE_ES_MEMSTATS_TLM_MSG /* 0x0810 */

#endif
