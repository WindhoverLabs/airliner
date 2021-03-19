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
#include "vc_transmit_udp.h"
#include "cfe.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/


/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

/**
 * Global data structure for custom device transmit layer
 */
VC_AppCustomData_t VC_AppCustomData;


int32 VC_CustomTransmit_InitData()
{
    int32 iStatus = CFE_SUCCESS;
    
    /* Set all struct zero values */
    bzero(&VC_AppCustomData, sizeof(VC_AppCustomData));
    
    /* Set all non-zero values for channel zero */
    VC_AppCustomData.Channel[0].Mode = VC_CHANNEL_ENABLED;
    VC_AppCustomData.Channel[0].ChannelID = 0;
    VC_AppCustomData.Channel[0].DestPort = VC_DESTINATION_PORT;
    VC_AppCustomData.Channel[0].SocketFd = 0;

    strncpy(VC_AppCustomData.Channel[0].DestIP, VC_DESTINATION_IP, INET_ADDRSTRLEN); 
    strncpy(VC_AppCustomData.Channel[0].MyIP, VC_SOURCE_IP, INET_ADDRSTRLEN); 
    
//    VC_AppCustomData.Channel[1].Mode = VC_CHANNEL_ENABLED;
//    VC_AppCustomData.Channel[1].ChannelID = 1;
//    VC_AppCustomData.Channel[1].DestPort = VC_DESTINATION_PORT + 1;
//    VC_AppCustomData.Channel[1].SocketFd = 0;
//
//    strncpy(VC_AppCustomData.Channel[1].DestIP, VC_DESTINATION_IP, INET_ADDRSTRLEN);
//    strncpy(VC_AppCustomData.Channel[1].MyIP, VC_SOURCE_IP, INET_ADDRSTRLEN);

    /* Initialize output messages*/
	CFE_SB_InitMsg(&OpticalFlowFrameMsg, FLOW_FRAME_MID,
				sizeof(PX4_OpticalFlowFrameMsg_t), TRUE);

    return (iStatus);
}


int32 VC_EnableChannel(uint8 ChannelID)
{
    int32 returnCode = 0;
    uint32 i         = 0;
    struct sockaddr_in servaddr;
    int status;
    int reuseaddr    = 1;

    if((VC_AppCustomData.Channel[ChannelID].Mode == VC_CHANNEL_ENABLED) &&
        (VC_AppCustomData.Channel[ChannelID].SocketFd != 0))
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                        "VC UDP for channel %u already enabled.", (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    if(ChannelID >= VC_MAX_OUTPUT_CHANNELS)
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                        "VC ChannelID (%u) invalid.", (unsigned int)ChannelID);
        returnCode = -1;
        goto end_of_function;
    }

    if((VC_AppCustomData.Channel[ChannelID].SocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        VC_AppCustomData.Channel[ChannelID].Mode = VC_CHANNEL_DISABLED;
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                        "VC socket errno: %i on channel %u", errno, (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    /* Set the Reuse Address flag.  If we don't set this flag, the socket will
     * lock the port on termination and the kernel won't unlock it until it
     * times out after a minute or so.
     */
    setsockopt(VC_AppCustomData.Channel[ChannelID].SocketFd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    /* Set the input arguments to the socket bind.
     */
     
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(VC_AppCustomData.Channel[ChannelID].MyPort);
    
    if (inet_aton(VC_AppCustomData.Channel[ChannelID].MyIP, &servaddr.sin_addr) == 0)
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                        "VC inet_aton errno: %i on channel %u", errno, (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }
    
    status = bind(VC_AppCustomData.Channel[ChannelID].SocketFd,
            (struct sockaddr *)&servaddr,sizeof(servaddr));
    if(status < 0)
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                        "VC bind errno: %i on channel %u", errno, (unsigned int)i);
        VC_AppCustomData.Channel[ChannelID].Mode = VC_CHANNEL_DISABLED;
        returnCode = -1;
    }

end_of_function:
    return (returnCode);
}


int32 VC_Init_CustomTransmitters(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;

    for (i=0; i < VC_MAX_OUTPUT_CHANNELS; i++)
    {
        if(VC_AppCustomData.Channel[i].Mode == VC_CHANNEL_ENABLED)
        {
            if(VC_EnableChannel(i))
            {
                returnCode = -1;
                VC_AppCustomData.Channel[i].Mode = VC_CHANNEL_DISABLED;
            }
            else
            {
                (void) CFE_EVS_SendEvent(VC_CHA_INF_EID, CFE_EVS_INFORMATION,
                        "VC UDP output enabled channel %u to %s:%u",
                        (unsigned int)i, VC_AppCustomData.Channel[i].DestIP,
                        (unsigned int)VC_AppCustomData.Channel[i].DestPort);
            }
        }
    }
    return (returnCode);
}


int32 VC_DisableChannel(uint8 ChannelID)
{
    int32 returnCode = 0;

    if(VC_AppCustomData.Channel[ChannelID].Mode != VC_CHANNEL_ENABLED)
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                            "UDP VC for channel %u is not enabled.", ChannelID);
        returnCode = -1;
        goto end_of_function;
    }

    if(-1 == close(VC_AppCustomData.Channel[ChannelID].SocketFd))
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                            "Socket close error on channel %u", ChannelID);
        returnCode = -1;
        goto end_of_function;
    }
    else
    {
        VC_AppCustomData.Channel[ChannelID].SocketFd = 0;
    }

end_of_function:
    return (returnCode);
}


int32 VC_CleanupCustom(void)
{
    uint32 i         = 0;
    int32 returnCode = 0;
    
    for(i=0; i < VC_MAX_OUTPUT_CHANNELS; i++)
    {
        if(VC_AppCustomData.Channel[i].Mode == VC_CHANNEL_ENABLED)
        {
            if(-1 == VC_DisableChannel(i))
            {
                returnCode = -1;
            }
        }
    }
    return returnCode;
}


boolean VC_Transmit_Init(void)
{
    boolean returnBool = TRUE;
    if(-1 == VC_Init_CustomTransmitters())
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, \
                "VC_Transmit_Init Failed");
        returnBool = FALSE;
    }
    return (returnBool);
}


boolean VC_Transmit_Uninit(void)
{
    boolean returnBool = TRUE;
    if(-1 == VC_CleanupCustom())
    {
        (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR, \
                "VC_Transmit_Uninit Failed");
        returnBool = FALSE;
    }
    return (returnBool);
}


void VC_Transmit_Critical_Cleanup(void)
{
    uint8 i = 0;

    for(i=0; i < VC_MAX_OUTPUT_CHANNELS; i++)
    {
        if(VC_AppCustomData.Channel[i].SocketFd != 0)
        {
            close(VC_AppCustomData.Channel[i].SocketFd);
        }
    }
    return;
}


int32 VC_SendData(uint32 ChannelID, const char* Buffer, uint32 Size)
{
    static struct sockaddr_in s_addr;
    int status       = 0;
    int32 returnCode = 0;

    bzero((char *) &s_addr, sizeof(s_addr));
    s_addr.sin_family      = AF_INET;

    if(ChannelID < VC_MAX_OUTPUT_CHANNELS)
    {
        VC_Transmit_Handle_t *channel = &VC_AppCustomData.Channel[ChannelID];

        if(channel->Mode == VC_CHANNEL_ENABLED)
        {
            CFE_ES_PerfLogEntry(VC_SOCKET_SEND_PERF_ID);
            /* Send message via UDP socket */
            s_addr.sin_addr.s_addr = inet_addr(channel->DestIP);
            s_addr.sin_port        = htons(channel->DestPort);

            status = sendto(channel->SocketFd, (char *)Buffer, Size, 0,
                                    (struct sockaddr *) &s_addr,
                                    sizeof(s_addr));

            if (status < 0)
            {
                if(errno == 90)
                {
                    (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID,CFE_EVS_ERROR,
                                "L%d VC sendto errno %d.  Message too long.  Size=%u", __LINE__, errno, (unsigned int)Size);
                }
                else
                {
                    (void) CFE_EVS_SendEvent(VC_SOCKET_ERR_EID, CFE_EVS_ERROR,
                                "L%d VC sendto errno %d.", __LINE__, errno);
                    channel->Mode = VC_CHANNEL_DISABLED;
                }
                returnCode = -1;
            }
            CFE_ES_PerfLogExit(VC_SOCKET_SEND_PERF_ID);
        }
    }
    return (returnCode);
}


boolean VC_Address_Verification(const char *Address)
{
    boolean returnBool = FALSE;
    static struct sockaddr_in s_addr;
    if(0 != inet_aton(Address, &s_addr.sin_addr))
    {
        returnBool = TRUE;
    }
    return (returnBool);
}


boolean VC_Update_Destination(const char *Address, uint16 Port)
{
    boolean returnBool = TRUE;
    uint32 i = 0;
    
    for (i=0; i < VC_MAX_OUTPUT_CHANNELS; i++)
    {
        if(VC_AppCustomData.Channel[i].Mode == VC_CHANNEL_ENABLED)
        {
            VC_AppCustomData.Channel[i].DestPort = Port;
            
            if(!strncpy(VC_AppCustomData.Channel[i].DestIP, Address, sizeof(Address)))
            {
                returnBool = FALSE;
            }
        }
    }
    return (returnBool);
}


int32 VC_Transmit_InitData()
{
    return VC_CustomTransmit_InitData();
}

