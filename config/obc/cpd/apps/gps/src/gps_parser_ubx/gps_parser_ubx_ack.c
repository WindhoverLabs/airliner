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
#include "gps_parser_ubx_ack.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/
#define GPS_PARSER_ACK_ACK_MSG_LENGTH    (2)
#define GPS_PARSER_ACK_NAK_MSG_LENGTH    (2)

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
void GPS_Ack_ParseChar_ACK(uint8 byte, GPS_DeviceMessage_t* message)
{
    uint16 checkMsg = 0;
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_ACK_ACK_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received ACK message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_ACK_ACK_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->clsID = byte;
                GPS_AppCustomData.AckRcvdMsgCls = byte;
                break;
            }
            case 1:
            {
                payload->msgID = byte;
                
                checkMsg = ((GPS_AppCustomData.AckRcvdMsgCls) | byte << 8);
                        
                if(GPS_ACK_WAITING == GPS_AppCustomData.AckState)
                {
                    if(checkMsg == GPS_AppCustomData.AckWaitingMsg)
                    {                
                        GPS_AppCustomData.AckState = GPS_ACK_GOT_ACK;
                        GPS_AppCustomData.AckWaitingRcvd = TRUE;
                    }
                }
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


void GPS_Ack_ParseChar_NAK(uint8 byte, GPS_DeviceMessage_t* message)
{
    uint16 checkMsg = 0;
    if(GPS_AppCustomData.ParserStatus.MsgLength != GPS_PARSER_ACK_NAK_MSG_LENGTH)
    {
        GPS_AppCustomData.ParserStatus.ParseError++;
        CFE_EVS_SendEvent(GPS_INIT_DEVICE_PARSER_ERR_EID, CFE_EVS_ERROR,
                      "Received ACK message with incorrect length (%u).", GPS_AppCustomData.ParserStatus.MsgLength);
        GPS_Parser_Reset();
    }
    else
    {
        GPS_ACK_NAK_t *payload = (void*)CFE_SB_GetUserData((CFE_SB_MsgPtr_t)message);
        switch(GPS_AppCustomData.ParserStatus.PayloadCursor)
        {
            case 0:
            {
                CFE_SB_MsgId_t sbMsgID = GPS_TranslateMsgID(GPS_AppCustomData.ParserStatus.ClassID, GPS_AppCustomData.ParserStatus.MsgID);
                uint16 sbHdrSize = CFE_SB_MsgHdrSize(sbMsgID);
                uint16 sbTotalMsgSize = sbHdrSize + GPS_AppCustomData.ParserStatus.MsgLength;
                CFE_SB_InitMsg(message, sbMsgID, sbTotalMsgSize, TRUE);
    
                payload->clsID = byte;
                GPS_AppCustomData.AckRcvdMsgCls = byte;
                break;
            }
            case 1:
            {
                payload->msgID = byte;
    
                checkMsg = ((GPS_AppCustomData.AckRcvdMsgCls) | byte << 8);
                if(GPS_ACK_WAITING == GPS_AppCustomData.AckState)
                {
                    if(checkMsg == GPS_AppCustomData.AckWaitingMsg)
                    {                
                        GPS_AppCustomData.AckState = GPS_ACK_GOT_NAK;
                        GPS_AppCustomData.AckWaitingRcvd = TRUE;
                    }
                }
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

