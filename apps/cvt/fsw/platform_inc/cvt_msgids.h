    
#ifndef CVT_MSGIDS_H
#define CVT_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/***** TODO:  These Message ID values are default and may need to be changed
******        by the developer or mission integrator!
******/

/**
** \brief CVT application-specific command MID
*/
#define CVT_CMD_MID                 (0x18C0)

/**
** \brief Command for the application to send HK data
*/
#define CVT_SEND_HK_MID             (0x18C1)

/**
** \brief Wakeup command for the application to perform processing
*/
#define CVT_WAKEUP_MID              (0x18D0)

/**
** \brief Application housekeeping telemetry data
*/
#define CVT_HK_TLM_MID              (0x08BB)


#ifdef __cplusplus
}
#endif

#endif /* CVT_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
