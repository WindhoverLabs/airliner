#include "sbn_mailbox_if.h"
#include "sbn_interfaces.h"
#include "sbn_platform_cfg.h"
#include "stdbool.h"
#include <string.h>

extern PQ_ChannelTbl_t PQ_BackupConfigTbl;

SBN_Mailbox_Data_t SBN_Mailbox_Data;

void SBN_PQ_Output_Task(void);
void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel);


/* Blocking write */
int MailboxWrite(XMbox *instance, const unsigned int *buffer, unsigned int size)
{
    int Status                  = 0;
    unsigned int BytesSent      = 0;
    unsigned int TotalBytesSent = 0;
    /* Total size in bytes of data to send. */
    unsigned int RequestedBytes = size * 4;

    while(1)
    {
        XMbox_Write(instance, &buffer[TotalBytesSent/4], RequestedBytes, &BytesSent);
        /* Subtract bytes sent. */
        RequestedBytes = RequestedBytes - BytesSent;
        /* Add bytes sent to total bytes sent. */
        TotalBytesSent = TotalBytesSent + BytesSent;
        if(TotalBytesSent < (size * 4))
        {
            /* Sleep */
            OS_TaskDelay(SBN_MAILBOX_BLOCKING_DELAY);
        }
        else
        {
            break;
        }
    }

    Status = TotalBytesSent;

end_of_function:
    return Status;
}


int MailboxRead(XMbox *instance, unsigned int *buffer, unsigned int size)
{
    int Status              = 0;
    unsigned int BytesRecvd = 0;

    Status = XMbox_Read(instance, buffer, size, &BytesRecvd);

    if(Status == XST_NO_DATA)
    {
        Status = 0;
        goto end_of_function;
    }

    if(Status == XST_SUCCESS)
    {
        Status = BytesRecvd;
    }
    else
    {
        printf("XMbox_Read Failed %u.\r\n", Status);
    }

    printf("received %u\n", Status);

end_of_function:
    return Status;
}


static int InitNet(SBN_NetInterface_t *Net)
{
    int Status = SBN_SUCCESS;

    memset(&SBN_Mailbox_Data, 0x0, sizeof(SBN_Mailbox_Data));

    SBN_Mailbox_Data.HkTlm.ChannelMaxMem = PQ_NUM_BYTES_IN_MEM_POOL;

    SBN_Mailbox_Data.MboxConfigPtr = XMbox_LookupConfig(XPAR_SED_MBOX_MAILBOX_CPD_TO_SED_IF_1_DEVICE_ID);
    if (SBN_Mailbox_Data.MboxConfigPtr == (XMbox_Config *)NULL)
    {
        printf ("XMbox_LookupConfig Failed.\r\n");
        Status = SBN_ERROR;
        goto end_of_function;
    }

    Status = XMbox_CfgInitialize(&SBN_Mailbox_Data.Mbox, 
                                 SBN_Mailbox_Data.MboxConfigPtr, 
                                 SBN_Mailbox_Data.MboxConfigPtr->BaseAddress);
    if (Status != XST_SUCCESS)
    {
        printf ("XMbox_CfgInitialize Failed.\r\n");
        Status = SBN_ERROR;
        goto end_of_function;
    }


    /* Initialize PQ channel. */
    Status = PQ_Channel_Init(SBN_PQ_CHANNEL_NUMBER, &SBN_Mailbox_Data.Channel);
    if (Status != CFE_SUCCESS)
    {
        /* TODO update to event. */
        OS_printf("PQ_Channel_Init failed%u\n", Status);
        Status = SBN_ERROR;
        goto end_of_function;
    }

    /* Open PQ channel. */
    Status = PQ_Channel_OpenChannel(
                      &SBN_Mailbox_Data.Channel,
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
    SBN_Mailbox_Data.SendTask = SBN_PQ_Output_Task;
    Status = CFE_ES_CreateChildTask(
        &SBN_Mailbox_Data.ChildTaskID,
        SBN_PQ_SEND_TASK_NAME,
        SBN_Mailbox_Data.SendTask,
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


int SBN_MailboxRecv(void *instance, const unsigned int *buffer, unsigned int size)
{
    
}


void SBN_PQ_Output_Task(void)
{
    CFE_ES_RegisterChildTask();

    SBN_PQ_ChannelHandler(&SBN_Mailbox_Data.Channel);

    CFE_ES_ExitChildTask();
}


void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel)
{
    int32 iStatus = CFE_SUCCESS;
    uint32 msgSize = 0;
    char *buffer;
    
    SBN_Mailbox_Data.TaskContinueFlag = TRUE;

    while(SBN_Mailbox_Data.TaskContinueFlag)
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
                SBN_Mailbox_Data.OutputBuffer[0] = MAILBOX_MSG_CADU;
                /* Set size of the payload in words. */
                SBN_Mailbox_Data.OutputBuffer[1] = SizeInWords;
                /* Copy the payload. */
                memcpy(&SBN_Mailbox_Data.OutputBuffer[2], &Buf[0], BufSz);

                Checksum = 0;
                /* Checksum Calculation */
                for(i = 0; i < SizeInWords - 1; ++i)
                {
                    Checksum += SBN_Mailbox_Data.OutputBuffer[i + 2];
                }
                /* Set the checksum. */
                SBN_Mailbox_Data.OutputBuffer[SizeInWords + 2] = Checksum;

                /* Blocking write. */
                (void) MailboxWrite(&SBN_Mailbox_Data.Mbox, &SBN_Mailbox_Data.OutputBuffer[0], SizeInWords + MAILBOX_HEADER_SIZE_WORDS);

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
    PQ_Channel_ProcessTelemetry(&SBN_Mailbox_Data.Channel, Payload);

end_of_function:
    return Status;
}


static int Recv(SBN_NetInterface_t *Net, SBN_MsgType_t *MsgTypePtr,
                SBN_MsgSz_t *MsgSzPtr, SBN_CpuID_t *CpuIDPtr, void *Payload)
{
    int SizeRead            = 0;
    unsigned int i          = 0;
    int ReturnValue         = SBN_IF_EMPTY;
    boolean MessageComplete = FALSE;

    SizeRead = MailboxRead(&SBN_Mailbox_Data.Mbox, 
                           &SBN_Mailbox_Data.InputBuffer[0], 
                           sizeof(SBN_Mailbox_Data.InputBuffer));
    if(SizeRead > 0)
    {
        for(i = 0; i < SizeRead; ++i)
        {
            unsigned int Size = MAILBOX_MAX_BUFFER_SIZE_BYTES;
            unsigned int Status = ParseMessage(&SBN_Mailbox_Data.Parser,
                                               SBN_Mailbox_Data.InputBuffer[i],
                                               &SBN_Mailbox_Data.ParserBuffer[0],
                                               &Size);
            if(Status == MPS_MESSAGE_COMPLETE)
            {
                printf("message complete\n");
                MessageComplete = TRUE;
                if (SBN_UnpackMsg(&SBN_Mailbox_Data.ParserBuffer[0], MsgSzPtr, MsgTypePtr, CpuIDPtr, Payload) == false)
                {
                    OS_printf("Unpack failed.\n");
                    ReturnValue = SBN_ERROR;
                    goto end_of_function;
                }
                ReturnValue = SBN_SUCCESS;
            }
        }
    }
    
    if(MessageComplete == FALSE)
    {
        ReturnValue = SBN_IF_EMPTY;
        goto end_of_function;
    }

    CFE_SB_MsgId_t MsgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)Payload);
    printf("Received %u CPUID %u, %x\n", SizeRead, *CpuIDPtr, MsgID);

    /* TODO move this into the if connected check. */
    SBN_PeerInterface_t *Peer = SBN_GetPeer(Net, *CpuIDPtr);
    if(Peer == NULL)
    {
        ReturnValue = SBN_ERROR;
        goto end_of_function;
    }

    /* TODO update this flag to peer data. */
    if(!SBN_Mailbox_Data.ConnectedFlag)
    {
        OS_printf("CPU %d connected", *CpuIDPtr);

        SBN_Mailbox_Data.ConnectedFlag = TRUE;

        SBN_SendLocalSubsToPeer(Peer);
    }

#ifdef SBN_RECV_TASK
    (void) OS_TaskDelay(MAILBOX_SBN_TASK_DELAY_MSEC);
#endif

end_of_function:
    return ReturnValue;
}


static int ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet)
{
    PQ_Channel_CopyStats(&SBN_Mailbox_Data.HkTlm, &SBN_Mailbox_Data.Channel);

    CFE_SB_InitMsg(&SBN_Mailbox_Data.HkTlm, SBN_MODULE_HK_MID, 
                   sizeof(SBN_Mailbox_Data.HkTlm), FALSE);
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &SBN_Mailbox_Data.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &SBN_Mailbox_Data.HkTlm);

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


SBN_IfOps_t SBN_Mbox_Ops = 
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


