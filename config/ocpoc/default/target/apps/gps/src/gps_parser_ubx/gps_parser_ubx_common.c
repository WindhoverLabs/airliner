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

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "gps_parser_ubx_common.h"
#include "msg_ids.h"
//#include "GPS_events.h"
//#include "GPS_msgids.h"


/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Structure Declarations
*************************************************************************/

/*************************************************************************
** Local function prototypes
**************************************************************************/

void GPS_SetChecksum(uint8 *Buffer, uint16 Size)
{
    uint8 ckA = 0;
    uint8 ckB = 0;
    uint16 i = 0;

    for(i = 2; i < (Size-2); ++i)
    {
        ckA = ckA + Buffer[i];
        ckB = ckB + ckA;
    }

    Buffer[Size-2] = ckA;
    Buffer[Size-1] = ckB;
}


void GPS_Parser_Reset()
{
    GPS_AppCustomData.ParserStatus.ParseState = GPS_PARSE_STATE_IDLE;
}


void GPS_Parser_StateChange(GPS_ParserState_t newState)
{
    switch(newState)
    {
        case GPS_PARSE_STATE_UNINIT:
        {
            break;
        }

        case GPS_PARSE_STATE_IDLE:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_SYNC1:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_SYNC2:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_CLASS:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_ID:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_LENGTH1:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_LENGTH2:
        {
            GPS_AppCustomData.ParserStatus.PayloadCursor = 0;
            break;
        }

        case GPS_PARSE_STATE_GOT_PAYLOAD:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_CHECKSUMA:
        {
            break;
        }

        case GPS_PARSE_STATE_GOT_CHECKSUMB:
        {
            break;
        }

        default:
        {
            break;
        }
    }

    GPS_AppCustomData.ParserStatus.ParseState = newState;
}


uint16 GPS_ParseChar(uint8 byte, GPS_DeviceMessage_t* message, boolean *done)
{
    uint16 msg_received = 0;

    switch(GPS_AppCustomData.ParserStatus.ParseState)
    {
        
        case GPS_PARSE_STATE_UNINIT:
        {
            GPS_AppCustomData.ParserStatus.ParseError++;
            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                              "Parser not initialized");
            break;
        }

        case GPS_PARSE_STATE_IDLE:
        {
            if(byte == GPS_PARSER_SYNC1_VALUE)
            {
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_SYNC1);
            }
            else if(byte == 0xff)
            {
                *done = TRUE;
            }
            else
            {
                //GPS_AppCustomData.ParserStatus.ParseError++;
                //CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                //                  "Received unexpected byte (0x%.2x) in IDLE state.", byte);
            }
            break;
        }

        case GPS_PARSE_STATE_GOT_SYNC1:
        {
            if(byte == GPS_PARSER_SYNC2_VALUE)
            {
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_SYNC2);
            }
            else
            {
                GPS_AppCustomData.ParserStatus.ParseError++;
                CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                  "Received unexpected byte (0x%.2x) in GOT_SYNC1 state", byte);
                GPS_Parser_Reset();
            }
            break;
        }

        case GPS_PARSE_STATE_GOT_SYNC2:
        {
            switch(byte)
            {
                case GPS_PARSER_CLASS_NAV_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_RXM_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_INF_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_ACK_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_CFG_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_UPD_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_MON_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_AID_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_TIM_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_ESF_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_MGA_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_LOG_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_SEC_VALUE:
                /* Fall through */
                case GPS_PARSER_CLASS_HNR_VALUE:
                {
                    GPS_AppCustomData.ParserStatus.ClassID = byte;
                    GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_CLASS);
                    break;
                }

                default:
                {
                    GPS_AppCustomData.ParserStatus.ParseError++;
                    CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                  "Received message with unexpected class ID (0x%.2x)", byte);
                    GPS_Parser_Reset();
                }
            }
            break;
        }

        case GPS_PARSE_STATE_GOT_CLASS:
        {
            switch(GPS_AppCustomData.ParserStatus.ClassID)
            {
                case GPS_PARSER_CLASS_NAV_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_NAV_POSECEF_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_POSLLH_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_STATUS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_DOP_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_ATT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_SOL_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_NAVPVT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_ODO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_RESETODO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_VELECEF_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_VELNED_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMEGPS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMEUTC_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_CLOCK_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMEGLO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMEBDS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMEGAL_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_TIMELS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_SVINFO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_DGPS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_SBAS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_ORB_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_NAVSAT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_GEOFENCE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_AOPSTATUS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_NAV_EOE_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for NAV class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_RXM_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_RXM_SFRBX_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_MEASX_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_RAWX_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_SVSI_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_PMREQ_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_RLM_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_RXM_IMES_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for RXM class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                }

                case GPS_PARSER_CLASS_INF_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_INF_ERROR_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_INF_WARNING_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_INF_NOTICE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_INF_TEST_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_INF_DEBUG_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for INF class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_ACK_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_ACK_NAK_ID_VALUE:
                        { 
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }
                        case GPS_PARSER_ACK_ACK_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for ACK class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_CFG_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_CFG_PRT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_MSG_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_INF_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_RST_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_DAT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_RATE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_CFG_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_RXM_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_ANT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_SBAS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_NMEA_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_USB_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_ODO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_NAVX5_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_NAV5_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_TP5_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_RINV_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_ITFM_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_PM2_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_TMOD2_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_GNSS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_LOGFILTER_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_PWR_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_TXSLOT_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_HNR_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_ESRC_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_DOSC_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_SMGR_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_GEOFENCE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_FIXSEED_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_DYNSEED_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_CFG_PMS_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for CFG class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_UPD_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_UPD_SOS_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for UPD class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_MON_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_MON_IO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_VER_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_MSGPP_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_RXBUF_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_TXBUF_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_HW_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_HW2_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_RXR_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_PATCH_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_GNSS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MON_SMGR_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for MON class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_AID_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_AID_INI_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_AID_HUI_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_AID_ALM_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_AID_EPH_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_AID_AOP_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for AID class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_TIM_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_TIM_TP_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_TM2_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_SVIN_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_VRFY_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_DOSC_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_TOS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_SMEAS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_VCOCAL_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_FCHG_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_TIM_HOC_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for TIM class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_ESF_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_ESF_MEAS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_ESF_RAW_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_ESF_INS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_ESF_STATUS_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for ESF class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_MGA_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_MGA_GPS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_GAL_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_BDS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_QZSS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_GLO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_ANO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_FLASH_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_INI_XYZ_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_ACK_DATA0_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_MGA_DBD_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for MGA class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_LOG_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_LOG_ERASE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_STRING_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_CREATE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_INFO_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_RETRIEVE_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_RETRIEVEPOS_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_RETRIEVESTRING_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_FINDTIME_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_LOG_RETRIEVEPOSEXTRA_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for LOG class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_SEC_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_SEC_SIGN_ID_VALUE:
                        /* Fall through */
                        case GPS_PARSER_SEC_UNIQID_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for SEC class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_HNR_VALUE:
                {
                    switch(byte)
                    {
                        case GPS_PARSER_HNR_PVT_ID_VALUE:
                        {
                            GPS_AppCustomData.ParserStatus.MsgID = byte;
                            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_ID);
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for HNR class.", byte);
                            GPS_Parser_Reset();
                        }
                    }
                    break;
                }

                default:
                {
                    GPS_AppCustomData.ParserStatus.ParseError++;
                    CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                  "Received message with unexpected class ID (0x%.2x)", byte);
                    GPS_Parser_Reset();
                }
            }
            break;
        }

        case GPS_PARSE_STATE_GOT_ID:
        {
            GPS_AppCustomData.ParserStatus.MsgLength = byte;
            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_LENGTH1);
            break;
        }

        case GPS_PARSE_STATE_GOT_LENGTH1:
        {
            GPS_AppCustomData.ParserStatus.MsgLength += byte << 8;
            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_LENGTH2);
            break;
        }

        case GPS_PARSE_STATE_GOT_LENGTH2:
        {
            switch(GPS_AppCustomData.ParserStatus.ClassID)
            {
                case GPS_PARSER_CLASS_NAV_VALUE:
                {
                    switch(GPS_AppCustomData.ParserStatus.MsgID)
                    {
                        case GPS_PARSER_NAV_POSECEF_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_POSECEF(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_POSLLH_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_POSLLH(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_STATUS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_STATUS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_DOP_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_DOP(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_ATT_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_ATT(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_SOL_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_SOL(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_NAVPVT_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_PVT(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_ODO_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_ODO(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_RESETODO_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_RESETODO(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_VELECEF_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_VELECEF(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_VELNED_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_VELNED(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMEGPS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMEGPS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMEUTC_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMEUTC(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_CLOCK_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_CLOCK(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMEGLO_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMEGLO(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMEBDS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMEBDS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMEGAL_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMEGAL(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_TIMELS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_TIMELS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_SVINFO_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_SVINFOH(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_DGPS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_DGPS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_SBAS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_SBAS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_ORB_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_ORB(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_NAVSAT_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_SAT(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_GEOFENCE_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_GEOFENCE(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_AOPSTATUS_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_AOPSTATUS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_NAV_EOE_ID_VALUE:
                        {
                            GPS_Nav_ParseChar_EOE(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        default:
                        {
                            GPS_AppCustomData.ParserStatus.ParseError++;
                            CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                                          "Received message with unexpected message ID (0x%.2x) for NAV class.", byte);
                            GPS_Parser_Reset();
                        }
                    }

                    break;
                }

                case GPS_PARSER_CLASS_RXM_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_RXM_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_INF_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_INF_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_ACK_VALUE:
                {
                    switch(GPS_AppCustomData.ParserStatus.MsgID)
                    {
                        case GPS_PARSER_ACK_NAK_ID_VALUE:
                        {
                            GPS_Ack_ParseChar_NAK(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }
                        
                        case GPS_PARSER_ACK_ACK_ID_VALUE:
                        {
                            GPS_Ack_ParseChar_ACK(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }
                    }
                    break;
                }

                case GPS_PARSER_CLASS_CFG_VALUE:
                {
                    /* */
                    OS_printf("GPS:  GPS_PARSER_CLASS_CFG_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_UPD_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_UPD_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_MON_VALUE:
                {
                    switch(GPS_AppCustomData.ParserStatus.MsgID)
                    {
                        case GPS_PARSER_MON_IO_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_IO(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_VER_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_VER(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_MSGPP_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_MSGPP(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_RXBUF_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_RXBUF(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_TXBUF_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_TXBUF(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_HW_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_HW(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_HW2_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_HW2(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_RXR_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_RXR(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_PATCH_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_PATCH(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_GNSS_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_GNSS(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }

                        case GPS_PARSER_MON_SMGR_ID_VALUE:
                        {
                            GPS_Mon_ParseChar_SMGR(byte, message);
                            GPS_AppCustomData.ParserStatus.PayloadCursor++;
                            break;
                        }
                    }

                    break;
                }

                case GPS_PARSER_CLASS_AID_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_AID_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_TIM_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_TIM_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_ESF_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_ESF_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_MGA_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_MGA_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_LOG_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_LOG_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_SEC_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_SEC_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                case GPS_PARSER_CLASS_HNR_VALUE:
                {
                    OS_printf("GPS:  GPS_PARSER_CLASS_HNR_VALUE not yet implemented.\n");
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                    break;
                }

                default:
                {
                    GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
                }
            }

            break;
        }

        case GPS_PARSE_STATE_GOT_PAYLOAD:
        {
            GPS_AppCustomData.ParserStatus.ChecksumA = byte;
            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_CHECKSUMA);
            break;
        }

        case GPS_PARSE_STATE_GOT_CHECKSUMA:
        {
            CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)message);
            msg_received = 1;
            *done = TRUE;
            GPS_Parser_StateChange(GPS_PARSE_STATE_IDLE);
            break;
        }

        default:
        {
        }
    }

    return msg_received;
}



CFE_SB_MsgId_t GPS_TranslateMsgID(uint16 ClassID, uint16 MsgID)
{
    CFE_SB_MsgId_t sbMsgID = 0;

    switch(ClassID)
    {
        case GPS_PARSER_CLASS_NAV_VALUE:
        {
            switch(MsgID)
            {
                //case GPS_PARSER_NAV_POSECEF_ID_VALUE:
                    //sbMsgID = GPS_NAV_POSECEF_MID;
                    //break;

                //case GPS_PARSER_NAV_POSLLH_ID_VALUE:
                    //sbMsgID = GPS_NAV_POSLLH_MID;
                    //break;

                //case GPS_PARSER_NAV_STATUS_ID_VALUE:
                    //sbMsgID = GPS_NAV_STATUS_MID;
                    //break;

                case GPS_PARSER_NAV_DOP_ID_VALUE:
                {
                    sbMsgID = GPS_NAV_DOP_MID;
                    break;
                }
                //case GPS_PARSER_NAV_ATT_ID_VALUE:
                    //sbMsgID = GPS_NAV_ATT_MID;
                    //break;

                //case GPS_PARSER_NAV_SOL_ID_VALUE:
                    //sbMsgID = GPS_NAV_SOL_MID;
                    //break;

                case GPS_PARSER_NAV_NAVPVT_ID_VALUE:
                {
                    sbMsgID = GPS_NAV_NAVPVT_MID;
                    break;
                }
                //case GPS_PARSER_NAV_ODO_ID_VALUE:
                    //sbMsgID = GPS_NAV_ODO_MID;
                    //break;

                //case GPS_PARSER_NAV_RESETODO_ID_VALUE:
                    //sbMsgID = GPS_NAV_RESETODO_MID;
                    //break;

                //case GPS_PARSER_NAV_VELECEF_ID_VALUE:
                    //sbMsgID = GPS_NAV_VELECEF_MID;
                    //break;

                //case GPS_PARSER_NAV_VELNED_ID_VALUE:
                    //sbMsgID = GPS_NAV_VELNED_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMEGPS_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMEGPS_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMEUTC_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMEUTC_MID;
                    //break;

                //case GPS_PARSER_NAV_CLOCK_ID_VALUE:
                    //sbMsgID = GPS_NAV_CLOCK_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMEGLO_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMEGLO_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMEBDS_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMEBDS_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMEGAL_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMEGAL_MID;
                    //break;

                //case GPS_PARSER_NAV_TIMELS_ID_VALUE:
                    //sbMsgID = GPS_NAV_TIMELS_MID;
                    //break;

                case GPS_PARSER_NAV_SVINFO_ID_VALUE:
                {
                    sbMsgID = GPS_NAV_SVINFO_MID;
                    break;
                }
                //case GPS_PARSER_NAV_DGPS_ID_VALUE:
                    //sbMsgID = GPS_NAV_DGPS_MID;
                    //break;

                //case GPS_PARSER_NAV_SBAS_ID_VALUE:
                    //sbMsgID = GPS_NAV_SBAS_MID;
                    //break;

                //case GPS_PARSER_NAV_ORB_ID_VALUE:
                    //sbMsgID = GPS_NAV_ORB_MID;
                    //break;

                //case GPS_PARSER_NAV_NAVSAT_ID_VALUE:
                    //sbMsgID = GPS_NAV_NAVSAT_MID;
                    //break;

                //case GPS_PARSER_NAV_GEOFENCE_ID_VALUE:
                    //sbMsgID = GPS_NAV_GEOFENCE_MID;
                    //break;

                //case GPS_PARSER_NAV_AOPSTATUS_ID_VALUE:
                    //sbMsgID = GPS_NAV_AOPSTATUS_MID;
                    //break;

                //case GPS_PARSER_NAV_EOE_ID_VALUE:
                    //sbMsgID = GPS_NAV_EOE_MID;
                    //break;

                default:
                {
                    break;
                }
            }
            break;
        }
        case GPS_PARSER_CLASS_RXM_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_INF_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_ACK_VALUE:
        {
            switch(MsgID)
            {
                case GPS_PARSER_ACK_NAK_ID_VALUE:
                {
                    sbMsgID = GPS_ACK_NAK_MID;
                    break;
                }
                case GPS_PARSER_ACK_ACK_ID_VALUE:
                {
                    sbMsgID = GPS_ACK_ACK_MID;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        case GPS_PARSER_CLASS_CFG_VALUE:
        {
            switch(MsgID)
            {
                case GPS_PARSER_CFG_PRT_ID_VALUE:
                {
                    sbMsgID = GPS_CFG_PRT_MID;
                    break;
                }

                case GPS_PARSER_CFG_MSG_ID_VALUE:
                {
                    sbMsgID = GPS_CFG_MSG_MID;
                    break;
                }
                //case GPS_PARSER_CFG_INF_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_RST_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_DAT_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                case GPS_PARSER_CFG_RATE_ID_VALUE:
                {
                    sbMsgID = GPS_CFG_RATE_MID;
                    break;
                }
                //case GPS_PARSER_CFG_CFG_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_RXM_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_ANT_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                case GPS_PARSER_CFG_SBAS_ID_VALUE:
                {
                    sbMsgID = GPS_CFG_SBAS_MID;
                    break;
                }
                //case GPS_PARSER_CFG_NMEA_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_USB_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_ODO_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_NAVX5_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                case GPS_PARSER_CFG_NAV5_ID_VALUE:
                {
                    sbMsgID = GPS_CFG_NAV5_MID;
                    break;
                }
                //case GPS_PARSER_CFG_TP5_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_RINV_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_ITFM_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_PM2_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_TMOD2_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_GNSS_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_LOGFILTER_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_PWR_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_TXSLOT_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_HNR_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_ESRC_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_DOSC_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_SMGR_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_GEOFENCE_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_FIXSEED_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_DYNSEED_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                //case GPS_PARSER_CFG_PMS_ID_VALUE:
                    //sbMsgID = ;
                    //break;

                default:
                {
                    break;
                }
            }
        }
        case GPS_PARSER_CLASS_UPD_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_MON_VALUE:
        {
            switch(MsgID)
            {
                //case GPS_PARSER_MON_IO_ID_VALUE:
                    //sbMsgID = GPS_MON_IO_MID;
                    //break;

                //case GPS_PARSER_MON_VER_ID_VALUE:
                    //sbMsgID = GPS_MON_VER_MID;
                    //break;

                //case GPS_PARSER_MON_MSGPP_ID_VALUE:
                    //sbMsgID = GPS_MON_MSGPP_MID;
                    //break;

                //case GPS_PARSER_MON_RXBUF_ID_VALUE:
                    //sbMsgID = GPS_MON_RXBUF_MID;
                    //break;

                //case GPS_PARSER_MON_TXBUF_ID_VALUE:
                    //sbMsgID = GPS_MON_TXBUF_MID;
                    //break;

                case GPS_PARSER_MON_HW_ID_VALUE:
                {
                    sbMsgID = GPS_MON_HW_MID;
                    break;
                }
                //case GPS_PARSER_MON_HW2_ID_VALUE:
                    //sbMsgID = GPS_MON_HW2_MID;
                    //break;

                //case GPS_PARSER_MON_RXR_ID_VALUE:
                    //sbMsgID = GPS_MON_RXR_MID;
                    //break;

                //case GPS_PARSER_MON_PATCH_ID_VALUE:
                    //sbMsgID = GPS_MON_PATCH_MID;
                    //break;

                //case GPS_PARSER_MON_GNSS_ID_VALUE:
                    //sbMsgID = GPS_MON_GNSS_MID;
                    //break;

                //case GPS_PARSER_MON_SMGR_ID_VALUE:
                    //sbMsgID = GPS_MON_SMGR_MID;
                    //break;

                default:
                {
                    break;
                }
            }
        }

        case GPS_PARSER_CLASS_AID_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_TIM_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_ESF_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_MGA_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_LOG_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_SEC_VALUE:
        /* Fall through */
        case GPS_PARSER_CLASS_HNR_VALUE:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return sbMsgID;
}

/************************/
/*  End of File Comment */
/************************/

