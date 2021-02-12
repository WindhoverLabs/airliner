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

#ifndef PQ_PRIORITY_QUEUE_H
#define PQ_PRIORITY_QUEUE_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "pq_structs.h"
#include "pq_errors.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

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
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void PQ_PriorityQueue_ResetCountsAll(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Buildup all priority queues.
**
**  \par Description
**       This function is called at when the application has loaded a new
**       configuration table.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \return
**  CFE_SUCCESS if no error occurred.  On error, a PQ error or an OSAL error 
**  is returned indicating what error occured.
**
*************************************************************************/
int32 PQ_PriorityQueue_BuildupAll(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Queue message.
**
**  \par Description
**       This function is called by the Classifier to queue a message
**       into a priority queue.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message

**
**  \param [in]   PQueue        A #PQ_PriorityQueue_t pointer to
**                              the priority queue object.
**
**  \returns
**  0 if successful.  OSAL error if unsuccessful.
**  \endreturns
**
*************************************************************************/
int32 PQ_PriorityQueue_QueueMsg(PQ_ChannelData_t *Channel, CFE_SB_MsgPtr_t MsgPtr, uint32 PQueueIndex);



/************************************************************************/
/** \brief Teardown all priority queues.
**
**  \par Description
**       This function is called at when the application is about to load
**       a new configuration table.  This will flush the queues and
**       deallocate the messages contained in the queues.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \return
**  0 if successful.  OSAL error if unsuccessful.
**
*************************************************************************/
int32 PQ_PriorityQueue_TeardownAll(PQ_ChannelData_t *Channel);



/************************************************************************/
/** \brief Query a priority queue.
**
**  \par Description
**       This function is called when a ground command is received to
**       query a message flow.  An event is raised with the following
**       text format:
**       "PQI=<PQueueIndex> CI=<ChannelID> S=<State> ML=<MsgLimit> QT=<QueueType> D=<DroppedMsgCnt> Q=<QueuedMsgCnt> CQ=<CurrentlyQueuedCnt> HWM=<HighwaterMark>"
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   PQueueIdx     Index of the priority queue.
**
**  \return
**  TRUE if priority queue is found.  FALSE if not found.
**
*************************************************************************/
osalbool PQ_PriorityQueue_Query(PQ_ChannelData_t *Channel, uint16 PQueueIdx);


/************************************************************************/
/** \brief Checks whether a priority index is valid or not.
**
**  \par Description
**       This function is used internally to determine if an index is
**       to a valid priority queue.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   PQueueIdx     Index of the priority queue.
**
**  \return
**  TRUE if priority queue is valid.  FALSE if not.
**
*************************************************************************/
osalbool PQ_PriorityQueue_IsValid(PQ_ChannelData_t *Channel, uint32 PQueueIdx);


#endif
