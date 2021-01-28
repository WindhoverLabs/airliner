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
//#include "GPS_events.h"
//#include "GPS_msgids.h"
#include "gps_parser_ubx_common.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

#define GPS_PARSER_NAV_AOPSTATUS_MSG_LENGTH  (16)
#define GPS_PARSER_NAV_ATT_MSG_LENGTH        (32)
#define GPS_PARSER_NAV_CLOCK_MSG_LENGTH      (20)
#define GPS_PARSER_NAV_DGPS_MSG_LENGTH       (16)
#define GPS_PARSER_NAV_DOP_MSG_LENGTH        (18)
#define GPS_PARSER_NAV_EOE_MSG_LENGTH        (4)
#define GPS_PARSER_NAV_GEOFENCE_MSG_LENGTH   (8)
#define GPS_PARSER_NAV_ODO_MSG_LENGTH        (20)
#define GPS_PARSER_NAV_ORB_MSG_LENGTH        (8)
#define GPS_PARSER_NAV_POSECEF_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_POSLLH_MSG_LENGTH     (28)
#define GPS_PARSER_NAV_PVT_MSG_LENGTH        (92)
#define GPS_PARSER_NAV_SAT_MSG_LENGTH        (8)
#define GPS_PARSER_NAV_SBAS_MSG_LENGTH       (12)
#define GPS_PARSER_NAV_SOL_MSG_LENGTH        (52)
#define GPS_PARSER_NAV_STATUS_MSG_LENGTH     (16)
#define GPS_PARSER_NAV_SVINFOP1_MSG_LENGTH   (8)
#define GPS_PARSER_NAV_SVINFOP2_MSG_LENGTH   (12)
#define GPS_PARSER_NAV_TIMEBDS_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_TIMEGAL_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_TIMEGLO_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_TIMEGPS_MSG_LENGTH    (16)
#define GPS_PARSER_NAV_TIMELS_MSG_LENGTH     (24)
#define GPS_PARSER_NAV_TIMEUTC_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_VELECEF_MSG_LENGTH    (20)
#define GPS_PARSER_NAV_VELNED_MSG_LENGTH     (36)

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
void GPS_Nav_ParseChar_POSECEF(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_POSECEF_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received POSECEF message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_POSECEF_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->iTOW = byte;
                break;
            }
    
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->ecefX = byte;
                break;
            }
            case 5:
            {
                payload->ecefX += byte << 8;
                break;
            }
            case 6:
            {
                payload->ecefX += byte << 16;
                break;
            }
            case 7:
            {
                payload->ecefX += byte << 24;
                break;
            }
            case 8:
            {
                payload->ecefY = byte;
                break;
            }
            case 9:
            {
                payload->ecefY += byte << 8;
                break;
            }
            case 10:
            {
                payload->ecefY += byte << 16;
                break;
            }
            case 11:
            {
                payload->ecefY += byte << 24;
                break;
            }
            case 12:
            {
                payload->ecefZ = byte;
                break;
            }
            case 13:
            {
                payload->ecefZ += byte << 8;
                break;
            }
            case 14:
            {
                payload->ecefZ += byte << 16;
                break;
            }
            case 15:
            {
                payload->ecefZ += byte << 24;
                break;
            }
            case 16:
            {
                payload->pAcc = byte;
                break;
            }
            case 17:
            {
                payload->pAcc += byte << 8;
                break;
            }
            case 18:
            {
                payload->pAcc += byte << 16;
                break;
            }
            case 19:
            {
                payload->pAcc += byte << 24;
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

/* Incoming messages. */
void GPS_Nav_ParseChar_POSLLH(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_POSLLH_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received POSLLH message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_POSLLH_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->lon = byte;
                break;
            }
            case 5:
            {
                payload->lon += byte << 8;
                break;
            }
            case 6:
            {
                payload->lon += byte << 16;
                break;
            }
            case 7:
            {
                payload->lon += byte << 24;
                break;
            }
            case 8:
            {
                payload->lat = byte;
                break;
            }
            case 9:
            {
                payload->lat += byte << 8;
                break;
            }
            case 10:
            {
                payload->lat += byte << 16;
                break;
            }
            case 11:
            {
                payload->lat += byte << 24;
                break;
            }
            case 12:
            {
                payload->height = byte;
                break;
            }
            case 13:
            {
                payload->height += byte << 8;
                break;
            }
            case 14:
            {
                payload->height += byte << 16;
                break;
            }
            case 15:
            {
                payload->height += byte << 24;
                break;
            }
            case 16:
            {
                payload->hMSL = byte;
                break;
            }
            case 17:
            {
                payload->hMSL += byte << 8;
                break;
            }
            case 18:
            {
                payload->hMSL += byte << 16;
                break;
            }
            case 19:
            {
                payload->hMSL += byte << 24;
                break;
            }
            case 20:
            {
                payload->hAcc = byte;
                break;
            }
            case 21:
            {
                payload->hAcc += byte << 8;
                break;
            }
            case 22:
            {
                payload->hAcc += byte << 16;
                break;
            }
            case 23:
            {
                payload->hAcc += byte << 24;
                break;
            }
            case 24:
            {
                payload->vAcc = byte;
                break;
            }
            case 25:
            {
                payload->vAcc += byte << 8;
                break;
            }
            case 26:
            {
                payload->vAcc += byte << 16;
                break;
            }
            case 27:
            {
                payload->vAcc += byte << 24;
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


void GPS_Nav_ParseChar_STATUS(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_STATUS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received STATUS message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_STATUS_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->gpsFix = byte;
                break;
            }
            case 5:
            {
                payload->flags = byte;
                break;
            }
            case 6:
            {
                payload->fixStat = byte;
                break;
            }
            case 7:
            {
                payload->flags2 = byte;
                break;
            }
            case 8:
            {
                payload->ttff = byte;
                break;
            }
            case 9:
            {
                payload->ttff += byte << 8;
                break;
            }
            case 10:
            {
                payload->ttff += byte << 16;
                break;
            }
            case 11:
            {
                payload->ttff += byte << 24;
                break;
            }
            case 12:
            {
                payload->msss = byte;
                break;
            }
            case 13:
            {
                payload->msss += byte << 8;
                break;
            }
            case 14:
            {
                payload->msss += byte << 16;
                break;
            }
            case 15:
            {
                payload->msss += byte << 24;
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



void GPS_Nav_ParseChar_DOP(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_DOP_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received DOP message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_DOP_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
                }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->gDOP = byte;
                break;
            }
            case 5:
            {
                payload->gDOP += byte << 8;
                break;
            }
            case 6:
            {
                payload->pDOP = byte;
                break;
            }
            case 7:
            {
                payload->pDOP += byte << 8;
                break;
            }
            case 8:
            {
                payload->tDOP = byte;
                break;
            }
            case 9:
            {
                payload->tDOP += byte << 8;
                break;
            }
            case 10:
            {
                payload->vDOP = byte;
                break;
            }
            case 11:
            {
                payload->vDOP += byte << 8;
                break;
            }
            case 12:
            {
                payload->hDOP = byte;
                break;
            }
            case 13:
            {
                payload->hDOP += byte << 8;
                break;
            }
            case 14:
            {
                payload->nDOP = byte;
                break;
            }
            case 15:
            {
                payload->nDOP += byte << 8;
                break;
            }
            case 16:
            {
                payload->eDOP = byte;
                break;
            }
            case 17:
            {
                payload->eDOP += byte << 8;
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


void GPS_Nav_ParseChar_ATT(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_ATT_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received ATT message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_ATT_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->version = byte;
                break;
            }
            case 5:
            /* Fall Through */
            case 6:
            /* Fall Through */
            case 7:
            {
                break;
            }
            case 8:
            {
                payload->roll = byte;
                break;
            }
            case 9:
            {
                payload->roll += byte << 8;
                break;
            }
            case 10:
            {
                payload->roll += byte << 16;
                break;
            }
            case 11:
            {
                payload->roll += byte << 24;
                break;
            }
            case 12:
            {
                payload->pitch = byte;
                break;
            }
            case 13:
            {
                payload->pitch += byte << 8;
                break;
            }
            case 14:
            {
                payload->pitch += byte << 16;
                break;
            }
            case 15:
            {
                payload->pitch += byte << 24;
                break;
            }
            case 16:
            {
                payload->heading = byte;
                break;
            }
            case 17:
            {
                payload->heading += byte << 8;
                break;
            }
            case 18:
            {
                payload->heading += byte << 16;
                break;
            }
            case 19:
            {
                payload->heading += byte << 24;
                break;
            }
            case 20:
            {
                payload->accRoll = byte;
                break;
            }
            case 21:
            {
                payload->accRoll += byte << 8;
                break;
            }
            case 22:
            {
                payload->accRoll += byte << 16;
                break;
            }
            case 23:
            {
                payload->accRoll += byte << 24;
                break;
            }
            case 24:
            {
                payload->accPitch = byte;
                break;
            }
            case 25:
            {
                payload->accPitch += byte << 8;
                break;
            }
            case 26:
            {
                payload->accPitch += byte << 16;
                break;
            }
            case 27:
            {
                payload->accPitch += byte << 24;
                break;
            }
            case 28:
            {
                payload->accHeading = byte;
                break;
            }
            case 29:
            {
                payload->accHeading += byte << 8;
                break;
            }
            case 30:
            {
                payload->accHeading += byte << 16;
                break;
            }
            case 31:
            {
                payload->accHeading += byte << 24;
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



void GPS_Nav_ParseChar_SOL(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_SOL_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received SOL message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_SOL_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->fTOW = byte;
                break;
            }
            case 5:
            {
                payload->fTOW = byte << 8;
                break;
            }
            case 6:
            {
                payload->fTOW += byte << 16;
                break;
            }
            case 7:
            {
                payload->fTOW += byte << 24;
                break;
            }
            case 8:
            {
                payload->week = byte;
                break;
            }
            case 9:
            {
                payload->week += byte << 8;
                break;
            }
            case 10:
            {
                payload->gpsFix = byte;
                break;
            }
            case 11:
            {
                payload->flags = byte;
                break;
            }
            case 12:
            {
                payload->ecefX = byte;
                break;
            }
            case 13:
            {
                payload->ecefX += byte << 8;
                break;
            }
            case 14:
            {
                payload->ecefX += byte << 16;
                break;
            }
            case 15:
            {
                payload->ecefX += byte << 24;
                break;
            }
            case 16:
            {
                payload->ecefY = byte;
                break;
            }
            case 17:
            {
                payload->ecefY += byte << 8;
                break;
            }
            case 18:
            {
                payload->ecefY += byte << 16;
                break;
            }
            case 19:
            {
                payload->ecefY += byte << 24;
                break;
            }
            case 20:
            {
                payload->ecefZ = byte;
                break;
            }
            case 21:
            {
                payload->ecefZ += byte << 8;
                break;
            }
            case 22:
            {
                payload->ecefZ += byte << 16;
                break;
            }
            case 23:
            {
                payload->ecefZ += byte << 24;
                break;
            }
            case 24:
            {
                payload->pAcc = byte;
                break;
            }
            case 25:
            {
                payload->pAcc += byte << 8;
                break;
            }
            case 26:
            {
                payload->pAcc += byte << 16;
                break;
            }
            case 27:
            {
                payload->pAcc += byte << 24;
                break;
            }
            case 28:
            {
                payload->ecefVX = byte;
                break;
            }
            case 29:
            {
                payload->ecefVX += byte << 8;
                break;
            }
            case 30:
            {
                payload->ecefVX += byte << 16;
                break;
            }
            case 31:
            {
                payload->ecefVX += byte << 24;
                break;
            }
            case 32:
            {
                payload->ecefVY = byte;
                break;
            }
            case 33:
            {
                payload->ecefVY += byte << 8;
                break;
            }
            case 34:
            {
                payload->ecefVY += byte << 16;
                break;
            }
            case 35:
            {
                payload->ecefVY += byte << 24;
                break;
            }
            case 36:
            {
                payload->ecefVZ = byte;
                break;
            }
            case 37:
            {
                payload->ecefVZ += byte << 8;
                break;
            }
            case 38:
            {
                payload->ecefVZ += byte << 16;
                break;
            }
            case 39:
            {
                payload->ecefVZ += byte << 24;
                break;
            }
            case 40:
            {
                payload->sAcc = byte;
                break;
            }
            case 41:
            {
                payload->sAcc += byte << 8;
                break;
            }
            case 42:
            {
                payload->sAcc += byte << 16;
                break;
            }
            case 43:
            {
                payload->sAcc += byte << 24;
                break;
            }
            case 44:
            {
                payload->pDOP = byte;
                break;
            }
            case 45:
            {
                payload->pDOP += byte << 8;
                break;
            }
            case 46:
            {
                break;
            }
            case 47:
            {
                payload->numSV = byte;
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



void GPS_Nav_ParseChar_PVT(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_PVT_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received PVT message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_PVT_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->year = byte;
                break;
            }
            case 5:
            {
                payload->year = byte << 8;
                break;
            }
            case 6:
            {
                payload->month = byte;
                break;
            }
            case 7:
            {
                payload->day = byte;
                break;
            }
            case 8:
            {
                payload->hour = byte;
                break;
            }
            case 9:
            {
                payload->min = byte;
                break;
            }
            case 10:
            {
                payload->sec = byte;
                break;
            }
            case 11:
            {
                payload->valid = byte;
                break;
            }
            case 12:
            {
                payload->tAcc = byte;
                break;
            }
            case 13:
            {
                payload->tAcc += byte << 8;
                break;
            }
            case 14:
            {
                payload->tAcc += byte << 16;
                break;
            }
            case 15:
            {
                payload->tAcc += byte << 24;
                break;
            }
            case 16:
            {
                payload->nano = byte;
                break;
            }
            case 17:
            {
                payload->nano += byte << 8;
                break;
            }
            case 18:
            {
                payload->nano += byte << 16;
                break;
            }
            case 19:
            {
                payload->nano += byte << 24;
                break;
            }
            case 20:
            {
                payload->fixType = byte;
                break;
            }
            case 21:
            {
                payload->flags = byte;
                break;
            }
            case 22:
            {
                payload->flags2 = byte;
                break;
            }
            case 23:
            {
                payload->numSV = byte;
                break;
            }
            case 24:
            {
                payload->lon = byte;
                break;
            }
            case 25:
            {
                payload->lon += byte << 8;
                break;
            }
            case 26:
            {
                payload->lon += byte << 16;
                break;
            }
            case 27:
            {
                payload->lon += byte << 24;
                break;
            }
            case 28:
            {
                payload->lat = byte;
                break;
            }
            case 29:
            {
                payload->lat += byte << 8;
                break;
            }
            case 30:
            {
                payload->lat += byte << 16;
                break;
            }
            case 31:
            {
                payload->lat += byte << 24;
                break;
            }
            case 32:
            {
                payload->height = byte;
                break;
            }
            case 33:
            {
                payload->height += byte << 8;
                break;
            }
            case 34:
            {
                payload->height += byte << 16;
                break;
            }
            case 35:
            {
                payload->height += byte << 24;
                break;
            }
            case 36:
            {
                payload->hMSL = byte;
                break;
            }
            case 37:
            {
                payload->hMSL += byte << 8;
                break;
            }
            case 38:
            {
                payload->hMSL += byte << 16;
                break;
            }
            case 39:
            {
                payload->hMSL += byte << 24;
                break;
            }
            case 40:
            {
                payload->hAcc = byte;
                break;
            }
            case 41:
            {
                payload->hAcc += byte << 8;
                break;
            }
            case 42:
            {
                payload->hAcc += byte << 16;
                break;
            }
            case 43:
            {
                payload->hAcc += byte << 24;
                break;
            }
            case 44:
            {
                payload->vAcc = byte;
                break;
            }
            case 45:
            {
                payload->vAcc += byte << 8;
                break;
            }
            case 46:
            {
                payload->vAcc += byte << 16;
                break;
            }
            case 47:
            {
                payload->vAcc += byte << 24;
                break;
            }
            case 48:
            {
                payload->velN = byte;
                break;
            }
            case 49:
            {
                payload->velN += byte << 8;
                break;
            }
            case 50:
            {
                payload->velN += byte << 16;
                break;
            }
            case 51:
            {
                payload->velN += byte << 24;
                break;
            }
            case 52:
            {
                payload->velE = byte;
                break;
            }
            case 53:
            {
                payload->velE += byte << 8;
                break;
            }
            case 54:
            {
                payload->velE += byte << 16;
                break;
            }
            case 55:
            {
                payload->velE += byte << 24;
                break;
            }
            case 56:
            {
                payload->velD = byte;
                break;
            }
            case 57:
            {
                payload->velD += byte << 8;
                break;
            }
            case 58:
            {
                payload->velD += byte << 16;
                break;
            }
            case 59:
            {
                payload->velD += byte << 24;
                break;
            }
            case 60:
            {
                payload->gSpeed = byte;
                break;
            }
            case 61:
            {
                payload->gSpeed += byte << 8;
                break;
            }
            case 62:
            {
                payload->gSpeed += byte << 16;
                break;
            }
            case 63:
            {
                payload->gSpeed += byte << 24;
                break;
            }
            case 64:
            {
                payload->headMot = byte;
                break;
            }
            case 65:
            {
                payload->headMot += byte << 8;
                break;
            }
            case 66:
            {
                payload->headMot += byte << 16;
                break;
            }
            case 67:
            {
                payload->headMot += byte << 24;
                break;
            }
            case 68:
            {
                payload->sAcc = byte;
                break;
            }
            case 69:
            {
                payload->sAcc += byte << 8;
                break;
            }
            case 70:
            {
                payload->sAcc += byte << 16;
                break;
            }
            case 71:
            {
                payload->sAcc += byte << 24;
                break;
            }
            case 72:
            {
                payload->headAcc = byte;
                break;
            }
            case 73:
            {
                payload->headAcc += byte << 8;
                break;
            }
            case 74:
            {
                payload->headAcc += byte << 16;
                break;
            }
            case 75:
            {
                payload->headAcc += byte << 24;
                break;
            }
            case 76:
            {
                payload->pDOP = byte;
                break;
            }
            case 77:
            {
                payload->pDOP += byte << 8;
                break;
            }
            case 78:
            /* Fall Through */
            case 79:
            /* Fall Through */
            case 80:
            /* Fall Through */
            case 81:
            /* Fall Through */
            case 82:
            /* Fall Through */
            case 83:
            {
                break;
            }
            case 84:
            {
                payload->headVeh = byte;
                break;
            }
            case 85:
            {
                payload->headVeh += byte << 8;
                break;
            }
            case 86:
            {
                payload->headVeh += byte << 16;
                break;
            }
            case 87:
            {
                payload->headVeh += byte << 24;
                break;
            }
            case 88:
            {
                payload->magDec = byte;
                break;
            }
            case 89:
            {
                payload->magDec += byte << 8;
                break;
            }
            case 90:
            {
                payload->magAcc = byte;
                break;
            }
            case 91:
            {
                payload->magAcc += byte << 8;
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



void GPS_Nav_ParseChar_ODO(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_ODO_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received ODO message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_ODO_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
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
            /* Fall Through */
            case 2:
            /* Fall Through */
            case 3:
            {
                break;
            }
            case 4:
            {
                payload->iTOW = byte;
                break;
            }
            case 5:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 6:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 7:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 8:
            {
                payload->distance = byte;
                break;
            }
            case 9:
            {
                payload->distance += byte << 8;
                break;
            }
            case 10:
            {
                payload->distance += byte << 16;
                break;
            }
            case 11:
            {
                payload->distance += byte << 24;
                break;
            }
            case 12:
            {
                payload->totalDistance = byte;
                break;
            }
            case 13:
            {
                payload->totalDistance += byte << 8;
                break;
            }
            case 14:
            {
                payload->totalDistance += byte << 16;
                break;
            }
            case 15:
            {
                payload->totalDistance += byte << 24;
                break;
            }
            case 16:
            {
                payload->distanceStd = byte;
                break;
            }
            case 17:
            {
                payload->distanceStd += byte << 8;
                break;
            }
            case 18:
            {
                payload->distanceStd += byte << 16;
                break;
            }
            case 19:
            {
                payload->distanceStd += byte << 24;
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



void GPS_Nav_ParseChar_RESETODO(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_VELECEF(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_VELNED(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMEGPS(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMEUTC(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_TIMEUTC_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received TIMEUTC message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_TIMEUTC_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 1:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->tAcc = byte;
                break;
            }
            case 5:
            {
                payload->tAcc += byte << 8;
                break;
            }
            case 6:
            {
                payload->tAcc += byte << 16;
                break;
            }
            case 7:
            {
                payload->tAcc += byte << 24;
                break;
            }
            case 8:
            {
                payload->nano = byte;
                break;
            }
            case 9:
            {
                payload->nano += byte << 8;
                break;
            }
            case 10:
            {
                payload->nano += byte << 16;
                break;
            }
            case 11:
            {
                payload->nano += byte << 24;
                break;
            }
            case 12:
            {
                payload->year = byte;
                break;
            }
            case 13:
            {
                payload->year += byte << 8;
                break;
            }
            case 14:
            {
                payload->month = byte;
                break;
            }
            case 15:
            {
                payload->day = byte;
                break;
            }
            case 16:
            {
                payload->hour = byte;
                break;
            }
            case 17:
            {
                payload->min = byte;
                break;
            }
            case 18:
            {
                payload->sec = byte;
                break;
            }
            case 19:
            {
                payload->valid = byte;
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



void GPS_Nav_ParseChar_CLOCK(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMEGLO(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMEBDS(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMEGAL(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_TIMELS(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_SVINFOH(uint8 byte, GPS_DeviceMessage_t* message)
{
    static int i = 0;
    /* SVINFO is variable length 8 + 12*numCH so no length check here */
    GPS_NAV_SVINFO_Combined_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
    /* If we're in part 1 of the SVINFO message*/
    if(GPS_AppCustomData.ParserStatus.PayloadCursor < 8)
    {
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->svinfo.iTOW = byte;
                break;
            }
            case 1:
            {
                payload->svinfo.iTOW += byte << 8;
                break;
            }
            case 2:
            {
                payload->svinfo.iTOW += byte << 16;
                break;
            }
            case 3:
            {
                payload->svinfo.iTOW += byte << 24;
                break;
            }
            case 4:
            {
                payload->svinfo.numCh = byte;
                /* Length check */
                if (byte != (GPS_AppCustomData.ParserStatus.MsgLength - 8) / 12 &&
                    byte != 0)
                {
                    GPS_AppCustomData.ParserStatus.ParseError++;
                    CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                          "Received SVINFO message with incorrect length (%u), numCh (%u).", 
                          GPS_AppCustomData.ParserStatus.MsgLength, byte);
                    GPS_Parser_Reset();
                }
                break;
            }
            case 5:
            {
                payload->svinfo.globalFlags = byte;
                break;
            }
            case 6:
            {
                payload->svinfo.reserved2 = byte;
                break;
            }
            case 7:
            {
                payload->svinfo.reserved2 += byte << 8;
                /* If we don't have any part 2 messages */
                /* TODO verify this is possible */
                if (0 == (GPS_AppCustomData.ParserStatus.MsgLength - 8) / 12)
                {
                    GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
                }
                break;
            }
            default:
            {
                GPS_Parser_Reset();
                return;
            }
        }
    }
    /* If we're in part 2 of the SVINFO message */
    else if(GPS_AppCustomData.ParserStatus.PayloadCursor >= 8)
    {
        /* Save a max limim of part 2 SVINFO messages */
        if (i < (GPS_AppCustomData.ParserStatus.MsgLength - 8) / 12 &&
            i < PX4_SAT_INFO_MAX_SATELLITES)
        {
            switch((GPS_AppCustomData.ParserStatus.PayloadCursor - 8) % 12)
            {
                case 0:
                {
                    payload->numCh[i].chn       = byte;
                    break;
                }
                case 1:
                {
                    payload->numCh[i].svid      = byte;
                    break;
                }
                case 2:
                {
                    payload->numCh[i].flags     = byte;
                    break;
                }
                case 3:
                {
                    payload->numCh[i].quality   = byte;
                    break;
                }
                case 4:
                {
                    payload->numCh[i].cno       = byte;
                    break;
                }
                case 5:
                {
                    payload->numCh[i].elev      = byte;
                    break;
                }
                case 6:
                {
                    payload->numCh[i].azim      = byte;
                    break;
                }
                case 7:
                {
                    payload->numCh[i].azim      += byte << 8;
                    break;
                }
                case 8:
                {
                    payload->numCh[i].prRes     = byte;
                    break;
                }
                case 9:
                {
                    payload->numCh[i].prRes     += byte << 8;
                    break;
                }
                case 10:
                {
                    payload->numCh[i].prRes     += byte << 16;
                    break;
                }
                case 11:
                {
                    payload->numCh[i].prRes     += byte << 24;
                    i++;
                    break;
                }
                default:
                {
                    GPS_Parser_Reset();
                    return;
                }
            }
        }
        /* end of message */
        else if (i == (GPS_AppCustomData.ParserStatus.MsgLength - 8) / 12)
        {
            i = 0;
            GPS_Parser_StateChange(GPS_PARSE_STATE_GOT_PAYLOAD);
        }
        /* Max limit of part 2 SVINFO messages reached */
        else if (i >= PX4_SAT_INFO_MAX_SATELLITES)
        {
            /* Do nothing, continue parsing until end of message */
        }
        else
        {
            GPS_Parser_Reset();
        }
    }
}



void GPS_Nav_ParseChar_DGPS(uint8 byte, GPS_DeviceMessage_t* message)
{

}



void GPS_Nav_ParseChar_SBAS(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_SBAS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received SBAS message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_SBAS_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 5:
            {
                payload->geo = byte;
                break;
            }
            case 6:
            {
                payload->mode = byte;
                break;
            }
            case 7:
            {
                payload->sys = byte;
                break;
            }
            case 8:
            {
                payload->service = byte;
                break;
            }
            case 9:
            {
                payload->cnt = byte;
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



void GPS_Nav_ParseChar_ORB(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_ORB_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received ORB message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_ORB_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 5:
            {
                payload->version = byte;
                break;
            }
            case 6:
            {
                payload->numSv = byte;
                break;
            }
            case 7:
            /* Fall Through */
            case 8:
            {
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



void GPS_Nav_ParseChar_SAT(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_SAT_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received SAT message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_SAT_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 5:
            {
                payload->version = byte;
                break;
            }
            case 6:
            {
                payload->numSvs = byte;
                break;
            }
            case 7:
            /* Fall Through */
            case 8:
            {
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



void GPS_Nav_ParseChar_GEOFENCE(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_GEOFENCE_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received GEOFENCE message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_GEOFENCE_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 5:
            {
                payload->version = byte;
                break;
            }
            case 6:
            {
                payload->status = byte;
                break;
            }
            case 7:
            {
                payload->numFences = byte;
                break;
            }
            case 8:
            {
                payload->combState = byte;
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



void GPS_Nav_ParseChar_AOPSTATUS(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_AOPSTATUS_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received AOPSTATUS message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_AOPSTATUS_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
                break;
            }
            case 5:
            {
                payload->aopCfg = byte;
                break;
            }
            case 6:
            {
                payload->status;
                break;
            }
            case 7:
            /* Fall Through */
            case 8:
            /* Fall Through */
            case 9:
            /* Fall Through */
            case 10:
            /* Fall Through */
            case 11:
            /* Fall Through */
            case 12:
            /* Fall Through */
            case 13:
            /* Fall Through */
            case 14:
            /* Fall Through */
            case 15:
            /* Fall Through */
            case 16:
            {
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



void GPS_Nav_ParseChar_EOE(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_NAV_EOE_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received EOE message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_NAV_EOE_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->iTOW = byte;
                break;
            }
            case 2:
            {
                payload->iTOW += byte << 8;
                break;
            }
            case 3:
            {
                payload->iTOW += byte << 16;
                break;
            }
            case 4:
            {
                payload->iTOW += byte << 24;
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

