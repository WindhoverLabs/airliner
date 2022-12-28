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

const char TestQInfoFile1[] = "qinfofile1.dat";

const char TestPbDir[] = "/cf/";
const char TestDstDir[] = "gndpath/";
const char TestInDir[] = "inpath/";

const char TestPeerEntityId[] = "2.25";


void CF_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemSet(&CF_AppData, 0x00, sizeof(CF_AppData));

    CFE_ES_GetPoolBufHookCallCnt = 0;
    ReaddirHookCallCnt = 0;
    memset((void*)&ReaddirHookDirEntry, 0x00, sizeof(ReaddirHookDirEntry));

    Ut_CFE_ES_Reset();
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_TIME_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

#if 0
    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTable);
#else
    Ut_CFE_TBL_AddTable(CF_CONFIG_TABLE_FILENAME, (void*)&CF_ConfigTableUnitTest);
#endif
}


void CF_Test_TearDown(void)
{
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


void CF_TstUtil_CreateOnePendingQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Execute a playback file command so that one queue entry is added
       to the pending queue */
    CFE_SB_InitMsg((void*)pCmd, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)pCmd, CF_PLAYBACK_FILE_CC);
    pCmd->Class = 1;
    pCmd->Channel = 0;
    pCmd->Priority = 0;
    pCmd->Preserve = CF_KEEP_FILE;
    strcpy(pCmd->PeerEntityId, TestPeerEntityId);
    strcpy(pCmd->SrcFilename, TestPbDir);
    strcat(pCmd->SrcFilename, TestPbFile1);
    strcpy(pCmd->DstFilename, TestDstDir);

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

    /* execute the playback file cmd to get a queue entry on the
       pending queue */
    CF_AppInit();     /* reset CF globals etc */

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)pCmd;
    CF_AppPipe(CF_AppData.MsgPtr);
}


void CF_TstUtil_CreateOnePbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    CF_TstUtil_CreateOnePendingQueueEntry(pCmd);

    /* Force OS_stat to return a valid size and success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    CF_StartNextFile(0);
}


void CF_TstUtil_CreateOnePbHistoryQueueEntry(CF_PlaybackFileCmd_t *pCmd)
{
    CF_CARSCmd_t    CARSCmdMsg;

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&CARSCmdMsg, CF_CMD_MID, sizeof(CF_CARSCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CARSCmdMsg, CF_ABANDON_CC);
    strcpy(CARSCmdMsg.Trans, "All");

    CF_TstUtil_CreateOnePbActiveQueueEntry(pCmd);

    cfdp_cycle_each_transaction();

    /* Send Abandon Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CARSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    cfdp_cycle_each_transaction();

    cfdp_cycle_each_transaction();
}


void CF_TstUtil_CreateOneUpActiveQueueEntry(void)
{
    INDICATION_TYPE IndType = IND_MACHINE_ALLOCATED;
    TRANS_STATUS    TransInfo;

    /* reset CF globals etc */
    CF_AppInit();

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    CF_Indication(IndType,TransInfo);
}


void CF_TstUtil_CreateOneUpHistoryQueueEntry(void)
{
    INDICATION_TYPE IndType = IND_MACHINE_DEALLOCATED;
    TRANS_STATUS    TransInfo;

    CF_TstUtil_CreateOneUpActiveQueueEntry();

    TransInfo.role = CLASS_1_RECEIVER;
    TransInfo.trans.number = 500;
    TransInfo.trans.source_id.value[0] = 0;
    TransInfo.trans.source_id.value[1] = 23;
    TransInfo.final_status = FINAL_STATUS_SUCCESSFUL;
    strcpy(TransInfo.md.dest_file_name, TestInDir);
    strcat(TransInfo.md.dest_file_name, TestInFile1);

    CF_Indication(IndType,TransInfo);
}


void CF_ResetEngine(void)
{

    CF_CARSCmd_t  CmdMsg;

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CF_CARSCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, CF_ABANDON_CC);
    strcpy(CmdMsg.Trans, "All");

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    cfdp_cycle_each_transaction();

    misc__set_trans_seq_num(1);
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
