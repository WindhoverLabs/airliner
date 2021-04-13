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


void MPU9250_Custom_InitData(void)
{

}


boolean MPU9250_Custom_Init()
{
    return TRUE;
}


boolean MPU9250_Custom_Uninit(void)
{
    return TRUE;
}


int32 MPU9250_ResetDevice(void)
{
    return TRUE;
}


boolean MPU9250_WriteReg(uint8 Addr, uint8 Data)
{
    return TRUE;
}


boolean MPU9250_ReadReg(uint8 Addr, uint8 *returnVal)
{
    return TRUE;
}


boolean MPU9250_SetAccScale(uint8 Scale, float *AccDivider)
{
    return TRUE;
}


boolean MPU9250_SetGyroScale(uint32 Scale, float *GyroDivider)
{
    return TRUE;
}


boolean MPU9250_Read_Gyro(int16 *X, int16 *Y, int16 *Z)
{
    return TRUE;
}


boolean MPU9250_Read_Accel(int16 *X, int16 *Y, int16 *Z)
{
    return TRUE;
};



boolean MPU9250_Read_Temp(uint16 *Temp)
{
    return TRUE;
}


boolean MPU9250_Read_WhoAmI(uint8 *Value)
{
    return TRUE;
}


boolean MPU9250_Perform_AccelSelfTest(void)
{
    return TRUE;
}


boolean MPU9250_Perform_GyroSelfTest(void)
{
    return TRUE;
}


/* TODO */
boolean MPU9250_Read_ImuStatus(boolean *WOM, boolean *FifoOvflw, boolean *Fsync, boolean *DataReady)
{
    return TRUE;
}


boolean MPU9250_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    return TRUE;
}


boolean MPU9250_Custom_Max_Events_Not_Reached(int32 ind)
{
    return TRUE;
}


int32 MPU9250_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    return 0;
}


void MPU9250_Get_Rotation(uint8 *Rotation)
{

}
