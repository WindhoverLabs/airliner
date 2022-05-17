/**
 * @file mfa_app.h
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the Airspeed Sensor App Main File Header
 * @version 1.0.0
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MFA_APP_H
#define MFA_APP_H

#ifdef __cpluplus
extern "C" {
#endif

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msgs.h"

/**
 * @brief This is the Major Version Number for the App
 * 
 */
#define MFA_MAJOR_VERSION    (1)

/**
 * @brief This is the Minor Version Number for the App
 * 
 */
#define MFA_MINOR_VERSION    (0)

/**
 * @brief This is the Patch Version Number for the App
 * 
 */
#define MFA_PATCH_VERSION    (0)

/**
 * @brief This is the structure that defines what data the App is storing
 * 
 */
typedef struct {
    CFE_TBL_Handle_t  ConfigTblHdl;     /* This variable holds the Config Table */
    MFA_ConfigTbl_t  *ConfigTblPtr;     /* This variable holds the pointer to the Config Table*/
    MFA_HkTlm_t       HkTlm;            /* This variable holds the House Keeping Telemetry Table*/
    CFE_SB_PipeId_t   SchPipeId;        /* This variable holds the software bus pipe ID fot the Scheduler*/
} MFA_AppData_t;


/**
 * @brief This is the main entry point for the App
 * 
 */
void MFA_AppMain(void);

/**
 * @brief This is the function that initializes the App
 * 
 * @return uint32 CFE_ES_APP_RUN if initialization was a success, CFE_ES_APP_ERROR if not
 */
uint32 MFA_AppInit(void);

/**
 * @brief This function kills the app
 * 
 * @param exitStatus It requires an error status on why the app is exiting.
 */
void MFA_AppDeinit(uint32 exitStatus);

/**
 * @brief This function initializes the Event tables
 * 
 * @return uint32 CFE_ES_APP_RUN if initialization was a success, CFE_ES_APP_ERROR if not
 */
uint32 MFA_InitEvents(void);

/**
 * @brief This function initializes the Config tables
 * 
 * @return uint32 CFE_ES_APP_RUN if initialization was a success, CFE_ES_APP_ERROR if not
 */
uint32 MFA_InitTables(void);

/**
 * @brief This function initializes Housekeeping Telemetry
 * 
 * @return uint32 CFE_ES_APP_RUN if initialization was a success, CFE_ES_APP_ERROR if not
 */
uint32 MFA_InitData(void);

/**
 * @brief This function initializes a Software Bus and subscribes to MFA_WAKEUP_MID and MFA_SEND_HK_MID messages
 * 
 * @return uint32 CFE_ES_APP_RUN if initialization was a success, CFE_ES_APP_ERROR if not
 */
uint32 MFA_InitPipes(void);

/**
 * @brief This function sends an event saying the App was initialized correctly
 * 
 */
void MFA_SendAppInitializedEvent(void);

#ifdef __cpluplus
}
#endif

#endif /*MFA_APP_H*/