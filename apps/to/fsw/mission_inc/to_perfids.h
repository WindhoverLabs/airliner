#ifndef TO_PERFIDS_H
#define TO_PERFIDS_H


/**
 * @brief Main task performance ID
 *
 * This is logged every time the TO application main task wakes up.
 * Start is logged when the application wakes up, and stop is logged
 * immediately before it pends on a #TO_SEND_HK_MID #TO_SEND_TLM_MID
 * message.
 */
#define TO_MAIN_TASK_PERF_ID        60

/**
 * @brief Socket send performance ID
 *
 * This is logged when a child output task when the it sends a message.
 * Start is logged immediately before the socket sendto function and
 * stop is logged immediately after the sendto function completes.
 */
#define TO_SOCKET_SEND_PERF_ID      62

#endif
