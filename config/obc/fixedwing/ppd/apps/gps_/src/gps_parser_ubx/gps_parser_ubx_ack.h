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
#ifndef _GPS_PARSER_UBX_ACK_H_
#define _GPS_PARSER_UBX_ACK_H_

#include "cfe.h"
#include "../gps_custom_shared.h"
//#include "GPS_events.h"
//#include "GPS_msgids.h"
#include "gps_parser_ubx_common.h"
#include "gps_ubx_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/


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


void GPS_Ack_ParseChar_ACK(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Ack_ParseChar_NAK(uint8 byte, GPS_DeviceMessage_t* message);


#ifdef __cplusplus
}
#endif 

#endif /* _GPS_PARSER_UBX_CFG_H_ */

/************************/
/*  End of File Comment */
/************************/

