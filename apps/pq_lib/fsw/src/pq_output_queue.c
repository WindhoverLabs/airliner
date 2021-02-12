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

#include "pq_output_queue.h"
#include "pq_events.h"
#include "pq_structs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Buildup a channel output queue                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_OutputQueue_Buildup(PQ_ChannelData_t* Channel)
{
    int32 status = OS_SUCCESS;

    if (NULL == Channel)
    {
        CFE_EVS_SendEvent(PQ_NULL_POINTER_ERR_EID,
                      CFE_EVS_ERROR,
                      "Null pointer in PQ_OutputQueue_Buildup");
        return -1;
    }

    /*
     * Now that we have the new table, create all the new resources we need
     * starting with queues for the channels and priority queues.
     */
    char QueueName[OS_MAX_API_NAME];
    snprintf(QueueName, OS_MAX_API_NAME, "PQ_%s_OUT", (const char*)&Channel->ChannelName[0]);
    
    status = OS_QueueCreate(&Channel->OutputQueue.OSALQueueID,
                            QueueName,
                            PQ_OUTPUT_QUEUE_DEPTH,
                            sizeof(CFE_SB_Msg_t*),
                            0);
            
    if (status != OS_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create '%s' output channel queue for channel %lu. err=%ld",
                                 Channel->ChannelName, Channel->channelIdx, status);

        Channel->OutputQueue.OSALQueueID = OS_MAX_QUEUES;
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Teardown channel output queue in preparation for a              */
/* reconfiguration.                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_OutputQueue_Teardown(PQ_ChannelData_t *Channel)
{
    int32 status = CFE_SUCCESS;
    int32 putStatus = 0;
    void *buffer = NULL;
    uint32 nBytesCopied = 0;    

    if (NULL == Channel)
    {
        CFE_EVS_SendEvent(PQ_NULL_POINTER_ERR_EID,
                      CFE_EVS_ERROR,
                      "Null pointer in PQ_OutputQueue_Teardown");
        return -1;
    }

    if (Channel->OutputQueue.OSALQueueID != OS_MAX_QUEUES)
    {
        while (OS_SUCCESS == status)
        {
            status =  OS_QueueGet(
                    Channel->OutputQueue.OSALQueueID,
                    &buffer, sizeof(buffer), &nBytesCopied, OS_CHECK);
            if (OS_SUCCESS == status)
            {
                putStatus = CFE_ES_PutPoolBuf(Channel->MemPoolHandle, (uint32*)buffer);
                if (putStatus < 0)
                {
                    /* Failed to return memory back to memory pool.  Not much we can do but report it
                     * and keep processing the queue until its empty.
                     */
                    (void) CFE_EVS_SendEvent(PQ_PUT_POOL_ERR_EID,
                                             CFE_EVS_ERROR,
                                             "Failed to return message back to memory pool on output queue teardown, channel %lu. (%d)",
                                             Channel->channelIdx, 
                                             (int)putStatus);
                } else {
                    /* Since status is positive, it is safe to cast */
                    Channel->MemInUse -= (uint32)putStatus;
                }
            }
      
        }
    
        if (status != OS_QUEUE_EMPTY)
        {
            /* The OSAL failed to pop all the messages off.  Not much we can do.  Report it and keep going. */
            (void) CFE_EVS_SendEvent(PQ_OSQUEUE_GET_ERROR_EID,
                                     CFE_EVS_ERROR,
                                     "Failed to pop all messages from channel '%s' (channel id = %lu) output queue. (%d)",
                                     Channel->ChannelName, 
                                     Channel->channelIdx, 
                                     (int)status);
        }
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset all dynamic metrics.                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_OutputQueue_ResetCounts(PQ_ChannelData_t *Channel)
{
    if (NULL == Channel)
    {
        CFE_EVS_SendEvent(PQ_NULL_POINTER_ERR_EID,
                      CFE_EVS_ERROR,
                      "Null pointer in PQ_OutputQueue_ResetCounts");
        return;
    }

    Channel->OutputQueue.SentCount = 0;
    Channel->OutputQueue.HighwaterMark = 0;
    Channel->OutputQueue.SentBytes = 0;
    Channel->OutputQueue.CurrentlyQueuedCnt = 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Push a message on the output channel queue.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_OutputQueue_QueueMsg(PQ_ChannelData_t *Channel, CFE_SB_MsgPtr_t MsgPtr)
{
    int32 status = 0;

    if (NULL == Channel)
    {
        CFE_EVS_SendEvent(PQ_NULL_POINTER_ERR_EID,
                      CFE_EVS_ERROR,
                      "Null pointer in PQ_OutputQueue_QueueMsg");
        return -1;
    }

    status = OS_QueuePut(Channel->OutputQueue.OSALQueueID, &MsgPtr, sizeof(MsgPtr), 0);
    if (OS_QUEUE_FULL == status)
    {
        /* This is not supposed to happen since we only queue when the channel
         * is not full, but trap it anyway.  Since the queue is full and the
         * message will not be sent, deallocate the memory allocated as it 
         * won't be needed
         */
        status = CFE_ES_PutPoolBuf(Channel->MemPoolHandle, (uint32 *)MsgPtr);
        if (status < 0)
        {
            (void) CFE_EVS_SendEvent(PQ_PUT_POOL_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "PutPoolBuf: channel=%lu, error=%i",
                                     Channel->channelIdx, 
                                     (int)status);

            return status;
        }
        
        /* Since status is positive, it is safe to cast */
        Channel->MemInUse -= (uint32)status;
    }
    else if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PQ_OSQUEUE_PUT_ERROR_EID,
                                 CFE_EVS_ERROR,
                                 "OS_QueuePut failed: channel=%lu size=%u error=%i",
                                 Channel->channelIdx, 
                                 sizeof(MsgPtr), 
                                 (int)status);
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Query an output channel.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_OutputChannel_Query(PQ_ChannelData_t *Channel)
{

    if (NULL == Channel)
    {
        CFE_EVS_SendEvent(PQ_NULL_POINTER_ERR_EID,
                      CFE_EVS_ERROR,
                      "Null pointer in PQ_OutputChannel_Query");
        return FALSE;
    }

    /* Next, see if the channel is open. */
    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(PQ_OUT_CH_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu not open.",
                                 Channel->channelIdx);

        return FALSE;
    }

    (void) CFE_EVS_SendEvent(PQ_OUT_CH_INFO_EID,
                             CFE_EVS_INFORMATION,
                             "CHANNEL=%lu S=%d ML=%i SC=%u CQC=%d HWM=%d",
                             Channel->channelIdx,
                             (unsigned int)Channel->State,
                             PQ_OUTPUT_QUEUE_DEPTH,
                             (unsigned int)Channel->OutputQueue.SentCount,
                             (unsigned int)Channel->OutputQueue.CurrentlyQueuedCnt,
                             (unsigned int)Channel->OutputQueue.HighwaterMark);
    return TRUE;
}
