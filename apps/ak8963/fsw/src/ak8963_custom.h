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

#ifndef AK8963_CUSTOM_H
#define AK8963_CUSTOM_H

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
void AK8963_Custom_InitData(void);

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
boolean AK8963_Custom_Init(void);


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
boolean AK8963_Custom_Uninit(void);

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
int32 AK8963_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl);


/************************************************************************/
/** \brief Read raw mag data.
**
**  \par Description
**       This function reads raw X-axis, Y-axis, and Z-axis mag data.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [out]   X      Raw X-axis value.
**
**  \param [out]   Y      Raw Y-axis value.
**
**  \param [out]   Z      Raw Z-axis value.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean AK8963_Read_Mag(int16 *X, int16 *Y, int16 *Z);



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
void AK8963_Get_Rotation(uint8 *Rotation);


/* TODO */
//boolean AK8963_Read_MagStatus(boolean *Overrun, boolean *DataReady, boolean *Overflow, boolean *Output16Bit);

/************************************************************************/
/** \brief Read the mag sensitivity adjustment values.
**
**  \par Description
**       Reads the sensitivity adjustment values of the mag (AK8963).
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \param [out]   X      X-axis value.
**
**  \param [out]   Y      Y-axis value.
**
**  \param [out]   Z      Z-axis value.
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean AK8963_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z);

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
boolean AK8963_Apply_Platform_Rotation(float *X, float *Y, float *Z);


/************************************************************************/
/** \brief Returns the device ID of the Mag (AK8963).
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
boolean AK8963_Read_MagDeviceID(uint8 *Value);

/************************************************************************/
/** \brief Soft reset the device.
**
**  \par Description
**       This function will soft reset the device and sleep until
**       the device is inialized again.
**
**  \par Assumptions, External Events, and Notes:
**       Initialization must be completed before this function is 
**       called. 
**
**  \returns TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean AK8963_ResetDevice(void);

boolean AK8963_PowerOn(void);

boolean AK8963_RunSelfTest(void);

#ifdef __cplusplus
}
#endif 

#endif /* AK8963_CUSTOM_H */
