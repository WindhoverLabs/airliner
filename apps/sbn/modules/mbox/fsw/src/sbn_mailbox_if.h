#ifndef SBN_MAILBOX_IF_H
#define SBN_MAILBOX_IF_H

#include "mailbox_parser.h"
#include "xmbox.h"

#define MAILBOX_HEADER_SIZE_BYTES (12)
#define MAILBOX_WORD_SIZE         (4)
#define MAILBOX_HEADER_SIZE_WORDS (MAILBOX_HEADER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_MAX_BUFFER_SIZE_BYTES   (1500)
#define MAILBOX_MAX_BUFFER_SIZE_WORDS   (MAILBOX_MAX_BUFFER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_SBN_HEADER_SIZE_WORDS   (2)
#define MAILBOX_SBN_TASK_DELAY_MSEC     (1)


typedef struct
{
    XMbox Mbox;
    XMbox_Config *MboxConfigPtr;
    unsigned int OutputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int InputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int ParserBuffer[1500/sizeof(unsigned int)];
    unsigned int PackedBuffer[1500/sizeof(unsigned int)];
    Mailbox_Parser_Handle_t Parser;
} SBN_Mailbox_Data_t;


int MailboxWrite(XMbox *instance, const unsigned int *buffer, unsigned int size);
int MailboxRead(XMbox *instance, unsigned int *buffer, unsigned int size);


#endif
