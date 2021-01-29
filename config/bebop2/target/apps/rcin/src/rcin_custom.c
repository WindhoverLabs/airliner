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
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "rcin_udp.h"
#include "rcin_events.h"
#include "perf_ids.h"
#include "px4lib.h"
#include "rcin_platform_cfg.h"
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

typedef enum {

/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    RCIN_DEVICE_ERR_EID = RCIN_EVT_CNT,
/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the RC input stream is out of 
**  sync.
**
*/
    RCIN_SYNC_ERR_EID,

/** \brief <tt> 'RCIN - ' </tt>
**  \event <tt> 'RCIN - ' </tt>
**  
**  \par Type: Info
**
**  \par Cause:
**
**  This event message is issued for device info events.
**
*/
    RCIN_DEVICE_INF_EID,

/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    RCIN_CUSTOM_EVT_CNT

} RCIN_CustomEventIds_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
RCIN_AppCustomData_t RCIN_AppCustomData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/


void RCIN_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&RCIN_AppCustomData, sizeof(RCIN_AppCustomData));
    /* Set all non-zero values */
    RCIN_AppCustomData.ContinueFlag          = TRUE;
    RCIN_AppCustomData.Priority              = RCIN_STREAMING_TASK_PRIORITY;
    RCIN_AppCustomData.StreamingTask         = RCIN_Stream_Task;

    RCIN_AppCustomData.Socket                = 0;
    RCIN_AppCustomData.Port                  = 777;

    /* Initialize for failsafe and RcLost */
    RCIN_AppCustomData.Measure.RcFailsafe    = 1;
    RCIN_AppCustomData.Measure.RcLost        = 1;
    return;
}


boolean RCIN_Custom_Init(void)
{
    boolean returnBool = TRUE;
    int32 returnCode   = 0;
    int reuseaddr = 1;
    struct sockaddr_in address;
    struct sockaddr_in serial_address;

    if((RCIN_AppCustomData.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "Socket errno: %i", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    setsockopt(RCIN_AppCustomData.Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(RCIN_AppCustomData.Port);

    if ( (bind(RCIN_AppCustomData.Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,"Bind socket failed = %d", errno);
        returnBool = FALSE;
        goto end_of_function;
    }

    CFE_EVS_SendEvent(RCIN_DEVICE_INF_EID, CFE_EVS_INFORMATION,
        "RCIN UDP command input enabled on port %u.", RCIN_AppCustomData.Port);

    RCIN_AppCustomData.Status = RCIN_CUSTOM_INITIALIZED;

    /* Create mutex for shared data */
    returnCode = OS_MutSemCreate(&RCIN_AppCustomData.Mutex, RCIN_MUTEX_NAME, 0);
    if (returnCode != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN mutex create failed in custom init");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Create streaming task */
    /* Set loop flag to continue forever */
    RCIN_AppCustomData.ContinueFlag = TRUE;
    
        /* Create the streaming task */
    returnCode = CFE_ES_CreateChildTask(
        &RCIN_AppCustomData.ChildTaskID,
        RCIN_STREAMING_TASK_NAME,
        RCIN_AppCustomData.StreamingTask,
        0,
        CFE_ES_DEFAULT_STACK_SIZE,
        RCIN_AppCustomData.Priority,
        RCIN_STREAMING_TASK_FLAGS);
        
    if(CFE_SUCCESS != returnCode)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
        returnBool = FALSE;
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN create streaming task failed");
        goto end_of_function;
    }
    else
    {
        RCIN_AppCustomData.Status = RCIN_CUSTOM_ENABLED;
    }

end_of_function:
    return (returnBool);
}


void RCIN_Stream_Task(void)
{
    int32 returnCode     = 0;
    int32 timeouts       = 0;
    uint32 maxFd         = 0;
    uint32 retryAttempts = 0;
    uint32 iStatus       = -1;
    fd_set fds;
    struct timeval timeValue;

    iStatus = CFE_ES_RegisterChildTask();
    
    if (iStatus == CFE_SUCCESS)
    {
        /* Task should continue forever until cleanup (uninit) */
        while (RCIN_AppCustomData.ContinueFlag == TRUE)
        {
            maxFd = 0;
            returnCode = 0;
            
            /* Select modifies the timeout value with time left until 
             * the timeout would expire so timeValue needs to be set
             * every loop iteration
             */
            timeValue.tv_sec = RCIN_BUFFER_FILL_TIMEOUT_SEC;
            timeValue.tv_usec = RCIN_BUFFER_FILL_TIMEOUT_USEC;

            /* Initialize the set */
            FD_ZERO(&fds);

            FD_SET(RCIN_AppCustomData.Socket, &fds);
            
            /* Get the greatest fd value for select() */
            maxFd = RCIN_AppCustomData.Socket; 

            CFE_ES_PerfLogEntry(RCIN_DEVICE_GET_PERF_ID);
            /* Wait for RC data */
            returnCode = select(maxFd + 1, &fds, 0, 0, &timeValue);
            CFE_ES_PerfLogExit(RCIN_DEVICE_GET_PERF_ID);

            /* select() wasn't successful */
            if (-1 == returnCode)
            {
                /* select was interrupted, try again */
                if (EINTR == errno)
                {
                    RCIN_Custom_RC_Lost(TRUE);
                    (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RCIN select was interrupted");
                    usleep(RCIN_MAX_RETRY_SLEEP_USEC);
                    continue;
                }
                else
                {
                    RCIN_Custom_RC_Lost(TRUE);
                    /* select returned an error other than EINTR */
                    (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                        "RCIN stream failed select() returned %i", errno);
                    usleep(RCIN_MAX_RETRY_SLEEP_USEC);
                    continue;
                }
            }
            /* select timed out */
            if (0 == returnCode)
            {
                RCIN_Custom_RC_Lost(TRUE);
                continue;
            } 
            /* select() returned and data is ready to be read */
            if(returnCode > 0)
            {
                RCIN_Custom_RC_Lost(FALSE);
                RCIN_Custom_Read();
                continue;
            }
        } /* end while loop */
    } /* end if status == success */

    /* Take shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);
    /* Set failsafe and RcLost */
    RCIN_AppCustomData.Measure.RcFailsafe    = 1;
    RCIN_AppCustomData.Measure.RcLost        = 1;
    /* Give shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);

    /* Streaming task is exiting so set app flag to initialized */
    RCIN_AppCustomData.Status = RCIN_CUSTOM_ENABLED;
    CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
        "RCIN receive task exited with return code %li task status (0x%08lX)",
        returnCode, iStatus);

    /* The child task was successfully registered so exit from it */
    if (iStatus == CFE_SUCCESS)
    {
        CFE_ES_ExitChildTask();
    }
}



void RCIN_Custom_RC_Lost(boolean notReset)
{
    static int errorCount = 0;

    /* Take shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    if (FALSE == notReset)
    {
        /* Set status to streaming state */
        RCIN_AppCustomData.Status = RCIN_CUSTOM_STREAMING;
        errorCount = 0;
    }
    else
    {
        /* Set status to not streaming if we're not in RC Lost*/
        if (RCIN_AppCustomData.Status != RCIN_CUSTOM_RC_LOST 
            && RCIN_AppCustomData.Status != RCIN_CUSTOM_NOTSTREAMING)
        {
            RCIN_AppCustomData.Status = RCIN_CUSTOM_NOTSTREAMING;
        }
        errorCount++;
    }

    if(RCIN_MAX_ERROR_COUNT == errorCount)
    {
        /* We're at max error count, set failsafe and lost flags */
        RCIN_AppCustomData.Measure.RcFailsafe = TRUE;
        RCIN_AppCustomData.Measure.RcLost = TRUE;

        if (RCIN_AppCustomData.Status != RCIN_CUSTOM_RC_LOST)
        {
            CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN RC LOST, setting failsafe and lost flags");
            RCIN_AppCustomData.Status = RCIN_CUSTOM_RC_LOST;
        }
        errorCount = 0;
    }
    /* Give shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);
    return;
}


boolean RCIN_Custom_Measure(PX4_InputRcMsg_t *Measure)
{
    boolean returnBool    = TRUE;
    uint8 i               = 0;

    /* Null pointer check */
    if (0 == Measure)
    {
        (void) CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "RCIN device read null pointer error");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Take the shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    if(RCIN_CUSTOM_STREAMING != RCIN_AppCustomData.Status)
    {
        returnBool = FALSE;
    }
    
    if(RCIN_AppCustomData.Measure.RcFailsafe == TRUE &&
       RCIN_AppCustomData.Measure.RcLost == TRUE)
    {
        returnBool = FALSE;
    }

    Measure->Timestamp = RCIN_AppCustomData.Measure.Timestamp;
    Measure->ChannelCount = RCIN_AppCustomData.Measure.ChannelCount;
    Measure->RSSI = RCIN_AppCustomData.Measure.RSSI;
    Measure->RcLostFrameCount = RCIN_AppCustomData.Measure.RcLostFrameCount;
    Measure->RcTotalFrameCount = RCIN_AppCustomData.Measure.RcTotalFrameCount;
    Measure->RcPpmFrameLength = RCIN_AppCustomData.Measure.RcPpmFrameLength;
    for(i = 0; i<18; i++)
    {
        Measure->Values[i] = RCIN_AppCustomData.Measure.Values[i];
    }
    Measure->RcFailsafe = RCIN_AppCustomData.Measure.RcFailsafe;
    Measure->RcLost = RCIN_AppCustomData.Measure.RcLost;
    Measure->InputSource = RCIN_AppCustomData.Measure.InputSource;

    /* Give the shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);

end_of_function:
    return (returnBool);
}


boolean RCIN_Custom_Uninit(void)
{
    return close(RCIN_AppCustomData.Socket);
}


boolean RCIN_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;

    if ((ind < RCIN_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 RCIN_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == RCIN_Custom_Max_Events_Not_Reached(customEventCount + 2))
    {
        EventTbl[  customEventCount].EventID = RCIN_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
        EventTbl[  customEventCount].EventID = RCIN_SYNC_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = -1;
        goto end_of_function;
    }

end_of_function:

    return (customEventCount);
}


void RCIN_Custom_Read(void)
{
    struct sockaddr_in si_other;
    int recv_len = 0;
    int slen = sizeof(si_other);
    RCIN_UDP_Packet_t packet = {0};
    boolean failsafe_flag = FALSE;
    boolean lost_flag     = FALSE;
    uint8 i               = 0;

    recv_len = recv(RCIN_AppCustomData.Socket, &packet, sizeof(packet), 0);

    /* Partial socket read. */
    if(recv_len != sizeof(packet))
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN partial socket read");
        failsafe_flag = TRUE;
        lost_flag     = TRUE;
    }

    /* Version check. */
    if(packet.version != RCIN_UDP_VERSION)
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN bad protocol version. ");
    }

    /* Timestamp check */
    if(RCIN_AppCustomData.Last_Packet_Timestamp !=0 &&
       (packet.timestamp_us - RCIN_AppCustomData.Last_Packet_Timestamp) > 100000)
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN timestamp delay exceeded.");
        failsafe_flag = TRUE;
        lost_flag     = TRUE;
    }
    RCIN_AppCustomData.Last_Packet_Timestamp = packet.timestamp_us;

    if((RCIN_AppCustomData.Last_Packet_Sequence - packet.sequence) > 10)
    {
        CFE_EVS_SendEvent(RCIN_DEVICE_ERR_EID, CFE_EVS_ERROR,
                    "RCIN large gap in commands.");
        failsafe_flag = TRUE;
        lost_flag     = TRUE;
    }
    RCIN_AppCustomData.Last_Packet_Sequence = packet.sequence;


    /* Take the shared data mutex */
    OS_MutSemTake(RCIN_AppCustomData.Mutex);

    /* Timestamp */
    RCIN_AppCustomData.Measure.Timestamp = PX4LIB_GetPX4TimeUs();

    /* Channel count */
    RCIN_AppCustomData.Measure.ChannelCount = RCIN_CHANNEL_COUNT;
    RCIN_AppCustomData.Measure.RSSI = 100;
    RCIN_AppCustomData.Measure.RcLostFrameCount = 0;
    RCIN_AppCustomData.Measure.RcTotalFrameCount = 1;
    RCIN_AppCustomData.Measure.RcPpmFrameLength = 100;
    RCIN_AppCustomData.Measure.RcFailsafe = failsafe_flag;
    RCIN_AppCustomData.Measure.RcLost = lost_flag;
    /* TODO */
    RCIN_AppCustomData.Measure.InputSource = PX4_RC_INPUT_SOURCE_PX4FMU_SBUS;

    /* Copy PWM values. */
    for( i = 0; i < RCIN_CHANNEL_COUNT; ++i)
    {
        RCIN_AppCustomData.Measure.Values[i] = packet.pwms[i];
    }

    /* Give the shared data mutex */
    OS_MutSemGive(RCIN_AppCustomData.Mutex);
}
