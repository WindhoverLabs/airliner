#ifndef AK8963_MSGIDS_H
#define AK8963_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Housekeeping telemetry message ID.
 *
 * This is is the message ID assigned to the #AK8963_HkTlm_t
 * message.
 */
#define AK8963_HK_TLM_MID                            (0x0cc1)

/**
 * @brief Send housekeeping message command message ID.
 *
 * This message can be scheduled by Scheduler or sent by the
 * ground.  Upon receipt, this will cause the AK8963
 * application to send the #AK8963_HkTlm_t message.
 */
#define AK8963_SEND_HK_MID                           (0x1cc2)

/**
 * @brief Wakeup command message ID.
 *
 * This message causes AK8963 to wakeup and read the device.
 * This is normally schedule by Scheduler.  Upon receipt,
 * this will cause the AK8963 application to call #ReadDevice.
 */
#define AK8963_WAKEUP_MID                            (0x1cc3)

/**
 * @brief Ground command message ID.
 *
 * This is the message ID assigned to all ground commands.
 */
#define AK8963_CMD_MID                               (0x1cc4)

/**
 * @brief Diagnostic telemetry message ID.
 *
 * This is the message ID assigned to the AK8963_DiagPacket_t
 * message.
 */
#define AK8963_DIAG_TLM_MID                          (0x0cc5)

#ifdef __cplusplus
}
#endif

#endif /* AK8963_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
