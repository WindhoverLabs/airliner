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
/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"
#include "../gps_custom_shared.h"
//#include "GPS_events.h"
//#include "GPS_msgids.h"
#include "gps_parser_ubx_cfg.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/
#define GPS_PARSER_CFG_CFG_MSG_LENGTH1    (12)
#define GPS_PARSER_CFG_CFG_MSG_LENGTH2    (13)
#define GPS_PARSER_CFG_NAV5_MSG_LENGTH    (36)
#define GPS_PARSER_CFG_SBAS_MSG_LENGTH    (8)
#define GPS_PARSER_CFG_TMODE3_MSG_LENGTH  (40)
#define GPS_PARSER_CFG_PRT_MSG_LENGTH     (20)
#define GPS_PARSER_CFG_MSG_MSG_LENGTH1    (8)
#define GPS_PARSER_CFG_MSG_MSG_LENGTH2    (8)

/*************************************************************************
**
** Type definitions
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Imported data
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Exported data
**
**************************************************************************/



/*************************************************************************
**
** File data
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
** Local function prototypes
**************************************************************************/
void GPS_Cfg_ParseChar_CFG(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_CFG_MSG_LENGTH1 ||
       GPS_PARSER_CFG_CFG_MSG_LENGTH2)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received CFG message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_CFG_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->clearMask = byte;
                break;
            }
            case 1:
            {
                payload->clearMask += byte << 8;
                break;
            }
            case 2:
            {
                payload->clearMask += byte << 16;
                break;
            }
            case 3:
            {
                payload->clearMask += byte << 24;
                break;
            }
            case 4:
            {
                payload->saveMask = byte;
                break;
            }
            case 5:
            {
                payload->saveMask += byte << 8;
                break;
            }
            case 6:
            {
                payload->saveMask += byte << 16;
                break;
            }
            case 7:
            {
                payload->saveMask += byte << 24;
                break;
            }
            case 8:
            {
                payload->loadMask = byte;
                break;
            }
            case 9:
            {
                payload->loadMask += byte << 8;
                break;
            }
            case 10:
            {
                payload->loadMask += byte << 16;
                break;
            }
            case 11:
            {
                payload->loadMask += byte << 24;
                if(GPS_AppCustomData.ParserStatus.MsgLength == GPS_PARSER_CFG_CFG_MSG_LENGTH1)
                {
                    GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                }
                break;
            }
            case 12:
            {
                payload->deviceMask = byte;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}


void GPS_Cfg_ParseChar_NAV5(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_NAV5_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received NAV5 message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_NAV5_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->mask = byte;
                break;
            }
            case 1:
            {
                payload->mask += byte << 8;
                break;
            }
            case 2:
            {
                payload->dynModel = byte;
                break;
            }
            case 3:
            {
                payload->fixMode = byte;
                break;
            }
            case 4:
            {
                payload->fixedAlt = byte;
                break;
            }
            case 5:
            {
                payload->fixedAlt += byte << 8;
                break;
            }
            case 6:
            {
                payload->fixedAlt += byte << 16;
                break;
            }
            case 7:
            {
                payload->fixedAlt += byte << 24;
                break;
            }
            case 8:
            {
                payload->fixedAltVar = byte;
                break;
            }
            case 9:
            {
                payload->fixedAltVar += byte << 8;
                break;
            }
            case 10:
            {
                payload->fixedAltVar += byte << 16;
                break;
            }
            case 11:
            {
                payload->fixedAltVar += byte << 24;
                break;
            }
            case 12:
            {
                payload->minElev = byte;
                break;
            }
            case 13:
            {
                payload->drLimit = byte;
                break;
            }
            case 14:
            {
                payload->pDop = byte;
                break;
            }
            case 15:
            {
                payload->pDop += byte << 8;
                break;
            }
            case 16:
            {
                payload->tDop = byte;
                break;
            }
            case 17:
            {
                payload->tDop += byte << 8;
                break;
            }
            case 18:
            {
                payload->pAcc = byte;
                break;
            }
            case 19:
            {
                payload->pAcc += byte << 8;
                break;
            }
            case 20:
            {
                payload->tAcc = byte;
                break;
            }
            case 21:
            {
                payload->tAcc += byte << 8;
                break;
            }
            case 22:
            {
                payload->staticHoldThresh = byte;
                break;
            }
            case 23:
            {
                payload->dgnssTimeout = byte;
                break;
            }
            case 24:
            {
                payload->cnoThreshNumSVs = byte;
                break;
            }
            case 25:
            {
                payload->cnoThresh = byte;
                break;
            }
            case 26:
            {
                payload->reserved1 = byte;
                break;
            }
            case 27:
            {
                payload->reserved1 += byte << 8;
                break;
            }
            case 28:
            {
                payload->staticHoldMaxDist = byte;
                break;
            }
            case 29:
            {
                payload->staticHoldMaxDist += byte << 8;
                break;
            }
            case 30:
            {
                payload->utcStandard = byte;
                break;
            }
            case 31:
            {
                payload->reserved2 = byte;
                break;
            }
            case 32:
            {
                payload->reserved3 = byte;
                break;
            }
            case 33:
            {
                payload->reserved3 += byte << 8;
                break;
            }
            case 34:
            {
                payload->reserved3 += byte << 16;
                break;
            }
            case 35:
            {
                payload->reserved3 += byte << 24;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}


void GPS_Cfg_ParseChar_SBAS(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_SBAS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received NAV5 message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_SBAS_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->mode = byte;
                break;
            }
            case 1:
            {
                payload->usage = byte;
                break;
            }
            case 2:
            {
                payload->maxSBAS = byte;
                break;
            }
            case 3:
            {
                payload->scanmode2 = byte;
                break;
            }
            case 4:
            {
                payload->scanmode1 = byte;
                break;
            }
            case 5:
            {
                payload->scanmode1 += byte << 8;
                break;
            }
            case 6:
            {
                payload->scanmode1 += byte << 16;
                break;
            }
            case 7:
            {
                payload->scanmode1 += byte << 24;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}


void GPS_Cfg_ParseChar_TMODE3(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_SBAS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received NAV5 message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_TMODE3_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->version = byte;
                break;
            }
            case 1:
            {
                payload->reserved1 = byte;
                break;
            }
            case 2:
            {
                payload->flags = byte;
                break;
            }
            case 3:
            {
                payload->flags = byte << 8;
                break;
            }
            case 4:
            {
                payload->ecefXOrLat = byte;
                break;
            }
            case 5:
            {
                payload->ecefXOrLat += byte << 8;
                break;
            }
            case 6:
            {
                payload->ecefXOrLat += byte << 16;
                break;
            }
            case 7:
            {
                payload->ecefXOrLat += byte << 24;
                break;
            }
            case 8:
            {
                payload->ecefYOrLon = byte;
                break;
            }
            case 9:
            {
                payload->ecefYOrLon += byte << 8;
                break;
            }
            case 10:
            {
                payload->ecefYOrLon += byte << 16;
                break;
            }
            case 11:
            {
                payload->ecefYOrLon += byte << 24;
                break;
            }
            case 12:
            {
                payload->ecefZOrAlt = byte;
                break;
            }
            case 13:
            {
                payload->ecefZOrAlt += byte << 8;
                break;
            }
            case 14:
            {
                payload->ecefZOrAlt += byte << 16;
                break;
            }
            case 15:
            {
                payload->ecefZOrAlt += byte << 24;
                break;
            }
            case 16:
            {
                payload->ecefXOrLatHP = byte;
                break;
            }
            case 17:
            {
                payload->ecefYOrLonHP = byte;
                break;
            }
            case 18:
            {
                payload->ecefZOrAltHP = byte;
                break;
            }
            case 19:
            {
                payload->reserved2 = byte;
                break;
            }
            case 20:
            {
                payload->fixedPosAcc = byte;
                break;
            }
            case 21:
            {
                payload->fixedPosAcc += byte << 8;
                break;
            }
            case 22:
            {
                payload->fixedPosAcc += byte << 16;
                break;
            }
            case 23:
            {
                payload->fixedPosAcc += byte << 24;
                break;
            }
            case 24:
            {
                payload->svinMinDur = byte;
                break;
            }
            case 25:
            {
                payload->svinMinDur += byte << 8;
                break;
            }
            case 26:
            {
                payload->svinMinDur += byte << 16;
                break;
            }
            case 27:
            {
                payload->svinMinDur += byte << 24;
                break;
            }
            case 28:
            {
                payload->svinAccLimit = byte;
                break;
            }
            case 29:
            {
                payload->svinAccLimit += byte << 8;
                break;
            }
            case 30:
            {
                payload->svinAccLimit += byte << 16;
                break;
            }
            case 31:
            {
                payload->svinAccLimit += byte << 24;
                break;
            }
            case 32:
            {
                payload->reserved3[0] = byte;
                break;
            }
            case 33:
            {
                payload->reserved3[1] = byte;
                break;
            }
            case 34:
            {
                payload->reserved3[2] = byte;
                break;
            }
            case 35:
            {
                payload->reserved3[3] = byte;
                break;
            }
            case 36:
            {
                payload->reserved3[4] = byte;
                break;
            }
            case 37:
            {
                payload->reserved3[5] = byte;
                break;
            }
            case 38:
            {
                payload->reserved3[6] = byte;
                break;
            }
            case 39:
            {
                payload->reserved3[7] = byte;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}


void GPS_Cfg_ParseChar_PRT(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_SBAS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received NAV5 message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_PRT_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->portID = byte;
                break;
            }
            case 1:
            {
                payload->reserved1 = byte;
                break;
            }
            case 2:
            {
                payload->txReady = byte;
                break;
            }
            case 3:
            {
                payload->txReady = byte << 8;
                break;
            }
            case 4:
            {
                payload->mode = byte;
                break;
            }
            case 5:
            {
                payload->mode += byte << 8;
                break;
            }
            case 6:
            {
                payload->mode += byte << 16;
                break;
            }
            case 7:
            {
                payload->mode += byte << 24;
                break;
            }
            case 8:
            {
                payload->baudRate = byte;
                break;
            }
            case 9:
            {
                payload->baudRate += byte << 8;
                break;
            }
            case 10:
            {
                payload->baudRate += byte << 16;
                break;
            }
            case 11:
            {
                payload->baudRate += byte << 24;
                break;
            }
            case 12:
            {
                payload->inProtoMask = byte;
                break;
            }
            case 13:
            {
                payload->inProtoMask += byte << 8;
                break;
            }
            case 14:
            {
                payload->outProtoMask = byte;
                break;
            }
            case 15:
            {
                payload->outProtoMask += byte << 8;
                break;
            }
            case 16:
            {
                payload->flags = byte;
                break;
            }
            case 17:
            {
                payload->flags += byte << 8;
                break;
            }
            case 18:
            {
                payload->reserved2 = byte;
                break;
            }
            case 19:
            {
                payload->reserved2 += byte << 8;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}


void GPS_Cfg_ParseChar_MSG(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_CFG_SBAS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received NAV5 message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_CFG_MSG_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->msgClass = byte;
                break;
            }
            case 1:
            {
                payload->msgID = byte;
                break;
            }
            case 2:
            {
                payload->rate[0] = byte;
                break;
            }
            case 3:
            {
                payload->rate[1] = byte;
                break;
            }
            case 4:
            {
                payload->rate[2] = byte;
                break;
            }
            case 5:
            {
                payload->rate[3] = byte;
                break;
            }
            case 6:
            {
                payload->rate[4] = byte;
                break;
            }
            case 7:
            {
                payload->rate[5] = byte;
                GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
}

/************************/
/*  End of File Comment */
/************************/

