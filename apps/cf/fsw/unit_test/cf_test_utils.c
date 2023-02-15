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

#include "cf_test_utils.h"
#include "cf_custom_hooks.h"

#include "structures.h"
#include "timer.h"
#include "machine.h"
#include "misc.h"
#include "cf_playback.h"
#include "cf_callbacks.h"

#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"

#include <string.h>


const char TestPbFile1[] = "pbfile1.dat";
const char TestPbFile2[] = "pbfile2.dat";
const char TestPbFile3[] = "pbfile3.dat";
const char TestPbFile4[] = "pbfile4.dat";
const char TestPbFile5[] = "pbfile5.dat";
const char TestPbFile6[] = "pbfile6.dat";

const char TestInFile1[] = "infile1.dat";
const char TestInFile2[] = "infile2.dat";
const char TestInNoFile[] = "";

const char TestQInfoFile1[] = "qinfo1.dat";

const char TestPbDir0[] = "/dn/cpd/cl1/pr2/";
const char TestPbDir1[] = "/dn/cpd/cl2/pr2/";
const char TestPbDir2[] = "/dn/cpd/cl1/pr4/";
const char TestPbDir3[] = "/dn/cpd/cl2/pr4/";

const char TestDstDir0[] = "/cpd/cl1/pr2/";
const char TestDstDir1[] = "/cpd/cl2/pr2/";
const char TestDstDir2[] = "/cpd/cl1/pr4/";
const char TestDstDir3[] = "/cpd/cl2/pr4/";

const char TestInDir0[] = "/up/ppd/cl1/";
const char TestInDir1[] = "/up/ppd/cl2/";
const char TestInDir2[] = "/up/gnd/cl1/";
const char TestInDir3[] = "/up/gnd/cl2/";

const char TestQInfoDir[] = "/qinfo/";

const char TestPbPeerEntityId[] = "2.25";
const char TestInSrcEntityId1[] = "0.2";
const char TestInSrcEntityId2[] = "0.1";


void CF_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemSet(&CF_AppData, 0x00, sizeof(CF_AppData));

    CFE_ES_GetPoolBufHookCallCnt = 0;

    SemGetInfoHookCallCnt = 0;
    SemGetIdByNameHookCallCnt = 0;

    ZeroCopyGetPtrHookCallCnt = 0;
    ZeroCopyGetPtrHookOffset = 0;

    ReaddirHookReturnCnt = 0;
    memset((void*)&ReaddirHookDirEntry, 0x00, sizeof(ReaddirHookDirEntry));

    ReadHookCalledCnt = 0;
    ReadHook_IncomingFlag = FALSE;

    cfdp_reset_totals();
    misc__thaw_all_partners();
    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_TIME_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTable);
}


void CF_Test_SetupUnitTest(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemSet(&CF_AppData, 0x00, sizeof(CF_AppData));

    CFE_ES_GetPoolBufHookCallCnt = 0;

    SemGetInfoHookCallCnt = 0;
    SemGetIdByNameHookCallCnt = 0;

    ZeroCopyGetPtrHookCallCnt = 0;
    ZeroCopyGetPtrHookOffset = 0;

    ReaddirHookReturnCnt = 0;
    memset((void*)&ReaddirHookDirEntry, 0x00, sizeof(ReaddirHookDirEntry));

    ReadHookCalledCnt = 0;
    ReadHook_IncomingFlag = FALSE;

    cfdp_reset_totals();
    misc__thaw_all_partners();
    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_TIME_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTableUnitTest);
}


void CF_Test_TearDown(void)
{
}


void CF_ShowQs()
{
    uint32              i;
    CF_QueueEntry_t     *PtrToEntry;

    OS_printf("\nUplink Active Queue files:\n");
    PtrToEntry = CF_AppData.UpQ[CF_UP_ACTIVEQ].HeadPtr;
    while(PtrToEntry != NULL)
    {
        OS_printf("%s_%d %s\n",PtrToEntry->SrcEntityId,
                        (int)PtrToEntry->TransNum, PtrToEntry->SrcFile);
        PtrToEntry = PtrToEntry->Next;
    }
    OS_printf("Uplink Active Queue - File Count = %d\n\n",
                    (int)CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt);

    OS_printf("Uplink History Queue files:\n");
    PtrToEntry = CF_AppData.UpQ[CF_UP_HISTORYQ].HeadPtr;
    while(PtrToEntry != NULL)
    {
        OS_printf("%s_%d %s\n",PtrToEntry->SrcEntityId,
                  (int)PtrToEntry->TransNum, PtrToEntry->SrcFile);
        PtrToEntry = PtrToEntry->Next;
    }
    OS_printf("Uplink History Queue - File Count = %d\n\n",
              (int)CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt);

    for(i = 0; i < CF_MAX_PLAYBACK_CHANNELS; i++)
    {
        if(CF_AppData.Tbl->OuCh[i].EntryInUse == CF_ENTRY_IN_USE)
        {
            OS_printf("Playback Pending Queue %d files:\n", (int)i);
            PtrToEntry = CF_AppData.Chan[i].PbQ[CF_PB_PENDINGQ].HeadPtr;
            while(PtrToEntry != NULL)
            {
                OS_printf("%s_%d %s %d\n",PtrToEntry->SrcEntityId,
                          (int)PtrToEntry->TransNum, PtrToEntry->SrcFile,
                          PtrToEntry->Priority);
                PtrToEntry = PtrToEntry->Next;
            }

            OS_printf("Playback Pending Queue %d File Count = %d\n\n", (int)i,
                       (int)CF_AppData.Chan[i].PbQ[CF_PB_PENDINGQ].EntryCnt);

            OS_printf("Playback Active Queue %d files:\n", (int)i);
            PtrToEntry = CF_AppData.Chan[i].PbQ[CF_PB_ACTIVEQ].HeadPtr;
            while(PtrToEntry != NULL)
            {
                OS_printf("%s_%d %s %d\n",PtrToEntry->SrcEntityId,
                          (int)PtrToEntry->TransNum, PtrToEntry->SrcFile,
                          PtrToEntry->Priority);
                PtrToEntry = PtrToEntry->Next;
            }

            OS_printf("Playback Active Queue %d File Count = %d\n\n", (int)i,
                      (int)CF_AppData.Chan[i].PbQ[CF_PB_ACTIVEQ].EntryCnt);

            OS_printf("Playback History Queue %d files:\n", (int)i);
            PtrToEntry = CF_AppData.Chan[i].PbQ[CF_PB_HISTORYQ].HeadPtr;
            while(PtrToEntry != NULL)
            {
                OS_printf("%s_%d %s %d\n",PtrToEntry->SrcEntityId,
                          (int)PtrToEntry->TransNum, PtrToEntry->SrcFile,
                          PtrToEntry->Priority);
                PtrToEntry = PtrToEntry->Next;
            }

            OS_printf("Playback History Queue %d File Count = %d\n\n", (int)i,
                      (int)CF_AppData.Chan[i].PbQ[CF_PB_HISTORYQ].EntryCnt);
        }
    }
}


void CF_TstUtil_InitApp()
{
    CF_AppInit();

    CF_ResetEngine();
}


void CF_ResetEngine(void)
{
    int                 i;
    CF_CARSCmd_t        AbandonCmdMsg;
    CF_PurgeQueueCmd_t  PurgeUpCmdMsg;
    CF_PurgeQueueCmd_t  PurgePbPendCmdMsg;
    CF_PurgeQueueCmd_t  PurgePbHistCmdMsg;

    CF_TstUtil_FinishPbActiveQueueEntries();

    /* Abandon Up/Down Active Queue Entries */
    CFE_SB_InitMsg((void*)&AbandonCmdMsg, CF_CMD_MID,
                   sizeof(AbandonCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AbandonCmdMsg, CF_ABANDON_CC);
    strcpy(AbandonCmdMsg.Trans, "All");

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AbandonCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Purge Up History Queue Entries */
    CFE_SB_InitMsg((void*)&PurgeUpCmdMsg, CF_CMD_MID,
                   sizeof(PurgeUpCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeUpCmdMsg, CF_PURGE_QUEUE_CC);
    PurgeUpCmdMsg.Type = CF_INCOMING;
    PurgeUpCmdMsg.Chan = 0;
    PurgeUpCmdMsg.Queue = CF_HISTORYQ;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeUpCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    for (i = 0; i < CF_MAX_PLAYBACK_CHANNELS; i ++)
    {
        /* Purge Pb Pending Queue Entries */
        CFE_SB_InitMsg((void*)&PurgePbPendCmdMsg, CF_CMD_MID,
                       sizeof(PurgePbPendCmdMsg), TRUE);
        CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgePbPendCmdMsg,
                          CF_PURGE_QUEUE_CC);
        PurgePbPendCmdMsg.Type = CF_OUTGOING;
        PurgePbPendCmdMsg.Chan = i;
        PurgePbPendCmdMsg.Queue = CF_PB_PENDINGQ;

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgePbPendCmdMsg;
        CF_AppPipe(CF_AppData.MsgPtr);

        /* Purge Pb History Queue Entries */
        CFE_SB_InitMsg((void*)&PurgePbHistCmdMsg, CF_CMD_MID,
                       sizeof(PurgePbHistCmdMsg), TRUE);
        CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgePbHistCmdMsg,
                          CF_PURGE_QUEUE_CC);
        PurgePbHistCmdMsg.Type = CF_OUTGOING;
        PurgePbHistCmdMsg.Chan = i;
        PurgePbHistCmdMsg.Queue = CF_PB_HISTORYQ;

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgePbHistCmdMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    misc__set_trans_seq_num(1);
}


int32 CF_TstUtil_VerifyListOrder(char *OrderGiven)
{
    CF_QueueEntry_t   *PtrToEntry;
    char              Buf[64];
    uint32            i=0;

    PtrToEntry = CF_AppData.Chan[0].PbQ[CF_PB_PENDINGQ].HeadPtr;

    while(PtrToEntry != NULL)
    {
        sprintf(&Buf[i],"%d",(int)PtrToEntry->TransNum);
        PtrToEntry = PtrToEntry->Next;
        i++;
    }

    if(strncmp(OrderGiven,Buf,64)==0)
    {
        return CF_SUCCESS;
    }
    else
    {
        printf("VerfiyList is comparing given %s with %s\n",
               OrderGiven,Buf);

        return CF_ERROR;
    }
}


void CF_TstUtil_CreateOnePbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    /* reset the transactions seq number/summary statistics
       used by the engine */
    cfdp_reset_totals();
    cfdp_set_trans_seq_num(1);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Send a Pb file command to add to the Pb Pending Q */
    CFE_SB_InitMsg((void*)pCmd, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd, CF_PLAYBACK_FILE_CC);
    pCmd->Class = CF_CLASS_1;
    pCmd->Channel = 0;
    pCmd->Priority = 2;
    pCmd->Preserve = CF_AppData.Tbl->OuCh[pCmd->Channel].PollDir[0].Preserve;
    strcpy(pCmd->PeerEntityId,
           CF_AppData.Tbl->OuCh[pCmd->Channel].PollDir[0].PeerEntityId);
    strcpy(pCmd->SrcFilename,
           CF_AppData.Tbl->OuCh[pCmd->Channel].PollDir[0].SrcPath);
    strcat(pCmd->SrcFilename, TestPbFile1);
    strcpy(pCmd->DstFilename,
           CF_AppData.Tbl->OuCh[pCmd->Channel].PollDir[0].DstPath);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_CreateTwoPbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                             CF_PlaybackFileCmd_t *pCmd2)
{
    /* reset the transactions seq number/summary statistics
       used by the engine */
    cfdp_reset_totals();
    cfdp_set_trans_seq_num(1);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Send Pb file #1 to add to the Pb Pending Q */
    CFE_SB_InitMsg((void*)pCmd1, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd1, CF_PLAYBACK_FILE_CC);
    pCmd1->Class = CF_CLASS_1;
    pCmd1->Channel = 0;
    pCmd1->Priority = 2;
    pCmd1->Preserve = CF_AppData.Tbl->OuCh[pCmd1->Channel].PollDir[0].Preserve;
    strcpy(pCmd1->PeerEntityId,
           CF_AppData.Tbl->OuCh[pCmd1->Channel].PollDir[0].PeerEntityId);
    strcpy(pCmd1->SrcFilename,
           CF_AppData.Tbl->OuCh[pCmd1->Channel].PollDir[0].SrcPath);
    strcat(pCmd1->SrcFilename, TestPbFile1);
    strcpy(pCmd1->DstFilename,
           CF_AppData.Tbl->OuCh[pCmd1->Channel].PollDir[0].DstPath);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send Pb file #2 to add to the Pb Pending Q */
    CFE_SB_InitMsg((void*)pCmd2, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd2, CF_PLAYBACK_FILE_CC);
    pCmd2->Class = CF_CLASS_1;
    pCmd2->Channel = 0;
    pCmd2->Priority = 2;
    pCmd2->Preserve = CF_AppData.Tbl->OuCh[pCmd2->Channel].PollDir[0].Preserve;
    strcpy(pCmd2->PeerEntityId,
           CF_AppData.Tbl->OuCh[pCmd2->Channel].PollDir[0].PeerEntityId);
    strcpy(pCmd2->SrcFilename,
           CF_AppData.Tbl->OuCh[pCmd2->Channel].PollDir[0].SrcPath);
    strcat(pCmd2->SrcFilename, TestPbFile2);
    strcpy(pCmd2->DstFilename,
           CF_AppData.Tbl->OuCh[pCmd2->Channel].PollDir[0].DstPath);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_CreateOnePbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    CF_TstUtil_CreateOnePbPendingQueueEntry(pCmd);

    CF_StartNextFile(0);
}


void CF_TstUtil_CreateTwoPbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                            CF_PlaybackFileCmd_t *pCmd2)
{
    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    CF_TstUtil_CreateTwoPbPendingQueueEntry(pCmd1, pCmd2);

    CF_StartNextFile(0);

    CF_StartNextFile(0);
}


void CF_TstUtil_FinishPbActiveQueueEntries()
{
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                             (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();
}


void CF_TstUtil_CreateOnePbHistoryQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    CF_CARSCmd_t    CARSCmdMsg;

    CF_TstUtil_CreateOnePbActiveQueueEntry(pCmd);

    cfdp_cycle_each_transaction();

    /* Send Abandon Cmd */
    CFE_SB_InitMsg((void*)&CARSCmdMsg, CF_CMD_MID, sizeof(CF_CARSCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CARSCmdMsg, CF_ABANDON_CC);
    strcpy(CARSCmdMsg.Trans, "All");

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CARSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();
}


uint32 CF_TstUtil_GenPDUHeader(CF_Test_InPDUMsg_t *pCmd,
                               CF_Test_InPDUInfo_t *pInfo, uint16 PDataLen)
{
    uint8    byte;
    uint8    upper_byte, lower_byte;
    uint8    byte0, byte1, byte2, byte3;
    int      i;
    uint32   index;
    uint32   TransSeqNum;
    ID       SrcEntityID;
    ID       DstEntityID;

    index = 0;

    /* Byte 0: pdu_type, direction, mode, crc */
    byte = 0;
    if (pInfo->pdu_type == FILE_DATA_PDU)
    {
        byte = byte | 0x10;
    }
    if (pInfo->direction == TEST_TO_SENDER)
    {
        byte = byte | 0x80;
    }
    if (pInfo->mode == TEST_UNACK_MODE)
    {
        byte = byte | 0x04;
    }
    if (pInfo->use_crc)
    {
        /* <NOT_SUPPORTED> CRC in PDUs */
    }
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Byte 1 - 2: Packet Data field length */
    upper_byte = PDataLen / 256;
    lower_byte = PDataLen % 256;
    memcpy(&pCmd->PduContent.Content[index++], &upper_byte, 1);
    memcpy(&pCmd->PduContent.Content[index++], &lower_byte, 1);

    /* Byte 3: EntityID length and TransID length */
    byte = 0;
    byte = byte | ((pInfo->trans.source_id.length - 1) << 4);
    byte = byte | (HARD_CODED_TRANS_SEQ_NUM_LENGTH - 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Byte 4 - 5: Source Entity ID: Just in case, add zeros on the left */
    memcpy(&SrcEntityID, &pInfo->trans.source_id, sizeof(SrcEntityID));
    memcpy(&DstEntityID, &pInfo->dest_id, sizeof(DstEntityID));
    byte = 0;
    for (i = SrcEntityID.length; i < DstEntityID.length; i++)
    {
        memcpy(&pCmd->PduContent.Content[index++], &byte, 1);
    }
    memcpy(&pCmd->PduContent.Content[index], SrcEntityID.value,
           SrcEntityID.length);
    index += SrcEntityID.length;

    /* Byte 6 - 9: Trans Sequence Number */
    TransSeqNum = pInfo->trans.number;
    byte0 = (TransSeqNum & 0xff000000) >> 24;
    byte1 = (TransSeqNum & 0x00ff0000) >> 16;
    byte2 = (TransSeqNum & 0x0000ff00) >> 8;
    byte3 = TransSeqNum & 0x000000ff;
    memcpy(&pCmd->PduContent.Content[index++], &byte0, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte1, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte2, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte3, 1);

    /* Byte 10 - 11: Dest Entity ID: Just in case, add zeros on the left */
    byte = 0;
    for (i = DstEntityID.length; i < SrcEntityID.length; i++)
    {
        memcpy(&pCmd->PduContent.Content[index++], &byte, 1);
    }
    memcpy(&pCmd->PduContent.Content[index], DstEntityID.value,
           DstEntityID.length);
    index += DstEntityID.length;

    return index;
}


void CF_TstUtil_BuildMDPdu(CF_Test_InPDUMsg_t *pCmd,
                           CF_Test_InPDUInfo_t *pInfo)
{
    uint8    byte;
    uint8    str_len;
    uint8    byte0, byte1, byte2, byte3;
    uint16   PDataLen;
    uint32   FileSize;
    uint32   header_len;
    uint32   index;

    PDataLen = 6;

    /* "No file transfer" is not considered */
    PDataLen += strlen(pInfo->src_filename) + 1;
    PDataLen += strlen(pInfo->dst_filename) + 1;

    header_len = CF_TstUtil_GenPDUHeader(pCmd, pInfo, PDataLen);
    index = header_len;

    /**** Data Field ****/
    /* Byte 0: File-dir code */
    byte = MD_PDU;
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Byte 1: Segmentation control: not supported */
    byte = 0;
    byte = byte | 0x80;
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Bytes 2 - 5 : file size */
    FileSize = pInfo->file_size;
    byte0 = (FileSize & 0xff000000) >> 24;
    byte1 = (FileSize & 0x00ff0000) >> 16;
    byte2 = (FileSize & 0x0000ff00) >> 8;
    byte3 = FileSize & 0x000000ff;
    memcpy(&pCmd->PduContent.Content[index++], &byte0, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte1, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte2, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte3, 1);

    /* Source file name */
    str_len = strlen(pInfo->src_filename);
    memcpy(&pCmd->PduContent.Content[index++], &str_len, 1);
    memcpy(&pCmd->PduContent.Content[index], pInfo->src_filename, str_len);
    index += str_len;

    /* Destination file name */
    str_len = strlen(pInfo->dst_filename);
    memcpy(&pCmd->PduContent.Content[index++], &str_len, 1);
    memcpy(&pCmd->PduContent.Content[index], pInfo->dst_filename, str_len);
    index += str_len;

    return;
}


void CF_TstUtil_BuildFDPdu(CF_Test_InPDUMsg_t *pCmd,
                           CF_Test_InPDUInfo_t *pInfo)
{
    uint8   byte0, byte1, byte2, byte3;
    uint16  PDataLen;
    uint32  index;
    uint32  Offset;
    uint32  FileSize;
    uint32  header_len;

    FileSize = pInfo->file_size;
    PDataLen = FileSize + 4;

    header_len = CF_TstUtil_GenPDUHeader(pCmd, pInfo, PDataLen);
    index = header_len;

    /**** Data Field ****/
    /* Byte 0 - 3 : Offset */
    Offset = pInfo->offset;
    byte0 = (Offset & 0xff000000) >> 24;
    byte1 = (Offset & 0x00ff0000) >> 16;
    byte2 = (Offset & 0x0000ff00) >> 8;
    byte3 = Offset & 0x000000ff;
    memcpy(&pCmd->PduContent.Content[index++], &byte0, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte1, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte2, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte3, 1);

    /* Byte 4 - Byte (FileSize + 4 - 1) : File buffer */
    memset((void *)&pCmd->PduContent.Content[index], 0xff, FileSize);
    index += FileSize;

    return;
}


void CF_TstUtil_BuildEOFPdu(CF_Test_InPDUMsg_t *pCmd,
                            CF_Test_InPDUInfo_t *pInfo)
{
    uint8   byte;
    uint8   byte0, byte1, byte2, byte3;
    uint8   cond_code;
    uint16  PDataLen;
    uint32  header_len;
    uint32  index;
    uint32  Checksum;
    uint32  FileSize;

    PDataLen = 10;

    header_len = CF_TstUtil_GenPDUHeader(pCmd, pInfo, PDataLen);
    index = header_len;

    /**** Data Field ****/
    /* Byte 0: File Dir Code */
    byte = EOF_PDU;
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Byte 1: Condition Code */
    cond_code = NO_ERROR;
    byte = cond_code << 4;
    memcpy(&pCmd->PduContent.Content[index++], &byte, 1);

    /* Byte 2 - 5: Checksum: Not Supported */
    Checksum = 0;
    byte0 = (Checksum & 0xff000000) >> 24;
    byte1 = (Checksum & 0x00ff0000) >> 16;
    byte2 = (Checksum & 0x0000ff00) >> 8;
    byte3 = Checksum & 0x000000ff;
    memcpy(&pCmd->PduContent.Content[index++], &byte0, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte1, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte2, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte3, 1);

    /* Byte 6 - 9: File Size */
    FileSize = pInfo->file_size;
    byte0 = (FileSize & 0xff000000) >> 24;
    byte1 = (FileSize & 0x00ff0000) >> 16;
    byte2 = (FileSize & 0x0000ff00) >> 8;
    byte3 = FileSize & 0x000000ff;
    memcpy(&pCmd->PduContent.Content[index++], &byte0, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte1, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte2, 1);
    memcpy(&pCmd->PduContent.Content[index++], &byte3, 1);

    return;
}


void CF_Test_PrintCmdMsg(void *pMsg, uint32 size)
{
    unsigned char *pBuff;
    int           i = 0;

    pBuff = (unsigned char*)pMsg;
    printf("Emulated Cmd message:");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    return;
}


time_t CF_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
