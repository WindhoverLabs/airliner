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
#ifndef _GPS_PARSER_UBX_NAV_H_
#define _GPS_PARSER_UBX_NAV_H_

#include "cfe.h"
#include "../gps_custom_shared.h"
//#include "GPS_events.h"
//#include "GPS_msgids.h"

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

void GPS_Nav_ParseChar_POSECEF(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_POSLLH(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_STATUS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_DOP(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_ATT(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_SOL(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_PVT(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_ODO(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_RESETODO(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_VELECEF(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_VELNED(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMEGPS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMEUTC(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_CLOCK(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMEGLO(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMEBDS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMEGAL(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_TIMELS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_SVINFOH(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_DGPS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_SBAS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_ORB(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_SAT(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_GEOFENCE(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_AOPSTATUS(uint8 byte, GPS_DeviceMessage_t* message);
void GPS_Nav_ParseChar_EOE(uint8 byte, GPS_DeviceMessage_t* message);

//int32 GPS_Nav_EnablePOSLLH(void);

#ifdef __cplusplus
}
#endif 

#endif /* _GPS_PARSER_UBX_NAV_H_*/

/************************/
/*  End of File Comment */
/************************/

