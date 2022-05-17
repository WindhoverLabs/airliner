/**
 * @file mfa_aspd4525.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This file handles the ASPD-4525 and calculates the air speed
 * @version 1.0.0
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "cfe.h"
#include "mfa_aspd4525.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"

#include "uah_app.h"

/**
 * @brief This macro returns the absolute value of a double number
 * 
 */
#define absolute(number)         ((number>0)?(number):-(number))

/**
 * @brief This function calculates the Square Root of a double using the Newton-Raphson Method
 * 
 * @param number This is the number whose sqrt is to be calculated, make sure it is positive.  The function will not check for positivity.
 * @return double returns the square root of the number
 */
double sqrt(double number);

int32 MFA_ASPD4525_Setup () {
    int32 status = CFE_SUCCESS;
    UAH_AppSetup();

    return status;
}

void MFA_ASPD4525_Loop (
    MFA_ConfigTbl_t* configTblP,
    MFA_HkTlm_t* tlmTabP 
) {
    UAH_AppLoop(
        (unsigned int*) &tlmTabP->pressureCount, 
        (unsigned int*) &tlmTabP->temperatureCount,
        (unsigned int*) &tlmTabP->status);
    
    if (MFA_ASPD4525_SUCCESS == tlmTabP->status) {
        double deltaPressurePascals = 
            MFA_ASPD4525_GetDeltaPressure(
                configTblP,
                tlmTabP->pressureCount
            );
        
        double temperatureCelsius = 
            MFA_ASPD4525_GetTemperature(
                configTblP,
                tlmTabP->temperatureCount
            );

        double pressurePascals = 
            MFA_ASPD4525_GetPressure(
                configTblP
            );
        
        double airDensity = 
            MFA_ASPD4525_GetAirDensity(
                configTblP,
                temperatureCelsius,
                pressurePascals
            );

        tlmTabP->Temperature = temperatureCelsius;
        tlmTabP->AirSpeed = 
            MFA_ASPD4525_GetAirSpeed(
                deltaPressurePascals,
                airDensity
            );
    }
}

int32 aspd_4525_status(uint32 status)
{
  int32 success = CFE_SUCCESS;
  switch(status)
  {
    case 0:
      success = MFA_ASPD4525_SUCCESS;
      break;
    case 1:
      success = MFA_ASPD4525_BUSY;
      break;
    case 2:
      success = MFA_ASPD4525_STALE;
      break;
    default:
      success = MFA_ASPD4525_ERROR;
      break;
  }
  return success;
}


double MFA_ASPD4525_GetDeltaPressure(
    MFA_ConfigTbl_t* configTblP, 
    uint32 pressureDataCounts
) {
    double pMinPSI = configTblP->pressureMinimum_PSI; 
    double pMaxPSI = configTblP->pressureMaximum_PSI;
    double pressurePSI = (
        (((double)(pressureDataCounts)-(double)(MFA_ASPD4525_PRESSURE_COUNTS_MAX*0.05))*(pMaxPSI-pMinPSI))
        /((double)(MFA_ASPD4525_PRESSURE_COUNTS_MAX*0.9)))
        +pMinPSI;

    double deltaPressurePSI = absolute(pressurePSI-((pMinPSI+pMaxPSI)/2));


    double deltaPressurePascals = MFA_ASPD4525_PSI2PASCALS(deltaPressurePSI);
    return (deltaPressurePascals);
}


double MFA_ASPD4525_GetTemperature(
    MFA_ConfigTbl_t* configTblP, 
    uint32 temperatureDataCounts
) {
    double tMinCelsius = configTblP->temperatureMinimum_Celcius; 
    double tMaxCelsius = configTblP->temperatureMaximum_Celcius;
    double tempCelsius = 
        (double)temperatureDataCounts * (tMaxCelsius - tMinCelsius)
        / ((double)(MFA_ASPD4525_TEMPERATURE_COUNTS_MAX))
        + tMinCelsius;
    return (tempCelsius);
}

double MFA_ASPD4525_GetPressure(
    MFA_ConfigTbl_t* configTblP
) {
    return MFA_ASPD4525_ATM2PASCALS(1);
}

double MFA_ASPD4525_GetAirDensity(
    MFA_ConfigTbl_t* configTblP, 
    double temperatureCelsius,
    double pressurePascals
) {
    double rho = pressurePascals / ((MFA_ASPD4525_CELSIUS2KELVIN(temperatureCelsius))*((configTblP->airGasConstantR_SI)/(configTblP->airMolarMass_SI)));
    return rho;
}

double MFA_ASPD4525_GetAirSpeed(
    double deltaPressure,
    double airDensity
) {
    double airSpeed = sqrt(2*deltaPressure/airDensity);
    return airSpeed;
}

double sqrt(double number) {
    double sqrt = number / 2;
    double temp = 0;

    // Iterate until sqrt is different of temp, that is updated on the loop
    while(sqrt != temp){
        // initially 0, is updated with the initial value of 128
        // (on second iteration = 65)
        // and so on
        temp = sqrt;

        // Then, replace values (256 / 128 + 128 ) / 2 = 65
        // (on second iteration 34.46923076923077)
        // and so on
        sqrt = ( number/temp + temp) / 2;
    }
    return sqrt;
}