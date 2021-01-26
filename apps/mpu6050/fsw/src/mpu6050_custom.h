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

#ifndef MPU6050_CUSTOM_H
#define MPU6050_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/
#define MPU6050_MAX_FIFO_LENGTH              (74)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
typedef struct
{
    int16 GX; 
    int16 GY; 
    int16 GZ; 
    int16 AX; 
    int16 AY; 
    int16 AZ; 
    int16 Temp;
} MPU6050_Measurement_t;


typedef struct
{
    MPU6050_Measurement_t Samples[MPU6050_MAX_FIFO_LENGTH];
    uint32 SampleIntervalUs;
    int16 SampleCount;
} MPU6050_SampleQueue_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Custom function to initialize custom device data structure. 
**
**  \par Description
**       This function is called on app startup, reload, restart etc
**       to initialize non-zero data.
**
*************************************************************************/
void MPU6050_Custom_InitData(void);

/************************************************************************/
/** \brief Custom function to initialize custom device(s).
**
**  \par Description
**       This function is called at initialization and allows the
**       custom layer to provide specific functionality to initialize
**       internal objects.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean MPU6050_Custom_Init(void);


/************************************************************************/
/** \brief Custom function to uninitialize custom device(s).
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything. Also, called in cleanup to close and uninitialize
**       device resources.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean MPU6050_Custom_Uninit(void);

/************************************************************************/
/** \brief Custom function to initialize custom events. 
**
**  \par Description
**       This function is called in init event before CFE_EVS_Register
**       to add custom events to the event filter table.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**       The number of events written to the filter table and -1 for 
**       failure i.e. CFE_EVS_MAX_EVENT_FILTERS reached.
**
*************************************************************************/
int32 MPU6050_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl);

/************************************************************************/
/** \brief Set accel scale.
**
**  \par Description
**       This function sets the scale of the accelerometer.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [in]   Scale      The scale to set.
**
**  \param [out]   Divider   The divider for the scale set.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean MPU6050_SetAccScale(uint8 Scale, float *AccDivider);

/************************************************************************/
/** \brief Set gyro scale.
**
**  \par Description
**       This function sets the scale of the gyroscope.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [in]   Scale      The scale to set.
**
**  \param [out]   Divider   The divider for the scale set.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean MPU6050_SetGyroScale(uint32 Scale, float *GyroDivider);

/************************************************************************/
/** \brief Read raw gyro data.
**
**  \par Description
**       This function reads raw X-axis, Y-axis, and Z-axis gyro data.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called.
**
**  \param [in/out]   SampleQueue      Raw gyro, accel, and temp samples.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean MPU6050_Measure(MPU6050_SampleQueue_t *SampleQueue);


/************************************************************************/
/** \brief Get platform rotation.
**
**  \par Description
**       This function reads raw temperature data.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [out]   Rotation    The platform rotation.
**
*************************************************************************/
void MPU6050_Get_Rotation(uint8 *Rotation);


/************************************************************************/
/** \brief Apply the platform rotation to measurements.
**
**  \par Description
**       This applies any required platform rotation to the X, Y, and Z
**       measurements.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [in/out]   X      X-axis value.
**
**  \param [in/out]   Y      Y-axis value.
**
**  \param [in/out]   Z      Z-axis value.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean MPU6050_Apply_Platform_Rotation(float *X, float *Y, float *Z);


/************************************************************************/
/** \brief Returns the device ID of the MPU6050.
**
**  \par Description
**       This returns the device ID for identification.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [out]   Value      Device ID value.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean MPU6050_Read_WhoAmI(uint8 *Value);


#ifdef __cplusplus
}
#endif 

#endif /* MPU6050_CUSTOM_H */
