#ifndef TO_MISSION_CFG_H
#define TO_MISSION_CFG_H

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
#define TO_MAX_MESSAGE_FLOWS       (180)

/**
 * \brief Maximum number of Priority Queues.
 *
 * \note Limits:
 *       There has to be at least one valid Priority Queue entry. However,
 *       if a following Priority Queue entry is invalid, the entire table
 *       is invalid.
 *       Zero Priority Queue table entries will result in an invalid table.
 */
#define TO_MAX_PRIORITY_QUEUES     (8)

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
#define TO_MAX_QUEUE_SIZE_LIMIT    (128)

/**
 * \brief Maximum number of messages in a Message Flow.
 *
 * \note Limits:
 *       This maximum number is arbitrary. Minimum is 1.
 *       The maximum MsgLimit is limited by the available memory. 
 */
#define TO_MAX_PQ_MSG_SIZE_LIMIT   (64)

#ifdef __cplusplus
}
#endif

#endif /* TO_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
