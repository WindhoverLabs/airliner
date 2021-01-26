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

#include "ms5611_custom_stubs.h"

void MS5611_Custom_InitData(void)
{
    return;
}


osalbool MS5611_Custom_Init(void)
{
    return TRUE;
}


osalbool MS5611_Custom_Uninit(void)
{
    return TRUE;
}


void MS5611_Critical_Cleanup(void)
{
    return;
}


osalbool MS5611_ReadPROM(uint8 Addr, uint16 *returnVal)
{
    /* Example values from MS5611 data sheet */
    switch(Addr)
    {
        case 0:
        {
            *returnVal = 40127;
            break;
        }
        case 1:
        {
            *returnVal = 36924;
            break;
        }
        case 2:
        {
            *returnVal = 23317;
            break;
        }
        case 3:
        {
            *returnVal = 23282;
            break;
        }
        case 4:
        {
            *returnVal = 33464;
            break;
        }
        case 5:
        {
            *returnVal = 28312;
            break;
        }
        case 6:
        {
            *returnVal = 9;
            break;
        }
        case 7:
        {
            *returnVal = 2053;
            break;
        }
    }
    return TRUE;
}


osalbool MS5611_D1Conversion(void)
{
    return TRUE;
}


osalbool MS5611_D2Conversion(void)
{
    return TRUE;
}


osalbool MS5611_ReadADCResult(uint32 *returnVal)
{
    return TRUE;
}


int32 MS5611_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    return 0;
}


uint64 PX4LIB_GetPX4TimeUs(void)
{
    return 0;
}
