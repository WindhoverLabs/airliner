#ifndef _CFE_PSP_PRIVATE_H_
#define _CFE_PSP_PRIVATE_H_

#include "cfe_psp.h"
#include "osapi-os-rtm.h"

#define CFE_PSP_CPU_NAME_LENGTH  32

/************************************************************************/
/** \brief Main global PSP data struct
 **
 **  \par Description
 **       This is the main data structure used by the PSP.
 **
 *************************************************************************/
typedef struct
{
    uint32 TimerCounter;
    uint32 CFE_PSP_SpacecraftId;
    uint32 CFE_PSP_CpuId;
    char CFE_PSP_CpuName[CFE_PSP_CPU_NAME_LENGTH];
    OS_RunTimeModeEnum_t CFE_PSP_RunMode;
} CFE_PSP_GlobalData_t;

#endif
