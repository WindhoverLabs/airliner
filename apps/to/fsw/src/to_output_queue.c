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

#include "to_platform_cfg.h"
#include "to_app.h"
#include "to_output_queue.h"
#include "to_custom.h"
#include "cfe_evs_msg.h"
#include "stddef.h"

uint32  TO_MemPoolDefSize[TO_MAX_MEMPOOL_BLK_SIZES] =
{
    TO_MAX_BLOCK_SIZE,
    TO_MEM_BLOCK_SIZE_07,
    TO_MEM_BLOCK_SIZE_06,
    TO_MEM_BLOCK_SIZE_05,
    TO_MEM_BLOCK_SIZE_04,
    TO_MEM_BLOCK_SIZE_03,
    TO_MEM_BLOCK_SIZE_02,
    TO_MEM_BLOCK_SIZE_01
};


int32 TO_OutputQueue_Init(TO_ChannelData_t* channel)
{
    int32 status;

    /* Initialize the memory pool */
    status = CFE_ES_PoolCreateEx(&channel->OutputQueue.MemPoolHandle,
    		      channel->OutputQueue.MemPoolBuffer,
                  TO_NUM_BYTES_IN_MEM_POOL,
                  TO_MAX_MEMPOOL_BLK_SIZES,
                  &TO_MemPoolDefSize[0],
                  CFE_ES_USE_MUTEX);
    if (status != CFE_SUCCESS)
    {
        /* This is a critical error for this channel.  No sense in continuing.  Destroy
         * the counting semaphore, and delete OSAL queue before returning the error
         * back to the caller.
         */

        (void) CFE_EVS_SendEvent(TO_CR_POOL_ERR_EID,
                                 CFE_EVS_ERROR,
                "Error creating memory pool (0x%08X) for %s",(unsigned int)status,
				channel->ChannelName);
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Buildup a channel output queue                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputQueue_Buildup(TO_ChannelData_t* channel, const char *CfCntSemName, uint32 CfCntSemMax)
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
                                 "Failed to create '%s' output channel queue for channel %lu. err=%ld",
                                 channel->ChannelName, channel->channelIdx, status);

        channel->OutputQueue.OSALQueueID = OS_MAX_QUEUES;
    }

    /* Set up CF Throttling semaphore */
    channel->OutputQueue.CfCntSemMax = CfCntSemMax;
    strncpy(channel->OutputQueue.CfCntSemName, CfCntSemName,
            sizeof(channel->OutputQueue.CfCntSemName));
    channel->OutputQueue.CfCntSemName[OS_MAX_API_NAME - 1] =  '\0';

    /* Initialize CF Counting Sem to CfCntSemMax */
    /* NOTE: This will fail if two channels attempt to use the same
     * CF channel. */
    status = OS_CountSemCreate(&channel->OutputQueue.CfCntSemId,
                                channel->OutputQueue.CfCntSemName,
                                channel->OutputQueue.CfCntSemMax, 0);
    if (status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID,
                          CFE_EVS_ERROR,
                          "Failed to create counting semaphore "
                          "for CF channel semaphore:%s for TO channel %s . "
                          "(OSAL Error:%d)",
                          channel->OutputQueue.CfCntSemName,
						  channel->ChannelName, (int)status);

        return status;
    }

    channel->OutputQueue.CurrentlyQueuedCnt = 0;
    channel->OutputQueue.MsgScratchPad = 0;

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

    /* Delete the scratch message, if there is one. */
    if(channel->OutputQueue.MsgScratchPad != 0)
    {
        putStatus = CFE_ES_PutPoolBuf(channel->OutputQueue.MemPoolHandle, (uint32*)channel->OutputQueue.MsgScratchPad);
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
        }

        channel->OutputQueue.MsgScratchPad = 0;
    }

    if (channel->OutputQueue.OSALQueueID != OS_MAX_QUEUES)
    {
        while (OS_SUCCESS == status)
        {
            status =  OS_QueueGet(
                    channel->OutputQueue.OSALQueueID,
                    &buffer, sizeof(buffer), &nBytesCopied, OS_CHECK);
            if (OS_SUCCESS == status)
            {
                putStatus = CFE_ES_PutPoolBuf(channel->OutputQueue.MemPoolHandle, (uint32*)buffer);
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
                    channel->OutputQueue.MemInUse -= (uint32)putStatus;
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

    (void) OS_CountSemDelete(channel->OutputQueue.CfCntSemId);

    channel->OutputQueue.CurrentlyQueuedCnt = 0;

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
    channel->OutputQueue.PeakMemInUse = 0;
    channel->OutputQueue.MemFullCount = 0;
}


osalbool TO_OutputQueue_IsFull(TO_ChannelData_t *channel)
{
    osalbool isFull = FALSE;

    if(channel->OutputQueue.CurrentlyQueuedCnt >= TO_OUTPUT_QUEUE_DEPTH)
    {
    	isFull = TRUE;
    }

    return isFull;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the CFE EVS Event Message length                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 TO_GetEventMsgLength(CFE_EVS_Packet_t *msgPtr);



int32 TO_OutputQueue_GetMsg(TO_ChannelData_t *channel, CFE_SB_MsgPtr_t *MsgPtr, int32 Timeout )
{
	int32  status = -1;
    uint32 nBytesCopied = 0;
    int32  putStatus;

    TO_Channel_LockByRef(channel);
    /* Delete the scratch message, if there is one. */
    if(channel->OutputQueue.MsgScratchPad != 0)
    {
        putStatus = CFE_ES_PutPoolBuf(channel->OutputQueue.MemPoolHandle, (uint32*)channel->OutputQueue.MsgScratchPad);
        if (putStatus < 0)
        {
            /* Failed to return memory back to memory pool.  Not much we can do but report it
             * and keep processing the queue until its empty.
             */
            (void) CFE_EVS_SendEvent(TO_PUT_POOL_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Failed to return message back to memory pool on TO_OutputQueue_GetMsg, channel %d. (%d)",
                                     (unsigned int)channel->channelIdx,
                                     (int)putStatus);
        }
        else
        {
            /* Since status is positive, it is safe to cast */
            channel->OutputQueue.MemInUse -= (uint32)putStatus;
        }

        channel->OutputQueue.MsgScratchPad = 0;
    }

    if (channel->OutputQueue.OSALQueueID != OS_MAX_QUEUES)
    {
        TO_Channel_UnlockByRef(channel);
        status = OS_QueueGet(
                    channel->OutputQueue.OSALQueueID,
					&channel->OutputQueue.MsgScratchPad, sizeof(channel->OutputQueue.MsgScratchPad), &nBytesCopied, Timeout);

        if(OS_SUCCESS == status)
        {
        	uint16 msgID;

            TO_Channel_LockByRef(channel);

   		    *MsgPtr = channel->OutputQueue.MsgScratchPad;

            if(channel->OutputQueue.CurrentlyQueuedCnt >= 1)
            {
                channel->OutputQueue.CurrentlyQueuedCnt--;
            }

            msgID = CFE_SB_GetMsgId(*MsgPtr);

            /* Check if this is an event message. */
            if (CFE_EVS_EVENT_MSG_MID == msgID)
            {
            	/* It is an event. Adjust the size accordingly. */
            	uint32 eventMsgSize = TO_GetEventMsgLength((CFE_EVS_Packet_t *)*MsgPtr);

                CFE_SB_SetTotalMsgLength((CFE_SB_MsgPtr_t)*MsgPtr, eventMsgSize);
            }

            TO_Channel_UnlockByRef(channel);
        }
    }

    return status;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Push a message on the output channel queue.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputQueue_QueueMsg(TO_ChannelData_t *channel, CFE_SB_MsgPtr_t MsgPtr)
{
    int32  status = 0;
    uint32 *copyBuffer = NULL;
    uint32 bufferSize;

    /* Allocate a chunk of memory from the memory pool to store the message
     * copy.
     */
    bufferSize = CFE_SB_GetTotalMsgLength(MsgPtr);
    status = CFE_ES_GetPoolBuf(&copyBuffer,
                                channel->OutputQueue.MemPoolHandle, bufferSize);
    if ((status < 0) || (copyBuffer == NULL))
    {
        /* The allocation failed.  There's nothing we can do.  Rather than
         * treat this as a critical failure, we're going to keep going and try
         * processing other messages.  We also aren't going to send an event.
         * Sending an event may make this problem worse since we're
         * potentially adding more messages to an already full buffer.  So,
         * let's just increment a counter, and let the caller know the queue
         * failed.
         */
        TO_Channel_LockByRef(channel);
        channel->OutputQueue.MemFullCount++;
        TO_Channel_UnlockByRef(channel);
        return TO_MEMORY_FULL_ERR;
    }
    else
    {
        TO_Channel_LockByRef(channel);
        channel->OutputQueue.MemInUse += (uint32)status;
        if (channel->OutputQueue.MemInUse > channel->OutputQueue.PeakMemInUse)
        {
            channel->OutputQueue.PeakMemInUse = channel->OutputQueue.MemInUse;
        }
        TO_Channel_UnlockByRef(channel);
    }

    /* Copy the message into the temp buffer. */
    (void) CFE_PSP_MemCpy(copyBuffer, MsgPtr, bufferSize);

    status = OS_QueuePut(channel->OutputQueue.OSALQueueID, &copyBuffer, sizeof(copyBuffer), 0);
    if (OS_QUEUE_FULL == status)
    {
        /* This is not supposed to happen since we only queue when the channel
         * is not full, but trap it anyway.  Since the queue is full and the
         * message will not be sent, deallocate the memory allocated as it 
         * won't be needed
         */
        status = CFE_ES_PutPoolBuf(channel->OutputQueue.MemPoolHandle, (uint32 *)copyBuffer);
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
        TO_Channel_LockByRef(channel);
        channel->OutputQueue.MemInUse -= (uint32)status;
        TO_Channel_UnlockByRef(channel);
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
    else
    {
        TO_Channel_LockByRef(channel);
    	channel->OutputQueue.CurrentlyQueuedCnt++;

        if (channel->OutputQueue.HighwaterMark < channel->OutputQueue.CurrentlyQueuedCnt)
        {
        	channel->OutputQueue.HighwaterMark = channel->OutputQueue.CurrentlyQueuedCnt;
        }

        channel->OutputQueue.QueuedMsgCount++;

        TO_Channel_UnlockByRef(channel);
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



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the CFE EVS Event Message length                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 TO_GetEventMsgLength(CFE_EVS_Packet_t *msgPtr)
{
    uint32 msgLen;
    uint32 textStart = offsetof(CFE_EVS_Packet_t, Payload) + offsetof(CFE_EVS_Packet_Payload_t, Message);
    uint32 textLength = strnlen(msgPtr->Payload.Message, CFE_EVS_MAX_MESSAGE_LENGTH);

    msgLen = textStart + textLength + 1;

    return msgLen;
}
