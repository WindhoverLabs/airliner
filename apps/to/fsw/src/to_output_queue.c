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

#include "to_app.h"
#include "to_output_queue.h"
#include "to_custom.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Buildup a channel output queue                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputQueue_Buildup(TO_ChannelData_t* channel)
{
    int32 status = OS_SUCCESS;

    /*
     * Now that we have the new table, create all the new resources we need
     * starting with queues for the channels and priority queues.
     */
    char QueueName[OS_MAX_API_NAME];
    snprintf(QueueName, OS_MAX_API_NAME, "TO_%s_OUT", (const char*)&channel->ChannelName[0]);
    
    status = OS_QueueCreate(&channel->OutputQueue.OSALQueueID,
                            QueueName,
                            TO_OUTPUT_QUEUE_DEPTH,
                            sizeof(CFE_SB_Msg_t*),
                            0);
            
    if (status != OS_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_CONFIG_TABLE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create '%s' output channel queue for channel %d. err=%ld",
                                 channel->ChannelName, channel->channelIdx, status);

        channel->OutputQueue.OSALQueueID = OS_MAX_QUEUES;
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Teardown channel output queue in preparation for a              */
/* reconfiguration.                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputQueue_Teardown(TO_ChannelData_t *channel)
{
    int32 status = CFE_SUCCESS;
    int32 putStatus = 0;
    void *buffer = NULL;
    uint32 nBytesCopied = 0;    

    if (channel->OutputQueue.OSALQueueID != OS_MAX_QUEUES)
    {
        while (OS_SUCCESS == status)
        {
            status =  OS_QueueGet(
                    channel->OutputQueue.OSALQueueID,
                    &buffer, sizeof(buffer), &nBytesCopied, OS_CHECK);
            if (OS_SUCCESS == status)
            {
                putStatus = CFE_ES_PutPoolBuf(channel->MemPoolHandle, (uint32*)buffer);
                if (putStatus < 0)
                {
                    /* Failed to return memory back to memory pool.  Not much we can do but report it
                     * and keep processing the queue until its empty.
                     */
                    (void) CFE_EVS_SendEvent(TO_PUT_POOL_ERR_EID,
                                             CFE_EVS_ERROR,
                                             "Failed to return message back to memory pool on output queue teardown, channel %d. (%d)",
                                             (unsigned int)channel->channelIdx, 
                                             (int)putStatus);
                } else {
                    /* Since status is positive, it is safe to cast */
                    channel->MemInUse -= (uint32)putStatus;
                }
            }
      
        }
    
        if (status != OS_QUEUE_EMPTY)
        {
            /* The OSAL failed to pop all the messages off.  Not much we can do.  Report it and keep going. */
            (void) CFE_EVS_SendEvent(TO_OSQUEUE_GET_ERROR_EID,
                                     CFE_EVS_ERROR,
                                     "Failed to pop all messages from channel '%s' (channel id = %d) output queue. (%d)",
                                     channel->ChannelName, 
                                     (unsigned int)channel->channelIdx, 
                                     (int)status);
        }
    }

    status = TO_OutputChannel_CustomTeardown(channel->channelIdx);

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset all dynamic metrics.                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_OutputQueue_ResetCounts(TO_ChannelData_t *channel)
{
    channel->OutputQueue.SentCount = 0;
    channel->OutputQueue.HighwaterMark = 0;
    channel->OutputQueue.SentBytes = 0;
    channel->OutputQueue.CurrentlyQueuedCnt = 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Push a message on the output channel queue.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputQueue_QueueMsg(TO_ChannelData_t *channel, CFE_SB_MsgPtr_t MsgPtr)
{
    int32 status = 0;

    status = OS_QueuePut(channel->OutputQueue.OSALQueueID, &MsgPtr, sizeof(MsgPtr), 0);
    if (OS_QUEUE_FULL == status)
    {
        /* This is not supposed to happen since we only queue when the channel
         * is not full, but trap it anyway.  Since the queue is full and the
         * message will not be sent, deallocate the memory allocated as it 
         * won't be needed
         */
        status = CFE_ES_PutPoolBuf(channel->MemPoolHandle, (uint32 *)MsgPtr);
        if (status < 0)
        {
            (void) CFE_EVS_SendEvent(TO_PUT_POOL_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "PutPoolBuf: channel=%d, error=%i",
                                     (unsigned int)channel->channelIdx, 
                                     (int)status);

            return status;
        }
        
        /* Since status is positive, it is safe to cast */
        channel->MemInUse -= (uint32)status;
    }
    else if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_OSQUEUE_PUT_ERROR_EID,
                                 CFE_EVS_ERROR,
                                 "OS_QueuePut failed: channel=%d size=%u error=%i",
                                 (unsigned int)channel->channelIdx, 
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
osalbool TO_OutputChannel_Query(uint16 ChannelIdx)
{
    TO_ChannelData_t *channel = NULL;

    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_OUT_CH_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index (index = %d, max = %d).",
                                 (unsigned int)ChannelIdx, 
                                 TO_MAX_CHANNELS);

        return FALSE;
    }
    channel = &TO_AppData.ChannelData[ChannelIdx];   

    /* Next, see if the channel is open. */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_OUT_CH_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %d not open.", 
                                 (unsigned int)ChannelIdx);

        return FALSE;
    }

    (void) CFE_EVS_SendEvent(TO_OUT_CH_INFO_EID,
                             CFE_EVS_INFORMATION,
                             "CHANNEL=%d S=%d ML=%i SC=%u CQC=%d HWM=%d",
                             (unsigned int)ChannelIdx,
                             (unsigned int)channel->State,
                             TO_OUTPUT_QUEUE_DEPTH,
                             (unsigned int)channel->OutputQueue.SentCount,
                             (unsigned int)channel->OutputQueue.CurrentlyQueuedCnt,
                             (unsigned int)channel->OutputQueue.HighwaterMark);
    return TRUE;
}
