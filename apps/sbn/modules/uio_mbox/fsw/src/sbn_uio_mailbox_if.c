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
#include "msg_ids.h"
#include "pq_events.h"

extern PQ_ChannelTbl_t PQ_BackupConfigTbl;

SBN_UIO_Mailbox_Data_t SBN_UIO_Mailbox_Data;

void SBN_PQ_Output_Task(void);
void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel);

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
        OS_printf("Invalid UIO device file:%s.\n", path);
        goto end_of_function;
    }

    mailbox_ptr = mmap(NULL, MAILBOX_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (mailbox_ptr == MAP_FAILED)
    {
        /* TODO remove after debug. */
        OS_printf("Mmap call failure. errno %d\n", errno);
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


/* Blocking write */
int MailboxWrite(void *instance, const unsigned int *buffer, unsigned int size)
{
    int status     = size;
    bool isFull    = false;
    unsigned int i = 0;

    for(i = 0; i < size; ++i)
    {
        while(MailboxFull(instance))
        {
            OS_TaskDelay(SBN_MAILBOX_BLOCKING_DELAY);
        }
        uio_write(instance, MAILBOX_WRITE_REG, *buffer++);
    }

end_of_function:
    return status;
}

/* Non-blocking read */
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


void SBN_PQ_Output_Task(void)
{
    CFE_ES_RegisterChildTask();

    SBN_PQ_ChannelHandler(&SBN_UIO_Mailbox_Data.Channel);

    CFE_ES_ExitChildTask();
}


void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel)
{
    int32 iStatus = CFE_SUCCESS;
    uint32 msgSize = 0;
    char *buffer;
    
    SBN_UIO_Mailbox_Data.TaskContinueFlag = TRUE;

    while(SBN_UIO_Mailbox_Data.TaskContinueFlag)
    {
        if(PQ_Channel_State(Channel) == PQ_CHANNEL_OPENED)
        {
            PQ_OutputQueue_t *chQueue = &Channel->OutputQueue;

            iStatus =  OS_QueueGet(
                    chQueue->OSALQueueID,
                    &buffer, sizeof(buffer), &msgSize, SBN_PQ_CHANNEL_GET_TIMEOUT);

            if(iStatus == OS_SUCCESS)
            {
                uint16 actualMessageSize = CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)buffer);
                CFE_SB_MsgId_t MsgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)buffer);
                SBN_MsgType_t MsgType;
                size_t BufSz = actualMessageSize + SBN_PACKED_HDR_SZ;
                /* TODO fix this. */
                uint8 Buf[BufSz];
                unsigned int SizeInBytes = 0;
                unsigned int SizeInWords = 0;
                unsigned int Checksum    = 0;
                unsigned int i           = 0;

                if(MsgID == SBN_SUB_MID || MsgID == SBN_ALLSUB_MID)
                {
                    MsgType = SBN_SUBSCRIBE_MSG;
                }
                else if (MsgID == SBN_UNSUB_MID)
                {
                    MsgType = SBN_UN_SUBSCRIBE_MSG;
                }
                else
                {
                    MsgType = SBN_APP_MSG;
                }

                SBN_PackMsg(&Buf, actualMessageSize, MsgType, CFE_PSP_GetProcessorId(), buffer);

                SizeInBytes = BufSz;
                /* Ensure word boundary */
                SizeInBytes = (BufSz + (MAILBOX_WORD_SIZE - (BufSz % MAILBOX_WORD_SIZE)));
                SizeInWords = SizeInBytes / MAILBOX_WORD_SIZE;

                //printf("BufSz %u\n", BufSz);
                //printf("SizeInBytes %u\n", SizeInBytes);
                //printf("SizeInWords %u\n", SizeInWords);

                if(SizeInWords + MAILBOX_HEADER_SIZE_WORDS > MAILBOX_MAX_BUFFER_SIZE_WORDS)
                {
                    /* TODO update to event. */
                    OS_printf("SBN_MailboxSend Send size too large %u bytes, %u words.\n", SizeInBytes, SizeInWords);
                    continue;
                }

                /* Set CADU. */
                SBN_UIO_Mailbox_Data.OutputBuffer[0] = MAILBOX_MSG_CADU;
                /* Set size of the payload in words. */
                SBN_UIO_Mailbox_Data.OutputBuffer[1] = SizeInWords;
                /* Copy the payload. */
                memcpy(&SBN_UIO_Mailbox_Data.OutputBuffer[2], &Buf[0], BufSz);

                Checksum = 0;
                /* Checksum Calculation */
                for(i = 0; i < SizeInWords - 1; ++i)
                {
                    Checksum += SBN_UIO_Mailbox_Data.OutputBuffer[i + 2];
                }
                /* Set the checksum. */
                SBN_UIO_Mailbox_Data.OutputBuffer[SizeInWords + 2] = Checksum;

                /* Blocking write. */
                (void) MailboxWrite(SBN_UIO_Mailbox_Data.Instance, &SBN_UIO_Mailbox_Data.OutputBuffer[0], SizeInWords + MAILBOX_HEADER_SIZE_WORDS);

                iStatus = CFE_ES_PutPoolBuf(Channel->MemPoolHandle, (uint32 *)buffer);
                if(iStatus < 0)
                {
                    (void) CFE_EVS_SendEvent(PQ_PUT_POOL_ERR_EID, CFE_EVS_ERROR,
                                "PutPoolBuf: error=0x%08lx",
                                    (unsigned long)iStatus);
                }
                else
                {
                    //OS_MutSemTake(TO_AppData.MutexID);
                    Channel->MemInUse -= iStatus;
                    //OS_MutSemGive(TO_AppData.MutexID);

                    PQ_Channel_LockByRef(Channel);
                    chQueue->CurrentlyQueuedCnt--;
                    chQueue->SentCount++;
                    PQ_Channel_UnlockByRef(Channel);
                }

            }
            else if(iStatus == OS_QUEUE_TIMEOUT)
            {
                /* Do nothing.  Just loop back around and check the guard. */
            }
            else
            {
                CFE_EVS_SendEvent(PQ_OSQUEUE_GET_ERROR_EID, CFE_EVS_ERROR,
                                "Send task failed to pop message from queue. (%i).", (int)iStatus);
            }
        }
    }
}


static int InitNet(SBN_NetInterface_t *Net)
{
    int Status = SBN_SUCCESS;

    memset(&SBN_UIO_Mailbox_Data, 0x0, sizeof(SBN_UIO_Mailbox_Data));

    SBN_UIO_Mailbox_Data.HkTlm.ChannelMaxMem = PQ_NUM_BYTES_IN_MEM_POOL;

    SBN_UIO_Mailbox_Data.Instance = InitDevice(MAILBOX_UIO_PATH);
    if(SBN_UIO_Mailbox_Data.Instance == NULL)
    {
        /* TODO update to event. */
        OS_printf("SBN_UIO_Mailbox InitDevice Instance Null.\n");
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* Initialize PQ channel. */
    Status = PQ_Channel_Init(SBN_PQ_CHANNEL_NUMBER, &SBN_UIO_Mailbox_Data.Channel);
    if (Status != CFE_SUCCESS)
    {
        /* TODO update to event. */
        OS_printf("PQ_Channel_Init failed%u\n", Status);
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* Open PQ channel. */
    Status = PQ_Channel_OpenChannel(
                      &SBN_UIO_Mailbox_Data.Channel,
                      SBN_PQ_CHANNEL_NAME,
                      SBN_PQ_CONFIG_TABLENAME,
                      SBN_PQ_CONFIG_TABLE_FILENAME,
                      &PQ_BackupConfigTbl,
                      SBN_PQ_DUMP_TABLENAME, 
                      SBN_PQ_CF_SEM_INIT_VALUE, 
                      SBN_PQ_CF_THROTTLE_SEM_NAME);

    if (Status != CFE_SUCCESS)
    {
        /* TODO update to event. */
        OS_printf("PQ_Channel_OpenChannel failed %u\n", Status);
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* Create send task. */
    SBN_UIO_Mailbox_Data.SendTask = SBN_PQ_Output_Task;
    Status = CFE_ES_CreateChildTask(
        &SBN_UIO_Mailbox_Data.ChildTaskID,
        SBN_PQ_SEND_TASK_NAME,
        SBN_UIO_Mailbox_Data.SendTask,
        0,
        SBN_PQ_SEND_TASK_STACK_SIZE,
        SBN_PQ_SEND_TASK_PRIORITY,
        SBN_PQ_SEND_TASK_FLAGS);
    if (Status != CFE_SUCCESS)
    {
        /* TODO update to event. */
        printf("CFE_ES_CreateChildTask failed%u\n", Status);
        Status = SBN_ERROR;
        goto end_of_function;
    }

end_of_function:
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

    printf("MsgSz into queue %u\n", MsgSz);
    /* Push message onto the PQ */
    PQ_Channel_ProcessTelemetry(&SBN_UIO_Mailbox_Data.Channel, Payload);

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
    PQ_Channel_CopyStats(&SBN_UIO_Mailbox_Data.HkTlm, &SBN_UIO_Mailbox_Data.Channel);

    CFE_SB_InitMsg(&SBN_UIO_Mailbox_Data.HkTlm, SBN_MODULE_HK_MID, 
                   sizeof(SBN_UIO_Mailbox_Data.HkTlm), FALSE);
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &SBN_UIO_Mailbox_Data.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &SBN_UIO_Mailbox_Data.HkTlm);

    return SBN_SUCCESS;
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


