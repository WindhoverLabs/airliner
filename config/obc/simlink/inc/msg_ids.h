#ifndef MSG_IDS_H
#define MSG_IDS_H




#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"


#define CMD_MSG(X)  (CFE_MSG_CPU_BASE + CFE_CMD_MID_BASE + X)
#define TLM_MSG(X)  (CFE_MSG_CPU_BASE + CFE_TLM_MID_BASE + X)
#define FROM_CPD(X) (CPD_CPU_BASE + X)


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
#define CF_INCOMING_PDU_MID                       CMD_MSG(   31 )  /* 0x1f */
#define CF_TRANS_TLM_MID                          TLM_MSG(   32 )  /* 0x20 */
#define CF_CMD_MID                                CMD_MSG(   33 )  /* 0x21 */
#define CF_SEND_HK_MID                            CMD_MSG(   34 )  /* 0x22 */
#define CF_SPACE_TO_GND_PDU_MID                   TLM_MSG(   35 )  /* 0x23 */
#define CF_WAKE_UP_REQ_CMD_MID                    CMD_MSG(   36 )  /* 0x24 */
#define CF_XCHANNEL_INCOMING_PDU_MID              CMD_MSG(   37 )  /* 0x25 */
#define CF_XCHANNEL_OUTGOING_PDU_MID              TLM_MSG(   38 )  /* 0x26 */


/* CI                                                                      */
#define CI_HK_TLM_MID                             TLM_MSG(   39 )  /* 0x27 */
#define CI_CMD_MID                                CMD_MSG(   40 )  /* 0x28 */
#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(   41 )  /* 0x29 */
#define CI_SEND_HK_MID                            CMD_MSG(   42 )  /* 0x2a */
#define CI_WAKEUP_MID                             CMD_MSG(   43 )  /* 0x2b */
#define CI_INGEST_COMMANDS_MID                    CMD_MSG(   44 )  /* 0x2c */


/* CS                                                                      */
#define CS_HK_TLM_MID                             TLM_MSG(   45 )  /* 0x2d */
#define CS_BACKGROUND_CYCLE_MID                   CMD_MSG(   46 )  /* 0x2e */
#define CS_CMD_MID                                CMD_MSG(   47 )  /* 0x2f */
#define CS_SEND_HK_MID                            CMD_MSG(   48 )  /* 0x30 */


/* CVT                                                                    */
#define CVT_CMD_MID                               CMD_MSG(   49 )  /* 0x31 */
#define CVT_SEND_HK_MID                           CMD_MSG(   50 )  /* 0x32 */
#define CVT_WAKEUP_MID                            CMD_MSG(   51 )  /* 0x33 */
#define CVT_HK_TLM_MID                            TLM_MSG(   52 )  /* 0x34 */


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
#define TO_DATA_TYPE_MID                          TLM_MSG(  104 )  /* 0x68 */
#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(  105 )  /* 0x69 */
#define TO_DIAG_TLM_MID                           TLM_MSG(  106 )  /* 0x6a */
#define TO_HK_TLM_MID                             TLM_MSG(  107 )  /* 0x6b */
#define TO_CMD_MID                                CMD_MSG(  108 )  /* 0x6c */
#define TO_SEND_HK_MID                            CMD_MSG(  109 )  /* 0x6d */
#define TO_SEND_TLM_MID                           CMD_MSG(  110 )  /* 0x6e */













/* SIMLINK                                                                 */
#define SIMLINK_CMD_MID                           CMD_MSG(  128 )  /* 0x80 */
#define SIMLINK_SEND_HK_MID                       CMD_MSG(   69 )  /* 0x45 */
#define SIMLINK_WAKEUP_MID                        CMD_MSG(   70 )  /* 0x46 */
#define SIMLINK_OUT_DATA_MID                      TLM_MSG(   71 )  /* 0x47 */
#define SIMLINK_HK_TLM_MID                        TLM_MSG(   72 )  /* 0x48 */


/* UBLOX_M8N                                                               */
#define UBLOX_M8N_CMD_MID                         CMD_MSG(   78 )  /* 0x4e */
#define UBLOX_M8N_SEND_HK_MID                     CMD_MSG(   79 )  /* 0x4f */
#define UBLOX_M8N_WAKEUP_MID                      CMD_MSG(   80 )  /* 0x50 */
#define UBLOX_M8N_OUT_DATA_MID                    TLM_MSG(   81 )  /* 0x51 */
#define UBLOX_M8N_HK_TLM_MID                      TLM_MSG(   82 )  /* 0x52 */


/* HMC5883                                                                 */
#define HMC5883_CMD_MID                           CMD_MSG(   83 )  /* 0x53 */
#define HMC5883_SEND_HK_MID                       CMD_MSG(   84 )  /* 0x54 */
#define HMC5883_WAKEUP_MID                        CMD_MSG(   85 )  /* 0x55 */
#define HMC5883_OUT_DATA_MID                      TLM_MSG(   86 )  /* 0x56 */
#define HMC5883_HK_TLM_MID                        TLM_MSG(   87 )  /* 0x57 */


/* ICM20689                                                                */
#define ICM20689_CMD_MID                          CMD_MSG(   88 )  /* 0x58 */
#define ICM20689_SEND_HK_MID                      CMD_MSG(   89 )  /* 0x59 */
#define ICM20689_WAKEUP_MID                       CMD_MSG(   90 )  /* 0x5a */
#define ICM20689_OUT_DATA_MID                     TLM_MSG(   91 )  /* 0x5b */
#define ICM20689_HK_TLM_MID                       TLM_MSG(   92 )  /* 0x5c */
#define ICM20689_MEASURE_MID                      CMD_MSG(   93 )
#define ICM20689_DIAG_TLM_MID                     TLM_MSG(   94 )

/* MS5611                                                                  */
#define MS5611_CMD_MID                            CMD_MSG(   93 )  /* 0x5d */
#define MS5611_SEND_HK_MID                        CMD_MSG(   94 )  /* 0x5e */
#define MS5611_WAKEUP_MID                         CMD_MSG(   95 )  /* 0x5f */
#define MS5611_OUT_DATA_MID                       TLM_MSG(   96 )  /* 0x60 */
#define MS5611_HK_TLM_MID                         TLM_MSG(   97 )  /* 0x61 */
#define MS5611_MEASURE_MID                        CMD_MSG(   97 )  /* 0x61 */
#define MS5611_DIAG_TLM_MID                       TLM_MSG(   97 )  /* 0x61 */


/* RFD900X                                                                 */
#define RFD900X_CMD_MID                           CMD_MSG(   98 )  /* 0x62 */
#define RFD900X_SEND_HK_MID                       CMD_MSG(   99 )  /* 0x63 */
#define RFD900X_WAKEUP_MID                        CMD_MSG(  100 )  /* 0x64 */
#define RFD900X_OUT_DATA_MID                      TLM_MSG(  101 )  /* 0x65 */
#define RFD900X_HK_TLM_MID                        TLM_MSG(  102 )  /* 0x66 */


/* SEDPWM                                                                  */
#define SEDPWM_CMD_MID                            CMD_MSG(  103 )  /* 0x67 */
#define SEDPWM_SEND_HK_MID                        CMD_MSG(  104 )  /* 0x68 */
#define SEDPWM_WAKEUP_MID                         CMD_MSG(  105 )  /* 0x69 */
#define SEDPWM_OUT_DATA_MID                       TLM_MSG(  106 )  /* 0x6a */
#define SEDPWM_HK_TLM_MID                         TLM_MSG(  107 )  /* 0x6b */


/* AMC                                                       */
#define AMC_HK_TLM_MID                            TLM_MSG(  193 )
#define AMC_OUT_DATA_MID                          TLM_MSG(  194 )
#define AMC_CMD_MID                               CMD_MSG(  195 )
#define AMC_SEND_HK_MID                           CMD_MSG(  196 )
#define AMC_UPDATE_MOTORS_MID                     CMD_MSG(  197 )


/* LD                                                        */
#define LD_DIAG_TLM_MID                           TLM_MSG(  226 )
#define LD_HK_TLM_MID                             TLM_MSG(  227 )
#define LD_CMD_MID                                CMD_MSG(  228 )
#define LD_SEND_HK_MID                            CMD_MSG(  229 )
#define LD_WAKEUP_MID                             CMD_MSG(  230 )


/* LGC                                                       */
#define LGC_HK_TLM_MID                            TLM_MSG(  231 )
#define LGC_CMD_MID                               CMD_MSG(  232 )
#define LGC_SEND_HK_MID                           CMD_MSG(  233 )
#define LGC_WAKEUP_MID                            CMD_MSG(  234 )


/* MAC                                                       */
#define MAC_HK_TLM_MID                            TLM_MSG(  235 )
#define MAC_CMD_MID                               CMD_MSG(  236 )
#define MAC_RUN_CONTROLLER_MID                    CMD_MSG(  237 )
#define MAC_SEND_HK_MID                           CMD_MSG(  238 )


/* MPC                                                       */
#define MPC_DIAG_TLM_MID                          TLM_MSG(  239 )
#define MPC_HK_TLM_MID                            TLM_MSG(  240 )
#define MPC_CMD_MID                               CMD_MSG(  241 )
#define MPC_SEND_HK_MID                           CMD_MSG(  242 )
#define MPC_WAKEUP_MID                            CMD_MSG(  243 )

/* QAE                                                       */
#define QAE_HK_TLM_MID                            TLM_MSG(  288 )
#define QAE_CMD_MID                               CMD_MSG(  289 )
#define QAE_SEND_HK_MID                           CMD_MSG(  290 )
#define QAE_WAKEUP_MID                            CMD_MSG(  291 )


/* RCIN                                                      */
#define RCIN_HK_TLM_MID                           TLM_MSG(  292 )
#define RCIN_CMD_MID                              CMD_MSG(  293 )
#define RCIN_SEND_HK_MID                          CMD_MSG(  294 )
#define RCIN_WAKEUP_MID                           CMD_MSG(  295 )

/* SBND                                                      */
#define SBND_HK_TLM_MID                           TLM_MSG(  307 )
#define SBND_CMD_MID                              CMD_MSG(  308 )
#define SBND_SEND_HK_MID                          CMD_MSG(  309 )
#define SBND_WAKEUP_MID                           CMD_MSG(  310 )

/* SENS                                                      */
#define SENS_HK_TLM_MID                           TLM_MSG(  311 )
#define SENS_CMD_MID                              CMD_MSG(  312 )
#define SENS_SEND_HK_MID                          CMD_MSG(  313 )
#define SENS_WAKEUP_MID                           CMD_MSG(  314 )


/* SONAR                                                     */
#define SONAR_HK_TLM_MID                          TLM_MSG(  319 )
#define SONAR_CMD_MID                             CMD_MSG(  320 )
#define SONAR_MEASURE_MID                         CMD_MSG(  321 )
#define SONAR_SEND_HK_MID                         CMD_MSG(  322 )


/* ULR                                                       */
#define ULR_HK_TLM_MID                            TLM_MSG(  323 )
#define ULR_CMD_MID                               CMD_MSG(  324 )
#define ULR_MEASURE_MID                           CMD_MSG(  325 )
#define ULR_SEND_HK_MID                           CMD_MSG(  326 )

/* VM                                                        */
#define VM_CONFIG_TLM_MID                         TLM_MSG(  331 )
#define VM_HK_TLM_MID                             TLM_MSG(  332 )
#define VM_CMD_MID                                CMD_MSG(  333 )
#define VM_SEND_HK_MID                            CMD_MSG(  334 )
#define VM_WAKEUP_MID                             CMD_MSG(  335 )

/* ZUSUP                                                     */
#define ZUSUP_DIAG_TLM_MID                        TLM_MSG(  336 )
#define ZUSUP_HK_TLM_MID                          TLM_MSG(  337 )
#define ZUSUP_CMD_MID                             CMD_MSG(  338 )
#define ZUSUP_SEND_HK_MID                         CMD_MSG(  339 )
#define ZUSUP_WAKEUP_MID                          CMD_MSG(  340 )

/* ADSBR                                                     */
#define ADSBR_HK_TLM_MID                          TLM_MSG(  341 )
#define ADSBR_OUT_DATA_MID                        TLM_MSG(  342 )
#define ADSBR_CMD_MID                             CMD_MSG(  343 )
#define ADSBR_SEND_HK_MID                         CMD_MSG(  344 )
#define ADSBR_WAKEUP_MID                          CMD_MSG(  345 )

/* NAV                                                       */
#define NAV_HK_TLM_MID                            TLM_MSG(  274 )
#define NAV_CMD_MID                               CMD_MSG(  275 )
#define NAV_SEND_HK_MID                           CMD_MSG(  276 )
#define NAV_WAKEUP_MID                            CMD_MSG(  277 )

/* PE                                                        */
#define PE_HK_TLM_MID                             TLM_MSG(  278 )
#define PE_CMD_MID                                CMD_MSG(  279 )
#define PE_SEND_HK_MID                            CMD_MSG(  280 )
#define PE_WAKEUP_MID                             CMD_MSG(  281 )
#define PE_DIAG_TLM_MID                           TLM_MSG(  282 )




/* PX4                                                       */
#define PX4_ACTUATOR_ARMED_MID                    TLM_MSG(   97 )
#define PX4_ACTUATOR_CONTROLS_0_MID               TLM_MSG(   98 )
#define PX4_ACTUATOR_CONTROLS_1_MID               TLM_MSG(   99 )
#define PX4_ACTUATOR_CONTROLS_2_MID               TLM_MSG(  100 )
#define PX4_ACTUATOR_CONTROLS_3_MID               TLM_MSG(  101 )
#define PX4_ACTUATOR_DIRECT_MID                   TLM_MSG(  102 )
#define PX4_ACTUATOR_OUTPUTS_MID                  TLM_MSG(  103 )
#define PX4_ADC_REPORT_MID                        TLM_MSG(  104 )
#define PX4_AIRSPEED_MID                          TLM_MSG(  105 )
#define PX4_ATT_POS_MOCAP_MID                     TLM_MSG(  106 )
#define PX4_BATTERY_STATUS_MID                    TLM_MSG(  107 )
#define PX4_CAMERA_TRIGGER_MID                    TLM_MSG(  108 )
#define PX4_COMMANDER_STATE_MID                   TLM_MSG(  109 )
#define PX4_CONTROL_STATE_MID                     TLM_MSG(  110 )
#define PX4_CPULOAD_MID                           TLM_MSG(  111 )
#define PX4_DEBUG_KEY_VALUE_MID                   TLM_MSG(  112 )
#define PX4_DIFFERENTIAL_PRESSURE_MID             TLM_MSG(  113 )
#define PX4_DISTANCE_SENSOR_MID                   TLM_MSG(  114 )
#define PX4_FW_POS_CTRL_STATUS_MID                TLM_MSG(  115 )
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  116 )
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  117 )
#define PX4_EKF2_INNOVATIONS_MID                  TLM_MSG(  118 )
#define PX4_EKF2_REPLAY_MID                       TLM_MSG(  119 )
#define PX4_ESC_REPORT_MID                        TLM_MSG(  120 )
#define PX4_ESC_STATUS_MID                        TLM_MSG(  121 )
#define PX4_ESTIMATOR_STATUS_MID                  TLM_MSG(  122 )
#define PX4_FENCE_MID                             TLM_MSG(  123 )
#define PX4_FENCE_VERTEX_MID                      TLM_MSG(  124 )
#define PX4_FILTERED_BOTTOM_FLOW_MID              TLM_MSG(  125 )
#define PX4_FOLLOW_TARGET_MID                     TLM_MSG(  126 )
#define PX4_GEOFENCE_RESULT_MID                   TLM_MSG(  127 )
#define PX4_GPS_DUMP_MID                          TLM_MSG(  128 )
#define PX4_GPS_INJECT_DATA_MID                   TLM_MSG(  129 )
#define PX4_HIL_SENSOR_MID                        TLM_MSG(  130 )
#define PX4_HOME_POSITION_MID                     TLM_MSG(  131 )
#define PX4_INPUT_RC_MID                          TLM_MSG(  132 )
#define PX4_LED_CONTROL_MID                       TLM_MSG(  133 )
#define PX4_LOG_MESSAGE_MID                       TLM_MSG(  134 )
#define PX4_MANUAL_CONTROL_SETPOINT_MID           TLM_MSG(  135 )
#define PX4_MAVLINK_LOG_MID                       TLM_MSG(  136 )
#define PX4_MC_ATT_CTRL_STATUS_MID                TLM_MSG(  137 )
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  138 )
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  139 )
#define PX4_MISSION_MID                           TLM_MSG(  140 )
#define PX4_MISSION_RESULT_MID                    TLM_MSG(  141 )
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           TLM_MSG(  142 )
#define PX4_OFFBOARD_CONTROL_MODE_MID             TLM_MSG(  143 )
#define PX4_OPTICAL_FLOW_MID                      TLM_MSG(  144 )
#define PX4_OUTPUT_PWM_MID                        TLM_MSG(  145 )
#define PX4_PARAMETER_UPDATE_MID                  TLM_MSG(  146 )
#define PX4_POSITION_SETPOINT_MID                 TLM_MSG(  147 )
#define PX4_POSITION_SETPOINT_TRIPLET_MID         TLM_MSG(  148 )
#define PX4_PWM_INPUT_MID                         TLM_MSG(  149 )
#define PX4_QSHELL_REQ_MID                        TLM_MSG(  150 )
#define PX4_RC_CHANNELS_MID                       TLM_MSG(  151 )
#define PX4_RC_PARAMETER_MAP_MID                  TLM_MSG(  152 )
#define PX4_SAFETY_MID                            TLM_MSG(  153 )
#define PX4_SATELLITE_INFO_MID                    TLM_MSG(  154 )
#define PX4_SENSOR_ACCEL_MID                      TLM_MSG(  155 )
#define PX4_SENSOR_BARO_MID                       TLM_MSG(  156 )
#define PX4_SENSOR_COMBINED_MID                   TLM_MSG(  157 )
#define PX4_SENSOR_GYRO_MID                       TLM_MSG(  158 )
#define PX4_SENSOR_MAG_MID                        TLM_MSG(  159 )
#define PX4_SERVORAIL_STATUS_MID                  TLM_MSG(  160 )
#define PX4_SUBSYSTEM_INFO_MID                    TLM_MSG(  161 )
#define PX4_SYSTEM_POWER_MID                      TLM_MSG(  162 )
#define PX4_TECS_STATUS_MID                       TLM_MSG(  163 )
#define PX4_TELEMETRY_STATUS_MID                  TLM_MSG(  164 )
#define PX4_TEST_MOTOR_MID                        TLM_MSG(  165 )
#define PX4_TIME_OFFSET_MID                       TLM_MSG(  166 )
#define PX4_TRANSPONDER_REPORT_MID                TLM_MSG(  167 )
#define PX4_UAVCAN_PARAMETER_REQUEST_MID          TLM_MSG(  168 )
#define PX4_UAVCAN_PARAMETER_VALUE_MID            TLM_MSG(  169 )
#define PX4_VEHICLE_ATTITUDE_MID                  TLM_MSG(  170 )
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         TLM_MSG(  171 )
#define PX4_VEHICLE_COMMAND_ACK_MID               TLM_MSG(  172 )
#define PX4_VEHICLE_COMMAND_MID                   TLM_MSG(  173 )
#define PX4_VEHICLE_CONTROL_MODE_MID              TLM_MSG(  174 )
#define PX4_VEHICLE_FORCE_SETPOINT_MID            TLM_MSG(  175 )
#define PX4_VEHICLE_GLOBAL_POSITION_MID           TLM_MSG(  176 )
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  TLM_MSG(  177 )
#define PX4_VEHICLE_GPS_POSITION_MID              TLM_MSG(  178 )
#define PX4_VEHICLE_LAND_DETECTED_MID             TLM_MSG(  179 )
#define PX4_VEHICLE_LOCAL_POSITION_MID            TLM_MSG(  180 )
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   TLM_MSG(  181 )
#define PX4_VEHICLE_RATES_SETPOINT_MID            TLM_MSG(  182 )
#define PX4_VEHICLE_STATUS_MID                    TLM_MSG(  183 )
#define PX4_VISION_POSITION_ESTIMATE_MID          TLM_MSG(  184 )
#define PX4_VTOL_VEHICLE_STATUS_MID               TLM_MSG(  185 )
#define PX4_WIND_ESTIMATE_MID                     TLM_MSG(  186 )
#define PX4_SENSOR_CORRECTION_MID                 TLM_MSG(  187 )




//#define CPD_CPU_BASE (0x0200)
//
//#define CFE_ES_HK_TLM_MID_CPD       (CPD_CPU_BASE + CFE_ES_HK_TLM_MID)
//#define CFE_EVS_HK_TLM_MID_CPD      (CPD_CPU_BASE + CFE_EVS_HK_TLM_MID)
//#define CFE_SB_HK_TLM_MID_CPD       (CPD_CPU_BASE + CFE_SB_HK_TLM_MID)
//#define CFE_TBL_HK_TLM_MID_CPD      (CPD_CPU_BASE + CFE_TBL_HK_TLM_MID)
//#define CFE_TIME_HK_TLM_MID_CPD     (CPD_CPU_BASE + CFE_TIME_HK_TLM_MID)
//#define CFE_TIME_DIAG_TLM_MID_CPD   (CPD_CPU_BASE + CFE_TIME_DIAG_TLM_MID)
//#define CFE_EVS_EVENT_MSG_MID_CPD   (CPD_CPU_BASE + CFE_EVS_EVENT_MSG_MID)
//#define CFE_SB_STATS_TLM_MID_CPD    (CPD_CPU_BASE + CFE_SB_STATS_TLM_MID)
//#define CFE_ES_APP_TLM_MID_CPD      (CPD_CPU_BASE + CFE_ES_APP_TLM_MID)
//#define CFE_TBL_REG_TLM_MID_CPD     (CPD_CPU_BASE + CFE_TBL_REG_TLM_MID)
//#define CFE_ES_SHELL_TLM_MID_CPD    (CPD_CPU_BASE + CFE_ES_SHELL_TLM_MID)
//#define CFE_ES_MEMSTATS_TLM_MID_CPD (CPD_CPU_BASE + CFE_ES_MEMSTATS_TLM_MID)
//#define PX4_SENSOR_ACCEL_MID_CPD    (CPD_CPU_BASE + PX4_SENSOR_ACCEL_MID)
//#define PX4_SENSOR_GYRO_MID_CPD     (CPD_CPU_BASE + PX4_SENSOR_GYRO_MID)
//#define PX4_SENSOR_BARO_MID_CPD     (CPD_CPU_BASE + PX4_SENSOR_BARO_MID)
//#define HS_HK_TLM_MID_CPD           (CPD_CPU_BASE + HS_HK_TLM_MID)
//#define ZUSUP_HK_TLM_MID_CPD        (CPD_CPU_BASE + ZUSUP_HK_TLM_MID)
//#define PX4_SENSOR_COMBINED_MID_CPD (CPD_CPU_BASE + PX4_SENSOR_COMBINED_MID)
//#define PE_HK_TLM_MID_CPD           (CPD_CPU_BASE + PE_HK_TLM_MID)
//#define VM_HK_TLM_MID_CPD           (CPD_CPU_BASE + VM_HK_TLM_MID)
//#define BAT_HK_TLM_MID_CPD          (CPD_CPU_BASE + BAT_HK_TLM_MID)
//#define RCIN_HK_TLM_MID_CPD         (CPD_CPU_BASE + RCIN_HK_TLM_MID)
//#define TO_HK_TLM_MID_CPD           (CPD_CPU_BASE + TO_HK_TLM_MID)

#endif



//#ifndef MSG_IDS_H
//#define MSG_IDS_H
//
//
//#include "cfe_mission_cfg.h"
//#include "cfe_platform_cfg.h"
//
//
//#define CMD_MSG(X)  (CFE_MSG_CPU_BASE + CFE_CMD_MID_BASE + X)
//#define TLM_MSG(X)  (CFE_MSG_CPU_BASE + CFE_TLM_MID_BASE + X)
//#define FROM_CPD(X) (0x0200 | X)
//#define FROM_PPD(X) ((~CFE_MSG_CPU_MASK) & X)
//
//
//#define CFE_EVS_CMD_MID                           CMD_MSG(    0 )
//#define CFE_SB_CMD_MID                            CMD_MSG(    1 )
//#define CFE_TBL_CMD_MID                           CMD_MSG(    2 )
//#define CFE_TIME_CMD_MID                          CMD_MSG(    3 )
//#define CFE_ES_CMD_MID                            CMD_MSG(    4 )
//
//#define CFE_ES_SEND_HK_MID                        CMD_MSG(    5 )
//#define CFE_EVS_SEND_HK_MID                       CMD_MSG(    6 )
//#define CFE_SB_SEND_HK_MID                        CMD_MSG(    7 )
//#define CFE_TBL_SEND_HK_MID                       CMD_MSG(    8 )
//#define CFE_TIME_SEND_HK_MID                      CMD_MSG(    9 )
//
//#define CFE_TIME_TONE_CMD_MID                     CMD_MSG(   10 )
//#define CFE_TIME_1HZ_CMD_MID                      CMD_MSG(   11 )
//
//
///*
//** cFE Global Command Message Id's
//*/
//#define CFE_TIME_DATA_CMD_MID                     CMD_MSG(   12 )
//#define CFE_TIME_FAKE_CMD_MID                     CMD_MSG(   13 )
//#define CFE_TIME_SEND_CMD_MID                     CMD_MSG(   14 )
//
//
///*
//** CFE Telemetry Message Id's
//*/
//#define CFE_ES_HK_TLM_MID                         TLM_MSG(   15 )
//#define CFE_EVS_HK_TLM_MID                        TLM_MSG(   16 )
//#define CFE_SB_HK_TLM_MID                         TLM_MSG(   17 )
//#define CFE_TBL_HK_TLM_MID                        TLM_MSG(   18 )
//#define CFE_TIME_HK_TLM_MID                       TLM_MSG(   19 )
//#define CFE_TIME_DIAG_TLM_MID                     TLM_MSG(   20 )
//#define CFE_EVS_EVENT_MSG_MID                     TLM_MSG(   21 )
//#define CFE_SB_STATS_TLM_MID                      TLM_MSG(   22 )
//#define CFE_ES_APP_TLM_MID                        TLM_MSG(   23 )
//#define CFE_TBL_REG_TLM_MID                       TLM_MSG(   24 )
//#define CFE_SB_ALLSUBS_TLM_MID                    TLM_MSG(   25 )
//#define CFE_SB_ONESUB_TLM_MID                     TLM_MSG(   26 )
//#define CFE_ES_SHELL_TLM_MID                      TLM_MSG(   27 )
//#define CFE_ES_MEMSTATS_TLM_MID                   TLM_MSG(   28 )
//
//
///* CI                                                          */
//#define CI_HK_TLM_MID                             TLM_MSG(   32 )
//#define CI_CMD_MID                                CMD_MSG(   33 )
//#define CI_INGEST_COMMANDS_MID                    CMD_MSG(   34 )
//#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(   35 )
//#define CI_SEND_HK_MID                            CMD_MSG(   36 )
//#define CI_WAKEUP_MID                             CMD_MSG(   37 )
//
//
///* CF                                                          */
//#define CF_CONFIG_TLM_MID                         TLM_MSG(   38 )
//#define CF_HK_TLM_MID                             TLM_MSG(   39 )
//#define CF_INCOMING_PDU_MID                       CMD_MSG(   40 )
//#define CF_TRANS_TLM_MID                          TLM_MSG(   41 )
//#define CF_CMD_MID                                CMD_MSG(   42 )
//#define CF_SEND_HK_MID                            CMD_MSG(   43 )
//#define CF_SPACE_TO_GND_PDU_MID                   TLM_MSG(   44 )
//#define CF_WAKE_UP_REQ_CMD_MID                    CMD_MSG(   45 )
//
//
///* CS                                                          */
//#define CS_HK_TLM_MID                             TLM_MSG(   46 )
//#define CS_BACKGROUND_CYCLE_MID                   CMD_MSG(   47 )
//#define CS_CMD_MID                                CMD_MSG(   48 )
//#define CS_SEND_HK_MID                            CMD_MSG(   49 )
//
//
///* DS                                                          */
//#define DS_DIAG_TLM_MID                           TLM_MSG(   50 )
//#define DS_HK_TLM_MID                             TLM_MSG(   51 )
//#define DS_CMD_MID                                CMD_MSG(   52 )
//#define DS_SEND_HK_MID                            CMD_MSG(   53 )
//
//
///* FM                                                          */
//#define FM_DIR_LIST_TLM_MID                       TLM_MSG(   54 )
//#define FM_FILE_INFO_TLM_MID                      TLM_MSG(   55 )
//#define FM_FREE_SPACE_TLM_MID                     TLM_MSG(   56 )
//#define FM_HK_TLM_MID                             TLM_MSG(   57 )
//#define FM_OPEN_FILES_TLM_MID                     TLM_MSG(   58 )
//#define FM_CMD_MID                                CMD_MSG(   59 )
//#define FM_SEND_HK_MID                            CMD_MSG(   60 )
//
//
///* HK                                                        */
//#define HK_HK_TLM_MID                             TLM_MSG(   61 )
//#define HK_CMD_MID                                CMD_MSG(   62 )
//#define HK_SEND_COMBINED_PKT_MID                  CMD_MSG(   63 )
//#define HK_SEND_HK_MID                            CMD_MSG(   64 )
//
//
///* HS                                                        */
//#define HS_HK_TLM_MID                             TLM_MSG(   65 )
//#define HS_CMD_MID                                CMD_MSG(   66 )
//#define HS_SEND_HK_MID                            CMD_MSG(   67 )
//#define HS_WAKEUP_MID                             CMD_MSG(   68 )
//
//
///* LC                                                        */
//#define LC_HK_TLM_MID                             TLM_MSG(   69 )
//#define LC_CMD_MID                                CMD_MSG(   70 )
//#define LC_SAMPLE_AP_MID                          CMD_MSG(   71 )
//#define LC_SEND_HK_MID                            CMD_MSG(   72 )
//
//
///* MD                                                        */
//#define MD_DWELL_PKT_MID_BASE                     TLM_MSG(   73 )
//#define MD_HK_TLM_MID                             TLM_MSG(   74 )
//#define MD_CMD_MID                                CMD_MSG(   75 )
//#define MD_SEND_HK_MID                            CMD_MSG(   76 )
//#define MD_WAKEUP_MID                             CMD_MSG(   77 )
//
//
///* MM                                                        */
//#define MM_HK_TLM_MID                             TLM_MSG(   78 )
//#define MM_CMD_MID                                CMD_MSG(   79 )
//#define MM_SEND_HK_MID                            CMD_MSG(   80 )
//
//
///* SC                                                        */
//#define SC_HK_TLM_MID                             TLM_MSG(   81 )
//#define SC_1HZ_WAKEUP_MID                         CMD_MSG(   82 )
//#define SC_CMD_MID                                CMD_MSG(   83 )
//#define SC_SEND_HK_MID                            CMD_MSG(   84 )
//
//
///* SCH                                                       */
//#define SCH_ACTIVITY_DONE_MID                     TLM_MSG(   85 )
//#define SCH_DIAG_TLM_MID                          TLM_MSG(   86 )
//#define SCH_HK_TLM_MID                            TLM_MSG(   87 )
//#define SCH_CMD_MID                               CMD_MSG(   88 )
//#define SCH_SEND_HK_MID                           CMD_MSG(   89 )
//#define SCH_UNUSED_MID                                    (0)
//
//
///* TO                                                        */
//#define TO_DATA_TYPE_MID                          TLM_MSG(   90 )
//#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(   91 )
//#define TO_DIAG_TLM_MID                           TLM_MSG(   92 )
//#define TO_HK_TLM_MID                             TLM_MSG(   93 )
//#define TO_CMD_MID                                CMD_MSG(   94 )
//#define TO_SEND_HK_MID                            CMD_MSG(   95 )
//#define TO_SEND_TLM_MID                           CMD_MSG(   96 )
//
//
///* PX4                                                       */
//#define PX4_ACTUATOR_ARMED_MID                    TLM_MSG(   97 )
//#define PX4_ACTUATOR_CONTROLS_0_MID               TLM_MSG(   98 )
//#define PX4_ACTUATOR_CONTROLS_1_MID               TLM_MSG(   99 )
//#define PX4_ACTUATOR_CONTROLS_2_MID               TLM_MSG(  100 )
//#define PX4_ACTUATOR_CONTROLS_3_MID               TLM_MSG(  101 )
//#define PX4_ACTUATOR_DIRECT_MID                   TLM_MSG(  102 )
//#define PX4_ACTUATOR_OUTPUTS_MID                  TLM_MSG(  103 )
//#define PX4_ADC_REPORT_MID                        TLM_MSG(  104 )
//#define PX4_AIRSPEED_MID                          TLM_MSG(  105 )
//#define PX4_ATT_POS_MOCAP_MID                     TLM_MSG(  106 )
//#define PX4_BATTERY_STATUS_MID                    TLM_MSG(  107 )
//#define PX4_CAMERA_TRIGGER_MID                    TLM_MSG(  108 )
//#define PX4_COMMANDER_STATE_MID                   TLM_MSG(  109 )
//#define PX4_CONTROL_STATE_MID                     TLM_MSG(  110 )
//#define PX4_CPULOAD_MID                           TLM_MSG(  111 )
//#define PX4_DEBUG_KEY_VALUE_MID                   TLM_MSG(  112 )
//#define PX4_DIFFERENTIAL_PRESSURE_MID             TLM_MSG(  113 )
//#define PX4_DISTANCE_SENSOR_MID                   TLM_MSG(  114 )
//#define PX4_FW_POS_CTRL_STATUS_MID                TLM_MSG(  115 )
//#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  116 )
//#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  117 )
//#define PX4_EKF2_INNOVATIONS_MID                  TLM_MSG(  118 )
//#define PX4_EKF2_REPLAY_MID                       TLM_MSG(  119 )
//#define PX4_ESC_REPORT_MID                        TLM_MSG(  120 )
//#define PX4_ESC_STATUS_MID                        TLM_MSG(  121 )
//#define PX4_ESTIMATOR_STATUS_MID                  TLM_MSG(  122 )
//#define PX4_FENCE_MID                             TLM_MSG(  123 )
//#define PX4_FENCE_VERTEX_MID                      TLM_MSG(  124 )
//#define PX4_FILTERED_BOTTOM_FLOW_MID              TLM_MSG(  125 )
//#define PX4_FOLLOW_TARGET_MID                     TLM_MSG(  126 )
//#define PX4_GEOFENCE_RESULT_MID                   TLM_MSG(  127 )
//#define PX4_GPS_DUMP_MID                          TLM_MSG(  128 )
//#define PX4_GPS_INJECT_DATA_MID                   TLM_MSG(  129 )
//#define PX4_HIL_SENSOR_MID                        TLM_MSG(  130 )
//#define PX4_HOME_POSITION_MID                     TLM_MSG(  131 )
//#define PX4_INPUT_RC_MID                          TLM_MSG(  132 )
//#define PX4_LED_CONTROL_MID                       TLM_MSG(  133 )
//#define PX4_LOG_MESSAGE_MID                       TLM_MSG(  134 )
//#define PX4_MANUAL_CONTROL_SETPOINT_MID           TLM_MSG(  135 )
//#define PX4_MAVLINK_LOG_MID                       TLM_MSG(  136 )
//#define PX4_MC_ATT_CTRL_STATUS_MID                TLM_MSG(  137 )
//#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  138 )
//#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  139 )
//#define PX4_MISSION_MID                           TLM_MSG(  140 )
//#define PX4_MISSION_RESULT_MID                    TLM_MSG(  141 )
//#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           TLM_MSG(  142 )
//#define PX4_OFFBOARD_CONTROL_MODE_MID             TLM_MSG(  143 )
//#define PX4_OPTICAL_FLOW_MID                      TLM_MSG(  144 )
//#define PX4_OUTPUT_PWM_MID                        TLM_MSG(  145 )
//#define PX4_PARAMETER_UPDATE_MID                  TLM_MSG(  146 )
//#define PX4_POSITION_SETPOINT_MID                 TLM_MSG(  147 )
//#define PX4_POSITION_SETPOINT_TRIPLET_MID         TLM_MSG(  148 )
//#define PX4_PWM_INPUT_MID                         TLM_MSG(  149 )
//#define PX4_QSHELL_REQ_MID                        TLM_MSG(  150 )
//#define PX4_RC_CHANNELS_MID                       TLM_MSG(  151 )
//#define PX4_RC_PARAMETER_MAP_MID                  TLM_MSG(  152 )
//#define PX4_SAFETY_MID                            TLM_MSG(  153 )
//#define PX4_SATELLITE_INFO_MID                    TLM_MSG(  154 )
//#define PX4_SENSOR_ACCEL_MID                      TLM_MSG(  155 )
//#define PX4_SENSOR_BARO_MID                       TLM_MSG(  156 )
//#define PX4_SENSOR_COMBINED_MID                   TLM_MSG(  157 )
//#define PX4_SENSOR_GYRO_MID                       TLM_MSG(  158 )
//#define PX4_SENSOR_MAG_MID                        TLM_MSG(  159 )
//#define PX4_SERVORAIL_STATUS_MID                  TLM_MSG(  160 )
//#define PX4_SUBSYSTEM_INFO_MID                    TLM_MSG(  161 )
//#define PX4_SYSTEM_POWER_MID                      TLM_MSG(  162 )
//#define PX4_TECS_STATUS_MID                       TLM_MSG(  163 )
//#define PX4_TELEMETRY_STATUS_MID                  TLM_MSG(  164 )
//#define PX4_TEST_MOTOR_MID                        TLM_MSG(  165 )
//#define PX4_TIME_OFFSET_MID                       TLM_MSG(  166 )
//#define PX4_TRANSPONDER_REPORT_MID                TLM_MSG(  167 )
//#define PX4_UAVCAN_PARAMETER_REQUEST_MID          TLM_MSG(  168 )
//#define PX4_UAVCAN_PARAMETER_VALUE_MID            TLM_MSG(  169 )
//#define PX4_VEHICLE_ATTITUDE_MID                  TLM_MSG(  170 )
//#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         TLM_MSG(  171 )
//#define PX4_VEHICLE_COMMAND_ACK_MID               TLM_MSG(  172 )
//#define PX4_VEHICLE_COMMAND_MID                   TLM_MSG(  173 )
//#define PX4_VEHICLE_CONTROL_MODE_MID              TLM_MSG(  174 )
//#define PX4_VEHICLE_FORCE_SETPOINT_MID            TLM_MSG(  175 )
//#define PX4_VEHICLE_GLOBAL_POSITION_MID           TLM_MSG(  176 )
//#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  TLM_MSG(  177 )
//#define PX4_VEHICLE_GPS_POSITION_MID              TLM_MSG(  178 )
//#define PX4_VEHICLE_LAND_DETECTED_MID             TLM_MSG(  179 )
//#define PX4_VEHICLE_LOCAL_POSITION_MID            TLM_MSG(  180 )
//#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   TLM_MSG(  181 )
//#define PX4_VEHICLE_RATES_SETPOINT_MID            TLM_MSG(  182 )
//#define PX4_VEHICLE_STATUS_MID                    TLM_MSG(  183 )
//#define PX4_VISION_POSITION_ESTIMATE_MID          TLM_MSG(  184 )
//#define PX4_VTOL_VEHICLE_STATUS_MID               TLM_MSG(  185 )
//#define PX4_WIND_ESTIMATE_MID                     TLM_MSG(  186 )
//#define PX4_SENSOR_CORRECTION_MID                 TLM_MSG(  187 )
//
//
///* AK8963                                                    */
//#define AK8963_DIAG_TLM_MID                       TLM_MSG(  188 )
//#define AK8963_HK_TLM_MID                         TLM_MSG(  189 )
//#define AK8963_CMD_MID                            CMD_MSG(  190 )
//#define AK8963_SEND_HK_MID                        CMD_MSG(  191 )
//#define AK8963_WAKEUP_MID                         CMD_MSG(  192 )
//
//
///* AMC                                                       */
//#define AMC_HK_TLM_MID                            TLM_MSG(  193 )
//#define AMC_OUT_DATA_MID                          TLM_MSG(  194 )
//#define AMC_CMD_MID                               CMD_MSG(  195 )
//#define AMC_SEND_HK_MID                           CMD_MSG(  196 )
//#define AMC_UPDATE_MOTORS_MID                     CMD_MSG(  197 )
//
//
///* BAT                                                       */
//#define BAT_HK_TLM_MID                            TLM_MSG(  198 )
//#define BAT_CMD_MID                               CMD_MSG(  199 )
//#define BAT_SEND_HK_MID                           CMD_MSG(  200 )
//#define BAT_WAKEUP_MID                            CMD_MSG(  201 )
//
//
///* EA                                                        */
//#define EA_HK_TLM_MID                             TLM_MSG(  202 )
//#define EA_CMD_MID                                CMD_MSG(  203 )
//#define EA_SEND_HK_MID                            CMD_MSG(  204 )
//#define EA_WAKEUP_MID                             CMD_MSG(  205 )
//
//
///* GPS                                                       */
//#define GPS_ACK_ACK_MID                           TLM_MSG(  206 )
//#define GPS_ACK_NAK_MID                           TLM_MSG(  207 )
//#define GPS_CFG_MSG_MID                           TLM_MSG(  208 )
//#define GPS_CFG_NAV5_MID                          TLM_MSG(  209 )
//#define GPS_CFG_PRT_MID                           TLM_MSG(  210 )
//#define GPS_CFG_RATE_MID                          TLM_MSG(  211 )
//#define GPS_CFG_SBAS_MID                          TLM_MSG(  212 )
//#define GPS_HK_TLM_MID                            TLM_MSG(  213 )
//#define GPS_MON_HW_MID                            TLM_MSG(  214 )
//#define GPS_NAV_DOP_MID                           TLM_MSG(  215 )
//#define GPS_NAV_NAVPVT_MID                        TLM_MSG(  216 )
//#define GPS_NAV_SVINFO_MID                        TLM_MSG(  217 )
//#define GPS_CMD_MID                               CMD_MSG(  218 )
//#define GPS_SEND_HK_MID                           CMD_MSG(  219 )
//#define GPS_READ_SENSOR_MID                       CMD_MSG(  220 )
///* Individual GPS messages                                   */
//
//
///* HMC5883                                                   */
//#define HMC5883_DIAG_TLM_MID                      TLM_MSG(  221 )
//#define HMC5883_HK_TLM_MID                        TLM_MSG(  222 )
//#define HMC5883_CMD_MID                           CMD_MSG(  223 )
//#define HMC5883_SEND_HK_MID                       CMD_MSG(  224 )
//#define HMC5883_WAKEUP_MID                        CMD_MSG(  225 )
//
//
///* LD                                                        */
//#define LD_DIAG_TLM_MID                           TLM_MSG(  226 )
//#define LD_HK_TLM_MID                             TLM_MSG(  227 )
//#define LD_CMD_MID                                CMD_MSG(  228 )
//#define LD_SEND_HK_MID                            CMD_MSG(  229 )
//#define LD_WAKEUP_MID                             CMD_MSG(  230 )
//
//
///* LGC                                                       */
//#define LGC_HK_TLM_MID                            TLM_MSG(  231 )
//#define LGC_CMD_MID                               CMD_MSG(  232 )
//#define LGC_SEND_HK_MID                           CMD_MSG(  233 )
//#define LGC_WAKEUP_MID                            CMD_MSG(  234 )
//
//
///* MAC                                                       */
//#define MAC_HK_TLM_MID                            TLM_MSG(  235 )
//#define MAC_CMD_MID                               CMD_MSG(  236 )
//#define MAC_RUN_CONTROLLER_MID                    CMD_MSG(  237 )
//#define MAC_SEND_HK_MID                           CMD_MSG(  238 )
//
//
///* MPC                                                       */
//#define MPC_DIAG_TLM_MID                          TLM_MSG(  239 )
//#define MPC_HK_TLM_MID                            TLM_MSG(  240 )
//#define MPC_CMD_MID                               CMD_MSG(  241 )
//#define MPC_SEND_HK_MID                           CMD_MSG(  242 )
//#define MPC_WAKEUP_MID                            CMD_MSG(  243 )
//
//
///* MPU6050                                                   */
//#define MPU6050_DIAG_TLM_MID                      TLM_MSG(  244 )
//#define MPU6050_HK_TLM_MID                        TLM_MSG(  245 )
//#define MPU6050_CMD_MID                           CMD_MSG(  246 )
//#define MPU6050_MEASURE_MID                       CMD_MSG(  247 )
//#define MPU6050_SEND_HK_MID                       CMD_MSG(  248 )
//
///* SED                                                       */
//#define SED_DIAG_TLM_MID                          TLM_MSG(  249 )
//#define SED_HK_TLM_MID                            TLM_MSG(  250 )
//#define SED_CMD_MID                               CMD_MSG(  251 )
//#define SED_MEASURE_MID                           CMD_MSG(  252 )
//#define SED_SEND_HK_MID                           CMD_MSG(  253 )
//
///* MPU9250                                                   */
//#define MPU9250_DIAG_TLM_MID                      TLM_MSG(  254 )
//#define MPU9250_HK_TLM_MID                        TLM_MSG(  255 )
//#define MPU9250_CMD_MID                           CMD_MSG(  256 )
//#define MPU9250_MEASURE_MID                       CMD_MSG(  257 )
//#define MPU9250_SEND_HK_MID                       CMD_MSG(  258 )
//
///* ICM20689                                                  */
//#define ICM20689_DIAG_TLM_MID                     TLM_MSG(  259 )
//#define ICM20689_HK_TLM_MID                       TLM_MSG(  260 )
//#define ICM20689_CMD_MID                          CMD_MSG(  261 )
//#define ICM20689_MEASURE_MID                      CMD_MSG(  262 )
//#define ICM20689_SEND_HK_MID                      CMD_MSG(  263 )
//
//
///* MS5607                                                    */
//#define MS5607_DIAG_TLM_MID                       TLM_MSG(  264 )
//#define MS5607_HK_TLM_MID                         TLM_MSG(  265 )
//#define MS5607_CMD_MID                            CMD_MSG(  266 )
//#define MS5607_MEASURE_MID                        CMD_MSG(  267 )
//#define MS5607_SEND_HK_MID                        CMD_MSG(  268 )
//
//
///* MS5611                                                    */
//#define MS5611_DIAG_TLM_MID                       TLM_MSG(  269 )
//#define MS5611_HK_TLM_MID                         TLM_MSG(  270 )
//#define MS5611_CMD_MID                            CMD_MSG(  271 )
//#define MS5611_MEASURE_MID                        CMD_MSG(  272 )
//#define MS5611_SEND_HK_MID                        CMD_MSG(  273 )
//
//
///* NAV                                                       */
//#define NAV_HK_TLM_MID                            TLM_MSG(  274 )
//#define NAV_CMD_MID                               CMD_MSG(  275 )
//#define NAV_SEND_HK_MID                           CMD_MSG(  276 )
//#define NAV_WAKEUP_MID                            CMD_MSG(  277 )
//
//
///* PE                                                        */
//#define PE_HK_TLM_MID                             TLM_MSG(  278 )
//#define PE_CMD_MID                                CMD_MSG(  279 )
//#define PE_SEND_HK_MID                            CMD_MSG(  280 )
//#define PE_WAKEUP_MID                             CMD_MSG(  281 )
//#define PE_DIAG_TLM_MID                           TLM_MSG(  282 )
//
///* PRM                                                       */
//#define PRM_HK_TLM_MID                            TLM_MSG(  283 )
//#define PRM_OUT_DATA_MID                          TLM_MSG(  284 )
//#define PRM_CMD_MID                               CMD_MSG(  285 )
//#define PRM_SEND_HK_MID                           CMD_MSG(  286 )
//#define PRM_WAKEUP_MID                            CMD_MSG(  287 )
//
//
///* QAE                                                       */
//#define QAE_HK_TLM_MID                            TLM_MSG(  288 )
//#define QAE_CMD_MID                               CMD_MSG(  289 )
//#define QAE_SEND_HK_MID                           CMD_MSG(  290 )
//#define QAE_WAKEUP_MID                            CMD_MSG(  291 )
//
//
///* RCIN                                                      */
//#define RCIN_HK_TLM_MID                           TLM_MSG(  292 )
//#define RCIN_CMD_MID                              CMD_MSG(  293 )
//#define RCIN_SEND_HK_MID                          CMD_MSG(  294 )
//#define RCIN_WAKEUP_MID                           CMD_MSG(  295 )
//
//
///* SBN                                                       */
//#define SBN_TLM_MID                               TLM_MSG(  300 )
//#define SBN_MODULE_HK_TLM_MID                     TLM_MSG(  301 )
///* These MIDs must match for SBN to SBN communication. */
//#define SBN_SUB_MID                               TLM_MSG(  302 ) - CFE_MSG_CPU_BASE
//#define SBN_ALLSUB_MID                            TLM_MSG(  303 ) - CFE_MSG_CPU_BASE
//#define SBN_UNSUB_MID                             TLM_MSG(  304 ) - CFE_MSG_CPU_BASE
//
//#define SBN_CMD_MID                               CMD_MSG(  305 )
//#define SBN_WAKEUP_MID                            CMD_MSG(  306 )
//
///* SBND                                                      */
//#define SBND_HK_TLM_MID                           TLM_MSG(  307 )
//#define SBND_CMD_MID                              CMD_MSG(  308 )
//#define SBND_SEND_HK_MID                          CMD_MSG(  309 )
//#define SBND_WAKEUP_MID                           CMD_MSG(  310 )
//
///* SENS                                                      */
//#define SENS_HK_TLM_MID                           TLM_MSG(  311 )
//#define SENS_CMD_MID                              CMD_MSG(  312 )
//#define SENS_SEND_HK_MID                          CMD_MSG(  313 )
//#define SENS_WAKEUP_MID                           CMD_MSG(  314 )
//
///* SIM                                                       */
//#define SIM_HK_TLM_MID                            TLM_MSG(  315 )
//#define SIM_CMD_MID                               CMD_MSG(  316 )
//#define SIM_SEND_HK_MID                           CMD_MSG(  317 )
//#define SIM_WAKEUP_MID                            CMD_MSG(  318 )
//
//
///* SONAR                                                     */
//#define SONAR_HK_TLM_MID                          TLM_MSG(  319 )
//#define SONAR_CMD_MID                             CMD_MSG(  320 )
//#define SONAR_MEASURE_MID                         CMD_MSG(  321 )
//#define SONAR_SEND_HK_MID                         CMD_MSG(  322 )
//
//
///* ULR                                                       */
//#define ULR_HK_TLM_MID                            TLM_MSG(  323 )
//#define ULR_CMD_MID                               CMD_MSG(  324 )
//#define ULR_MEASURE_MID                           CMD_MSG(  325 )
//#define ULR_SEND_HK_MID                           CMD_MSG(  326 )
//
//
///* VC                                                        */
//#define VC_HK_TLM_MID                             TLM_MSG(  327 )
//#define VC_CMD_MID                                CMD_MSG(  328 )
//#define VC_PROCESS_CMDS_MID                       CMD_MSG(  329 )
//#define VC_SEND_HK_MID                            CMD_MSG(  330 )
//
//
///* VM                                                        */
//#define VM_CONFIG_TLM_MID                         TLM_MSG(  331 )
//#define VM_HK_TLM_MID                             TLM_MSG(  332 )
//#define VM_CMD_MID                                CMD_MSG(  333 )
//#define VM_SEND_HK_MID                            CMD_MSG(  334 )
//#define VM_WAKEUP_MID                             CMD_MSG(  335 )
//
///* ZUSUP                                                     */
//#define ZUSUP_DIAG_TLM_MID                        TLM_MSG(  336 )
//#define ZUSUP_HK_TLM_MID                          TLM_MSG(  337 )
//#define ZUSUP_CMD_MID                             CMD_MSG(  338 )
//#define ZUSUP_SEND_HK_MID                         CMD_MSG(  339 )
//#define ZUSUP_WAKEUP_MID                          CMD_MSG(  340 )
//
///* ADSBR                                                     */
//#define ADSBR_HK_TLM_MID                          TLM_MSG(  341 )
//#define ADSBR_OUT_DATA_MID                        TLM_MSG(  342 )
//#define ADSBR_CMD_MID                             CMD_MSG(  343 )
//#define ADSBR_SEND_HK_MID                         CMD_MSG(  344 )
//#define ADSBR_WAKEUP_MID                          CMD_MSG(  345 )
//
//
//
//
//#endif
