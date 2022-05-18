#ifndef MFA_MSGS_H
#define MFA_MSGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cfe.h"

typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
} MFA_NoArgCmd_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*MFA_MSGS_H*/
