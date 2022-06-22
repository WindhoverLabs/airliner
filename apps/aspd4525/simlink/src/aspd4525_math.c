#include "cfe.h"
#include "aspd4525_tbldefs.h"
#include "aspd4525_math.h"
#include "aspd4525_map.h"
#include <math.h>

uint16 ASPD4525_MATH_GetTempDataCounts(
    ASPD4525_ConfigTblEntry_t *configTblP, 
    float fTemperature
) {
    uint16 uTemp = 0;
    if (configTblP!=NULL) {
        float tMinCelsius = configTblP->fTemperatureMinimum_Celcius; 
        float tMaxCelsius = configTblP->fTemperatureMaximum_Celcius;
        uTemp = 
            (uint16)
                (
                    (ASPD4525_MATH_KELVIN2CELSIUS(fTemperature)-tMinCelsius) * (ASPD4525_MATH_TEMPERATURE_COUNTS_MAX)
                    /(tMaxCelsius - tMinCelsius)
                );
    }
    return uTemp;
}

uint16 ASPD4525_MATH_GetDiffPressureDataCounts(
    ASPD4525_ConfigTblEntry_t *configTblP, 
    float fDiffPressure
) {
    uint16 uDiffPressureCnt = 0;
    if (configTblP!=NULL) {
        float pMinPSI = configTblP->fPressureMinimum_PSI;
        float pMaxPSI = configTblP->fPressureMaximum_PSI;
        float pPsi = ASPD4525_MATH_HECTOPASCALS2PSI(fabsf(fDiffPressure)) + ((pMinPSI+pMaxPSI)/2);
        
        uDiffPressureCnt = (uint16)(ASPD4525_MATH_PRESSURE_COUNTS_MAX*0.05) +
            (uint16)(((pPsi-pMinPSI)*(ASPD4525_MATH_PRESSURE_COUNTS_MAX*0.9))/(pMaxPSI-pMinPSI));
    }
    return uDiffPressureCnt;
}

uint32 ASPD4525_MATH_PackOutGoingData(
    uint16 uDiffPressureDataCounts,
    uint16 uTempDataCounts,
    uint8 status
) {
    uint32 returnData = 
        ((uint32)(status&(ASPD4525_STATUS_MASK>>ASPD4525_STATUS_SHIFT)))<<ASPD4525_STATUS_SHIFT |
        ((uint32)(uDiffPressureDataCounts&(ASPD4525_DIFF_PRESSURE_MASK>>ASPD4525_DIFF_PRESSURE_SHIFT)))<<ASPD4525_DIFF_PRESSURE_SHIFT |
        ((uint32)(uTempDataCounts&(ASPD4525_TEMPERATURE_MASK>>ASPD4525_TEMPERATURE_SHIFT)))<<ASPD4525_TEMPERATURE_SHIFT;
    return returnData;
}

uint32 ASPD4525_MATH_ByteFlip(
    uint32 data
) {
    uint32 returnData = ((data&0xff)<<24)|((data&0xff00)<<8)|((data&0xff0000)>>8)|((data&0xff000000)>>24);
    return returnData;
}