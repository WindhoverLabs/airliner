#include "slip.h"



int32 SLIP_LibInit(void)
{
	return CFE_SUCCESS;
}


SLIP_ReturnCode_t SLIP_Encoder_Init(SLIP_EncoderHandle_t *Encoder, uint8 *Buffer, uint32 Size)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Encoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	if(0 == Buffer)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	if(0 == Size)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	CFE_PSP_MemSet(Encoder, 0, sizeof(Encoder));

	Encoder->Buffer = Buffer;
	Encoder->BufferSize = Size;

end_of_function:

	return returnCode;
}


SLIP_ReturnCode_t SLIP_Encoder_QueueByte(SLIP_EncoderHandle_t *Encoder, uint8 Data)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Encoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	/* Before we continue, check to see if we have room for more data. */
	if(Encoder->BytesInBuffer >= Encoder->BufferSize)
	{
		/* No we don't. If we got to this point, we ran out of room before
		 * we got to the end of the message. Let the caller know we did not
		 * queue the data.
		 */
        returnCode = SLIP_BUFFER_FULL_ERR;
    }
    else
    {
        /* Yes, we have more room. */
        switch(Encoder->State)
		{
			case SLIP_NOMINAL:
			{
				/* Process the byte. */
				switch(Data)
				{
					case SLIP_END:
					{
						/* This is a special byte that needs to be
						 * properly encoded.
						 */
						Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_ESC;
						Encoder->BytesInBuffer++;
						Encoder->State = SLIP_ENCODING_ESC_END;

						/* Enqueueing this byte requires two steps, so call
						 * this function again.
						 */
						returnCode = SLIP_Encoder_QueueByte(Encoder, Data);
						break;
					}

					case SLIP_ESC:
					{
						/* This is a special byte that needs to be
						 * properly encoded.
						 */
						Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_ESC;
						Encoder->BytesInBuffer++;
						Encoder->State = SLIP_ENCODING_ESC_ESC;

						/* Enqueueing this byte requires two steps, so call
						 * this function again.
						 */
						returnCode = SLIP_Encoder_QueueByte(Encoder, Data);
						break;
					}

					default:
					{
						/* There is nothing special about this byte.
						 * Just queue it.
						 */
						Encoder->Buffer[Encoder->BytesInBuffer] = Data;
						Encoder->BytesInBuffer++;
					}
				}

				break;
			}

			case SLIP_ENCODING_END:
			{
				/* Queue the END byte and transition back to the NOMINAL
				 * state.
				 */
				Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_END;
				Encoder->BytesInBuffer++;
				Encoder->State = SLIP_NOMINAL;

				/* This does not require 2 steps to encode but if we got here
				 * its because we did not have room in the previous frame to
				 * queue it.  Since we also still need to enqueue the byte
				 * passed by the caller, we need to call the function again.
				 */
				returnCode = SLIP_Encoder_QueueByte(Encoder, Data);
				break;
			}

			case SLIP_ENCODING_ESC_ESC:
			{
				/* Queue the ESC_ESC byte and transition back to the NOMINAL
				 * state.
				 */
				Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_ESC_ESC;
				Encoder->BytesInBuffer++;
				Encoder->State = SLIP_NOMINAL;

				break;
			}

			case SLIP_ENCODING_ESC_END:
			{
				/* Queue the ESC_END byte and transition back to the NOMINAL
				 * state.
				 */
				Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_ESC_END;
				Encoder->BytesInBuffer++;
				Encoder->State = SLIP_NOMINAL;

				break;
			}
		}

    	/* Check to see if we filled up the outgoing buffer. */
        if(SLIP_OK == returnCode)
        {
			if(Encoder->BytesInBuffer >= Encoder->BufferSize)
			{
				/* Yes we did. If we got to this point, we queued the data, but
				 * we now have no room to queue any more data.
				 */
				returnCode = SLIP_BUFFER_FULL_OK;
			}
        }
	}

end_of_function:

	return returnCode;
}



SLIP_ReturnCode_t SLIP_Encoder_Reset(SLIP_EncoderHandle_t *Encoder)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Encoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	Encoder->BytesInBuffer = 0;

end_of_function:

	return returnCode;
}



SLIP_ReturnCode_t SLIP_Encoder_MsgStart(SLIP_EncoderHandle_t *Encoder)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Encoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	Encoder->State = SLIP_NOMINAL;

end_of_function:

	return returnCode;
}



SLIP_ReturnCode_t SLIP_Encoder_MsgComplete(SLIP_EncoderHandle_t *Encoder)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Encoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	/* First, check to see if we have room for more data. */
	if(Encoder->BytesInBuffer >= Encoder->BufferSize)
	{
		/* No we don't. If we got to this point, we ran out of room before
		 * we got to the end of the message. Transition to the
		 * SLIP_ENCODING_END state so we can complete this action in the next
		 * frame, and let the caller know we did not
		 * queue the data.
		 */

		Encoder->State = SLIP_ENCODING_END;
        returnCode = SLIP_BUFFER_FULL_ERR;
    }
    else
    {
        /* Yes, we have more room. Queue the END byte and transition back to
         * the NOMINAL state.
		 */
		Encoder->Buffer[Encoder->BytesInBuffer] = SLIP_END;
		Encoder->BytesInBuffer++;
		Encoder->State = SLIP_NOMINAL;
	}

end_of_function:

	return returnCode;
}


