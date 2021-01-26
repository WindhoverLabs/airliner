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

#ifndef TO_MESSAGE_FLOW_H
#define TO_MESSAGE_FLOW_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "to_platform_cfg.h"
#include "to_priority_queue.h"

/************************************************************************
** Local Defines
*************************************************************************/
typedef struct TO_ChannelData_t TO_ChannelData_t;

/** \brief A bad argument was passed to a function */
#define TO_MESSAGE_FLOW_BAD_ARG_ERR   (-2)

/** \brief No table data is available */
#define TO_MESSAGE_FLOW_NO_TABLE_ERR  (-3)

/************************************************************************
** Local Structure Definitions
*************************************************************************/


/** \brief Definition for a single telemetry message flow entry */
typedef struct
{
    /**
     * \brief  The Message ID is used to select the message from the cFE Software Bus (SB) to send
     *  through the TO channel managed by the table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be defined in cfe_mission_cfg.h
     */
    CFE_SB_MsgId_t  MsgId;

    /**
     * \brief  Maximum number of messages in a Message Flow.
     *
     * \par
     * Units: Number of messages that can be in a Message Flow.
     *
     * \par
     * Limits: The mininum value is 1.  The maximum value depends on available memory and is
     *         an OPS driven parameter.  The sum of all the msglimits in a priority queue 
     *         should not be greater than the msglimit of that priority queue.
     */
    uint16          MsgLimit;

    /**
     * \brief  The Priority Queue ID is an index which selects the priority queue the message will be
     *  placed into.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: If the Priority Queue ID does not match a priority queue then the table is invalid
     */
    uint16          PQueueID;
       
} TO_MessageFlow_t                                                        OS_ALIGN(2);


/** \brief Definition for the metrics collected for a single telemetry message flow entry */
typedef struct
{
    /**
     * \brief  Dropped message count is a count of number of times a specific message was 
     *  dropped.  
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    uint32          DroppedMsgCnt;

    /**
     * \brief  Queued message count is a count of number of times a specific message was queued for sending.  
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    uint32          QueuedMsgCnt;

    /**
     * \brief  SBMsgCnt is the total number of messages for the particular message ID pulled from the 
     *         software bus for that channel. Used for message decimation.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: Increments on each message received. Rolls over to 0 at max value.
     */
    uint32          SBMsgCnt;

} TO_MessageFlowMetrics_t;

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
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
*************************************************************************/
void TO_MessageFlow_ResetCountsAll(TO_ChannelData_t *channel);


/************************************************************************/
/** \brief Teardown all output channel queues.
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
int32 TO_MessageFlow_TeardownAll(TO_ChannelData_t *channel);


/************************************************************************/
/** \brief Buildup all message flows.
**
**  \par Description
**       This function is called at when the application has loaded a new
**       configuration table.
**
**  \return
**  CFE_SUCCESS if no error occurred.  On error, an OSAL error is returned
**  indicating what error occured.
**
*************************************************************************/
int32 TO_MessageFlow_Buildup(TO_ChannelData_t *channel);


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
**  \param [in]   channel       A #TO_ChannelData_t pointer that
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
TO_MessageFlow_t* TO_MessageFlow_GetObject(TO_ChannelData_t *channel, 
                                           CFE_SB_MsgId_t MsgID, 
                                           uint32 *Index);


/************************************************************************/
/** \brief Get priority queue object for the message flow.
**
**  \par Description
**       This function is used by the classifier to get the priority
**       queue object from the message flow object.
**
**  \param [in]   channel       A #TO_ChannelData_t pointer that
**                              references the channel data structure
**
**  \param [in]   MsgFlow       A #TO_MessageFlow_t pointer that
**                              references the message flow.
**
**  \param [in,out] Index       Pointer to a value that will be set to the array
**                              index of the message flow object returned.
**                              Value is not set if a NULL pointer passed in.
** 
**  \return
**  Pointer to the #TO_PriorityQueue_t object when successful.  NULL
**  pointer when unsuccessful.
**
*************************************************************************/
TO_PriorityQueue_t* TO_MessageFlow_GetPQueue(TO_ChannelData_t *channel, 
         TO_MessageFlow_t *MsgFlow, uint32 *Index);


/************************************************************************/
/** \brief Add a message flow.
**
**  \par Description
**       This function adds message flow, if the message flow is not
**       found on a given channel.
**
**  \param [in]   ChannelIdx    Index for a given channel
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
osalbool TO_MessageFlow_Add(uint16 ChannelIdx, CFE_SB_MsgId_t MsgID, uint16 MsgLimit, uint16 PQueueIdx);


/************************************************************************/
/** \brief Remove a message flow.
**
**  \par Description
**       This function removes a message flow from a given channel if it
**       is found.
**
**  \param [in]   ChannelIdx    Index for a given channel
**                              Ground = 0, Radio = 1, Storage = 2
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
** 
**  \return
**  TRUE if message flow is found and removed.  FALSE if not.
**
*************************************************************************/
osalbool TO_MessageFlow_Remove(uint16 ChannelIdx, CFE_SB_MsgId_t MsgID);


/************************************************************************/
/** \brief Query a message flow.
**
**  \par Description
**       This function queries a message flow to determine if it exists
**       on a given channel.
**
**  \param [in]   ChannelIdx    Index for a given channel
**                              Ground = 0, Radio = 1, Storage = 2
**
**  \param [in]   MsgID         A #CFE_SB_MsgId_t that references the
**                              software bus message.
** 
**  \return
**  TRUE if message flow is found.  FALSE if not found.
**
*************************************************************************/
osalbool TO_MessageFlow_Query(uint16 ChannelIdx, CFE_SB_MsgId_t MsgID);



#endif
