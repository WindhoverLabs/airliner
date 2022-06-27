/**
 * @file aspd4525_atm.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the C file that handles atmospheric concerns for the airspeed app
 * @version 0.1
 * @date 2022-06-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "aspd4525_atm.h"
#include "aspd4525_math.h"
#include <math.h>

float ASPD4525_ATM_GetPressure(
    ASPD4525_ConfigTblEntry_t* configTblP
) {
    if (configTblP!=NULL) {
        /*Put code here*/
    }
    return ASPD4525_MATH_ATM2PASCALS(1);
}


float ASPD4525_ATM_GetAltitude(
    ASPD4525_ConfigTblEntry_t* configTblP
) {
    if (configTblP!=NULL) {
        /*Put code here*/
    }
//    return 71010;
//    return 51010;
//    return 47010;
//    return 32010;
//    return 20010;
//    return 11010;
//    return 10010;
    return 10.0; /* Altitude of 10 meters */
}

float ASPD4525_ATM_GetAirDensity(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    float temperatureCelsius,
    float pressurePascals
) {
    float rho = 0.0;
    if (configTblP!=NULL) {
        rho = pressurePascals / ((ASPD4525_MATH_CELSIUS2KELVIN(temperatureCelsius))*((configTblP->fAirGasConstantR_SI)/(configTblP->fAirMolarMass_SI)));
    }
    return rho;
}

float ASPD4525_ATM_GetAirDensityWithAlt(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters
) {
    float rho = 0.0;
    if (configTblP!= NULL) {
        uint8 b;
        uint8 equationNumber;
        float Tb, Lb, rho_b;
        float hb;
        float T;

        float g0 = configTblP->fGravitationalAccereleration_SI; //in m/s2
        float R = configTblP->fAirGasConstantR_SI; // in J/mol.K
        float M = configTblP->fAirMolarMass_SI; // kg/mol


        ASPD4525_ATM_findAirColumn(
            configTblP,
            altitudeMeters, 
            &b, 
            &equationNumber,
            &Tb,
            &Lb,
            &rho_b,
            &hb
        );

        T = Tb + (altitudeMeters - hb) * Lb;

        switch (equationNumber) {
            case 1:
                rho = rho_b * powf((Tb/T),(1+((g0*M)/(R*Lb))));
                break;
            case 2:
                rho = rho_b * expf(-(g0*M*(altitudeMeters-hb))/(R*Tb));
                break;
            default:
                /* Serial.println("Something wrong.  Bad Equation Number"); */
                break;
        }
    }
    return rho;
}

float ASPD4525_ATM_GetAirDensityWithAltTemp(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters,
    float temperatureCelsius
) {
    float rho = 0.0;
    if (configTblP!=NULL) {
        uint8 b;
        uint8 equationNumber;
        float Tb, Lb, rho_b;
        float hb;
        float T;

        float g0 = configTblP->fGravitationalAccereleration_SI; //in m/s2
        float R = configTblP->fAirGasConstantR_SI; // in J/mol.K
        float M = configTblP->fAirMolarMass_SI; // kg/mol


        ASPD4525_ATM_findAirColumn(
            configTblP,
            altitudeMeters, 
            &b, 
            &equationNumber,
            &Tb,
            &Lb,
            &rho_b,
            &hb
        );

        T = temperatureCelsius + 273.15;

        switch (equationNumber) {
            case 1:
                rho = rho_b * powf((Tb/T),(1+((g0*M)/(R*Lb))));
                break;
            case 2:
                rho = rho_b * expf(-(g0*M*(altitudeMeters-hb))/(R*Tb));
                break;
            default:
                /* Serial.println("Something wrong.  Bad Equation Number"); */
                break;
        }
    }
    return rho;
}

void ASPD4525_ATM_findAirColumn(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters,
    uint8* b_airColumnIndexP,
    uint8 *equationNumber_bp,
    float *Tbp, 
    float *Lbp, 
    float *rho_bp,
    float *hbp
) {
    if (configTblP!=NULL) {
        for (uint8 b=0; b<ASPD4525_NUM_ATM_LAYERS-1; b++) {
            if (altitudeMeters<configTblP->fAltitudeMeters_bs[b+1]) {
                *b_airColumnIndexP=b;
                *rho_bp = configTblP->fRho_bs[b];
                *Tbp = configTblP->fTemp_bs[b];
                *Lbp = configTblP->fLapseRate_bs[b];
                *hbp = configTblP->fAltitudeMeters_bs[b];
                *equationNumber_bp = configTblP->uEquationNo_bs[b];
                return;
            } else {
                if ((ASPD4525_NUM_ATM_LAYERS-2)==b) {
                    *b_airColumnIndexP=b+1;
                    *rho_bp = configTblP->fRho_bs[b+1];
                    *Tbp = configTblP->fTemp_bs[b+1];
                    *Lbp = configTblP->fLapseRate_bs[b+1];
                    *hbp = configTblP->fAltitudeMeters_bs[b+1];
                    *equationNumber_bp = configTblP->uEquationNo_bs[b+1];
                    return;
                }
            }
        }
    }
}

float ASPD4525_ATM_GetAirSpeed(
    float deltaPressure,
    float airDensity
) {
    float airSpeed = sqrtf(2*deltaPressure/airDensity);
    return airSpeed;
}
