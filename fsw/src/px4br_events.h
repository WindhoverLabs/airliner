/*==============================================================================
 Copyright (c) 2015, Windhover Labs
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of TlmOut nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PX4BR_EVENTS_H_
#define _PX4BR_EVENTS_H_

/* Event message ID's */
typedef enum
{
    PX4BR_INIT_INF_EID            			= 1,
    PX4BR_PIPE_ERR_EID       				= 2,
    PX4BR_RCVMSG_ERR_EID        			= 3,
    PX4BR_MSGID_ERR_EID          			= 4,
    PX4BR_FNCODE_ERR_EID      				= 5,
    PX4BR_CMD_ARG_INVALID_EID 				= 6,
    PX4BR_NOOP_INF_EID           			= 7,
    PX4BR_ENGAGE_TEST_MODE_EID 	            = 8,
    PX4BR_HK_CREATE_CHDTASK_ERR_EID			= 9,
    PX4BR_CMD_INVALID_MODE_EID 				= 10,
    PX4BR_CMD_RUN_FOR_TIME_EID				= 11,
    PX4BR_RUNTIME_QUEUE_ACQ_ERR_EID			= 12,
    PX4BR_STEP_FRAME_INF_EID				= 13,
    PX4BR_HK_TASK_INIT_FAILED_EID			= 14,
    PX4BR_INIT_EID,
    PX4BR_PEER_INIT_FAILED_EID,
    PX4BR_FILE_ERR_EID,
    PX4BR_PROTO_INIT_ERR_EID,
    PX4BR_VOL_FAILED_EID,
    PX4BR_NONVOL_FAILED_EID,
    PX4BR_INV_LINE_EID,
    PX4BR_FILE_OPENED_EID,
    PX4BR_CPY_ERR_EID,
    PX4BR_PEERPIPE_CR_ERR_EID,

    PX4BR_SOCK_FAIL_EID,
    PX4BR_BIND_FAIL_EID,
    PX4BR_PEERPIPE_CR_EID,
    PX4BR_SND_APPMSG_ERR_EID,
    PX4BR_APPMSG_SENT_EID,
    PX4BR_PEER_ALIVE_EID,
    PX4BR_HB_LOST_EID,
    PX4BR_STATE_ERR_EID,

    PX4BR_MSG_TRUNCATED_EID,
    PX4BR_LSC_ERR1_EID,
    PX4BR_SUBTYPE_ERR_EID,
    PX4BR_NET_RCV_PROTO_ERR_EID,
    PX4BR_SRCNAME_ERR_EID,
    PX4BR_ANN_RCVD_EID,
    PX4BR_ANN_ACK_RCVD_EID,
    PX4BR_HB_RCVD_EID,
    PX4BR_HB_ACK_RCVD_EID,
    PX4BR_SUB_RCVD_EID,
    PX4BR_UNSUB_RCVD_EID,
    PX4BR_PEERIDX_ERR1_EID,
    PX4BR_SUB_ERR_EID,
    PX4BR_DUP_SUB_EID,
    PX4BR_PEERIDX_ERR2_EID,
    PX4BR_NO_SUBS_EID,
    PX4BR_SUB_NOT_FOUND_EID,
    PX4BR_SOCK_RCV_APP_ERR_EID,
    PX4BR_SB_SEND_ERR_EID,
    PX4BR_MSGTYPE_ERR_EID,
    PX4BR_LSC_ERR2_EID,
    PX4BR_ENTRY_ERR_EID,
    PX4BR_PEERIDX_ERR3_EID,
    PX4BR_UNSUB_CNT_EID,
    PX4BR_PROTO_SENT_EID,
    PX4BR_PROTO_SEND_ERR_EID,
    PX4BR_EVT_COUNT
} PX4BR_EventEnum_t;



#endif  /* _PX4BR_EVENTS_H_ */

/* ---------- end of file eta_events.h ----------*/
