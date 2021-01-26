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

#ifndef TO_TBLDEFS_H
#define TO_TBLDEFS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "to_mission_cfg.h"
#include "to_platform_cfg.h"
#include "to_message_flow.h"
#include "to_priority_queue.h"
#include "to_output_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
/**
 * \brief Defines the number of entries in the table
 */
#define TO_CONFIG_TABLE_MAX_ENTRIES  (1)

/**
 * \brief Defines the error return codes for table validation
 */
#define TO_CONFIG_TABLE_RETURN_INVALID_VERSION        (-50)
#define TO_CONFIG_TABLE_RETURN_NO_SECONDARY_HEADER    (-51)
#define TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR        (-52)
#define TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR            (-53)
#define TO_CONFIG_TABLE_NULL_PTR_ERR                  (-54)
#define TO_CONFIG_TABLE_PQUEUE_STATE_ERR              (-55)
#define TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR              (-56)
#define TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR          (-57)
#define TO_CONFIG_TABLE_NO_PQUEUES_ERR                (-58)
#define TO_CONFIG_TABLE_MSG_FLOW_FILTER_ERR           (-59)
#define TO_CONFIG_TABLE_QUEUE_ERR                     (-60)


/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \totbl TO Configuration Table 
 *
 * \brief Definition for a TO Configuration table 
 */
typedef struct
{
    /**
     * \brief  The table identifier, used to identify a specific config table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be greater than or equal to 0
     *
     * \par
     * Note: TableID value 0xFFFFFFFF is reserved for each channel's 
     * backup emergency table.  The backup emergency tables are loaded
     * when the channel's table file could not be loaded.
     * It is the largest uint32 available 4294967295.     
     */
    uint32                       TableID;

    /**
     * \brief  MessageFlow defines a single telemetry message flow entry.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: The maximum number of message flows is defined in to_mission_cfg.h
     */
    TO_MessageFlow_t             MessageFlow[TO_MAX_MESSAGE_FLOWS];

    /**
     * \brief  The Priority Queue defines a single priority queue entry.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: The maximum number of priority queues is defined in to_mission_cfg.h
     */
    TO_PriorityQueue_t           PriorityQueue[TO_MAX_PRIORITY_QUEUES];

} TO_ChannelTbl_t;


/** \brief Definition for a single TO dump table entry */
typedef struct
{
    /**
     * \brief  The table identifier, used to identify a specific dump table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be greater than or equal to 0
     */
    uint16                       TableID;

    /**
     * \brief  This MessageFlow is a structure which defines the message flow metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: TO_MAX_QUEUE_SIZE_LIMIT.
     */
    TO_MessageFlowMetrics_t      MessageFlow[TO_MAX_MESSAGE_FLOWS];

    /**
     * \brief  This PriorityQueue is a structure which defines the priority queue metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: TO_MAX_PRIORITY_QUEUES
     */
    TO_PriorityQueueMetrics_t    PriorityQueue[TO_MAX_PRIORITY_QUEUES];

    /**
     * \brief  This OutputQueue is a structure which defines the output queue metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    TO_OutputQueue_t             OutputQueue;

} TO_ChannelDumpTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* TO_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
