#ifndef MSG_IDS_H
#define MSG_IDS_H

#include "cfe_msgids.h"
#include "cf_msgids.h"

/* Commands */

/* TO                                                      */
/* Range: 0x900 - 0x90f                                    */
#define TO_CMD_MID                                   (0x1900)
#define TO_SEND_HK_MID                               (0x1901)
#define TO_HK_TLM_MID                                (0x0902)
#define TO_SEND_TLM_MID                              (0x1903)
#define TO_DATA_TYPE_MID                             (0x0904)
#define TO_DIAG_TLM_MID                              (0x0911)
#define TO_DIAG_MSG_FLOW_MID                         (0x0912)

/* CI                                                      */
/* Range: 0x900 - 0x90f                                    */
#define CI_CMD_MID                                   (0x1905)
#define CI_SEND_HK_MID                               (0x1906)
#define CI_HK_TLM_MID                                (0x0907)
#define CI_WAKEUP_MID                                (0x1908)
#define CI_OUT_DATA_MID                              (0x1909)
#define CI_PROCESS_TIMEOUTS_MID                      (0x190a)

/* CF                                                      */
/* Range: 0x910 - 0x92f                                    */
#define CF_CMD_MID                                   (0x1910)
#define CF_INCOMING_PDU_MID                          (0x0FFD)
#define CF_SEND_HK_MID                               (0x1912)
#define CF_SPARE1_CMD_MID                            (0x1913)
#define CF_SPARE2_CMD_MID                            (0x1914)
#define CF_SPARE3_CMD_MID                            (0x1915)
#define CF_SPARE4_CMD_MID                            (0x1916)
#define CF_SPARE5_CMD_MID                            (0x1917)
#define CF_WAKE_UP_REQ_CMD_MID                       (0x1918)
#define CF_CONFIG_TLM_MID                            (0x0919)
#define CF_HK_TLM_MID                                (0x091a)
#define CF_SPARE0_TLM_MID                            (0x091b)
#define CF_SPARE1_TLM_MID                            (0x091c)
#define CF_SPARE2_TLM_MID                            (0x091d)
#define CF_SPARE3_TLM_MID                            (0x091e)
#define CF_SPARE4_TLM_MID                            (0x091f)
#define CF_TRANS_TLM_MID                             (0x0920)

/*
** NOTE: the definition below is NOT used by the code. The code uses the MsgId
** defined in the CF table. For the purpose of keeping all CF related message
** IDs defined in this file, the CF table should reference this macro
** definition.
*/
#define CF_SPACE_TO_GND_PDU_MID                      (0x0FFE)


/* CS                                                      */
/* Range: 0x930 - 0x93f                                    */
#define CS_CMD_MID                                   (0x1930)
#define CS_BACKGROUND_CYCLE_MID                      (0x1931)
#define CS_SEND_HK_MID                               (0x1932)
#define CS_HK_TLM_MID                                (0x0933)


/* DS                                                      */
/* Range: 0x940 - 0x94f                                    */
#define DS_CMD_MID                                   (0x1940)
#define DS_SEND_HK_MID                               (0x1941)
#define DS_HK_TLM_MID                                (0x0942)
#define DS_DIAG_TLM_MID                              (0x0943)


/* FM                                                      */
/* Range: 0x980 - 0x98f                                    */
#define FM_CMD_MID                                   (0x1980)
#define FM_SEND_HK_MID                               (0x1981)
#define FM_HK_TLM_MID                                (0x0982)
#define FM_OPEN_FILES_TLM_MID                        (0x0983)
#define FM_FREE_SPACE_TLM_MID                        (0x0984)
#define FM_FILE_INFO_TLM_MID                         (0x0985)
#define FM_DIR_LIST_TLM_MID                          (0x0986)


/* HK                                                      */
/* Range: 0x990 - 0x99f                                    */
#define HK_CMD_MID                                   (0x1990)
#define HK_SEND_HK_MID                               (0x1991)
#define HK_SEND_COMBINED_PKT_MID                     (0x1992)
#define HK_HK_TLM_MID                                (0x0993)
#define HK_COMBINED_PKT1_MID                         (0x0994)
#define HK_COMBINED_PKT2_MID                         (0x0995)
#define HK_COMBINED_PKT3_MID                         (0x0996)
#define HK_COMBINED_PKT4_MID                         (0x0997)
#define HK_COMBINED_PKT5_MID                         (0x0998)
#define HK_COMBINED_PKT6_MID                         (0x0999)
#define HK_COMBINED_PKT7_MID                         (0x099a)
#define HK_COMBINED_PKT8_MID                         (0x099b)
#define HK_COMBINED_PKT9_MID                         (0x099c)
#define HK_COMBINED_PKT10_MID                        (0x099d)


/* HS                                                      */
/* Range: 0x9a0 - 0x9af                                    */
#define HS_CMD_MID                                   (0x19a0)
#define HS_SEND_HK_MID                               (0x19a1)
#define HS_WAKEUP_MID                                (0x19a2)
#define HS_HK_TLM_MID                                (0x09a3)
#define HS_CUSTOM_HK_TLM_MID                         (0x09a4)


/* LC                                                      */
/* Range: 0x9b0 - 0x9bf                                    */
#define LC_HK_TLM_MID                                (0x09b0)
#define LC_SEND_HK_MID                               (0x19b1)
#define LC_CMD_MID                                   (0x19b2)
#define LC_SAMPLE_AP_MID                             (0x09b3)


/* MM                                                      */
/* Range: 0x9c0 - 0x9c2                                    */
#define MM_CMD_MID                                   (0x19c0)
#define MM_SEND_HK_MID                               (0x19c1)
#define MM_HK_TLM_MID                                (0x09c2)


/* MD                                                      */
/* Range: 0x9c3 - 0x9cf                                    */
#define MD_CMD_MID                                   (0x19c3)
#define MD_WAKEUP_MID                                (0x19c4)
#define MD_SEND_HK_MID                               (0x19c5)
#define MD_HK_TLM_MID                                (0x09c6)
#define MD_DWELL_PKT_MID_BASE                        (0x09c7)


/* SC                                                      */
/* Range: 0x9e0 - 0x9e3                                    */
#define SC_CMD_MID                                   (0x19e0)
#define SC_SEND_HK_MID                               (0x19e1)
#define SC_1HZ_WAKEUP_MID                            (0x19e2)
#define SC_HK_TLM_MID                                (0x09e3)


/* SCH                                                     */
/* Range: 0x9e4 - 0x9ef                                    */
#define SCH_CMD_MID                                  (0x19e4)
#define SCH_SEND_HK_MID                              (0x19e5)
#define SCH_UNUSED_MID                                    (0)
#define SCH_DIAG_TLM_MID                             (0x09e7)
#define SCH_HK_TLM_MID                               (0x09e8)


#define SCH_ACTIVITY_DONE_MID                        (0x0f00)


/* EA                                                      */
#define EA_CMD_MID                                   (0x1c29)
#define EA_SEND_HK_MID                               (0x1c2a)
#define EA_HK_TLM_MID                                (0x0c2b)
#define EA_WAKEUP_MID                                (0x1c2c)
#define EA_OUT_DATA_MID                              (0x0c2d)
#define EA_PERFMON_MID                               (0x1c2e)

/* VC                                                      */
#define VC_HK_TLM_MID                                (0x0c39)
#define VC_SEND_HK_MID                               (0x1c3a)
#define VC_CMD_MID                                   (0x1c3b)
#define VC_PROCESS_CMDS_MID                          (0x1c3c)

/* AMC                                                     */
#define AMC_CMD_MID                                  (0x1c3d)
#define AMC_SEND_HK_MID                              (0x1c3e)
#define AMC_UPDATE_MOTORS_MID                        (0x1c3f)
#define AMC_OUT_DATA_MID                             (0x0c40)
#define AMC_HK_TLM_MID                               (0x0c41)

/* MAC                                                     */
#define MAC_CMD_MID                                  (0x1c42)
#define MAC_SEND_HK_MID                              (0x1c43)
#define MAC_RUN_CONTROLLER_MID                       (0x1c44)
#define MAC_OUT_DATA_MID                             (0x0c45)
#define MAC_HK_TLM_MID                               (0x0c46)

/* ULR                                                     */
#define ULR_CMD_MID                                  (0x1c47)
#define ULR_SEND_HK_MID                              (0x1c48)
#define ULR_MEASURE_MID                              (0x1c49)
#define ULR_OUT_DATA_MID                             (0x0c4a)
#define ULR_HK_TLM_MID                               (0x0c4b)

/* RGBLED                                                  */
#define RGBLED_CMD_MID                               (0x1c4c)
#define RGBLED_SEND_HK_MID                           (0x1c4d)
#define RGBLED_WAKEUP_MID                            (0x1c4e)
#define RGBLED_HK_TLM_MID                            (0x0c4f)

/* GPS                                                     */
#define GPS_CMD_MID                                  (0x1c51)
#define GPS_SEND_HK_MID                              (0x1c52)
#define GPS_READ_SENSOR_MID                          (0x1c53)
#define GPS_HK_TLM_MID                               (0x0c54)

/* SENS                                                    */
#define SENS_HK_TLM_MID                              (0x0c55)
#define SENS_SEND_HK_MID                             (0x1c56)
#define SENS_WAKEUP_MID                              (0x1c57)
#define SENS_CMD_MID                                 (0x1c58)

/* QAE                                                     */
#define QAE_HK_TLM_MID                               (0x0c59)
#define QAE_SEND_HK_MID                              (0x1c5a)
#define QAE_WAKEUP_MID                               (0x1c5b)
#define QAE_CMD_MID                                  (0x1c5c)

/* LD                                                      */
#define LD_HK_TLM_MID                                (0x0c5d)
#define LD_SEND_HK_MID                               (0x1c5e)
#define LD_WAKEUP_MID                                (0x1c5f)
#define LD_CMD_MID                                   (0x1c60)

/* MPC                                                     */
#define MPC_HK_TLM_MID                               (0x0c61)
#define MPC_SEND_HK_MID                              (0x1c62)
#define MPC_WAKEUP_MID                               (0x1c63)
#define MPC_CMD_MID                                  (0x1c64)

/* NAV                                                     */
#define NAV_HK_TLM_MID                               (0x0c65)
#define NAV_SEND_HK_MID                              (0x1c66)
#define NAV_WAKEUP_MID                               (0x1c67)
#define NAV_CMD_MID                                  (0x1c68)

/* RCIN                                                    */
#define RCIN_HK_TLM_MID                              (0x0c69)
#define RCIN_SEND_HK_MID                             (0x1c6a)
#define RCIN_WAKEUP_MID                              (0x1c6b)
#define RCIN_CMD_MID                                 (0x1c6c)

/* VM                                                      */
#define VM_HK_TLM_MID                                (0x0c6d)
#define VM_SEND_HK_MID                               (0x1c6e)
#define VM_WAKEUP_MID                                (0x1c6f)
#define VM_CMD_MID                                   (0x1c70)

/* BAT                                                     */
#define BAT_HK_TLM_MID                               (0x0c71)
#define BAT_SEND_HK_MID                              (0x1c72)
#define BAT_WAKEUP_MID                               (0x1c73)
#define BAT_CMD_MID                                  (0x1c74)

/* PE                                                      */
#define PE_HK_TLM_MID                                (0x0c79)
#define PE_SEND_HK_MID                               (0x1c7a)
#define PE_WAKEUP_MID                                (0x1c7b)
#define PE_CMD_MID                                   (0x1c7c)

/* SIM                                                     */
#define SIM_HK_TLM_MID                               (0x0c7d)
#define SIM_SEND_HK_MID                              (0x1c7e)
#define SIM_WAKEUP_MID                               (0x1c7f)
#define SIM_CMD_MID                                  (0x1c80)

/* MAVLINK                                                 */
#define MAVLINK_CMD_MID                              (0x1c81)
#define MAVLINK_SEND_HK_MID                          (0x1c82)
#define MAVLINK_HK_TLM_MID                           (0x0c83)
#define MAVLINK_WAKEUP_MID                           (0x1c84)
#define MAVLINK_OUT_DATA_MID                         (0x1c85)
#define MAVLINK_PARAM_DATA_MID                       (0x1c86)
#define MAVLINK_REQUEST_PARAMS_MID                   (0x1c87)

/* Individual GPS messages                                 */
#define GPS_NAV_DOP_MID                              (0x0c87)
#define GPS_NAV_NAVPVT_MID                           (0x0c88)
#define GPS_NAV_SVINFO_MID                           (0x0c89)
#define GPS_ACK_NAK_MID                              (0x0c90)
#define GPS_ACK_ACK_MID                              (0x0c91)
#define GPS_CFG_PRT_MID                              (0x0c92)
#define GPS_CFG_MSG_MID                              (0x0c93)
#define GPS_CFG_RATE_MID                             (0x0c94)
#define GPS_CFG_SBAS_MID                             (0x0c95)
#define GPS_CFG_NAV5_MID                             (0x0c96)
#define GPS_MON_HW_MID                               (0x0c97)

/* HMC5883                                                 */
#define HMC5883_HK_TLM_MID                           (0x0ca1)
#define HMC5883_SEND_HK_MID                          (0x1ca2)
#define HMC5883_WAKEUP_MID                           (0x1ca3)
#define HMC5883_CMD_MID                              (0x1ca4)
#define HMC5883_DIAG_TLM_MID                         (0x0ca5)

/* PRMLIB                                                  */
#define PRMLIB_PARAM_UPDATED_MID                     (0x0ca6)

/* LGC                                                     */
#define LGC_HK_TLM_MID                               (0x0cb0)
#define LGC_SEND_HK_MID                              (0x1cb1)
#define LGC_WAKEUP_MID                               (0x1cb2)
#define LGC_CMD_MID                                  (0x1cb3)

/* MS5607                                                  */
#define MS5607_SEND_HK_MID                           (0x0cb5)
#define MS5607_HK_TLM_MID                            (0x0cb6)
#define MS5607_DIAG_TLM_MID                          (0x0cb7)
#define MS5607_CMD_MID                               (0x0cb8)
#define MS5607_MEASURE_MID                           (0x0cb9)

/* AK8963                                                  */
#define AK8963_HK_TLM_MID                            (0x0cc1)
#define AK8963_SEND_HK_MID                           (0x1cc2)
#define AK8963_WAKEUP_MID                            (0x1cc3)
#define AK8963_CMD_MID                               (0x1cc4)
#define AK8963_DIAG_TLM_MID                          (0x0cc5)

/* MPU6050                                                 */
#define MPU6050_SEND_HK_MID                          (0x1cd0)
#define MPU6050_HK_TLM_MID                           (0x0cd1)
#define MPU6050_DIAG_TLM_MID                         (0x0cd2)
#define MPU6050_CMD_MID                              (0x1cd3)
#define MPU6050_MEASURE_MID                          (0x1cd4)

/* SONAR                                                   */
#define SONAR_CMD_MID                                (0x1cd6)
#define SONAR_SEND_HK_MID                            (0x1cd7)
#define SONAR_MEASURE_MID                            (0x1cd8)
#define SONAR_OUT_DATA_MID                           (0x0cd9)
#define SONAR_HK_TLM_MID                             (0x0ce0)

/* FLOW                                                    */
#define FLOW_HK_TLM_MID                              (0x0ce1)
#define FLOW_SEND_HK_MID                             (0x1ce2)
#define FLOW_WAKEUP_MID                              (0x1ce3)
#define FLOW_CMD_MID                                 (0x1ce4)
#define FLOW_FRAME_MID                               (0x0ce5)
#define FLOW_DIAG_TLM_MID                            (0x0ce6)

/* MPC                                                     */
#define MPC_DIAG_TLM_MID                             (0x0cea)

/* LD                                                      */
#define LD_DIAG_TLM_MID                              (0x0ceb)


#define VM_CONFIG_TLM_MID                            (0x0cec)

/* PRM                                                     */
#define PRM_CMD_MID                                  (0x1ced)
#define PRM_SEND_HK_MID                              (0x1cee)
#define PRM_WAKEUP_MID                               (0x1cef)
#define PRM_OUT_DATA_MID                             (0x1cf0)
#define PRM_HK_TLM_MID                               (0x0cf1)

#endif
