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

#include "ak8963_custom_stubs.h"

AK8963_Custom_Returns_t AK8963_Custom_Returns;


void AK8963_Custom_InitData(void)
{
    
}


boolean AK8963_Custom_Init(void)
{
    return TRUE;
}


boolean AK8963_Custom_Uninit(void)
{
    return TRUE;
}


int32 AK8963_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    return 0;
}


boolean AK8963_Read_Mag(int16 *X, int16 *Y, int16 *Z)
{
	if((0 != X) && (0 != Y) && (0 != Z))
	{
		*X = AK8963_Custom_Returns.AK8963_Read_Mag_X_Value;
		*Y = AK8963_Custom_Returns.AK8963_Read_Mag_Y_Value;
		*Z = AK8963_Custom_Returns.AK8963_Read_Mag_Z_Value;
	}

    return AK8963_Custom_Returns.AK8963_Read_Mag_Return;
}



boolean AK8963_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z)
{
    return TRUE;
}


boolean AK8963_Read_MagDeviceID(uint8 *Value)
{
	if(0 != Value)
	{
		*Value = AK8963_Custom_Returns.AK8963_WhoAmI_Value;
	}

    return AK8963_Custom_Returns.AK8963_WhoAmI_Return;
}


boolean AK8963_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
	return TRUE;
}


void AK8963_Get_Rotation(uint8 *Rotation)
{
    return;
}


boolean AK8963_ResetDevice(void)
{
    return TRUE;
}


boolean AK8963_PowerOn(void)
{
    return TRUE;
}


boolean AK8963_RunSelfTest(void)
{
    return TRUE;
}
