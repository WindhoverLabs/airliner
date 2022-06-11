/**
 * @file aspd4525_math.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the header file for aspd4525_math.c
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ASPD4525_MATH_H
#define ASPD4525_MATH_H

#include "cfe.h"
#include "aspd4525_tbldefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This is the maximum pressure count that the ASPD-4525 is capable of returning
 * 
 */
#define ASPD4525_MATH_PRESSURE_COUNTS_MAX        ((uint32) 0x3fff)

/**
 * @brief This is the maximum temperature count that the ASPD-4525 is capable of returning
 * 
 */
#define ASPD4525_MATH_TEMPERATURE_COUNTS_MAX     ((uint32) 0x07ff)


/**
 * @brief This macro can be used to convert Pressure in PSI to Pressure in Pascals
 * 
 */
#define ASPD4525_MATH_PSI2PASCALS(psi)           (((float) 6894.76)*(psi))

/**
 * @brief This macro can be used to convert Pressure in PSI to Pressure in Pascals
 * 
 */
#define ASPD4525_MATH_HECTOPASCALS2PSI(hPa)           (((float) 0.0145038)*(hPa))

/**
 * @brief This macro can be used to convert Pressure in atms to Pressure in Pascals
 * 
 */
#define ASPD4525_MATH_ATM2PASCALS(atm)           (((float) 101325.0)*(atm))

/**
 * @brief This macro can be used to calculate temperature in kelvin when celcius temperature is given
 * 
 */
#define ASPD4525_MATH_CELSIUS2KELVIN(celsius)    (((float) 273.15)+(celsius))


/**
 * @brief This macro can be used to calculate temperature in celcius when kelvin temperature is given
 * 
 */
#define ASPD4525_MATH_KELVIN2CELSIUS(kelvin)    ((kelvin) - 273.15)

/**
 * @brief This function returns the floating point differential pressure in Pascals
 * 
 * @param configTblP config table pointer
 * @param pressureDataCounts Pressaure diff counts
 * @return float Pressure Difference in Pascals
 */
float ASPD4525_MATH_GetDeltaPressure(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    uint32 pressureDataCounts
);

/**
 * @brief This function returns the floating point temperature in Celcius
 * 
 * @param configTblP config table pointer
 * @param temperatureDataCounts Temperature Counts
 * @return float Temperature in Celsius
 */
float ASPD4525_MATH_GetTemperature(
    ASPD4525_ConfigTblEntry_t* configTblP, 
    uint32 temperatureDataCounts
);

/**
 * @brief This function returns the fixed point temperature counts device outputs
 * 
 * @param configTblP config table pointer
 * @param fTemperature floating point temperature in Kelvin
 * @return uint16 Temperature Counts
 */
uint16 ASPD4525_MATH_GetTempDataCounts(
    ASPD4525_ConfigTblEntry_t configTblP, 
    float fTemperature
);

/**
 * @brief This function returns the fixed point pressure difference counts device outputs
 * 
 * @param configTblP config table pointer
 * @param fDiffPressure foating point preesure difference in hPa
 * @return uint16 Pressure Difference Counts
 */
uint16 ASPD4525_MATH_GetDiffPressureDataCounts(
    ASPD4525_ConfigTblEntry_t configTblP, 
    float fDiffPressure
);

#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_MATH_H */
