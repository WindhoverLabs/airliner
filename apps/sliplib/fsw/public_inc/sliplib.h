#ifndef SLIPLIB_H
#define SLIPLIB_H

#include "cfe.h"

typedef enum
{
	SLIP_MESSAGE_FOUND_OK   =  2,
	SLIP_BUFFER_FULL_OK     =  1,
	SLIP_OK                 =  0,
	SLIP_INVALID_ARGS_ERR   = -1,
	SLIP_BUFFER_FULL_ERR    = -2,
	SLIP_PROTOCOL_VIOLATION = -3,
	SLIP_UNKNOWN_ERROR      = -4
} SLIP_ReturnCode_t;


typedef enum
{
	SLIP_ENCODER_UNKNOWN           = 0,
	SLIP_ENCODER_NOMINAL           = 1,
	SLIP_ENCODER_ENCODING_END      = 2,
	SLIP_ENCODER_ENCODING_ESC_ESC  = 3,
	SLIP_ENCODER_ENCODING_ESC_END  = 4
} SLIP_EncoderState_t;


typedef enum
{
	SLIP_DECODER_UNKNOWN           = 0,
	SLIP_DECODER_NOMINAL           = 1,
	SLIP_DECODER_PARSING_ESC       = 2
} SLIP_DecoderState_t;


typedef struct
{
	uint8               *Buffer;
	uint32               BufferSize;
	uint32               BytesInBuffer;
	SLIP_EncoderState_t  State;
} SLIP_EncoderHandle_t;


typedef struct
{
	uint8               *Buffer;
	uint32               BufferSize;
	uint32               BytesInBuffer;
	SLIP_DecoderState_t  State;
} SLIP_DecoderHandle_t;


int32 SLIP_LibInit(void);

/* Encoder */
SLIP_ReturnCode_t SLIP_Encoder_Init(SLIP_EncoderHandle_t *Encoder, uint8 *Buffer, uint32 Size);
SLIP_ReturnCode_t SLIP_Encoder_Reset(SLIP_EncoderHandle_t *Encoder);
SLIP_ReturnCode_t SLIP_Encoder_MsgStart(SLIP_EncoderHandle_t *Encoder);
SLIP_ReturnCode_t SLIP_Encoder_QueueByte(SLIP_EncoderHandle_t *Encoder, uint8 inByte);
SLIP_ReturnCode_t SLIP_Encoder_MsgComplete(SLIP_EncoderHandle_t *Encoder);

/* Decoder */
SLIP_ReturnCode_t SLIP_Decoder_Init(SLIP_DecoderHandle_t *Decoder, uint8 *Buffer, uint32 Size);
SLIP_ReturnCode_t SLIP_Decoder_Reset(SLIP_DecoderHandle_t *Decoder);
SLIP_ReturnCode_t SLIP_Decoder_ParseByte(SLIP_DecoderHandle_t *Decoder, uint8 inByte);


#endif /* SLIP_LIB_H */
