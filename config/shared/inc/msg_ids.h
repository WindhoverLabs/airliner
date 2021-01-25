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

#endif
