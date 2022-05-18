#ifndef MFA_PLATFORM_CFG_H
#define MFA_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cfe.h"

#define MFA_SCH_PIPE_DEPTH              (2)
#define MFA_SCH_PIPE_NAME               "MFA_SCH_PIPE"
#define MFA_SCH_PIPE_WAKEUP_RESERVED    (1)
#define MFA_SCH_PEND_TIME               (CFE_SB_PEND_FOREVER)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*MFA_PLATFORM_CFG_H*/
