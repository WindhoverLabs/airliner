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

#ifndef GPS_CUSTOM_SHARED_H
#define GPS_CUSTOM_SHARED_H
/************************************************************************
** Includes
*************************************************************************/
#include "gps_events.h"
#include "cfe.h"
#include "px4_msgs.h"
#include "priorities.h"

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************
** Local Defines
*************************************************************************/

/** \brief GPS read buffer size.
**
**  \par Description:
**       MON_VER from u-blox modules can be ~190 bytes
*/
#define GPS_READ_BUFFER_SIZE                      (250)
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

/** \brief Message ID navigation position velocity time solution.
**
**  \par Description:
**       Message ID for NAV-PVT.
*/
#define GPS_MESSAGE_ID_NAV_PVT                    (0x07)

#define GPS_MESSAGE_CLASS_NAV                     (0x01)

#define GPS_MESSAGE_NAV_PVT_LENGTH                (92)
#define GPS_MESSAGE_UBX_HEADER_LENGTH             (6)
#define GPS_MESSAGE_UBX_CHKSUM_LENGTH             (2)

#define M_DEG_TO_RAD_F                            (0.01745329251994329576f)

/** \brief GPS packet timeout (ms).
**
**  \par Description:
**       ms, timeout to receive packet.
*/
#define GPS_PACKET_TIMEOUT                        (200)

// TODO: this number seems wrong
#define GPS_EPOCH_SECS                ((time_t)1234567890ULL)

/************************************************************************
** Structure Declarations
*************************************************************************/
typedef enum 
{

/** \brief <tt> 'GPS - ' </tt>
**  \event <tt> 'GPS - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    GPS_DEVICE_ERR_EID = GPS_EVT_CNT,

/** \brief <tt> 'GPS - ' </tt>
**  \event <tt> 'GPS - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a GPS message parser encounters an 
**  error.
**
*/
    GPS_INIT_DEVICE_PARSER_ERR_EID,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    GPS_CUSTOM_EVT_CNT
} GPS_CustomEventIds_t;


typedef struct
{
    /*! \brief cFE Software Bus Telemetry Message Header */
    uint8       TlmHeader[CFE_SB_TLM_HDR_SIZE];          
    uint8       Payload[GPS_READ_BUFFER_SIZE];
} GPS_DeviceMessage_t;


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
    uint8       class;
    uint8       id;
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


/**
 * \brief Parser state.
 */
typedef enum 
{
    GPS_PARSE_STATE_UNINIT=0,
    GPS_PARSE_STATE_IDLE,
    GPS_PARSE_STATE_GOT_SYNC1,
    GPS_PARSE_STATE_GOT_SYNC2,
    GPS_PARSE_STATE_GOT_CLASS,
    GPS_PARSE_STATE_GOT_ID,
    GPS_PARSE_STATE_GOT_LENGTH1,
    GPS_PARSE_STATE_GOT_LENGTH2,
    GPS_PARSE_STATE_GOT_PAYLOAD,
    GPS_PARSE_STATE_GOT_CHECKSUMA,
    GPS_PARSE_STATE_GOT_CHECKSUMB
} GPS_ParserState_t;


typedef struct
{
    /*! Number of received messages */
    uint32 MsgReceived;
    /*! Number of parse errors */
    uint32 ParseError;
    /*! Parsing state machine */
    GPS_ParserState_t ParseState;
    uint16 PayloadCursor;
    uint8 ClassID;
    uint8 MsgID;
    uint16 MsgLength;
    uint16 ChecksumA;
} GPS_ParserStatus_t;


/**
 * \brief GPS device status
 */
typedef enum
{
    /*! GPS status uninitialized */
    GPS_CUSTOM_UNINITIALIZED  = 0,
    /*! GPS status initialized */
    GPS_CUSTOM_INITIALIZED   = 1,
    /*! GPS status streaming */
    GPS_CUSTOM_STREAMING     = 2
} GPS_Custom_Status_t;


typedef enum 
{
    GPS_ACK_IDLE    = 0,
    GPS_ACK_WAITING = 1,
    GPS_ACK_GOT_ACK = 2,
    GPS_ACK_GOT_NAK = 3
} GPS_Ack_State_t;


typedef struct
{
    /*! Device file descriptor */
    //int                          DeviceFd;
    /*! The current device status */
    GPS_Custom_Status_t          Status;
    /*! The current baud */
    uint32                       Baud;
    /*! The current parser status */
    GPS_ParserStatus_t           ParserStatus;
    /*! Streaming task priority */
    uint8                           Priority;
    /*! Streaming child task identifier */
    uint32                          ChildTaskID;
    /*! Streaming task function pointer */
    CFE_ES_ChildTaskMainFuncPtr_t   StreamingTask;
    //PX4_GpsDumpMsg_t             GpsDumpMsg;
    PX4_VehicleGpsPositionMsg_t  GpsPositionMsg;
    PX4_SatelliteInfoMsg_t       GpsSatInfoMsg;
    GPS_Ack_State_t              AckState;
    uint16                       AckWaitingMsg;
    uint8                        AckRcvdMsgCls;
    boolean                      AckWaitingRcvd;
    /*! The shared data mutex */
    //uint32                       MutexDump;
    uint32                       MutexPosition;
    uint32                       MutexSatInfo;
    /*! */
    uint32                       RateCountVel;
    uint32                       RateCountLatLon;
    boolean                      GotPosllh;
    boolean                      GotVelned;
    /*! Flag to start and stop streaming */
    boolean                      ContinueFlag;
    /*! The last timestamp */
    //CFE_TIME_SysTime_t           LastTimeStamp;
    uint32						  TaskFlags;
} GPS_AppCustomData_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Determines if the maximum of event filters has been reached.
**
**  \par Description
**       This function checks if an index has reached the maximum
**       number of events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]    ind    The current index to check.
**                             
**
**  \returns    boolean
**
*************************************************************************/
boolean GPS_Custom_Max_Events_Not_Reached(int32 ind);

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


#ifdef __cplusplus
}
#endif 

#endif /* GPS_CUSTOM_SHARED_H */
