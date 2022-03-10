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

#ifndef TO_OUTPUT_QUEUE_H
#define TO_OUTPUT_QUEUE_H

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

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief Definition for a single output channel queue entry.
*/
typedef struct
{
    /** \brief OSAL queue descriptor */
    uint32                  OSALQueueID;
    
    /** \brief Count of the number of bytes sent from the output queue */
    uint32                  SentBytes;
    
    /** \brief Count of the number of messages sent from the output queue */
    uint32                  SentCount;
    
    /** \brief Count of the number of messages currently queued on output queue */
    uint16                  CurrentlyQueuedCnt;
    
    /** \brief Count of the number of bytes sent from the output queue */
    uint16                  HighwaterMark;

    /** \brief Memory pool buffer for queued messages. */
    uint8                   MemPoolBuffer[TO_NUM_BYTES_IN_MEM_POOL];

    /** \totlmmnemonic  TO_MEMPOOLHNDL
        \brief Memory pool handle for queued messages. */
    uint32                  MemPoolHandle;

    /** \totlmmnemonic  TO_MEMINUSE
        \brief The total memory currently in use */
    uint32                  MemInUse;

    /** \totlmmnemonic  TO_PKMEMINUSE
        \brief The peak memory in use */
    uint32                  PeakMemInUse;

    /**\brief  CF Throttling Semaphore ID*/
    uint32                  CfCntSemId;

    /**\brief  CF Throttling Semaphore Max Count*/
    uint32                  CfCntSemMax;

    /** \brief CF Throttling Semaphore Name*/
    char                    CfCntSemName[OS_MAX_API_NAME];

    void                   *MsgScratchPad;

    uint32                  QueuedMsgCount;


    /** \brief The number of times a message was not queued due to memory pool being full. */
    uint32                  MemFullCount;

} TO_OutputQueue_t;


/************************************************************************
** External Global Variables
*************************************************************************/



/************************************************************************/
/** \brief Cleanup all output channels.
**
**  \par Description
**       This function is call at application termination and empties
**       the queue, deallocating each message its popped off the queue.
**
*************************************************************************/
void  TO_OutputQueue_CleanupAll(TO_ChannelData_t* channel);



/************************************************************************/
/** \brief Resets all metrics.
**
**  \par Description
**       This function is called at when the Reset command is received.
**       It resets all metrics, which include the Sent Counts and the
**       Highwater Marks.
**
*************************************************************************/
void  TO_OutputQueue_ResetCounts(TO_ChannelData_t* channel);



/************************************************************************/
/** \brief Teardown all output channel queues.
**
**  \par Description
**       This function is called at when the application is about to load
**       a new configuration table.  This will flush the queues and
**       deallocate the messages contained in the queues.
**
*************************************************************************/
int32 TO_OutputQueue_Teardown(TO_ChannelData_t* channel);



/************************************************************************/
/** \brief Buildup all output channel queues.
**
**  \par Description
**       This function is called at when the application has loaded a new
**       configuration table.
**
**  \returns
**  0 if no error occurred.  On error, an OSAL error is returned
**  indicating what error occured.
**  \endreturns
**
*************************************************************************/
int32 TO_OutputQueue_Buildup(TO_ChannelData_t* Channel, const char* CfCntSemName,
		                     uint32 CfCntSemMax);



/************************************************************************/
/** \brief Queue message.
**
**  \par Description
**       This function is called by the Scheduler to queue a message
**       for downlink.
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message

**
**  \param [in]   OutChannel    A #TO_TlmOutputChannelQueue_t pointer to
**                              the output channel object.
**
**  \returns
**  0 if successful.  OSAL error if unsuccessful.
**  \endreturns
**
*************************************************************************/
int32 TO_OutputQueue_QueueMsg(TO_ChannelData_t* channel, CFE_SB_MsgPtr_t MsgPtr);



/************************************************************************/
/** \brief Query.
**
**  \par Description
**       This function is called when a ground command is received to
**       query an output channel.  This will raise an information event
**       containing text with Sent Count and Highwater Mark.
**
**  \param [in]   OutputChannelIdx  Index of the output channel to query.
**
**  \returns
**  TRUE if successful.  FALSE if an error occurred.
**  \endreturns
**
*************************************************************************/
boolean TO_OutputChannel_Query(uint16 ChannelIdx);

/* TODO:  Add Doxygen */
osalbool TO_OutputQueue_IsFull(TO_ChannelData_t *channel);
int32    TO_OutputQueue_Init(TO_ChannelData_t* channel);
int32    TO_OutputQueue_GetMsg(TO_ChannelData_t *channel, CFE_SB_MsgPtr_t *MsgPtr, int32 Timeout );


#endif
