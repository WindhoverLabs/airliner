#include "sbn_mailbox_if.h"
#include "sbn_interfaces.h"
#include "sbn_platform_cfg.h"


SBN_Mailbox_Data_t SBN_Mailbox_Data;


int MailboxWrite(XMbox *instance, const unsigned int *buffer, unsigned int size)
{
    int Status             = 0;
    unsigned int BytesSent = 0;
    
    Status = XMbox_Write(instance, buffer, size, &BytesSent);
    if(Status != XST_SUCCESS)
    {
        printf("XMbox_Write Failed %u.\r\n", Status);
    }

    Status = BytesSent;

end_of_function:
    return Status;
}


int MailboxRead(XMbox *instance, unsigned int *buffer, unsigned int size)
{
    int Status              = 0;
    unsigned int BytesRecvd = 0;

    Status = XMbox_Read(instance, buffer, size, BytesRecvd);

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

end_of_function:
    return Status;
}


static int InitNet(SBN_NetInterface_t *Net)
{
    int Status = SBN_SUCCESS;

    memset(&SBN_Mailbox_Data, 0x0, sizeof(SBN_UIO_Mailbox_Data));

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
    SizeOutWords = MailboxWrite(SBN_UIO_Mailbox_Data.Mbox, &SBN_UIO_Mailbox_Data.OutputBuffer[0], SizeInWords + MAILBOX_HEADER_SIZE_WORDS);
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
    Status = SBN_MailboxSend(SBN_UIO_Mailbox_Data.Mbox, &SBN_UIO_Mailbox_Data.PackedBuffer[0], TotalSize);

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


