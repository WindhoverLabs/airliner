#ifndef SBND_PLATFORM_CFG_H
#define SBND_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** SBND Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for SBND application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "amc_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define SBND_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SBND_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SBND_SCH_PIPE_NAME              ("SBND_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define SBND_SCH_PIPE_PEND_TIME         (4)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       SBND_WAKEUP_MID_MAX_MSG_COUNT and SBND_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than SBND_SCH_PIPE_DEPTH.
*/
#define SBND_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       SBN_WAKEUP_MID_MAX_MSG_COUNT and SBND_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than SBND_SCH_PIPE_DEPTH.
*/
#define SBND_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SBND_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SBND_CMD_PIPE_NAME              ("SBND_CMD_PIPE")

#define SBND_CONFIG_MAX_MIDS            (170)

#define SBND_DATA_MID_MAX_MSG_COUNT      (1)

#define SBND_MAX_RECV_MSGS_PER_WAKEUP    (SBND_CONFIG_MAX_MIDS)

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SBND_DATA_PIPE_DEPTH            SBND_CONFIG_MAX_MIDS

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SBND_DATA_PIPE_NAME             ("SBND_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define SBND_CONFIG_TABLE_FILENAME      ("/ram/apps/sbnd_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define SBND_STARTUP_TIMEOUT_MSEC    (1000)

#define SBND_CF_THROTTLE_SEM_NAME       "SBND_CF_SEM"
#define SBND_CF_THROTTLE_SEM_MAX_COUNT  (1)
#define SBND_CF_PDU_MID                 CF_CPD_TO_PPD_PDU_MID

#ifdef __cplusplus
}
#endif

#endif /* SBND_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
