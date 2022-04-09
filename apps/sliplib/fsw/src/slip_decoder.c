#include "slip_decoder.h"



SLIP_ReturnCode_t SLIP_Decoder_Init(SLIP_DecoderHandle_t *Decoder, uint8 *Buffer, uint32 Size)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Decoder)
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

	CFE_PSP_MemSet(Decoder, 0, sizeof(Decoder));

	Decoder->Buffer = Buffer;
	Decoder->BufferSize = Size;
	Decoder->State = SLIP_DECODER_NOMINAL;
	Decoder->BytesInBuffer = 0;

end_of_function:

	return returnCode;
}



SLIP_ReturnCode_t SLIP_Decoder_ParseByte(SLIP_DecoderHandle_t *Decoder, uint8 inByte)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Decoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	/* Before we continue, check to see if we have room for more data. */
	if(Decoder->BytesInBuffer >= Decoder->BufferSize)
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
        switch(Decoder->State)
		{
			case SLIP_DECODER_NOMINAL:
			{
				switch(inByte)
				{
					case SLIP_END:
					{
						/* We found the end of a message. */
						returnCode = SLIP_MESSAGE_FOUND_OK;

						break;
					}

					case SLIP_ESC:
					{
						/* This is a special byte that needs to be
						 * properly decoded.  Dequeueing this byte requires we
						 * read the next byte.  So just return "OK" and wait for
						 * the caller to give us the next byte.
						 */
						Decoder->State = SLIP_DECODER_PARSING_ESC;
						returnCode = SLIP_OK;

						break;
					}

					default:
					{
						/* Nothing special. Just another character to copy. */
						Decoder->Buffer[Decoder->BytesInBuffer] = inByte;
						Decoder->BytesInBuffer++;
					}
				}

				break;
			}

			case SLIP_DECODER_PARSING_ESC:
			{
				switch(inByte)
				{
					case SLIP_ESC_END:
					{
						/* This is just an END symbol in the payload.
						 * Copy an END symbol to the output buffer. */
						Decoder->Buffer[Decoder->BytesInBuffer] = SLIP_END;
						Decoder->BytesInBuffer++;
						Decoder->State = SLIP_DECODER_NOMINAL;

						break;
					}

					case SLIP_ESC_ESC:
					{
						/* This is just an ESC symbol in the payload.
						 * Copy an ESC symbol to the output buffer. */
						Decoder->Buffer[Decoder->BytesInBuffer] = SLIP_ESC;
						Decoder->BytesInBuffer++;
						Decoder->State = SLIP_DECODER_NOMINAL;

						break;
					}

					default:
					{
						/* This is an illegal character. Return this back to the
						 * caller so it can maybe raise an event or something.
						 * Reset the machine.
						 */
						returnCode = SLIP_PROTOCOL_VIOLATION;

						break;
					}
				}

				break;
			}

			default:
			{
				/* How did we get here?? */
				returnCode = SLIP_UNKNOWN_ERROR;
			}
		}
    }

end_of_function:

	return returnCode;

}



SLIP_ReturnCode_t SLIP_Decoder_Reset(SLIP_DecoderHandle_t *Decoder)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Decoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}

	Decoder->BytesInBuffer = 0;
	Decoder->State = SLIP_DECODER_NOMINAL;

end_of_function:

	return returnCode;
}
