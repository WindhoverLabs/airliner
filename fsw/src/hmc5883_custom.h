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

#ifndef HMC5883_CUSTOM_H
#define HMC5883_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "hmc5883_msg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

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
void HMC5883_Custom_InitData(void);

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
boolean HMC5883_Custom_Init(void);

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
boolean HMC5883_Custom_Uninit(void);

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
**  \param [in]   ind       The current event table index.
**
**  \param [out]  EventTbl  The current event table to modify.
**
**  \returns
**       The number of events written to the filter table and -1 for 
**       failure i.e. CFE_EVS_MAX_EVENT_FILTERS reached.
**
*************************************************************************/
int32 HMC5883_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl);

/************************************************************************/
/** \brief Read magnetometer values from the HMC5883.
**
**  \par Description
**       This function returns raw X, Y, and Z values from the HMC5883.
**
**  \param [out]    X    Raw X value.
**
**  \param [out]    Y    Raw Y value.
**
**  \param [out]    Z    Raw Z value.
**
**  \returns
**       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Measure(int16 *X, int16 *Y, int16 *Z);

/************************************************************************/
/** \brief Apply any required rotation of the sensor x, y, and z axis.
**
**  \par Description
**       This function will apply rotation to match the orientation
**       of the sensor on the vehicle.
**
**  \param [out]    X    Raw X value.
**
**  \param [out]    Y    Raw Y value.
**
**  \param [out]    Z    Raw Z value.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Apply_Platform_Rotation(float *X, float *Y, float *Z);

/************************************************************************/
/** \brief Apply any required rotation of the sensor x, y, and z axis.
**
**  \par Description
**       This function will apply rotation to match the orientation
**       of the sensor on the vehicle.
**
**  \param [out]    X    Raw X value.
**
**  \param [out]    Y    Raw Y value.
**
**  \param [out]    Z    Raw Z value.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Apply_Platform_Rotation_Float(float *X, float *Y, float *Z);

/************************************************************************/
/** \brief Validate the device ID.
**
**  \par Description
**       This function will complete a "who am i" check of the device.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_ValidateID(void);

/************************************************************************/
/** \brief Set the sensor field range (gain).
**
**  \par Description
**       This function sets the configuration register B gain
**       gain configuration bits.
**
**  \param [in]    Range   The desired bit mask.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Set_Range(uint8 Range);

/************************************************************************/
/** \brief Get the sensor field range (gain).
**
**  \par Description
**       This function gets the current configuration register B gain
**       gain configuration bits.
**
**  \param [out]    Range   The output range.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Get_Range(uint8 *Range);

/************************************************************************/
/** \brief Check the value of the sensor field range (gain).
**
**  \par Description
**       This function checks and if the value isn't found sets the 
**       configuration register B gain gain configuration bits.
**
**  \param [in]    Range   The desired bit mask to check.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Check_Range(uint8 Range);

/************************************************************************/
/** \brief Set the sensor configuration.
**
**  \par Description
**       This function sets the configuration register A for data 
**       output rate and measurement configuration.
**
**  \param [in]    Range   The desired bit mask.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Check_Config(uint8 Config);

/************************************************************************/
/** \brief Check the value of the sensor configuration.
**
**  \par Description
**       This function checks and if the value isn't found sets the
**       configuration register A for data output rate and
**       measurement configuration.
**
**  \param [in]    Range   The desired bit mask to check.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Set_Config(uint8 Config);

/************************************************************************/
/** \brief Get the value of the sensor configuration.
**
**  \par Description
**       This function gets the current configuration.
**
**  \param [out]    Config   The configuration output.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Get_Config(uint8 *Config);

/************************************************************************/
/** \brief Read temperature values from a HMC5983.
**
**  \par Description
**       This function returns raw temperature from the HMC5883.
**
**  \param [out]    Temp    Raw temperature value.
**
**  \returns
**       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean HMC5883_Custom_Measure_Temp(int16 *Temp);

/************************************************************************/
/** \brief Get the platform rotation.
**
**  \par Description
**       This function returns the current platform rotation.
**
**  \param [out]    Rotation    The current rotation enum.
**
*************************************************************************/
void HMC5883_Get_Rotation(uint8 *Rotation);


#ifdef __cplusplus
}
#endif 

#endif /* HMC5883_CUSTOM_H */
