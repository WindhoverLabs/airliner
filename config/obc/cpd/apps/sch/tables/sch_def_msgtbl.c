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
#include "sch_tbldefs.h"
#include "msg_ids.h"


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

#define SCH_FIX_HEADER(a, b, c) CFE_MAKE_BIG16(a), CFE_MAKE_BIG16(b), CFE_MAKE_BIG16(c) 

/* Message table entry map */
SCH_MessageEntry_t SCH_DefaultMessageTable[SCH_MAX_MESSAGES] =
{
   /* **  DO NOT USE -- entry #0 reserved for "unused" command ID - DO NOT USE */
   /* Command ID #0 */
   { { SCH_UNUSED_MID, 0, 0, 0 } },
   /* Command ID #1 - Executive Services HK Request   */
   { { SCH_FIX_HEADER(CFE_ES_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #2 - Event Services HK Request     */
   { { SCH_FIX_HEADER(CFE_EVS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #3 - Software Bus HK Request       */
   { { SCH_FIX_HEADER(CFE_SB_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #4 - Time Services HK Request      */
   { { SCH_FIX_HEADER(CFE_TIME_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #5 - Table Services HK Request     */
   { { SCH_FIX_HEADER(CFE_TBL_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #6 */
   { { SCH_FIX_HEADER(CFE_TIME_FAKE_CMD_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #7 - Data storage HK request*/
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #8 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #9 */
   { { SCH_FIX_HEADER(LC_SAMPLE_AP_MID, 0xC000, 0x0007), 0x0000, 0x0000, 0x0001, 0x0001 } },
   /* Command ID #10 - Health and safety HK request*/
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #11 - Limit checker HK request*/
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #12 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #13 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #14 - Wakeup store command app*/
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0,0), 0 } },
   /* Command ID #15 - Scheduler HK request*/
   { { SCH_FIX_HEADER(SCH_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #16 */
   { { SCH_FIX_HEADER(TO_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #17 */
   { { SCH_FIX_HEADER(HK_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #18 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #19 */
   { { SCH_FIX_HEADER(TO_SEND_TLM_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #20 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT1_MID } },
   /* Command ID #21 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT2_MID } },
   /* Command ID #22 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT3_MID } },
   /* Command ID #23 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT4_MID } },
   /* Command ID #24 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT5_MID } },
   /* Command ID #25 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT6_MID } },
   /* Command ID #26 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT7_MID } },
   /* Command ID #27 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT8_MID } },
   /* Command ID #28 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT9_MID } },
   /* Command ID #29 */
   { { SCH_FIX_HEADER(HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003), 0x0000, HK_COMBINED_PKT10_MID } },
   /* Command ID #30 */
   { { SCH_FIX_HEADER(FM_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #31 */
   { { SCH_FIX_HEADER(LC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #32 */
   { { SCH_FIX_HEADER(HS_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #33 */
   { { SCH_FIX_HEADER(HS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #34 */
   { { SCH_FIX_HEADER(MD_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #35 */
   { { SCH_FIX_HEADER(MD_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #36 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #37 */
   { { SCH_FIX_HEADER(DS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #38 */
   { { SCH_FIX_HEADER(CS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #39 */
   { { SCH_FIX_HEADER(CS_BACKGROUND_CYCLE_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #40 */
   { { SCH_FIX_HEADER(SC_1HZ_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #41 */
   { { SCH_FIX_HEADER(SC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #42 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #43 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #44 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #45 */
   { { SCH_FIX_HEADER(VC_PROCESS_CMDS_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #46 */
   { { SCH_FIX_HEADER(VC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #47 */
   { { SCH_FIX_HEADER(RGBLED_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #48 */
   { { SCH_FIX_HEADER(RGBLED_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #49 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #50 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #51 */
   { { SCH_FIX_HEADER(CI_PROCESS_TIMEOUTS_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #52 */
   { { SCH_FIX_HEADER(CI_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #53 */
   { { SCH_FIX_HEADER(CI_INGEST_COMMANDS_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #54 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #55 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #56 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #57 */
   { { SCH_FIX_HEADER(CF_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #58 */
   { { SCH_FIX_HEADER(CF_WAKE_UP_REQ_CMD_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #59 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #60 */
   { { SCH_FIX_HEADER(MAC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #61 */
   { { SCH_FIX_HEADER(MAC_RUN_CONTROLLER_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #62 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #63 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #64 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #65 */
   { { SCH_FIX_HEADER(AMC_UPDATE_MOTORS_MID, 0xC000, 0x0001), 0 } },
   /* Command ID #66 */
   { { SCH_FIX_HEADER(AMC_SEND_HK_MID, 0xC000, 0x0001), 0 } },
   /* Command ID #67 */
   { { SCH_FIX_HEADER(SED_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #68 */
   { { SCH_FIX_HEADER(SED_MEASURE_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #69 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #70 */
   { { SCH_FIX_HEADER(MM_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #71 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #72 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #73 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #74 */
   { { SCH_FIX_HEADER(PE_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #75 */
   { { SCH_FIX_HEADER(PE_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #76 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #77 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #78 */
   { { SCH_FIX_HEADER(MS5611_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #79 */
   { { SCH_FIX_HEADER(MS5611_MEASURE_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #80 */
   { { SCH_FIX_HEADER(ULR_MEASURE_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #81 */
   { { SCH_FIX_HEADER(ULR_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #82 */
   { { SCH_FIX_HEADER(HMC5883_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #83 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #84 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #85 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #86 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #87 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #88 */
   { { SCH_FIX_HEADER(HMC5883_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #89 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #90 */
   { { SCH_FIX_HEADER(SENS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #91 */
   { { SCH_FIX_HEADER(SENS_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #92 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #93 */
   { { SCH_FIX_HEADER(LGC_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #94 */
   { { SCH_FIX_HEADER(LGC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #95*/
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #96 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #97 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #98 */
   /* SBN Wakeup command code. */
   { { SCH_FIX_HEADER(SBN_CMD_MID, 0xC000, 0x0001), 0x6400 } },
   /* Command ID #99 */
   /* SBN Send HK command code. */
   { { SCH_FIX_HEADER(SBN_CMD_MID, 0xC000, 0x0001), 0x0A00 } },
   /* Command ID #100 */
   { { SCH_FIX_HEADER(SBND_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #101 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #102 */
   { { SCH_FIX_HEADER(QAE_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #103 */
   { { SCH_FIX_HEADER(QAE_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #104 */
   { { SCH_FIX_HEADER(LD_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #105 */
   { { SCH_FIX_HEADER(LD_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #106 */
   { { SCH_FIX_HEADER(MPC_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #107 */
   { { SCH_FIX_HEADER(MPC_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #108 */
   { { SCH_FIX_HEADER(NAV_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #109 */
   { { SCH_FIX_HEADER(NAV_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #110 */
   { { SCH_FIX_HEADER(CFE_TIME_TONE_CMD_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #111 */
   { { SCH_FIX_HEADER(CFE_TIME_1HZ_CMD_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #112 */
   { { SCH_FIX_HEADER(RCIN_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #113 */
   { { SCH_FIX_HEADER(RCIN_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #114 */
   { { SCH_FIX_HEADER(VM_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #115 */
   { { SCH_FIX_HEADER(VM_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #116 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #117 */
   { { SCH_FIX_HEADER(BAT_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #118 */
   { { SCH_FIX_HEADER(BAT_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #119 */
   { { SCH_FIX_HEADER(ZUSUP_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #120 */
   { { SCH_FIX_HEADER(ZUSUP_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #121 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #122 */
   { { SCH_FIX_HEADER(GPS_READ_SENSOR_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #123 */
   { { SCH_FIX_HEADER(GPS_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #124 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #125 */
   { { SCH_FIX_HEADER(EA_WAKEUP_MID, 0xC000, 0x0001), 0x0000 } },
   /* Command ID #126 */
   { { SCH_FIX_HEADER(SCH_UNUSED_MID, 0, 0), 0 } },
   /* Command ID #127 */
   { { SCH_FIX_HEADER(EA_SEND_HK_MID, 0xC000, 0x0001), 0x0000 } }
};


/* Table file header */
CFE_TBL_FILEDEF(SCH_DefaultMessageTable, SCH.MSG_DEFS, SCH message definitions table, sch_def_msgtbl.tbl )



#ifdef __cplusplus
}
#endif

/*=======================================================================================
** End of file sch_def_msgtbl.c
**=====================================================================================*/
    
