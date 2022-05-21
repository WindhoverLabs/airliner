/**
 * @file mfa_app.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the Header File for the MFA App
 * @version 1.0.0
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_APP_H
#define MFA_APP_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"

/**
 * @brief This is the major version number for this app
 * 
 */
#define MFA_MAJOR_VERSION    (1)

/**
 * @brief This is the minor version number for this app
 * 
 */
#define MFA_MINOR_VERSION    (0)

/**
 * @brief This is the patch version number for this app
 * 
 */
#define MFA_PATCH_VERSION    (0)


typedef struct {
    CFE_TBL_Handle_t  ConfigTblHdl;
    MFA_ConfigTbl_t  *ConfigTblPtr;
    MFA_HkTlm_t       HkTlm;
    CFE_SB_PipeId_t   SchPipeId;
} MFA_AppData_t;


/**
 * @brief This is the App Main Function
 * 
 */
void MFA_AppMain(void);

/**
 * @brief This function gets called to initialize the App within Flight Software.
 * It registers the app with ES.
 * It registers events with EVS by calling the MFA_InitEvents function.
 * It sends the event MFA_INITIALIZED_EID when done.
 * 
 * @return uint32 returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_AppInit(void);

/**
 * @brief This function Unregisters the App from ES.
 * This function should not be called in normal operating conditions.  If this function is called, something has gone wrong.
 * 
 * @param exitStatus This is the exit error status ID that the system is reporting.
 */
void MFA_AppDeinit(uint32 exitStatus);

/**
 * @brief This function registers the App with EVS with a event filter mask.
 * 
 * @return uint32 returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_InitEvents(void);

/**
 * @brief This function initializes telemetry messaging
 * 
 * @return uint32  returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_InitData(void);

/**
 * @brief This function initializes the Config Tables
 * 
 * @return uint32  returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_InitTables(void);

/**
 * @brief This function loads data into the Config Tables
 * 
 * @return uint32 returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_LoadConfigTable(void);

/**
 * @brief This function gets the address to the config table
 * 
 * @return uint32 returns CFE_ES_APP_RUN if everything is fine.  returns CFE_ES_APP_ERROR if there is an error
 */
uint32 MFA_GetConfigTableAddress(void);

/**
 * @brief This function sends an event saying the initialization of the MFA was completed successfully.
 * 
 */
void MFA_SendInitializedEvent(void);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*MFA_APP_H*/
