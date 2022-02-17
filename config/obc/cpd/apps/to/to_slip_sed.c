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
	TO_QUEUE_MSG_OK                      = 0,
	TO_QUEUE_MSG_BUFFER_FULL_OK          = 1,
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
    SEDLIB_ReturnCode_t rc;
    uint32 i = 0;

    CFE_PSP_MemSet(&TO_AppCustomData, 0, sizeof(TO_AppCustomData));

    CFE_SB_InitMsg(&TO_AppCustomData.Channel[0].UartQueueDataCmd,
    		        UART_CMD_MSG_ID, sizeof(TO_AppCustomData.Channel[0].UartQueueDataCmd), TRUE);

    CFE_SB_SetCmdCode(&TO_AppCustomData.Channel[0].UartQueueDataCmd, UART_QUEUE_DATA_FOR_TX_CC);

    /*
     * Msg Port Interface
     */
    TO_AppCustomData.Channel[0].Mode = TO_CHANNEL_ENABLED;
    TO_AppCustomData.Channel[0].UartQueueDataCmd.Version = 1;

    status = SEDLIB_GetPipe(
    		"UART1_CMD",
			sizeof(UART_QueueDataCmd_t),
			&TO_AppCustomData.Channel[0].MsgPortHandle);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(TO_INIT_APP_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize UART MsgPort. (0x%08lX)",
                                 status);
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



TO_QueueMsgReturnCode_t TO_OutputChannel_QueueMsg(uint32 ChannelID, const char* Buffer, uint32 Size)
{
	osalbool cont = TRUE;
	TO_TlmChannel_t *channel = &TO_AppCustomData.Channel[ChannelID];
	TO_QueueMsgReturnCode_t returnCode = TO_QUEUE_MSG_OK;

	while(cont)
	{
	    /* Before we continue, check to see if we have room for more data. */
		if(channel->BytesQueued >= sizeof(channel->UartQueueDataCmd.Buffer))
		{
			/* No we don't. If we got to this point, we ran out of room before
			 * we got to the end of the message. Let the caller know there is
			 * still a message pending.
			 */
			returnCode = TO_QUEUE_MSG_BUFFER_FULL_MSG_DEFERRED;
			cont = FALSE;
		}
		else
		{
			/* Yes, we have more room. */
			switch(channel->EncoderState)
			{
				case TO_SLIP_NOMINAL:
				{
					/* Do we have more data to queue? */
					if(channel->InputCursor >= Size)
					{
						/* No we don't.  We're done but before we go, we need to add the
						 * SLIP_END symbol. Transition to ENCODING_END and let the
						 * machine handle it. */
						channel->EncoderState = TO_SLIP_ENCODING_END;
					}
					else
					{
						/* Yes we do. Process the next byte. */
						switch(Buffer[channel->InputCursor])
						{
							case SLIP_END:
							{
								/* This is a special byte that needs to be
								 * properly encoded.
								 */
								channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = SLIP_ESC;
								channel->BytesQueued++;
								channel->InputCursor++;
								channel->EncoderState = TO_SLIP_ENCODING_ESC_END;
								break;
							}

							case SLIP_ESC:
							{
								/* This is a special byte that needs to be
								 * properly encoded.
								 */
								channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = SLIP_ESC;
								channel->BytesQueued++;
								channel->InputCursor++;
								channel->EncoderState = TO_SLIP_ENCODING_ESC_ESC;
								break;
							}

							default:
							{
								/* There is nothing special about this byte.
								 * Just queue it.
								 */
								channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = Buffer[channel->InputCursor];
								channel->BytesQueued++;
								channel->InputCursor++;
							}
						}
					}

					break;
				}

				case TO_SLIP_ENCODING_END:
				{
					/* Queue the END byte and transition back to the NOMINAL
					 * state.
					 */
					channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = SLIP_END;
					channel->BytesQueued++;
					channel->EncoderState = TO_SLIP_NOMINAL;
					cont = FALSE;

					/* Is the output buffer full? */
					if(channel->BytesQueued >= sizeof(channel->UartQueueDataCmd.Buffer))
					{
						/* Yes it is full. Let the caller know we
						 * successfully queued this message but only just
						 * barely. We are literally at the end of the
						 * buffer. The message should go out, but there is
						 * no need to process a partial message on the next
						 * frame.
						 */
						returnCode = TO_QUEUE_MSG_BUFFER_FULL_OK;
					}
					else
					{
						/* No its not full. Let the caller know we queued
						 * the message, but the buffer is not full so we can
						 * still process more.
						 */
						returnCode = TO_QUEUE_MSG_OK;
					}

					break;
				}

				case TO_SLIP_ENCODING_ESC_ESC:
				{
					/* Queue the ESC_ESC byte and transition back to the NOMINAL
					 * state.
					 */
					channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = SLIP_ESC_ESC;
					channel->BytesQueued++;
					channel->EncoderState = TO_SLIP_NOMINAL;

					break;
				}

				case TO_SLIP_ENCODING_ESC_END:
				{
					/* Queue the ESC_END byte and transition back to the NOMINAL
					 * state.
					 */
					channel->UartQueueDataCmd.Buffer[channel->BytesQueued] = SLIP_ESC_END;
					channel->BytesQueued++;
					channel->EncoderState = TO_SLIP_NOMINAL;

					break;
				}
			}
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
			/* No.  Either there is nothing there to be received yet, or
			 * the receiver has already acknowledged it.  Go ahead and
			 * process some messages. */

			while(cont)
			{
				int32 status = OS_SUCCESS;

				if(FALSE == TO_AppCustomData.Channel[index].MsgProcessInProgress)
				{
					/* Read in a new message first. */
					TO_AppCustomData.Channel[index].InputCursor = 0;

					status = TO_OutputQueue_GetMsg(&TO_AppData.ChannelData[index], &TO_AppCustomData.Channel[index].InWorkMsg, OS_CHECK );
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

				if(TRUE == cont)
				{
					TO_QueueMsgReturnCode_t queueStatus;
					uint32 size = CFE_SB_GetTotalMsgLength(TO_AppCustomData.Channel[index].InWorkMsg);

					queueStatus = TO_OutputChannel_QueueMsg(index, (const char*)TO_AppCustomData.Channel[index].InWorkMsg, size);

					if(TO_QUEUE_MSG_BUFFER_FULL_MSG_DEFERRED == queueStatus)
					{
						/* The buffer is full but we were not able to fully
						 * process the current message. Some of it is still
						 * in the input buffer and will have to be processed
						 * in the next frame. */
						TO_AppCustomData.Channel[index].MsgProcessInProgress = TRUE;

						/* Break out of the loop. */
						cont = FALSE;
					}
					else if(TO_QUEUE_MSG_BUFFER_FULL_OK == queueStatus)
					{
						/* The buffer is full but the current message just barely
						 * fit.  We do need to go to the next frame but we don't
						 * need to process a partial message. */
						TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;

						/* Break out of the loop. */
						cont = FALSE;

						/* Take credit for the message send. */
						TO_AppData.ChannelData[index].SentMsgCount++;
					}
					else
					{
						/* The message was fully processed and there is still
						 * room left in the outgoing buffer.  Keep going.
						 */
						TO_AppCustomData.Channel[index].InputCursor = 0;
						TO_AppCustomData.Channel[index].MsgProcessInProgress = FALSE;

						/* Take credit for the message send. */
						TO_AppData.ChannelData[index].SentMsgCount++;
					}
				}
			}

			/* Do we have anything in the buffer to send? */
			if(TO_AppCustomData.Channel[0].BytesQueued > 0)
			{
				/* Yes we do. Send it. */
				CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&TO_AppCustomData.Channel[index].UartQueueDataCmd);
				TO_AppCustomData.Channel[0].UartQueueDataCmd.BytesInBuffer = TO_AppCustomData.Channel[index].BytesQueued;
				TO_AppCustomData.Channel[index].UartQueueDataCmd.TxFrameID++;
				SEDLIB_SendMsg(
						TO_AppCustomData.Channel[index].MsgPortHandle,
						(CFE_SB_MsgPtr_t)&TO_AppCustomData.Channel[index].UartQueueDataCmd);

				/* Update metrics. */
				TO_AppData.ChannelData[index].OutputQueue.SentCount++;
				TO_AppData.ChannelData[index].OutputQueue.SentBytes += TO_AppCustomData.Channel[index].BytesQueued;

				/* Reset queue */
				TO_AppCustomData.Channel[index].BytesQueued = 0;
			}
		}
	}
}


