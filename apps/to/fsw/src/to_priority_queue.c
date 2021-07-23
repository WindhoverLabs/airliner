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
#include <string.h>
#include <stddef.h>


extern TO_AppData_t TO_AppData;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Clear all dynamics metrics.                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_PriorityQueue_ResetCountsAll(TO_ChannelData_t* channel)
{
    uint32 i;

    for (i = 0; i < TO_MAX_PRIORITY_QUEUES; ++i)
    {
//        channel->DumpTbl.PriorityQueue[i].DroppedMsgCnt = 0;
//        channel->DumpTbl.PriorityQueue[i].QueuedMsgCnt = 0;
//        channel->DumpTbl.PriorityQueue[i].HighwaterMark = 0;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Buildup all priority queues after a reconfiguration.            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_PriorityQueue_BuildupAll(TO_ChannelData_t *channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;

    /* First, check if the channel NULL. */
    if (NULL == channel) 
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_MISSING_TBL_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Channel is NULL.");
        status = TO_CHANNEL_PTR_NULL;
    }    
    
    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to create priority queues on channel %lu, missing table.",
                                 channel->channelIdx);
        status = TO_NO_TABLE_ERR;
    }

    /*
     * Create the priority queues.
     */
    for (i = 0; i < TO_MAX_PRIORITY_QUEUES; ++i)
    {
        if (channel->ConfigTblPtr->PriorityQueue[i].State != TO_PQUEUE_UNUSED)
        {
        	TO_PriorityQueue_Buildup(channel, i);
        }
    }

    return status;
}



int32 TO_PriorityQueue_Buildup(TO_ChannelData_t *channel, uint32 PQueueIndex)
{
	int32 status;
    char QueueName[OS_MAX_API_NAME];
    uint32 i;

    (void) CFE_PSP_MemSet(QueueName, 0, sizeof(QueueName));
    snprintf(QueueName, OS_MAX_API_NAME, "TO_%s_%u", (const char*)&channel->ChannelName[0], (uint8)PQueueIndex);

    status = CFE_SB_CreatePipe(
            &channel->DumpTbl.PriorityQueue[PQueueIndex].SBPipeID,
			channel->ConfigTblPtr->PriorityQueue[PQueueIndex].Depth,
			QueueName);
    if (status != CFE_SUCCESS)
    {
        /* For some reason, we failed to create the queue, but this is not a critical error.
         * Report the failure and continue.
         */
        (void) CFE_EVS_SendEvent(TO_PQUEUE_CREATE_ERR_EID,
                             CFE_EVS_ERROR,
                            "Failed to create '%s' priority queue #%lu on channel %lu. (%ld)",
							QueueName,
							PQueueIndex,
                            channel->channelIdx,
                            status);

        channel->State = TO_CHANNEL_CLOSED;

        return status;
    }

	/*
	 * Create message flows by subscribing to messages.
	 */
	for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
	{
		if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
		{
			/* Subscribe to message. */
			if(channel->ConfigTblPtr->MessageFlow[i].PQueueID == PQueueIndex)
			{
				status = CFE_SB_SubscribeEx(channel->ConfigTblPtr->MessageFlow[i].MsgId, channel->DumpTbl.PriorityQueue[PQueueIndex].SBPipeID,
											 CFE_SB_Default_Qos, channel->ConfigTblPtr->MessageFlow[i].MsgLimit);
				if (status != CFE_SUCCESS)
				{
					/* We failed to subscribe to a message.  However, lets just keep going so we can maybe
					 * subscribe to at least some of the messages.  Report and keep going.
					 */
					(void) CFE_EVS_SendEvent(TO_SUBSCRIBE_ERR_EID,
											 CFE_EVS_ERROR,
											 "Message flow failed to subscribe to (0x%08X) on channel %lu. (%ld)",
											 channel->ConfigTblPtr->MessageFlow[i].MsgId,
											 channel->channelIdx,
											 status);
				}
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
int32 TO_PriorityQueue_TeardownAll(TO_ChannelData_t *channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;

    if(NULL != channel->ConfigTblPtr)
    {
        for(i = 0; i < TO_MAX_PRIORITY_QUEUES; ++i)
        {
            if(channel->ConfigTblPtr->PriorityQueue[i].State != TO_PQUEUE_UNUSED)
            {
            	(void) TO_PriorityQueue_Teardown(channel, i);
            }
        }
    }
    else
    {
        /* No table.  Just keep going and try processing the next channel. */
        (void) CFE_EVS_SendEvent(TO_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to tear down priority queues on channel %d, missing table.",
                                 (unsigned int)channel->channelIdx);
    }

    return status;
}


int32 TO_PriorityQueue_Teardown(TO_ChannelData_t *channel, uint32 PQueueIndex)
{
    uint32 i;

    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
        {
        	int32 status;

            /* Unsubscribe from message. */
            status = CFE_SB_Unsubscribe(channel->ConfigTblPtr->MessageFlow[i].MsgId,
            		                    channel->DumpTbl.PriorityQueue[PQueueIndex].SBPipeID);

            if (status != CFE_SUCCESS)
            {
                /*  This is not a critical error.  Just continue processing the rest of the messages.
                 *  Will not return the failure back.
                 */
                (void) CFE_EVS_SendEvent(TO_UNSUBSCRIBE_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message flow failed to unsubscribe from 0x%04x on channel %ld. (%ld)",
                                         channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         channel->channelIdx,
                                         status);
            }
        }
    }

    (void) CFE_SB_DeletePipe(channel->DumpTbl.PriorityQueue[PQueueIndex].SBPipeID);

    return CFE_SUCCESS;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Query a priority queue.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_PriorityQueue_Query(uint16 ChannelIdx, uint16 PQueueIdx)
{
    osalbool returnCode = FALSE;
    TO_ChannelData_t *channel = NULL;

    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index (index = %d, max = %d).",
                                 ChannelIdx, TO_MAX_CHANNELS);
        return returnCode;
    }
    
    channel = &TO_AppData.ChannelData[ChannelIdx];

    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel missing valid table data (index = %d).",
                                 ChannelIdx);
        return returnCode;
    }

    TO_Channel_LockByRef(channel);

    /* Next, see if the channel is open. */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %d not open.", ChannelIdx);
        TO_Channel_UnlockByRef(channel);
        return returnCode;
    }

    if (PQueueIdx >= TO_MAX_PRIORITY_QUEUES)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "PQueueIdx %u exceeds the largest available priority queue index, %d", 
                                 PQueueIdx,
                                 TO_MAX_PRIORITY_QUEUES - 1);
    }
    else
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_INFO_EID,
                                 CFE_EVS_INFORMATION,
                                 "CHANNEL=%d PQI=%u S=%u ML=%u QT=%u",
                                 ChannelIdx,
                                 PQueueIdx,
                                 channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State,
                                 channel->ConfigTblPtr->PriorityQueue[PQueueIdx].Depth,
                                 channel->ConfigTblPtr->PriorityQueue[PQueueIdx].QType);

        returnCode = TRUE;
    }

    TO_Channel_UnlockByRef(channel);

    return returnCode;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Is this index a valid priority queue?                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_PriorityQueue_IsValid(TO_ChannelData_t *channel, uint32 PQueueIdx)
{
    if ((NULL == channel) || (NULL == channel->ConfigTblPtr))
    {
        return FALSE;
    }

    if (PQueueIdx >= TO_MAX_PRIORITY_QUEUES)
    {
        return FALSE;
    }
    else if (channel->ConfigTblPtr->PriorityQueue[PQueueIdx].State != TO_PQUEUE_ENA)
    {
        return FALSE;
    }

    return TRUE;
}



int32 TO_PriorityQueue_Get(TO_ChannelData_t *Channel, uint16 PQueueIdx,
		                   CFE_SB_MsgPtr_t  *Msg)
{
	int32  status;
	uint16 msgID;

    status = CFE_SB_RcvMsg(Msg, Channel->DumpTbl.PriorityQueue[PQueueIdx].SBPipeID, CFE_SB_POLL);
    if(CFE_SUCCESS == status)
    {
    	Channel->DumpTbl.PriorityQueue[PQueueIdx].QueuedMsgCnt++;

        msgID = CFE_SB_GetMsgId(*Msg);

        /* Check if this is a CFDP message. */
        if(CF_SPACE_TO_GND_PDU_MID == msgID)
        {
            /* This is a CFDP message. Release the throttling semaphore. */
            OS_CountSemGive(Channel->OutputQueue.CfCntSemId);
        }
    }
    else if(status != CFE_SB_NO_MESSAGE)
    {
        (void) CFE_EVS_SendEvent(TO_PQUEUE_SB_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "PQ pipe read error (0x%08X)", (unsigned int)status);
    }

    return status;
}
