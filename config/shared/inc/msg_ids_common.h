#ifndef MSG_IDS_COMMON_H
#define MSG_IDS_COMMON_H


#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"

#define CFE_MSG_MASK      (0xF9FF)
#define CFE_MSG_APID_MASK (0x01FF)
#define FROM_PPD(X)       ((CFE_MSG_MASK & X) + PPD_CPU_BASE)
#define FROM_CPD(X)       ((CFE_MSG_MASK & X) + CPD_CPU_BASE)
#define FROM_SIMLINK(X)   ((CFE_MSG_MASK & X) + SIMLINK_CPU_BASE)
#define TO_PPD(X)         ((CFE_MSG_MASK & X) + PPD_CPU_BASE)
#define TO_CPD(X)         ((CFE_MSG_MASK & X) + CPD_CPU_BASE)
#define TO_SIMLINK(X)     ((CFE_MSG_MASK & X) + SIMLINK_CPU_BASE)

#define CMD_MSG(X)        (CFE_MSG_CPU_BASE + CFE_CMD_MID_BASE + (CFE_MSG_APID_MASK & X))
#define TLM_MSG(X)        (CFE_MSG_CPU_BASE + CFE_TLM_MID_BASE + (CFE_MSG_APID_MASK & X))
#define CPD_CMD_MSG(X)    (CPD_CPU_BASE + CFE_CMD_MID_BASE + (CFE_MSG_APID_MASK & X))
#define CPD_TLM_MSG(X)    (CPD_CPU_BASE + CFE_TLM_MID_BASE + (CFE_MSG_APID_MASK & X))
#define PPD_CMD_MSG(X)    (PPD_CPU_BASE + CFE_CMD_MID_BASE + (CFE_MSG_APID_MASK & X))
#define PPD_TLM_MSG(X)    (PPD_CPU_BASE + CFE_TLM_MID_BASE + (CFE_MSG_APID_MASK & X))


/* Core                                                                    */
#define CFE_EVS_CMD_MID                           CMD_MSG(    0 )  /* 0x00 */
#define CFE_SB_CMD_MID                            CMD_MSG(    1 )  /* 0x01 */
#define CFE_TBL_CMD_MID                           CMD_MSG(    2 )  /* 0x02 */
#define CFE_TIME_CMD_MID                          CMD_MSG(    3 )  /* 0x03 */
#define CFE_ES_CMD_MID                            CMD_MSG(    4 )  /* 0x04 */

#define CFE_ES_SEND_HK_MID                        CMD_MSG(    5 )  /* 0x05 */
#define CFE_EVS_SEND_HK_MID                       CMD_MSG(    6 )  /* 0x06 */
#define CFE_SB_SEND_HK_MID                        CMD_MSG(    7 )  /* 0x07 */
#define CFE_TBL_SEND_HK_MID                       CMD_MSG(    8 )  /* 0x08 */
#define CFE_TIME_SEND_HK_MID                      CMD_MSG(    9 )  /* 0x09 */

#define CFE_TIME_TONE_CMD_MID                     CMD_MSG(   10 )  /* 0x0a */
#define CFE_TIME_1HZ_CMD_MID                      CMD_MSG(   11 )  /* 0x0b */

#define CFE_TIME_DATA_CMD_MID                     CMD_MSG(   12 )  /* 0x0c */
#define CFE_TIME_FAKE_CMD_MID                     CMD_MSG(   13 )  /* 0x0d */
#define CFE_TIME_SEND_CMD_MID                     CMD_MSG(   14 )  /* 0x0e */

#define CFE_ES_HK_TLM_MID                         TLM_MSG(   15 )  /* 0x0f */
#define CFE_EVS_HK_TLM_MID                        TLM_MSG(   16 )  /* 0x10 */
#define CFE_SB_HK_TLM_MID                         TLM_MSG(   17 )  /* 0x11 */
#define CFE_TBL_HK_TLM_MID                        TLM_MSG(   18 )  /* 0x12 */
#define CFE_TIME_HK_TLM_MID                       TLM_MSG(   19 )  /* 0x13 */
#define CFE_TIME_DIAG_TLM_MID                     TLM_MSG(   20 )  /* 0x14 */
#define CFE_EVS_EVENT_MSG_MID                     TLM_MSG(   21 )  /* 0x15 */
#define CFE_SB_STATS_TLM_MID                      TLM_MSG(   22 )  /* 0x16 */
#define CFE_ES_APP_TLM_MID                        TLM_MSG(   23 )  /* 0x17 */
#define CFE_TBL_REG_TLM_MID                       TLM_MSG(   24 )  /* 0x18 */
#define CFE_SB_ALLSUBS_TLM_MID                    TLM_MSG(   25 )  /* 0x19 */
#define CFE_SB_ONESUB_TLM_MID                     TLM_MSG(   26 )  /* 0x1a */
#define CFE_ES_SHELL_TLM_MID                      TLM_MSG(   27 )  /* 0x1b */
#define CFE_ES_MEMSTATS_TLM_MID                   TLM_MSG(   28 )  /* 0x1c */


/* CF                                                                      */
#define CF_CONFIG_TLM_MID                         TLM_MSG(   29 )  /* 0x1d */
#define CF_HK_TLM_MID                             TLM_MSG(   30 )  /* 0x1e */
#define CF_PPD_TO_CPD_PDU_MID                 CPD_CMD_MSG(   31 )  /* 0x1f */
#define CF_TRANS_TLM_MID                          TLM_MSG(   32 )  /* 0x20 */
#define CF_PPD_TO_GND_PDU_MID                 PPD_TLM_MSG(   33 )  /* 0x21 */
#define CF_CPD_TO_GND_PDU_MID                 CPD_TLM_MSG(   34 )  /* 0x22 */
#define CF_CMD_MID                                CMD_MSG(   35 )  /* 0x23 */
#define CF_CPD_TO_PPD_PDU_MID                 PPD_CMD_MSG(   36 )  /* 0x24 */
#define CF_SEND_HK_MID                            CMD_MSG(   37 )  /* 0x25 */
#define CF_WAKE_UP_REQ_CMD_MID                    CMD_MSG(   38 )  /* 0x26 */


/* CI                                                                      */
#define CI_HK_TLM_MID                             TLM_MSG(   39 )  /* 0x27 */
#define CI_CMD_MID                                CMD_MSG(   40 )  /* 0x28 */
#define CI_INGEST_COMMANDS_MID                    CMD_MSG(   41 )  /* 0x29 */
#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(   42 )  /* 0x2a */
#define CI_SEND_HK_MID                            CMD_MSG(   43 )  /* 0x2b */
#define CI_WAKEUP_MID                             CMD_MSG(   44 )  /* 0x2c */


/* CS                                                                      */
#define CS_HK_TLM_MID                             TLM_MSG(   45 )  /* 0x2d */
#define CS_BACKGROUND_CYCLE_MID                   CMD_MSG(   46 )  /* 0x2e */
#define CS_CMD_MID                                CMD_MSG(   47 )  /* 0x2f */
#define CS_SEND_HK_MID                            CMD_MSG(   48 )  /* 0x30 */


/* CVT                                                                    */
#define CVT_HK_TLM_MID                            TLM_MSG(   49 )  /* 0x31 */
#define CVT_CMD_MID                               CMD_MSG(   50 )  /* 0x32 */
#define CVT_SEND_HK_MID                           CMD_MSG(   51 )  /* 0x33 */
#define CVT_WAKEUP_MID                            CMD_MSG(   52 )  /* 0x34 */


/* DS                                                                      */
#define DS_DIAG_TLM_MID                           TLM_MSG(   53 )  /* 0x35 */
#define DS_HK_TLM_MID                             TLM_MSG(   54 )  /* 0x36 */
#define DS_CMD_MID                                CMD_MSG(   55 )  /* 0x37 */
#define DS_SEND_HK_MID                            CMD_MSG(   56 )  /* 0x38 */


/* EA                                                        */
#define EA_HK_TLM_MID                             TLM_MSG(   57 )  /* 0x39 */
#define EA_CMD_MID                                CMD_MSG(   58 )  /* 0x3a */
#define EA_SEND_HK_MID                            CMD_MSG(   59 )  /* 0x3b */
#define EA_WAKEUP_MID                             CMD_MSG(   60 )  /* 0x3c */


/* FM                                                                      */
#define FM_DIR_LIST_TLM_MID                       TLM_MSG(   61 )  /* 0x3d */
#define FM_FILE_INFO_TLM_MID                      TLM_MSG(   62 )  /* 0x3e */
#define FM_FREE_SPACE_TLM_MID                     TLM_MSG(   63 )  /* 0x3f */
#define FM_HK_TLM_MID                             TLM_MSG(   64 )  /* 0x40 */
#define FM_OPEN_FILES_TLM_MID                     TLM_MSG(   65 )  /* 0x41 */
#define FM_CMD_MID                                CMD_MSG(   66 )  /* 0x42 */
#define FM_SEND_HK_MID                            CMD_MSG(   67 )  /* 0x43 */


/* HK                                                        */
#define HK_HK_TLM_MID                             TLM_MSG(   68 )  /* 0x44 */
#define HK_CMD_MID                                CMD_MSG(   69 )  /* 0x45 */
#define HK_SEND_COMBINED_PKT_MID                  CMD_MSG(   70 )  /* 0x46 */
#define HK_SEND_HK_MID                            CMD_MSG(   71 )  /* 0x47 */


/* HS                                                                      */
#define HS_HK_TLM_MID                             TLM_MSG(   72 )  /* 0x48 */
#define HS_CMD_MID                                CMD_MSG(   73 )  /* 0x49 */
#define HS_SEND_HK_MID                            CMD_MSG(   74 )  /* 0x4a */
#define HS_WAKEUP_MID                             CMD_MSG(   75 )  /* 0x4b */


/* LC                                                        */
#define LC_HK_TLM_MID                             TLM_MSG(   76 )  /* 0x4c */
#define LC_CMD_MID                                CMD_MSG(   77 )  /* 0x4d */
#define LC_SAMPLE_AP_MID                          CMD_MSG(   78 )  /* 0x4e */
#define LC_SEND_HK_MID                            CMD_MSG(   79 )  /* 0x4f */


/* MD                                                                      */
#define MD_DWELL_PKT_MID_BASE                     TLM_MSG(   80 )  /* 0x50 */
#define MD_HK_TLM_MID                             TLM_MSG(   81 )  /* 0x51 */
#define MD_CMD_MID                                CMD_MSG(   82 )  /* 0x52 */
#define MD_SEND_HK_MID                            CMD_MSG(   83 )  /* 0x53 */
#define MD_WAKEUP_MID                             CMD_MSG(   84 )  /* 0x54 */


/* MM                                                                      */
#define MM_HK_TLM_MID                             TLM_MSG(   85 )  /* 0x55 */
#define MM_CMD_MID                                CMD_MSG(   86 )  /* 0x56 */
#define MM_SEND_HK_MID                            CMD_MSG(   87 )  /* 0x57 */


/* SBN                                                       */
#define SBN_TLM_MID                               TLM_MSG(   88 )  /* 0x58 */
#define SBN_MODULE_HK_TLM_MID                     TLM_MSG(   89 )  /* 0x59 */
/* These MIDs must match for SBN to SBN communication. */
#define SBN_SUB_MID                              (TLM_MSG(   90 ) - CFE_MSG_CPU_BASE)  /* 0x5a */
#define SBN_ALLSUB_MID                           (TLM_MSG(   91 ) - CFE_MSG_CPU_BASE)  /* 0x5b */
#define SBN_UNSUB_MID                            (TLM_MSG(   92 ) - CFE_MSG_CPU_BASE)  /* 0x5c */

#define SBN_CMD_MID                               CMD_MSG(   93 )  /* 0x5d */
#define SBN_WAKEUP_MID                            CMD_MSG(   94 )  /* 0x5e */


/* SC                                                        */
#define SC_HK_TLM_MID                             TLM_MSG(   95 )  /* 0x5f */
#define SC_1HZ_WAKEUP_MID                         CMD_MSG(   96 )  /* 0x60 */
#define SC_CMD_MID                                CMD_MSG(   97 )  /* 0x61 */
#define SC_SEND_HK_MID                            CMD_MSG(   98 )  /* 0x62 */


/* SCH                                                                     */
#define SCH_ACTIVITY_DONE_MID                     TLM_MSG(   99 )  /* 0x63 */
#define SCH_DIAG_TLM_MID                          TLM_MSG(  100 )  /* 0x64 */
#define SCH_HK_TLM_MID                            TLM_MSG(  101 )  /* 0x65 */
#define SCH_CMD_MID                               CMD_MSG(  102 )  /* 0x66 */
#define SCH_SEND_HK_MID                           CMD_MSG(  103 )  /* 0x67 */
#define SCH_UNUSED_MID                                   (    0 )


/* TO                                                                      */
#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(  105 )  /* 0x69 */
#define TO_DIAG_TLM_MID                           TLM_MSG(  106 )  /* 0x6a */
#define TO_HK_TLM_MID                             TLM_MSG(  107 )  /* 0x6b */
#define TO_CMD_MID                                CMD_MSG(  108 )  /* 0x6c */
#define TO_SEND_HK_MID                            CMD_MSG(  109 )  /* 0x6d */
#define TO_SEND_TLM_MID                           CMD_MSG(  110 )  /* 0x6e */

#define CF_GND_TO_PPD_PDU_MID                 PPD_CMD_MSG(  111 ) /* 0x6f */
#define CF_GND_TO_CPD_PDU_MID                 CPD_CMD_MSG(  112 ) /* 0x70 */


#define TO_CUSTOM_HK_TLM_MID                      TLM_MSG(  113 )  /* 0x71 */
#define CI_CUSTOM_HK_TLM_MID                      TLM_MSG(  114 )  /* 0x72 */


#endif
