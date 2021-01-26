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

#ifndef MPU6050_CUSTOM_STUBS_H
#define MPU6050_CUSTOM_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"


typedef struct
{
    boolean    MPU6050_WhoAmI_Return;
    int8       MPU6050_WhoAmI_Value;
    boolean    MPU6050_Read_Gyro_Return;
    int16      MPU6050_Read_Gyro_X_Value;
    int16      MPU6050_Read_Gyro_Y_Value;
    int16      MPU6050_Read_Gyro_Z_Value;
    int16      MPU6050_Read_Accel_X_Value;
    int16      MPU6050_Read_Accel_Y_Value;
    int16      MPU6050_Read_Accel_Z_Value;
    boolean    MPU6050_Read_Accel_Return;
    boolean    MPU6050_Read_Temp_Return;
    uint16     MPU6050_Read_Temp_Value;
    uint64     MPU6050_Get_Time_Return;
} MPU6050_Custom_Returns_t;


extern MPU6050_Custom_Returns_t MPU6050_Custom_Returns;


#ifdef __cplusplus
}
#endif

#endif /* MPU6050_CUSTOM_STUBS_H */
