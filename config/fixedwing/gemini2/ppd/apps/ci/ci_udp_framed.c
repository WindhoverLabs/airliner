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

#include "ci_custom.h"
#include "ci_platform_cfg.h"
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "ci_events.h"
#include <strings.h>
#include <unistd.h>
#include "tctf.h"
#include "cop1.h"
#include "tc_sync.h"

#define CI_CUSTOM_RETURN_CODE_NULL_POINTER      (-1)
#define TO_SET_OCF_DATA_CC                      (40)


typedef struct
{
    uint8	     ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    COP1_Clcw_t  clcw;  /**< COP-1 CLCW Data */
} TO_CustomSetOcfCmd_t;


typedef struct
{
    TCTF_ChannelService_t   chnlService;
    uint8                   pktBuff[CI_CUSTOM_BUFFER_SIZE];
    TO_CustomSetOcfCmd_t    clcwCmd;    /**< CLCW Update Command      */
} CI_CustomVChnl_t;

typedef struct
{
    CI_CustomVChnl_t        vChnls[CI_CUSTOM_TF_CHANNELS];
    uint8                   tfBuff[CI_CUSTOM_TF_BUFF_SIZE];
} CI_CustomMChnl_t;

typedef struct
{
    CI_CustomMChnl_t     mc;        /**< Master channel             */
    boolean              cltuRand;  /**< Is the cltu code blocks
                                          randomized                */
    uint8                cltuBuff[CI_CUSTOM_CLTU_BUFF_SIZE];
} CI_CustomPChnl_t;

typedef struct
{
    int               Socket;
    uint16            Port;
    CI_CustomPChnl_t  pc;        /**< Physical channel           */
    CFE_SB_Msg_t     *pSbMsg;
    uint8            *pSbMsgCursor;
    CFE_SB_MsgId_t    msgId;
    CFE_SB_Msg_t     *pClcwCmd;
    COP1_Clcw_t      *pClcw;
    uint16            tfScId;
    uint16            tfVcId;
	int               remainingBytes;
} CI_AppCustomData_t;

CI_AppCustomData_t CI_AppCustomData = {0, 5010};


osalbool CI_AddCustomEventFilters(uint32 *count)
{	
    return TRUE;
}


int32 CI_InitCustom(void)
{
    int32 Status = CFE_SUCCESS;
    int reuseaddr = 1;
    struct sockaddr_in address;

    if((CI_AppCustomData.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
    	CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
    		   "Socket errno: %i", errno);
    		Status = -1;
    		goto end_of_function;
    }

    setsockopt(CI_AppCustomData.Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    fcntl(CI_AppCustomData.Socket, F_SETFL, fcntl(CI_AppCustomData.Socket, F_GETFL, 0) | O_NONBLOCK);

    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(CI_AppCustomData.Port);

    if ( (bind(CI_AppCustomData.Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
    {
        CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,"Bind socket failed = %d", errno);
        Status = -1;
        goto end_of_function;
    }

    /* Populate the ChannelService Table */
    TCTF_ChannelService_t   channelCfgTblUdp[CI_CUSTOM_TF_CHANNELS] = {
        {TCTF_SERVICE_VCP, 0, CFE_SPACECRAFT_ID, 0, 0, 0, 0}
    };

    CFE_PSP_MemCpy((void *) &CI_AppCustomData.pc.mc.vChnls[0].chnlService,
                   (void *) &channelCfgTblUdp[0], sizeof(TCTF_ChannelService_t));

    /* Setup the CI Output Message (CLCW Message) */
    CFE_SB_InitMsg((CFE_SB_MsgPtr_t) &CI_AppCustomData.pc.mc.vChnls[0].clcwCmd,
                   TO_CMD_MID, sizeof(TO_CustomSetOcfCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &CI_AppCustomData.pc.mc.vChnls[0].clcwCmd,
                      TO_SET_OCF_DATA_CC);
    COP1_InitClcw(&CI_AppCustomData.pc.mc.vChnls[0].clcwCmd.clcw, 0);

    /* Initialize the managed parameters of physical channels */
    CI_AppCustomData.pc.cltuRand = CI_CUSTOM_CLTU_RANDOM;

    CFE_EVS_SendEvent(CI_ENA_INF_EID, CFE_EVS_INFORMATION,
                      "UDP command input enabled on port %u.",
                      CI_AppCustomData.Port);

end_of_function:
    return Status;

}


void CI_ReadMessage(uint8* Buffer, uint32* outSize)
{
	int32 status;
    TCTF_Hdr_t    *pTf = (TCTF_Hdr_t *) &CI_AppCustomData.pc.mc.tfBuff;

    /* Are we currently processing a frame? */
    if(CI_AppCustomData.remainingBytes <= 0)
    {
    	CI_AppCustomData.remainingBytes = sizeof(CI_AppCustomData.pc.cltuBuff);
    	/* We are not currently processing a frame. Try to read the next frame. */
    	CI_AppCustomData.remainingBytes = recv(CI_AppCustomData.Socket,
						   (char *)&CI_AppCustomData.pc.cltuBuff[0],
						   (size_t)CI_AppCustomData.remainingBytes, 0);

    	if(CI_AppCustomData.remainingBytes > 0)
    	{
			/* Get the de-randomized transfer frame from the CLTU */
			status = TC_SYNC_GetTransferFrame(CI_AppCustomData.pc.mc.tfBuff,
					                          CI_AppCustomData.pc.cltuBuff,
											  sizeof(CI_AppCustomData.pc.mc.tfBuff),
											  sizeof(CI_AppCustomData.pc.cltuBuff),
											  CI_AppCustomData.pc.cltuRand);
			if(status < 0)
			{
				/* Here we will ignore any non-cltu message. */
				CFE_EVS_SendEvent(CI_CUSTOM_ERR_EID, CFE_EVS_ERROR,
								  "Received invalid CLTU. Message ignored. %i", status);
				*outSize = 0;
				CI_AppCustomData.remainingBytes = 0;
			}
			else
			{
				/* We have a new frame.  Initialize frame processing. */
				/* Get Transfer Frame Size */
				CI_AppCustomData.tfScId = TCTF_GetScId(pTf);
				CI_AppCustomData.tfVcId = TCTF_GetVcId(pTf);
				CI_AppCustomData.pSbMsg = NULL;
				CI_AppCustomData.pSbMsgCursor = NULL;
				CI_AppCustomData.msgId = 0;
				CI_AppCustomData.pClcwCmd = (CFE_SB_Msg_t *) &CI_AppCustomData.pc.mc.vChnls[0].clcwCmd;
				CI_AppCustomData.pClcw =
					(COP1_Clcw_t *) CFE_SB_GetUserData((void *)&CI_AppCustomData.pc.mc.vChnls[0].clcwCmd);

				/* Here you would route to different virtual channels based on the
				 * tfVcId. We are not using MAP services so all channels are virtual
				 * channels. We only have one virtual channel in this example. */
				uint16 chIdx = 0;

				uint16 tfDataSize =
					TCTF_GetPayloadLength(pTf, &CI_AppCustomData.pc.mc.vChnls[chIdx].chnlService);

				if (tfDataSize > CI_CUSTOM_BUFFER_SIZE)
				{
					CFE_EVS_SendEvent(CI_CUSTOM_ERR_EID, CFE_EVS_ERROR,
									  "CI: Transfer Frame length larger than buffer. "
									  "Transfer Frame SC ID:0x%x, VC ID:0x%x dropped.",
									  CI_AppCustomData.tfScId, CI_AppCustomData.tfVcId);
					*outSize = 0;
					CI_AppCustomData.remainingBytes = 0;
				}

				/* NOTE: For MAP Service, a packet may be split over multiple TF */
				CI_AppCustomData.pSbMsg = (CFE_SB_Msg_t *) CI_AppCustomData.pc.mc.vChnls[chIdx].pktBuff;
				CI_AppCustomData.pSbMsgCursor = (uint8 *) CI_AppCustomData.pSbMsg;

				/* Process the TCTF with COP1 */
				CI_AppCustomData.remainingBytes = COP1_ProcessFrame((uint8 *) CI_AppCustomData.pSbMsg, CI_AppCustomData.pClcw, pTf,
										 &CI_AppCustomData.pc.mc.vChnls[chIdx].chnlService);

				/* Send the CLCW message for TO */
				CFE_SB_SendMsg(CI_AppCustomData.pClcwCmd);
			}
    	}
    	else
    	{
			*outSize = 0;
    	}
    }

    /* Now are we processing a frame? */
    if(CI_AppCustomData.remainingBytes > 0)
    {
	    uint16 msgSize = 0;

    	msgSize = CFE_SB_GetTotalMsgLength(CI_AppCustomData.pSbMsg);

    	CI_AppCustomData.msgId = CFE_SB_GetMsgId(CI_AppCustomData.pSbMsg);

        CI_AppCustomData.pSbMsg = (CFE_SB_Msg_t *) CI_AppCustomData.pSbMsgCursor;
        CI_AppCustomData.pSbMsgCursor += msgSize;
        CI_AppCustomData.remainingBytes -= msgSize;

        /* Note that this can be  normal behavior for MAP service.
         * MAP services may split packets over multiple MAP channels
         * We aren't using MAP service in this example. */
        if (CI_AppCustomData.remainingBytes < 0)
        {
            CFE_EVS_SendEvent(CI_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                "Incomplete packet in Transfer Frame dropped. "
                "Transfer Frame SC ID:0x%x, VC ID:0x%x, "
                "Packet ID:0x%x.", CI_AppCustomData.tfScId, CI_AppCustomData.tfVcId, CI_AppCustomData.msgId);
			*outSize = 0;
            CI_AppCustomData.remainingBytes = 0;
        }
        else
        {
        	if(msgSize > *outSize)
        	{
        		/* Command doesn't fit into buffer. */
        		/* TODO */
        	}
        	else
        	{
        		CFE_PSP_MemCpy(Buffer, CI_AppCustomData.pSbMsg, msgSize);
        		*outSize = msgSize;
        	}
        }
    }
    else
    {
		*outSize = 0;
    }
}


void CI_CleanupCustom(void)
{
    close(CI_AppCustomData.Socket);
}

