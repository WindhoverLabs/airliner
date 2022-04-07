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

end_of_function:

	return returnCode;
}



SLIP_ReturnCode_t SLIP_Decoder_ParseByte(SLIP_DecoderHandle_t *Decoder, uint8 Data)
{
	SLIP_ReturnCode_t returnCode = SLIP_OK;

	if(0 == Decoder)
	{
		returnCode = SLIP_INVALID_ARGS_ERR;

		goto end_of_function;
	}



end_of_function:

	return returnCode;
}
