#ifndef MAILBOX_PARSER_H
#define MAILBOX_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAILBOX_MSG_CADU        (0x01020304)

typedef enum
{
	MPS_WAITING_FOR_CADU,
	MPS_WAITING_FOR_SIZE,
	MPS_PARSING_MESSAGE,
	MPS_WAITING_FOR_CHECKSUM,
	MPS_MESSAGE_COMPLETE,
	MPS_BUFFER_OVERFLOW
} Mailbox_Parser_State_t;

typedef struct
{
	Mailbox_Parser_State_t State;
	unsigned int           CurrentChecksum;
	unsigned int           InputBufferCursor;
	unsigned int           FullMessageSize;
} Mailbox_Parser_Handle_t;


unsigned int SED_ParseMessage(Mailbox_Parser_Handle_t *Handle, unsigned int Input, unsigned int* Buffer, unsigned int *BufferSize);

#ifdef __cplusplus
}
#endif

#endif
