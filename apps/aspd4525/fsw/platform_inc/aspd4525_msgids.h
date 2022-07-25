    
#ifndef ASPD4525_MSGIDS_H
#define ASPD4525_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

/**
** \brief ASPD4525 application-specific command MID
*/
#define ASPD4525_CMD_MID                 (0x18C0)

/**
** \brief Command for the application to send HK data
*/
#define ASPD4525_SEND_HK_MID             (0x18C1)

/**
** \brief Wakeup command for the application to perform processing
*/
#define ASPD4525_WAKEUP_MID              (0x18D0)

/**
** \brief Application-specific data output
*/
#define ASPD4525_OUT_DATA_MID            (0x08D1)

/**
** \brief Application housekeeping telemetry data
*/
#define ASPD4525_HK_TLM_MID              (0x08BB)


#define PX4_AIRSPEED_MID                 (0x08BC)


#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
