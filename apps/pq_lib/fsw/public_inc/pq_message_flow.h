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

#ifndef PQ_MESSAGE_FLOW_H
#define PQ_MESSAGE_FLOW_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "pq_platform_cfg.h"
#include "pq_priority_queue.h"
#include "pq_channel.h"
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
**       This function is called when the Reset command is received.
**       It resets all metrics, which include the Dropped Message Count,
**       Queued Message Count, Software Bus Message Count, and Sent 
**       Message Count.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void PQ_MessageFlow_ResetCountsAll(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Teardown all output channel queues.
**
**  \par Description
**       This function is called at when the application is about to load
**       a new configuration table.  This will flush the queues and
**       deallocate the messages contained in the queues.
**
**  \param [in]   channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**  
**  \return
**  0 if successful.  OSAL error if unsuccessful.
**
*************************************************************************/
int32 PQ_MessageFlow_TeardownAll(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Get the next message flow object.
**
**  \par Description
**       This function is used by the classifier to get the next message
**       flow object with the specified message ID.
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in,out] Index       Pointer to a value that will be set to the array
**                              index of the message flow object returned.
**                              Value is not set if a NULL pointer passed in.
**
** \note  It is safe to give a null pointer as the third argument
**        because the function checks if the Index pointer is NULL
**        If it is NULL and will not assign it if it is NULL.
**        The Index contents can however be zero.
**  
**  \return
**  Pointer to the #TO_MessageFlow_t object when successful.  NULL
**  pointer when unsuccessful.
**
*************************************************************************/
PQ_MessageFlow_t* PQ_MessageFlow_GetObject(PQ_ChannelData_t *Channel, 
                                           CFE_SB_MsgId_t MsgID, 
                                           uint32 *Index);


/************************************************************************/
/** \brief Get priority queue object for the message flow.
**
**  \par Description
**       This function is used by the classifier to get the priority
**       queue object from the message flow object.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgFlow       A #PQ_MessageFlow_t pointer that
**                              references the message flow.
**
**  \param [in,out] Index       Pointer to a value that will be set to the array
**                              index of the message flow object returned.
**                              Value is not set if a NULL pointer passed in.
** 
**  \return
**  Pointer to the #PQ_PriorityQueue_t object when successful.  NULL
**  pointer when unsuccessful.
**
*************************************************************************/
PQ_PriorityQueue_t* PQ_MessageFlow_GetPQueue(PQ_ChannelData_t *Channel, 
         PQ_MessageFlow_t *MsgFlow, uint32 *Index);


/************************************************************************/
/** \brief Add a message flow.
**
**  \par Description
**       This function adds message flow, if the message flow is not
**       found on a given channel.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
**
**  \param [in]   MsgLimit      The maximum messages the message flow
**                              queue will contain before overflowing.
**
**  \param [in]   PQueueIdx     Index of the priority queue.
**
**  TRUE if added.  FALSE if not.
**
*************************************************************************/
osalbool PQ_MessageFlow_Add(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID, uint16 MsgLimit, uint16 PQueueIdx);


/************************************************************************/
/** \brief Remove a message flow.
**
**  \par Description
**       This function removes a message flow from a given channel if it
**       is found.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
** 
**  \return
**  TRUE if message flow is found and removed.  FALSE if not.
**
*************************************************************************/
osalbool PQ_MessageFlow_Remove(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID);


/************************************************************************/
/** \brief Query a message flow.
**
**  \par Description
**       This function queries a message flow to determine if it exists
**       on a given channel.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
** 
**  \return
**  TRUE if message flow is found.  FALSE if not found.
**
*************************************************************************/
osalbool PQ_MessageFlow_Query(PQ_ChannelData_t *Channel, CFE_SB_MsgId_t MsgID);

#endif
