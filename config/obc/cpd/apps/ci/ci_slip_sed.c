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
#include "ci_events.h"
#include <strings.h>
#include "sedlib.h"
#include "sliplib.h"

#define CI_CUSTOM_RETURN_CODE_NULL_POINTER      (-1)


#define UART_BUFFER_SIZE        (1500)

typedef enum
{
	CI_BUFFER_EMPTY               = 0,
	CI_BUFFER_PARSE_INPROGRESS    = 1,
	CI_BUFFER_PARSE_COMPLETE      = 2
} CI_IncomingBufferState_t;

typedef enum
{
	CI_SLIP_PARSER_COMPLETE            = 1,
	CI_SLIP_PARSER_MESSAGE_FOUND       = 2,
	CI_SLIP_PARSER_PROTOCOL_VIOLATION  = 3,
	CI_SLIP_PARSER_BUFFER_OVERFLOW     = 4
} CI_SlipParserReturnCode_t;

/* UART In message */
#pragma pack(push, 1)
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32          Version;
	uint16          TransmitInterrupts;		/**< Number of transmit interrupts */
	uint16          ReceiveInterrupts;		/**< Number of receive interrupts */
	uint16          StatusInterrupts;		/**< Number of status interrupts */
	uint16          ModemInterrupts;		/**< Number of modem interrupts */
	uint16          CharactersTransmitted; 	/**< Number of characters transmitted */
	uint16          CharactersReceived;		/**< Number of characters received */
	uint16          ReceiveOverrunErrors;	/**< Number of receive overruns */
	uint16          ReceiveParityErrors;	/**< Number of receive parity errors */
	uint16          ReceiveFramingErrors;	/**< Number of receive framing errors */
	uint16          ReceiveBreakDetected;	/**< Number of receive breaks */
	uint32          LastCmdCode;
	uint32          LastCmdResponse;
	uint32          RxFrameID;
	uint32          BytesInBuffer;
    uint8           RxBuffer[UART_BUFFER_SIZE];
} UART_StatusTlm_t;
#pragma pack(pop)



typedef struct
{
    uint32                   MsgPortHandle;
    UART_StatusTlm_t         UartStatusTlm;
    uint8                    SlipBuffer[CI_MAX_CMD_INGEST];
    uint32                   SlipInCursor;
    uint32                   SlipOutCursor;
    CI_IncomingBufferState_t BufferState;
    SLIP_DecoderHandle_t     Decoder;
} CI_AppCustomData_t;

CI_AppCustomData_t CI_AppCustomData = {0};


CI_SlipParserReturnCode_t CI_ProcessMessage(uint8* inBuffer, uint32 inSize, uint8* inOutBuffer, uint32* inOutSize);


osalbool CI_AddCustomEventFilters(uint32 *count)
{	
    return TRUE;
}


int32 CI_InitCustom(void)
{
    int32 Status = CFE_SUCCESS;
    SLIP_ReturnCode_t slipStatus;

    slipStatus = SLIP_Decoder_Init(&CI_AppCustomData.Decoder, CI_AppCustomData.SlipBuffer, sizeof(CI_AppCustomData.SlipBuffer));
    if(slipStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize SLIP decoder. %i",
								 slipStatus);
    }

    Status = SEDLIB_GetPipe(
    		"UART1_STATUS",
			sizeof(UART_StatusTlm_t),
			&CI_AppCustomData.MsgPortHandle);
    if(Status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize UART MsgPort. (0x%08lX)",
                                 Status);
    }
    else
    {
        CFE_EVS_SendEvent(CI_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "CI command input initialized.");
    }

end_of_function:
    return Status;

}


void CI_ReadMessage(uint8* buffer, uint32* size)
{
	CI_SlipParserReturnCode_t slipReturnCode;
	osalbool parseBuffer = FALSE;

    switch(CI_AppCustomData.BufferState)
    {
        /* fallthru */
        case CI_BUFFER_EMPTY:
        case CI_BUFFER_PARSE_COMPLETE:
        {
        	/* We either haven't read the buffer in yet, or we already parsed
        	 * the entire buffer and are on the next frame.  Either way, we need
        	 * to read in a new message into the buffer. Reset the SlipInCursor,
             * set the new state to INPROGRESS, and try to read the next
             * message.
        	 */
	    	SEDLIB_ReturnCode_t rc;

        	CI_AppCustomData.SlipInCursor  = 0;

            rc = SEDLIB_ReadMsg(CI_AppCustomData.MsgPortHandle, (CFE_SB_MsgPtr_t)&CI_AppCustomData.UartStatusTlm);
            if(SEDLIB_MSG_FRESH_OK == rc)
            {
            	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_INPROGRESS;
            	parseBuffer = TRUE;
            }
            else if(SEDLIB_MSG_STALE_OK == rc)
            {
            	/* There was no message waiting for us. */
            	parseBuffer = FALSE;
            	*size = 0;
            }
            else
            {
            	/* An error occurred. We can't read from the incoming port. */
            	parseBuffer = FALSE;
            	*size = 0;
            }

        	break;
        }

        case CI_BUFFER_PARSE_INPROGRESS:
        {
        	parseBuffer = TRUE;
        	break;
        }


        default:
        {
        	parseBuffer = FALSE;
        	*size = 0;
        	break;
        }
    }

    if(parseBuffer)
    {
		slipReturnCode = CI_ProcessMessage(
				                   CI_AppCustomData.UartStatusTlm.RxBuffer,
				                   CI_AppCustomData.UartStatusTlm.BytesInBuffer,
								   CI_AppCustomData.SlipBuffer,
				                   size);
		switch(slipReturnCode)
		{
		    case CI_SLIP_PARSER_COMPLETE:
		    {
		    	/* We've parsed all the incoming bytes but didn't find
		    	 * any new messages. */
	        	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_COMPLETE;
		    	*size = 0;

		    	break;
		    }

		    case CI_SLIP_PARSER_MESSAGE_FOUND:
		    {
		    	/* We've parsed some of the bytes and found a message.
		    	 * The message contents are in the SlipBuffer and the number of
		    	 * bytes in the message are in the size argument. Copy the
		    	 * SlipBuffer contents to the outgoing buffer argument. Remain
		    	 * in the PARSE_INPROGRESS state so we can see if there are more
		    	 * messages in the buffer.
		    	 */
		    	int32 rc;

		    	rc = CFE_PSP_MemCpy(buffer, CI_AppCustomData.SlipBuffer, *size);
		    	if(rc != CFE_PSP_SUCCESS)
		    	{
		    		/* An error occurred trying to copy the contents. Reject
		    		 * the rest of the buffer. */
		        	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_COMPLETE;
		    		*size = 0;
		    	}

		    	break;
		    }

		    case CI_SLIP_PARSER_PROTOCOL_VIOLATION:
		    {
		    	/* We parsed some of the bytes in the message but found
		    	 * an illegal sequence of bytes. Reject the rest of the
		    	 * buffer.
		    	 */
	        	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_COMPLETE;
		    	*size = 0;
		    	break;
		    }

		    case CI_SLIP_PARSER_BUFFER_OVERFLOW:
		    {
		    	/* We've parsed some of the bytes but ran into the end
		    	 * of the  output buffer before we found a message. This
		    	 * might be a fat command bigger than we can actually
		    	 * receive. Reject the buffer.
		    	 */
	        	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_COMPLETE;
		    	*size = 0;
		    	break;
		    }

		    default:
		    {
		    	/* This is not possible, but return zero anyway. */
	        	CI_AppCustomData.BufferState = CI_BUFFER_PARSE_COMPLETE;
		    	*size = 0;
		    	break;
		    }
		}
    }
}


void CI_CleanupCustom(void)
{
}



CI_SlipParserReturnCode_t CI_ProcessMessage(uint8* inBuffer, uint32 inSize, uint8* inOutBuffer, uint32* inOutSize)
{
	osalbool cont = TRUE;
    int received = 0;
    CI_SlipParserReturnCode_t returnCode;

	/* Sit in a loop reading bytes until we put together
	 * a whole packet or run out of bytes to parse.
	 * Make sure not to copy them into the packet if we
	 * run out of room.
	  */
	while(cont)
	{
		/* Break out of the loop if we've parsed the entire input buffer. */
		if(CI_AppCustomData.SlipInCursor >= inSize)
		{
			returnCode = CI_SLIP_PARSER_COMPLETE;
			cont = FALSE;
		}

		/* Break out of the loop if we're about to overrun the output buffer. */
		if(CI_AppCustomData.SlipOutCursor >= *inOutSize)
		{
			returnCode = CI_SLIP_PARSER_BUFFER_OVERFLOW;
			cont = FALSE;
		}

		if(cont)
		{
//			switch(CI_AppCustomData.ParserState)
//			{
//				case CI_SLIP_STATE_MESSAGE_FOUND:
//				{
//					/* Start processing the next message. */
//					CI_AppCustomData.SlipOutCursor = 0;
//					CI_AppCustomData.ParserState   = CI_SLIP_STATE_PARSING_MESSAGE;
//
//					break;
//				}
//
//				case CI_SLIP_STATE_PARSING_MESSAGE:
//				{
//					switch(inBuffer[CI_AppCustomData.SlipInCursor])
//					{
//						case SLIP_END:
//						{
//							/* We found the end of a message. Skip over this
//							 * byte and break out of the loop. */
//							*inOutSize = CI_AppCustomData.SlipOutCursor;
//							CI_AppCustomData.SlipInCursor++;
//							returnCode = CI_SLIP_PARSER_MESSAGE_FOUND;
//							CI_AppCustomData.ParserState = CI_SLIP_STATE_MESSAGE_FOUND;
//							cont = FALSE;
//
//							break;
//						}
//
//						case SLIP_ESC:
//						{
//							/* We found the beginning of an escape sequence.
//							 * Transition to the PARSED_ESC state so we can defer
//							 * our next action to see what the next character
//							 * is.
//							 */
//							CI_AppCustomData.SlipInCursor++;
//							CI_AppCustomData.ParserState = CI_SLIP_STATE_PARSED_ESC;
//
//							break;
//						}
//
//						default:
//						{
//							/* Nothing special. Just another character to copy. */
//							inOutBuffer[CI_AppCustomData.SlipOutCursor] =
//									inBuffer[CI_AppCustomData.SlipInCursor];
//							CI_AppCustomData.SlipOutCursor++;
//							CI_AppCustomData.SlipInCursor++;
//						}
//					}
//
//					break;
//				}
//
//				case CI_SLIP_STATE_PARSED_ESC:
//				{
//					switch(inBuffer[CI_AppCustomData.SlipInCursor])
//					{
//						case SLIP_ESC_END:
//						{
//							/* This is just an END symbol in the payload.
//							 * Copy an END symbol to the output buffer. */
//							inOutBuffer[CI_AppCustomData.SlipOutCursor] =
//									SLIP_END;
//							CI_AppCustomData.SlipOutCursor++;
//							CI_AppCustomData.SlipInCursor++;
//							CI_AppCustomData.ParserState = CI_SLIP_STATE_PARSING_MESSAGE;
//
//							break;
//						}
//
//						case SLIP_ESC_ESC:
//						{
//							/* This is just an ESC symbol in the payload.
//							 * Copy an ESC symbol to the output buffer. */
//							inOutBuffer[CI_AppCustomData.SlipOutCursor] =
//									SLIP_ESC;
//							CI_AppCustomData.SlipOutCursor++;
//							CI_AppCustomData.SlipInCursor++;
//							CI_AppCustomData.ParserState = CI_SLIP_STATE_PARSING_MESSAGE;
//
//							break;
//						}
//
//						default:
//						{
//							/* This is an illegal character. Return this back to the
//							 * caller so it can maybe raise an event or something.
//							 * Reset the machine.
//							 */
//							returnCode = CI_SLIP_PARSER_PROTOCOL_VIOLATION;
//							CI_AppCustomData.SlipOutCursor = 0;
//							CI_AppCustomData.ParserState = CI_SLIP_STATE_PARSING_MESSAGE;
//							cont = FALSE;
//
//							break;
//						}
//					}
//
//					break;
//				}
//
//				default:
//				{
//					/* How did we get here?? */
//					returnCode = CI_SLIP_PARSER_PROTOCOL_VIOLATION;
//					CI_AppCustomData.SlipOutCursor = 0;
//					CI_AppCustomData.ParserState = CI_SLIP_STATE_PARSING_MESSAGE;
//				}
//			}
		}
	}

	return returnCode;
}




