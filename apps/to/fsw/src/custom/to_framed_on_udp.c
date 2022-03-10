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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "to_custom_udp.h"
#include "to_platform_cfg.h"
#include "to_events.h"
#include "io_lib_utils.h"
#include "tm_sdlp.h"
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/************************************************************************
** Local Defines
*************************************************************************/
#define TO_CUSTOM_CHANNEL_GET_TIMEOUT (500) /* msec */


/************************************************************************
** Local Function Definitions
*************************************************************************/



TO_AppCustomData_t     TO_AppCustomData;

TO_EnableChannelCmd_t  TO_EnableChannelCmd_S;
TO_DisableChannelCmd_t TO_DisableChannelCmd_S;

extern TO_ChannelTbl_t TO_BackupConfigTbl;


/*
** Local Variables
*/
static uint8           idlePattern[32];
static const uint16    iCaduSize = TO_CUSTOM_TF_SIZE + TM_SYNC_ASM_SIZE;



/* Set channel config table */
TM_SDLP_ChannelConfig_t chnlConfig[] =
{
    {0, 0, 0, 0, 0, 0, TO_CUSTOM_TF_OVERFLOW_SIZE}
};


void TO_OutputChannel_FrameSend(uint32 ChannelIdx);


uint8 TO_OutputChannel_Status(uint32 Index)
{
    /* Use Index, instead of index, because vxworks-6.9/target/h/string.h:100
     * has a global named "index". */
    return TO_AppCustomData.Channel[Index].Mode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Initialize All.  Nothing to do here.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_Custom_Init(void)
{
    int32 iStatus = 0;
    uint32 i = 0;

    /* Initialize Idle pattern as pseudo-random sequence. */
    IO_LIB_UTIL_GenPseudoRandomSeq(TO_AppCustomData.idleBuff, 0xa9, 0xff);

    /* Initialize Idle packet with repeating idle pattern */
    TM_SDLP_InitIdlePacket((CFE_SB_MsgPtr_t)TO_AppCustomData.idleBuff, idlePattern,
                           TO_CUSTOM_TF_IDLE_SIZE, 255);

    TO_AppCustomData.mcConfig.scId        = CFE_SPACECRAFT_ID;
    TO_AppCustomData.mcConfig.frameLength = TO_CUSTOM_TF_SIZE;
    TO_AppCustomData.mcConfig.hasErrCtrl  = TO_CUSTOM_TF_ERR_CTRL;
    TO_AppCustomData.mcFrameCnt           = 0;

    CFE_PSP_MemCpy((void *) &TO_AppCustomData.vcConfig, (void *) &chnlConfig,
                   sizeof(TO_AppCustomData.vcConfig));

    if (TM_SDLP_InitChannel(&TO_AppCustomData.frameInfo,
                            &TO_AppCustomData.buffer[TM_SYNC_ASM_SIZE],
                            TO_AppCustomData.ofBuff,
                            &TO_AppCustomData.mcConfig,
                            &TO_AppCustomData.vcConfig) < 0)
    {
        iStatus = -1;
        goto end_of_function;
    }

    iStatus = TM_SDLP_StartFrame(&TO_AppCustomData.frameInfo);
    if(iStatus != TM_SDLP_SUCCESS)
    {
        iStatus = -1;
        goto end_of_function;
    }

    /*
     * UDP development interface
     */
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    strncpy(TO_AppCustomData.Channel[0].IP, TO_UDP_CHANNEL_ADDRESS, INET_ADDRSTRLEN);
    TO_AppCustomData.Channel[0].DstPort = TO_UDP_CHANNEL_PORT;
    TO_AppCustomData.Channel[0].Priority = TO_CHANNEL_TASK_PRIORITY;
    TO_AppCustomData.Channel[0].ListenerTask = TO_OutputChannel_UDPChannelTask;
    TO_AppCustomData.Channel[0].Socket = 0;
    TO_AppCustomData.Channel[0].ChildTaskID = 0;

    /* Ground dev interface is optional */
    iStatus = TO_Channel_OpenChannel(
                      0,
                      TO_UDP_CHANNEL_NAME,
                      TO_UDP_CONFIG_TABLENAME,
                      TO_UDP_CONFIG_TABLE_FILENAME,
                      &TO_BackupConfigTbl,
                      TO_UDP_DUMP_TABLENAME, 
                      1, 
                      TO_UDP_CF_THROTTLE_SEM_NAME);

    for (i=0; i < TO_MAX_CHANNELS; i++)
    {
        if(TO_OutputChannel_Status(i) == TO_CHANNEL_ENABLED)
        {
            if(TO_OutputChannel_Enable(i, TO_AppCustomData.Channel[i].IP, TO_AppCustomData.Channel[i].DstPort))
            {
                TO_AppCustomData.Channel[i].Mode = TO_CHANNEL_DISABLED;
            }
            else
            {
                CFE_EVS_SendEvent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION,
                        "UDP telemetry output enabled channel %u to %s:%u",
                        (unsigned int)i, TO_AppCustomData.Channel[i].IP,
                        (unsigned int)TO_AppCustomData.Channel[i].DstPort);
            }
        }
    }

end_of_function:
    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Send.  Send the message out the socket.                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputChannel_Send(uint32 ChannelID, const char* Buffer, uint32 Size)
{
    struct sockaddr_in s_addr;
    int    status = 0;
    int32  returnCode = 0;

    bzero((char *) &s_addr, sizeof(s_addr));
    s_addr.sin_family      = AF_INET;

    if((Size > 0) && (ChannelID < TO_MAX_CHANNELS))
    {
        TO_TlmChannel_t *channel = &TO_AppCustomData.Channel[ChannelID];

        if(channel->Mode == TO_CHANNEL_ENABLED)
        {
            CFE_ES_PerfLogEntry(TO_SOCKET_SEND_PERF_ID);
            /* Send message via UDP socket */
            s_addr.sin_addr.s_addr = inet_addr(channel->IP);
            s_addr.sin_port        = htons(channel->DstPort);
            status = sendto(channel->Socket, (char *)Buffer, Size, 0,
                                    (struct sockaddr *) &s_addr,
                                     sizeof(s_addr));
            if (status < 0)
            {
                CFE_EVS_SendEvent(TO_TLMOUTSTOP_ERR_EID,CFE_EVS_ERROR,
                            "L%d TO sendto errno %d.", __LINE__, errno);
                    channel->Mode = TO_CHANNEL_DISABLED;
                returnCode = -1;
            }

            CFE_ES_PerfLogExit(TO_SOCKET_SEND_PERF_ID);
        }
    }

    return returnCode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Cleanup All.  Disable all the enable channels so we      */
/* don't try sending messages when the sealed framework is torn    */
/* down.                                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_OutputChannel_CustomCleanupAll(void)
{
    uint32 i = 0;

    for (i=0; i < TO_MAX_CHANNELS; i++)
    {
        if(TO_OutputChannel_Status(i) == TO_CHANNEL_ENABLED)
        {
            TO_OutputChannel_Disable(i);
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Buildup All.  Enable all the 'enabled' channels.  If it  */
/* fails, disable it.                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputChannel_CustomBuildupAll(uint32 index)
{
    return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Teardown All.  Nothing to do here.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputChannel_CustomTeardown(uint32 index)
{
    return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Custom Commands.  Raise an error event if the       */
/* command is unknown.                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_OutputChannel_ProcessNewCustomCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uint16 inSize = CFE_SB_GetTotalMsgLength(MsgPtr);
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        OS_MutSemTake(TO_AppData.MutexID);
        switch (uiCmdCode)
        {
            case TO_ENABLE_CHANNEL_CC:
            {
                TO_EnableChannelCmd_t *cmd = (TO_EnableChannelCmd_t*) MsgPtr;
                /* Validate arguments. */
                if(inSize != sizeof(TO_EnableChannelCmd_t))
                {
                    TO_AppData.HkTlm.CmdErrCnt++;
                    (void) CFE_EVS_SendEvent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                                      "Invalid message length.  Received %u.  Expected %u.",
                                      (unsigned int)inSize, sizeof(TO_EnableChannelCmd_t));
                    break;
                }

                if(TO_OutputChannel_Enable(cmd->ChannelID, cmd->DestinationAddress, cmd->DestinationPort))
                {
                    TO_AppData.HkTlm.CmdErrCnt++;
                    break;
                }

                TO_AppData.HkTlm.CmdCnt++;
                (void) CFE_EVS_SendEvent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION,
                                  "Enabled channel %u to %s:%u.",
                                  cmd->ChannelID,
                                  cmd->DestinationAddress,
                                  cmd->DestinationPort);
                break;
            }

            case TO_DISABLE_CHANNEL_CC:
            {
                TO_DisableChannelCmd_t *cmd = (TO_DisableChannelCmd_t*) MsgPtr;
                /* Validate arguments. */
                if(inSize != sizeof(TO_DisableChannelCmd_t))
                {
                    TO_AppData.HkTlm.CmdErrCnt++;
                    (void) CFE_EVS_SendEvent(TO_MSG_LEN_ERR_EID, CFE_EVS_ERROR,
                                      "Invalid message length.  Received %u.  Expected %u.",
                                      (unsigned int)inSize, sizeof(TO_DisableChannelCmd_t));
                    break;
                }

                if(TO_OutputChannel_Disable(cmd->ChannelID))
                {
                    TO_AppData.HkTlm.CmdErrCnt++;
                    break;
                }

                TO_AppData.HkTlm.CmdCnt++;
                break;
            }

            default:
                TO_AppData.HkTlm.CmdErrCnt++;
                (void) CFE_EVS_SendEvent(TO_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
        OS_MutSemGive(TO_AppData.MutexID);
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Enable Channel.  This will bind the socket for transmission.    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputChannel_Enable(uint32 ChannelID, const char *DestinationAddress, uint16 DestinationPort)
{
    int32 returnCode = 0;
    uint32 i = 0;
    struct sockaddr_in servaddr;
    int status;
    int reuseaddr=1;

    if(DestinationAddress == 0)
    {
        CFE_EVS_SendEvent(TO_TLMOUTENA_ERR_EID, CFE_EVS_ERROR,
                "Destination address for channel %u is null.", (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    if(ChannelID >= TO_MAX_CHANNELS)
    {
        CFE_EVS_SendEvent(TO_TLMOUTENA_ERR_EID, CFE_EVS_ERROR,
                "ChannelID (%u) invalid.", (unsigned int)ChannelID);
        returnCode = -1;
        goto end_of_function;
    }

    if((TO_AppCustomData.Channel[ChannelID].Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_DISABLED;
        CFE_EVS_SendEvent(TO_TLMOUTSOCKET_ERR_EID, CFE_EVS_ERROR,
                "TLM socket errno: %i on channel %u", errno, (unsigned int)ChannelID);
        returnCode = -1;
        goto end_of_function;
    }

    /* Set the Reuse Address flag.  If we don't set this flag, the socket will
     * lock the port on termination and the kernel won't unlock it until it
     * times out after a minute or so.
     */
    setsockopt(TO_AppCustomData.Channel[i].Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    strncpy(TO_AppCustomData.Channel[ChannelID].IP, DestinationAddress, sizeof(TO_AppCustomData.Channel[ChannelID].IP));
    TO_AppCustomData.Channel[ChannelID].DstPort = DestinationPort;

    /* Set the input arguments to the socket bind.
     */
    bzero((void*)&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port=0;
    status = bind(TO_AppCustomData.Channel[ChannelID].Socket,
            (struct sockaddr *)&servaddr,sizeof(servaddr));
    if(status < 0)
    {
        CFE_EVS_SendEvent(TO_TLMOUTSOCKET_ERR_EID, CFE_EVS_ERROR,
                "TLM bind errno: %i on channel %u", errno, (unsigned int)i);
        TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_DISABLED;
        returnCode = -1;
        goto end_of_function;
    }

    /* Enable the channel for transmission. */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;

    /* Create the child listener task. */
    char TaskName[OS_MAX_API_NAME];
    snprintf(TaskName, OS_MAX_API_NAME, "TO_OUTCH_%u", (unsigned int)ChannelID);
    returnCode = CFE_ES_CreateChildTask(
            &TO_AppCustomData.Channel[ChannelID].ChildTaskID,
            (const char *)TaskName,
            TO_AppCustomData.Channel[ChannelID].ListenerTask,
            0,
			TO_CUSTOM_TASK_STACK_SIZE,
            TO_AppCustomData.Channel[ChannelID].Priority,
			TO_CUSTOM_CHILD_TASK_FLAGS);

end_of_function:
    return returnCode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Disable channel.  Close the socket.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_OutputChannel_Disable(uint32 ChannelID)
{
    int32 returnCode = 0;
    uint32 i = 0;

    if(TO_AppCustomData.Channel[ChannelID].Mode != TO_CHANNEL_ENABLED)
    {
        CFE_EVS_SendEvent(TO_TLMOUTDIS_ERR_EID, CFE_EVS_ERROR,
                        "UDP telemetry for channel %u is not enabled.", (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    /* Disable the channel before we close the socket so if the handler
     * task is in the loop it will know the reason why the send
     * function failed is because the channel is disabled.
     */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_DISABLED;
    close(TO_AppCustomData.Channel[ChannelID].Socket);
    TO_AppCustomData.Channel[ChannelID].Socket = 0;

    CFE_EVS_SendEvent(TO_TLMOUTDIS_INF_EID, CFE_EVS_INFORMATION,
                      "Disabled channel %u.",
                      (unsigned int)ChannelID);

end_of_function:
    return returnCode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* The UDP Development Channel Task Entry Point                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_OutputChannel_UDPChannelTask(void)
{
    CFE_ES_RegisterChildTask();

    TO_OutputChannel_ChannelHandler(0);

    CFE_ES_ExitChildTask();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Channel Handler                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TO_OutputChannel_ChannelHandler(uint32 ChannelIdx)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_MsgPtr_t msg;

    while(TO_OutputChannel_Status(ChannelIdx) == TO_CHANNEL_ENABLED)
    {
        if(TO_Channel_State(ChannelIdx) == TO_CHANNEL_OPENED)
        {
        	iStatus = TO_OutputQueue_GetMsg(&TO_AppData.ChannelData[ChannelIdx], &msg, TO_CUSTOM_CHANNEL_GET_TIMEOUT );
            if(iStatus == OS_SUCCESS)
            {
            	/* Add packet to the outgoing frame */
                iStatus = TM_SDLP_AddPacket(&TO_AppCustomData.frameInfo, msg);
                if (iStatus < 0)
                {
                    /* TODO: React appropriately. */
                }
                else if(0 == iStatus)
                {
                	TO_OutputChannel_FrameSend(ChannelIdx);
                }

                TO_Channel_LockByIndex(ChannelIdx);
                TO_AppData.ChannelData[ChannelIdx].OutputQueue.SentCount++;
                TO_Channel_UnlockByIndex(ChannelIdx);
            }
            else if(iStatus == OS_QUEUE_TIMEOUT)
            {
            	TO_OutputChannel_FrameSend(ChannelIdx);
            }
            else
            {
                CFE_EVS_SendEvent(TO_OSQUEUE_GET_ERROR_EID, CFE_EVS_ERROR,
                                "Listener failed to pop message from queue. (%i).", (int)iStatus);
                TO_Channel_LockByIndex(ChannelIdx);
                TO_AppData.ChannelData[ChannelIdx].State = TO_CHANNEL_CLOSED;
                TO_Channel_UnlockByIndex(ChannelIdx);
            }
        }
    }
}



void TO_OutputChannel_FrameSend(uint32 ChannelIdx)
{
    int32    status = CFE_SUCCESS;
	osalbool sendMessage = TRUE;
    int32    caduSize = 0;

	/* The frame isn't full, but go ahead and send it anyway. But,
	 * first check if there is packets, otherwise, fill with OID. */
    status = TM_SDLP_FrameHasData(&TO_AppCustomData.frameInfo);
    if(1 == status)
    {
        /* Add an idle packet to fill remaining free space */
        status = TM_SDLP_AddIdlePacket(&TO_AppCustomData.frameInfo, (CFE_SB_MsgPtr_t)TO_AppCustomData.idleBuff);
    }
    else if (0 == status)
    {
        /* Set frame as Only Idle Data (OID) */
        status = TM_SDLP_SetOidFrame(&TO_AppCustomData.frameInfo, (CFE_SB_MsgPtr_t)TO_AppCustomData.idleBuff);
    }

    /* Complete the frame */
	status = TM_SDLP_CompleteFrame(&TO_AppCustomData.frameInfo, &TO_AppCustomData.mcFrameCnt, &TO_AppCustomData.ocfBuff[0]);
    if(status != TM_SDLP_SUCCESS)
	{
        /* TODO: React appropriately. */
    	sendMessage = FALSE;
    }

    /* Synchronize frame into CADU */
    caduSize = TM_SYNC_Synchronize(&TO_AppCustomData.buffer[0], TM_SYNC_ASM_STR,
                                    TM_SYNC_ASM_SIZE,
                                    TO_CUSTOM_TF_SIZE,
                                    TO_CUSTOM_TF_RANDOMIZE);
    if (caduSize < 0)
    {
        /* TODO: React appropriately. */
    	sendMessage = FALSE;
    }

    if(TRUE == sendMessage)
    {
        int32 sendResult = TO_OutputChannel_Send(ChannelIdx, &TO_AppCustomData.buffer[TM_SYNC_ASM_SIZE], caduSize - TM_SYNC_ASM_SIZE);
        if (sendResult != 0)
        {
        	TO_OutputChannel_Disable(ChannelIdx);
        }
    }

    /* Start a new frame. */
    status = TM_SDLP_StartFrame(&TO_AppCustomData.frameInfo);
    if(status != TM_SDLP_SUCCESS)
    {
        /* TODO: React appropriately. */
    }
}


int32 TO_Custom_InitEvent(int32 *ind)
{
    return 0;
}


void TO_PrintCustomVersion(void)
{

}
