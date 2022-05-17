#include "cfe.h"
#include "mfa_aspd4525.h"
#include "mfa_tbldefs.h"

#include "uah_app.h"


int32 MFA_ASPD4525_Setup () {
    int32 status = CFE_SUCCESS;
    UAH_AppSetup();

    return status;
}

void MFA_ASPD4525_Loop (uint32* pressureCountP, uint32* temperatureCountP, uint32* statusP) {
    UAH_AppLoop(
        (unsigned int*) pressureCountP, 
        (unsigned int*) temperatureCountP,
        (unsigned int*) statusP);
}