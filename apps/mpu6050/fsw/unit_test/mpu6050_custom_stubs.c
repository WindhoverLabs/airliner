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

#include "mpu6050_custom_stubs.h"

MPU6050_Custom_Returns_t MPU6050_Custom_Returns;


void MPU6050_Custom_InitData(void)
{
    
}


boolean MPU6050_Custom_Init(void)
{
    return TRUE;
}


boolean MPU6050_Custom_Uninit(void)
{
    return TRUE;
}


int32 MPU6050_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    return 0;
}


boolean MPU6050_SetAccScale(uint8 Scale, float *AccDivider)
{
    return TRUE;
}


boolean MPU6050_SetGyroScale(uint32 Scale, float *GyroDivider)
{
    return TRUE;
}


boolean MPU6050_Measure(int16 *GX, int16 *GY, int16 *GZ, int16 *AX, int16 *AY, int16 *AZ, int16 *Temp)
{
    *GX = MPU6050_Custom_Returns.MPU6050_Read_Gyro_X_Value;
    *GY = MPU6050_Custom_Returns.MPU6050_Read_Gyro_Y_Value;
    *GZ = MPU6050_Custom_Returns.MPU6050_Read_Gyro_Z_Value;
    *AX = MPU6050_Custom_Returns.MPU6050_Read_Accel_X_Value;
    *AY = MPU6050_Custom_Returns.MPU6050_Read_Accel_Y_Value;
    *AZ = MPU6050_Custom_Returns.MPU6050_Read_Accel_Z_Value;
    *Temp = MPU6050_Custom_Returns.MPU6050_Read_Temp_Value;
    /* TODO add single return. */
    return (MPU6050_Custom_Returns.MPU6050_Read_Gyro_Return &&
    MPU6050_Custom_Returns.MPU6050_Read_Accel_Return &&
    MPU6050_Custom_Returns.MPU6050_Read_Temp_Return);
}


boolean MPU6050_Read_WhoAmI(uint8 *Value)
{
    *Value = MPU6050_Custom_Returns.MPU6050_WhoAmI_Value;
    return MPU6050_Custom_Returns.MPU6050_WhoAmI_Return;
}


boolean MPU6050_Read_MagDeviceID(uint8 *Value)
{
    return TRUE;
}


uint64 PX4LIB_GetPX4TimeUs(void)
{
    return MPU6050_Custom_Returns.MPU6050_Get_Time_Return;
}


boolean MPU6050_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    return TRUE;
}


void MPU6050_Get_Rotation(uint8 *Rotation)
{
    return;
}
