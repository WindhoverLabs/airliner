    
#ifndef MS5611_MSGIDS_H
#define MS5611_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

#include "msg_ids.h"

/**
** \brief MS5611 application-specific command MID
*/
#define MS5611_CMD_MID                 (0x18C0)

/**
** \brief Command for the application to send HK data
*/
#define MS5611_SEND_HK_MID             (0x18C1)

/**
** \brief Wakeup command for the application to perform processing
*/
#define MS5611_WAKEUP_MID              (0x18D0)

/**
** \brief Application housekeeping telemetry data
*/
#define MS5611_HK_TLM_MID              (0x08BB)


#ifdef __cplusplus
}
#endif

#endif /* MS5611_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
