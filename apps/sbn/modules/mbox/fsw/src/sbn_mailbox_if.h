#ifndef SBN_MAILBOX_IF_H
#define SBN_MAILBOX_IF_H

#include "mailbox_parser.h"
#include "xmbox.h"
#include "pq_includes.h"


#define MAILBOX_HEADER_SIZE_BYTES (12)
#define MAILBOX_WORD_SIZE         (4)
#define MAILBOX_HEADER_SIZE_WORDS (MAILBOX_HEADER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_MAX_BUFFER_SIZE_BYTES   (1500)
#define MAILBOX_MAX_BUFFER_SIZE_WORDS   (MAILBOX_MAX_BUFFER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_SBN_HEADER_SIZE_WORDS   (2)
#define MAILBOX_SBN_TASK_DELAY_MSEC     (1)

#define SBN_MAILBOX_BLOCKING_DELAY      (1)

/* PQ specific */
#define SBN_PQ_CHANNEL_NAME          ("MBOX")
#define SBN_PQ_CHANNEL_NUMBER        (0)
#define SBN_PQ_CONFIG_TABLENAME      ("CFG")
#define SBN_PQ_DUMP_TABLENAME        ("DMP")
#define SBN_PQ_CONFIG_TABLE_FILENAME ("/cf/apps/pq_cfg.tbl")
#define SBN_PQ_CF_THROTTLE_SEM_NAME  ("PQ_CF_CH0_SEM")
#define SBN_PQ_CF_SEM_INIT_VALUE     (1)

/* Send task specific */
#define SBN_PQ_SEND_TASK_NAME        ("PQ_OUTCH_0")
#define SBN_PQ_SEND_TASK_STACK_SIZE  (131072)
#define SBN_PQ_SEND_TASK_PRIORITY    (50)
#define SBN_PQ_SEND_TASK_FLAGS       (OS_ENABLE_CORE_0)
#define SBN_PQ_CHANNEL_GET_TIMEOUT   (500)


typedef struct
{
    boolean ConnectedFlag;
} SBN_Mailbox_Peer_t;


typedef struct
{
    XMbox Mbox;
    XMbox_Config *MboxConfigPtr;
    unsigned int OutputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int InputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
    unsigned int ParserBuffer[1500/sizeof(unsigned int)];
    unsigned int PackedBuffer[1500/sizeof(unsigned int)];
    Mailbox_Parser_Handle_t Parser;
    /* PQ specific. */
    PQ_ChannelData_t Channel;
    PQ_HkTlm_t HkTlm;
    /* Send task specific. */
    uint32 ChildTaskID;
    CFE_ES_ChildTaskMainFuncPtr_t SendTask;
    boolean TaskContinueFlag;
    /* TODO move to peer data. */
    boolean ConnectedFlag;
} SBN_Mailbox_Data_t;


int MailboxWrite(XMbox *instance, const unsigned int *buffer, unsigned int size);
int MailboxRead(XMbox *instance, unsigned int *buffer, unsigned int size);


#endif
