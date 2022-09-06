#ifndef AMC_PLATFORM_CFG_H
#define AMC_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** amc Platform Configuration Parameter Definitions
*/

#define AMC_MULTIROTOR_MIXER_MAX_MIXERS   (1)
#define AMC_SIMPLE_MIXER_MAX_MIXERS       (10)
#define AMC_SIMPLE_MIXER_MAX_CONTROLS     (2)

#define AMC_PWM_FREQUENCY_CHANNEL_0       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_1       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_2       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_3       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_4       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_5       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_6       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_7       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_8       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_9       (400)
#define AMC_PWM_FREQUENCY_CHANNEL_10      (400)
#define AMC_PWM_FREQUENCY_CHANNEL_11      (400)
#define AMC_PWM_FREQUENCY_CHANNEL_12      (400)
#define AMC_PWM_FREQUENCY_CHANNEL_13      (400)
#define AMC_PWM_FREQUENCY_CHANNEL_14      (400)
#define AMC_PWM_FREQUENCY_CHANNEL_15      (400)


/** \brief Mission specific version number for AMC application
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
#define AMC_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define AMC_SCH_PIPE_DEPTH             (15)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define AMC_SCH_PIPE_NAME              ("AMC_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define AMC_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       AMC_SCH_PIPE_WAKEUP_RESERVED and AMC_SCH_PIPE_SEND_HK_RESERVED
**       must be less than AMC_SCH_PIPE_DEPTH.
*/
#define AMC_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       AMC_SCH_PIPE_WAKEUP_RESERVED and AMC_SCH_PIPE_SEND_HK_RESERVED
**       must be less than AMC_SCH_PIPE_DEPTH.
*/
#define AMC_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define AMC_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define AMC_CMD_PIPE_NAME              ("AMC_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define AMC_PARAM_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define AMC_PARAM_PIPE_NAME             ("AMC_PARAM_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define AMC_CONFIG_TABLE_FILENAME            "/ram/apps/amc_config.tbl"
#define AMC_MIXER_CONFIG_TABLE_FILENAME      "/ram/apps/amc_mixercfg.tbl"

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define AMC_STARTUP_TIMEOUT_MSEC    (1000)

/* TODO:  Add Doxygen markup. */
#define AMC_MAX_MOTOR_OUTPUTS       (10)

#define AMC_MAX_PARAMS       (3)



#ifdef __cplusplus
}
#endif

#endif /* AMC_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
