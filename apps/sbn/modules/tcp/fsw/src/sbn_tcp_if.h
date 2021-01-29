#ifndef _sbn_tcp_if_h_
#define _sbn_tcp_if_h_

#include "sbn_constants.h"
#include "sbn_interfaces.h"
#include "cfe.h"

int SBN_TCP_LoadNet(const char **Row, int FieldCount,
        SBN_NetInterface_t *Net);

int SBN_TCP_LoadPeer(const char **Row, int FieldCount,
        SBN_PeerInterface_t *Peer);

int SBN_TCP_InitNet(SBN_NetInterface_t *Net);

int SBN_TCP_InitPeer(SBN_PeerInterface_t *Peer);

int SBN_TCP_PollPeer(SBN_PeerInterface_t *Peer);

int SBN_TCP_Send(SBN_PeerInterface_t *Peer, SBN_MsgType_t MsgType,
    SBN_MsgSz_t MsgSz, void *Payload);

int SBN_TCP_Recv(SBN_NetInterface_t *Net, SBN_PeerInterface_t *Peer,
        SBN_MsgType_t *MsgTypePtr, SBN_MsgSz_t *MsgSzPtr,
        SBN_CpuID_t *CpuIDPtr, void *PayloadBuffer);

int SBN_TCP_ReportModuleStatus(SBN_ModuleStatusPacket_t *Packet);

int SBN_TCP_ResetPeer(SBN_PeerInterface_t *Peer);

int SBN_TCP_UnloadNet(SBN_NetInterface_t *Net);

int SBN_TCP_UnloadPeer(SBN_PeerInterface_t *Peer);

SBN_IfOps_t SBN_TCP_Ops =
{
    SBN_TCP_LoadNet,
    SBN_TCP_LoadPeer,
    SBN_TCP_InitNet,
    SBN_TCP_InitPeer,
    SBN_TCP_PollPeer,
    SBN_TCP_Send,
    SBN_TCP_Recv,
    NULL,
    SBN_TCP_ReportModuleStatus,
    SBN_TCP_ResetPeer,
    SBN_TCP_UnloadNet,
    SBN_TCP_UnloadPeer
};

#define SBN_TCP_HEARTBEAT_MSG 0xA0

#endif /* _sbn_tcp_if_h_ */
