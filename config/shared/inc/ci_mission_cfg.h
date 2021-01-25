#ifndef CI_MISSION_CFG_H
#define CI_MISSION_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** CI Mission Configuration Parameter Definitions
*/

/** \brief Max number of registered commands in CI table. */
#define CI_MAX_RGST_CMDS		(100)

/** \brief Flag for whether checksum is required for command validation.
**
**  \par Limits:
**       0 for not required. 1 for required.
*/
#define CI_CHECKSUM_REQUIRED	(0)

/** \brief Timeout for authorized two step commands. 
**
**  \note This number represents number of CI_PROCESS_TIMEOUTS_MID messages
**        received until timeout. CI_PROCESS_TIMEOUTS_MID is scheduled at
**        1hz by default so this is equivalent to seconds unless the scheduled
**        rate changes.
*/
#define CI_CMD_MAX_TIMEOUT	20

#ifdef __cplusplus
}
#endif

#endif /* CI_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
