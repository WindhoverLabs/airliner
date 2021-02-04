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
#include "pq_config_utils.h"
#include "pq_channel.h"
#include "pq_output_queue.h"
#include "pq_priority_queue.h"
#include "pq_message_flow.h"
#include "pq_mission_cfg.h"
#include "pq_structs.h"
#include "pq_events.h"
#include <string.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern PQ_AppData_t PQ_AppData;

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_InitTables(PQ_ChannelData_t *channel)
{
    int32  status = CFE_SUCCESS;
    int result;

    /* Register Config tables */
    status = CFE_TBL_Register(&channel->ConfigTblHdl,
                               channel->ConfigTableName,
                               sizeof(PQ_ChannelTbl_t),
                               CFE_TBL_OPT_DEFAULT,
                               PQ_ValidateConfigTbl);

    if (status != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(PQ_INIT_CONFIG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X) for channel %d",
                                 (unsigned int)status,
                                 (unsigned int)channel->channelIdx);
        return status;
    }

    /* Register Dump table */
    status = CFE_TBL_Register(&channel->DumpTblHdl,
    			       channel->DumpTableName,
                               sizeof(PQ_ChannelDumpTbl_t),
			       CFE_TBL_OPT_USR_DEF_ADDR,
                               0);
    if (status != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(PQ_INIT_CONFIG_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to register Dump table (0x%08X) for channel %d",
                                 (unsigned int)status,
                                 (unsigned int)channel->channelIdx);
        return status;
    }

    /* Load Config table file */
    status = CFE_TBL_Load(channel->ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           channel->ConfigTableFileName);
    if (status != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(PQ_INIT_CONFIG_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X) for channel %d",
                                 (unsigned int)status,
                                 (unsigned int)channel->channelIdx);

        /* Load emergency backup table */
        PQ_LoadBackupConfigTable(channel);
    }

    /* Load Dump table */
    status = CFE_TBL_Load(channel->DumpTblHdl,
                           CFE_TBL_SRC_ADDRESS,
                           &channel->DumpTbl);
    if (status != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(PQ_INIT_CONFIG_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Failed to load Dump Table (0x%08X) for channel %d",
                                 (unsigned int)status,
                                 (unsigned int)channel->channelIdx);
        return status;
    }

    /* Force a manage on this channel's tables since this is the first time */
    PQ_ManageChannelTables(TRUE, channel);

    return (status);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Load Backup Config Table                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_LoadBackupConfigTable(PQ_ChannelData_t *channel)
{
    int32  status = CFE_SUCCESS;

    /* Load emergency backup TO Config table */
    status = CFE_TBL_Load(channel->ConfigTblHdl,
                           CFE_TBL_SRC_ADDRESS,
                           (void*)channel->BackupTblPtr);

    if (CFE_SUCCESS == status)
    {
        (void) CFE_EVS_SendEvent(PQ_BACKUP_TABLE_INF_EID,
                                CFE_EVS_INFORMATION,
                                "Loaded built-in emergency backup config table! - channel %u",
                                (unsigned int)channel->channelIdx);
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_BACKUP_TABLE_ERR_EID,
                                CFE_EVS_ERROR,
                                "Failed to load built-in emergency backup config table! - channel %u",
                                (unsigned int)channel->channelIdx);    
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Config Table                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_ValidateConfigTbl(void *configTblPtr)
{
    int32  status = CFE_SUCCESS;
    PQ_ChannelTbl_t *PQ_ConfigTblPtr = NULL;

    /* For validating PriorityQueues */
    uint16 PQueueIdx;
    uint16 unused_PQ_Count = 0;
    uint16 valid_PQ_Count = 0;

    /* For validating MessageFlows/Ground Queues */
    uint16 MFlowIdx;
    uint16 unused_MF_Count = 0;
    CCSDS_PriHdr_t  CCSDSPriHdr;

    uint8 queueIdx = 0;

    /* Check if configTblPtr is NULL */
    if (configTblPtr == NULL)
    {
        (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_NULL_PTR_EID,
                                 CFE_EVS_ERROR,
                                 "Config table pointer is NULL, table validation ended");

        return PQ_CONFIG_TABLE_NULL_PTR_ERR;
    }

    /* Pointer to the table data being verified */
    PQ_ConfigTblPtr = (PQ_ChannelTbl_t *)configTblPtr;

    /* Check the Priority Queue section of the table */
    for (PQueueIdx = 0; PQueueIdx < PQ_MAX_PRIORITY_QUEUES; PQueueIdx++)
    {
        /* Check the Priority Queue State for each table entry */
        if (PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].State != PQ_PQUEUE_UNUSED &&
            PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].State != PQ_PQUEUE_ENA)
        {
            (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_PQUEUE_STATE_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Priority Queue State is invalid (%d), for Table ID (%lu) at table index (%d)",
                                     PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].State,
                                     PQ_ConfigTblPtr->TableID,
                                     PQueueIdx);

           return PQ_CONFIG_TABLE_PQUEUE_STATE_ERR;
        }
        else if (PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].State == PQ_PQUEUE_UNUSED)
        {
           /* Found unused priority queue. Increment counter */
           unused_PQ_Count++;
        }
        else
        {
           /* Check individual Priority Queue's Qtype and MsgLimit for error.
            * If the Priority Queue type is incorrect or MsgLimit out of range,
            * the entire table is invalid.
            *
            * Check Priority Queue QType
            */
           if ((PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].QType != PQ_PRIORITY_QUEUE_TYPE_SINGLE) &&
        	   (PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].QType != PQ_PRIORITY_QUEUE_TYPE_FIFO))

           {
               (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID,
                                        CFE_EVS_ERROR,
                                        "Priority Queue Qtype is invalid (%d) for Table ID (%lu) at table index (%d)",
                                        PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].QType,
                                        PQ_ConfigTblPtr->TableID,
                                        PQueueIdx);

               return PQ_CONFIG_TABLE_PQUEUE_QTYPE_ERR;
           }
           /* Check Priority Queue MsgLimit */
           if (PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit > PQ_MAX_QUEUE_SIZE_LIMIT ||
               PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit < 1)
           {
               (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID,
                                        CFE_EVS_ERROR,
                                        "Priority Queue MsgLimit invalid (%d) for Table ID (%lu) at table index (%d)",
                                        PQ_ConfigTblPtr->PriorityQueue[PQueueIdx].MsgLimit,
                                        PQ_ConfigTblPtr->TableID,
                                        PQueueIdx);

               return PQ_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR;
           }

           valid_PQ_Count++;
        }
    }

    /* If all priority queues are unused or there's nothing in the table, the entire table is invalid.
     * No need to check Message Flows. Stop table validation.
     */
    if (PQ_MAX_PRIORITY_QUEUES == unused_PQ_Count)
    {
        (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_NO_PQUEUES_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "No Priority Queues or all queue states are unused in the config table with Table ID (%lu)",
                                 PQ_ConfigTblPtr->TableID);

        return PQ_CONFIG_TABLE_NO_PQUEUES_ERR;
    }

    /* No issues found with Priority Queues. Check the Message Flows */
    for (MFlowIdx = 0; MFlowIdx < PQ_MAX_MESSAGE_FLOWS; MFlowIdx++)
    {
        if ((PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgId == 0x0000) &&
            (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit == 0) &&
            (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID == 0))
        {
           /* Found unused queue increment count */
           unused_MF_Count++;
        }
        else
        {
            /*
             * Check each MsgId's the primary header for the correct CCSDS version,
             * for the presence of the secondary header, and for packet completeness.
             */

            /* Clear the primary header. */
            CCSDS_CLR_PRI_HDR(CCSDSPriHdr);

            /* Set the stream ID */
            CCSDSPriHdr.StreamId[0] = (uint8) (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgId >> 8);  
            CCSDSPriHdr.StreamId[1] = (uint8) (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgId & 0xff);
           
            /* Validate secondary header present */
            if (CCSDS_RD_SHDR(CCSDSPriHdr) == CCSDS_NO_SEC_HDR)
            {
                (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_SHDR_ABSENT_EID,
                                         CFE_EVS_ERROR,
                                         "Secondary Header is absent for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
                                         MFlowIdx,
                                         PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgId,
                                         PQ_ConfigTblPtr->TableID);

                return PQ_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER;
            }

            /* Validate CCSDS version */
            if (CCSDS_RD_VERS(CCSDSPriHdr) != 0)
            {
                (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_CCSDS_VER_INVALID_EID,
                                         CFE_EVS_ERROR,
                                         "Invalid CCSDS Version for table index (%d), MsgId (0x%04X) in Table ID (%lu)",
                                         MFlowIdx,
                                         PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgId,
                                         PQ_ConfigTblPtr->TableID);

                return PQ_CONFIG_TABLE_RETURN_INVALID_VERSION;
            }

            /* Validate Message Flow message limit is in the proper range */
            if (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit > PQ_MAX_PQ_MSG_SIZE_LIMIT ||
                PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit < 1)
            {
                (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message Flow MsgLimit invalid (%u) in Table ID (%lu) at table index (%d) ",
                                         (unsigned int)PQ_ConfigTblPtr->MessageFlow[MFlowIdx].MsgLimit,
                                         PQ_ConfigTblPtr->TableID,
                                         MFlowIdx);

                return PQ_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR;
            }

            /* Validate that the Message Flow Priority Queue ID is within range */
            if (PQ_ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID >= valid_PQ_Count)
            {
                (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message Flow Priority Queue ID is invalid (%d) for Table ID (%lu) at table index (%d)",
                                         PQ_ConfigTblPtr->MessageFlow[MFlowIdx].PQueueID,
                                         PQ_ConfigTblPtr->TableID,
                                         MFlowIdx);

                return PQ_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR;

            }
        }
    }

    /* Check to see if no message flows found.
     * Finding no message flows does not indicate a problem,
     * but send an event to alert the user that there are no
     * Message Flows.
     */
    if (PQ_MAX_MESSAGE_FLOWS == unused_MF_Count)
    {
        (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_NO_MSG_FLOW_INF_EID,
                                 CFE_EVS_INFORMATION,
                                 "INFO: No Message Flow entries in the config table, Table Id (%lu)",
                                 PQ_ConfigTblPtr->TableID);
    }

    return (status);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Config Table                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PQ_ProcessNewConfigTbl(PQ_ChannelData_t* channel)
{
    int32 status = CFE_SUCCESS;

    /* Build up the new configuration. */
    status = PQ_MessageFlow_Buildup(channel);
    if (status != CFE_SUCCESS)
    {
        /* If this function fails, no message flows were created.
         * No sense in continuing with this channel.
         */
        return status;
    }

    status = PQ_PriorityQueue_BuildupAll(channel);

    return status;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Manage all tables                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void PQ_ManageAllAppTables(void)
//{
    //uint32 i;

    //for (i = 0; i < PQ_MAX_CHANNELS; ++i)
    //{
        //PQ_ManageChannelTables(FALSE, i);
    //}
//}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Manage all tables                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQ_ManageChannelTables(osalbool initialManage, PQ_ChannelData_t *Channel)
{
    int32 status = CFE_SUCCESS;

    osalbool dequeueStatus = TRUE;

    /* T=call CFE_TBL_Manage() on the config table, since CFE_TBL_GetStatus() might indicate
     * there is no need. TRUE would only be expected when a table is initialized */
    osalbool tableNeedsManaged;

    /* Override if this is the first time through to force a config table update. */
    tableNeedsManaged = initialManage;

    //PQ_Channel_LockByIndex(ChannelID);
    PQ_Channel_LockByRef(Channel);

    if (Channel->State != PQ_CHANNEL_OPENED)
    {
        PQ_Channel_UnlockByRef(Channel);
        return;
    }

    /* Always manage the dump table for this channel */
    (void) CFE_TBL_Manage(Channel->DumpTblHdl);

    status = CFE_TBL_GetStatus(Channel->ConfigTblHdl);
    if ((status & CFE_SEVERITY_BITMASK) == CFE_SEVERITY_ERROR)
    {
        (void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Channel config table handle failure! (0x%08X)",
                                 (unsigned int)status);

        /* Null the table pointer, because we don't have confidence in the handle
         * and the pointer might be to an invalid table address.
         * Given this error the pointer MIGHT be set by CFE_TBL_GetAddress(), below,
         * but perhaps not. */
        Channel->ConfigTblPtr = 0;
        CFE_TBL_ReleaseAddress(Channel->ConfigTblHdl);

        /* let's attempt Manage() below, in case we can get a valid table back */
        tableNeedsManaged = TRUE;
    }
    else if (CFE_TBL_INFO_UPDATE_PENDING == status)
    {
        /* Teardown message flows and PQueues*/
        (void) PQ_MessageFlow_TeardownAll(Channel);
        (void) PQ_PriorityQueue_TeardownAll(Channel);       

        /* Clear remaining messages from SB data pipe*/
        //dequeueStatus = PQ_Channel_SBPipe_Dequeue_All(ChannelID);
        //if (FALSE == dequeueStatus)
        //{
            //(void) CFE_EVS_SendEvent(PQ_CONFIG_TABLE_ERR_EID,
                                     //CFE_EVS_ERROR,
                                     //"Failed to clear SB data pipe for channel %u, (0x%08X)",
                                     //ChannelID,
                                     //(unsigned int)status);
        //}

        /* Now null the table pointer, because it is going to be updated.  Also
         * so that if something goes wrong it won't be set to an invalid address.
         * It will be set by CFE_TBL_GetAddress(), below. */
        Channel->ConfigTblPtr = 0;
        CFE_TBL_ReleaseAddress(Channel->ConfigTblHdl);
        tableNeedsManaged = TRUE;
    }
    else if (CFE_TBL_INFO_VALIDATION_PENDING == status)
    {
        tableNeedsManaged = TRUE;
    }

    if (tableNeedsManaged) 
    {
        /* Manage the config table */
        status = CFE_TBL_Manage(Channel->ConfigTblHdl);
        if ((status != CFE_SUCCESS) && (status != CFE_TBL_INFO_UPDATED))
        {
            (void) CFE_EVS_SendEvent(PQ_CONFIG_MANAGE_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Failed to manage Config table for channel (0x%08X)",
                                     (unsigned int)status);
        }
        else
        {
            /*
            ** Get a pointer to the table
            */
            status = CFE_TBL_GetAddress(
                    (void *) &Channel->ConfigTblPtr,
                    Channel->ConfigTblHdl);
            if (CFE_TBL_INFO_UPDATED == status)
            {
                status = PQ_ProcessNewConfigTbl(Channel);
                if (status != CFE_SUCCESS) 
                {
                    (void) CFE_EVS_SendEvent(PQ_CONFIG_PROCESS_CONFIG_TBL_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Process New Config Table Failed for channel (0x%08X)",
                                         (unsigned int)status);
                }
            }
            else if (status != CFE_SUCCESS)
            {
                (void) CFE_EVS_SendEvent(PQ_CONFIG_ADDR_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Failed to get Config table's address for channel (0x%08X)",
                                         (unsigned int)status);
            }
        }
    }

    PQ_Channel_UnlockByRef(Channel);
}



/************************/
/*  End of File Comment */
/************************/
    
