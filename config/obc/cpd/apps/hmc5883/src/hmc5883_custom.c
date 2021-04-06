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
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"


void HMC5883_Custom_InitData(void)
{

}



boolean HMC5883_Custom_Init(void)
{
    return TRUE;
}



boolean HMC5883_Custom_Uninit(void)
{
    return TRUE;
}



int32 HMC5883_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
	return 0;
}



boolean HMC5883_Custom_Measure(int16 *X, int16 *Y, int16 *Z)
{
    return TRUE;
}



boolean HMC5883_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    return TRUE;
}



boolean HMC5883_Apply_Platform_Rotation_Float(float *X, float *Y, float *Z)
{
    return TRUE;
}



boolean HMC5883_Custom_ValidateID(void)
{
    return TRUE;
}



boolean HMC5883_Custom_Set_Range(uint8 Range)
{
    return TRUE;
}



boolean HMC5883_Custom_Get_Range(uint8 *Range)
{
    return TRUE;
}



boolean HMC5883_Custom_Check_Range(uint8 Range)
{
    return TRUE;
}



boolean HMC5883_Custom_Check_Config(uint8 Config)
{
    return TRUE;
}



boolean HMC5883_Custom_Set_Config(uint8 Config)
{
    return TRUE;
}



boolean HMC5883_Custom_Get_Config(uint8 *Config)
{
    return TRUE;
}



boolean HMC5883_Custom_Measure_Temp(int16 *Temp)
{
    return TRUE;
}



void HMC5883_Get_Rotation(uint8 *Rotation)
{

}


