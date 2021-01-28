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

#ifndef RCIN_UDP_H
#define RCIN_UDP_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "rcin_custom.h"
#include "px4_msgs.h"

/************************************************************************
** Local Defines
*************************************************************************/
/** \brief UDP port to receive RC input. */
#define RCIN_UDP_PORT                     (777)

/** \brief The channel count. */
#define RCIN_CHANNEL_COUNT                (8)

/** \brief RCIN UDP protocol version. */
#define RCIN_UDP_VERSION                  (2)


/* Timeout settings */
/** \brief Wait time for data in seconds.
**
**  \par Limits:
**       None.
*/
#define RCIN_BUFFER_FILL_TIMEOUT_SEC    (0)

/** \brief Wait time for data in microseconds.
**
**  \par Limits:
**       None.
*/
#define RCIN_BUFFER_FILL_TIMEOUT_USEC   (13000)

/** \brief Sleep time micro seconds for interrupted calls.
**
**  \par Limits:
**       None.
*/
#define RCIN_MAX_RETRY_SLEEP_USEC       (10)

/** \brief Streaming task priority
**
**  \par Limits:
**       0 to MAX_PRIORITY (usually 255)
*/
#define RCIN_STREAMING_TASK_PRIORITY    (85)

/** \brief RCIN shared data mutex name. */
#define RCIN_MUTEX_NAME                "RCIN_MUTEX"

/** \brief Streaming task name
**
**  \par Limits:
**       OS_MAX_API_NAME
*/
#define RCIN_STREAMING_TASK_NAME       "RCIN_STREAM"

/** \brief Maximum amount of errors before going to failsafe. */
#define RCIN_MAX_ERROR_COUNT            (25)

/************************************************************************
** Structure Declarations
*************************************************************************/

/**
 * \brief RCIN UDP protocol packet.
 */
#pragma pack(push, 1)
typedef struct
{
    uint32 version;
    uint64 timestamp_us;
    uint16 sequence;
    uint16 pwms[RCIN_CHANNEL_COUNT];
} RCIN_UDP_Packet_t;
#pragma pack(pop)


/**
 * \brief Device status
 */
typedef enum
{
    /*! Status uninitialized */
    RCIN_CUSTOM_UNINITIALIZED   = 0,
    /*! Status initialized */
    RCIN_CUSTOM_INITIALIZED     = 1,
    /*! Status enabled */
    RCIN_CUSTOM_ENABLED         = 2,
    /*! Status not streaming */
    RCIN_CUSTOM_NOTSTREAMING    = 3,
    /*! Status streaming */
    RCIN_CUSTOM_STREAMING       = 4,
    /*! Status RC Lost */
    RCIN_CUSTOM_RC_LOST         = 5
} RCIN_Custom_Status_t;


typedef struct
{
    /*! Socket descriptor */
    int                             Socket;
    /*! UDP port */
    uint16                          Port;
    /*! The current device status */
    RCIN_Custom_Status_t            Status;
    /*! Flag to start and stop streaming */
    boolean                         ContinueFlag;
    /*! Streaming task priority */
    uint8                           Priority;
    /*! Streaming child task identifier */
    uint32                          ChildTaskID;
    /*! Streaming task function pointer */
    CFE_ES_ChildTaskMainFuncPtr_t   StreamingTask;
    /*! RCInput message for storing SBUS data */
    PX4_InputRcMsg_t                Measure;
    /*! The shared data mutex */
    uint32                          Mutex;
    uint64                          Last_Packet_Timestamp;
    uint16                          Last_Packet_Sequence;
} RCIN_AppCustomData_t;


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/
/************************************************************************/
/** \brief Attempt to read from the udp socket.
**
**  \par Description
**       Attempts to read and parse the rcin udp socket.
**
*************************************************************************/
void RCIN_Custom_Read(void);


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
boolean RCIN_Custom_Max_Events_Not_Reached(int32 ind);


/************************************************************************/
/** \brief The stream task that actively reads the RCIN input stream.
**
**  \par Description
**       This thread runs until uninit.
**
*************************************************************************/
void RCIN_Stream_Task(void);


/************************************************************************/
/** \brief Called if the input stream is out of sync or a device read
**         error occurs.
**
**  \par Description
**       This function keeps an internal counter that is incremented 
**       with every call. If the max error count is reached the failsafe
**       and RC lost booleans in the RC_Input message are set.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]    notReset    TRUE to not reset the counter, FALSE to 
**                             reset the counter.
**
*************************************************************************/
void RCIN_Custom_RC_Lost(boolean notReset);


#ifdef __cplusplus
}
#endif 

#endif /* RCIN_UDP_H */
