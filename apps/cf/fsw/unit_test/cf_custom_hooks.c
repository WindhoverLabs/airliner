/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "cf_custom_hooks.h"
#include "cf_test_utils.h"
#include "cf_utils.h"
#include "structures.h"

#include "cf_app.h"

#include "ut_cfe_sb_hooks.h"

#include <string.h>


os_dir_t     OpendirHookDir;

uint32       ReaddirHookReturnCnt = 0;
os_dirent_t  ReaddirHookDirEntry;

uint32       SemGetInfoHookCallCnt = 0;
uint32       SemGetIdByNameHookCallCnt = 0;

uint32       ZeroCopyGetPtrHookCallCnt = 0;
uint32       ZeroCopyGetPtrHookOffset = 0;

uint32       CFE_ES_GetPoolBufHookCallCnt = 0;

uint8        CFE_SB_ZeroCopyGetPtrHook_Buf[CFE_SB_MAX_SB_MSG_SIZE];
uint8        *BufPtrs[128];

boolean      ZeroCopySendHook_AckEofNoErr_Rcvd = NO;
boolean      ZeroCopySendHook_AckEofCancel_Rcvd = NO;
boolean      ZeroCopySendHook_FinNoErr_Rcvd = NO;
boolean      ZeroCopySendHook_FinInactTimeout_Rcvd = NO;
boolean      ZeroCopySendHook_Nak_Rcvd = NO;
CFDP_DATA    InPDU;
CFDP_DATA    HookOutPDU;


void Get_PduHeader(uint8 id_len, uint8 trans_seq_len,
                   uint8 *pdu_ptr, HDR *pdu_hdr);
int  Gen_PduHeader(uint8 trans_seq_len, uint16 data_field_len,
                   HDR *pdu_hdr, CFDP_DATA *out_pdu);

void Build_AckFinPdu(uint8 trans_seq_len, HDR *pdu_hdr,
                     ACK *ack, CFDP_DATA *out_pdu);


/**************************************************************************
 * CF Custom Hook Functions
 **************************************************************************/

int32 CFE_ES_GetPoolBufHook(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr,
                            uint32 Size)
{
    uint32  Offset;
    uint8   *BytePtr;

    Offset = (CFE_ES_GetPoolBufHookCallCnt * sizeof (CF_QueueEntry_t));

    BytePtr = CF_AppData.Mem.Partition;

    BytePtr += Offset;

    *BufPtr = (uint32 *)BytePtr;

    CFE_ES_GetPoolBufHookCallCnt++;

    return Size;
}


int32 CFE_ES_PutPoolBufHook(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{
    /* Note the actual memory pool has not been deallocated:
       the CFE_ES_GetPoolBufHookCallCnt remains the same value,
       but the CF_AppData.Hk.App.MemInUse will be reduced
       as it is supposed to be in the actual environment */
    memset(BufPtr, 0x00, sizeof(CF_QueueEntry_t));

    return sizeof(CF_QueueEntry_t);
}


int32 CFE_SB_ZeroCopyGetPtrHook(uint16 MsgSize,
                                    CFE_SB_ZeroCopyHandle_t *BufferHandle)
{
    int32 ret_val;

    *BufferHandle = ZeroCopyGetPtrHookCallCnt;

    BufPtrs[ZeroCopyGetPtrHookCallCnt] =
                  &CFE_SB_ZeroCopyGetPtrHook_Buf[ZeroCopyGetPtrHookOffset];

    ret_val = (int32)&BufPtrs[ZeroCopyGetPtrHookCallCnt];

    ZeroCopyGetPtrHookOffset += MsgSize;
    ZeroCopyGetPtrHookCallCnt ++;

    return (ret_val);
}


void Get_PduHeader(uint8 id_len, uint8 trans_seq_len,
                   uint8 *pdu_ptr, HDR *pdu_hdr)
{
    int i;

    pdu_hdr->pdu_type = (pdu_ptr[0] & 0x10) ? FILE_DATA_PDU : FILE_DIR_PDU;
    pdu_hdr->direction = (pdu_ptr[0] & 0x08) ? TOWARD_SENDER : TOWARD_RECEIVER;
    pdu_hdr->mode = (pdu_ptr[0] & 0x04) ? UNACK_MODE : ACK_MODE;
    pdu_hdr->use_crc = NO;

    pdu_hdr->trans.source_id.length = id_len;
    pdu_hdr->trans.source_id.value[0] = pdu_ptr[4];
    pdu_hdr->trans.source_id.value[1] = pdu_ptr[5];

    pdu_hdr->trans.number = 0;
    for (i = 0; i < trans_seq_len; i++)
    {
        pdu_hdr->trans.number = (pdu_hdr->trans.number * 256) + pdu_ptr[6 + i];
    }

    pdu_hdr->dest_id.length = id_len;
    pdu_hdr->dest_id.value[0] = pdu_ptr[10];
    pdu_hdr->dest_id.value[1] = pdu_ptr[11];

    return;
}


int Gen_PduHeader(uint8 trans_seq_len, uint16 data_field_len,
                  HDR *pdu_hdr, CFDP_DATA *out_pdu)
{
    uint8   byte;
    uint8   byte0, byte1, byte2, byte3;
    uint8   upper_byte, lower_byte;
    uint8   trans_seq_num_len;
    int     i;
    int     index;
    uint32  trans_seq_num;
    ID      src_id;
    ID      dest_id;

    src_id = pdu_hdr->trans.source_id;
    dest_id = pdu_hdr->dest_id;
    trans_seq_num_len = trans_seq_len;
    trans_seq_num = pdu_hdr->trans.number;

    index = 0;

    /* Byte 0: pdu_type, direction, mode, crc */
    byte = 0;
    if (pdu_hdr->pdu_type == FILE_DATA_PDU)
    {
        byte = byte | 0x10;
    }
    if (pdu_hdr->direction == TOWARD_SENDER)
    {
        byte = byte | 0x80;
    }
    if (pdu_hdr->mode == UNACK_MODE)
    {
        byte = byte | 0x04;
    }
    if (pdu_hdr->use_crc)
    {
    }
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 1 - 2: Packet Data field length */
    upper_byte = data_field_len / 256;
    lower_byte = data_field_len % 256;
    out_pdu->content[index] = upper_byte;
    index ++;
    out_pdu->content[index] = lower_byte;
    index ++;

    /* Byte 3: EntityID length and TransSeqNumber length */
    byte = 0;
    byte = byte | ((src_id.length - 1) << 4);
    byte = byte | (trans_seq_num_len - 1);
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 4 - 5: Source Entity ID: Just in case, add zeros on the left */
    byte = 0;
    for (i = src_id.length; i < dest_id.length; i++)
    {
        out_pdu->content[index] = byte;
        index ++;
    }
    memcpy(&out_pdu->content[index], src_id.value, src_id.length);
    index += src_id.length;

    /* Byte 6 - 9: Trans Sequence Number */
    byte0 = (trans_seq_num & 0xff000000) >> 24;
    byte1 = (trans_seq_num & 0x00ff0000) >> 16;
    byte2 = (trans_seq_num & 0x0000ff00) >> 8;
    byte3 = trans_seq_num & 0x000000ff;
    out_pdu->content[index] = byte0;
    index ++;
    out_pdu->content[index] = byte1;
    index ++;
    out_pdu->content[index] = byte2;
    index ++;
    out_pdu->content[index] = byte3;
    index ++;

    /* Byte 10 - 11: Dest Entity ID: Just in case, add zeros on the left */
    byte = 0;
    for (i = dest_id.length; i < src_id.length; i++)
    {
        out_pdu->content[index] = byte;
        index ++;
    }
    memcpy(&out_pdu->content[index], dest_id.value, dest_id.length);
    index += dest_id.length;

    return index;
}


void Build_AckFinPdu(uint8 trans_seq_len, HDR *pdu_hdr,
                     ACK *ack, CFDP_DATA *out_pdu)
{
    uint8   byte;
    uint8   dir_code, dir_subtype_code;
    uint8   cond_code, deli_code, trans_stat;
    uint16  data_field_len;
    int     index;
    int     hdr_len;

    data_field_len = 3;

    hdr_len = Gen_PduHeader(trans_seq_len, data_field_len, pdu_hdr, out_pdu);
    index = hdr_len;
printf("!!Build_AckFinPdu: hdr_len(%d)\n", hdr_len);

    /**** Data Field ****/
    /* Byte 0: File directive code */
    byte = ACK_PDU;
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 1: Ack directive_code*/
    byte = 0;
    dir_code = ack->directive_code;
    dir_subtype_code = ack->directive_subtype_code;
    byte = byte | (dir_code << 4);
    byte = byte | dir_subtype_code;
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 2: Ack condition code/delivery_code/transaction_status */
    byte = 0;
    cond_code = ack->condition_code;
    deli_code = ack->delivery_code;
    trans_stat = ack->transaction_status;
    byte = byte | (cond_code << 4);
    byte = byte | (deli_code << 2);
    byte = byte | trans_stat;
    out_pdu->content[index] = byte;
    index ++;

    out_pdu->length = index;

printf("!!Build_AckFinPdu: Packet length(%lu)\n", out_pdu->length);
for(int i = 0; i < out_pdu->length; i++)
{
    printf("%02x ", out_pdu->content[i]);
}
printf("\n");

    return;
}


void Build_EofCancelPdu(uint8 trans_seq_len, HDR *pdu_hdr,
                        EOHF *eof, CFDP_DATA *out_pdu)
{
    uint8   byte;
    uint8   byte0, byte1, byte2, byte3;
    uint8   cond_code;
    uint16  data_field_len;
    int     index;
    int     hdr_len;
    uint32  Checksum;
    uint32  FileSize;

    data_field_len = 10;

    hdr_len = Gen_PduHeader(trans_seq_len, data_field_len, pdu_hdr, out_pdu);
    index = hdr_len;
printf("!!Build_EofCancelPdu: hdr_len(%d)\n", hdr_len);

    /**** Data Field ****/
    /* Byte 0: File Dir Code */
    byte = EOF_PDU;
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 1: Condition Code */
    cond_code = eof->condition_code;
    byte = cond_code << 4;
    out_pdu->content[index] = byte;
    index ++;

    /* Byte 2 - 5: Checksum: Not Supported */
    Checksum = eof->file_checksum;
    byte0 = (Checksum & 0xff000000) >> 24;
    byte1 = (Checksum & 0x00ff0000) >> 16;
    byte2 = (Checksum & 0x0000ff00) >> 8;
    byte3 = Checksum & 0x000000ff;
    out_pdu->content[index] = byte0;
    index ++;
    out_pdu->content[index] = byte1;
    index ++;
    out_pdu->content[index] = byte2;
    index ++;
    out_pdu->content[index] = byte3;
    index ++;

    /* Byte 6 - 9: File Size */
    FileSize = eof->file_size;
    byte0 = (FileSize & 0xff000000) >> 24;
    byte1 = (FileSize & 0x00ff0000) >> 16;
    byte2 = (FileSize & 0x0000ff00) >> 8;
    byte3 = FileSize & 0x000000ff;
    out_pdu->content[index] = byte0;
    index ++;
    out_pdu->content[index] = byte1;
    index ++;
    out_pdu->content[index] = byte2;
    index ++;
    out_pdu->content[index] = byte3;
    index ++;

    out_pdu->length = index;

printf("!!Build_EofCancelPdu: Packet length(%lu)\n", out_pdu->length);
for(int i = 0; i < out_pdu->length; i++)
{
    printf("%02x ", out_pdu->content[i]);
}
printf("\n");

    return;
}


int32 CFE_SB_ZeroCopySendHook(CFE_SB_Msg_t *MsgPtr,
                              CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    uint8               byte;
    uint8               IdLen, TransSeqNumLen, PduHdrLen;
    uint8               file_dir_code;
    uint8               *pduPtr;
    uint8               *pBuff;
    uint16              PDataLen;
    uint16              msgLen = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    int                 i;
    int                 index;
    HDR                 PduHdr;
    ACK                 Ack;
    FIN                 Fin;
    NAK                 Nak;
    EOHF                Eof;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    pBuff = (uint8 *)MsgPtr;
    printf("###CFE_SB_ZeroCopySendHook(msgLen %u): ", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("%02x ", pBuff[i]);
    }
    printf("\n");

    if (CF_GetPktType(CFE_SB_GetMsgId(MsgPtr))==CF_CMD)
    {
        pBuff += CFE_SB_CMD_HDR_SIZE;
    }
    else
    {
        pBuff += CFE_SB_TLM_HDR_SIZE;
    }

    /* Extract PDU Hdr */
    pduPtr = pBuff;

    PDataLen = (pduPtr[1] * 256) + pduPtr[2];

    IdLen = ((pduPtr[3] >> 4) & 0x07) + 1;
    TransSeqNumLen = (pduPtr[3] & 0x07) + 1;
    PduHdrLen = CF_PDUHDR_FIXED_FIELD_BYTES + (IdLen * 2) +
                  TransSeqNumLen;

    Get_PduHeader(IdLen, TransSeqNumLen, pduPtr, &PduHdr);

    InPDU.length = PDataLen + PduHdrLen;
    memcpy(InPDU.content, pduPtr, InPDU.length);

printf("!!!CFE_SB_ZeroCopySendHook: InPDU.length(%lu)\n", InPDU.length);
for (i = 0; i < InPDU.length; i++)
{
    printf("%02x ", InPDU.content[i]);
}
printf("\n");

    switch(MsgId)
    {
        case CF_CPD_TO_PPD_PDU_MID:
            printf("Received CF_CPD_TO_PPD_PDU_MID\n");

            if (PduHdr.pdu_type == FILE_DIR_PDU)
            {
                index = PduHdrLen;
                file_dir_code = InPDU.content[index];
                index ++;

                if (file_dir_code == ACK_PDU)
                {
                    Ack.directive_code = (InPDU.content[index] & 0xf0) >> 4;
                    index ++;

                    Ack.condition_code = (InPDU.content[index] & 0xf0) >> 4;
                    Ack.delivery_code = (InPDU.content[index] & 0x0c) >> 2;
                    Ack.transaction_status = InPDU.content[index] & 0x03;
                    index ++;

                    if (Ack.directive_code == EOF_PDU)
                    {
                        if (Ack.condition_code == NO_ERROR)
                        {
                            ZeroCopySendHook_AckEofNoErr_Rcvd = YES;
                        }
                        else if(Ack.condition_code == FILE_SIZE_ERROR)
                        {
                            ZeroCopySendHook_AckEofCancel_Rcvd = YES;
                        }
                        printf("Received ACK-EOF\n");
                        printf("directive_code: %u\n", Ack.directive_code);
                        printf("condition_code: %u\n", Ack.condition_code);
                        printf("delivery_code: %u\n", Ack.delivery_code);
                        printf("trans_stat: %u\n", Ack.transaction_status);
                    }
                    else
                    {
                        printf("Received ACK-Other\n");
                    }
                }
                else if (file_dir_code == FIN_PDU)
                {
                    Fin.condition_code = (InPDU.content[index] & 0xf0) >> 4;
                    Fin.end_system_status = (InPDU.content[index] & 0x08) >> 3;
                    Fin.delivery_code = (InPDU.content[index] & 0x04) >> 2;
                    index ++;

                    printf("Received FIN\n");
                    printf("condition_code(%u)\n", Fin.condition_code);
                    printf("end_system_status(%u)\n", Fin.end_system_status);
                    printf("delivery_code(%u)\n", Fin.delivery_code);

                    if (Fin.condition_code == NO_ERROR &&
                        Fin.delivery_code == DATA_COMPLETE)
                    {
                        ZeroCopySendHook_FinNoErr_Rcvd = YES;

                        Ack.directive_code = FIN_PDU;
                        Ack.directive_subtype_code = 0;
                        Ack.condition_code = Fin.condition_code;
                        Ack.delivery_code = Fin.delivery_code;
                        Ack.transaction_status = TRANS_UNDEFINED;
printf("Sending Ack-Fin: Ack.directive_code(%u)\n", Ack.directive_code);
printf("Sending Ack-Fin: Ack.directive_subtype_code(%u)\n", Ack.directive_subtype_code);
printf("Sending Ack-Fin: Ack.condition_code(%u)\n", Ack.condition_code);
printf("Sending Ack-Fin: Ack.delivery_code(%u)\n", Ack.delivery_code);
printf("Sending Ack-Fin: Ack.transaction_status(%u)\n", Ack.transaction_status);

                        memset(&HookOutPDU, 0x00, sizeof(HookOutPDU));
                        PduHdr.direction = TOWARD_RECEIVER;
                        Build_AckFinPdu(TransSeqNumLen, &PduHdr, &Ack, &HookOutPDU);
                        printf("##CFE_SB_ZeroCopySendHook: Sending output PDU\n");
                    }
                    else if (Fin.condition_code == INACTIVITY_DETECTED)
                    {
                        ZeroCopySendHook_FinInactTimeout_Rcvd = YES;

                        Ack.directive_code = FIN_PDU;
                        Ack.directive_subtype_code = 0;
                        Ack.condition_code = Fin.condition_code;
                        Ack.delivery_code = DONT_CARE_0;
                        /* Solely for ack fin */
                        Ack.transaction_status = TRANS_UNDEFINED;
printf("Sending Ack-Fin Inact: Ack.directive_code(%u)\n", Ack.directive_code);
printf("Sending Ack-Fin Inact: Ack.directive_subtype_code(%u)\n", Ack.directive_subtype_code);
printf("Sending Ack-Fin Inact: Ack.condition_code(%u)\n", Ack.condition_code);
printf("Sending Ack-Fin Inact: Ack.delivery_code(%u)\n", Ack.delivery_code);
printf("Sending Ack-Fin Inact: Ack.transaction_status(%u)\n", Ack.transaction_status);

                        memset(&HookOutPDU, 0x00, sizeof(HookOutPDU));
                        PduHdr.direction = TOWARD_RECEIVER;
                        Build_AckFinPdu(TransSeqNumLen, &PduHdr, &Ack, &HookOutPDU);
                        printf("##CFE_SB_ZeroCopySendHook: Sending output PDU\n");
                    }
                    else
                    {
                    }
                }
                else if (file_dir_code == NAK_PDU)
                {
                    Nak.start_of_scope = 0;
                    for (i = 0; i < sizeof(u_int_4); i++)
                    {
                        Nak.start_of_scope = (Nak.start_of_scope * 256) +
                                             InPDU.content[index + i];
                    }
                    index += sizeof(u_int_4);

                    Nak.end_of_scope = 0;
                    for (i = 0; i < sizeof(u_int_4); i++)
                    {
                        Nak.end_of_scope = (Nak.end_of_scope * 256) +
                                           InPDU.content[index + i];
                    }
                    index += sizeof(u_int_4);

                    byte = InPDU.content[index];
                    Nak.is_metadata_missing = (byte & 0x80) >> 7 ;
                    index ++;

                    printf("Received NAK\n");
                    printf("Nak.start_of_scope(%lu)\n", Nak.start_of_scope);
                    printf("Nak.end_of_scope(%lu)\n", Nak.end_of_scope);
                    printf("Nak.is_metadata_missing(%u)\n",
                            Nak.is_metadata_missing);

                    ZeroCopySendHook_Nak_Rcvd = YES;

                    Eof.condition_code = FILE_SIZE_ERROR;
                    Eof.file_checksum = 0;
                    Eof.file_size = TEST_FILE_SIZE2;

                    memset(&HookOutPDU, 0x00, sizeof(HookOutPDU));
                    PduHdr.direction = TOWARD_RECEIVER;
                    Build_EofCancelPdu(TransSeqNumLen, &PduHdr, &Eof, &HookOutPDU);
                    printf("##CFE_SB_ZeroCopySendHook: Sending output PDU\n");
                }
                else
                {
                    printf("!!!CFE_SB_ZeroCopySendHook: Received unknown "
                           "file_dir_code(%u)\n", file_dir_code);
                }
            }
            else
            {
                printf("!!!CFE_SB_ZeroCopySendHook: Received FILE_DATA_PDU\n");
            }

            break;
        case CF_CPD_TO_GND_PDU_MID:
            printf("Received CF_CPD_TO_GND_PDU_MID\n");
            break;
        default:
            printf("Received MID(0x%04X)\n", MsgId);
            break;
    }

    return CFE_SUCCESS;
}


int32 CFE_FS_WriteHeaderHook(int32 FileDes, CFE_FS_Header_t *Hdr)
{
    size_t bytesWritten = 0;

    if ((FileDes != 0) && (FileDes != TEST_FD_ZERO_REPLACEMENT))
    {
        bytesWritten = fwrite((void *)Hdr, 1, sizeof(CFE_FS_Header_t),
                              (FILE *)FileDes);
    }

printf("!!!CFE_FS_WriteHeaderHook: bytesWritten is %d\n", (int)bytesWritten);
    return ((int32)bytesWritten);
}


int32 OS_creatHook(const char *path, int32 access)
{
    FILE *fp = 0;

    fp = fopen(path, "wb+");

printf("!!!!OS_creatHook fp(%x)\n", (unsigned int)fp);
    return ((int32)fp);
}


int32 OS_openHook(const char *path, int32 access, uint32 mode)
{
    FILE *fp = 0;

    if (access == OS_READ_ONLY)
    {
        fp = fopen(path, "rb");
    }
    else if (access == OS_READ_WRITE)
    {
        fp = fopen(path, "rb+");
    }
    else
    {
printf("!!!OP_openHook else access type(%ld)\n", access);
    }

printf("!!!!OS_openHook fp(%x)\n", (unsigned int)fp);
    return ((int32)fp);
}


int32 OS_readHook(int32  filedes, void *buffer, uint32 nbytes)
{
    int32 readbytes = 0;

    if ((filedes != 0) && (filedes != TEST_FD_ZERO_REPLACEMENT))
    {
        readbytes = fread(buffer, 1, nbytes, (FILE *)filedes);
    }

printf("##OS_readHook: readbytes(%ld)\n", readbytes);
    return readbytes;
}


int32 OS_writeHook(int32 filedes, const void *buffer, uint32 nbytes)
{
    size_t bytesWritten = 0;

    if ((filedes != 0) && (filedes != TEST_FD_ZERO_REPLACEMENT))
    {
        bytesWritten = fwrite((void *)buffer, 1, nbytes, (FILE *)filedes);
    }

printf("!!!OS_writeHook: bytesWritten is %d\n", (int)bytesWritten);
    return ((int32)bytesWritten);
}


int32 OS_closeHook(int32 filedes)
{
    int ret = EOF;    // -1

    if ((filedes != 0) && (filedes != TEST_FD_ZERO_REPLACEMENT))
    {
        ret = fclose((FILE *)filedes);
    }

printf("!!!OS_closeHook: ret is %d\n", ret);
    return ((int32)ret);
}


int32 OS_statHook(const char *path, os_fstat_t *filestats)
{
    filestats->st_size = TEST_FILE_SIZE1;

    return OS_FS_SUCCESS;
}


int32 OS_FDGetInfoHook (int32 filedes, OS_FDTableEntry *fd_prop)
{
    strcpy(fd_prop->Path, TestPbDir0);
    strcat(fd_prop->Path, TestPbFile1);

    return OS_FS_SUCCESS;
}


os_dirp_t OS_opendirHook(const char *path)
{
    strcpy(OpendirHookDir.DirEnt.d_name, path);
printf("!!!OS_opendirHook entered(d_name: %s\n", OpendirHookDir.DirEnt.d_name);
    OpendirHookDir.DirEnt.d_type = OS_DT_DIR;

    return (&OpendirHookDir);
}


os_dirent_t *OS_readdirHook (os_dirp_t directory)
{
    if (strcmp(directory->DirEnt.d_name, TestPbDir0) == 0)
    {
        if(ReaddirHookReturnCnt == 0)
        {
            strcpy(ReaddirHookDirEntry.d_name, ".");
            ReaddirHookDirEntry.d_type = OS_DT_DIR;
        }
        else if(ReaddirHookReturnCnt == 1)
        {
            strcpy(ReaddirHookDirEntry.d_name, "..");
            ReaddirHookDirEntry.d_type = OS_DT_DIR;
        }
        else if(ReaddirHookReturnCnt == 2)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile3);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if(ReaddirHookReturnCnt == 3)
        {
            strcpy(ReaddirHookDirEntry.d_name, "ThisFilenameIsTooLongItExceeds"
                   "64ThisFilenameIsTooLongItIs65charas");
            ReaddirHookDirEntry.d_type = 5;
        }
        else if(ReaddirHookReturnCnt == 4)
        {
            strcpy(ReaddirHookDirEntry.d_name, "ThisFilenameIsTooLongWhenThe"
                   "PathIsAttachedToIt.ItIs63Characters");
            ReaddirHookDirEntry.d_type = 5;
        }
        else if(ReaddirHookReturnCnt == 5)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile2);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if(ReaddirHookReturnCnt == 6)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile1);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else
        {
            return NULL;
        }
    }
    else if (strcmp(directory->DirEnt.d_name, TestPbDir1) == 0)
    {
        return NULL;
    }
    else if (strcmp(directory->DirEnt.d_name, TestPbDir2) == 0)
    {
        if (ReaddirHookReturnCnt == 7)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile4);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if (ReaddirHookReturnCnt == 8)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile5);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else if (ReaddirHookReturnCnt == 9)
        {
            strcpy(ReaddirHookDirEntry.d_name, TestPbFile6);
            ReaddirHookDirEntry.d_type = OS_DT_FILE;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    ReaddirHookReturnCnt++;

    return(&ReaddirHookDirEntry);
}


int32 OS_CountSemGetIdByNameHook(uint32 *sem_id, const char *sem_name)
{
    *sem_id = SemGetIdByNameHookCallCnt;
    SemGetIdByNameHookCallCnt ++;

    return OS_SUCCESS;
}


int32 OS_CountSemGetInfoHook(uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    count_prop->creator = 1;
    count_prop->value = 10 - SemGetInfoHookCallCnt;

    SemGetInfoHookCallCnt ++;

    return OS_SUCCESS;
}


void CFE_PSP_GetTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void)
{
    int                 iStatus;
    CFE_TIME_SysTime_t  CfeTime;
    struct timespec     time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        CfeTime.Seconds = time.tv_sec;
        CfeTime.Subseconds = time.tv_nsec / 1000;
    }

    return CfeTime;
}


void CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_SB_SetMsgTime(MsgPtr, CFE_TIME_GetTime());

    return;
}
