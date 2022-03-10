    
#ifndef RFD900X_MSGIDS_H
#define RFD900X_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

#include "msg_ids.h"

/**
** \brief RFD900X application-specific command MID
*/
#define RFD900X_CMD_MID                 (0x18C0)

/**
** \brief Command for the application to send HK data
*/
#define RFD900X_SEND_HK_MID             (0x18C1)

/**
** \brief Wakeup command for the application to perform processing
*/
#define RFD900X_WAKEUP_MID              (0x18D0)

/**
** \brief Application-specific data output
*/
#define RFD900X_OUT_DATA_MID            (0x18D1)

/**
** \brief Application housekeeping telemetry data
*/
#define RFD900X_HK_TLM_MID              (0x08BB)


#ifdef __cplusplus
}
#endif

#endif /* RFD900X_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
