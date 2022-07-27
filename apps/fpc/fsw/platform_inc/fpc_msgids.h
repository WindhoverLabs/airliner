    
#ifndef FPC_MSGIDS_H
#define FPC_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

/**
** \brief FPC application-specific command MID
*/
#define FPC_CMD_MID                 (0x1c64)

/**
** \brief Command for the application to send HK data
*/
#define FPC_SEND_HK_MID             (0x1c62)

/**
** \brief Wakeup command for the application to perform processing
*/
#define FPC_WAKEUP_MID              (0x1c63)

/**
** \brief Application-specific data output
*/
#define FPC_OUT_DATA_MID            (0x18D1)

/**
** \brief Application housekeeping telemetry data
*/
#define FPC_HK_TLM_MID              (0x0c61)


#define PX4_MANUAL_CONTROL_SETPOINT_MID     0x0a5c
#define PX4_AIRSPEED_MID                    0x0a5d
#define PX4_VEHICLE_GLOBAL_POSITION_MID     0x0a5e


#ifdef __cplusplus
}
#endif

#endif /* FPC_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
