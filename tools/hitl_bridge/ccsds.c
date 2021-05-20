/*
** Include Files
*/

#include "ccsds.h"


/******************************************************************************
**  Function:  CCSDS_InitPkt()
**
**  Purpose:
**    Initialize a CCSDS packet.  The primary header is initialized with
**    specified values, and if the Clear flag is set, the rest of the packet
**    is filled with zeros.
**
**  Arguments:
**    PktPtr   : Pointer to primary header of packet.
**    StreamId : Stream ID to use for the packet.
**    Length   : Length of the packet in bytes.
**    Clear    : Indicates whether to clear the entire packet:
**                TRUE = fill sequence count and packet data with zeros
**                       (used after a cold restart)
**                FALSE = leave sequence count and packet data unchanged
**                       (used after a warm restart if data must be preserved)
**
**  Return:
**    (none)
*/

void CCSDS_InitPkt (CCSDS_PriHdr_t  *PktPtr,
                    uint16_t         StreamId,
                    uint16_t         Length,
                    uint8_t          Clear )
{
    uint16_t SeqCount;

    /* Save the sequence count in case it must be preserved. */
    SeqCount = CCSDS_RD_SEQ(*PktPtr);

    /* Zero the entire packet if needed. */
    if (Clear)  memset((void *)PktPtr, 0, Length);

    /* Clear the primary header. */
    CCSDS_CLR_PRI_HDR(*PktPtr);

    /* Set the stream ID and length fields in the primary header. */
    CCSDS_WR_SID(*PktPtr, StreamId);
    CCSDS_WR_LEN(*PktPtr, Length);

    /* Restore the sequence count if needed. */
    if (!Clear)  CCSDS_WR_SEQ(*PktPtr, SeqCount);

} /* END CCSDS_InitPkt() */


/******************************************************************************
**  Function:  CCSDS_LoadCheckSum()
**
**  Purpose:
**    Compute and load a checksum for a CCSDS command packet that has a
**    secondary header.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.  The checksum field in the packet
**               will be modified.
**
**  Return:
**    (none)
*/

void CCSDS_LoadCheckSum (CCSDS_CmdPkt_t *PktPtr)
{
   uint8_t CheckSum;

   /* Clear the checksum field so the new checksum is correct. */
   CCSDS_WR_CHECKSUM(PktPtr->SecHdr, 0);

   /* Compute and load new checksum. */
   CheckSum = CCSDS_ComputeCheckSum(PktPtr);
   CCSDS_WR_CHECKSUM(PktPtr->SecHdr, CheckSum);

} /* END CCSDS_LoadCheckSum() */


/******************************************************************************
**  Function:  CCSDS_ValidCheckSum()
**
**  Purpose:
**    Determine whether a checksum in a command packet is valid.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
**    A valid checksum is 0.
*/

uint8_t CCSDS_ValidCheckSum (CCSDS_CmdPkt_t *PktPtr)
{

   return (CCSDS_ComputeCheckSum(PktPtr) == 0);

} /* END CCSDS_ValidCheckSum() */


/******************************************************************************
**  Function:  CCSDS_ComputeCheckSum()
**
**  Purpose:
**    Compute the checksum for a command packet.  The checksum is the XOR of
**    all bytes in the packet; a valid checksum is zero.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
*/

uint8_t CCSDS_ComputeCheckSum (CCSDS_CmdPkt_t *PktPtr)
{
	uint16_t   PktLen   = CCSDS_RD_LEN(PktPtr->PriHdr);
	uint8_t   *BytePtr  = (uint8_t *)PktPtr;
	uint8_t    CheckSum;

    CheckSum = 0xFF;
    while (PktLen--)  CheckSum ^= *(BytePtr++);

    return CheckSum;

} /* END CCSDS_ComputeCheckSum() */



CFE_SB_MsgId_t CFE_SB_GetMsgId(CFE_SB_MsgPtr_t MsgPtr)
{
    return CCSDS_RD_SID(MsgPtr->Hdr);
}/* end CFE_SB_GetMsgId */


void *CFE_SB_GetUserData(CFE_SB_MsgPtr_t MsgPtr)
{
    uint8_t         *BytePtr;
    CFE_SB_MsgId_t  MsgId;
    uint16_t        HdrSize;

    BytePtr = (uint8_t *)MsgPtr;
    MsgId = CCSDS_RD_SID(MsgPtr->Hdr);
    HdrSize = CFE_SB_MsgHdrSize(MsgId);

    return (BytePtr + HdrSize);
}


uint16_t CFE_SB_MsgHdrSize(CFE_SB_MsgId_t MsgId)
{
    uint16_t size;
    CCSDS_PriHdr_t  CCSDSPriHdr;

    CCSDS_WR_SID(CCSDSPriHdr,MsgId);

    /* if secondary hdr is not present... */
    if(CCSDS_RD_SHDR(CCSDSPriHdr) == 0){

        size = sizeof(CCSDS_PriHdr_t);

    }else if(CCSDS_RD_TYPE(CCSDSPriHdr) == CCSDS_CMD){

        size = CFE_SB_CMD_HDR_SIZE;

    }else{

        size = CFE_SB_TLM_HDR_SIZE;
    }

    return size;

}/* end CFE_SB_MsgHdrSize */

/*****************************************************************************/
