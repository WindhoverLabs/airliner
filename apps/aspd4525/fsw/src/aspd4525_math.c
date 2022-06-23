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
#include "aspd4525_atm.h"
#include "aspd4525_msg.h"
#include <math.h>

float ASPD4525_MATH_GetDeltaPressure(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    uint32 pressureDataCounts
) {
    float deltaPressurePascals = 0.0;
    if (configTblP!=NULL) {
        float pMinPSI = configTblP->fPressureMinimum_PSI; 
        float pMaxPSI = configTblP->fPressureMaximum_PSI;
        float m = (pMaxPSI-pMinPSI)/(((float)(ASPD4525_MATH_PRESSURE_COUNTS_MAX))*0.9);
        float b = pMinPSI - (m * (((float)(ASPD4525_MATH_PRESSURE_COUNTS_MAX))*0.05));
        float deltaPressurePSI = fabsf(m * pressureDataCounts + b);
        deltaPressurePascals = ASPD4525_MATH_PSI2PASCALS(deltaPressurePSI);
    }
    return (deltaPressurePascals);
}

void ASPD4525_MATH_CalibrateAirSpeedPressures(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    ASPD4525_LabCalibArgCmd_t* labCalibArgCmdPtr
) {
    if ((configTblP!=NULL)&&(labCalibArgCmdPtr!=NULL)) {
        float rho = AIR_DENSITY_SEA_LEVEL;
        uint32 CM = ASPD4525_MATH_PRESSURE_COUNTS_MAX;
        float CL = ((float)CM) * 0.05;
        float CS = ((float)CM) * 0.9;
        float C1 = (float)labCalibArgCmdPtr->uPCountHigh;
        float C0 = (float)labCalibArgCmdPtr->uPCountLow;

        float P1 = 0.5 * rho * (labCalibArgCmdPtr->fVelocityHigh_SI * labCalibArgCmdPtr->fVelocityHigh_SI);
        float P0 = 0.5 * rho * (labCalibArgCmdPtr->fVelocityLow_SI * labCalibArgCmdPtr->fVelocityLow_SI);

        float m = ( P1 - P0 ) / ( C1 - C0 );
        float b = - C0 * m;
        #if defined(STDIO_DEBUG)
        printf("Came Here 0: [%f, %f)\n", labCalibArgCmdPtr->fVelocityLow_SI, labCalibArgCmdPtr->fVelocityHigh_SI);

        printf("Came Here 1: [0x%04x, 0x%04x)\n", labCalibArgCmdPtr->uPCountLow, labCalibArgCmdPtr->uPCountHigh);
        printf("Came Here P: [%f, %f)\n", P0, P1);
        printf("Came Here C: [%f, %f)\n", CL, CS);
        #endif


        configTblP->fPressureMaximum_PSI = ( m * ( CL + CS ) + b ) / ASPD4525_MATH_PSI2PASCALS(1);
        configTblP->fPressureMinimum_PSI = ( m * ( CL      ) + b ) / ASPD4525_MATH_PSI2PASCALS(1);
    }
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