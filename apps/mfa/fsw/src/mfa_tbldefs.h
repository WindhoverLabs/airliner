/**
 * @file mfa_tbldefs.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This header file contains the contents of the config table
 * @version 1.0.0
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_TBLDEFS_H
#define MFA_TBLDEFS_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * @brief This is the maximum size of the string that the config Message can hold
 * 
 */
#define MFA_MESSAGE_MAX_SIZE (100)

/**
 * @brief This is the struct definition for the Config Table
 * 
 */
typedef struct {
	char Message[MFA_MESSAGE_MAX_SIZE];
} MFA_ConfigTbl_t;


#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*MFA_TBLDEFS_H*/