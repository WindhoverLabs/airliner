/****************************************************************************
 *
 *   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Windhover Labs nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
    
#ifndef EM91X_MSG_H
#define EM91X_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define EM91X_PASSWORD_LENGTH_MAX                           (50)

/************************************************************************
** EM91X Command Codes
*************************************************************************/

/** \em91xcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the EM91X task is alive
**
**  \em91xcmdmnemonic \EM91X_NOOP
**
**  \par Command Structure
**       #EM91X_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EM91X_CMDACPTCNT - command counter will increment
**       - The #EM91X_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \EM91X_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EM91X_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EM91X_RESET_CC
*/
#define EM91X_NOOP_CC                 (0)

/** \em91xcmd Reset Counters
**  
**  \par Description
**       Resets the em91x housekeeping counters
**
**  \em91xcmdmnemonic \EM91X_TLMRST
**
**  \par Command Structure
**       #EM91X_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \EM91X_CMDACTPCNT       - command counter will be cleared
**       - \b \c \EM91X_CMDRJCTCNT       - command error counter will be cleared
**       - The #EM91X_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \EM91X_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #EM91X_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #EM91X_NOOP_CC
*/
#define EM91X_RESET_COUNTS_CC       (  1)

/* TODO */
/* Password Commands                                                    */
#define EM91X_ENTERCND_CC           (  2)
#define EM91X_SETCND_CC             (  3)

/* Modem Status, Customization, and Reset Commands.                     */
#define EM91X_ANTSEL_CC             ( 10)
#define EM91X_BAND_QUERY_CC         ( 11)
#define EM91X_BAND_SET_CC           ( 12)
#define EM91X_BOOTHOLD_CC           ( 13)
#define EM91X_CUSTOM_CC             ( 13)
#define EM91X_DATALOOPBACK_CC       ( 14)
#define EM91X_DISABLEDEBUG_CC       ( 15)
#define EM91X_GCFEN_CC              ( 16)
#define EM91X_GSTATUS_CC            ( 17)
#define EM91X_HWID_CC               ( 18)
#define EM91X_IMAGE_CC              ( 19)
#define EM91X_IMPREF_CC             ( 20)
#define EM91X_LTEINFO_CC            ( 21)
#define EM91X_LTERXCONTROL_CC       ( 22)
#define EM91X_MMWBYPASSSCAN_CC      ( 23)
#define EM91X_MMWCAL_CC             ( 24)
#define EM91X_NRINFO_CC             ( 25)
#define EM91X_NVENCRYPTIMEI_CC      ( 26)
#define EM91X_NVPLMN_CC             ( 27)
#define EM91X_PCIESSDID_CC          ( 28)
#define EM91X_PCIESSVID_CC          ( 29)
#define EM91X_PCINFO_CC             ( 30)
#define EM91X_PCOFFEN_CC            ( 31)
#define EM91X_PCTEMP_CC             ( 32)
#define EM91X_PCTEMPLIMITS_CC       ( 33)
#define EM91X_PCVOLT_CC             ( 34)
#define EM91X_PCVOLTLIMITS_CC       ( 35)
#define EM91X_PRID_CC               ( 36)
#define EM91X_RATCA_CC              ( 37)
#define EM91X_RESETMODEM_CC         ( 38)
#define EM91X_RFCID_CC              ( 39)
#define EM91X_RFCMBNSCAN_CC         ( 40)
#define EM91X_RFDEVSTATUS_CC        ( 41)
#define EM91X_RXDEN_CC              ( 42)
#define EM91X_SELRAT_CC             ( 43)
#define EM91X_TMCONFIG_CC           ( 44)
#define EM91X_TMSTATUS_CC           ( 45)
#define EM91X_USBCOMP_CC            ( 46)
#define EM91X_USBPID_CC             ( 47)
#define EM91X_USBVID_CC             ( 48)
#define EM91X_VERINFO_CC            ( 49)
#define EM91X_GETOPMODE_CC          ( 50)

/* Diagnostic Commands.                                                 */
#define EM91X_BCFWUPDATESTATUS_CC   ( 51)
#define EM91X_DISPLAY_DIAG_CC       ( 52)
#define EM91X_GCCLR_CC              ( 53)
#define EM91X_GCDUMP_CC             ( 54)
#define EM91X_IMSTESTMODE_CC        ( 55)
#define EM91X_LEDTEST_CC            ( 56)

/* Test Commands.                                                       */
#define EM91X_DACGPSCTON_CC         ( 57)
#define EM91X_DACGPSSTANDALONE_CC   ( 58)
#define EM91X_DACGPSTESTMODE_CC     ( 59)
#define EM91X_DAFTMACT_CC           ( 60)
#define EM91X_DAFTMDEACT_CC         ( 61)
#define EM91X_DAGFTMRXAGC_CC        ( 62)
#define EM91X_DAUPDATEPARAM_CC      ( 63)
#define EM91X_DARCONFIG_CC          ( 64)
#define EM91X_DARCONFIGDROP_CC      ( 65)
#define EM91X_DASUB6TECHACT_CC      ( 66)
#define EM91X_DATXCONTROL_CC        ( 67)
#define EM91X_DAMMWACT_CC           ( 68)
#define EM91X_DAMMWDEACT_CC         ( 69)

/* Memory Management Commands.                                          */
#define EM91X_NVBACKUP_CC           ( 70)
#define EM91X_RMARESET_CC           ( 71)

/* GNSS Commands.                                                       */
#define EM91X_GPSAUTOSTART_CC       ( 72)
#define EM91X_GPSCLRASSIST_CC       ( 73)
#define EM91X_GPSCOLDSTART_CC       ( 74)
#define EM91X_GPSEND_CC             ( 75)
#define EM91X_GPSFIX_CC             ( 76)
#define EM91X_GPSLBSAPN_CC          ( 77)
#define EM91X_GPSLOC_CC             ( 78)
#define EM91X_GPSMOMETHOD_CC        ( 79)
#define EM91X_GPSPORTID_CC          ( 80)
#define EM91X_GPSSATINFO_CC         ( 81)
#define EM91X_GPSSTATUS_CC          ( 82)
#define EM91X_GPSSUPLURL_CC         ( 83)
#define EM91X_GPSSUPLVER_CC         ( 84)
#define EM91X_GPSTRACK_CC           ( 85)

/* SIM Commands.                                                        */
#define EM91X_UIMS_CC               ( 86)
#define EM91X_IMSIM_CC              ( 87)

/* SAR Commands.                                                        */
#define EM91X_SARINTGPIOMODE_CC     ( 88)
#define EM91X_SARSTATE_CC           ( 89)
#define EM91X_STEFS_CC              ( 90)

/* DM Commands.                                                         */
#define EM91X_DMSESSION_CC          ( 91)
#define EM91X_DMREAD_CC             ( 92)
#define EM91X_DMREADALL_CC          ( 93)
#define EM91X_DMDEBUG_CC            ( 94)
#define EM91X_DMSUPPORT_CC          ( 95)

/* Supported 3GPP and Carrier Commands.                                 */
#define EM91X_V250_GMI_CC           ( 96)
#define EM91X_V250_GMM_CC           ( 97)
#define EM91X_V250_GMR_CC           ( 98)
#define EM91X_V250_E_CC             ( 99)
#define EM91X_V250_I_CC             (100)
#define EM91X_V250_V1_CC            (101)
#define EM91X_27005_CMGC_CC         (102)
#define EM91X_27005_CMGD_CC         (103)
#define EM91X_27005_CMGF_CC         (104)
#define EM91X_27005_CMGL_CC         (105)
#define EM91X_27005_CMGR_CC         (106)
#define EM91X_27005_CMGS_CC         (107)
#define EM91X_27005_CMGW_CC         (108)
#define EM91X_27005_CMMS_CC         (109)
#define EM91X_27005_CMSS_CC         (110)
#define EM91X_27005_CMT_CC          (111)
#define EM91X_27005_CMTI_CC         (112)
#define EM91X_27005_CNMA_CC         (113)
#define EM91X_27005_CPMS_CC         (114)
#define EM91X_27005_CRES_CC         (115)
#define EM91X_27005_CSAS_CC         (116)
#define EM91X_27005_CSCA_CC         (117)
#define EM91X_27005_CSCB_CC         (118)
#define EM91X_27005_CSDH_CC         (119)
#define EM91X_27005_CSMP_CC         (120)
#define EM91X_27005_CSMS_CC         (121)
#define EM91X_27007_CBST_CC         (122)
#define EM91X_27007_CCLK_CC         (123)
#define EM91X_27007_CEER_CC         (124)
#define EM91X_27007_CEREG_CC        (125)
#define EM91X_27007_CGACT_CC        (126)
#define EM91X_27007_CGATT_CC        (127)
#define EM91X_27007_CGCMOD_CC       (128)
#define EM91X_27007_CGDCONT_CC      (129)
#define EM91X_27007_CGDSCONT_CC     (130)
#define EM91X_27007_CGEQREQ_CC      (131)
#define EM91X_27007_CGMI_CC         (132)
#define EM91X_27007_CGMM_CC         (133)
#define EM91X_27007_CGMR_CC         (134)
#define EM91X_27007_CGPADDR_CC      (135)
#define EM91X_27007_CGPIAF_CC       (136)
#define EM91X_27007_CGSMS_CC        (137)
#define EM91X_27007_CGSN_CC         (138)
#define EM91X_27007_CGTFT_CC        (139)
#define EM91X_27007_CIMI_CC         (140)
#define EM91X_27007_CIND_CC         (141)
#define EM91X_27007_CLAC_CC         (142)
#define EM91X_27007_CLCK_CC         (143)
#define EM91X_27007_CMEE_CC         (144)
#define EM91X_27007_CMER_CC         (145)
#define EM91X_27007_CNUM_CC         (146)
#define EM91X_27007_COPN_CC         (147)
#define EM91X_27007_COPS_CC         (148)
#define EM91X_27007_CPAS_CC         (149)
#define EM91X_27007_CPIN_CC         (150)
#define EM91X_27007_CPOL_CC         (151)
#define EM91X_27007_CPWD_CC         (152)
#define EM91X_27007_CREG_CC         (153)
#define EM91X_27007_CRSM_CC         (154)
#define EM91X_27007_CSCS_CC         (155)
#define EM91X_27007_CSIM_CC         (156)
#define EM91X_27007_CSQ_CC          (157)
#define EM91X_27007_CUSD_CC         (158)
#define EM91X_27007_WS46_CC         (159)
#define EM91X_VERIZON_VZWAPNE_CC    (160)
#define EM91X_VERIZON_VZWRSRP_CC    (161)
#define EM91X_VERIZON_VZWRSRQ_CC    (162)
#define EM91X_ATT_RSCP_CC           (163)


/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #EM91X_NOOP_CC, #EM91X_RESET_CC
**  Also see #EM91X_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} EM91X_NoArgCmd_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific incoming data
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices.
    **        Option: for data that is already defined by another app, include
    **        that app's message header above.
    */

} EM91X_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} EM91X_OutData_t;

/** 
**  \brief EM91X application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \em91xtlmmnemonic \EM91X_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              CmdCounter;

    /** \em91xtlmmnemonic \EM91X_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              ErrCounter;

} EM91X_HkTlm_t;


typedef struct
{
    uint8  CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char   Password[EM91X_PASSWORD_LENGTH_MAX];
} EM91X_PasswordCmd_t;


typedef struct
{
    uint8  CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8  TechNum;
    uint8  Band;
    uint8  GPIO1;
    uint8  GPIO2;
    uint8  GPIO3;
    uint8  GPIO4;
} EM91X_AntSelCmd_t;


#ifdef __cplusplus
}
#endif

#endif /* EM91X_MSG_H */

/************************/
/*  End of File Comment */
/************************/
