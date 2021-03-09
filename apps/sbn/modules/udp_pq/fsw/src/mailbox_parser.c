#include "mailbox_parser.h"


unsigned int ParseMessage(Mailbox_Parser_Handle_t *Handle, unsigned int Input, unsigned int* Buffer, unsigned int *BufferSize)
{
	unsigned int sizeInWords = (*BufferSize+3)/4;

    switch(Handle->State)
	{
        /* Fallthru */
        case MPS_MESSAGE_COMPLETE:
	    case MPS_WAITING_FOR_CADU:
	    {
	    	if(Input == MAILBOX_MSG_CADU)
	    	{
                printf("got CADU\n");
		    	Handle->InputBufferCursor = 0;
		    	Handle->CurrentChecksum = 0;
	    		Handle->State = MPS_WAITING_FOR_SIZE;
	    	}
	    	else
	    	{
		    	Handle->State = MPS_WAITING_FOR_CADU;
	    	}
	    	break;
	    }

	    case MPS_WAITING_FOR_SIZE:
	    {
            printf("got size %u\n", Input);
	    	Handle->FullMessageSize = Input;
	    	Handle->State = MPS_PARSING_MESSAGE;
	    	break;
	    }

	    case MPS_PARSING_MESSAGE:
	    {
	    	if(Handle->InputBufferCursor < sizeInWords)
	    	{
	    		Buffer[Handle->InputBufferCursor] = Input;
	    	}
	    	Handle->CurrentChecksum += Input;
	    	Handle->InputBufferCursor++;
	    	if(Handle->InputBufferCursor >= Handle->FullMessageSize)
	    	{
		    	Handle->State = MPS_WAITING_FOR_CHECKSUM;
	    	}
	    	break;
	    }

	    case MPS_WAITING_FOR_CHECKSUM:
	    {
	    	if(Input != Handle->CurrentChecksum)
	    	{
                printf("checksum mismatch Input %x current %x\n", Input, Handle->CurrentChecksum);
	    		/* Checkum mismatch. */
		    	Handle->State = MPS_WAITING_FOR_CADU;
	    	}
	    	else
	    	{
	    		*BufferSize = Handle->InputBufferCursor*4;
		    	Handle->State = MPS_MESSAGE_COMPLETE;
	    	}
	    	break;
	    }
	}

    return Handle->State;
}
