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
#include "pq_events.h"
#include "pq_message_flow.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Build up all the message flows                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_MessageFlow_Buildup(PQ_ChannelData_t *Channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;
    
    if (NULL == Channel)
    {
        return PQ_MESSAGE_FLOW_BAD_ARG_ERR;
    }

    if  (NULL == Channel->ConfigTblPtr)
    {
        return PQ_MESSAGE_FLOW_NO_TABLE_ERR;
    }
    
    ///*
     //* Create message flows by subscribing to messages.
     //*/
    //for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    //{
        //if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
        //{
            ///* Subscribe to message. */
            //status = CFE_SB_SubscribeEx(channel->ConfigTblPtr->MessageFlow[i].MsgId, channel->DataPipeId,
                                         //CFE_SB_Default_Qos, channel->ConfigTblPtr->MessageFlow[i].MsgLimit);
            //if (status != CFE_SUCCESS)
            //{
                ///* We failed to subscribe to a message.  However, lets just keep going so we can maybe
                 //* subscribe to at least some of the messages.  Report and keep going.
                 //*/
                //(void) CFE_EVS_SendEvent(PQ_SUBSCRIBE_ERR_EID,
                                         //CFE_EVS_ERROR,
                                         //"Message flow failed to subscribe to (0x%08X) on channel %d. (%ld)",
                                         //channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         //channel->channelIdx,
                                         //status);
            //}
        //}
    //}
    return CFE_SUCCESS;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Teardown all the message flows                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_MessageFlow_TeardownAll(PQ_ChannelData_t *Channel)
{
    uint32 i;
    int32 status = CFE_SUCCESS;
    
    if (NULL == Channel)
    {
        return PQ_MESSAGE_FLOW_BAD_ARG_ERR;
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        return PQ_MESSAGE_FLOW_NO_TABLE_ERR;
    }
    
    //for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    //{
        //if (channel->ConfigTblPtr->MessageFlow[i].MsgId != 0)
        //{
            ///* Unsubscribe from message. */
            //status = CFE_SB_Unsubscribe(channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         //channel->DataPipeId);
                        
            //if (status != CFE_SUCCESS)
            //{
                ///*  This is not a critical error.  Just continue processing the rest of the messages. 
                 //*  Will not return the failure back.
                 //*/
                //(void) CFE_EVS_SendEvent(PQ_UNSUBSCRIBE_ERR_EID,
                                         //CFE_EVS_ERROR,
                                         //"Message flow failed to unsubscribe from 0x%04x on channel %d. (%ld)",
                                         //channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                         //channel->channelIdx,
                                         //status);
            //}
        //}
    //}
    return CFE_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset all runtime metrics.                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_MessageFlow_ResetCountsAll(PQ_ChannelData_t *Channel)
{
    uint32 i;
    uint32 j;
    
    if (NULL == Channel)
    {
        return;
    }

    for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    {
        Channel->DumpTbl.MessageFlow[i].DroppedMsgCnt = 0;
        Channel->DumpTbl.MessageFlow[i].QueuedMsgCnt = 0;
        Channel->DumpTbl.MessageFlow[i].SBMsgCnt = 0;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get message flow object                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PQ_MessageFlow_t* PQ_MessageFlow_GetObject(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID, uint32 *Index)
{
    PQ_MessageFlow_t *outMsgFlow = NULL;
    uint32 i;

    if (NULL == Channel)
    {
        return NULL;
    }

    if  (NULL == Channel->ConfigTblPtr)
    {
        return NULL;
    }    

    for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    {
        if (Channel->ConfigTblPtr->MessageFlow[i].MsgId == MsgID)
        {
            outMsgFlow = &Channel->ConfigTblPtr->MessageFlow[i];

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
PQ_PriorityQueue_t* PQ_MessageFlow_GetPQueue(PQ_ChannelData_t *Channel, PQ_MessageFlow_t *msgFlow, uint32 *Index)
{
    uint32 idx = 0;
    PQ_PriorityQueue_t* outPQueue = NULL;

    if (NULL == msgFlow)
    {
        return NULL;
    }
    
    if (NULL == Channel)
    {
        return NULL;
    }
    
    if (NULL == Channel->ConfigTblPtr)
    {
        return NULL;
    }    

    idx = msgFlow->PQueueID;
    if (idx >= PQ_MAX_PRIORITY_QUEUES)
    {
        return NULL;
    }

    if (Index != NULL)
    {
        *Index = idx;
    }
    
    outPQueue = &Channel->ConfigTblPtr->PriorityQueue[idx];

    return outPQueue;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Add a message flow                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_MessageFlow_Add(
    PQ_ChannelData_t *Channel, 
    CFE_SB_MsgId_t MsgID, 
    uint16 MsgLimit, 
    uint16 PQueueIdx)
{
    uint32 i;
    uint32 j;
    PQ_MessageFlow_t *msgFlow   = NULL;
    osalbool added              = FALSE;
    int32 status                = CFE_SUCCESS;
    uint8 queueIdx              = 0;
    uint32 decimationFactorsSum = 0;

    if (NULL == Channel)
    {
        return FALSE;
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu missing valid table data.", Channel->channelIdx);

        return FALSE;
    }
    
    PQ_Channel_LockByRef(Channel);

    /* Next, see if the channel is open. */
    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(PQ_CMD_ADD_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu not open.", Channel->channelIdx);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    /* Next, see if there is a flow with this message ID. */
    msgFlow = PQ_MessageFlow_GetObject(Channel, MsgID, NULL);

    if (NULL != msgFlow)
    {
        /* A message flow already exists.  Reject the request. */
        (void) CFE_EVS_SendEvent(PQ_CMD_ADD_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow is already defined for channel %lu.",
                                 Channel->channelIdx);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    /* Now that we know a message flow definition doesn't already exist, find
     * an unused entry so we can add one.  Unused entries have the MsgID set
     * to 0.
     */
    for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    {
        if (Channel->ConfigTblPtr->MessageFlow[i].MsgId == 0)
        {
            /* We found an unused entry.  Set the message flow definition
             * here.  First, make sure the requested priority queue is valid.
             */
            if (PQ_PriorityQueue_IsValid(Channel, PQueueIdx) == FALSE)
            {
                /* This is an invalid priority queue. */
                (void) CFE_EVS_SendEvent(PQ_CMD_ADD_MSG_FLOW_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Priority Queue Index %u is invalid for channel %lu.",
                                         PQueueIdx,
                                         Channel->channelIdx);
                                         
                PQ_Channel_UnlockByRef(Channel);
                return FALSE;
            }

            ///* Now subscribe to the message to ensure the message ID is
             //* valid.
             //*/
            //status = CFE_SB_SubscribeEx(MsgID, channel->DataPipeId,
                                         //CFE_SB_Default_Qos, MsgLimit);
            //if (status != CFE_SUCCESS)
            //{
                //(void) CFE_EVS_SendEvent(PQ_CMD_ADD_MSG_FLOW_ERR_EID,
                                         //CFE_EVS_ERROR,
                                         //"Message flow failed to subscribe to (0x%08X) on channel %d. (%ld)",
                                         //MsgID, ChannelIdx, status);
                                         
                //PQ_Channel_UnlockByRef(channel);
                //return FALSE;
            //}

            /* Now that the message was successfully subscribed to, set the
             * message flow definition.
             */
            Channel->ConfigTblPtr->MessageFlow[i].MsgId = MsgID;
            Channel->ConfigTblPtr->MessageFlow[i].MsgLimit = MsgLimit;
            Channel->ConfigTblPtr->MessageFlow[i].PQueueID = PQueueIdx;
            Channel->DumpTbl.MessageFlow[i].DroppedMsgCnt = 0;
            Channel->DumpTbl.MessageFlow[i].QueuedMsgCnt = 0;
            Channel->DumpTbl.MessageFlow[i].SBMsgCnt = 0;
            
            CFE_TBL_Modified(Channel->ConfigTblHdl);

            added = TRUE;
            break;
        }
    }
    
    if (FALSE == added)
    {
        (void) CFE_EVS_SendEvent(PQ_CMD_ADD_MSG_FLOW_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Failed to add message flow. No unused slots available on channel %lu.", 
                                Channel->channelIdx);
    }    

    PQ_Channel_UnlockByRef(Channel);

    return added;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Remove a message flow                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_MessageFlow_Remove(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID)
{
    uint32 msgFlowIndex       = 0;
    uint32 i                 = 0;
    PQ_MessageFlow_t *msgFlow = NULL;
    int32 status              = CFE_SUCCESS;

    if (NULL == Channel)
    {
        return FALSE;
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu missing valid table data.", Channel->channelIdx);

        return FALSE;
    }
    
    PQ_Channel_LockByRef(Channel);

    /* Next, see if the channel is open. */
    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(PQ_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu not open.", Channel->channelIdx);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    /* Next, see if there is a flow with this message ID */
    msgFlow = PQ_MessageFlow_GetObject(Channel, MsgID, &msgFlowIndex);
    if (NULL == msgFlow)
    {
        /* A message flow does not exist.  Reject the request. */
        (void) CFE_EVS_SendEvent(PQ_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow is not defined for channel %lu.", Channel->channelIdx);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    /* Now that we have the message flow object, unsubscribe from it and clear
     * the entries. 
     */
    status = CFE_SB_Unsubscribe(msgFlow->MsgId, Channel->DataPipeId);
    
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PQ_CMD_REMOVE_MSG_FLOW_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Message flow failed to unsubscribe from 0x%08X on channel %lu. (%ld)",
                                 msgFlow->MsgId, Channel->channelIdx, status);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    /* Now just clear the entries. */
    Channel->ConfigTblPtr->MessageFlow[msgFlowIndex].MsgId = 0;
    Channel->ConfigTblPtr->MessageFlow[msgFlowIndex].MsgLimit = 0;
    Channel->ConfigTblPtr->MessageFlow[msgFlowIndex].PQueueID = 0;
    Channel->DumpTbl.MessageFlow[msgFlowIndex].DroppedMsgCnt = 0;
    Channel->DumpTbl.MessageFlow[msgFlowIndex].QueuedMsgCnt = 0;
    Channel->DumpTbl.MessageFlow[msgFlowIndex].SBMsgCnt = 0;

    PQ_Channel_UnlockByRef(Channel);

    CFE_TBL_Modified(Channel->ConfigTblHdl);

    return TRUE;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Query a message flow                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PQ_MessageFlow_Query(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID)
{
    uint32 i = 0;
    osalbool found = FALSE;

    if (NULL == Channel)
    {
        return FALSE;
    }

    if (NULL == Channel->ConfigTblPtr)
    {
        (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_MISSING_TBL_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu missing valid table data.", Channel->channelIdx);

        return FALSE;
    }
    
    PQ_Channel_LockByRef(Channel);

    /* Next, see if the channel is open. */
    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel %lu not open.", Channel->channelIdx);
                                 
        PQ_Channel_UnlockByRef(Channel);
        return FALSE;
    }

    for (i = 0; i < PQ_MAX_MESSAGE_FLOWS; ++i)
    {
        if (Channel->ConfigTblPtr->MessageFlow[i].MsgId == MsgID)
        {
            (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_INFO_EID,
                                     CFE_EVS_INFORMATION,
                                     "CHANNEL=%lu MID=0x%04x ML=%u PQI=%u D=%lu Q=%lu SB=%lu",
                                     Channel->channelIdx,
                                     Channel->ConfigTblPtr->MessageFlow[i].MsgId,
                                     Channel->ConfigTblPtr->MessageFlow[i].MsgLimit,
                                     Channel->ConfigTblPtr->MessageFlow[i].PQueueID,
                                     Channel->DumpTbl.MessageFlow[i].DroppedMsgCnt,
                                     Channel->DumpTbl.MessageFlow[i].QueuedMsgCnt,
                                     Channel->DumpTbl.MessageFlow[i].SBMsgCnt);
            found = TRUE;
            break;
        }
    }

    PQ_Channel_UnlockByRef(Channel);

    if (found != TRUE)
    {
        (void) CFE_EVS_SendEvent(PQ_MSG_FLOW_INFO_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "MsgID=0x%04x not found.", MsgID);
    }
    
    return found;
}

