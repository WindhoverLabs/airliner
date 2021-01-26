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

#ifndef MS5611_CUSTOM_H
#define MS5611_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

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
void MS5611_Custom_InitData(void);


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
osalbool MS5611_Custom_Init(void);


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
osalbool MS5611_Custom_Uninit(void);


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
int32 MS5611_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl);


/************************************************************************/
/** \brief Read the MS5611 Read-Only Memory.
**
**  \par Description
**       This function reads the MS5611 factory calibration coefficient
**       data from PROM.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be called after initialization and before
**       reading sensor data to get the specific factory calibration.
**
**  \param [in]   Addr    Which PROM register to read (0-7).
**
**  \param [in/out]   returnVal The PROM contents.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
osalbool MS5611_ReadPROM(uint8 Addr, uint16 *returnVal);


/************************************************************************/
/** \brief Start D1 Raw Pressure Conversion.
**
**  \par Description
**       This starts the raw pressure conversion internal to the MS5611.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be called after initialization.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
osalbool MS5611_D1Conversion(void);


/************************************************************************/
/** \brief Start D1 Raw Temperature Conversion.
**
**  \par Description
**       This starts the raw temperature conversion internal to the 
**       MS5611.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be called after initialization.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
osalbool MS5611_D2Conversion(void);


/************************************************************************/
/** \brief Reads the result of a D1, or D2 conversion.
**
**  \par Description
**       This reads the result of a raw pressure or temperature 
**       conversion from the MS5611.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be called after initialization.
**
**  \param [in/out]   returnVal The ADC result.
**
**  \returns    TRUE for success, FALSE for failure.
**
*************************************************************************/
osalbool MS5611_ReadADCResult(uint32 *returnVal);


/************************************************************************/
/** \brief Custom function to perform any critical cleanup after a 
**         failure.
**
**  \par Description
**       This function is called in cleanup to close and uninitialize
**       device resources.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
*************************************************************************/
void MS5611_Critical_Cleanup(void);


#ifdef __cplusplus
}
#endif 

#endif /* MS5611_CUSTOM_H */
