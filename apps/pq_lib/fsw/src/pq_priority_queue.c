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

#include "pq_priority_queue.h"
#include <string.h>
#include <stddef.h>
#include "pq_channel.h"
#include "pq_structs.h"
#include "pq_events.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the CFE EVS Event Message length                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 PQ_GetEventMsgLength(CFE_EVS_Packet_t *msgPtr);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Clear all dynamics metrics.                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_PriorityQueue_ResetCountsAll(PQ_ChannelData_t* Channel)
{
    uint32 i;

    for (i = 0; i < PQ_MAX_PRIORITY_QUEUES; ++i)
    {
        Channel->DumpTbl.PriorityQueue[i].DroppedMsgCnt = 0;
        Channel->DumpTbl.PriorityQueue[i].QueuedMsgCnt = 0;
        Channel->DumpTbl.PriorityQueue[i].HighwaterMark = 0;
        Channel->DumpTbl.PriorityQueue[i].CurrentlyQueuedCnt = 0;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Buildup all priority queues after a reconfiguration.            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_PriorityQueue_BuildupAll(PQ_ChannelData_t *Channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;

    /* First, check if the channel NULL. */
    if (NULL == Channel) 
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_MISSING_TBL_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Channel is NULL.");
        return PQ_CHANNEL_PTR_NULL;      
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create priority queues on channel %lu, missing table.",
                                 Channel->channelIdx);
        return PQ_NO_TABLE_ERR;
    }

    /*
     * Create the priority queues.
     */
    for (i = 0; i < PQ_MAX_PRIORITY_QUEUES; ++i)
    {
        if (Channel->ConfigTblPtr->PriorityQueue[i].State != PQ_PQUEUE_UNUSED)
        {
            char QueueName[OS_MAX_API_NAME];
            (void) CFE_PSP_MemSet(QueueName, 0, sizeof(QueueName));
            snprintf(QueueName, OS_MAX_API_NAME, "PQ_%s_%u", (const char*)&Channel->ChannelName[0], (uint8)i);
            status = OS_QueueCreate(
                    &Channel->DumpTbl.PriorityQueue[i].OSALQueueID,
                    QueueName,
                    Channel->ConfigTblPtr->PriorityQueue[i].MsgLimit,
                    sizeof(CFE_SB_Msg_t*),
                    0);
            if (status != OS_SUCCESS)
            {
                /* For some reason, we failed to create the queue, but this is not a critical error.
                 * Report the failure, set the queue ID to OS_MAX_QUEUES, and continue.  We set the
                 * ID to OS_MAX_QUEUES because the queue ID is a uint32 and 0 is a valid queue ID.
                 * OS_MAX_QUEUES is an invalid ID.  That will prevent us from accidentally trying
                 * to queue messages into a nonexistent queue later.
                 */
                (void) CFE_EVS_SendEvent(PQ_PQUEUE_CREATE_ERR_EID,
                                     CFE_EVS_ERROR,
                                    "Failed to create '%s' priority queue #%u on channel %lu. (%i)",
                                    Channel->ChannelName,
                                    (unsigned int)i,
                                    Channel->channelIdx,
                                    (int)status);

                Channel->DumpTbl.PriorityQueue[i].OSALQueueID = OS_MAX_QUEUES;
            }
        }
    }

    return CFE_SUCCESS;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Teardown all priority queues after a reconfiguration.           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_PriorityQueue_TeardownAll(PQ_ChannelData_t *Channel)
{
    uint32 i;
    int32 status = 0;
    int32 bytesReleased = 0;
    uint32 bufferSize = 0;
    void *buffer = NULL;

    if(NULL != Channel->ConfigTblPtr)
    {
        for(i = 0; i < PQ_MAX_PRIORITY_QUEUES; ++i)
        {
            if(Channel->ConfigTblPtr->PriorityQueue[i].State != PQ_PQUEUE_UNUSED)
            {
                if(Channel->DumpTbl.PriorityQueue[i].OSALQueueID < OS_MAX_QUEUES)
                {
                    buffer = NULL;
                    bufferSize = 0;
                    while(OS_SUCCESS == status)
                    {
                        status =  OS_QueueGet(
                                Channel->DumpTbl.PriorityQueue[i].OSALQueueID,
                                &buffer, sizeof(buffer), &bufferSize, OS_CHECK);
                        if(OS_SUCCESS == status)
                        {
                            bytesReleased = CFE_ES_PutPoolBuf(Channel->MemPoolHandle, (uint32*)buffer);
                            if(bytesReleased < 0)
                            {
                                (void) CFE_EVS_SendEvent(PQ_PQUEUE_TEARDOWN_ERR_EID,
                                                         CFE_EVS_ERROR,
                                                         "Failed to return message back to memory pool on tbl load for channel %lu. (%ld)",
                                                         Channel->channelIdx, 
                                                         (unsigned long int)bytesReleased);
                            } else {
                                PQ_Channel_LockByRef(Channel);
                                /* Since bytesReleased is positive, it is safe to cast */
                                Channel->MemInUse -= (uint32)bytesReleased;
                                PQ_Channel_UnlockByRef(Channel);
                            }
                        }
                    }

                    if(status != OS_QUEUE_EMPTY)
                    {
                        (void) CFE_EVS_SendEvent(PQ_PQUEUE_TEARDOWN_ERR_EID,
                                                 CFE_EVS_ERROR,
                                                 "Message flow failed to pop all messages from pqueue %u for channel %lu. (%d)",
                                                 (unsigned int)i,
                                                 Channel->channelIdx,
                                                 (int)status);
                    }


                    /* Queue may or may not be empty.  If we ran into an error, just press forward and delete the
                     * queue.
                     */
                    status = OS_QueueDelete(
                            Channel->DumpTbl.PriorityQueue[i].OSALQueueID);
                    if(status != OS_SUCCESS)
                    {
                        /* We ran into an error.  Send an event, but just keep going.  Nothing we can do. */
                        (void) CFE_EVS_SendEvent(PQ_PQUEUE_TEARDOWN_ERR_EID,
                                                 CFE_EVS_ERROR,
                                                 "Failed to delete priority queue %u for channel %lu. (%d)",
                                                 (unsigned int)i,
                                                 Channel->channelIdx,
                                                 (int)status);
                    }

                    Channel->DumpTbl.PriorityQueue[i].OSALQueueID = OS_MAX_QUEUES;
                }
            }
        }
    }
    else
    {
        /* No table.  Just keep going and try processing the next channel. */
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to tear down priority queues on channel %lu, missing table.",
                                 Channel->channelIdx);
    }

    return status;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Push a message on a priority queue.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_PriorityQueue_QueueMsg(
    PQ_ChannelData_t *Channel, 
    CFE_SB_MsgPtr_t MsgPtr, 
    uint32 PQueueIndex)
{
    int32 status = 0;
    uint32 bufferSize = 0;
    PQ_PriorityQueueMetrics_t* pqueueDump = NULL;
    void *PQ_CopyBuffer = NULL;
    CFE_SB_MsgId_t  DataMsgID;
    
    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_MISSING_TBL_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to queue message on channel %lu, missing table!",
                                 Channel->channelIdx);
        return PQ_NO_TABLE_ERR;
    }    

    pqueueDump = &Channel->DumpTbl.PriorityQueue[PQueueIndex];

    DataMsgID = CFE_SB_GetMsgId(MsgPtr);
    bufferSize = CFE_SB_GetTotalMsgLength(MsgPtr);
    
    /* We're going to be modifying the event message.
     * Send minimum-sized event messages.
     * Trim message before memory pool get.
     */
    if (CFE_EVS_EVENT_MSG_MID == DataMsgID)
    {
        bufferSize = PQ_GetEventMsgLength((CFE_EVS_Packet_t *)MsgPtr);
    }    
    
    /* Allocate a chunk of memory from the memory pool to store the message
     * copy.
     */
    status = CFE_ES_GetPoolBuf((uint32 **) &PQ_CopyBuffer,
                                Channel->MemPoolHandle, bufferSize);
    if ((status < 0) || (PQ_CopyBuffer == NULL))
    {
        /* The allocation failed.  There's nothing we can do.  Rather than
         * treat this as a critical failure, we're going to keep going and try
         * processing other messages.  We also aren't going to send an event.
         * Sending an event may make this problem worse since we're
         * potentially adding more messages to an already full buffer.  So,
         * let's just increment a counter, and let the caller know the queue
         * failed.
         */
        PQ_Channel_LockByRef(Channel);
        Channel->MemFullCount++;
        pqueueDump->DroppedMsgCnt++;
        PQ_Channel_UnlockByRef(Channel);
        return PQ_MEMORY_FULL_ERR;
    }

    PQ_Channel_LockByRef(Channel);
    /* Since status is positive, it is safe to cast */
    Channel->MemInUse += (uint32)status;
    if (Channel->MemInUse > Channel->PeakMemInUse)
    {
        Channel->PeakMemInUse = Channel->MemInUse;
    }
    PQ_Channel_UnlockByRef(Channel);

    /* Copy the message into the temp buffer. */
    (void) CFE_PSP_MemCpy(PQ_CopyBuffer, MsgPtr, bufferSize);

    /* If the message is an event, set the corrected message size. */
    if (CFE_EVS_EVENT_MSG_MID == DataMsgID)
    {
        CFE_SB_SetTotalMsgLength((CFE_SB_MsgPtr_t)PQ_CopyBuffer, bufferSize);
    }    

    /* Queue the pointer to the message copy. */
    status = OS_QueuePut(pqueueDump->OSALQueueID, &PQ_CopyBuffer, sizeof(PQ_CopyBuffer), 0);
    if (OS_QUEUE_FULL == status)
    {
        /* This is ok.  Just let the caller know the queue is full.  But,
         * deallocate the memory allocated first since we don't need it
         * anymore.
         */
        status = CFE_ES_PutPoolBuf(Channel->MemPoolHandle, (uint32*)PQ_CopyBuffer);
        if (status < 0)
        {
            pqueueDump->DroppedMsgCnt++;
            (void) CFE_EVS_SendEvent(PQ_PUT_POOL_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "PutPoolBuf: channel=%lu error=%i",
                                     Channel->channelIdx,
                                     (int)status);
            return status;
        }
        PQ_Channel_LockByRef(Channel);
        Channel->MemInUse -= (uint32)status;

        PQ_Channel_UnlockByRef(Channel);
        status = PQ_PRIORITY_QUEUE_FULL_ERR;
    }
    else if (status != OS_SUCCESS)
    {
        pqueueDump->DroppedMsgCnt++;
        (void) CFE_EVS_SendEvent(PQ_OSQUEUE_PUT_ERROR_EID,
                                 CFE_EVS_ERROR,
                                 "OS_QueuePut failed: channel=%lu size=%u error=%i",
                                 Channel->channelIdx, 
                                 sizeof(PQ_CopyBuffer), 
                                 (int)status);
        return status;
    }

    /* Update metrics. */
    pqueueDump->DroppedMsgCnt++;
    pqueueDump->QueuedMsgCnt++;
    pqueueDump->CurrentlyQueuedCnt++;

    if (pqueueDump->HighwaterMark < pqueueDump->CurrentlyQueuedCnt)
    {
        pqueueDump->HighwaterMark = pqueueDump->CurrentlyQueuedCnt;
    }

    return status;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Query a priority queue.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_PriorityQueue_Query(PQ_ChannelData_t *Channel, uint16 PQueueIdx)
{
    osalbool returnCode = FALSE;

    if (NULL == Channel)
    {
        return FALSE;
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel missing valid table data (index = %lu).",
                                 Channel->channelIdx);
        return returnCode;
    }

    PQ_Channel_LockByRef(Channel);

    /* Next, see if the channel is open. */
    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu not open.", Channel->channelIdx);
        PQ_Channel_UnlockByRef(Channel);
        return returnCode;
    }

    if (PQueueIdx >= PQ_MAX_PRIORITY_QUEUES)
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "PQueueIdx %u exceeds the largest available priority queue index, %d", 
                                 PQueueIdx,
                                 PQ_MAX_PRIORITY_QUEUES - 1);
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_PQUEUE_INFO_EID,
                                 CFE_EVS_INFORMATION,
                                 "CHANNEL=%lu PQI=%u S=%u ML=%u QT=%u D=%lu Q=%lu CQ=%u HWM=%u",
                                 Channel->channelIdx,
                                 PQueueIdx,
                                 Channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State,
                                 Channel->ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit,
                                 Channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType,
                                 Channel->DumpTbl.PriorityQueue[PQueueIdx].DroppedMsgCnt,
                                 Channel->DumpTbl.PriorityQueue[PQueueIdx].QueuedMsgCnt,
                                 Channel->DumpTbl.PriorityQueue[PQueueIdx].CurrentlyQueuedCnt,
                                 Channel->DumpTbl.PriorityQueue[PQueueIdx].HighwaterMark);

        returnCode = TRUE;
    }

    PQ_Channel_UnlockByRef(Channel);

    return returnCode;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Is this index a valid priority queue?                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_PriorityQueue_IsValid(PQ_ChannelData_t *Channel, uint32 PQueueIdx)
{
    if ((NULL == Channel) || (NULL == Channel->ConfigTblPtr))
    {
        return FALSE;
    }

    if (PQueueIdx >= PQ_MAX_PRIORITY_QUEUES)
    {
        return FALSE;
    }
    else if (Channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State != PQ_PQUEUE_ENA)
    {
        return FALSE;
    }

    return TRUE;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the CFE EVS Event Message length                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 PQ_GetEventMsgLength(CFE_EVS_Packet_t *msgPtr)
{
    uint32 msgLen;
    uint32 textStart = offsetof(CFE_EVS_Packet_t, Payload) + offsetof(CFE_EVS_Packet_Payload_t, Message);
    uint32 textLength = strnlen(msgPtr->Payload.Message, CFE_EVS_MAX_MESSAGE_LENGTH);
    
    msgLen = textStart + textLength + 1;

    return msgLen;
}
