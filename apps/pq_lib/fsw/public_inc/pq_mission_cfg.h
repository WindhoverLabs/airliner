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

#ifndef PQ_MISSION_CFG_H
#define PQ_MISSION_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** TO Mission Configuration Parameter Definitions
*/

/**
 * @brief Maximum number of Message Flows.
 *
 * \note Limits:
 *       Zero Message Flows table entries will not invalidate the table.
 *       If the Message Flows table has one invalid entry the entire
 *       table is invalid.
 */
#define PQ_MAX_MESSAGE_FLOWS       (200)

/**
 * \brief Maximum number of Priority Queues.
 *
 * \note Limits:
 *       There has to be at least one valid Priority Queue entry. However,
 *       if a following Priority Queue entry is invalid, the entire table
 *       is invalid.
 *       Zero Priority Queue table entries will result in an invalid table.
 */
#define PQ_MAX_PRIORITY_QUEUES     (8)

/**
 * \brief Maximum number of messages in a Priority Queue.
 *
 * \note  Limits:
 *        This maximum number is arbitrary. Minimum is 1.
 *        This maximum value is limited by the available memory.
 *
 *        The local rules for setting this value are as follows:
 *
 *        The sum of all the msglimits in a priority queue should not 
 *        be greater than the msglimit of that priority queue.
 * 
 *        Another rule of thumb is to limit to small values.
 *  
 */
#define PQ_MAX_QUEUE_SIZE_LIMIT    (128)

/**
 * \brief Maximum number of messages in a Message Flow.
 *
 * \note Limits:
 *       This maximum number is arbitrary. Minimum is 1.
 *       The maximum MsgLimit is limited by the available memory. 
 */
#define PQ_MAX_PQ_MSG_SIZE_LIMIT   (64)

#ifdef __cplusplus
}
#endif

#endif /* PQ_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
