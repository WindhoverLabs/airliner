#ifndef SBN_UIO_MAILBOX_IF_H
#define SBN_UIO_MAILBOX_IF_H

#include <stdbool.h>
#include "mailbox_parser.h"

#define MAILBOX_SIZE              (0x10000)
#define MAILBOX_UIO_PATH          "/dev/uio0"
#define MAILBOX_HEADER_SIZE_BYTES (12)
#define MAILBOX_WORD_SIZE         (4)
#define MAILBOX_HEADER_SIZE_WORDS (MAILBOX_HEADER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_MAX_BUFFER_SIZE_BYTES   (1500)
#define MAILBOX_MAX_BUFFER_SIZE_WORDS   (MAILBOX_MAX_BUFFER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_SBN_HEADER_SIZE_WORDS   (2)
#define MAILBOX_SBN_TASK_DELAY_MSEC     (1)


typedef struct
{
    void *Instance;
    unsigned int OutputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int InputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int ParserBuffer[1500/sizeof(unsigned int)];
    unsigned int PackedBuffer[1500/sizeof(unsigned int)];
    Mailbox_Parser_Handle_t Parser;
} SBN_UIO_Mailbox_Data_t;


/* Write to shared memory. */
static void uio_write(void *base, unsigned int offset, unsigned int value);
/* Read from shared memory. */
static unsigned int uio_read(void *base, unsigned int offset);
/* Open and memory map uio. */
void *InitDevice(const char *path);
/* Check if mailbox is full. */
bool MailboxFull(void *instance);
/* Check if mailbox is empty. */
bool MailboxEmpty(void *instance);
/* Check for full error. */
bool MailboxFullError(void *instance);
/* Check for empty error. */
bool MailboxEmptyError(void *instance);
/* Write to mailbox. */
int MailboxWrite(void *instance, const unsigned int *buffer, unsigned int size);
/* Read from mailbox. */
int MailboxRead(void *instance, unsigned int *buffer, unsigned int size);



#endif
