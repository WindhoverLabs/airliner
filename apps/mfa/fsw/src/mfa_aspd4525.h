#ifndef MFA_ASPD4525_H
#define MFA_ASPD4525_H

#ifdef __cpluplus
extern "C" {
#endif

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"


#include "uah_app.h"

#define MFA_ASPD4525_SUCCESS                    (0)
#define MFA_ASPD4525_BUSY                       (1)
#define MFA_ASPD4525_STALE                      (2)
#define MFA_ASPD4525_ERROR                      (-1)



#define MFA_ASPD4525_PRESSURE_COUNTS_MAX        ((uint32) 0x3fff)
#define MFA_ASPD4525_TEMPERATURE_COUNTS_MAX     ((uint32) 0x07ff)

#define MFA_ASPD4525_PSI2PASCALS(psi)           (((double) 6894.76)*(psi))
#define MFA_ASPD4525_ATM2PASCALS(atm)           (((double) 101325.0)*(atm))

#define MFA_ASPD4525_CELSIUS2KELVIN(celsius)    (((double) 273.15)+(celsius))


int32 MFA_ASPD4525_Setup ();

void MFA_ASPD4525_Loop (
    MFA_ConfigTbl_t* configTblP, 
    MFA_HkTlm_t* tlmTabP 
);

double MFA_ASPD4525_GetDeltaPressure(
    MFA_ConfigTbl_t* configTblP, 
    uint32 pressureDataCounts
);

double MFA_ASPD4525_GetTemperature(
    MFA_ConfigTbl_t* configTblP, 
    uint32 temperatureDataCounts
);

double MFA_ASPD4525_GetPressure(
    MFA_ConfigTbl_t* configTblP
);

double MFA_ASPD4525_GetAirDensity(
    MFA_ConfigTbl_t* configTblP, 
    double temperatureCelsius,
    double pressurePascals
);

double MFA_ASPD4525_GetAirSpeed(
    double deltaPressure,
    double airDensity
);
#ifdef __cpluplus
}
#endif

#endif /* MFA_ASPD4525_H */