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
#define GPS_PARSER_MON_MSGPP_MSG_LENGTH     (120)
#define GPS_PARSER_MON_RXBUF_MSG_LENGTH     (24)
#define GPS_PARSER_MON_TXBUF_MSG_LENGTH     (28)
#define GPS_PARSER_MON_HW_MSG_LENGTH        (60)
#define GPS_PARSER_MON_HW2_MSG_LENGTH       (28)
#define GPS_PARSER_MON_RXR_MSG_LENGTH       (1)
#define GPS_PARSER_MON_GNSS_MSG_LENGTH      (8)
#define GPS_PARSER_MON_SMGR_MSG_LENGTH      (16)

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

/* Incoming messages. */
void GPS_Mon_ParseChar_IO(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_VER(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_MSGPP(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_RXBUF(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_TXBUF(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_HW(uint8 byte, GPS_DeviceMessage_t* message)
{
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_MON_HW_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received TIMEUTC message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_MON_HW_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);

                payload->pinSel = byte;
                break;
            }

            case 1:
            {
                payload->pinSel += byte << 8;
                break;
            }

            case 2:
            {
                payload->pinSel += byte << 16;
                break;
            }

            case 3:
            {
                payload->pinSel += byte << 24;
                break;
            }

            case 4:
            {
                payload->pinBank = byte;
                break;
            }

            case 5:
            {
                payload->pinBank += byte << 8;
                break;
            }

            case 6:
            {
                payload->pinBank += byte << 16;
                break;
            }

            case 7:
            {
                payload->pinBank += byte << 24;
                break;
            }

            case 8:
            {
                payload->pinDir = byte;
                break;
            }

            case 9:
            {
                payload->pinDir += byte << 8;
                break;
            }

            case 10:
            {
                payload->pinDir += byte << 16;
                break;
            }

            case 11:
            {
                payload->pinDir += byte << 24;
                break;
            }

            case 12:
            {
                payload->pinVal = byte;
                break;
            }

            case 13:
            {
                payload->pinVal += byte << 8;
                break;
            }

            case 14:
            {
                payload->pinVal += byte << 16;
                break;
            }

            case 15:
            {
                payload->pinVal += byte << 24;
                break;
            }

            case 16:
            {
                payload->noisePerMS = byte;
                break;
            }

            case 17:
            {
                payload->noisePerMS += byte << 8;
                break;
            }

            case 18:
            {
                payload->agcCnt = byte;
                break;
            }

            case 19:
            {
                payload->agcCnt += byte << 8;
                break;
            }

            case 20:
            {
                payload->aStatus = byte;
                break;
            }

            case 21:
            {
                payload->aPower = byte;
                break;
            }

            case 22:
            {
                payload->flags = byte;
                break;
            }

            case 23:
            {
                /* Reserved byte */
                break;
            }

            case 24:
            {
                payload->usedMask = byte;
                break;
            }

            case 25:
            {
                payload->usedMask += byte << 8;
                break;
            }

            case 26:
            {
                payload->usedMask += byte << 16;
                break;
            }

            case 27:
            {
                payload->usedMask += byte << 24;
                break;
            }

            /*  Fall Through.  These are the 17 elements of an array. */
            case 28:
            /* Fall Through */
            case 29:
            /* Fall Through */
            case 30:
            /* Fall Through */
            case 31:
            /* Fall Through */
            case 32:
            /* Fall Through */
            case 33:
            /* Fall Through */
            case 34:
            /* Fall Through */
            case 35:
            /* Fall Through */
            case 36:
            /* Fall Through */
            case 37:
            /* Fall Through */
            case 38:
            /* Fall Through */
            case 39:
            /* Fall Through */
            case 40:
            /* Fall Through */
            case 41:
            /* Fall Through */
            case 42:
            /* Fall Through */
            case 43:
            /* Fall Through */
            case 44:
            {
                payload->vp[GPS_AppCustomData.ParserStatus.PayloadCursor - 28] = byte;
                break;
            }

            case 45:
            {
                payload->jamInd = byte;
                break;
            }

            case 46:
            /* Fall Through */
            case 47:
            {
                /* Reserved bytes */
                break;
            }

            case 48:
            {
                payload->pinIrq = byte;
                break;
            }

            case 49:
            {
                payload->pinIrq += byte << 8;
                break;
            }

            case 50:
            {
                payload->pinIrq += byte << 16;
                break;
            }

            case 51:
            {
                payload->pinIrq += byte << 24;
                break;
            }

            case 52:
            {
                payload->pullH = byte;
                break;
            }

            case 53:
            {
                payload->pullH += byte << 8;
                break;
            }

            case 54:
            {
                payload->pullH += byte << 16;
                break;
            }

            case 55:
            {
                payload->pullH += byte << 24;
                break;
            }

            case 56:
            {
                payload->pullL = byte;
                break;
            }

            case 57:
            {
                payload->pullL += byte << 8;
                break;
            }

            case 58:
            {
                payload->pullL += byte << 16;
                break;
            }

            case 59:
            {
                payload->pullL += byte << 24;
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

void GPS_Mon_ParseChar_HW2(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_RXR(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_PATCH(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_GNSS(uint8 byte, GPS_DeviceMessage_t* message)
{

}

void GPS_Mon_ParseChar_SMGR(uint8 byte, GPS_DeviceMessage_t* message)
{

}


/************************/
/*  End of File Comment */
/************************/

