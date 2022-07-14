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

#ifndef _GPS_PARSER_UBX_COMMON_H_
#define _GPS_PARSER_UBX_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "gps_parser_ubx_ack.h"
#include "gps_parser_ubx_cfg.h"
#include "gps_parser_ubx_mon.h"
#include "gps_parser_ubx_nav.h"
#include "gps_ubx_msg.h"
#include "../gps_custom_shared.h"



/************************************************************************
** Local Defines
*************************************************************************/
/* UBX header contents */
/** \brief Header symbol 1.
**
**  \par Description:
**       UBX header symbol 1.
*/
#define GPS_PARSER_SYNC1_VALUE                    (0xb5)

/** \brief Header symbol 2.
**
**  \par Description:
**       UBX header symbol 2.
*/
#define GPS_PARSER_SYNC2_VALUE                    (0x62)

/************************************************************************
** External Global Variables
*************************************************************************/
extern GPS_AppCustomData_t GPS_AppCustomData;


/************************************************************************
** Structure Declarations
*************************************************************************/

typedef enum
{
    GPS_PARSER_CLASS_NAV_VALUE          = 0x01,
    GPS_PARSER_CLASS_RXM_VALUE          = 0x02,
    GPS_PARSER_CLASS_INF_VALUE          = 0x04,
    GPS_PARSER_CLASS_ACK_VALUE          = 0x05,
    GPS_PARSER_CLASS_CFG_VALUE          = 0x06,
    GPS_PARSER_CLASS_UPD_VALUE          = 0x09,
    GPS_PARSER_CLASS_MON_VALUE          = 0x0a,
    GPS_PARSER_CLASS_AID_VALUE          = 0x0b,
    GPS_PARSER_CLASS_TIM_VALUE          = 0x0d,
    GPS_PARSER_CLASS_ESF_VALUE          = 0x10,
    GPS_PARSER_CLASS_MGA_VALUE          = 0x13,
    GPS_PARSER_CLASS_LOG_VALUE          = 0x21,
    GPS_PARSER_CLASS_SEC_VALUE          = 0x27,
    GPS_PARSER_CLASS_HNR_VALUE          = 0x28
} GPS_ParserClassValue_t;


typedef enum
{
    GPS_PARSER_NAV_POSECEF_ID_VALUE     = 0x01,
    GPS_PARSER_NAV_POSLLH_ID_VALUE      = 0x02,
    GPS_PARSER_NAV_STATUS_ID_VALUE      = 0x03,
    GPS_PARSER_NAV_DOP_ID_VALUE         = 0x04,
    GPS_PARSER_NAV_ATT_ID_VALUE         = 0x05,
    GPS_PARSER_NAV_SOL_ID_VALUE         = 0x06,
    GPS_PARSER_NAV_NAVPVT_ID_VALUE      = 0x07,
    GPS_PARSER_NAV_ODO_ID_VALUE         = 0x09,
    GPS_PARSER_NAV_RESETODO_ID_VALUE    = 0x10,
    GPS_PARSER_NAV_VELECEF_ID_VALUE     = 0x11,
    GPS_PARSER_NAV_VELNED_ID_VALUE      = 0x12,
    GPS_PARSER_NAV_TIMEGPS_ID_VALUE     = 0x20,
    GPS_PARSER_NAV_TIMEUTC_ID_VALUE     = 0x21,
    GPS_PARSER_NAV_CLOCK_ID_VALUE       = 0x22,
    GPS_PARSER_NAV_TIMEGLO_ID_VALUE     = 0x23,
    GPS_PARSER_NAV_TIMEBDS_ID_VALUE     = 0x24,
    GPS_PARSER_NAV_TIMEGAL_ID_VALUE     = 0x25,
    GPS_PARSER_NAV_TIMELS_ID_VALUE      = 0x26,
    GPS_PARSER_NAV_SVINFO_ID_VALUE      = 0x30,
    GPS_PARSER_NAV_DGPS_ID_VALUE        = 0x31,
    GPS_PARSER_NAV_SBAS_ID_VALUE        = 0x32,
    GPS_PARSER_NAV_ORB_ID_VALUE         = 0x34,
    GPS_PARSER_NAV_NAVSAT_ID_VALUE      = 0x35,
    GPS_PARSER_NAV_GEOFENCE_ID_VALUE    = 0x39,
    GPS_PARSER_NAV_AOPSTATUS_ID_VALUE   = 0x60,
    GPS_PARSER_NAV_EOE_ID_VALUE         = 0x61,
} GPS_ParserNavIDValue_t;


typedef enum
{
    GPS_PARSER_RXM_SFRBX_ID_VALUE       = 0x13,
    GPS_PARSER_RXM_MEASX_ID_VALUE       = 0x14,
    GPS_PARSER_RXM_RAWX_ID_VALUE        = 0x15,
    GPS_PARSER_RXM_SVSI_ID_VALUE        = 0x20,
    GPS_PARSER_RXM_PMREQ_ID_VALUE       = 0x41,
    GPS_PARSER_RXM_RLM_ID_VALUE         = 0x59,
    GPS_PARSER_RXM_IMES_ID_VALUE        = 0x61
} GPS_ParserRxmIDValue_t;


typedef enum
{
    GPS_PARSER_ACK_NAK_ID_VALUE         = 0x00,
    GPS_PARSER_ACK_ACK_ID_VALUE         = 0x01
} GPS_ParserAckIDValue_t;


typedef enum
{
    GPS_PARSER_CFG_PRT_ID_VALUE         = 0x00,
    GPS_PARSER_CFG_MSG_ID_VALUE         = 0x01,
    GPS_PARSER_CFG_INF_ID_VALUE         = 0x02,
    GPS_PARSER_CFG_RST_ID_VALUE         = 0x04,
    GPS_PARSER_CFG_DAT_ID_VALUE         = 0x06,
    GPS_PARSER_CFG_RATE_ID_VALUE        = 0x08,
    GPS_PARSER_CFG_CFG_ID_VALUE         = 0x09,
    GPS_PARSER_CFG_RXM_ID_VALUE         = 0x11,
    GPS_PARSER_CFG_ANT_ID_VALUE         = 0x13,
    GPS_PARSER_CFG_SBAS_ID_VALUE        = 0x16,
    GPS_PARSER_CFG_NMEA_ID_VALUE        = 0x17,
    GPS_PARSER_CFG_USB_ID_VALUE         = 0x1B,
    GPS_PARSER_CFG_ODO_ID_VALUE         = 0x1E,
    GPS_PARSER_CFG_NAVX5_ID_VALUE       = 0x23,
    GPS_PARSER_CFG_NAV5_ID_VALUE        = 0x24,
    GPS_PARSER_CFG_TP5_ID_VALUE         = 0x31,
    GPS_PARSER_CFG_RINV_ID_VALUE        = 0x34,
    GPS_PARSER_CFG_ITFM_ID_VALUE        = 0x39,
    GPS_PARSER_CFG_PM2_ID_VALUE         = 0x3B,
    GPS_PARSER_CFG_TMOD2_ID_VALUE       = 0x3D,
    GPS_PARSER_CFG_GNSS_ID_VALUE        = 0x3E,
    GPS_PARSER_CFG_LOGFILTER_ID_VALUE   = 0x47,
    GPS_PARSER_CFG_PWR_ID_VALUE         = 0x57,
    GPS_PARSER_CFG_TXSLOT_ID_VALUE      = 0x53,
    GPS_PARSER_CFG_HNR_ID_VALUE         = 0x5C,
    GPS_PARSER_CFG_ESRC_ID_VALUE        = 0x60,
    GPS_PARSER_CFG_DOSC_ID_VALUE        = 0x61,
    GPS_PARSER_CFG_SMGR_ID_VALUE        = 0x62,
    GPS_PARSER_CFG_GEOFENCE_ID_VALUE    = 0x69,
    GPS_PARSER_CFG_FIXSEED_ID_VALUE     = 0X84,
    GPS_PARSER_CFG_DYNSEED_ID_VALUE     = 0x85,
    GPS_PARSER_CFG_PMS_ID_VALUE         = 0x86
} GPS_ParserCfgIDValue_t;


typedef enum
{
    GPS_PARSER_UPD_SOS_ID_VALUE         = 0x14,
} GPS_ParserUpdIDValue_t;


typedef enum
{
    GPS_PARSER_MON_IO_ID_VALUE          = 0x02,
    GPS_PARSER_MON_VER_ID_VALUE         = 0x04,
    GPS_PARSER_MON_MSGPP_ID_VALUE       = 0x06,
    GPS_PARSER_MON_RXBUF_ID_VALUE       = 0x07,
    GPS_PARSER_MON_TXBUF_ID_VALUE       = 0x08,
    GPS_PARSER_MON_HW_ID_VALUE          = 0x09,
    GPS_PARSER_MON_HW2_ID_VALUE         = 0x0B,
    GPS_PARSER_MON_RXR_ID_VALUE         = 0x21,
    GPS_PARSER_MON_PATCH_ID_VALUE       = 0x27,
    GPS_PARSER_MON_GNSS_ID_VALUE        = 0x28,
    GPS_PARSER_MON_SMGR_ID_VALUE        = 0x2E
} GPS_ParserMonIDValue_t;


typedef enum
{
    GPS_PARSER_AID_INI_ID_VALUE         = 0x01,
    GPS_PARSER_AID_HUI_ID_VALUE         = 0x02,
    GPS_PARSER_AID_ALM_ID_VALUE         = 0x30,
    GPS_PARSER_AID_EPH_ID_VALUE         = 0x31,
    GPS_PARSER_AID_AOP_ID_VALUE         = 0x33
} GPS_ParserAidIDValue_t;


typedef enum
{
    GPS_PARSER_TIM_TP_ID_VALUE          = 0x01,
    GPS_PARSER_TIM_TM2_ID_VALUE         = 0x03,
    GPS_PARSER_TIM_SVIN_ID_VALUE        = 0x04,
    GPS_PARSER_TIM_VRFY_ID_VALUE        = 0x06,
    GPS_PARSER_TIM_DOSC_ID_VALUE        = 0x11,
    GPS_PARSER_TIM_TOS_ID_VALUE         = 0x12,
    GPS_PARSER_TIM_SMEAS_ID_VALUE       = 0x13,
    GPS_PARSER_TIM_VCOCAL_ID_VALUE      = 0x15,
    GPS_PARSER_TIM_FCHG_ID_VALUE        = 0x16,
    GPS_PARSER_TIM_HOC_ID_VALUE         = 0x17
} GPS_ParserTimIDValue_t;


typedef enum
{
    GPS_PARSER_ESF_MEAS_ID_VALUE        = 0x02,
    GPS_PARSER_ESF_RAW_ID_VALUE         = 0x03,
    GPS_PARSER_ESF_INS_ID_VALUE         = 0x15,
    GPS_PARSER_ESF_STATUS_ID_VALUE      = 0x10
} GPS_ParserEsfIDValue_t;


typedef enum
{
    GPS_PARSER_MGA_GPS_ID_VALUE         = 0x00,
    GPS_PARSER_MGA_GAL_ID_VALUE         = 0x02,
    GPS_PARSER_MGA_BDS_ID_VALUE         = 0x03,
    GPS_PARSER_MGA_QZSS_ID_VALUE        = 0x05,
    GPS_PARSER_MGA_GLO_ID_VALUE         = 0x06,
    GPS_PARSER_MGA_ANO_ID_VALUE         = 0x20,
    GPS_PARSER_MGA_FLASH_ID_VALUE       = 0x21,
    GPS_PARSER_MGA_INI_XYZ_ID_VALUE     = 0x40,
    GPS_PARSER_MGA_ACK_DATA0_ID_VALUE   = 0x60,
    GPS_PARSER_MGA_DBD_ID_VALUE         = 0x80
} GPS_ParserMgaIDValue_t;


typedef enum
{
    GPS_PARSER_LOG_ERASE_ID_VALUE            = 0x03,
    GPS_PARSER_LOG_STRING_ID_VALUE           = 0x04,
    GPS_PARSER_LOG_CREATE_ID_VALUE           = 0x07,
    GPS_PARSER_LOG_INFO_ID_VALUE             = 0x08,
    GPS_PARSER_LOG_RETRIEVE_ID_VALUE         = 0x09,
    GPS_PARSER_LOG_RETRIEVEPOS_ID_VALUE      = 0x0B,
    GPS_PARSER_LOG_RETRIEVESTRING_ID_VALUE   = 0x0D,
    GPS_PARSER_LOG_FINDTIME_ID_VALUE         = 0x0E,
    GPS_PARSER_LOG_RETRIEVEPOSEXTRA_ID_VALUE = 0x0F
} GPS_ParserLogIDValue_t;


typedef enum
{
    GPS_PARSER_SEC_SIGN_ID_VALUE             = 0x01,
    GPS_PARSER_SEC_UNIQID_ID_VALUE           = 0x03,
} GPS_ParserSecIDValue_t;


typedef enum
{
    GPS_PARSER_HNR_PVT_ID_VALUE              = 0x00,
} GPS_ParserHnrIDValue_t;


typedef enum
{
    GPS_PARSER_INF_ERROR_ID_VALUE            = 0x00,
    GPS_PARSER_INF_WARNING_ID_VALUE          = 0x01,
    GPS_PARSER_INF_NOTICE_ID_VALUE           = 0x02,
    GPS_PARSER_INF_TEST_ID_VALUE             = 0x03,
    GPS_PARSER_INF_DEBUG_ID_VALUE            = 0x04
} GPS_ParserInfIDValue_t;


uint16 GPS_ParseChar(uint8 byte, GPS_DeviceMessage_t* message, GPS_ParserStatus_t* status, boolean *done);
void GPS_Parser_Reset(void);
CFE_SB_MsgId_t GPS_TranslateMsgID(uint16 ClassID, uint16 MsgID);
void GPS_Parser_StateChange(GPS_ParserState_t newState);
void GPS_SetChecksum(uint8 *Buffer, uint16 Size);

#ifdef __cplusplus
}
#endif 

#endif /* _GPS_PARSER_UBX_COMMON_H_ */

