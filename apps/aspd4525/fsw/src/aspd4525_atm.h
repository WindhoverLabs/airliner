/**
 * @file aspd4525_atm.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the header file to the aspd4525_atm.c file
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ASPD4525_ATM_H
#define ASPD4525_ATM_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "cfe.h"
#include "aspd4525_tbldefs.h"

/**
 * @brief This is the air density at sea level at 15 degrees celsius
 * 
 */
#define AIR_DENSITY_SEA_LEVEL         (1.225) /* kg/m^3 */

/**
 * @brief This function is designed to get the current airpressure from the system
 * 
 * @param configTblP config table pointer
 * @return float air pressure in pascals
 */
float ASPD4525_ATM_GetPressure(
    ASPD4525_ConfigTblEntry_t* configTblP
);

/**
 * @brief This function gets the air density given temperaure and pressure
 * 
 * @param configTblP config table pointer
 * @param temperatureCelsius temperature in celsius
 * @param pressurePascals pressure in pascals
 * @return float air density in kg/m^3
 */
float ASPD4525_ATM_GetAirDensity(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    float temperatureCelsius,
    float pressurePascals
);

/**
 * @brief This function returns the altitude in meters from the system
 * 
 * @param configTblP config table pointer
 * @return float altitude in meters
 */
float ASPD4525_ATM_GetAltitude(
    ASPD4525_ConfigTblEntry_t* configTblP
);

/**
 * @brief This function returns the airdensity given altitude alone
 * 
 * @param configTblP config table pointer
 * @param altitudeMeters altiutde in meters
 * @return float air density in kg/m^3
 */
float ASPD4525_ATM_GetAirDensityWithAlt(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters
);

/**
 * @brief This funciton returns the air density given altitude and temperature
 * 
 * @param configTblP config table pointer
 * @param altitudeMeters altiutde in meters
 * @param temperatureCelsius temperature in celsius
 * @return float air density in kg/m^3
 */
float ASPD4525_ATM_GetAirDensityWithAltTemp(
    ASPD4525_ConfigTblEntry_t* configTblP,
    float altitudeMeters,
    float temperatureCelsius
);

/**
 * @brief this function uses altitude to return column data, such as
 * column index, equation number, temperature, lapse rate and airdensity
 * 
 * https://windhoverlabs.atlassian.net/wiki/spaces/~626004280f5cf500697ed4c2/pages/2469429249/Calculating+Air+Speed+using+Pitot+Tube#Density-Formula-with-Height
 *  * 
 * @param configTblP config table pointer
 * @param altitudeMeters altiutde in meters
 * @param b_airColumnIndexP column index return pointer
 * @param equationNumber_bp equation number return pointer
 * @param Tbp column temperature return pointer
 * @param Lbp column lapse rate return pointer
 * @param rho_bp column air density return pointer
 * @param hbp column height return pointer
 */
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

/**
 * @brief This function returns airspeed in m/s given delta pressure in Pascals and Air Density in kg/m^3
 * 
 * @param deltaPressure (Pascals)
 * @param airDensity (kg/m^3)
 * @return float air speed in (m/s)
 */
float ASPD4525_ATM_GetAirSpeed(
    float deltaPressure,
    float airDensity
);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* ASPD4525_ATM_H */