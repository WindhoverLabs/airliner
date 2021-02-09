#include "sbn_udp_if_struct.h"
#include "sbn_udp_if.h"
#include "sbn_udp_events.h"
#include "cfe.h"
#include <string.h>
#include <errno.h>
#include "pq_includes.h"
#include "msg_ids.h"
#include "pq_events.h"
#include "mailbox_parser.h"

/* at some point this will be replaced by the OSAL network interface */
#ifdef _VXWORKS_OS_
#include "selectLib.h"
#else
#include <sys/select.h>
#endif


#define MAILBOX_HEADER_SIZE_BYTES (12)
#define MAILBOX_WORD_SIZE         (4)
#define MAILBOX_HEADER_SIZE_WORDS (MAILBOX_HEADER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_MAX_BUFFER_SIZE_BYTES   (1500)
#define MAILBOX_MAX_BUFFER_SIZE_WORDS   (MAILBOX_MAX_BUFFER_SIZE_BYTES/MAILBOX_WORD_SIZE)
#define MAILBOX_SBN_HEADER_SIZE_WORDS   (2)
#define MAILBOX_SBN_TASK_DELAY_MSEC     (1)


void SBN_PQ_Output_Task(void);
void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel);

int SBN_UDP_Send_Direct(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType,
    SBN_MsgSz_t MsgSz, void *Payload);

extern PQ_ChannelTbl_t PQ_BackupConfigTbl;

/* TODO add to global data struct. */
PQ_ChannelData_t Channel;
PQ_HkTlm_t HkTlm;
struct sockaddr_in s_addr;
int Socket;
/* Mailbox specific */
unsigned int OutputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
unsigned int InputBuffer[1500/sizeof(unsigned int)] __attribute__ ((aligned(4)));
unsigned int ParserBuffer[1500/sizeof(unsigned int)];
Mailbox_Parser_Handle_t Parser;


int SBN_UDP_LoadNet(const char **Row, int FieldCnt,
    SBN_NetInterface_t *Net)
{
    char *ValidatePtr = NULL;
    SBN_UDP_Net_t *NetData = (SBN_UDP_Net_t *)Net->ModulePvt;

    if(FieldCnt < SBN_UDP_ITEMS_PER_FILE_LINE)
    {
        CFE_EVS_SendEvent(SBN_UDP_CONFIG_EID, CFE_EVS_ERROR,
                "invalid host entry (expected %d items, found %d)",
                SBN_UDP_ITEMS_PER_FILE_LINE, FieldCnt);
        return SBN_ERROR;
    }/* end if */

    strncpy(NetData->Host, Row[0], sizeof(NetData->Host));
    NetData->Port = strtol(Row[1], &ValidatePtr, 0);
    if(!ValidatePtr || ValidatePtr == Row[1])
    {
        CFE_EVS_SendEvent(SBN_UDP_CONFIG_EID, CFE_EVS_ERROR,
                "invalid port");
    }/* end if */

    return SBN_SUCCESS;
}/* end SBN_UDP_LoadNet */

int SBN_UDP_LoadPeer(const char **Row, int FieldCnt,
    SBN_PeerInterface_t *Peer)
{
    char *ValidatePtr = NULL;
    SBN_UDP_Peer_t *PeerData = (SBN_UDP_Peer_t *)Peer->ModulePvt;

    if(FieldCnt < SBN_UDP_ITEMS_PER_FILE_LINE)
    {
        CFE_EVS_SendEvent(SBN_UDP_CONFIG_EID, CFE_EVS_ERROR,
                "invalid peer entry (expected %d items, found %d)",
                SBN_UDP_ITEMS_PER_FILE_LINE, FieldCnt);
        return SBN_ERROR;
    }/* end if */

    strncpy(PeerData->Host, Row[0], sizeof(PeerData->Host));
    PeerData->Port = strtol(Row[1], &ValidatePtr, 0);
    if(!ValidatePtr || ValidatePtr == Row[1])
    {
        CFE_EVS_SendEvent(SBN_UDP_CONFIG_EID, CFE_EVS_ERROR,
                "invalid port");
    }/* end if */

    return SBN_SUCCESS;
}/* end SBN_UDP_LoadHost */

/**
 * Initializes an UDP host.
 *
 * @param  Interface data structure containing the file entry
 * @return SBN_SUCCESS on success, error code otherwise
 */
int SBN_UDP_InitNet(SBN_NetInterface_t *Net)
{
    int32 iStatus = 0;
    SBN_UDP_Net_t *NetData = (SBN_UDP_Net_t *)Net->ModulePvt;

    CFE_EVS_SendEvent(SBN_UDP_SOCK_EID, CFE_EVS_DEBUG,
        "creating socket (%s:%d)", NetData->Host, NetData->Port);

    if((NetData->Socket
        = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        CFE_EVS_SendEvent(SBN_UDP_SOCK_EID, CFE_EVS_ERROR,
            "socket call failed (errno=%d)", errno);
        return SBN_ERROR;
    }/* end if */

    iStatus = PQ_Channel_Init(0, &Channel);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO update to event. */
        printf("PQ_Channel_Init failed%u\n", iStatus);
        return SBN_ERROR;
    }

    iStatus = PQ_Channel_OpenChannel(
                      &Channel,
                      PQ_UDP_CHANNEL_NAME,
                      PQ_UDP_CONFIG_TABLENAME,
                      PQ_UDP_CONFIG_TABLE_FILENAME,
                      &PQ_BackupConfigTbl,
                      PQ_UDP_DUMP_TABLENAME, 
                      1, 
                      PQ_UDP_CF_THROTTLE_SEM_NAME);

    if (iStatus != CFE_SUCCESS)
    {
        /* TODO update to event. */
        printf("PQ_Channel_OpenChannel failed %u\n", iStatus);
        return SBN_ERROR;
    }

    static struct sockaddr_in my_addr;

    my_addr.sin_addr.s_addr = inet_addr(NetData->Host);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(NetData->Port);

    if(bind(NetData->Socket, (struct sockaddr *) &my_addr,
        sizeof(my_addr)) < 0)
    {
        CFE_EVS_SendEvent(SBN_UDP_SOCK_EID, CFE_EVS_ERROR,
            "bind call failed (%s:%d Socket=%d errno=%d)",
            NetData->Host, NetData->Port, NetData->Socket, errno);
        return SBN_ERROR;
    }/* end if */
    else
    {
        /* TODO update to event. */
        printf("SBN bound %s:%d\n", NetData->Host, NetData->Port);
    }

    char TaskName[OS_MAX_API_NAME];
    snprintf(TaskName, OS_MAX_API_NAME, "PQ_OUTCH_%u", 0);
    uint32 ChildTaskID = 0;
    CFE_ES_ChildTaskMainFuncPtr_t ListenerTask = SBN_PQ_Output_Task;

    /* Create a child task here. */
    iStatus = CFE_ES_CreateChildTask(
        &ChildTaskID,
        (const char *)TaskName,
        ListenerTask,
        0,
        131072,
        50,
        OS_ENABLE_CORE_0);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO update to event. */
        printf("CFE_ES_CreateChildTask failed%u\n", iStatus);
        return SBN_ERROR;
    }

    memset(&Parser, 0x0, sizeof(Parser));

    return SBN_SUCCESS;
}/* end SBN_UDP_InitNet */

/**
 * Initializes an UDP host or peer data struct depending on the
 * CPU name.
 *
 * @param  Interface data structure containing the file entry
 * @return SBN_SUCCESS on success, error code otherwise
 */
int SBN_UDP_InitPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
}/* end SBN_UDP_InitPeer */

int SBN_UDP_PollPeer(SBN_PeerInterface_t *Peer)
{
    //OS_time_t CurrentTime;
    //OS_GetLocalTime(&CurrentTime);

    //SBN_UDP_Peer_t *PeerData = (SBN_UDP_Peer_t *)Peer->ModulePvt;
    
    //if(PeerData->ConnectedFlag)
    //{
        //if(CurrentTime.seconds - Peer->LastRecv.seconds
            //> SBN_UDP_PEER_TIMEOUT)
        //{
            //CFE_EVS_SendEvent(SBN_UDP_DEBUG_EID, CFE_EVS_INFORMATION,
                //"CPU %d disconnected", Peer->ProcessorID);
            //PeerData->ConnectedFlag = FALSE;
            //return 0;
        //}/* end if */

        //if(CurrentTime.seconds - Peer->LastSend.seconds
            //> SBN_UDP_PEER_HEARTBEAT)
        //{
            //return SBN_UDP_Send_Direct(Peer, SBN_UDP_HEARTBEAT_MSG, 0, NULL);
        //}/* end if */
    //}
    //else
    //{
        //if(CurrentTime.seconds - Peer->LastSend.seconds
            //> SBN_UDP_ANNOUNCE_TIMEOUT)
        //{
            //return SBN_UDP_Send_Direct(Peer, SBN_UDP_ANNOUNCE_MSG, 0, NULL);
        //}/* end if */
    //}/* end if */
    return SBN_SUCCESS;
}/* end SBN_UDP_PollPeer */

int SBN_UDP_Send(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType,
    SBN_MsgSz_t MsgSz, void *Payload)
{
    //SBN_UDP_Send_Direct(Peer, MsgType, MsgSz, Payload);
    //return SBN_SUCCESS;
    
    printf("MsgSz into queue %u\n", MsgSz);
    PQ_Channel_LockByRef(&Channel);
    PQ_Classifier_Run(&Channel, Payload, &HkTlm);
    PQ_Scheduler_Run(&Channel);
    PQ_Channel_UnlockByRef(&Channel);

    //size_t BufSz = MsgSz + SBN_PACKED_HDR_SZ;
    //uint8 Buf[BufSz];

    SBN_UDP_Peer_t *PeerData = (SBN_UDP_Peer_t *)Peer->ModulePvt;
    SBN_NetInterface_t *Net = Peer->Net;
    SBN_UDP_Net_t *NetData = (SBN_UDP_Net_t *)Net->ModulePvt;

    //SBN_PackMsg(&Buf, MsgSz, MsgType, CFE_PSP_GetProcessorId(), Payload);

    /* TODO Save off this information for now. */
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(PeerData->Host);
    s_addr.sin_port = htons(PeerData->Port);
    Socket = NetData->Socket;

    //sendto(NetData->Socket, &Buf, BufSz, 0, (struct sockaddr *) &s_addr,
        //sizeof(s_addr));

    return SBN_SUCCESS;
}/* end SBN_UDP_Send */


/* Note that this Recv function is indescriminate, packets will be received
 * from all peers but that's ok, I just inject them into the SB and all is
 * good!
 */
int SBN_UDP_Recv(SBN_NetInterface_t *Net, SBN_MsgType_t *MsgTypePtr,
        SBN_MsgSz_t *MsgSzPtr, SBN_CpuID_t *CpuIDPtr,
        void *Payload)
{
    uint8 RecvBuf[SBN_MAX_PACKED_MSG_SZ];

    SBN_UDP_Net_t *NetData = (SBN_UDP_Net_t *)Net->ModulePvt;

#ifndef SBN_RECV_TASK

    /* task-based peer connections block on reads, otherwise use select */
  
    fd_set ReadFDs;
    struct timeval Timeout;

    FD_ZERO(&ReadFDs);
    FD_SET(NetData->Socket, &ReadFDs);

    memset(&Timeout, 0, sizeof(Timeout));

    if(select(FD_SETSIZE, &ReadFDs, NULL, NULL, &Timeout) == 0)
    {
        /* nothing to receive */
        return SBN_IF_EMPTY;
    }/* end if */

#endif /* !SBN_RECV_TASK */

    int Received = recv(NetData->Socket, (char *)&RecvBuf,
        CFE_SB_MAX_SB_MSG_SIZE, 0);

    if(Received < 0)
    {
        return SBN_ERROR;
    }/* end if */

    /* each UDP packet is a full SBN message */

    /******************************************************************/
    printf("Received %u\n", Received);
    memcpy(&InputBuffer[0], &RecvBuf[0], Received);

    /* TODO this can't handle two messages in a buffer. */
    if(Received > 0)
    {
        int i = 0;
        /* Add check for word length. */
        for(i = 0; i < Received/4; ++i)
        {
            unsigned int Size = MAILBOX_MAX_BUFFER_SIZE_BYTES;
            unsigned int Status = ParseMessage(&Parser,
                                               InputBuffer[i],
                                               &ParserBuffer[0],
                                               &Size);
            if(Status == MPS_MESSAGE_COMPLETE)
            {
                printf("message complete.\n");
                if (SBN_UnpackMsg(&ParserBuffer[0], MsgSzPtr, MsgTypePtr, CpuIDPtr, Payload) == FALSE)
                {
                    OS_printf("Unpack failed.\n");
                    return SBN_ERROR;
                }
            }
        }
    }
    /******************************************************************/

    //if(SBN_UnpackMsg(&RecvBuf, MsgSzPtr, MsgTypePtr, CpuIDPtr, Payload)
        //== FALSE)
    //{
        //return SBN_ERROR;
    //}/* end if */
    
    CFE_SB_MsgId_t MsgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)Payload);
    printf("Received %u CPUID %u, %x\n", Received, *CpuIDPtr, MsgID);

    SBN_PeerInterface_t *Peer = SBN_GetPeer(Net, *CpuIDPtr);
    if(Peer == NULL)
    {
        return SBN_ERROR;
    }

    SBN_UDP_Peer_t *PeerData = (SBN_UDP_Peer_t *)Peer->ModulePvt;

    if(!PeerData->ConnectedFlag)
    {
        CFE_EVS_SendEvent(SBN_UDP_DEBUG_EID, CFE_EVS_INFORMATION,
            "CPU %d connected", *CpuIDPtr);

        PeerData->ConnectedFlag = TRUE;

        SBN_SendLocalSubsToPeer(Peer);
    }

    return SBN_SUCCESS;
}/* end SBN_UDP_Recv */


int SBN_UDP_ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet)
{
    return SBN_NOT_IMPLEMENTED;
}/* end SBN_UDP_ReportModuleStatus */


int SBN_UDP_ResetPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_NOT_IMPLEMENTED;
}/* end SBN_UDP_ResetPeer */


int SBN_UDP_UnloadNet(SBN_NetInterface_t *Net)
{
    SBN_UDP_Net_t *NetData = (SBN_UDP_Net_t *)Net->ModulePvt;

    close(NetData->Socket);

    int PeerIdx = 0;
    for(PeerIdx = 0; PeerIdx < Net->PeerCnt; PeerIdx++)
    {
        SBN_UDP_UnloadPeer(&Net->Peers[PeerIdx]);
    }/* end if */

    return SBN_SUCCESS;
}/* end SBN_UDP_UnloadNet */

int SBN_UDP_UnloadPeer(SBN_PeerInterface_t *Peer)
{
    return SBN_SUCCESS;
}/* end SBN_UDP_UnloadPeer */


void SBN_PQ_Output_Task(void)
{
    CFE_ES_RegisterChildTask();

    SBN_PQ_ChannelHandler(&Channel);

    CFE_ES_ExitChildTask();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Channel Handler                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SBN_PQ_ChannelHandler(PQ_ChannelData_t *Channel)
{
    int32 iStatus = CFE_SUCCESS;
    uint32 msgSize = 0;
    char *buffer;

    /* TODO add flag */
    while(1)
    {
        if(PQ_Channel_State(Channel) == PQ_CHANNEL_OPENED)
        {
            PQ_OutputQueue_t *chQueue = &Channel->OutputQueue;

            iStatus =  OS_QueueGet(
                    chQueue->OSALQueueID,
                    &buffer, sizeof(buffer), &msgSize, 500);

            if(iStatus == OS_SUCCESS)
            {
                uint16 actualMessageSize = CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)buffer);
                CFE_SB_MsgId_t MsgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)buffer);
                SBN_MsgType_t MsgType;
                size_t BufSz = actualMessageSize + SBN_PACKED_HDR_SZ;
                uint8 Buf[BufSz];
                int status = 0;
                uint32 outputWord = 0;

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
                
                //printf("sent %u\n", BufSz);

                /* Mailbox specific */
                /******************************************************/
                unsigned int SizeInBytes = 0;
                unsigned int SizeInWords = 0;
                unsigned int Checksum    = 0;
                unsigned int i           = 0;

                SizeInBytes = BufSz;
                /* Ensure word boundary */
                if(SizeInBytes % MAILBOX_WORD_SIZE)
                {
                    SizeInBytes = (BufSz + (MAILBOX_WORD_SIZE - (BufSz % MAILBOX_WORD_SIZE)));
                    SizeInWords = SizeInBytes / MAILBOX_WORD_SIZE;
                }

                printf("BufSz %u\n", BufSz);
                printf("SizeInBytes %u\n", SizeInBytes);
                printf("SizeInWords %u\n", SizeInWords);

                if(SizeInWords + MAILBOX_HEADER_SIZE_WORDS > MAILBOX_MAX_BUFFER_SIZE_WORDS)
                {
                    /* TODO update to event. */
                    OS_printf("SBN_MailboxSend Send size too large %u bytes, %u words.\n", SizeInBytes, SizeInWords);
                    continue;
                }

                /* Set CADU. */
                OutputBuffer[0] = MAILBOX_MSG_CADU;
                /* Set size of the payload in words. */
                OutputBuffer[1] = SizeInWords;
                /* Copy the payload. */
                memcpy(&OutputBuffer[2], &Buf[0], BufSz);

                Checksum = 0;
                /* Checksum Calculation */
                for(i = 0; i < SizeInWords - 1; ++i)
                {
                    Checksum += OutputBuffer[i + 2];
                }
                /* Set the checkout. */
                OutputBuffer[SizeInWords + 2] = Checksum;

                /* Send payload size plus mailbox header size. */
                status = sendto(Socket, (const char*)&OutputBuffer[0], SizeInBytes + MAILBOX_HEADER_SIZE_BYTES, 0, (struct sockaddr *) &s_addr,
                sizeof(s_addr));
                if (status < 0)
                {
                    /* TODO */
                    printf("sendto failed errno %d\n", errno);
                    iStatus = -1;
                }
                /******************************************************/
                printf("sent %u\n", SizeInBytes + MAILBOX_HEADER_SIZE_BYTES);
                //status = sendto(Socket, (const char*)Buf, BufSz, 0, (struct sockaddr *) &s_addr,
                //sizeof(s_addr));
                //if (status < 0)
                //{
                    ///* TODO */
                    //printf("sendto failed errno %d\n", errno);
                    //iStatus = -1;
                //}

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
                                "Listener failed to pop message from queue. (%i).", (int)iStatus);
            }
        }
    }
}

