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
	UART_StatusHdr_t  Hdr;
    volatile uint8    RxBuffer[UART_BUFFER_SIZE];
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
        printf("size for CI_ProcessMessage:%d", size);
		slipReturnCode = CI_ProcessMessage(
				                   CI_AppCustomData.UartStatusTlm.RxBuffer,
				                   CI_AppCustomData.UartStatusTlm.Hdr.BytesInBuffer,
								   CI_AppCustomData.SlipBuffer,
				                   size);
        printf("CI_ReadMessage#1:%d\n", slipReturnCode);
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

    printf("CI_ProcessMessage#1\n");
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
         printf("CI_ProcessMessage#2\n");
		/* Break out of the loop if we've parsed the entire input buffer. */
        if(CI_AppCustomData.SlipInCursor >= inSize)
        {
            returnCode = CI_SLIP_PARSER_COMPLETE;
            cont = FALSE;
             printf("CI_ProcessMessage#3-->:%d\n", inSize);
             printf("CI_ProcessMessage#3-->inOutSize:%d\n", *inOutSize);
             printf("Last byte:%d\n", inBuffer[inSize-1]);
//             printf("");
        }

		/* Break out of the loop if we're about to overrun the output buffer. */
		if(CI_AppCustomData.SlipOutCursor >= *inOutSize)
		{
			returnCode = CI_SLIP_PARSER_BUFFER_OVERFLOW;
            printf("CI_ProcessMessage#5\n");
			cont = FALSE;
		}

		if(cont)
		{
			SLIP_ReturnCode_t status = SLIP_Decoder_ParseByte(&CI_AppCustomData.Decoder, inBuffer[CI_AppCustomData.SlipInCursor]);
			switch(status)
			{
				case SLIP_MESSAGE_FOUND_OK:
				{
                    printf("SLIP_MESSAGE_FOUND_OK\n");
					/* Start processing the next message. */
					CI_AppCustomData.SlipInCursor++;
					cont = FALSE;
					returnCode = CI_SLIP_PARSER_MESSAGE_FOUND;
					*inOutSize = CI_AppCustomData.Decoder.BytesInBuffer;
					SLIP_Decoder_Reset(&CI_AppCustomData.Decoder);

					break;
				}

				case SLIP_OK:
				{
                    printf("SLIP_OK\n");
					CI_AppCustomData.SlipInCursor++;
					break;
				}

				default:
				{
					/* Something went wrong. */
					/* TODO */
                    printf("CI_ProcessMessage#4\n");
					CI_AppCustomData.SlipInCursor++;
				}
			}
		}
	}
	return returnCode;
}




