/**
 * @file aspd4525_custom.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is a API Header for Custom Implementations of communications with the Hardware / Simulation
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ASPD4525_CUSTOM_H
#define ASPD4525_CUSTOM_H

#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/** \brief Custom function to initialize custom device data structure. 
**
**  \par Description
**       This function is called on app startup, reload, restart etc
**       to initialize non-zero data.
**
*************************************************************************/
void ASPD4525_Custom_InitData(void);

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
boolean ASPD4525_Custom_Init(void);

/************************************************************************/
/** \brief Read magnetometer values from the ASPD4525.
**
**  \par Description
**       This function returns raw pressureDiff, temperature, and status values from the ASPD4525.
**
**  \param [out]    pressureDiffP    Raw pressureDiff value output address.
**
**  \param [out]    temperatureP    Raw temperature value output address.
**
**  \param [out]    statusP    Raw status value output address.
**
**  \returns
**       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean ASPD4525_Custom_Measure(uint16 *pressureDiffP, uint16 *temperatureP, uint8 *statusP);


#ifdef __cplusplus
}
#endif 

#endif /* ASPD4525_CUSTOM_H */
