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

#include "to_custom_hooks.h"
#include "to_platform_cfg.h"
#include "to_channel.h"
#include "to_custom.h"
#include <fcntl.h>
#include <errno.h>
#include "to_events.h"
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "to_table_names.h"

extern TO_ChannelTbl_t TO_EmptyConfigTable;

typedef enum {
    /* TODO:  Add Doxygen markup. */
    TO_TLMOUTENA_INF_EID = TO_EVT_CNT,
    TO_TLMOUTENA_ERR_EID,
    TO_TLMOUTSTOP_ERR_EID,
    TO_CUSTOM_EVT_CNT
} TO_CustomEventIds_t;


typedef enum
{
    TO_CHANNEL_UNUSED       = 0,
    TO_CHANNEL_DISABLED     = 1,
    TO_CHANNEL_ENABLED      = 2
} TO_TLM_ChannelMode_t;


typedef struct
{
    TO_TLM_ChannelMode_t            Mode;
    char                            IP[INET_ADDRSTRLEN];
    uint16                          DstPort;
    uint8                           Priority;
    CFE_ES_ChildTaskMainFuncPtr_t   ListenerTask;
    int                             Socket;
    uint32                          ChildTaskID;
    uint8                           State;
} TO_TlmChannel_t;


typedef struct
{
    TO_TlmChannel_t                Channel[TO_MAX_CHANNELS];
} TO_AppCustomData_t;


TO_Custom_Hooks_t TO_Custom_Test_Hooks;

TO_AppCustomData_t TO_AppCustomData;


void TO_OutputChannel_GroundChannelTask(void)
{

}


int32 TO_Custom_InitHook(void)
{
    int32 iStatus = 0;
    uint32 i = 0;
    
    memset(&TO_AppCustomData, 0, sizeof(TO_AppCustomData));
    
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    strncpy(TO_AppCustomData.Channel[0].IP, "127.0.0.1", INET_ADDRSTRLEN);
    TO_AppCustomData.Channel[0].DstPort = 5011;
    TO_AppCustomData.Channel[0].Priority = 50;
    TO_AppCustomData.Channel[0].ListenerTask = TO_OutputChannel_GroundChannelTask;
    TO_AppCustomData.Channel[0].Socket = 0;
    TO_AppCustomData.Channel[0].ChildTaskID = 0;

    iStatus = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME, TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_DUMP_TABLENAME, TO_UDP_CF_MAX_PDUS, TO_UDP_CF_THROTTLE_SEM_NAME);
    if(iStatus == CFE_SUCCESS) {
        TO_AppCustomData.Channel[0].State = TO_CHANNEL_OPENED;
    }
    
    return iStatus;
}
int32 TO_Custom_InitHook4(void)
{
    int32 iStatus = 0;
    uint32 i = 0;
    
    memset(&TO_AppCustomData, 0, sizeof(TO_AppCustomData));
    
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    strncpy(TO_AppCustomData.Channel[0].IP, "127.0.0.1", INET_ADDRSTRLEN);
    TO_AppCustomData.Channel[0].DstPort = 5011;
    TO_AppCustomData.Channel[0].Priority = 50;
    TO_AppCustomData.Channel[0].ListenerTask = TO_OutputChannel_GroundChannelTask;
    TO_AppCustomData.Channel[0].Socket = 0;
    TO_AppCustomData.Channel[0].ChildTaskID = 0;

    iStatus = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME, TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_DUMP_TABLENAME, TO_UDP_CF_MAX_PDUS, TO_UDP_CF_THROTTLE_SEM_NAME);
    
    TO_AppCustomData.Channel[0].State = TO_CHANNEL_OPENED;

    return iStatus;
}

int32 TO_Custom_InitHook5(void)
{
    int32 iStatus = 0;
    uint32 i = 0;
    
    memset(&TO_AppCustomData, 0, sizeof(TO_AppCustomData));
    
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    strncpy(TO_AppCustomData.Channel[0].IP, "127.0.0.1", INET_ADDRSTRLEN);
    TO_AppCustomData.Channel[0].DstPort = 5011;
    TO_AppCustomData.Channel[0].Priority = 50;
    TO_AppCustomData.Channel[0].ListenerTask = TO_OutputChannel_GroundChannelTask;
    TO_AppCustomData.Channel[0].Socket = 0;
    TO_AppCustomData.Channel[0].ChildTaskID = 0;

    iStatus = TO_Channel_OpenChannel(0, "UDP", TO_UDP_CONFIG_TABLENAME, TO_UDP_CONFIG_TABLE_FILENAME, &TO_EmptyConfigTable, TO_DUMP_TABLENAME, TO_UDP_CF_MAX_PDUS, TO_UDP_CF_THROTTLE_SEM_NAME);
    
    TO_AppCustomData.Channel[0].State = TO_CHANNEL_OPENED;

    return iStatus;
}
