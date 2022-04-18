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

#include "to_slip_sed.h"
#include "to_platform_cfg.h"
#include "to_events.h"
#include "sedlib.h"

/************************************************************************
** Local Defines
*************************************************************************/

typedef enum
{
	TO_QUEUE_MSG_OK                       = 0,
	TO_QUEUE_MSG_BUFFER_FULL_OK           = 1,
	TO_QUEUE_MSG_BUFFER_FULL_MSG_DEFERRED = 2
} TO_QueueMsgReturnCode_t;

#define UART_NOOP_CC              (0)
#define UART_RESET_CC             (1)
#define UART_QUEUE_DATA_FOR_TX_CC (2)
#define UART_FLUSH_CC             (3)
#define UART_CONFIGURE_CC         (4)


/************************************************************************
** Local Function Definitions
*************************************************************************/



TO_AppCustomData_t     TO_AppCustomData;
TO_EnableChannelCmd_t  TO_EnableChannelCmd_S;
TO_DisableChannelCmd_t TO_DisableChannelCmd_S;

extern TO_ChannelTbl_t TO_BackupConfigTbl;



uint8 TO_OutputChannel_Status(uint32 index)
{
    return TO_AppCustomData.Channel[index].Mode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Custom Initialize All.  Nothing to do here.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TO_Custom_Init(void)
{
    int32 status = 0;
    SEDLIB_ReturnCode_t sedLibStatus = 0;
    SLIP_ReturnCode_t slipStatus = SLIP_OK;
    uint32 i = 0;

    CFE_PSP_MemSet(&TO_AppCustomData, 0, sizeof(TO_AppCustomData));

    CFE_SB_InitMsg(&TO_AppCustomData.Channel[0].UartQueueDataCmd,
    		        UART_CMD_MSG_ID, sizeof(TO_AppCustomData.Channel[0].UartQueueDataCmd), TRUE);

    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&TO_AppCustomData.Channel[0].UartQueueDataCmd, UART_QUEUE_DATA_FOR_TX_CC);

    slipStatus = SLIP_Encoder_Init(&TO_AppCustomData.Channel[0].Encoder, TO_AppCustomData.Channel[0].UartQueueDataCmd.Buffer, sizeof(TO_AppCustomData.Channel[0].UartQueueDataCmd.Buffer));
    if(slipStatus != SLIP_OK)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize SLIP encoder. (0x%08X)",
								 slipStatus);
    }

    /*
     * Msg Port Interface
     */
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    TO_AppCustomData.Channel[0].UartQueueDataCmd.Hdr.Version = 1;

    sedLibStatus = SEDLIB_GetPipe(
    		"UART1_CMD",
			sizeof(UART_QueueDataCmd_t),
			&TO_AppCustomData.Channel[0].MsgPortHandle);
    if(sedLibStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize UART MsgPort. (0x%08X)",
								 sedLibStatus);
    }
    else
    {
        CFE_EVS_SendEvent(TO_INIT_APP_INF_EID, CFE_EVS_INFORMATION,
                         "TO message port initialized.");
    }

    /* Ground dev interface is optional */
    status = TO_Channel_OpenChannel(
                      0,
                      TO_CHANNEL_NAME,
                      TO_CONFIG_TABLENAME,
                      TO_CONFIG_TABLE_FILENAME,
                      &TO_BackupConfigTbl,
                      TO_DUMP_TABLENAME,
                      1,
                      TO_CF_THROTTLE_SEM_NAME);

    for (i=0; i < TO_MAX_CHANNELS; i++)
    {
        if(TO_OutputChannel_Status(i) == TO_CHANNEL_ENABLED)
        {
            if(TO_OutputChannel_Enable(i))
            {
                TO_AppCustomData.Channel[i].Mode = TO_CHANNEL_DISABLED;
            }
            else
            {
                CFE_EVS_SendEvent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION,
                        "Telemetry output enabled channel %u",
                        (unsigned int)i);
            }
        }
    }

end_of_function:
    return status;
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

                if(TO_OutputChannel_Enable(cmd->ChannelID))
                {
                    TO_AppData.HkTlm.CmdErrCnt++;
                    break;
                }

                TO_AppData.HkTlm.CmdCnt++;
                (void) CFE_EVS_SendEvent(TO_TLMOUTENA_INF_EID, CFE_EVS_INFORMATION,
                                  "Enabled channel %u.",
                                  cmd->ChannelID);
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
int32 TO_OutputChannel_Enable(uint32 ChannelID)
{
    int32 returnCode = 0;
    uint32 i = 0;
    int status;

    if(ChannelID >= TO_MAX_CHANNELS)
    {
        CFE_EVS_SendEvent(TO_TLMOUTENA_ERR_EID, CFE_EVS_ERROR,
                "ChannelID (%u) invalid.", (unsigned int)ChannelID);
        returnCode = -1;
        goto end_of_function;
    }

    /* Enable the channel for transmission. */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_ENABLED;

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
                        "Telemetry for channel %u is not enabled.", (unsigned int)i);
        returnCode = -1;
        goto end_of_function;
    }

    /* Disable the channel before we close the socket so if the handler
     * task is in the loop it will know the reason why the send
     * function failed is because the channel is disabled.
     */
    TO_AppCustomData.Channel[ChannelID].Mode = TO_CHANNEL_DISABLED;

    CFE_EVS_SendEvent(TO_TLMOUTDIS_INF_EID, CFE_EVS_INFORMATION,
                      "Disabled channel %u.",
                      (unsigned int)ChannelID);

end_of_function:
    return returnCode;
}



int32 TO_Custom_InitEvent(int32 *ind)
{
    return 0;
}


void TO_PrintCustomVersion(void)
{

}


void TO_OutputChannel_SendTelemetry(uint32 index)
{
	if(TO_AppCustomData.Channel[index].Mode == TO_CHANNEL_ENABLED)
	{
		osalbool                cont = TRUE;
		SEDLIB_MsgReadStatus_t  readStatus;
		SEDLIB_MsgWriteStatus_t writeStatus;
		SEDLIB_ReturnCode_t     rc;

		/* Check to see if we the previous message has already been
		 * received and acknowledged. */
		rc = SEDLIB_GetMessageStatus(TO_AppCustomData.Channel[index].MsgPortHandle, &readStatus, &writeStatus);
		if(SEDLIB_OK != rc)
		{
			/* We can't read the status.  Abort. */
			/* TODO */
			return;
		}

		if(SEDLIB_MSG_READ_PENDING_ACKNOWLEDGE != readStatus)
		{
			/* Yes.  Either there is nothing there to be received yet, or
			 * the receiver has already acknowledged it.  Go ahead and
			 * process some messages. */

			while(cont)
			{
				int32 status = OS_SUCCESS;

				if(FALSE == TO_AppCustomData.Channel[index].MsgProcessInProgress)
				{
					/* Read in a new message first. */
					TO_AppCustomData.Channel[index].InputCursor = 0;
					SLIP_Encoder_MsgStart(&TO_AppCustomData.Channel[index].Encoder);

					status = TO_OutputQueue_GetMsg(&TO_AppData.ChannelData[index], &TO_AppCustomData.Channel[index].InWorkMsg, OS_CHECK );

					if(CFE_SUCCESS == status)
					{
					    uint32 size = CFE_SB_GetTotalMsgLength(TO_AppCustomData.Channel[index].InWorkMsg);

						TO_AppCustomData.Channel[index].MsgProcessInProgress = TRUE;
						if(status == OS_QUEUE_EMPTY)
						{
							/* We have no more messages to queue.  Break out of the
							 * loop. */
							cont = FALSE;
							TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;
						}
						else if(status != OS_SUCCESS)
						{
							CFE_EVS_SendEvent(TO_OSQUEUE_GET_ERROR_EID, CFE_EVS_ERROR,
											"Failed to pop message from queue. (%i).", (int)status);

							/* Break out of the loop. */
							cont = FALSE;
							TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;
						}
					}
					else
					{
						/* Break out of the loop. */
						cont = FALSE;
						TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;
					}
				}

				if(TRUE == cont)
				{
					SLIP_ReturnCode_t slipStatus;
					uint32 size = CFE_SB_GetTotalMsgLength(TO_AppCustomData.Channel[index].InWorkMsg);
					uint8 *msg = (uint8*)TO_AppCustomData.Channel[index].InWorkMsg;

					TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;

					while(TO_AppCustomData.Channel[index].InputCursor < size)
					{
						slipStatus = SLIP_Encoder_QueueByte(
								&TO_AppCustomData.Channel[index].Encoder,
								msg[TO_AppCustomData.Channel[index].InputCursor]);

						if(slipStatus == SLIP_BUFFER_FULL_ERR)
						{
							/* The buffer is full but we were not able to fully
							 * process the current message. Some of it is still
							 * in the input buffer and will have to be processed
							 * in the next frame. */
							TO_AppCustomData.Channel[index].MsgProcessInProgress = TRUE;

							/* Break out of the loop. */
							cont = FALSE;
							break;
						}

						TO_AppCustomData.Channel[index].InputCursor++;
					};

					/* Did we process the entire message? */
					if(FALSE == TO_AppCustomData.Channel[index].MsgProcessInProgress)
					{
						/* Yes.  We processed the entire message. */
						slipStatus = SLIP_Encoder_MsgComplete(&TO_AppCustomData.Channel[index].Encoder);
						if(SLIP_OK == slipStatus)
						{
							/* We successfully queued that message up.  Take credit for it. */
							TO_AppData.ChannelData[index].SentMsgCount++;
							TO_AppCustomData.Channel[index].InputCursor = 0;
						}
						else
						{
							TO_AppCustomData.Channel[index].MsgProcessInProgress = TRUE;

							/* Break out of the loop. */
							cont = FALSE;
						}
					}
				}
			}

			/* Do we have anything in the buffer to send? */
			if(TO_AppCustomData.Channel[index].Encoder.BytesInBuffer > 0)
			{
				/* Yes we do. Send it. */
				CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&TO_AppCustomData.Channel[index].UartQueueDataCmd);
				TO_AppCustomData.Channel[index].UartQueueDataCmd.Hdr.BytesInBuffer = TO_AppCustomData.Channel[index].Encoder.BytesInBuffer;
				TO_AppCustomData.Channel[index].UartQueueDataCmd.Hdr.TxFrameID++;
				SEDLIB_SendMsg(
						TO_AppCustomData.Channel[index].MsgPortHandle,
						(CFE_SB_MsgPtr_t)&TO_AppCustomData.Channel[index].UartQueueDataCmd);

				/* Update metrics. */
				TO_AppData.ChannelData[index].OutputQueue.SentCount++;
				TO_AppData.ChannelData[index].OutputQueue.SentBytes += TO_AppCustomData.Channel[index].UartQueueDataCmd.Hdr.BytesInBuffer;

				/* Reset queue */
				TO_AppCustomData.Channel[index].UartQueueDataCmd.Hdr.BytesInBuffer = 0;
				SLIP_Encoder_Reset(&TO_AppCustomData.Channel[index].Encoder);
			}
		}
	}
}


