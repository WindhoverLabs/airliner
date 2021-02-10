#include "mailbox_map.h"
#include "sbn_uio_mailbox_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "sbn_interfaces.h"
#include "sbn_platform_cfg.h"


SBN_UIO_Mailbox_Data_t SBN_UIO_Mailbox_Data;


static void uio_write(void *timer_base, unsigned int offset, unsigned int value)
{
    *((volatile unsigned *)(timer_base + offset)) = value;
}


static unsigned int uio_read(void *timer_base, unsigned int offset)
{
    return *((volatile unsigned *)(timer_base + offset));
}


void *InitDevice(const char *path)
{
    int fd            = 0;
    void *mailbox_ptr = 0;

    fd = open(path, O_RDWR);
    if (fd < 1)
    {
        /* TODO remove after debug. */
        printf("Invalid UIO device file:%s.\n", path);
        goto end_of_function;
    }

    mailbox_ptr = mmap(NULL, MAILBOX_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (mailbox_ptr == MAP_FAILED)
    {
        /* TODO remove after debug. */
        printf("Mmap call failure. errno %d\n", errno);
        goto end_of_function;
    }

    close(fd);
   
end_of_function:
    return mailbox_ptr;
}


bool MailboxFull(void *instance)
{
    unsigned int reg = 0;
    reg = uio_read(instance, MAILBOX_STATUS_REG);
    return reg & MAILBOX_STATUS_FIFO_FULL_BIT;
}


bool MailboxEmpty(void *instance)
{
    unsigned int reg = 0;
    reg = uio_read(instance, MAILBOX_STATUS_REG);
    return reg & MAILBOX_STATUS_FIFO_EMPTY_BIT;
}


bool MailboxFullError(void *instance)
{
    unsigned int reg = 0;
    reg = uio_read(instance, MAILBOX_ERROR_REG);
    return reg & MAILBOX_ERROR_FULL_BIT;
}


bool MailboxEmptyError(void *instance)
{
    unsigned int reg = 0;
    reg = uio_read(instance, MAILBOX_ERROR_REG);
    return reg & MAILBOX_ERROR_EMPTY_BIT;
}


int MailboxWrite(void *instance, const unsigned int *buffer, unsigned int size)
{
    int status     = 0;
    bool isFull    = false;
    unsigned int i = 0;
   
    isFull = MailboxFull(instance);
    if(isFull == true)
    {
        goto end_of_function;
    }

    for(i = 0; i < size; ++i)
    {
        uio_write(instance, MAILBOX_WRITE_REG, *buffer++);
        status = size;
        isFull = MailboxFull(instance);
        if(isFull == true)
        {
             goto end_of_function;
        }
    }

end_of_function:
    return status;
}


int MailboxRead(void *instance, unsigned int *buffer, unsigned int size)
{
    int status     = 0;
    bool isEmpty   = false;
    unsigned int i = 0;

    isEmpty = MailboxEmpty(instance);
    if(isEmpty == true)
    {
        goto end_of_function;
    }

    for(i = 0; i < size; ++i)
    {
        *buffer++ = uio_read(instance, MAILBOX_READ_REG);
        status = size;
        isEmpty = MailboxEmpty(instance);
        if(isEmpty == true)
        {
            goto end_of_function;
        }
    }

end_of_function:
    return status;
}


static int InitNet(SBN_NetInterface_t *Net)
{
    int Status = SBN_SUCCESS;

    memset(&SBN_UIO_Mailbox_Data, 0x0, sizeof(SBN_UIO_Mailbox_Data));

    SBN_UIO_Mailbox_Data.Instance = InitDevice(MAILBOX_UIO_PATH);
    if(SBN_UIO_Mailbox_Data.Instance == NULL)
    {
        /* TODO update to event. */
        OS_printf("SBN_UIO_Mailbox InitDevice Instance Null.\n");
        Status = SBN_ERROR;
    }

    return Status;
}


static int InitPeer(SBN_PeerInterface_t *Peer)
{

    return SBN_SUCCESS;
}


static int LoadNet(const char **Row, int FieldCnt, SBN_NetInterface_t *Net)
{
    return SBN_SUCCESS;
}


static int LoadPeer(const char **Row, int FieldCnt, SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
}


static int PollPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
}


static int SBN_MailboxSend(void *instance, unsigned int *buffer, unsigned int size)
{
    int Status               = SBN_SUCCESS;
    int SizeOutWords         = 0;
    unsigned int SizeInBytes = 0;
    unsigned int SizeInWords = 0;
    unsigned int Checksum    = 0;
    unsigned int i           = 0;

    SizeInBytes = size;
    /* Ensure word boundary */
    if(SizeInBytes % MAILBOX_WORD_SIZE)
    {
        SizeInBytes = (size + (MAILBOX_WORD_SIZE - (size % MAILBOX_WORD_SIZE)));
        SizeInWords = SizeInBytes / MAILBOX_WORD_SIZE;
    }

    if(SizeInWords + MAILBOX_HEADER_SIZE_WORDS > MAILBOX_MAX_BUFFER_SIZE_WORDS)
    {
        /* TODO update to event. */
        OS_printf("SBN_MailboxSend Send size too large %u bytes, %u words.\n", SizeInBytes, SizeInWords);
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* CADU */
    SBN_UIO_Mailbox_Data.OutputBuffer[0] = MAILBOX_MSG_CADU;
    /* Size */
    SBN_UIO_Mailbox_Data.OutputBuffer[1] = SizeInWords;
    /* Message */
    memcpy(&SBN_UIO_Mailbox_Data.OutputBuffer[2], buffer, size);
    /* Checksum Calculation */
    for(i = 0; i < SizeInWords - MAILBOX_HEADER_SIZE_WORDS; ++i)
    {
        Checksum += SBN_UIO_Mailbox_Data.OutputBuffer[i + 2];
    }

    /* Write Checksum */
    SBN_UIO_Mailbox_Data.OutputBuffer[SizeInWords - 1] = Checksum;

    /* Write to mailbox */
    SizeOutWords = MailboxWrite(SBN_UIO_Mailbox_Data.Instance, &SBN_UIO_Mailbox_Data.OutputBuffer[0], SizeInWords + MAILBOX_HEADER_SIZE_WORDS);
    if(SizeOutWords != SizeInWords + MAILBOX_HEADER_SIZE_WORDS)
    {
        /* TODO update to event. */
        OS_printf("SBN_MailboxSend incomplete write %u:%u.\n", SizeInWords, SizeOutWords);
        Status = SBN_ERROR;
    }

end_of_function:
    return Status;
}


static int Send(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType,
                SBN_MsgSz_t MsgSz, void *Payload)
{
    unsigned int TotalSize = MsgSz + SBN_PACKED_HDR_SZ;
    int Status = SBN_SUCCESS;

    if(TotalSize > MAILBOX_MAX_BUFFER_SIZE_BYTES)
    {
        /* TODO update to event. */
        OS_printf("SBN_MailboxSend Send size too large %u bytes.\n", TotalSize);
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* Add SBN header. */
    SBN_PackMsg(&SBN_UIO_Mailbox_Data.PackedBuffer[0], MsgSz, MsgType, CFE_PSP_GetProcessorId(), Payload);

    /* Send via Mailbox. */
    Status = SBN_MailboxSend(SBN_UIO_Mailbox_Data.Instance, &SBN_UIO_Mailbox_Data.PackedBuffer[0], TotalSize);

end_of_function:
    return Status;
}


static int Recv(SBN_NetInterface_t *Net, SBN_MsgType_t *MsgTypePtr,
                SBN_MsgSz_t *MsgSzPtr, SBN_CpuID_t *CpuIDPtr, void *Payload)
{
    int SizeRead            = 0;
    unsigned int i          = 0;
    int ReturnValue         = SBN_IF_EMPTY;

    SizeRead = MailboxRead(SBN_UIO_Mailbox_Data.Instance, 
                           &SBN_UIO_Mailbox_Data.InputBuffer[0], 
                           MAILBOX_MAX_BUFFER_SIZE_WORDS);
    if(SizeRead > 0)
    {
        for(i = 0; i < SizeRead; ++i)
        {
            unsigned int Size = MAILBOX_MAX_BUFFER_SIZE_BYTES;
            unsigned int Status = ParseMessage(&SBN_UIO_Mailbox_Data.Parser,
                                               SBN_UIO_Mailbox_Data.InputBuffer[i],
                                               &SBN_UIO_Mailbox_Data.ParserBuffer[0],
                                               &Size);
            if(Status == MPS_MESSAGE_COMPLETE)
            {
                if (SBN_UnpackMsg(&SBN_UIO_Mailbox_Data.ParserBuffer[0], MsgSzPtr, MsgTypePtr, CpuIDPtr, Payload) == false)
                {
                    OS_printf("Unpack failed.\n");
                    ReturnValue = SBN_ERROR;
                    goto end_of_function;
                }
                ReturnValue = SBN_SUCCESS;
            }
        }
    }

#ifdef SBN_RECV_TASK
    (void) OS_TaskDelay(MAILBOX_SBN_TASK_DELAY_MSEC);
#endif

end_of_function:
    return ReturnValue;
}


static int ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet)
{
    return SBN_NOT_IMPLEMENTED;
}


static int ResetPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_NOT_IMPLEMENTED;
}


static int UnloadNet(SBN_NetInterface_t *Net)
{
    return SBN_NOT_IMPLEMENTED;
}


static int UnloadPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_NOT_IMPLEMENTED;
}


SBN_IfOps_t SBN_UIO_Mbox_Ops = 
{
    LoadNet, 
    LoadPeer, 
    InitNet, 
    InitPeer,
    PollPeer, 
    Send, 
    NULL, 
    Recv,
    ReportModuleStatus,
    ResetPeer, 
    UnloadNet, 
    UnloadPeer
};


