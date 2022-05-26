/**
 * @file mfa_aspd4525.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the header file for the ASPD-4525 Airspeed Sensor
 * @version 1.0.0
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_ASPD4525_H
#define MFA_ASPD4525_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"


#include "uah_app.h"

/**
 * @brief This macro signifies that the data received from the ASPD-4525 is valid
 * 
 */
#define MFA_ASPD4525_SUCCESS                    (0)

/**
 * @brief This macro signifies that the data from ASPD-4525 is not ready to be read yet
 * 
 */
#define MFA_ASPD4525_BUSY                       (1)

/**
 * @brief This macro signifies that the data from ASPD-4525 is stale, and a new data should be read for accuracy
 * 
 */
#define MFA_ASPD4525_STALE                      (2)

/**
 * @brief This macro signifies that something is not right in the ASPD-4525 or its setup
 * 
 */
#define MFA_ASPD4525_ERROR                      (-1)



/**
 * @brief This is the maximum pressure count that the ASPD-4525 is capable of returning
 * 
 */
#define MFA_ASPD4525_PRESSURE_COUNTS_MAX        ((uint32) 0x3fff)

/**
 * @brief This is the maximum temperature count that the ASPD-4525 is capable of returning
 * 
 */
#define MFA_ASPD4525_TEMPERATURE_COUNTS_MAX     ((uint32) 0x07ff)


/**
 * @brief This macro can be used to convert Pressure in PSI to Pressure in Pascals
 * 
 */
#define MFA_ASPD4525_PSI2PASCALS(psi)           (((double) 6894.76)*(psi))
/**
 * @brief This macro can be used to convert Pressure in atms to Pressure in Pascals
 * 
 */
#define MFA_ASPD4525_ATM2PASCALS(atm)           (((double) 101325.0)*(atm))

/**
 * @brief This macro can be used to calculate temperature in kelvin when celcius temperature is given
 * 
 */
#define MFA_ASPD4525_CELSIUS2KELVIN(celsius)    (((double) 273.15)+(celsius))


/**
 * @brief This function when called, sets up the ASPD-4525
 * 
 * @return int32 CFE_SUCCESS
 */
int32 MFA_ASPD4525_Setup ();


/**
 * @brief This function is to be called from a loop in the calling function.
 * 
 * It uses the calibration data in the configTblP, to gnerate telemetry data to be stored in tlmTabP
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @param tlmTabP Pointer to the Telemetry data structure where the results of this function are stored
 */
void MFA_ASPD4525_Loop (
    MFA_ConfigTbl_t* configTblP, 
    MFA_HkTlm_t* tlmTabP 
);

/**
 * @brief This function calculates the pressure differential between the tubes in the pitot tube in Pascals.
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @param pressureDataCounts This is the raw pressure reading from the ASPD-4525
 * @return double Differential Pressure between the tubes in the ASPD-4525
 */
double MFA_ASPD4525_GetDeltaPressure(
    MFA_ConfigTbl_t* configTblP, 
    uint32 pressureDataCounts
);

/**
 * @brief This gets the temperature in celsius from the data in the ASPD-4525
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @param temperatureDataCounts This is the raw temperature reading from the ASPD-4525
 * @return double Air Temperature in Celsius
 */
double MFA_ASPD4525_GetTemperature(
    MFA_ConfigTbl_t* configTblP, 
    uint32 temperatureDataCounts
);

/**
 * @brief This is the function to get Air Pressure (Not Differential Pressure)
 * 
 * Since ASPD-4525 only generates differential pressure, the source of this data has be from some other source
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @return double Pressure in Pascals
 */
double MFA_ASPD4525_GetPressure(
    MFA_ConfigTbl_t* configTblP
);

/**
 * @brief This is the function to get altitude information in meters
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @return double altitude in meters
 */
double MFA_ASPD4525_GetAltitude(
    MFA_ConfigTbl_t* configTblP
);

/**
 * @brief This function calculates the air density using a variation of the Ideal Gas Formula
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @param temperatureCelsius Temperature in Celsius
 * @param pressurePascals Pressure in Pascals
 * @return double Air Density in kg/m^3
 */
double MFA_ASPD4525_GetAirDensity(
    MFA_ConfigTbl_t* configTblP, 
    double temperatureCelsius,
    double pressurePascals
);

/**
 * @brief This function can be used when altitude is high enough to change air density and also temperature information is unavailable.
 * 
 * At some altitudes, the temperature is going to be outside the range of the thermistor on the pitot-tube.
 * Use this function to get Air Density in these occasions.
 * 
 * @param configTblP Pointer to the Config Table where Calibration Data is to be found
 * @param altitudeMeters Altitude in meters
 * @return double Air Density in kg/m^3
 */
double MFA_ASPD4525_GetAirDensityWithAlt(
    MFA_ConfigTbl_t* configTblP,
    double altitudeMeters
);


/**
 * @brief This function can be used when altitude is high enough to change air density, but temperature data is also available.
 * 
 * @param configTblP  Pointer to the Config Table where Calibration Data is to be found
 * @param altitudeMeters Altitude in meters
 * @param temperatureCelsius Temperature in Celsius
 * @return double Air Density in kg/m^3
 */
double MFA_ASPD4525_GetAirDensityWithAltTemp(
    MFA_ConfigTbl_t* configTblP,
    double altitudeMeters,
    double temperatureCelsius
);

/**
 * @brief This is a function that finds the air column index based on altitude and then sets the aircolumn constants
 * 
 * There is a table at the location in the url:
 * https://windhoverlabs.atlassian.net/wiki/spaces/~626004280f5cf500697ed4c2/pages/2469429249/Calculating+Air+Speed+using+Pitot+Tube#Air-Density-Formulae
 * This function uses this table to set the values in the referenced by the pointers in the argument of the function
 * 
 * @param altitudeMeters This is the input to the function
 * @param b_airColumnIndexP This is the first column in the table called "Column Subscript"
 * @param equationNumber_bp This represents the last column in the table called "Equation"
 * @param Tbp This represents the column in the table called "Standard Temperature"
 * @param Lbp This represents the column in the table called "Temperature Lapse Rate"
 * @param rho_bp This represents the column in the table called "Mass Density"
 * @param hbp This represents the column in the table called "Height from Sea Level"
 */
void MFA_ASPD4525_findAirColumn(
    MFA_ConfigTbl_t* configTblP,
    double altitudeMeters,
    uint8* b_airColumnIndexP,
    uint8 *equationNumber_bp,
    double *Tbp, 
    double *Lbp, 
    double *rho_bp,
    double *hbp
);

/**
 * @brief This function calculates the air speed given pressure difference in the pitot tubes, and air density
 * 
 * @param deltaPressure Pressure difference between the pitot tubes
 * @param airDensity density of the air
 * @return double air speed in m/s^2
 */
double MFA_ASPD4525_GetAirSpeed(
    double deltaPressure,
    double airDensity
);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MFA_ASPD4525_H */
