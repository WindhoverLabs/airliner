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

#ifndef PQ_OUTPUT_QUEUE_H
#define PQ_OUTPUT_QUEUE_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "pq_channel.h"
#include "pq_structs.h"

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
/** \brief Push a message onto the output channel queue.
**
**  \par Description
**       This function is called by the Scheduler to push a message onto
**       the output channel for downlink.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   Buffer        Buffer containing the message to push.
**
**  \param [in]   Size          Size of the message, in bytes, to push.
**
*************************************************************************/
int32 PQ_OutputQueue_Push(PQ_ChannelData_t* Channel, const char* Buffer, uint32 Size);


/************************************************************************/
/** \brief Cleanup all output channels.
**
**  \par Description
**       This function is call at application termination and empties
**       the queue, deallocating each message its popped off the queue.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void  PQ_OutputQueue_CleanupAll(PQ_ChannelData_t* Channel);


/************************************************************************/
/** \brief Resets all metrics.
**
**  \par Description
**       This function is called at when the Reset command is received.
**       It resets all metrics, which include the Sent Counts and the
**       Highwater Marks.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void  PQ_OutputQueue_ResetCounts(PQ_ChannelData_t* Channel);


/************************************************************************/
/** \brief Teardown all output channel queues.
**
**  \par Description
**       This function is called at when the application is about to load
**       a new configuration table.  This will flush the queues and
**       deallocate the messages contained in the queues.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
int32 PQ_OutputQueue_Teardown(PQ_ChannelData_t* Channel);


/************************************************************************/
/** \brief Buildup all output channel queues.
**
**  \par Description
**       This function is called at when the application has loaded a new
**       configuration table.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \returns
**  0 if no error occurred.  On error, an OSAL error is returned
**  indicating what error occured.
**  \endreturns
**
*************************************************************************/
int32 PQ_OutputQueue_Buildup(PQ_ChannelData_t* Channel);


/************************************************************************/
/** \brief Queue message.
**
**  \par Description
**       This function is called by the Scheduler to queue a message
**       for downlink.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**
**  \returns
**  0 if successful.  OSAL error if unsuccessful.
**  \endreturns
**
*************************************************************************/
int32 PQ_OutputQueue_QueueMsg(PQ_ChannelData_t* Channel, CFE_SB_MsgPtr_t MsgPtr);


/************************************************************************/
/** \brief Query.
**
**  \par Description
**       This function is called when a ground command is received to
**       query an output channel.  This will raise an information event
**       containing text with Sent Count and Highwater Mark.
**
**  \param [in]   Channel       A #PQ_ChannelData_t pointer that
**                              references the channel data structure
**
**  \returns
**  TRUE if successful.  FALSE if an error occurred.
**  \endreturns
**
*************************************************************************/
boolean PQ_OutputChannel_Query(PQ_ChannelData_t* Channel);

#endif
