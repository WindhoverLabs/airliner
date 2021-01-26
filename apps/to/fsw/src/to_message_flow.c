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
#include "to_message_flow.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Build up all the message flows                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_MessageFlow_Buildup(TO_ChannelData_t *channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;
    
    if (NULL == channel)
    {
        return TO_MESSAGE_FLOW_BAD_ARG_ERR;
    }

    if  (NULL == channel->ConfigTblPtr)
    {
        return TO_MESSAGE_FLOW_NO_TABLE_ERR;
    }
    
    /*
     * Create message flows by subscribing to messages.
     */
    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
        {
            /* Subscribe to message. */
            status = CFE_SB_SubscribeEx(channel->ConfigTblPtr->MessageFlow[i].MsgId, channel->DataPipeId,
                                         CFE_SB_Default_Qos, channel->ConfigTblPtr->MessageFlow[i].MsgLimit);
            if (status != CFE_SUCCESS)
            {
                /* We failed to subscribe to a message.  However, lets just keep going so we can maybe
                 * subscribe to at least some of the messages.  Report and keep going.
                 */
                (void) CFE_EVS_SendEvent(TO_SUBSCRIBE_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message flow failed to subscribe to (0x%08X) on channel %d. (%ld)",
                                         channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         channel->channelIdx,
                                         status);
            }
        }
    }
    return CFE_SUCCESS;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Teardown all the message flows                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_MessageFlow_TeardownAll(TO_ChannelData_t *channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;
    
    if (NULL == channel)
    {
        return TO_MESSAGE_FLOW_BAD_ARG_ERR;
    }

    if (NULL == channel->ConfigTblPtr)
    {
        return TO_MESSAGE_FLOW_NO_TABLE_ERR;
    }
    
    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
        {
            /* Unsubscribe from message. */
            status = CFE_SB_Unsubscribe(channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         channel->DataPipeId);
                        
            if (status != CFE_SUCCESS)
            {
                /*  This is not a critical error.  Just continue processing the rest of the messages. 
                 *  Will not return the failure back.
                 */
                (void) CFE_EVS_SendEvent(TO_UNSUBSCRIBE_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message flow failed to unsubscribe from 0x%04x on channel %d. (%ld)",
                                         channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         channel->channelIdx,
                                         status);
            }
        }
    }
    return CFE_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset all runtime metrics.                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_MessageFlow_ResetCountsAll(TO_ChannelData_t *channel)
{
    uint32 i;
    uint32 j;
    
    if (NULL == channel)
    {
        return;
    }
    
    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        channel->DumpTbl.MessageFlow[i].DroppedMsgCnt = 0;
        channel->DumpTbl.MessageFlow[i].QueuedMsgCnt = 0;
        channel->DumpTbl.MessageFlow[i].SBMsgCnt = 0;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get message flow object                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TO_MessageFlow_t* TO_MessageFlow_GetObject(TO_ChannelData_t *channel, CFE_SB_MsgId_t MsgID, uint32 *Index)
{
    TO_MessageFlow_t *outMsgFlow = NULL;
    uint32 i;

    if (NULL == channel)
    {
        return NULL;
    }

    if  (NULL == channel->ConfigTblPtr)
    {
        return NULL;
    }    

    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId == MsgID)
        {
            outMsgFlow = &channel->ConfigTblPtr->MessageFlow[i];

            if (NULL != Index)
            {
                *Index = i;
            }
            break;
        }
    }

    return outMsgFlow;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the priority queue for a specific message flow              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TO_PriorityQueue_t* TO_MessageFlow_GetPQueue(TO_ChannelData_t *channel, TO_MessageFlow_t *msgFlow, uint32 *Index)
{
    uint32 idx = 0;
    TO_PriorityQueue_t* outPQueue = NULL;

    if (NULL == msgFlow)
    {
        return NULL;
    }
    
    if (NULL == channel)
    {
        return NULL;
    }
    
    if (NULL == channel->ConfigTblPtr)
    {
        return NULL;
    }    

    idx = msgFlow->PQueueID;
    if (idx >= TO_MAX_PRIORITY_QUEUES)
    {
        return NULL;
    }

    if (Index != NULL)
    {
        *Index = idx;
    }
    
    outPQueue = &channel->ConfigTblPtr->PriorityQueue[idx];

    return outPQueue;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Add a message flow                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_MessageFlow_Add(
    uint16 ChannelIdx, 
    CFE_SB_MsgId_t MsgID, 
    uint16 MsgLimit, 
    uint16 PQueueIdx)
{
    uint32 i;
    uint32 j;
    TO_MessageFlow_t *msgFlow   = NULL;
    osalbool added              = FALSE;
    int32 status                = CFE_SUCCESS;
    uint8 queueIdx              = 0;
    uint32 decimationFactorsSum = 0;

    TO_ChannelData_t *channel = NULL;

    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel (channel = %d, but max = %d).",
                                 ChannelIdx, TO_MAX_CHANNELS);

        return FALSE;
    }
    
    channel = &TO_AppData.ChannelData[ChannelIdx];

    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel missing valid table data (channel = %d).",
                                 ChannelIdx);

        return FALSE;
    }
    
    TO_Channel_LockByRef(channel);

    /* Next, see if the channel is open. */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel (%d) not open.", ChannelIdx);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    /* Next, see if there is a flow with this message ID. */
    msgFlow = TO_MessageFlow_GetObject(channel, MsgID, NULL);

    if (NULL != msgFlow)
    {
        /* A message flow already exists.  Reject the request. */
        (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow is already defined for channel %d.",
                                 ChannelIdx);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    /* Now that we know a message flow definition doesn't already exist, find
     * an unused entry so we can add one.  Unused entries have the MsgID set
     * to 0.
     */
    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId == 0)
        {
            /* We found an unused entry.  Set the message flow definition
             * here.  First, make sure the requested priority queue is valid.
             */
            if (TO_PriorityQueue_IsValid(channel, PQueueIdx) == FALSE)
            {
                /* This is an invalid priority queue. */
                (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Priority Queue Index %u is invalid for channel %d.",
                                         PQueueIdx, ChannelIdx);
                                         
                TO_Channel_UnlockByRef(channel);
                return FALSE;
            }

            /* Now subscribe to the message to ensure the message ID is
             * valid.
             */
            status = CFE_SB_SubscribeEx(MsgID, channel->DataPipeId,
                                         CFE_SB_Default_Qos, MsgLimit);
            if (status != CFE_SUCCESS)
            {
                (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message flow failed to subscribe to (0x%08X) on channel %d. (%ld)",
                                         MsgID, ChannelIdx, status);
                                         
                TO_Channel_UnlockByRef(channel);
                return FALSE;
            }

            /* Now that the message was successfully subscribed to, set the
             * message flow definition.
             */
            channel->ConfigTblPtr->MessageFlow[i].MsgId = MsgID;
            channel->ConfigTblPtr->MessageFlow[i].MsgLimit = MsgLimit;
            channel->ConfigTblPtr->MessageFlow[i].PQueueID = PQueueIdx;
            channel->DumpTbl.MessageFlow[i].DroppedMsgCnt = 0;
            channel->DumpTbl.MessageFlow[i].QueuedMsgCnt = 0;
            channel->DumpTbl.MessageFlow[i].SBMsgCnt = 0;
            
            CFE_TBL_Modified(channel->ConfigTblHdl);

            added = TRUE;
            break;
        }
    }
    
    if (FALSE == added)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_ADD_MSG_FLOW_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Failed to add message flow. No unused slots available on channel %d.",
                                ChannelIdx);
    }    

    TO_Channel_UnlockByRef(channel);

    return added;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Remove a message flow                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_MessageFlow_Remove(uint16 ChannelIdx, CFE_SB_MsgId_t MsgID)
{
    uint32 msgFlowIndex       = 0;
    uint32 i                 = 0;
    TO_MessageFlow_t *msgFlow = NULL;
    int32 status              = CFE_SUCCESS;
    TO_ChannelData_t *channel = NULL;

    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel (channel = %d, but max = %d).",
                                 ChannelIdx, TO_MAX_CHANNELS);
                                 
        return FALSE;
    }
    
    channel = &TO_AppData.ChannelData[ChannelIdx];

    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel missing valid table data (channel = %d).",
                                 ChannelIdx);

        return FALSE;
    }
    
    TO_Channel_LockByRef(channel);

    /* Next, see if the channel is open. */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel (%d) not open.", ChannelIdx);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    /* Next, see if there is a flow with this message ID */
    msgFlow = TO_MessageFlow_GetObject(channel, MsgID, &msgFlowIndex);
    if (NULL == msgFlow)
    {
        /* A message flow does not exist.  Reject the request. */
        (void) CFE_EVS_SendEvent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow is not defined for channel %d.",
                                 ChannelIdx);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    /* Now that we have the message flow object, unsubscribe from it and clear
     * the entries. 
     */
    status = CFE_SB_Unsubscribe(msgFlow->MsgId, channel->DataPipeId);
    
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow failed to unsubscribe from 0x%08X on channel %d. (%ld)",
                                 msgFlow->MsgId, ChannelIdx, status);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    /* Now just clear the entries. */
    channel->ConfigTblPtr->MessageFlow[msgFlowIndex].MsgId = 0;
    channel->ConfigTblPtr->MessageFlow[msgFlowIndex].MsgLimit = 0;
    channel->ConfigTblPtr->MessageFlow[msgFlowIndex].PQueueID = 0;
    channel->DumpTbl.MessageFlow[msgFlowIndex].DroppedMsgCnt = 0;
    channel->DumpTbl.MessageFlow[msgFlowIndex].QueuedMsgCnt = 0;
    channel->DumpTbl.MessageFlow[msgFlowIndex].SBMsgCnt = 0;

    TO_Channel_UnlockByRef(channel);

    CFE_TBL_Modified(channel->ConfigTblHdl);

    return TRUE;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Query a message flow                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool TO_MessageFlow_Query(uint16 ChannelIdx, CFE_SB_MsgId_t MsgID)
{
    uint32 i = 0;
    osalbool found = FALSE;
    TO_ChannelData_t *channel = NULL;
    
    /* First, check if the channel index is valid. */
    if (ChannelIdx >= TO_MAX_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel (channel = %d, but max = %d).",
                                 ChannelIdx, TO_MAX_CHANNELS);
          
        return FALSE;
    }

    channel = &TO_AppData.ChannelData[ChannelIdx];
    
    if (NULL == channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel missing valid table data (channel = %d).",
                                 ChannelIdx);

        return FALSE;
    }
    
    TO_Channel_LockByRef(channel);

    /* Next, see if the channel is open. */
    if (channel->State != TO_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel (%d) not open.", ChannelIdx);
                                 
        TO_Channel_UnlockByRef(channel);
        return FALSE;
    }

    for (i = 0; i < TO_MAX_MESSAGE_FLOWS; ++i)
    {
        if (channel->ConfigTblPtr->MessageFlow[i].MsgId == MsgID)
        {
            (void) CFE_EVS_SendEvent(TO_MSG_FLOW_INFO_EID,
                                     CFE_EVS_INFORMATION,
                                     "CHANNEL=%d MID=0x%04x ML=%u PQI=%u D=%lu Q=%lu SB=%lu",
                                     ChannelIdx,
                                     channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                     channel->ConfigTblPtr->MessageFlow[i].MsgLimit,
                                     channel->ConfigTblPtr->MessageFlow[i].PQueueID,
                                     channel->DumpTbl.MessageFlow[i].DroppedMsgCnt,
                                     channel->DumpTbl.MessageFlow[i].QueuedMsgCnt,
                                     channel->DumpTbl.MessageFlow[i].SBMsgCnt);
            found = TRUE;
            break;
        }
    }

    TO_Channel_UnlockByRef(channel);

    if (found != TRUE)
    {
        (void) CFE_EVS_SendEvent(TO_MSG_FLOW_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "MsgID=0x%04x not found (channel = %d)", MsgID, ChannelIdx);
    }
    
    return found;
}

