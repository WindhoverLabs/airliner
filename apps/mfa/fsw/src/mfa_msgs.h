#ifndef MFA_MSGS_H
#define MFA_MSGS_H

#ifdef __cpluplus
extern "C" {
#endif

#include "cfe.h"


typedef struct {
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 Commands;
    uint32 CmdErrors;
    uint32 HelloCount;
    uint32 pressureCount;
    uint32 temperatureCount;
    uint32 status;
} MFA_HkTlm_t;


typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
} MFA_NoArgCmd_t;

#ifdef __cpluplus
}
#endif

#endif /*MFA_MSGS_H*/