#ifndef ASPD4525_ATM_H
#define ASPD4525_ATM_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "cfe.h"
#include "aspd4525_tbldefs.h"

float ASPD4525_ATM_GetPressure(
    ASPD4525_ConfigTblEntry_t* configTblP
);

float ASPD4525_ATM_GetAirDensity(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    float temperatureCelsius,
    float pressurePascals
);

float ASPD4525_ATM_GetAltitude(
    ASPD4525_ConfigTblEntry_t* configTblP
);
      
float ASPD4525_ATM_GetAirDensityWithAlt(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters
);

float ASPD4525_ATM_GetAirDensityWithAltTemp(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters,
    float temperatureCelsius
);

void ASPD4525_ATM_findAirColumn(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters,
    uint8* b_airColumnIndexP,
    uint8 *equationNumber_bp,
    float *Tbp, 
    float *Lbp, 
    float *rho_bp,
    float *hbp
);

float ASPD4525_ATM_GetAirSpeed(
    float deltaPressure,
    float airDensity
);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* ASPD4525_ATM_H */