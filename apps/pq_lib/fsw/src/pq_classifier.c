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

#include "pq_structs.h"
#include "pq_classifier.h"
#include "pq_message_flow.h"
#include "pq_priority_queue.h"
#include "pq_events.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Run the Classifier algorithm                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_Classifier_Run(PQ_ChannelData_t *Channel, CFE_SB_MsgPtr_t DataMsgPtr)
{
    int32  status                 = CFE_SUCCESS;
    PQ_PriorityQueue_t *pqueue    = NULL;
    PQ_MessageFlow_t *msgFlow     = NULL;
    uint32 msgFlowIndex           = 0;
    uint32 pQueueIndex            = 0;
    CFE_SB_MsgId_t  DataMsgID;
    uint32 totalMsgLength         = 0;

    if (DataMsgPtr != NULL)
    {
        DataMsgID = CFE_SB_GetMsgId(DataMsgPtr);

        totalMsgLength = CFE_SB_GetTotalMsgLength(DataMsgPtr);
        if (totalMsgLength > PQ_MAX_MSG_LENGTH)
        {
            (void) CFE_EVS_SendEvent(PQ_TLM_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                                     "Message too long (size = %lu > max = %d) for msgId = (0x%04X) on channel (%u)",
                                     totalMsgLength,
                                     PQ_MAX_MSG_LENGTH,
                                     (unsigned short)DataMsgID,
                                     (unsigned short)Channel->channelIdx);
            Channel->DropMsgCount++;
            goto end_of_function;
        }

        /* Get the first Message Flow object.  If this returns null, the
         * Message ID is not in the table at all so we shouldn't have
         * received this message.  Raise an event.
         */
        msgFlow = PQ_MessageFlow_GetObject(Channel, DataMsgID, &msgFlowIndex);
        if (NULL == msgFlow)
        {
            (void) CFE_EVS_SendEvent(PQ_MF_MSG_ID_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Classifier Recvd invalid msgId (0x%04X) or message flow was removed on channel (%u)", 
                                     (unsigned short)DataMsgID,
                                     (unsigned short)Channel->channelIdx);
            Channel->DropMsgCount++;
            goto end_of_function;
        }

        /* Get the Priority Queue assigned to this Message Flow. */
        pqueue = PQ_MessageFlow_GetPQueue(Channel, msgFlow, &pQueueIndex);
        if (pqueue != NULL)
        {
            /* Queue the message. The else portion will handle all cases where the message 
            *  was not queued for the following reasons: configuration table pointer was not
            *  available, queue is full, or memory full error.
            */
            status = PQ_PriorityQueue_QueueMsg(Channel, DataMsgPtr, pQueueIndex);

            if (CFE_SUCCESS == status)
            {
                /* The message was queued.  Increment counters. */
                Channel->DumpTbl.MessageFlow[msgFlowIndex].QueuedMsgCnt++;
                Channel->QueuedMsgCount++;
            }
            /* The call to PQ_PriorityQueue_QueueMsg may generate the following errors:
             * PQ_PRIORITY_QUEUE_FULL_ERR (OS_QUEUE_FULL), PQ_MEMORY_FULL_ERR, CFE_ES_ERR_MEM_HANDLE,
             * OS_ERR_INVALID_ID, OS_INVALID_POINTER, OS_ERROR  are all handled by this else clause */
            else
            {
                /* Queue is full.  Increment counters and drop the message. */
                Channel->DumpTbl.MessageFlow[msgFlowIndex].DroppedMsgCnt++;

                Channel->DropMsgCount++;

                (void) CFE_EVS_SendEvent(PQ_MSG_DROP_FROM_FLOW_DBG_EID,
                                         CFE_EVS_DEBUG,
                                         "PQ full (PQ %u, channel %u). Error code (%d) Dropped message 0x%04x",
                                         (unsigned int)pQueueIndex,
                                         (unsigned int)Channel->channelIdx,
                                         (int)status,
                                         (unsigned int)DataMsgID);
            }
        }
    }

end_of_function:
    return;
}

