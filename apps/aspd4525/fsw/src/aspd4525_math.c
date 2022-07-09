/**
 * @file aspd4525_math.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the C File that does all the math computations for the ASPD4525 in software
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "aspd4525_tbldefs.h"
#include "aspd4525_math.h"
#include <math.h>

float ASPD4525_MATH_GetDeltaPressure(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    uint32 pressureDataCounts
) {
    float deltaPressurePascals = 0.0;
    if (configTblP!=NULL) {
        float pMinPSI = configTblP->fPressureMinimum_PSI; 
        float pMaxPSI = configTblP->fPressureMaximum_PSI;
        float pressurePSI = (
            (((float)(pressureDataCounts)-(float)(ASPD4525_MATH_PRESSURE_COUNTS_MAX*0.05))*(pMaxPSI-pMinPSI))
            /((float)(ASPD4525_MATH_PRESSURE_COUNTS_MAX*0.9)))
            +pMinPSI;

        float deltaPressurePSI = fabsf(pressurePSI-((pMinPSI+pMaxPSI)/2));


        deltaPressurePascals = ASPD4525_MATH_PSI2PASCALS(deltaPressurePSI);
    }
    return (deltaPressurePascals);
}

float ASPD4525_MATH_GetTemperature(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    uint32 temperatureDataCounts
) {
    float tempCelsius = 0.0;
    if (configTblP!=NULL) {
        float tMinCelsius = configTblP->fTemperatureMinimum_Celcius; 
        float tMaxCelsius = configTblP->fTemperatureMaximum_Celcius;
        tempCelsius = 
            (float)temperatureDataCounts * (tMaxCelsius - tMinCelsius)
            / ((float)(ASPD4525_MATH_TEMPERATURE_COUNTS_MAX))
            + tMinCelsius;
    }
    return (tempCelsius);
}