#ifndef MSG_IDS_H
#define MSG_IDS_H


#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"


#define CMD_MSG(X)  (CFE_MSG_CPU_BASE + CFE_CMD_MID_BASE + X)
#define TLM_MSG(X)  (CFE_MSG_CPU_BASE + CFE_TLM_MID_BASE + X)
#define FROM_CPD(X) (CPD_CPU_BASE + X)


#define CFE_EVS_CMD_MID                           CMD_MSG(    0 )
#define CFE_SB_CMD_MID                            CMD_MSG(    1 )
#define CFE_TBL_CMD_MID                           CMD_MSG(    2 )
#define CFE_TIME_CMD_MID                          CMD_MSG(    3 )
#define CFE_ES_CMD_MID                            CMD_MSG(    4 )

#define CFE_ES_SEND_HK_MID                        CMD_MSG(    5 )
#define CFE_EVS_SEND_HK_MID                       CMD_MSG(    6 )
#define CFE_SB_SEND_HK_MID                        CMD_MSG(    7 )
#define CFE_TBL_SEND_HK_MID                       CMD_MSG(    8 )
#define CFE_TIME_SEND_HK_MID                      CMD_MSG(    9 )

#define CFE_TIME_TONE_CMD_MID                     CMD_MSG(   10 )
#define CFE_TIME_1HZ_CMD_MID                      CMD_MSG(   11 )


/*
** cFE Global Command Message Id's
*/
#define CFE_TIME_DATA_CMD_MID                     CMD_MSG(   12 )
#define CFE_TIME_FAKE_CMD_MID                     CMD_MSG(   13 )
#define CFE_TIME_SEND_CMD_MID                     CMD_MSG(   14 )


/*
** CFE Telemetry Message Id's
*/
#define CFE_ES_HK_TLM_MID                         TLM_MSG(   15 )
#define CFE_EVS_HK_TLM_MID                        TLM_MSG(   16 )
#define CFE_SB_HK_TLM_MID                         TLM_MSG(   17 )
#define CFE_TBL_HK_TLM_MID                        TLM_MSG(   18 )
#define CFE_TIME_HK_TLM_MID                       TLM_MSG(   19 )
#define CFE_TIME_DIAG_TLM_MID                     TLM_MSG(   20 )
#define CFE_EVS_EVENT_MSG_MID                     TLM_MSG(   21 )
#define CFE_SB_STATS_TLM_MID                      TLM_MSG(   22 )
#define CFE_ES_APP_TLM_MID                        TLM_MSG(   23 )
#define CFE_TBL_REG_TLM_MID                       TLM_MSG(   24 )
#define CFE_SB_ALLSUBS_TLM_MID                    TLM_MSG(   25 )
#define CFE_SB_ONESUB_TLM_MID                     TLM_MSG(   26 )
#define CFE_ES_SHELL_TLM_MID                      TLM_MSG(   27 )
#define CFE_ES_MEMSTATS_TLM_MID                   TLM_MSG(   28 )


/* CI                                                          */
#define CI_HK_TLM_MID                             TLM_MSG(   29 )  /* 0x1d */
#define CI_CMD_MID                                CMD_MSG(   30 )  /* 0x1e */
#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(   31 )  /* 0x1f */
#define CI_SEND_HK_MID                            CMD_MSG(   32 )  /* 0x20 */
#define CI_WAKEUP_MID                             CMD_MSG(   33 )  /* 0x21 */
#define CI_INGEST_COMMANDS_MID                    CMD_MSG(   34 )  /* 0x22 */


/* DS                                                          */
#define DS_DIAG_TLM_MID                           TLM_MSG(   35 )  /* 0x23 */
#define DS_HK_TLM_MID                             TLM_MSG(   36 )  /* 0x24 */
#define DS_CMD_MID                                CMD_MSG(   37 )  /* 0x25 */
#define DS_SEND_HK_MID                            CMD_MSG(   38 )  /* 0x26 */


/* HS                                                        */
#define HS_HK_TLM_MID                             TLM_MSG(   39 )  /* 0x27 */
#define HS_CMD_MID                                CMD_MSG(   40 )  /* 0x28 */
#define HS_SEND_HK_MID                            CMD_MSG(   41 )  /* 0x29 */
#define HS_WAKEUP_MID                             CMD_MSG(   42 )  /* 0x2a */


/* MD                                                        */
#define MD_DWELL_PKT_MID_BASE                     TLM_MSG(   44 )  /* 0x2c */
#define MD_HK_TLM_MID                             TLM_MSG(   48 )  /* 0x30 */
#define MD_CMD_MID                                CMD_MSG(   49 )  /* 0x31 */
#define MD_SEND_HK_MID                            CMD_MSG(   50 )  /* 0x32 */
#define MD_WAKEUP_MID                             CMD_MSG(   51 )  /* 0x33 */


/* MM                                                        */
#define MM_HK_TLM_MID                             TLM_MSG(   52 )  /* 0x34 */
#define MM_CMD_MID                                CMD_MSG(   53 )  /* 0x35 */
#define MM_SEND_HK_MID                            CMD_MSG(   54 )  /* 0x36 */


/* SCH                                                       */
#define SCH_ACTIVITY_DONE_MID                     TLM_MSG(   55 )  /* 0x37 */
#define SCH_DIAG_TLM_MID                          TLM_MSG(   56 )  /* 0x38 */
#define SCH_HK_TLM_MID                            TLM_MSG(   57 )  /* 0x39 */
#define SCH_CMD_MID                               CMD_MSG(   58 )  /* 0x3a */
#define SCH_SEND_HK_MID                           CMD_MSG(   59 )  /* 0x3b */
#define SCH_UNUSED_MID                                    (0)


/* TO                                                        */
#define TO_DATA_TYPE_MID                          TLM_MSG(   60 )  /* 0x3c */
#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(   61 )  /* 0x3d */
#define TO_DIAG_TLM_MID                           TLM_MSG(   62 )  /* 0x3e */
#define TO_HK_TLM_MID                             TLM_MSG(   63 )  /* 0x3f */
#define TO_CMD_MID                                CMD_MSG(   64 )  /* 0x40 */
#define TO_SEND_HK_MID                            CMD_MSG(   65 )  /* 0x41 */
#define TO_SEND_TLM_MID                           CMD_MSG(   66 )  /* 0x42 */


#define CF_SPACE_TO_GND_PDU_MID                   TLM_MSG(   67 )  /* 0x43 */


/* SIMLINK                                                   */
#define SIMLINK_CMD_MID                           CMD_MSG(   68 )  /* 0x44 */
#define SIMLINK_SEND_HK_MID                       CMD_MSG(   69 )  /* 0x45 */
#define SIMLINK_WAKEUP_MID                        CMD_MSG(   70 )  /* 0x46 */
#define SIMLINK_OUT_DATA_MID                      TLM_MSG(   71 )  /* 0x47 */
#define SIMLINK_HK_TLM_MID                        TLM_MSG(   72 )  /* 0x48 */


/* CVT                                                      */
#define CVT_CMD_MID                               CMD_MSG(   73 )  /* 0x49 */
#define CVT_SEND_HK_MID                           CMD_MSG(   74 )  /* 0x4a */
#define CVT_WAKEUP_MID                            CMD_MSG(   75 )  /* 0x4b */
#define CVT_OUT_DATA_MID                          TLM_MSG(   76 )  /* 0x4c */
#define CVT_HK_TLM_MID                            TLM_MSG(   77 )  /* 0x4d */


/* UBLOX_M8N                                                 */
#define UBLOX_M8N_CMD_MID                         CMD_MSG(   78 )  /* 0x4e */
#define UBLOX_M8N_SEND_HK_MID                     CMD_MSG(   79 )  /* 0x4f */
#define UBLOX_M8N_WAKEUP_MID                      CMD_MSG(   80 )  /* 0x50 */
#define UBLOX_M8N_OUT_DATA_MID                    TLM_MSG(   81 )  /* 0x51 */
#define UBLOX_M8N_HK_TLM_MID                      TLM_MSG(   82 )  /* 0x52 */


/* HMC5883                                                   */
#define HMC5883_CMD_MID                           CMD_MSG(   83 )  /* 0x53 */
#define HMC5883_SEND_HK_MID                       CMD_MSG(   84 )  /* 0x54 */
#define HMC5883_WAKEUP_MID                        CMD_MSG(   85 )  /* 0x55 */
#define HMC5883_OUT_DATA_MID                      TLM_MSG(   86 )  /* 0x56 */
#define HMC5883_HK_TLM_MID                        TLM_MSG(   87 )  /* 0x57 */


/* ICM20689                                                  */
#define ICM20689_CMD_MID                          CMD_MSG(   88 )  /* 0x58 */
#define ICM20689_SEND_HK_MID                      CMD_MSG(   89 )  /* 0x59 */
#define ICM20689_WAKEUP_MID                       CMD_MSG(   90 )  /* 0x5a */
#define ICM20689_OUT_DATA_MID                     TLM_MSG(   91 )  /* 0x5b */
#define ICM20689_HK_TLM_MID                       TLM_MSG(   92 )  /* 0x5c */

/* MS5611                                                    */
#define MS5611_CMD_MID                            CMD_MSG(   93 )  /* 0x5d */
#define MS5611_SEND_HK_MID                        CMD_MSG(   94 )  /* 0x5e */
#define MS5611_WAKEUP_MID                         CMD_MSG(   95 )  /* 0x5f */
#define MS5611_OUT_DATA_MID                       TLM_MSG(   96 )  /* 0x60 */
#define MS5611_HK_TLM_MID                         TLM_MSG(   97 )  /* 0x61 */


/* RFD900X                                                   */
#define RFD900X_CMD_MID                           CMD_MSG(   98 )  /* 0x62 */
#define RFD900X_SEND_HK_MID                       CMD_MSG(   99 )  /* 0x63 */
#define RFD900X_WAKEUP_MID                        CMD_MSG(  100 )  /* 0x64 */
#define RFD900X_OUT_DATA_MID                      TLM_MSG(  101 )  /* 0x65 */
#define RFD900X_HK_TLM_MID                        TLM_MSG(  102 )  /* 0x66 */


/* SEDPWM                                                    */
#define SEDPWM_CMD_MID                            CMD_MSG(  103 )  /* 0x67 */
#define SEDPWM_SEND_HK_MID                        CMD_MSG(  104 )  /* 0x68 */
#define SEDPWM_WAKEUP_MID                         CMD_MSG(  105 )  /* 0x69 */
#define SEDPWM_OUT_DATA_MID                       TLM_MSG(  106 )  /* 0x6a */
#define SEDPWM_HK_TLM_MID                         TLM_MSG(  107 )  /* 0x6b */





#define CPD_CPU_BASE (0x0200)

#define CFE_ES_HK_TLM_MID_CPD       (CPD_CPU_BASE + CFE_ES_HK_TLM_MID)
#define CFE_EVS_HK_TLM_MID_CPD      (CPD_CPU_BASE + CFE_EVS_HK_TLM_MID)
#define CFE_SB_HK_TLM_MID_CPD       (CPD_CPU_BASE + CFE_SB_HK_TLM_MID)
#define CFE_TBL_HK_TLM_MID_CPD      (CPD_CPU_BASE + CFE_TBL_HK_TLM_MID)
#define CFE_TIME_HK_TLM_MID_CPD     (CPD_CPU_BASE + CFE_TIME_HK_TLM_MID)
#define CFE_TIME_DIAG_TLM_MID_CPD   (CPD_CPU_BASE + CFE_TIME_DIAG_TLM_MID)
#define CFE_EVS_EVENT_MSG_MID_CPD   (CPD_CPU_BASE + CFE_EVS_EVENT_MSG_MID)
#define CFE_SB_STATS_TLM_MID_CPD    (CPD_CPU_BASE + CFE_SB_STATS_TLM_MID)
#define CFE_ES_APP_TLM_MID_CPD      (CPD_CPU_BASE + CFE_ES_APP_TLM_MID)
#define CFE_TBL_REG_TLM_MID_CPD     (CPD_CPU_BASE + CFE_TBL_REG_TLM_MID)
#define CFE_ES_SHELL_TLM_MID_CPD    (CPD_CPU_BASE + CFE_ES_SHELL_TLM_MID)
#define CFE_ES_MEMSTATS_TLM_MID_CPD (CPD_CPU_BASE + CFE_ES_MEMSTATS_TLM_MID)
#define PX4_SENSOR_ACCEL_MID_CPD    (CPD_CPU_BASE + PX4_SENSOR_ACCEL_MID)
#define PX4_SENSOR_GYRO_MID_CPD     (CPD_CPU_BASE + PX4_SENSOR_GYRO_MID)
#define PX4_SENSOR_BARO_MID_CPD     (CPD_CPU_BASE + PX4_SENSOR_BARO_MID)
#define HS_HK_TLM_MID_CPD           (CPD_CPU_BASE + HS_HK_TLM_MID)
#define ZUSUP_HK_TLM_MID_CPD        (CPD_CPU_BASE + ZUSUP_HK_TLM_MID)
#define SED_HK_TLM_MID_CPD          (CPD_CPU_BASE + SED_HK_TLM_MID)
#define PX4_SENSOR_COMBINED_MID_CPD (CPD_CPU_BASE + PX4_SENSOR_COMBINED_MID)
#define PE_HK_TLM_MID_CPD           (CPD_CPU_BASE + PE_HK_TLM_MID)
#define VM_HK_TLM_MID_CPD           (CPD_CPU_BASE + VM_HK_TLM_MID)
#define BAT_HK_TLM_MID_CPD          (CPD_CPU_BASE + BAT_HK_TLM_MID)
#define RCIN_HK_TLM_MID_CPD         (CPD_CPU_BASE + RCIN_HK_TLM_MID)
#define TO_HK_TLM_MID_CPD           (CPD_CPU_BASE + TO_HK_TLM_MID)

#endif
