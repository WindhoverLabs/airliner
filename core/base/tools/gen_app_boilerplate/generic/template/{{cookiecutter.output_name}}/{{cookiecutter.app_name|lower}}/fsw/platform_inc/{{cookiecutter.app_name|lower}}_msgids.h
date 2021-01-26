    
#ifndef {{cookiecutter.app_name}}_MSGIDS_H
#define {{cookiecutter.app_name}}_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

#include "msg_ids.h"

/**
** \brief {{cookiecutter.app_name}} application-specific command MID
*/
#define {{cookiecutter.app_name}}_CMD_MID                 (0x18C0)

/**
** \brief Command for the application to send HK data
*/
#define {{cookiecutter.app_name}}_SEND_HK_MID             (0x18C1)

/**
** \brief Wakeup command for the application to perform processing
*/
#define {{cookiecutter.app_name}}_WAKEUP_MID              (0x18D0)

/**
** \brief Application-specific data output
*/
#define {{cookiecutter.app_name}}_OUT_DATA_MID            (0x18D1)

/**
** \brief Application housekeeping telemetry data
*/
#define {{cookiecutter.app_name}}_HK_TLM_MID              (0x08BB)


#ifdef __cplusplus
}
#endif

#endif /* {{cookiecutter.app_name}}_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
