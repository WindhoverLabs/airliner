#ifndef MFA_ASPD4525_H
#define MFA_ASPD4525_H

#ifdef __cpluplus
extern "C" {
#endif

#include "cfe.h"
#include "mfa_tbldefs.h"

#include "uah_app.h"


#define MFA_ASPD4525_PRESSURE_COUNTS_MAX        ((uint32) 0x3fff)
#define MFA_ASPD4525_TEMPERATURE_COUNTS_MAX     ((uint32) 0x07ff)

#define MFA_ASPD4525_PSI2PASCALS                ((double) 6894.76)


int32 MFA_ASPD4525_Setup ();

void MFA_ASPD4525_Loop (uint32* pressureCountP, uint32* temperatureCountP, uint32* statusP);


#ifdef __cpluplus
}
#endif

#endif /* MFA_ASPD4525_H */