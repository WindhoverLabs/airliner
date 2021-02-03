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
** Includes
*************************************************************************/
#include "pq_structs.h"
#include "pq_output_queue.h"
#include "pq_platform_cfg.h"
#include "pq_channel.h"
#include "pq_events.h"

#include <string.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

uint32  PQ_MemPoolDefSize[PQ_MAX_MEMPOOL_BLK_SIZES] =
{
    PQ_MAX_BLOCK_SIZE,
    PQ_MEM_BLOCK_SIZE_07,
    PQ_MEM_BLOCK_SIZE_06,
    PQ_MEM_BLOCK_SIZE_05,
    PQ_MEM_BLOCK_SIZE_04,
    PQ_MEM_BLOCK_SIZE_03,
    PQ_MEM_BLOCK_SIZE_02,
    PQ_MEM_BLOCK_SIZE_01
};

/************************************************************************
** External Global Variables
*************************************************************************/
extern PQ_AppData_t PQ_AppData;

/************************************************************************
** Local Function Definitions
*************************************************************************/

/**
 * \brief Forwards a single channel's telemetry data to PQ_Classifier
 *        and PQ_Scheduler
 *
 * \par Assumptions, External Events, and Notes:
 *      Assumes channel pointer is not NULL
 *
 * \param [in]   channel       A #PQ_ChannelData_t pointer that
 *                             references the channel data structures
 */
void PQ_Channel_ProcessTelemetry(PQ_ChannelData_t *channel);

/**
 * \brief Resets each channel's message flow, priority queue,
 *        and output queue counters.
 *
 * \par Assumptions, External Events, and Notes:
 *      Assumes channel pointer is not NULL
 *
 * \param [in]   channel       A #PQ_ChannelData_t pointer that
 *                             references the channel data structures
 */
void PQ_Channel_ResetCounts(PQ_ChannelData_t *channel);

/**
 * \brief Initializes a reference channel data, sets the channel's index,
 *        and creates a channel mutex.
 *
 * \par Assumptions, External Events, and Notes:
  *     If the input index is out of range, an event is generated
 *      #PQ_CHANNEL_OUT_OF_RANGE_ERR_EID
 *
 * \param [in]   index
 *               Index of the channel
 *
 * \returns
 * CFE_SUCCESS if successful.  #PQ_CHANNEL_OUT_OF_RANGE_ERR error if unsuccessful.
 * \endreturns
 */
int32 PQ_Channel_Init(uint16 index);

/**
 * \brief Calls functions to teardown an individual channel's priority queue, 
 *        output queue, and message flow(s).  It also deletes the channel's mutex.
 *
 * \par Assumptions, External Events, and Notes:
 *      If the input index is out of range, an event is generated
 *      #PQ_CHANNEL_OUT_OF_RANGE_ERR_EID
 *
 * \param [in]   index
 *               Index of the channel
 */
void PQ_Channel_Cleanup(uint16 index);


int32 PQ_Channel_OpenChannel(const uint32 index, const char *ChannelName,
    const char *ConfigTableName, const char *ConfigTableFileName,
    PQ_ChannelTbl_t *BackupTblPtr, const char *DumpTableName,
    const uint32 CfCntSemMax, const char *CfCntSemName)
{
    int32 status = PQ_CHANNEL_OPEN_ERR;
    char pipeName[OS_MAX_API_NAME];
    PQ_ChannelData_t *channel = NULL;

    if (index >= PQ_MAX_CHANNELS)
    {
        status = PQ_CHANNEL_OUT_OF_RANGE_ERR;
        (void) CFE_EVS_SendEvent(PQ_INIT_APP_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index of %u.", (unsigned int)index);
        return status;
    }

    channel = &PQ_AppData.ChannelData[index];

    if(channel->State == PQ_CHANNEL_CLOSED)
    {
        PQ_Channel_LockByRef(channel);

        snprintf(pipeName, OS_MAX_API_NAME, "PQ_%s", ChannelName);

        strncpy(channel->ChannelName,
                ChannelName, sizeof(channel->ChannelName));
        channel->ChannelName[sizeof(channel->ChannelName) - 1] = '\0';
            
        strncpy(channel->ConfigTableName,
                ConfigTableName, sizeof(channel->ConfigTableName));
        channel->ConfigTableName[sizeof(channel->ConfigTableName) - 1] = '\0';        
        
        strncpy(channel->ConfigTableFileName,
                ConfigTableFileName, sizeof(channel->ConfigTableFileName));
        channel->ConfigTableFileName[sizeof(channel->ConfigTableFileName) - 1] = '\0';

        channel->BackupTblPtr = BackupTblPtr;
                
        strncpy(channel->DumpTableName,
                DumpTableName, sizeof(channel->DumpTableName));
        channel->DumpTableName[sizeof(channel->DumpTableName) - 1] = '\0';

        /* Set up CF Throttling semaphore */
        channel->CfCntSemMax = CfCntSemMax;
        strncpy(channel->CfCntSemName, CfCntSemName,
                sizeof(channel->CfCntSemName));
        channel->CfCntSemName[OS_MAX_API_NAME - 1] =  '\0';       

        /* Initialize CF Counting Sem to CfCntSemMax */
        /* NOTE: This will fail if two channels attempt to use the same
         * CF channel. */
        status = OS_CountSemCreate(&channel->CfCntSemId,
                                    channel->CfCntSemName,
                                    channel->CfCntSemMax, 0);
        if (status != OS_SUCCESS)
        {
            CFE_EVS_SendEvent(PQ_INIT_APP_ERR_EID,
                              CFE_EVS_ERROR,
                              "Failed to create counting semaphore "
                              "for CF channel semaphore:%s for TO channel(%u):%s . "
                              "(OSAL Error:%d)",
                              channel->CfCntSemName, (unsigned int)index, channel->ChannelName, (int)status);
                              
            PQ_Channel_UnlockByRef(channel);                                   
            return status;
        }

        status = PQ_OutputQueue_Buildup(channel);
        if (status != OS_SUCCESS)
        {
            /* This is a critical error for this channel.  No sense in continuing.  Destroy
             * the counting semaphore we just created, and return the error back to the caller.
             */
            OS_CountSemDelete(channel->CfCntSemId);

            channel->State = PQ_CHANNEL_CLOSED;

            PQ_Channel_UnlockByRef(channel);
            return status;
        }
        
        /* TODO commented out. Messages passed in directly no pipes needed. */
        ///* Init data pipe and subscribe to messages on the data pipe */
        //status = CFE_SB_CreatePipe(&channel->DataPipeId,
                                     //PQ_DATA_PIPE_DEPTH,
                                     //pipeName);
        //if (status != CFE_SUCCESS)
        //{
            ///* This is a critical error for this channel.  No sense in continuing.  Destroy
             //* the counting semaphore and call the Output queue teardown function before
             //* returning the error back to the caller.
             //*/
            //(void) OS_CountSemDelete(channel->CfCntSemId);
            //(void) PQ_OutputQueue_Teardown(channel);

            //(void) CFE_EVS_SendEvent(PQ_INIT_DATAPIPE_ERR_EID,
                                     //CFE_EVS_ERROR,
                                     //"Failed to create channel (%u) '%s' pipe (0x%08X)",
                                     //(unsigned int)index,
                                     //ChannelName,
                                     //(unsigned int)status);

            //channel->State = PQ_CHANNEL_CLOSED;

            //PQ_Channel_UnlockByRef(channel);
            //return status;
        //}
        
        /* Set channel state: required state to complete table initialization */
        channel->State = PQ_CHANNEL_OPENED;
        
        status = PQ_InitTables(channel);
        if (status != CFE_SUCCESS)
        {
            /* This is a critical error for this channel.  No sense in continuing.  Destroy
             * the counting semaphore, call the Output queue teardown function, and delete
             * the pipe before returning the error back to the caller.
             */
            (void) OS_CountSemDelete(channel->CfCntSemId);
            (void) PQ_OutputQueue_Teardown(channel);
            (void) CFE_SB_DeletePipe(channel->DataPipeId);

            CFE_EVS_SendEvent(PQ_INIT_CONFIG_ERR_EID,
                              CFE_EVS_ERROR,
                              "Failed to init config tables (0x%08X) for channel %u",
                              (unsigned int)status,
                              (unsigned int)index);

            channel->State = PQ_CHANNEL_CLOSED;

            PQ_Channel_UnlockByRef(channel);
            return status;
        }

        /* Initialize the memory pool for the priority queues and output channel
         * queues.
         */
        status = CFE_ES_PoolCreateEx(&channel->MemPoolHandle,
                      channel->MemPoolBuffer,
                      PQ_NUM_BYTES_IN_MEM_POOL,
                      PQ_MAX_MEMPOOL_BLK_SIZES,
                      &PQ_MemPoolDefSize[0],
                      CFE_ES_USE_MUTEX);
        if (status != CFE_SUCCESS)
        {
            /* This is a critical error for this channel.  No sense in continuing.  Destroy
             * the counting semaphore, call the Output queue teardown function, and delete
             * the pipe before returning the error back to the caller.  Don't need to do
             * anything with the tables.  They will just be reinitialized the next time
             * we try opening the channel.
             */
            (void) OS_CountSemDelete(channel->CfCntSemId);
            (void) PQ_OutputQueue_Teardown(channel);
            (void) CFE_SB_DeletePipe(channel->DataPipeId);

            (void) CFE_EVS_SendEvent(PQ_CR_POOL_ERR_EID,
                                     CFE_EVS_ERROR,
                    "Error creating memory pool (0x%08X) for channel %u",(unsigned int)status,
                    (unsigned int)index);

            channel->State = PQ_CHANNEL_CLOSED;

            PQ_Channel_UnlockByRef(channel);

            return status;
        }
  
        PQ_Channel_UnlockByRef(channel);        
    }
    return status;
}


void PQ_Channel_ProcessTelemetryAll(void)
{
    uint32 i;
    PQ_ChannelData_t *channel = NULL;

    for (i = 0; i < PQ_MAX_CHANNELS; ++i)
    {
        channel = &PQ_AppData.ChannelData[i];
        if(PQ_CHANNEL_OPENED == channel->State)
        {
            PQ_Channel_ProcessTelemetry(channel);
        }
    }
}


void PQ_Channel_ProcessTelemetry(PQ_ChannelData_t *channel)
{
    PQ_Channel_LockByRef(channel);
    PQ_Classifier_Run(channel);
    PQ_Scheduler_Run(channel);
    PQ_Channel_UnlockByRef(channel);
}


void PQ_Channel_ResetCountsAll(void)
{
    uint32 i;
    PQ_ChannelData_t *channel = NULL;

    for (i = 0; i < PQ_MAX_CHANNELS; ++i)
    {
        channel = &PQ_AppData.ChannelData[i];
        PQ_Channel_ResetCounts(channel);
    }
}



void PQ_Channel_ResetCounts(PQ_ChannelData_t *channel)
{
    PQ_Channel_LockByRef(channel);
    channel->PeakMemInUse = 0;
    channel->MemFullCount = 0;
    channel->SentMsgCount = 0;
    channel->QueuedMsgCount = 0;
    channel->DropMsgCount = 0;
    channel->FailedMsgCount = 0;
    channel->BytesSent = 0;
    PQ_MessageFlow_ResetCountsAll(channel);
    PQ_PriorityQueue_ResetCountsAll(channel);
    PQ_OutputQueue_ResetCounts(channel);
    PQ_Channel_UnlockByRef(channel);
}



void PQ_Channel_LockByIndex(uint16 index)
{
    PQ_ChannelData_t *channel = NULL;
    
    if (index < PQ_MAX_CHANNELS)
    {
        channel = &PQ_AppData.ChannelData[index];
        PQ_Channel_LockByRef(channel);
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_CHANNEL_OUT_OF_RANGE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index (%u).",
                                 index);
    }
}


void PQ_Channel_UnlockByIndex(uint16 index)
{
    PQ_ChannelData_t *channel = NULL;
    
    if (index < PQ_MAX_CHANNELS)
    {
        channel = &PQ_AppData.ChannelData[index];
        PQ_Channel_UnlockByRef(channel);
        
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_CHANNEL_OUT_OF_RANGE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index (%u).",
                                 index);
    }
}


void PQ_Channel_LockByRef(PQ_ChannelData_t *channel)
{
    int32 status = OS_SUCCESS;
    
    if (NULL != channel)
    {
        status = OS_MutSemTake(channel->MutexID);
        if (OS_SUCCESS != status) 
        {
            (void) CFE_EVS_SendEvent(PQ_CHANNEL_LOCK_MUTEX_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Channel lock mutex failure (%i) on channel %u.",
                                     (int)status,
                                     (unsigned int)channel->channelIdx);
        }
    }    
}


void PQ_Channel_UnlockByRef(PQ_ChannelData_t *channel)
{
    int32 status = OS_SUCCESS;
    
    if (NULL != channel)
    {
        status = OS_MutSemGive(channel->MutexID);
        if (OS_SUCCESS != status) 
        {
            (void) CFE_EVS_SendEvent(PQ_CHANNEL_UNLOCK_MUTEX_ERR_EID,
                                     CFE_EVS_ERROR,
                                     "Channel unlock mutex failure (%i) on channel %u.",
                                     (int)status,
                                     (unsigned int)channel->channelIdx);
        }        
    }
}



void PQ_Channel_InitAll(void)
{
    uint32 i;

    for (i = 0; i < PQ_MAX_CHANNELS; ++i)
    {
        (void) PQ_Channel_Init(i);
    }
}


int32 PQ_Channel_Init(uint16 index)
{
    int32 status = CFE_SUCCESS;
    char mutexName[OS_MAX_API_NAME];
    PQ_ChannelData_t *channel = NULL;

    if (index >= PQ_MAX_CHANNELS)
    {
        status = PQ_CHANNEL_OUT_OF_RANGE_ERR;
        
        (void) CFE_EVS_SendEvent(PQ_CHANNEL_OUT_OF_RANGE_ERR_EID,
                         CFE_EVS_ERROR,
                         "Invalid channel index (%u).",
                         index);
        return status;
    }

    channel = &PQ_AppData.ChannelData[index];

    snprintf(mutexName, OS_MAX_API_NAME, "PQ_MUT_%u", (unsigned int)index);

    status = OS_MutSemCreate(&channel->MutexID, mutexName, 0);
    if(status != CFE_SUCCESS) {

        channel->State = PQ_CHANNEL_UNKNOWN;
    }
    else
    {
        channel->channelIdx = index;
        channel->OutputQueue.OSALQueueID = OS_MAX_QUEUES;
        channel->State = PQ_CHANNEL_CLOSED;
    }

    return status;
}


void PQ_Channel_CleanupAll(void)
{
    uint32 i;

    //PQ_OutputChannel_CustomCleanupAll();

    for (i = 0; i < PQ_MAX_CHANNELS; ++i)
    {
        PQ_Channel_Cleanup(i);
    }
}


void PQ_Channel_Cleanup(uint16 index)
{
    int32 status = CFE_SUCCESS;
    PQ_ChannelData_t *channel = NULL;
    
    if (index < PQ_MAX_CHANNELS)
    {
        channel = &PQ_AppData.ChannelData[index];

        if(channel->State != PQ_CHANNEL_UNKNOWN)
        {
            status = PQ_MessageFlow_TeardownAll(channel);
            if (status != CFE_SUCCESS)
            {
                (void) CFE_EVS_SendEvent(PQ_CHANNEL_TEARDOWN_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Message Flow Teardown failure %ld on channel %u.",
                                         status, index);
            }

            status = PQ_PriorityQueue_TeardownAll(channel);
            if (status != CFE_SUCCESS)
            {
                (void) CFE_EVS_SendEvent(PQ_CHANNEL_TEARDOWN_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Priority Queue Teardown failure %ld on channel %u.",
                                         status, index);
            }

            status = PQ_OutputQueue_Teardown(channel);
            if (status != CFE_SUCCESS)
            {
                (void) CFE_EVS_SendEvent(PQ_CHANNEL_TEARDOWN_ERR_EID,
                                         CFE_EVS_ERROR,
                                         "Output Queue Teardown failure %ld on channel %u.",
                                         status, index);
            }

            OS_MutSemDelete(channel->MutexID);
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_CHANNEL_OUT_OF_RANGE_ERR_EID,
                        CFE_EVS_ERROR,
                        "Invalid channel index (%u).",
                        index);

    }
}


uint8 PQ_Channel_State(uint16 index)
{
    if (index < PQ_MAX_CHANNELS) 
    {
        return PQ_AppData.ChannelData[index].State;
    }
    else
    {
        (void) CFE_EVS_SendEvent(PQ_CHANNEL_OUT_OF_RANGE_ERR_EID,
                                 CFE_EVS_ERROR,
                                 "Invalid channel index (%u).",
                                  index);
        return PQ_CHANNEL_UNKNOWN;    
    }
}


//osalbool PQ_Channel_SBPipe_Dequeue_All(uint16 index)
//{
    //int32 status = CFE_SUCCESS;
    //int32 i;
    //CFE_SB_MsgPtr_t  DataMsgPtr = NULL;
    //PQ_ChannelData_t *channel = NULL;

    //if (index >= PQ_MAX_CHANNELS)
    //{
        //(void) CFE_EVS_SendEvent(PQ_FLUSH_INVALID_CHIDX_ERR_EID, CFE_EVS_ERROR,
                            //"ChannelID %u out of range in SB pipe dequeue all.", (unsigned int)index);
        //return FALSE;
    //}

    //channel = &PQ_AppData.ChannelData[index];

    ///* Dequeue until empty */
    //for (i = 0; i < PQ_DATA_PIPE_DEPTH; ++i)
    //{
        //status = CFE_SB_RcvMsg(&DataMsgPtr, channel->DataPipeId, CFE_SB_POLL);
        //if (CFE_SUCCESS == status)
        //{
            //continue;
        //}
        //else if (CFE_SB_NO_MESSAGE == status)
        //{
            ///* Break early if empty */
            //break;
        //}
        //else
        //{
            //(void) CFE_EVS_SendEvent(PQ_PIPE_READ_ERR_EID,
                                     //CFE_EVS_ERROR,
                                     //"Data pipe read error on SBPipe dequeue all (0x%08X) on channel %d",
                                     //(unsigned int)status,
                                     //index);
            //return FALSE;
        //}  
    //}

    //return TRUE;
//}
