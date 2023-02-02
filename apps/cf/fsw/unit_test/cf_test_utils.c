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

const char TestInFile1[] = "infile1.dat";
const char TestInFile2[] = "infile2.dat";
const char TestInNoFile[] = "";

const char TestQInfoFile1[] = "qinfofile1.dat";

const char TestPbDir[] = "/ram/downlink/cpd_to_gnd/class1/priority2/";
const char TestDstDir[] = "gndpath/";
const char TestInDir[] = "inpath/";
const char TestQInfoDir[] = "qinfopath/";

const char TestPbPeerEntityId[] = "2.25";
const char TestInSrcEntityId1[] = "0.23";
const char TestInSrcEntityId2[] = "0.50";


void CF_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemSet(&CF_AppData, 0x00, sizeof(CF_AppData));

    CFE_ES_GetPoolBufHookCallCnt = 0;

    SemGetInfoHookCallCnt = 0;
    SemGetIdByNameHookCallCnt = 0;

    ZeroCopyGetPtrHookCallCnt = 0;
    ZeroCopyGetPtrHookOffset = 0;

    ReaddirHookCallCnt = 0;
    memset((void*)&ReaddirHookDirEntry, 0x00, sizeof(ReaddirHookDirEntry));

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

    ReaddirHookCallCnt = 0;
    memset((void*)&ReaddirHookDirEntry, 0x00, sizeof(ReaddirHookDirEntry));

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

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    ZeroCopyGetPtrHookCallCnt = 0;
    ZeroCopyGetPtrHookOffset = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                             (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Cycle each transaction in the machine list */
    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();

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

    /* Set to return that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute a playback file command so that one queue entry is added
       to the pending queue */
    CFE_SB_InitMsg((void*)pCmd, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd, CF_PLAYBACK_FILE_CC);
    pCmd->Class = CF_CLASS_1;
    pCmd->Channel = 0;
    pCmd->Priority = 0;
    pCmd->Preserve = CF_DELETE_FILE;
    strcpy(pCmd->PeerEntityId, TestPbPeerEntityId);
    strcpy(pCmd->SrcFilename, TestPbDir);
    strcat(pCmd->SrcFilename, TestPbFile1);
    strcpy(pCmd->DstFilename, TestDstDir);

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

    /* Set to return that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute first playback file command so that one queue entry is
       added to the pending queue */
    CFE_SB_InitMsg((void*)pCmd1, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd1, CF_PLAYBACK_FILE_CC);
    pCmd1->Class = CF_CLASS_1;
    pCmd1->Channel = 0;
    pCmd1->Priority = 3;
    pCmd1->Preserve = CF_DELETE_FILE;
    strcpy(pCmd1->PeerEntityId, TestPbPeerEntityId);
    strcpy(pCmd1->SrcFilename, TestPbDir);
    strcat(pCmd1->SrcFilename, TestPbFile1);
    strcpy(pCmd1->DstFilename, TestDstDir);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Execute second playback file command so that one queue entry is
       added to the pending queue */
    CFE_SB_InitMsg((void*)pCmd2, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd2, CF_PLAYBACK_FILE_CC);
    pCmd2->Class = CF_CLASS_1;
    pCmd2->Channel = 0;
    pCmd2->Priority = 5;
    pCmd2->Preserve = CF_DELETE_FILE;
    strcpy(pCmd2->PeerEntityId, TestPbPeerEntityId);
    strcpy(pCmd2->SrcFilename, TestPbDir);
    strcat(pCmd2->SrcFilename, TestPbFile2);
    strcpy(pCmd2->DstFilename, TestDstDir);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_CreateOnePbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    /* Force OS_stat to return a valid size and success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    CF_TstUtil_CreateOnePbPendingQueueEntry(pCmd);

    CF_StartNextFile(0);
}


void CF_TstUtil_CreateTwoPbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                            CF_PlaybackFileCmd_t *pCmd2)
{
    /* Force OS_stat to return a valid size and success */
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

    ZeroCopyGetPtrHookCallCnt = 0;
    ZeroCopyGetPtrHookOffset = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                             (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();
}


void CF_TstUtil_FinishOnePbActiveQueueEntryByInd(CF_PlaybackFileCmd_t *pCmd)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    TransInfo.role = CLASS_1_SENDER;
    TransInfo.trans.number = 1;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 3;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.source_file_name, pCmd->SrcFilename);
    strcpy(TransInfo.md.dest_file_name, pCmd->DstFilename);

    CF_Indication(IndType,TransInfo);
}


void CF_TstUtil_FinishTwoPbActiveQueueEntryByInd(CF_PlaybackFileCmd_t *pCmd1,
                                                 CF_PlaybackFileCmd_t *pCmd2)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    TransInfo.role = CLASS_1_SENDER;
    TransInfo.trans.number = 1;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 3;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.source_file_name, pCmd1->SrcFilename);
    strcpy(TransInfo.md.dest_file_name, pCmd1->DstFilename);

    CF_Indication(IndType,TransInfo);

    TransInfo.role = CLASS_1_SENDER;
    TransInfo.trans.number = 2;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 3;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.source_file_name, pCmd2->SrcFilename);
    strcpy(TransInfo.md.dest_file_name, pCmd2->DstFilename);

    CF_Indication(IndType,TransInfo);
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


void CF_TstUtil_CreateTwoPbHistoryQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                             CF_PlaybackFileCmd_t *pCmd2)
{
    CF_CARSCmd_t    CARSCmdMsg;

    CF_TstUtil_CreateTwoPbActiveQueueEntry(pCmd1, pCmd2);

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


void CF_TstUtil_CreateOneUpActiveQueueEntry(CF_Test_InPDUMsg_t *pCmd)
{
    uint32  PDataLen;
    int     hdr_len;
    int     index;
    int     str_len;
    char    src_filename[OS_MAX_PATH_LEN];
    char    dst_filename[OS_MAX_PATH_LEN];

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    hdr_len = sizeof(CF_PDU_Hdr_t);

    /* Execute a Incoming MetaData PDU command so that
       one queue entry is added to the active queue */
    CFE_SB_InitMsg((void*)pCmd, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd->PduContent.Content[index++] = 0x00;

    /* file size: 0x100 Little Endian */
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x01;
    pCmd->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile1);
    sprintf(dst_filename, "%s", TestInNoFile);

    str_len = strlen(src_filename);
    pCmd->PduContent.Content[index++] = str_len;
    memcpy((void *)&pCmd->PduContent.Content[index], (void *)src_filename,
           str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd->PduContent.Content[index++] = str_len;
    memcpy((void *)&pCmd->PduContent.Content[index], (void *)dst_filename,
           str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_CreateTwoUpActiveQueueEntry(CF_Test_InPDUMsg_t *pCmd1,
                                            CF_Test_InPDUMsg_t *pCmd2)
{
    uint32  PDataLen;
    int     hdr_len;
    int     index;
    int     str_len;
    char    src_filename[OS_MAX_PATH_LEN];
    char    dst_filename[OS_MAX_PATH_LEN];

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    hdr_len = sizeof(CF_PDU_Hdr_t);

    /* Execute the first Incoming MetaData PDU command so that
       one queue entry is added to the active queue */
    CFE_SB_InitMsg((void*)pCmd1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd1->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end: Little Endian */
    pCmd1->PduContent.PHdr.PDataLen = 0x0000; /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd1->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd1->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd1->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd1->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd1->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd1->PduContent.Content[index++] = 0x00;

    /* file size: 0x100 Little Endian */
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x01;
    pCmd1->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile1);
    sprintf(dst_filename, "%s", TestInNoFile);

    str_len = strlen(src_filename);
    pCmd1->PduContent.Content[index++] = str_len;
    memcpy((void *)&pCmd1->PduContent.Content[index], (void *)src_filename,
           str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd1->PduContent.Content[index++] = str_len;
    memcpy((void *)&pCmd1->PduContent.Content[index], (void *)dst_filename,
           str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd1->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd1->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Execute the second Incoming MetaData PDU command so that
       one queue entry is added to the active queue */
    CFE_SB_InitMsg((void*)pCmd2, CF_GND_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd2->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size : Modified at the end Little Endian */
    pCmd2->PduContent.PHdr.PDataLen = 0x0000;
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd2->PduContent.PHdr.Octet4 = 0x13;
    /* 0.50 : Little Endian */
    pCmd2->PduContent.PHdr.SrcEntityId = 0x3200;
    /* 0x2bc = 700 : Little Endian */
    pCmd2->PduContent.PHdr.TransSeqNum = 0xbc020000;
    /* 0.3 : Little Endian */
    pCmd2->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd2->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd2->PduContent.Content[index++] = 0x00;

    /* file size: 0x100 Little Endian */
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x01;
    pCmd2->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile2);
    sprintf(dst_filename, "%s", TestInNoFile);

    str_len = strlen(src_filename);
    pCmd2->PduContent.Content[index++] = str_len;
    memcpy((void *)&pCmd2->PduContent.Content[index], (void *)src_filename,
           str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd2->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd2->PduContent.Content[index]), (void *)dst_filename,
           str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd2->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd2->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_SendOneCompleteIncomingPDU(CF_Test_InPDUMsg_t *pCmd)
{
    uint32  PDataLen;
    int     hdr_len;
    int     index;
    int     str_len;
    char    src_filename[OS_MAX_PATH_LEN];
    char    dst_filename[OS_MAX_PATH_LEN];

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    hdr_len = sizeof(CF_PDU_Hdr_t);

    /* Meta Data PDU */
    CFE_SB_InitMsg((void*)pCmd, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd->PduContent.Content[index++] = 0x00;

    /* file size: 0x100 Little Endian */
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x01;
    pCmd->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile1);
    sprintf(dst_filename, "%s%s", TestInDir, TestInFile1);

    str_len = strlen(src_filename);
    pCmd->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd->PduContent.Content[index]),
           (void *)src_filename, str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd->PduContent.Content[index]),
           (void *)dst_filename, str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* File Data PDU */
    CFE_SB_InitMsg((void*)pCmd, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: FD_PDU,toward rcvr,class1,crc not present */
    pCmd->PduContent.PHdr.Octet1 = 0x14;
    /* pdu data field size: modified at the end */
    pCmd->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;

    /* Offset */
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;

    /* File contents: 0x100(file size) - 0x04(offset) */
    memset((void *)&(pCmd->PduContent.Content[index]), 0xff, 256 - 4);
    index += (256 - 4);

    PDataLen = index - hdr_len;
    pCmd->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* EOF PDU */
    CFE_SB_InitMsg((void*)pCmd, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: EOF_PDU,toward rcvr,class1,crc not present */
    pCmd->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd->PduContent.Content[index++] = EOF_PDU;
    /* Condition Code: NO_ERROR */
    pCmd->PduContent.Content[index++] = NO_ERROR << 4;
    /* Checksum */
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    /* File Size: 0x100 */
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x00;
    pCmd->PduContent.Content[index++] = 0x01;
    pCmd->PduContent.Content[index++] = 0x00;

    PDataLen = index - hdr_len;
    pCmd->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_SendTwoCompleteIncomingPDU(CF_Test_InPDUMsg_t *pCmd1,
                                           CF_Test_InPDUMsg_t *pCmd2)
{
    uint32  PDataLen;
    int     hdr_len;
    int     index;
    int     str_len;
    char    src_filename[OS_MAX_PATH_LEN];
    char    dst_filename[OS_MAX_PATH_LEN];

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    hdr_len = sizeof(CF_PDU_Hdr_t);

    /***** File 1 *****/
    /* Meta Data PDU */
    CFE_SB_InitMsg((void*)pCmd1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd1->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd1->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd1->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd1->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd1->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd1->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd1->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd1->PduContent.Content[index++] = 0x00;

    /* file size: 0x100 Little Endian */
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x01;
    pCmd1->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile1);
    sprintf(dst_filename, "%s%s", TestInDir, TestInFile1);

    str_len = strlen(src_filename);
    pCmd1->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd1->PduContent.Content[index]),
           (void *)src_filename, str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd1->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd1->PduContent.Content[index]),
           (void *)dst_filename, str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd1->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd1->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* File Data PDU */
    CFE_SB_InitMsg((void*)pCmd1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: FD_PDU,toward rcvr,class1,crc not present */
    pCmd1->PduContent.PHdr.Octet1 = 0x14;
    /* pdu data field size: modified at the end */
    pCmd1->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd1->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd1->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd1->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd1->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;

    /* Offset */
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;

    /* File contents: 0x100(file size) - 0x04(offset) */
    memset((void *)&(pCmd1->PduContent.Content[index]), 0xff, 256 - 4);
    index += (256 - 4);

    PDataLen = index - hdr_len;
    pCmd1->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd1->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* EOF PDU */
    CFE_SB_InitMsg((void*)pCmd1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: EOF_PDU,toward rcvr,class1,crc not present */
    pCmd1->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd1->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd1->PduContent.PHdr.Octet4 = 0x13;
    /* 0.23 : Little Endian */
    pCmd1->PduContent.PHdr.SrcEntityId = 0x1700; /* To Big Endian */
    /* 0x1f4 = 500 : Little Endian */
    pCmd1->PduContent.PHdr.TransSeqNum = 0xf4010000;
    /* 0.3 : Little Endian */
    pCmd1->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd1->PduContent.Content[index++] = EOF_PDU;
    /* Condition Code: NO_ERROR */
    pCmd1->PduContent.Content[index++] = NO_ERROR << 4;
    /* Checksum */
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    /* File Size: 0x100 */
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x00;
    pCmd1->PduContent.Content[index++] = 0x01;
    pCmd1->PduContent.Content[index++] = 0x00;

    PDataLen = index - hdr_len;
    pCmd1->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd1->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /***** File 2 *****/
    /* Meta Data PDU */
    CFE_SB_InitMsg((void*)pCmd2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: MD_PDU,toward rcvr,class1,crc not present */
    pCmd2->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd2->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd2->PduContent.PHdr.Octet4 = 0x13;
    /* 0.50 : Little Endian */
    pCmd2->PduContent.PHdr.SrcEntityId = 0x3200; /* To Big Endian */
    /* 0x2bc = 700 : Little Endian */
    pCmd2->PduContent.PHdr.TransSeqNum = 0xbc020000;
    /* 0.3 : Little Endian */
    pCmd2->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd2->PduContent.Content[index++] = MD_PDU;
    /* No Segmentation control */
    pCmd2->PduContent.Content[index++] = 0x00;

    /* file size: 0x200 Little Endian */
    pCmd2->PduContent.Content[index++] = 0x00;   // check this
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x02;
    pCmd2->PduContent.Content[index++] = 0x00;

    sprintf(src_filename, "%s%s", TestInDir, TestInFile2);
    sprintf(dst_filename, "%s%s", TestInDir, TestInFile2);

    str_len = strlen(src_filename);
    pCmd2->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd2->PduContent.Content[index]),
           (void *)src_filename, str_len);
    index += str_len;

    str_len = strlen(dst_filename);
    pCmd2->PduContent.Content[index++] = str_len;
    memcpy((void *)&(pCmd2->PduContent.Content[index]),
           (void *)dst_filename, str_len);
    index += str_len;

    PDataLen = index - hdr_len;
    pCmd2->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd2->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* File Data PDU */
    CFE_SB_InitMsg((void*)pCmd2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: FD_PDU,toward rcvr,class1,crc not present */
    pCmd2->PduContent.PHdr.Octet1 = 0x14;
    /* pdu data field size: modified at the end */
    pCmd2->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd2->PduContent.PHdr.Octet4 = 0x13;
    /* 0.50 : Little Endian */
    pCmd2->PduContent.PHdr.SrcEntityId = 0x3200; /* To Big Endian */
    /* 0x2bc = 700 : Little Endian */
    pCmd2->PduContent.PHdr.TransSeqNum = 0xbc020000;
    /* 0.3 : Little Endian */
    pCmd2->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;

    /* Offset */
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;

    /* File contents: 0x200(file size) - 0x04(offset) */
    memset((void *)&(pCmd2->PduContent.Content[index]), 0xff, 512 - 4);
    index += (512 - 4);

    PDataLen = index - hdr_len;
    pCmd2->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd2->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* EOF PDU */
    CFE_SB_InitMsg((void*)pCmd2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    /* file directive: EOF_PDU,toward rcvr,class1,crc not present */
    pCmd2->PduContent.PHdr.Octet1 = 0x04;
    /* pdu data field size: modified at the end */
    pCmd2->PduContent.PHdr.PDataLen = 0x0000;  /* To Big Endian */
    /*hex 1 - entityID len is 2, hex 3 - TransSeq len is 4 */
    pCmd2->PduContent.PHdr.Octet4 = 0x13;
    /* 0.50 : Little Endian */
    pCmd2->PduContent.PHdr.SrcEntityId = 0x3200; /* To Big Endian */
    /* 0x2bc = 700 : Little Endian */
    pCmd2->PduContent.PHdr.TransSeqNum = 0xbc020000;
    /* 0.3 : Little Endian */
    pCmd2->PduContent.PHdr.DstEntityId = 0x0300;

    index = hdr_len;
    /* File Dir Code */
    pCmd2->PduContent.Content[index++] = EOF_PDU;
    /* Condition Code: NO_ERROR */
    pCmd2->PduContent.Content[index++] = NO_ERROR << 4;
    /* Checksum */
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    /* File Size: 0x200 */
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x00;
    pCmd2->PduContent.Content[index++] = 0x02;
    pCmd2->PduContent.Content[index++] = 0x00;

    PDataLen = index - hdr_len;
    pCmd2->PduContent.Content[1] = (PDataLen & 0x0000ff00) >> 8;
    pCmd2->PduContent.Content[2] = PDataLen & 0x000000ff;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;
    CF_AppPipe(CF_AppData.MsgPtr);
}


/* This util adds one entry only to UpQueue but not to the machine list */
void CF_TstUtil_CreateOneUpActiveQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd)
{
    INDICATION_TYPE IndType = IND_MACHINE_ALLOCATED;
    TRANS_STATUS TransInfo;

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;

    CFE_SB_InitMsg((void*)pCmd, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;;

    CF_Indication(IndType,TransInfo);
}


/* This util adds two entries only to UpQueue but not to the machine list */
void CF_TstUtil_CreateTwoUpActiveQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd1,
                                                 CF_Test_InPDUMsg_t *pCmd2)
{
    INDICATION_TYPE IndType = IND_MACHINE_ALLOCATED;
    TRANS_STATUS TransInfo;

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* First incoming PDU */
    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;

    CFE_SB_InitMsg((void*)pCmd1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd1;;
    CF_Indication(IndType,TransInfo);

    /* Second incoming PDU */
    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 700;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 50;

    CFE_SB_InitMsg((void*)pCmd1, CF_GND_TO_CPD_PDU_MID,
                   sizeof(CF_Test_InPDUMsg_t), TRUE);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd2;;
    CF_Indication(IndType,TransInfo);
}


void CF_TstUtil_CreateOneUpHistoryQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    CF_TstUtil_CreateOneUpActiveQueueEntry(pCmd);

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.dest_file_name, TestInDir);
    strcat(TransInfo.md.dest_file_name, TestInFile1);

    CF_Indication(IndType,TransInfo);
}


void CF_TstUtil_CreateTwoUpHistoryQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd1,
                                                  CF_Test_InPDUMsg_t *pCmd2)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    CF_TstUtil_CreateTwoUpActiveQueueEntry(pCmd1, pCmd2);

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.dest_file_name, TestInDir);
    strcat(TransInfo.md.dest_file_name, TestInFile1);

    CF_Indication(IndType,TransInfo);

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 700;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 50;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.dest_file_name, TestInDir);
    strcat(TransInfo.md.dest_file_name, TestInFile2);

    CF_Indication(IndType,TransInfo);
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
