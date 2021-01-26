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

#ifndef TO_PRIORITY_QUEUE_H
#define TO_PRIORITY_QUEUE_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/

typedef struct TO_ChannelData_t TO_ChannelData;

/**
 * \brief The priority queue is missing.
 */
#define TO_NO_PRIORITY_QUEUE_ERR     (-2)

/**
 * \brief The table data for channel is missing.
 */
#define TO_NO_TABLE_ERR              (-4)

/**
 * \brief The message was not queued because the priority queue was full.
 */
#define TO_PRIORITY_QUEUE_FULL_ERR   (-5)

/**
 * \brief The message was not queued because the caller could not get
 *        enough memory from the memory pool.
 */
#define TO_MEMORY_FULL_ERR           (-6)

/** 
 * \brief Function received a null channel pointer. 
 */
#define TO_CHANNEL_PTR_NULL          (-7)


/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief Priority queue state.  Used to determine if a table entry is used or not.
*/
typedef enum
{
    /** \brief Priority Queue Unused state */
    TO_PQUEUE_UNUSED = 0,
    
    /** \brief Priority Queue Enabled state */
    TO_PQUEUE_ENA    = 1,

    /** \brief  This is a count of Priority Queue states and should not be used.  */
    TO_PQUEUE_COUNT

} TO_PriorityQueueState_t;

/**
** \brief Priority queue type.
*/
typedef enum
{
    /** \brief 'FIFO' queues are normal FIFO queues. */
    TO_PRIORITY_QUEUE_TYPE_FIFO = 0,

    /** \brief 'SINGLE' queues are currently unused. */
    TO_PRIORITY_QUEUE_TYPE_SINGLE = 1,

    /** \brief This is a count of Priority Queue types and should not be used.  */
    TO_PRIORITY_QUEUE_TYPE_COUNT,

} TO_PriorityQueueType_t;


/** \brief Definition for a single priority queue entry. */
typedef struct
{
    /** \brief Priority Queue State (unused or enabled) */
    uint8  State;
    
    /** \brief Priority Queue Message Limit.  */
    uint16 MsgLimit;
    
    /** \brief Priority Queue Type (only one type - FIFO) */
    TO_PriorityQueueType_t  QType;

} TO_PriorityQueue_t                 OS_ALIGN(2);

/** \brief Definition for Priority Queue Metrics */
typedef struct
{
    /** \brief Priority Queue Dropped Message Count */
    uint32 DroppedMsgCnt;
    
    /** \brief Priority Queue Queued Message Count */
    uint32 QueuedMsgCnt;
    
    /** \brief Priority Queue Passed (not filtered) Message Count */
    uint32 PassCnt;
    
    /** \brief Priority Queue Filtered (not sent/passed) Message Count */
    uint32 FilteredCnt;
    
    /** \brief Priority Queue Currently Queued Message Count */     
    uint16 CurrentlyQueuedCnt;
    
    /** \brief Priority Queue Highest recorded the Queued Message Count */
    uint16 HighwaterMark;
    
    /** \brief Priority Queue Queue ID  */ 
    uint32 OSALQueueID;
    
} TO_PriorityQueueMetrics_t;


/************************************************************************
** External Global Variables
*************************************************************************/


/************************************************************************/
/** \brief Resets all metrics.
**
**  \par Description
**       This function is called at when the Reset command is received.
**       It resets all metrics, which include the Dropped Message Counts,
**       and Queued Message Counts.
**
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void TO_PriorityQueue_ResetCountsAll(TO_ChannelData *channel);


/************************************************************************/
/** \brief Buildup all priority queues.
**
**  \par Description
**       This function is called at when the application has loaded a new
**       configuration table.
**
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
**  \return
**  CFE_SUCCESS if no error occurred.  On error, a TO error or an OSAL error 
**  is returned indicating what error occured.
**
*************************************************************************/
int32 TO_PriorityQueue_BuildupAll(TO_ChannelData *channel);


/************************************************************************/
/** \brief Queue message.
**
**  \par Description
**       This function is called by the Classifier to queue a message
**       into a priority queue.
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message

**
**  \param [in]   PQueue        A #TO_PriorityQueue_t pointer to
**                              the priority queue object.
**
**  \returns
**  0 if successful.  OSAL error if unsuccessful.
**  \endreturns
**
*************************************************************************/
int32 TO_PriorityQueue_QueueMsg(TO_ChannelData *channel, CFE_SB_MsgPtr_t MsgPtr, uint32 PQueueIndex);



/************************************************************************/
/** \brief Teardown all priority queues.
**
**  \par Description
**       This function is called at when the application is about to load
**       a new configuration table.  This will flush the queues and
**       deallocate the messages contained in the queues.
**
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
**  \return
**  0 if successful.  OSAL error if unsuccessful.
**
*************************************************************************/
int32 TO_PriorityQueue_TeardownAll(TO_ChannelData *channel);



/************************************************************************/
/** \brief Query a priority queue.
**
**  \par Description
**       This function is called when a ground command is received to
**       query a message flow.  An event is raised with the following
**       text format:
**       "PQI=<PQueueIndex> CI=<ChannelID> S=<State> ML=<MsgLimit> QT=<QueueType> D=<DroppedMsgCnt> Q=<QueuedMsgCnt> CQ=<CurrentlyQueuedCnt> HWM=<HighwaterMark>"
**
**  \param [in]   ChannelIdx    Index for a given channel
**
**  \param [in]   PQueueIdx     Index of the priority queue.
**
**  \return
**  TRUE if priority queue is found.  FALSE if not found.
**
*************************************************************************/
osalbool TO_PriorityQueue_Query(uint16 ChannelIdx, uint16 PQueueIdx);


/************************************************************************/
/** \brief Checks whether a priority index is valid or not.
**
**  \par Description
**       This function is used internally to determine if an index is
**       to a valid priority queue.
**
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   PQueueIdx     Index of the priority queue.
**
**  \return
**  TRUE if priority queue is valid.  FALSE if not.
**
*************************************************************************/
osalbool TO_PriorityQueue_IsValid(TO_ChannelData *channel, uint32 PQueueIdx);


#endif
