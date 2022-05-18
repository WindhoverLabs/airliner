/**
 * @file mfa_msgs.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This function defines the Housekeeping Telemetry Data Structures and Command Message Data Structures
 * @version 1.0.0
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_MSGS_H
#define MFA_MSGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cfe.h"

/**
 * @brief This structure defines the telemetry data structure
 * 
 */
typedef struct {
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32 Commands;
    uint32 CmdErrors;
    uint32 HelloCount;
    uint32 pressureCount;
    uint32 temperatureCount;
    uint32 status;
    double Temperature;
    double AirSpeed;
} MFA_HkTlm_t;

/**
 * @brief This structure defines the command message data structure
 * 
 */
typedef struct {
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
} MFA_NoArgCmd_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*MFA_MSGS_H*/
