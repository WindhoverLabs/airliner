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

#ifndef PQ_CHANNEL_H
#define PQ_CHANNEL_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "pq_structs.h"
#include "pq_errors.h"

/************************************************************************
** Functions
*************************************************************************/

/************************************************************************/
/** \brief Configures and initializes a single channel.
**
**  \par Description
**       Initializes channel specific data and structures. Sets up channel pipe
**       as well as initializing the cf throttling semaphore for each channel.
**
**  \par Assumptions, External Events, and Notes:
**       Semaphore name must be unique among channels; initialization will fail if two
**       channels attempt to use the same semaphore.
**       The pointers for ChannelName, ConfigTableName, ConfigTableFileName,
**       DumpTableName, and CfCntSemName are not checked for NULL since those
**       are passed in as arguments that have been macro defined in pq_platform_cfg.h
**       and are checked in a verify header to ensure the macros exist.
**
**       Only the index is range-checked and no other argument checking is performed. 
**
**  \param[in]   Channel
**
**  \param[in]   ChannelName
**                Name of channel, will be prefixed with PQ_ .
**                NOTE: Since each channel has a child task thread with the same name,
**                and in RTEMS threads must have names of 4 chars, this param should be a single char.
**
**                The 4-char limit is an RTEMS limit.  The RTEMS OSAL only takes the first 4 chars of 
**                the name per RTEMS limits for the underlying RTEMS message queue but still 
**                preserves the full name given to it in the OSAL
**                The 1-char limit helps with keeping the name short enough to view it via 
**                RTEMS reports
**
**  \param[in]   ConfigTableName
**                Name of the config table for the channel as referred to by TBL services
**
**  \param[in]   ConfigTableFileName
**                File path of the channel config table
**
**  \param[in]   DumpTableName
**                Name of the dump table for the channel as referred to by TBL services
**
**  \param[in]   CfCntSemMax
**                The initial maximum value of the CF throttling semaphore for this channel.
**
**  \param[in]   CfCntSemName
**                Name of the CF throttling semaphore, must be unique among each channel.
**
**  \return
**  CFE_SUCCESS if successful. OSAL error if unsuccessful.
**
*************************************************************************/
int32 PQ_Channel_OpenChannel(PQ_ChannelData_t *Channel, const char *ChannelName,
        const char *ConfigTableName, const char *ConfigTableFileName,
        PQ_ChannelTbl_t *BackupTblPtr, const char *DumpTableName,
        const uint32 CfCntSemMax, const char *CfCntSemName);


/************************************************************************/
/**
** \brief Forwards all channel's telemetry data to PQ_Classifier
**        and PQ_Scheduler
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void PQ_Channel_ProcessTelemetryAll(void);


/************************************************************************/
/**
** \brief Resets all channel's message flow, priority queue,
**        and output queue counters.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void PQ_Channel_ResetCountsAll(void);


/************************************************************************/
/**
** \brief The mutex object referenced by the channel pointer shall be locked 
**        by calling this function.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void PQ_Channel_LockByRef(PQ_ChannelData_t *Channel);


/************************************************************************/
/**
** \brief The mutex object referenced by the channel pointer shall be unlocked 
**        by calling this function.
**
** \par Assumptions, External Events, and Notes:
**      Assumes channel pointer is not NULL
*************************************************************************/
void PQ_Channel_UnlockByRef(PQ_ChannelData_t *Channel);


/************************************************************************/
/**
** \brief Returns a input channel's state.
**
** \par Assumptions, External Events, and Notes:
**      If the input index is out of range, the channel state is PQ_CHANNEL_UNKNOWN.
**
**  \param[in]   index
**               Index of the channel
**
**  \return
**  PQ_CHANNEL_CLOSED, PQ_CHANNEL_OPENED, or PQ_CHANNEL_UNKNOWN
**  
*************************************************************************/
uint8 PQ_Channel_State(PQ_ChannelData_t *Channel);


/************************************************************************/
/** \brief Flush priority and output queues for the channel.
**
**  \par Description
**       This function is called by command from the operators, and will wipe
**       all messages from output queue and priority queue buffers without resetting
**       the current table.
**
**  \param[in]   index
**               Index of the channel
**
**  \return
**  \retcode TRUE  \retdesc The command succeeded \endcode
**  \retcode FALSE \retdesc The command failed, if index is greater than or equal
**                          to PQ_MAX_CHANNELS or if #PQ_PriorityQueue_TeardownAll
**                          or #PQ_OutputQueue_Teardown do not return CFE_SUCCESS or
**                          if #PQ_Channel_SBPipe_Dequeue_All returns FALSE \endcode  
**  
*************************************************************************/
osalbool PQ_Channel_Flush(uint16 index);


/**
 * \brief Initializes a reference channel data, sets the channel's index,
 *        and creates a channel mutex.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 * 
 * \param [in]  Index
 *              Index of the channel
 *
 * \param [in/out]   Channel
 *
 * \returns
 * CFE_SUCCESS if successful.
 * \endreturns
 */
int32 PQ_Channel_Init(uint16 Index, PQ_ChannelData_t *Channel);


/**
 * \brief Calls functions to teardown an individual channel's priority queue, 
 *        output queue, and message flow(s).  It also deletes the channel's mutex.
 *
 * \par Assumptions, External Events, and Notes:
 *      If the input index is out of range, an event is generated
 *      #PQ_CHANNEL_OUT_OF_RANGE_ERR_EID
 *
 * \param [in/out]   Channel
 *
 */
void PQ_Channel_Cleanup(PQ_ChannelData_t *Channel);


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
void PQ_Channel_ResetCounts(PQ_ChannelData_t *Channel);


/**
 * \brief Forwards a single channel's telemetry data to PQ_Classifier
 *        and PQ_Scheduler
 *
 * \par Assumptions, External Events, and Notes:
 *      Assumes channel pointer is not NULL
 *
 * \param [in]   Channel       A #PQ_ChannelData_t pointer that
 *                             references the channel data structures
 * 
 * \param [in]   DataMsgPtr    A #CFE_SB_MsgPtr_t pointer of an SB message
 * 
 */
void PQ_Channel_ProcessTelemetry(PQ_ChannelData_t *Channel, CFE_SB_MsgPtr_t DataMsgPtr);


void PQ_Channel_CopyStats(PQ_HkTlm_t *HkTlm, PQ_ChannelData_t *Channel);

#endif
