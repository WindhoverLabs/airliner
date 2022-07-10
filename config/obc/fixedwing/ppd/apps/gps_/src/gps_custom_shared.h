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
    int                           DeviceFd;
    /*! The current device status */
    GPS_Custom_Status_t           Status;
    /*! The current baud */
    uint32                        Baud;
    /*! The current parser status */
    GPS_ParserStatus_t            ParserStatus;
    /*! The current parser message */
    GPS_DeviceMessage_t           Message;
    /*! Streaming task priority */
    uint8                         Priority;
    /*! Streaming child task identifier */
    uint32                        ChildTaskID;
    /*! Streaming task function pointer */
    CFE_ES_ChildTaskMainFuncPtr_t StreamingTask;
    //PX4_GpsDumpMsg_t             GpsDumpMsg;
    PX4_VehicleGpsPositionMsg_t   GpsPositionMsg;
    PX4_SatelliteInfoMsg_t        GpsSatInfoMsg;
    GPS_Ack_State_t               AckState;
    uint16                        AckWaitingMsg;
    uint8                         AckRcvdMsgCls;
    boolean                       AckWaitingRcvd;
    /*! The shared data mutex */
    //uint32                       MutexDump;
    uint32                        MutexPosition;
    uint32                        MutexSatInfo;
    /*! */
    uint32                        RateCountVel;
    uint32                        RateCountLatLon;
    boolean                       GotPosllh;
    boolean                       GotVelned;
    /*! Flag to start and stop streaming */
    boolean                       ContinueFlag;
    uint32						  TaskFlags;
} GPS_AppCustomData_t;


/************************************************************************
** External Global Variables
*************************************************************************/
extern GPS_AppCustomData_t GPS_AppCustomData;

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


#ifdef __cplusplus
}
#endif 

#endif /* GPS_CUSTOM_SHARED_H */
