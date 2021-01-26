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

#ifndef _PX4BR_APP_H_
#define _PX4BR_APP_H_

/*
 ** Include Files
 */

#include "common_types.h"
#include "cfe_time.h"
#include "cfe_evs.h"
#include "cfe_es.h"
#include "cfe_sb.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "osapi.h"
#include "px4br_cmds.h"
#include "px4br_msg.h"
#include "px4br_events.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 ** Local Defines
 */
#define PX4BR_VOL_PEER_FILENAME         "/ram/apps/PX4BridgeData.dat"
#define PX4BR_NONVOL_PEER_FILENAME      "/cf/apps/PX4BridgeData.dat"
#define PX4BR_PEER_FILE_LINE_SIZE       128
#define PX4BR_MAX_NETWORK_PEERS         6
#define PX4BR_PEER_PIPE_DEPTH           64
#define PX4BR_DEFAULT_MSG_LIM           8
#define PX4BR_ITEMS_PER_FILE_LINE       3
#define PX4BR_SUB_PIPE_DEPTH            256
#define PX4BR_MAX_ONESUB_PKTS_ON_PIPE   256
#define PX4BR_MAX_ALLSUBS_PKTS_ON_PIPE  64

#define PX4BR_MAX_MSG_SIZE              1500
#define PX4BR_MAX_SUBS_PER_PEER         256
#define PX4BR_MAX_PEERNAME_LENGTH       8

#define PX4BR_SIZE_FIELD_LENGTH         2
#define PX4BR_NAME_FIELD_LENGTH         50

#define PX4BR_SCH_PIPE_DEPTH      		2
#define PX4BR_CMD_PIPE_DEPTH      		2
#define PX4BR_ROUTER_PIPE_DEPTH    		PX4BR_MAX_SUBS_PER_PEER
#define PX4BR_SCH_PIPE_NAME       		"PX4BR_SCH_PIPE"
#define PX4BR_CMD_PIPE_NAME       		"PX4BR_CMD_PIPE"
#define PX4BR_ROUTER_PIPE_NAME     		"PX4BR_ROUTER_PIPE"

#define PX4BR_ROUTER_PIPE_TIMEOUT		1000

#define PX4BR_OK                        0
#define PX4BR_ERROR                     (-1)

/*
 ** Local Structures
 */

typedef enum {
	PX4BR_PEER_STATE_UNUSED,
	PX4BR_PEER_STATE_UNINITIALIZED,
	PX4BR_PEER_STATE_IDLE
} PX4BR_PeerState_t;

typedef struct {
    char            	Name[PX4BR_MAX_PEERNAME_LENGTH];
    int 				Socket;
    struct sockaddr_in	ServAddr;
    PX4BR_PeerState_t   State;
    uint32				ListenerTaskID;
    uint32				DataOutTaskID;
    boolean				IsConnected;
} PX4BR_Peer_t;

typedef struct
{
    PX4BR_HkPacket_t  	HkMsg;
    CFE_SB_PipeId_t 	SchPipe;
    CFE_SB_PipeId_t 	CmdPipe;
    CFE_SB_PipeId_t 	RouterPipe;
    uint32          	RunStatus;
    PX4BR_Peer_t   		Peer[PX4BR_MAX_NETWORK_PEERS];
    PX4BR_Peer_t		*InitializingPeer;
    uint32				InitializingPeerCount;
    char      			AppName[OS_MAX_API_NAME];
    uint32				AppID;
} PX4BR_AppData_t;



#endif  /* _PX4BR_APP_H_ */

/* ---------- end of file eta_app.h ----------*/
