/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef GPS_SERIAL_H
#define GPS_SERIAL_H
/************************************************************************
** Includes
*************************************************************************/
#include "gps_custom.h"
#include "../gps_custom_shared.h"
#include "cfe.h"
#include "px4_msgs.h"

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************
** Local Defines
*************************************************************************/
/** \brief GPS device path. */
#define GPS_SERIAL_DEVICE_PATH                    "/dev/ttyPA1"

/** \brief IO speed.
**
**  \par Description:
**       The serial IO speed 230400 baud.
*/
#define GPS_SERIAL_IO_SPEED                       (230400)

/** \brief Wait time (ms) before read.
**
**  \par Description:
**       ms, wait before reading to save read() calls.
*/
#define GPS_WAIT_BEFORE_READ                      (20)

/** \brief GPS packet timeout (ms).
**
**  \par Description:
**       ms, timeout to receive packet.
*/
#define GPS_PACKET_TIMEOUT                        (200)

/** \brief GPS ack timeout.
**
**  \par Description:
**       ms, timeout to wait for ack.
*/
#define GPS_ACK_TIMEOUT                           (1000)


/** \brief GPS payload scanner buffer size.
**
**  \par Description:
**       GPS payload scanner length.
*/
//#define GPS_SCANNER_BUFFER_LENGTH              (1024)

/* Message Classes */
/** \brief Configuration input class.
**
**  \par Description:
**       UBX class ID configuration.
*/
#define GPS_MESSAGE_CLASS_CFG                     (0x06)


#define GPS_MESSAGE_CLASS_NAV                     (0x01)

#define GPS_MESSAGE_CLASS_MON                     (0x0A)

#define GPS_MESSAGE_CLASS_NMEA                    (0xF0)

/* Message IDs */
/** \brief Message ID port configuration.
**
**  \par Description:
**       Message ID for port configuration.
*/
#define GPS_MESSAGE_ID_CFG_PRT                    (0x00)

/** \brief Message ID reset configuration.
**
**  \par Description:
**       Message ID for reset.
*/
#define GPS_MESSAGE_ID_CFG_RST                    (0x04)

/** \brief Message ID rate configuration.
**
**  \par Description:
**       Message ID for rate configuration.
*/
#define GPS_MESSAGE_ID_CFG_RATE                   (0x08)

/** \brief Message ID for message rate configuration.
**
**  \par Description:
**       Message ID for message rate configuration.
*/
#define GPS_MESSAGE_ID_CFG_MSG                    (0x01)

/** \brief Message ID NAV5 configuration.
**
**  \par Description:
**       Message ID for NAV5 configuration.
*/
#define GPS_MESSAGE_ID_CFG_NAV5                   (0x24)

/** \brief Message ID SBAS configuration.
**
**  \par Description:
**       Message ID for SBAS configuration.
*/
#define GPS_MESSAGE_ID_CFG_SBAS                   (0x16)

/** \brief Message ID navigation position velocity time solution.
**
**  \par Description:
**       Message ID for NAV-PVT.
*/
#define GPS_MESSAGE_ID_NAV_PVT                    (0x07)

/** \brief Message ID dilution of precision.
**
**  \par Description:
**       Message ID for NAV-DOP.
*/
#define GPS_MESSAGE_ID_NAV_DOP                    (0x04)

/** \brief Message ID space vehicle information.
**
**  \par Description:
**       Message ID for NAV-SVINFO.
*/
#define GPS_MESSAGE_ID_NAV_SVINFO                 (0x30)

/** \brief Message ID extended hardware status.
**
**  \par Description:
**       Message ID for MON-HW.
*/
#define GPS_MESSAGE_ID_MON_HW                     (0x09)

#define GPS_MESSAGE_ID_NMEA_GST                   (0x07)

#define GPS_MESSAGE_ID_NMEA_GNS                   (0x0D)

#define GPS_MESSAGE_ID_NAV_POSECEF                (0x01)

#define GPS_MESSAGE_ID_NAV_VELNED                 (0x12)

/* TX CFG-PRT message contents */
/** \brief UART 1 port number.
**
**  \par Description:
**       Port interface number.
*/
#define GPS_TX_CFG_PRT_PORTID                     (0x01)

/** \brief Mode bitfield configuration.
**
**  \par Description:
**       0b0000100011010000: 8N1
*/
#define GPS_TX_CFG_PRT_MODE                       (0x000008D0)

/** \brief Input protocol configuration.
**
**  \par Description:
**       UBX in.
*/
#define GPS_TX_CFG_PRT_INPROTOMASK_GPS            (0x01)

/** \brief Output protocol configuration.
**
**  \par Description:
**       UBX out.
*/
#define GPS_TX_CFG_PRT_OUTPROTOMASK_GPS           (0x01)

/** \brief USB port number.
**
**  \par Description:
**       Port interface number.
*/
#define GPS_TX_CFG_PRT_PORTID_USB                 (0x03)
/* TX CFG-Rate message contents */

/** \brief Measurement interval.
**
**  \par Description:
**       The elapsed time between GNSS measurements. 200ms for 5Hz.
*/
#define GPS_TX_CFG_RATE_MEASINTERVAL              (200)

/** \brief Navigation measurement rate.
**
**  \par Description:
**       This parameter is ignosed and the navRate is fixed to 1 in 
**       protocol versions less than 18.
*/
#define GPS_TX_CFG_RATE_NAVRATE                   (1)

/** \brief Time reference.
**
**  \par Description:
**       0: UTC, 1: GPS time.
*/
#define GPS_TX_CFG_RATE_TIMEREF                   (0)

/* TX CFG-NAV5 message contents */

/** \brief Nav5 parameters bitmask.
**
**  \par Description:
**       Only update dynamic model and fix mode.
*/
#define GPS_TX_CFG_NAV5_MASK                      (0x0005)

/** \brief Nav5 position fixing mode.
**
**  \par Description:
**       1 2D only, 2 3D only, 3 Auto 2D/3D.
*/
#define GPS_TX_CFG_NAV5_FIXMODE                   (2)

/** \brief Nav5 dynamic platform model.
**
**  \par Description:
**       7 = airborne with <2g acceleration.
*/
#define GPS_TX_CFG_NAV5_DYNMODEL                  (7)

/* TX CFG-SBAS message contents */
/** \brief SBAS configuration mode.
**
**  \par Description:
**       1 = SBAS enabled, 2 = SBAS disabled.
*/
#define  GPS_TX_CFG_SBAS_MODE                     (1)



/* UBX header contents */
/** \brief Header symbol 1.
**
**  \par Description:
**       UBX header symbol 1.
*/
#define GPS_HEADER_SYNC1_VALUE                    (0xb5)

/** \brief Header symbol 2.
**
**  \par Description:
**       UBX header symbol 2.
*/
#define GPS_HEADER_SYNC2_VALUE                    (0x62)

/* Message Classes & IDs */
#define GPS_MESSAGE_CFG_PRT          ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_PRT << 8)
                                      
#define GPS_MESSAGE_CFG_RATE         ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_RATE << 8)
                                      
#define GPS_MESSAGE_CFG_RST         ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_RST << 8)

#define GPS_MESSAGE_CFG_NAV5         ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_NAV5 << 8)

#define GPS_MESSAGE_CFG_SBAS         ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_SBAS << 8)

#define GPS_MESSAGE_CFG_MSG          ((GPS_MESSAGE_CLASS_CFG) | \
                                       GPS_MESSAGE_ID_CFG_MSG << 8)

#define GPS_MESSAGE_NAV_PVT          ((GPS_MESSAGE_CLASS_NAV) | \
                                       GPS_MESSAGE_ID_NAV_PVT << 8)
                                       
#define GPS_MESSAGE_NAV_DOP          ((GPS_MESSAGE_CLASS_NAV) | \
                                       GPS_MESSAGE_ID_NAV_DOP << 8)

#define GPS_MESSAGE_NAV_SVINFO       ((GPS_MESSAGE_CLASS_NAV) | \
                                       GPS_MESSAGE_ID_NAV_SVINFO << 8)

#define GPS_MESSAGE_MON_HW           ((GPS_MESSAGE_CLASS_MON) | \
                                       GPS_MESSAGE_ID_MON_HW << 8)

#define GPS_MESSAGE_NMEA_GST         ((GPS_MESSAGE_CLASS_NMEA) | \
                                       GPS_MESSAGE_ID_NMEA_GST << 8)

#define GPS_MESSAGE_NMEA_GNS         ((GPS_MESSAGE_CLASS_NMEA) | \
                                       GPS_MESSAGE_ID_NMEA_GNS << 8)

#define GPS_MESSAGE_NAV_POSECEF         ((GPS_MESSAGE_CLASS_NAV) | \
                                       GPS_MESSAGE_ID_NAV_POSECEF << 8)

#define GPS_MESSAGE_NAV_VELNED         ((GPS_MESSAGE_CLASS_NAV) | \
                                       GPS_MESSAGE_ID_NAV_VELNED << 8)

/** \brief Retry attemps for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define GPS_MAX_RETRY_ATTEMPTS                     (2)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define GPS_MAX_RETRY_SLEEP_USEC                   (10)

/** \brief Streaming task priority
**
**  \par Limits:
**       0 to MAX_PRIORITY (usually 255)
*/
#define GPS_STREAMING_TASK_PRIORITY    (94)

/** \brief GPS shared data mutex name. */
#define GPS_MUTEX_POS                "GPS_MUTEX_POS"

/** \brief GPS shared data mutex name. */
#define GPS_MUTEX_DUMP               "GPS_MUTEX_DUMP"

/** \brief GPS shared data mutex name. */
#define GPS_MUTEX_SAT                "GPS_MUTEX_SAT"

/** \brief Streaming task name
**
**  \par Limits:
**       OS_MAX_API_NAME
*/
#define GPS_STREAMING_TASK_NAME       "GPS_STREAM"

// TODO: this number seems wrong
#define GPS_EPOCH_SECS                ((time_t)1234567890ULL)

#define M_DEG_TO_RAD_F                (0.01745329251994329576f)

/************************************************************************
** Structure Declarations
*************************************************************************/

#pragma pack(push, 1)
/**
 * \brief GPS port configuration payload message.
 */
 /* TODO use struct defined in gps_ubx_msg instead */
typedef struct 
{
    /*! Port Identifier Number */
    uint8       portID;
    /*! Reserved */
    uint8       reserved1;
    /*! TX ready PIN configuration */
    uint16      txReady;
    /*! A bit mask describing the UART mode */
    uint32      mode;
    /*! Baud rate in bits/second */
    uint32      baudRate;
    /*! A mask describing which input protocols are active. */
    uint16      inProtoMask;
    /*! A mask describing which output protocols are active. */
    uint16      outProtoMask;
    /*! Flags bit mask */
    uint16      flags;
    /*! Reserved */
    uint16      reserved2;
} GPS_Payload_TX_CFG_PRT_t;


/**
 * \brief GPS message rate configuration payload message.
 */
/* TODO move to gps_ubx_msg */
typedef struct 
{
    /*! Measurement Rate */
    uint16       measRate;
    /*! Navigation Rate */
    uint16       navRate;
    /*! Alignment to reference time: 0 = UTC time, 1 = GPS time */
    uint16       timeRef;
} GPS_Payload_TX_CFG_Rate_t;


/**
 * \brief GPS message Reset Receiver / Clear Backup Data Structures
 */
/* TODO move to gps_ubx_msg */
typedef struct 
{
    /*! BBR Sections to clear.  */
    uint16      navBbrMask;
    /*! Reset Type. */
    uint8       resetMode;
    /*! Reserved. */
    uint8       reserved1;
} GPS_Payload_TX_CFG_Reset_t;


/* TODO move to gps_ubx_msg*/
/**
 * \brief GPS message rate configuration message.
 */
typedef struct 
{
    uint16       msg;
    uint8        rate;
} GPS_Payload_TX_CFG_Rate_Msg_t;

/**
 * \brief GPS UBX protocol header.
 */
typedef struct 
{
    /*! Sync symbol 1 */
    uint8       sync1;
    /*! Sync symbol 1 */
    uint8       sync2;
    /*! Message */
    uint16      msg;
    /*! Message length */
    uint16      length;
} GPS_Header_t;

/**
 * \brief GPS UBX protocol checksum.
 */
typedef struct 
{
    /*! checksum check A*/
    uint8       ck_a;
    /*! checksum check B*/
    uint8       ck_b;
} GPS_Checksum_t;

#pragma pack(pop)


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/
/************************************************************************/
/** \brief Negotiate GPS baud rate. 
**
**  \par Description
**       This function tries different baud rates to initially establish
**       communication and attempts to set the baud rate specified.
**
**  \par Assumptions, External Events, and Notes:
**       A device must be initialized before this function is called.
**
**  \param [in]     Baud           The baud rate to attempt to set.
**
**  \returns    boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_Negotiate_Baud(const uint32 Baud);

/************************************************************************/
/** \brief Set baud rate. 
**
**  \par Description
**       This function attempts to set the baud rate for the opened
**       device.
**
**  \par Assumptions, External Events, and Notes:
**       A device must be initialized before this function is called.
**
**  \param [in]     Baud           The baud rate to attempt to set.
**
**  \returns    boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_Set_Baud(const uint32 Baud);


/************************************************************************/
/** \brief Sends a message to the GPS device.
**
**  \par Description
**       This function attempts to send a message to the GPS device.
**
**  \par Assumptions, External Events, and Notes:
**       A device must be initialized and baudrate configured before
**       this function is called. This function does not send empty 
**       messages. A payload must be supplied. 
**
**  \param [in]    msg          The message id.
**
**  \param [in]    payload      The message payload.
**
**  \param [in]    length       The message length.
**
**  \returns    boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_SendMessage(const uint16 msg, const uint8 *payload, const uint16 length);

/************************************************************************/
/** \brief Sends a empty message to the GPS device.
**
**  \par Description
**       This function attempts to send an empty (no payload) message 
**       to the GPS device.
**
**  \par Assumptions, External Events, and Notes:
**       A device must be initialized and baudrate configured before
**       this function is called.
**
**  \param [in]    msg          The message id.
**
**  \returns    boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_SendEmptyMessage(const uint16 msg);

/************************************************************************/
/** \brief Set a checksum.
**
**  \par Description
**       This function calculates and populates the checksum structure.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [in]    buffer       The buffer to use for checksum 
**                              calculation.
**
**  \param [in]    length       The buffer length.
**
**  \param [out]   checksum     The checksum structure to populate with 
**                              the calculated checksum.
**
**  \returns    boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_SetChecksum(const uint8 *buffer, const uint16 length, GPS_Checksum_t *checksum);


/************************************************************************/
/** \brief Receive data from the GPS device.
**
**  \par Description
**       This receives data by calling select and read.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [out]    buffer      The output buffer.
**
**  \param [in]    length       The buffer length to attempt to read.
**
**  \param [in]    Timeout      The timeout for select.
**
**  \returns       int32,       returns bytes read if successful, -1 or
**                              for failure. Select or read will set
**                              errno.
**
*************************************************************************/
int32 GPS_Custom_Receive(uint8 *Buffer, uint32 Length, uint32 Timeout);


/************************************************************************/
/** \brief Call select and wait for data from the GPS device.
**
**  \par Description
**       This function calls select with the timeout specified.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [in]    TimeoutSec    The timeout in seconds.
**
**  \param [in]    TimeoutUSec   The timeout in microseconds.
**
**  \returns       int32,        returns greater than 0 for success, 0
**                               for timeout, negative value for error.
**
*************************************************************************/
int32 GPS_Custom_Select(const uint32 TimeoutSec, const uint32 TimeoutUSec);

/************************************************************************/
/** \brief Wait for an acknowledgement.
**
**  \par Description
**       This function waits for a specified period for a message.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [in]    msg           The message to wait for an ack for.
**
**  \param [in]    timeout       The timeout to wait.
**
**  \returns       boolean, TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_WaitForAck(const uint16 msg, const uint32 timeout);


/************************************************************************/
/** \brief Configures the GPS device.
**
**  \par Description
**       This sends all the configuration messages.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \returns       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_Configure(void);

/************************************************************************/
/** \brief Configure a message rate.
**
**  \par Description
**       Configure message rates. 
**
**  \par Assumptions, External Events, and Notes:
**       The rate argument is the divisor for the measurement rate. 
**       i.e. 1 means 5Hz.
**
**  \param [in]    msg        The message to set a rate for.
**
**  \param [in]    rate       The divisor for the measurement rate.
**
**  \returns       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_SendMessageRate(const uint16 msg, const uint8 rate);


/************************************************************************/
/** \brief The stream task that actively reads the GPS input stream.
**
**  \par Description
**       This thread runs until uninit.
**
*************************************************************************/
void GPS_Stream_Task(void);


/************************************************************************/
/** \brief Reads and parses messages from the GPS device.
**
**  \par Description
**       This should be called from waitforack or in the stream task
**       thread repeatedly.
**
**  \param [in]    timeout        The read timeout.
**
**  \returns       TRUE for success, FALSE for failure.
**
*************************************************************************/
boolean GPS_Custom_Read_and_Parse(const uint32 timeout);

boolean GPS_Custom_Disable_Bebop_Msgs(void);

#ifdef __cplusplus
}
#endif 

#endif /* GPS_SERIAL_H */
