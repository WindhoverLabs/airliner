/**
 * @file mfa_events.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This header file defines the different events ids the app sends
 * @version 1.0.0
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_EVENTS_H
#define MFA_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
/**
 * @brief It is an the event id that the app sends after app is initialized
 * 
 */
#define MFA_INITIALIZED_EID     (1)

/**
 * @brief It is an the event id that the app sends during its loop
 * 
 */
#define MFA_LOOPING_EID         (2)

/**
 * @brief It is an the event id that the app sends if there is a problem during initialization
 * 
 */
#define MFA_INIT_ERR_EID        (3)

/**
 * @brief It is an event id that the app sends when a Noop is being operated on
 * 
 */
#define MFA_CMD_NOOP_INF_EID    (4)

/**
 * @brief It is an event id that the app sends when a Reset is being operated on
 * 
 */
#define MFA_CMD_RESET_INF_EID   (5)

/**
 * @brief It is an event id that the app sends when the msg length received is wrong
 * 
 */
#define MFA_MSG_LEN_ERR_EID     (6)

/**
 * @brief It is an event id that the app sends when the app receieved an unexpected message id
 * 
 */
#define MFA_INVALID_MSG_EID     (7)

/**
 * @brief It is an event id that the app sends when the app is unable to run CFE_TBL_Modified
 * function successfully
 * 
 */
#define MFA_TABLEMOD_ERR_EID    (8)

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*MFA_EVENTS_H*/