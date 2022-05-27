#ifndef MFA_MSGS_H
#define MFA_MSGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cfe.h"

/**
 * @brief This is a Noop Command Code in the MFA_CMD_MID Message ID
 * 
 */
#define MFA_NOOP_CC         (0)

/**
 * @brief This is a Reset Command Code in the MFA_CMD_MID Message ID
 * 
 */
#define MFA_RESET_CC        (1)


/**
 * @brief This is the typedef struct for the House Keeping Telemetry sent to ground
 * 
 */
typedef struct {
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 Commands;
    uint32 CmdErrors;
    uint32 HelloCount;
} MFA_HkTlm_t;


/**
 * @brief This is the typedef struct for No Argument Commands sent to the App
 * 
 */
typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
} MFA_NoArgCmd_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*MFA_MSGS_H*/
