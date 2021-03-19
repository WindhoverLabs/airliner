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

#ifndef PQ_ERROR_H
#define PQ_ERROR_H

/* pq_channel errors */

/**
 * \brief The Channel index out of range.
 */
#define PQ_CHANNEL_OUT_OF_RANGE_ERR     (-1)

/**
 * \brief \brief The table data for channel is missing.
 */
#define PQ_CHANNEL_PTR_NULL_ERR         (-2)

/**
 * \brief The channel open failure.
 */
#define PQ_CHANNEL_OPEN_ERR             (-3)

/* pq_message_flow errors */

/** \brief A bad argument was passed to a function */
#define PQ_MESSAGE_FLOW_BAD_ARG_ERR     (-4)

/** \brief No table data is available */
#define PQ_MESSAGE_FLOW_NO_TABLE_ERR    (-5)

/* pq_priority_queue errors */

/**
 * \brief The priority queue is missing.
 */
#define PQ_NO_PRIORITY_QUEUE_ERR        (-6)

/**
 * \brief The table data for channel is missing.
 */
#define PQ_NO_TABLE_ERR                 (-7)

/**
 * \brief The message was not queued because the priority queue was full.
 */
#define PQ_PRIORITY_QUEUE_FULL_ERR      (-8)

/**
 * \brief The message was not queued because the caller could not get
 *        enough memory from the memory pool.
 */
#define PQ_MEMORY_FULL_ERR              (-9)

/** 
 * \brief Function received a null channel pointer. 
 */
#define PQ_CHANNEL_PTR_NULL             (-10)


/* Table validation errors */

/**
 * \brief Defines the error return codes for table validation
 */
#define PQ_CONFIG_TABLE_RETURN_INVALID_VERSION        (-50)
#define PQ_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER    (-51)
#define PQ_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR        (-52)
#define PQ_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR            (-53)
#define PQ_CONFIG_TABLE_NULL_PTR_ERR                  (-54)
#define PQ_CONFIG_TABLE_PQUEUE_STATE_ERR              (-55)
#define PQ_CONFIG_TABLE_PQUEUE_QTYPE_ERR              (-56)
#define PQ_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR          (-57)
#define PQ_CONFIG_TABLE_NO_PQUEUES_ERR                (-58)
#define PQ_CONFIG_TABLE_MSG_FLOW_FILTER_ERR           (-59)
#define PQ_CONFIG_TABLE_QUEUE_ERR                     (-60)

#endif
