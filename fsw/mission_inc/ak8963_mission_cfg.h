    
#ifndef AK8963_MISSION_CFG_H
#define AK8963_MISSION_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** AK8963 Mission Configuration Parameter Definitions
*/


/**
 * @brief Scheduler pipe software bus timeout.
 *
 * This defines the scheduler pipe software bus timeout.
 * This is normally set to #CFE_SB_PEND_FOREVER, but could
 * also be set to a time in milliseconds, allowing AK8963 to execute
 * even when the Scheduler application is not.  When you
 * do this, make sure the timeout is larger than the scheduled
 * period, with margin, to ensure that it doesn't get released early
 * and released a second time from the scheduler message.
 */
#define AK8963_SB_TIMEOUT         CFE_SB_PEND_FOREVER  /* Can be a value in milliseconds */

#ifdef __cplusplus
}
#endif

#endif /* AK8963_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
