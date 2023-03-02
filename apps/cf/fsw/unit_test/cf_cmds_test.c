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

#include "cf_cmds_test.h"
#include "cf_test_utils.h"
#include "cf_custom_hooks.h"

#include "cf_events.h"
#include "cf_version.h"
#include "structures.h"
#include "timer.h"
#include "machine.h"
#include "machine_list.h"
#include "misc.h"
#include "cf_playback.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>


extern int errno;

CFE_SB_MsgId_t  SendHkHook_MsgId = 0;
CF_HkPacket_t   HkHookPkt;

CFE_SB_MsgId_t     SendCfgParamsHook_MsgId = 0;
CF_ConfigPacket_t  CfgHookPkt;

uint32          SendTransDiagHook_CalledCnt = 0;
/* (Pb PendingQ depth + HistoryQ depth) * 2 channel + Up HistQ depth */
CFE_SB_MsgId_t   SendTransDiagHook_MsgId[500];
CF_TransPacket_t TransPkt[500];



/**************************************************************************
 * Tests for CF_AppPipe()
 **************************************************************************/
/**
 * Test CF_AppPipe, InvalidCmdMessage
 */
void Test_CF_AppPipe_InvalidCmdMessage(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, 0, sizeof(CmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unexpected Msg Received MsgId -- ID = 0x%04X", 0);

    /* Verify results */
    UtAssert_EventSent(CF_MID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppMain, fail InvalidCmdMessage: Event Sent");
}


/**
 * Test CF_AppPipe, InvalidCmdCode
 */
void Test_CF_AppPipe_InvalidCmdCode(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
        "Cmd Msg with Invalid command code Rcvd -- ID = 0x%04X, CC = %d",
        CF_CMD_MID, 100);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "Test_CF_AppPipe_InvalidCmdCode");

    UtAssert_EventSent(CF_CC_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "Test_CF_AppPipe_InvalidCmdCode: Event Sent");
}


/**
 * Test CF_AppPipe, NoopCmd
 */
void Test_CF_AppPipe_NoopCmd(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_NOOP_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF No-op command, Version %d.%d.%d.%d",
                      CF_MAJOR_VERSION, CF_MINOR_VERSION,
                      CF_REVISION, CF_MISSION_REV);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, NoopCmd");

    UtAssert_EventSent(CF_NOOP_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, NoopCmd: Event Sent");
}


/**
 * Test CF_AppPipe, NoopCmdInvLen
 */
void Test_CF_AppPipe_NoopCmdInvLen(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_NOOP_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_NOOP_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, NoopCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, NoopCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, RstCtrsCmd
 */
void Test_CF_AppPipe_RstCtrsCmd(void)
{
    CF_ResetCtrsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_RESET_CC);
    /* 0: All, 1: Cmd Cnt, 2: Flt Cnt, 3: Uplink Cnt, 4: Downlink Cnt */
    CmdMsg.Value = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Reset Counters command received - Value %u",
            CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, RstCtrsCmd");

    UtAssert_EventSent(CF_RESET_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, RstCtrsCmd: Event Sent");
}


/**
 * Test CF_AppPipe, RstCtrsCmdInvLen
 */
void Test_CF_AppPipe_RstCtrsCmdInvLen(void)
{
    CF_ResetCtrsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_RESET_CC);
    CmdMsg.Value = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_RESET_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, RstCtrsCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, RstCtrsCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileCmd
 */
void Test_CF_AppPipe_PbFileCmd(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  PeerEntityString[MAX_FILE_NAME_LENGTH];
    char  FullSrcFilename[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(PeerEntityString, PbFileCmdMsg.PeerEntityId);
    strcpy(FullSrcFilename, PbFileCmdMsg.SrcFilename);
    sprintf(expEvent,
        "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,Peer %s,File %s",
         PbFileCmdMsg.Class, PbFileCmdMsg.Channel, PbFileCmdMsg.Priority,
         PbFileCmdMsg.Preserve, PeerEntityString, FullSrcFilename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbFileCmd");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, PbFileCmd: Q EntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbFileCmd: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdNoMem
 */
void Test_CF_AppPipe_PbFileCmdNoMem(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute a playback file command so that one queue entry is added
       to the pending queue */
    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);
    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX,
                            CFE_ES_ERR_MEM_HANDLE, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "PB File %s Cmd Ignored,Error Allocating Queue Node.",
            PbFileCmdMsg.SrcFilename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdNoMem");

    UtAssert_EventSent(CF_QDIR_NOMEM1_EID, CFE_EVS_ERROR, expEvent,
                   "CF_AppPipe, PbFileCmdNoMem: AllocQ Error Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdInvLen
 */
void Test_CF_AppPipe_PbFileCmdInvLen(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_PLAYBACK_FILE_CC,
            sizeof(CF_PlaybackFileCmd_t), sizeof(CF_PlaybackFileCmd_t) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdParamErr
 */
void Test_CF_AppPipe_PbFileCmdParamErr(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_2 + 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Playback File Cmd Parameter error, class %d, chan %d",
            PbFileCmdMsg.Class, PbFileCmdMsg.Channel);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdParamErr");

    UtAssert_EventSent(CF_PB_FILE_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdParamErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdChanNotInUse
 */
void Test_CF_AppPipe_PbFileCmdChanNotInUse(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 1;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.Tbl->OuCh[PbFileCmdMsg.Channel].EntryInUse =
                                                  CF_ENTRY_UNUSED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "CF:Playback File Cmd Parameter Error, Chan %u is not in use.",
            PbFileCmdMsg.Channel);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdChanNotInUse");

    UtAssert_EventSent(CF_PB_FILE_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdChanNotInUse: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdInvSrcFilename
 */
void Test_CF_AppPipe_PbFileCmdInvSrcFilename(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, " pbfile1.dat");
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, "pbfile1.dat");

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Filename in %s must be terminated and have no spaces",
            "PlaybackFileCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdInvSrcFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdInvSrcFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdInvDstFilename
 */
void Test_CF_AppPipe_PbFileCmdInvDstFilename(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    /* dest filename not terminated */
    CFE_PSP_MemSet(PbFileCmdMsg.DstFilename, 0xFF, MAX_FILE_NAME_LENGTH);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Filename in %s must be terminated and have no spaces",
            "PlaybackFileCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdInvDstFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdInvDstFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdPendQFull
 */
void Test_CF_AppPipe_PbFileCmdPendQFull(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt =
                 CF_AppData.Tbl->OuCh[PbFileCmdMsg.Channel].PendingQDepth;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
      "CF:Playback File Cmd Error, Chan %u Pending Queue is full %u.",
      PbFileCmdMsg.Channel,
      (unsigned int)CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdPendQFull");

    UtAssert_EventSent(CF_PB_FILE_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdPendQFull: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdInvPeerId
 */
void Test_CF_AppPipe_PbFileCmdInvPeerId(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, "2555.255");
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:PB File Cmd Err, PeerEntityId %s must be "
            "2 byte,dotted decimal fmt.ex 0.24",
            PbFileCmdMsg.PeerEntityId);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdInvPeerId");

    UtAssert_EventSent(CF_PB_FILE_ERR6_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdInvPeerId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdFileOpen
 */
void Test_CF_AppPipe_PbFileCmdFileOpen(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = CF_CLASS_1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_DELETE_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId1);
    sprintf(PbFileCmdMsg.SrcFilename, "%s%s", TestPbDir0, TestPbFile1);
    sprintf(PbFileCmdMsg.DstFilename, "%s%s", TestDstDir0, TestPbFile1);

    CF_Test_PrintCmdMsg((void*)&PbFileCmdMsg, sizeof(PbFileCmdMsg));

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_FDGETINFO_INDEX,
                                 (void*)&OS_FDGetInfoHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Playback File Cmd Error, File is Open:%s",
            PbFileCmdMsg.SrcFilename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdFileOpen");

    UtAssert_EventSent(CF_PB_FILE_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdFileOpen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbFileCmdFileOnQ
 */
void Test_CF_AppPipe_PbFileCmdFileOnQ(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    /* Add same command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEvent, "CF:Playback File Cmd Error, File is Already "
            "Pending or Active:%s", FullSrcFileName);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdFileOnQ");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, PbFileCmdFileOnQ: Q EntryCnt");

    UtAssert_EventSent(CF_PB_FILE_ERR5_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileCmdFileOnQ: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdNoFileSuccess
 */
void Test_CF_AppPipe_PbDirCmdNoFileSuccess(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcDir[MAX_FILE_NAME_LENGTH];
    char  FullDstDir[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    /* Force OS_opendir to return success, instead of default NULL */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                          UT_OSFILEAPI_OPENDIR_INDEX);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    QEntryCntBefore =
          CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
          CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcDir, PbDirCmdMsg.SrcPath);
    strcpy(FullDstDir, PbDirCmdMsg.DstPath);
    sprintf(expEvent, "Playback Dir Cmd Rcvd,Ch %d,Cl %d,Pri %d,Pre %d,"
            "Peer %s, Src %s,Dst %s",
            PbDirCmdMsg.Chan, PbDirCmdMsg.Class, PbDirCmdMsg.Priority,
            PbDirCmdMsg.Preserve, PbDirCmdMsg.PeerEntityId,
            FullSrcDir, FullDstDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmdNoFileSuccess");

    UtAssert_True((QEntryCntBefore == 0) && (QEntryCntAfter == 0),
                  "CF_AppPipe, PbDirCmdNoFileSuccess: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_DIR_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbDirCmdNoFileSuccess: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdOpenErr
 */
void Test_CF_AppPipe_PbDirCmdOpenErr(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcDir[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, (int32)NULL, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                           UT_OSFILEAPI_OPENDIR_INDEX);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(FullSrcDir, PbDirCmdMsg.SrcPath);
    sprintf(expEvent, "Playback Dir Error %d,cannot open directory %s",
            (int)NULL, FullSrcDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdOpenErr");

    UtAssert_EventSent(CF_OPEN_DIR_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdOpenErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdInvLen
 */
void Test_CF_AppPipe_PbDirCmdInvLen(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_PLAYBACK_DIR_CC, sizeof(CF_PlaybackDirCmd_t),
            sizeof(CF_PlaybackDirCmd_t) + 5);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdParamErr
 */
void Test_CF_AppPipe_PbDirCmdParamErr(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_2 + 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Playback Dir Cmd Parameter error,class %d,"
            "chan %d,preserve %d",
            PbDirCmdMsg.Class, PbDirCmdMsg.Chan, PbDirCmdMsg.Preserve);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdParamErr");

    UtAssert_EventSent(CF_PB_DIR_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdParamErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdChanNotInUse
 */
void Test_CF_AppPipe_PbDirCmdChanNotInUse(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 1;
    PbDirCmdMsg.Priority = 2;

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].EntryInUse = CF_ENTRY_UNUSED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "CF:Playback Dir Cmd Parameter Error, Chan %u is not in use.",
            PbDirCmdMsg.Chan);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdChanNotInUse");

    UtAssert_EventSent(CF_PB_DIR_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdChanNotInUse: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdInvSrcPath
 */
void Test_CF_AppPipe_PbDirCmdInvSrcPath(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;
    /* The last char should be forward slash */
    strcpy(PbDirCmdMsg.SrcPath, "/cf/pbfile1.txt");

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "SrcPath in PB Dir Cmd must be terminated,"
            "have no spaces,slash at end");

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdInvSrcPath");

    UtAssert_EventSent(CF_PB_DIR_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdInvSrcPath: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdInvDstPath
 */
void Test_CF_AppPipe_PbDirCmdInvDstPath(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;
    /* dest filename not terminated */
    CFE_PSP_MemSet(PbDirCmdMsg.DstPath, 0xFF, MAX_FILE_NAME_LENGTH);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "DstPath in PB Dir Cmd must be terminated "
            "and have no spaces");

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdInvDstPath");

    UtAssert_EventSent(CF_PB_DIR_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdInvDstPath: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdInvPeerId
 */
void Test_CF_AppPipe_PbDirCmdInvPeerId(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;
    strcpy(PbDirCmdMsg.PeerEntityId, "2555.255");

    /* Execute the function being tested */
    CF_AppInit();
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:PB Dir Cmd Err,PeerEntityId %s must be 2 byte,"
            "dotted decimal fmt.ex 0.24", PbDirCmdMsg.PeerEntityId);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdInvPeerId");

    UtAssert_EventSent(CF_PB_DIR_ERR5_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdInvPeerId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdQFull
 */
void Test_CF_AppPipe_PbDirCmdQFull(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcDir[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                   UT_OSFILEAPI_CLOSEDIR_INDEX);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    /* makes the pending queue appear full */
    CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt =
                    CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PendingQDepth;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(FullSrcDir, PbDirCmdMsg.SrcPath);
    sprintf(expEvent, "Queue Dir %s Aborted,Ch %d Pending Queue is Full,"
        "%u Entries", FullSrcDir, PbDirCmdMsg.Chan,
        (unsigned int)CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdQFull");

    UtAssert_EventSent(CF_QDIR_PQFUL_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdQFull: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdNoMem
 */
void Test_CF_AppPipe_PbDirCmdNoMem(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcDir[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_CLOSEDIR_INDEX);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX,
                            CFE_ES_ERR_MEM_HANDLE, 1);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(FullSrcDir, PbDirCmdMsg.SrcPath);
    sprintf(expEvent, "PB Dir %s Aborted,Error Allocating Queue Node.",
            FullSrcDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdNoMem");

    UtAssert_EventSent(CF_QDIR_NOMEM2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdNoMem: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdFileOnQ
 */
void Test_CF_AppPipe_PbDirCmdFileOnQ(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                    UT_OSFILEAPI_CLOSEDIR_INDEX);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    QEntryCntBefore =
          CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
          CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(expEvent, "File %s not queued because it's active or pending",
            PbFileCmdMsg.SrcFilename);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmdFileOnQ");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 3),
                  "CF_AppPipe, PbDirCmdFileOnQ: QEntryCnt");

    UtAssert_EventSent(CF_QDIR_ACTIVEFILE_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbDirCmdFileOnQ: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdFileOpen
 */
void Test_CF_AppPipe_PbDirCmdFileOpen(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcFilename[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                    UT_OSFILEAPI_CLOSEDIR_INDEX);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_FDGETINFO_INDEX,
                                 (void*)&OS_FDGetInfoHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    QEntryCntBefore =
            CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
            CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(FullSrcFilename, "%s%s", PbDirCmdMsg.SrcPath, TestPbFile1);
    sprintf(expEvent, "File %s not queued because it's open",
            FullSrcFilename);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmdFileOpen");

    UtAssert_True((QEntryCntBefore == 0) && (QEntryCntAfter == 2),
                   "CF_AppPipe, PbDirCmdFileOpen: QEntry Cnt");

    UtAssert_EventSent(CF_QDIR_OPENFILE_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, PbDirCmdFileOpen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PbDirCmdSuccess
 */
void Test_CF_AppPipe_PbDirCmdSuccess(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcDir[MAX_FILE_NAME_LENGTH];
    char  FullDstDir[MAX_FILE_NAME_LENGTH];
    char  expEventTerm[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventLen[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = CF_CLASS_1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 2;

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                                    OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                         UT_OSFILEAPI_CLOSEDIR_INDEX);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();
    strcpy(PbDirCmdMsg.PeerEntityId,
           CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].PeerEntityId);
    PbDirCmdMsg.Preserve =
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].Preserve;
    strcpy(PbDirCmdMsg.SrcPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].SrcPath);
    strcpy(PbDirCmdMsg.DstPath,
                CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PollDir[0].DstPath);
    CF_Test_PrintCmdMsg((void*)&PbDirCmdMsg, sizeof(PbDirCmdMsg));

    QEntryCntBefore =
            CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
            CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcDir, PbDirCmdMsg.SrcPath);
    strcpy(FullDstDir, PbDirCmdMsg.DstPath);
    sprintf(expEventTerm,
            "File not queued from %s,Filename not terminated or too long",
            FullSrcDir);
    sprintf(expEventLen,
            "File not queued from %s,sum of Pathname,Filename too long",
            FullSrcDir);
    sprintf(expEvent, "Playback Dir Cmd Rcvd,Ch %d,Cl %d,Pri %d,Pre %d,"
            "Peer %s, Src %s,Dst %s",
            PbDirCmdMsg.Chan, PbDirCmdMsg.Class, PbDirCmdMsg.Priority,
            PbDirCmdMsg.Preserve, PbDirCmdMsg.PeerEntityId,
            FullSrcDir, FullDstDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmdSuccess");

    UtAssert_True((QEntryCntBefore == 0) && (QEntryCntAfter == 3),
                  "CF_AppPipe, PbDirCmdSuccess: QEntryCnt");

    UtAssert_EventSent(CF_QDIR_INV_NAME1_EID, CFE_EVS_ERROR, expEventTerm,
             "CF_AppPipe, PbDirCmdSuccess: Termination Error Event Sent");

    UtAssert_EventSent(CF_QDIR_INV_NAME2_EID, CFE_EVS_ERROR, expEventLen,
             "CF_AppPipe, PbDirCmdSuccess: Length Error Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_DIR_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, PbDirCmdSuccess: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Hook to support: Test CF_AppPipe, HousekeepingCmd
 */
int32 Test_CF_AppPipe_HousekeepingCmd_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    pBuff = (unsigned char *)MsgPtr;
    printf("###HousekeepingCmd_SendMsgHook(msgLen %u): ", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = CF_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case CF_HK_TLM_MID:
        {
            SendHkHook_MsgId = CF_HK_TLM_MID;
            printf("Sent CF_HK_TLM_MID\n");
            memcpy((void *)&HkHookPkt, (void *)MsgPtr, sizeof(HkHookPkt));

            printf("CmdCounter: %u\n", HkHookPkt.CmdCounter);
            printf("ErrCounter: %u\n", HkHookPkt.ErrCounter);

            printf("App.WakeupForFileProc: %lu\n",
                    HkHookPkt.App.WakeupForFileProc);
            printf("App.EngineCycleCount: %lu\n",
                    HkHookPkt.App.EngineCycleCount);
            printf("App.MemInUse: %lu\n", HkHookPkt.App.MemInUse);
            printf("App.PeakMemInUse: %lu\n", HkHookPkt.App.PeakMemInUse);
            printf("App.LowMemoryMark: %lu\n", HkHookPkt.App.LowMemoryMark);
            printf("App.MaxMemNeeded: %lu\n", HkHookPkt.App.MaxMemNeeded);
            printf("App.MemAllocated: %lu\n", HkHookPkt.App.MemAllocated);
            printf("App.BufferPoolHandle: %lu\n",
                    HkHookPkt.App.BufferPoolHandle);
            printf("App.QNodesAllocated: %lu\n",
                    HkHookPkt.App.QNodesAllocated);
            printf("App.QNodesDeallocated: %lu\n",
                    HkHookPkt.App.QNodesDeallocated);
            printf("App.PDUsReceived: %lu\n", HkHookPkt.App.PDUsReceived);
            printf("App.PDUsRejected: %lu\n", HkHookPkt.App.PDUsRejected);
            printf("App.TotalInProgTrans: %lu\n",
                    HkHookPkt.App.TotalInProgTrans);
            printf("App.TotalFailedTrans: %lu\n",
                    HkHookPkt.App.TotalFailedTrans);
            printf("App.TotalAbandonTrans: %lu\n",
                    HkHookPkt.App.TotalAbandonTrans);
            printf("App.TotalSuccessTrans: %lu\n",
                    HkHookPkt.App.TotalSuccessTrans);
            printf("App.TotalCompletedTrans: %lu\n",
                    HkHookPkt.App.TotalCompletedTrans);
            printf("App.LastFailedTrans: %s\n",
                    HkHookPkt.App.LastFailedTrans);

            printf("AutoSuspend.EnFlag: %lu\n",
                    HkHookPkt.AutoSuspend.EnFlag);
            printf("AutoSuspend.LowFreeMark: %lu\n",
                    HkHookPkt.AutoSuspend.LowFreeMark);

            printf("Cond.PosAckNum: %u\n", HkHookPkt.Cond.PosAckNum);
            printf("Cond.FileStoreRejNum: %u\n",
                    HkHookPkt.Cond.FileStoreRejNum);
            printf("Cond.FileChecksumNum: %u\n",
                    HkHookPkt.Cond.FileChecksumNum);
            printf("Cond.FileSizeNum: %u\n", HkHookPkt.Cond.FileSizeNum);
            printf("Cond.NakLimitNum: %u\n", HkHookPkt.Cond.NakLimitNum);
            printf("Cond.InactiveNum: %u\n", HkHookPkt.Cond.InactiveNum);
            printf("Cond.SuspendNum: %u\n", HkHookPkt.Cond.SuspendNum);
            printf("Cond.CancelNum: %u\n", HkHookPkt.Cond.CancelNum);

            printf("Eng.FlightEngineEntityId: %s\n",
                    HkHookPkt.Eng.FlightEngineEntityId);
            printf("Eng.Flags: %lu\n", HkHookPkt.Eng.Flags);
            printf("Eng.MachinesAllocated: %lu\n",
                    HkHookPkt.Eng.MachinesAllocated);
            printf("Eng.MachinesDeallocated: %lu\n",
                    HkHookPkt.Eng.MachinesDeallocated);
            printf("Eng.are_any_partners_frozen: %u\n",
                    HkHookPkt.Eng.are_any_partners_frozen);
            printf("Eng.how_many_senders: %lu\n",
                    HkHookPkt.Eng.how_many_senders);
            printf("Eng.how_many_receivers: %lu\n",
                    HkHookPkt.Eng.how_many_receivers);
            printf("Eng.how_many_frozen: %lu\n",
                    HkHookPkt.Eng.how_many_frozen);
            printf("Eng.how_many_suspended: %lu\n",
                    HkHookPkt.Eng.how_many_suspended);
            printf("Eng.total_files_sent: %lu\n",
                    HkHookPkt.Eng.total_files_sent);
            printf("Eng.total_files_received: %lu\n",
                    HkHookPkt.Eng.total_files_received);
            printf("Eng.total_unsuccessful_senders: %lu\n",
                    HkHookPkt.Eng.total_unsuccessful_senders);
            printf("Eng.total_unsuccessful_receivers: %lu\n",
                    HkHookPkt.Eng.total_unsuccessful_receivers);

            printf("Up.MetaCount: %lu\n", HkHookPkt.Up.MetaCount);
            printf("Up.UplinkActiveQFileCnt: %lu\n",
                    HkHookPkt.Up.UplinkActiveQFileCnt);
            printf("Up.SuccessCounter: %lu\n", HkHookPkt.Up.SuccessCounter);
            printf("Up.FailedCounter: %lu\n", HkHookPkt.Up.FailedCounter);
            printf("Up.LastFileUplinked: %s\n",
                    HkHookPkt.Up.LastFileUplinked);

            for (i = 0; i < CF_MAX_PLAYBACK_CHANNELS; i++)
            {
                printf("Chan[%d].PDUsSent: %lu\n",
                        i, HkHookPkt.Chan[i].PDUsSent);
                printf("Chan[%d].FilesSent: %lu\n",
                        i, HkHookPkt.Chan[i].FilesSent);
                printf("Chan[%d].SuccessCounter: %lu\n",
                        i, HkHookPkt.Chan[i].SuccessCounter);
                printf("Chan[%d].FailedCounter: %lu\n",
                        i, HkHookPkt.Chan[i].FailedCounter);
                printf("Chan[%d].PendingQFileCnt: %lu\n",
                        i, HkHookPkt.Chan[i].PendingQFileCnt);
                printf("Chan[%d].ActiveQFileCnt: %lu\n",
                        i, HkHookPkt.Chan[i].ActiveQFileCnt);
                printf("Chan[%d].HistoryQFileCnt: %lu\n",
                        i, HkHookPkt.Chan[i].HistoryQFileCnt);
                printf("Chan[%d].Flags: %lu\n", i, HkHookPkt.Chan[i].Flags);
                printf("Chan[%d].RedLightCntr: %lu\n",
                        i, HkHookPkt.Chan[i].RedLightCntr);
                printf("Chan[%d].GreenLightCntr: %lu\n",
                        i, HkHookPkt.Chan[i].GreenLightCntr);
                printf("Chan[%d].PollDirsChecked: %lu\n",
                        i, HkHookPkt.Chan[i].PollDirsChecked);
                printf("Chan[%d].PendingQChecked: %lu\n",
                        i, HkHookPkt.Chan[i].PendingQChecked);
                printf("Chan[%d].SemValue: %lu\n",
                        i, HkHookPkt.Chan[i].SemValue);
            }

            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test CF_AppPipe, HousekeepingCmdPbSuspend
 */
void Test_CF_AppPipe_HousekeepingCmdPbSuspend(void)
{
    uint32                QEntryCntActiveBefore;
    uint32                QEntryCntActiveAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_NoArgsCmd_t        HkCmdMsg;
    char  expEventTr[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&HkCmdMsg, CF_SEND_HK_MID, sizeof(HkCmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&HkCmdMsg, sizeof(HkCmdMsg));

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             (void *)&OS_CountSemGetInfoHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    memset((void *)&HkHookPkt, 0x00, sizeof(HkHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
    QEntryCntActiveBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    /* Suspend */
    strcpy(SuspendCmdMsg.Trans, PbFileCmdMsg1.SrcFilename);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActiveAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&HkCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventTr, "%s command received.%s",
            "Suspend", SuspendCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: No ErrCount");

    UtAssert_True((QEntryCntActiveBefore == 2) && (QEntryCntActiveAfter == 2)
                  && (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: QEntryCnt");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEventTr,
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Trans Event Sent");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Sent HK Telemetry");

    UtAssert_True((HkHookPkt.App.QNodesAllocated == 2) &&
                  (HkHookPkt.App.PDUsReceived == 0) &&
                  (HkHookPkt.App.TotalInProgTrans == 2) &&
                  (HkHookPkt.App.TotalSuccessTrans == 0) &&
                  (HkHookPkt.App.TotalCompletedTrans == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Hk.App params");

    UtAssert_True(HkHookPkt.Cond.SuspendNum == 0, /* No failed suspend */
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Hk.Cond param");

    UtAssert_True((CFE_TST(HkHookPkt.Eng.Flags, 0) == FALSE) &&
                  (HkHookPkt.Eng.are_any_partners_frozen == FALSE) &&
                  (HkHookPkt.Eng.how_many_senders == 2) &&
                  (HkHookPkt.Eng.how_many_receivers == 0) &&
                  (HkHookPkt.Eng.how_many_frozen == 0) &&
                  (HkHookPkt.Eng.how_many_suspended == 1) &&
                  (HkHookPkt.Eng.total_files_sent == 0) &&
                  (HkHookPkt.Eng.total_files_received == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_senders == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_receivers == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Hk.Eng params");

    UtAssert_True((HkHookPkt.Up.MetaCount == 0) &&
                  (HkHookPkt.Up.UplinkActiveQFileCnt == 0) &&
                  (HkHookPkt.Up.SuccessCounter == 0) &&
                  (HkHookPkt.Up.FailedCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Hk.Up params");

    UtAssert_True((HkHookPkt.Chan[0].PDUsSent == 0) &&
                  (HkHookPkt.Chan[0].FilesSent == 0) &&
                  (HkHookPkt.Chan[0].SuccessCounter == 0) &&
                  (HkHookPkt.Chan[0].FailedCounter == 0) &&
                  (HkHookPkt.Chan[0].PendingQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].ActiveQFileCnt == 2) &&
                  (HkHookPkt.Chan[0].HistoryQFileCnt == 0) &&
                  /* Channel 0: Dequeue Enable */
                  (CFE_TST(HkHookPkt.Chan[0].Flags, 0) == TRUE) &&
                  (HkHookPkt.Chan[0].SemValue == 10) &&
                  (HkHookPkt.Chan[1].SemValue == 9),
                  "CF_AppPipe, HousekeepingCmdPbSuspend: Hk.Chan[] params");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, HousekeepingCmdPbFreeze
 */
void Test_CF_AppPipe_HousekeepingCmdPbFreeze(void)
{
    uint32                QEntryCntActiveBefore;
    uint32                QEntryCntActiveAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_NoArgsCmd_t        FreezeCmdMsg;
    CF_NoArgsCmd_t        HkCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&FreezeCmdMsg, CF_CMD_MID,
                   sizeof(FreezeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FreezeCmdMsg, (uint16)CF_FREEZE_CC);

    CFE_SB_InitMsg((void*)&HkCmdMsg, CF_SEND_HK_MID, sizeof(HkCmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&HkCmdMsg, sizeof(HkCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             (void *)&OS_CountSemGetInfoHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    memset((void *)&HkHookPkt, 0x00, sizeof(HkHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                       (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
    QEntryCntActiveBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&FreezeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_TstUtil_FinishPbActiveQueueEntries();

    QEntryCntActiveAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&HkCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "Freeze command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: No ErrCount");

    UtAssert_True((QEntryCntActiveBefore == 2) && (QEntryCntActiveAfter == 2)
                  && (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 0),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: QEntryCnt");

    UtAssert_EventSent(CF_FREEZE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Freeze Event Sent");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Sent HK Telemetry");

    UtAssert_True((HkHookPkt.App.QNodesAllocated == 2) &&
                  (HkHookPkt.App.PDUsReceived == 0) &&
                  (HkHookPkt.App.TotalInProgTrans == 2) &&
                  (HkHookPkt.App.TotalSuccessTrans == 0) &&
                  (HkHookPkt.App.TotalCompletedTrans == 0),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Hk.App params");

                  /* frozen */
    UtAssert_True((CFE_TST(HkHookPkt.Eng.Flags, 0) == TRUE) &&
                  (HkHookPkt.Eng.are_any_partners_frozen == TRUE) &&
                  (HkHookPkt.Eng.how_many_senders == 2) &&
                  (HkHookPkt.Eng.how_many_receivers == 0) &&
                  (HkHookPkt.Eng.how_many_frozen == 2) &&
                  (HkHookPkt.Eng.how_many_suspended == 0) &&
                  (HkHookPkt.Eng.total_files_sent == 0) &&
                  (HkHookPkt.Eng.total_files_received == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_senders == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_receivers == 0),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Hk.Eng params");

    UtAssert_True((HkHookPkt.Up.MetaCount == 0) &&
                  (HkHookPkt.Up.UplinkActiveQFileCnt == 0) &&
                  (HkHookPkt.Up.SuccessCounter == 0) &&
                  (HkHookPkt.Up.FailedCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Hk.Up params");

    UtAssert_True((HkHookPkt.Chan[0].PDUsSent == 0) &&
                  (HkHookPkt.Chan[0].FilesSent == 0) &&
                  (HkHookPkt.Chan[0].SuccessCounter == 0) &&
                  (HkHookPkt.Chan[0].FailedCounter == 0) &&
                  (HkHookPkt.Chan[0].PendingQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].ActiveQFileCnt == 2) &&
                  (HkHookPkt.Chan[0].HistoryQFileCnt == 0) &&
                  /* Channel 0: Dequeue Enable */
                  (CFE_TST(HkHookPkt.Chan[0].Flags, 0) == TRUE) &&
                  (HkHookPkt.Chan[0].SemValue == 10) &&
                  (HkHookPkt.Chan[1].SemValue == 9),
                  "CF_AppPipe, HousekeepingCmdPbFreeze: Hk.Chan[] params");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, HousekeepingCmdPbSuccess
 */
void Test_CF_AppPipe_HousekeepingCmdPbSuccess(void)
{
    uint32                QEntryCntActiveBefore;
    uint32                QEntryCntActiveAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_NoArgsCmd_t        HkCmdMsg;
    TRANSACTION           trans;
    char  FullSrcFilename1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFilename2[MAX_FILE_NAME_LENGTH];
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&HkCmdMsg, CF_SEND_HK_MID, sizeof(HkCmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&HkCmdMsg, sizeof(HkCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             (void *)&OS_CountSemGetInfoHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    memset((void *)&HkHookPkt, 0x00, sizeof(HkHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                       (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
    QEntryCntActiveBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_TstUtil_FinishPbActiveQueueEntries();

    QEntryCntActiveAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_ShowQs();
machine_list__display_list();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&HkCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullTransString1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans);
    strcpy(FullSrcFilename1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventTr1, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFilename1);

    sprintf(FullTransString2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans);
    strcpy(FullSrcFilename2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventTr2, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFilename2);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: No ErrCount");

    UtAssert_True((QEntryCntActiveBefore == 2) && (QEntryCntActiveAfter == 0)
                  && (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 2),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: QEntryCnt");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr1,
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Trans1 Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr2,
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Trans2 Event Sent");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Sent HK Telemetry");

    UtAssert_True((HkHookPkt.App.QNodesAllocated == 2) &&
                  (HkHookPkt.App.PDUsReceived == 0) &&
                  (HkHookPkt.App.TotalInProgTrans == 0) &&
                  (HkHookPkt.App.TotalSuccessTrans == 2) &&
                  (HkHookPkt.App.TotalCompletedTrans == 2),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Hk.App params");

    UtAssert_True((CFE_TST(HkHookPkt.Eng.Flags, 0) == FALSE) &&
                  (HkHookPkt.Eng.are_any_partners_frozen == FALSE) &&
                  (HkHookPkt.Eng.how_many_senders == 0) &&
                  (HkHookPkt.Eng.how_many_receivers == 0) &&
                  (HkHookPkt.Eng.how_many_frozen == 0) &&
                  (HkHookPkt.Eng.how_many_suspended == 0) &&
                  (HkHookPkt.Eng.total_files_sent == 2) &&
                  (HkHookPkt.Eng.total_files_received == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_senders == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_receivers == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Hk.Eng params");

    UtAssert_True((HkHookPkt.Up.MetaCount == 0) &&
                  (HkHookPkt.Up.UplinkActiveQFileCnt == 0) &&
                  (HkHookPkt.Up.SuccessCounter == 0) &&
                  (HkHookPkt.Up.FailedCounter == 0),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Hk.Up params");

    UtAssert_True((HkHookPkt.Chan[0].PDUsSent == 6) &&
                  (HkHookPkt.Chan[0].FilesSent == 2) &&
                  (HkHookPkt.Chan[0].SuccessCounter == 2) &&
                  (HkHookPkt.Chan[0].FailedCounter == 0) &&
                  (HkHookPkt.Chan[0].PendingQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].ActiveQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].HistoryQFileCnt == 2) &&
                  /* Channel 0: Dequeue Enable */
                  (CFE_TST(HkHookPkt.Chan[0].Flags, 0) == TRUE) &&
                  (HkHookPkt.Chan[0].SemValue == 10) &&
                  (HkHookPkt.Chan[1].SemValue == 9),
                  "CF_AppPipe, HousekeepingCmdPbSuccess: Hk.Chan[] params");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, HousekeepingCmdUpFreezeWarn
 */
void Test_CF_AppPipe_HousekeepingCmdUpFreezeWarn(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntActBefore;
    uint32                QEntryCntActAfter;
    CF_Test_InPDUInfo_t   InPDUInfo1;
    CF_Test_InPDUInfo_t   InPDUInfo2;
    CF_NoArgsCmd_t        FreezeCmdMsg;
    CF_NoArgsCmd_t        HkCmdMsg;
    CF_Test_InPDUMsg_t    InMdPDUMsg1;
    CF_Test_InPDUMsg_t    InMdPDUMsg2;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build Freeze Command */
    CFE_SB_InitMsg((void*)&FreezeCmdMsg, CF_CMD_MID,
                   sizeof(FreezeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FreezeCmdMsg, (uint16)CF_FREEZE_CC);

    /* Build Hk Command */
    CFE_SB_InitMsg((void*)&HkCmdMsg, CF_SEND_HK_MID,
                   sizeof(HkCmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&HkCmdMsg, sizeof(HkCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             (void *)&OS_CountSemGetInfoHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    memset((void *)&HkHookPkt, 0x00, sizeof(HkHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                       (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* Incoming MD PDU Msg1 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming MD PDU Msg2 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);
    QEntryCntActBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* Freeze */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&FreezeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* Send Hk */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&HkCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullTransString1, "%d.%d_%d",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number);
    sprintf(expEventTr1, "cfdp_engine: ignored event '%s' in state S2 "
            "for trans '%s' <R1>.", "received Freeze Request",
            FullTransString1);

    sprintf(FullTransString2, "%d.%d_%d",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number);
    sprintf(expEventTr2, "cfdp_engine: ignored event '%s' in state S2 "
            "for trans '%s' <R1>.", "received Freeze Request",
            FullTransString2);

    sprintf(expEvent, "%s", "Freeze command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn");

    UtAssert_True((QEntryCntActBefore == 2) && (QEntryCntActAfter == 2),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: QEntryCnt");

    UtAssert_EventSent(CF_CFDP_ENGINE_WARN_EID, CFE_EVS_INFORMATION,
             expEventTr1,
             "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Trans1 Event Sent");

    UtAssert_EventSent(CF_CFDP_ENGINE_WARN_EID, CFE_EVS_INFORMATION,
             expEventTr2,
             "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Trans2 Event Sent");

    UtAssert_EventSent(CF_FREEZE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Freeze Event Sent");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
             "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Sent HK Telemetry");

    UtAssert_True((HkHookPkt.App.QNodesAllocated == 2) &&
                  (HkHookPkt.App.PDUsReceived == 2) &&
                  (HkHookPkt.App.PDUsRejected == 0) &&
                  (HkHookPkt.App.TotalInProgTrans == 2) &&
                  (HkHookPkt.App.TotalSuccessTrans == 0) &&
                  (HkHookPkt.App.TotalCompletedTrans == 0),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Hk.App params");

                  /* partners are frozen */
    UtAssert_True((CFE_TST(HkHookPkt.Eng.Flags, 0) == TRUE) &&
                  (HkHookPkt.Eng.are_any_partners_frozen == TRUE) &&
                  (HkHookPkt.Eng.how_many_senders == 0) &&
                  (HkHookPkt.Eng.how_many_receivers == 2) &&
                  /* UpQueue transactions frozen cnt */
                  (HkHookPkt.Eng.how_many_frozen == 0) &&
                  (HkHookPkt.Eng.how_many_suspended == 0) &&
                  (HkHookPkt.Eng.total_files_sent == 0) &&
                  (HkHookPkt.Eng.total_files_received == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_senders == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_receivers == 0),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Hk.Eng params");

    UtAssert_True((HkHookPkt.Up.MetaCount == 2) &&
                  (HkHookPkt.Up.UplinkActiveQFileCnt == 2) &&
                  (HkHookPkt.Up.SuccessCounter == 0) &&
                  (HkHookPkt.Up.FailedCounter == 0),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Hk.Up params");

    UtAssert_True((HkHookPkt.Chan[0].PDUsSent == 0) &&
                  (HkHookPkt.Chan[0].FilesSent == 0) &&
                  (HkHookPkt.Chan[0].SuccessCounter == 0) &&
                  (HkHookPkt.Chan[0].FailedCounter == 0) &&
                  (HkHookPkt.Chan[0].PendingQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].ActiveQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].HistoryQFileCnt == 0) &&
                  /* Channel 0: Dequeue Enable */
                  (CFE_TST(HkHookPkt.Chan[0].Flags, 0) == TRUE) &&
                  (HkHookPkt.Chan[0].SemValue == 10) &&
                  (HkHookPkt.Chan[1].SemValue == 9),
                  "CF_AppPipe, HousekeepingCmdUpFreezeWarn: Hk.Chan[] params");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, HousekeepingCmdUpSuccess
 */
void Test_CF_AppPipe_HousekeepingCmdUpSuccess(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_Test_InPDUInfo_t   InPDUInfo1;
    CF_Test_InPDUInfo_t   InPDUInfo2;
    CF_NoArgsCmd_t        HkCmdMsg;
    CF_Test_InPDUMsg_t    InMdPDUMsg1;
    CF_Test_InPDUMsg_t    InFdPDUMsg1;
    CF_Test_InPDUMsg_t    InEofPDUMsg1;
    CF_Test_InPDUMsg_t    InMdPDUMsg2;
    CF_Test_InPDUMsg_t    InFdPDUMsg2;
    CF_Test_InPDUMsg_t    InEofPDUMsg2;
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DATA_PDU;
    InPDUInfo1.offset = 0;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo1.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg1, &InPDUInfo1, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.cond_code = NO_ERROR;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    InPDUInfo1.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg1, &InPDUInfo1, hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DATA_PDU;
    InPDUInfo2.offset = 0;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo2.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.cond_code = NO_ERROR;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    InPDUInfo2.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build Hk Msg */
    CFE_SB_InitMsg((void*)&HkCmdMsg, CF_SEND_HK_MID,
                   sizeof(HkCmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&HkCmdMsg, sizeof(HkCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETINFO_INDEX,
                             (void *)&OS_CountSemGetInfoHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

#if 0
    /* Return the read bytes */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);
#endif

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    memset((void *)&HkHookPkt, 0x00, sizeof(HkHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                       (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    QEntryCntHistBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Send File1 MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send File1 FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send File1 EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send File2 MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send File2 FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send File2 EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntHistAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Send Hk */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&HkCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventTr1, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, InPDUInfo1.dst_filename);

    sprintf(expEventTr2, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, InPDUInfo2.dst_filename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, HousekeepingCmdUpSuccess");

    UtAssert_True((QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 2),
                  "CF_AppPipe, HousekeepingCmdUpSuccess: QEntryCnt");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr1,
             "CF_AppPipe, HousekeepingCmdUpSuccess: Trans1 Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr2,
             "CF_AppPipe, HousekeepingCmdUpSuccess: Trans2 Event Sent");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
             "CF_AppPipe, HousekeepingCmdUpSuccess: Sent HK Telemetry");

    UtAssert_True((HkHookPkt.App.QNodesAllocated == 2) &&
                  (HkHookPkt.App.PDUsReceived == 6) &&
                  (HkHookPkt.App.TotalInProgTrans == 0) &&
                  (HkHookPkt.App.TotalSuccessTrans == 2) &&
                  (HkHookPkt.App.TotalCompletedTrans == 2),
                  "CF_AppPipe, HousekeepingCmdUpSuccess: Hk.App params");

                  /* Not Frozen */
    UtAssert_True((CFE_TST(HkHookPkt.Eng.Flags, 0) == FALSE) &&
                  (HkHookPkt.Eng.are_any_partners_frozen == FALSE) &&
                  (HkHookPkt.Eng.how_many_senders == 0) &&
                  (HkHookPkt.Eng.how_many_receivers == 0) &&
                  (HkHookPkt.Eng.how_many_frozen == 0) &&
                  (HkHookPkt.Eng.how_many_suspended == 0) &&
                  (HkHookPkt.Eng.total_files_sent == 0) &&
                  (HkHookPkt.Eng.total_files_received == 2) &&
                  (HkHookPkt.Eng.total_unsuccessful_senders == 0) &&
                  (HkHookPkt.Eng.total_unsuccessful_receivers == 0),
                  "CF_AppPipe, HousekeepingCmdUpSuccess: Hk.Eng params");

    UtAssert_True((HkHookPkt.Up.MetaCount == 2) &&
                  (HkHookPkt.Up.UplinkActiveQFileCnt == 0) &&
                  (HkHookPkt.Up.SuccessCounter == 2) &&
                  (HkHookPkt.Up.FailedCounter == 0),
                  "CF_AppPipe, HousekeepingCmdUpSuccess: Hk.Up params");

    UtAssert_True((HkHookPkt.Chan[0].PDUsSent == 0) &&
                  (HkHookPkt.Chan[0].FilesSent == 0) &&
                  (HkHookPkt.Chan[0].SuccessCounter == 0) &&
                  (HkHookPkt.Chan[0].FailedCounter == 0) &&
                  (HkHookPkt.Chan[0].PendingQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].ActiveQFileCnt == 0) &&
                  (HkHookPkt.Chan[0].HistoryQFileCnt == 0) &&
                  /* Channel 0: Dequeue Enable */
                  (CFE_TST(HkHookPkt.Chan[0].Flags, 0) == TRUE) &&
                  (HkHookPkt.Chan[0].SemValue == 10) &&
                  (HkHookPkt.Chan[1].SemValue == 9),
                  "CF_AppPipe, HousekeepingCmdUpSuccess: Hk.Chan[] params");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, HousekeepingCmdInvLen
 */
void Test_CF_AppPipe_HousekeepingCmdInvLen(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_SEND_HK_MID, sizeof(CmdMsg) + 5, TRUE);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_SEND_HK_MID, 0,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, HousekeepingCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, HousekeepingCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, HousekeepingCmdValPending
 */
void Test_CF_AppPipe_HousekeepingCmdValPending(void)
{
    CF_NoArgsCmd_t  CmdMsg;

    CFE_SB_InitMsg((void*)&CmdMsg, CF_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* to get coverage in CF_CheckForTblRequests */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX,
                             CFE_TBL_INFO_VALIDATION_PENDING, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Verify results */
}


/**
 * Test CF_AppPipe, HousekeepingCmdUpdatePending
 */
void Test_CF_AppPipe_HousekeepingCmdUpdatePending(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* to get coverage in CF_CheckForTblRequests */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX,
                             CFE_TBL_INFO_UPDATE_PENDING, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "CF Config Tbl cannot be updated! "
            "Load attempt must be aborted!");

    /* Verify results */
    UtAssert_EventSent(CF_TBL_LD_ATTEMPT_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, HousekeepingCmdUpdatePending: Event Sent");
}


/**
 * Test CF_AppPipe, FreezeCmd
 */
void Test_CF_AppPipe_FreezeCmd(void)
{
    boolean               bPartnersFrozen;
    uint32                QEntryCnt;
    uint32                FrozenCnt;
    SUMMARY_STATUS        EngStat;
    CF_NoArgsCmd_t        FreezeCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&FreezeCmdMsg, CF_CMD_MID,
                   sizeof(FreezeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FreezeCmdMsg, (uint16)CF_FREEZE_CC);
    CF_Test_PrintCmdMsg((void*)&FreezeCmdMsg, sizeof(FreezeCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&FreezeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    EngStat = cfdp_summary_status();
    FrozenCnt = EngStat.how_many_frozen;
    bPartnersFrozen = EngStat.are_any_partners_frozen;

    sprintf(expEvent, "%s", "Freeze command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, FreezeCmd");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, FreezeCmd: QEntryCnt");

    UtAssert_True(bPartnersFrozen == YES,
                  "CF_AppPipe, FreezeCmd: Partners Frozen");

    UtAssert_True(CFE_TST(CF_AppData.Hk.Eng.Flags, 0) == TRUE,
                  "CF_AppPipe, FreezeCmd: Frozen Flag set");

    UtAssert_True(FrozenCnt == 2,
                  "CF_AppPipe, FreezeCmd: Frozen Count");

    UtAssert_EventSent(CF_FREEZE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, FreezeCmd: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, FreezeCmdInvLen
 */
void Test_CF_AppPipe_FreezeCmdInvLen(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_FREEZE_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_FREEZE_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, FreezeCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, FreezeCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ThawCmd
 */
void Test_CF_AppPipe_ThawCmd(void)
{
    boolean               bPartnersFrozenBefore;
    boolean               bPartnersFrozenAfter;
    uint32                QEntryCnt;
    uint32                FrozenCnt;
    uint32                ThawedCnt;
    SUMMARY_STATUS        EngStat;
    CF_NoArgsCmd_t        FreezeCmdMsg;
    CF_NoArgsCmd_t        ThawCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&FreezeCmdMsg, CF_CMD_MID,
                   sizeof(FreezeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FreezeCmdMsg, (uint16)CF_FREEZE_CC);

    CFE_SB_InitMsg((void*)&ThawCmdMsg, CF_CMD_MID, sizeof(ThawCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ThawCmdMsg, (uint16)CF_THAW_CC);
    CF_Test_PrintCmdMsg((void*)&ThawCmdMsg, sizeof(ThawCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&FreezeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    EngStat = cfdp_summary_status();
    FrozenCnt = EngStat.how_many_frozen;
    bPartnersFrozenBefore = EngStat.are_any_partners_frozen;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&ThawCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    EngStat = cfdp_summary_status();
    ThawedCnt = FrozenCnt - EngStat.how_many_frozen;
    bPartnersFrozenAfter = EngStat.are_any_partners_frozen;

    sprintf(expEvent, "%s", "Thaw command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 4) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, ThawCmd");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, ThawCmd: QEntryCnt");

    UtAssert_True((bPartnersFrozenBefore == YES) &&
                  (bPartnersFrozenAfter == NO),
                  "CF_AppPipe, ThawCmd: PartnersFrozen");

    UtAssert_True(CFE_TST(CF_AppData.Hk.Eng.Flags, 0) == FALSE,
                  "CF_AppPipe, ThawCmd: Frozen Flag cleared");

    UtAssert_True((FrozenCnt == 2) && (ThawedCnt == 2),
                  "CF_AppPipe, ThawCmd: Frozen/Thawed Cnt");

    UtAssert_EventSent(CF_THAW_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, ThawCmd: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ThawCmdInvLen
 */
void Test_CF_AppPipe_ThawCmdInvLen(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_THAW_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_THAW_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, ThawCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, ThawCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdTransId
 */
void Test_CF_AppPipe_SuspendCmdTransId(void)
{
    uint32                QEntryCnt;
    uint32                SuspendedCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_CARSCmd_t          SuspendCmdMsg;
    TRANSACTION           trans;
    SUMMARY_STATUS        EngStat;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEventInd[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    strcpy(SuspendCmdMsg.Trans, CF_AppData.Tbl->FlightEntityId);
    strcat(SuspendCmdMsg.Trans, "_1");
    CF_Test_PrintCmdMsg((void*)&SuspendCmdMsg, sizeof(SuspendCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    cfdp_trans_from_string(SuspendCmdMsg.Trans, &trans);
    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventInd, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName);

    sprintf(expEvent, "%s command received.%s", "Suspend",
            SuspendCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SuspendCmdTransId");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, SuspendCmdTransId: QEntryCnt");

    UtAssert_True(SuspendedCnt == 1,
                  "CF_AppPipe, SuspendCmdTransId: Suspended cnt");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION, expEventInd,
                  "CF_AppPipe, SuspendCmdTransId: Ind Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SuspendCmdTransId: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdTransIdInvLen
 */
void Test_CF_AppPipe_SuspendCmdTransIdInvLen(void)
{
    CF_CARSCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_SUSPEND_CC);

    /* Execute the function being tested */
    CF_AppInit();

    strcpy(CmdMsg.Trans, CF_AppData.Tbl->FlightEntityId);
    strcat(CmdMsg.Trans, "_1");
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_SUSPEND_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SuspendCmdTransIdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SuspendCmdTransIdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdFilename
 */
void Test_CF_AppPipe_SuspendCmdFilename(void)
{
    uint32                QEntryCnt;
    uint32                SuspendedCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_CARSCmd_t          SuspendCmdMsg;
    TRANSACTION           trans;
    SUMMARY_STATUS        EngStat;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEventInd[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    /* Suspend */
    strcpy(SuspendCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&SuspendCmdMsg, sizeof(SuspendCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    QEntryCnt =
         CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    sprintf(FullTransString, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString, &trans);
    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventInd, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName);

    sprintf(expEvent, "%s command received.%s", "Suspend",
            SuspendCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SuspendCmdFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, SuspendCmdFilename: QEntryCnt");

    UtAssert_True(SuspendedCnt == 1,
                  "CF_AppPipe, SuspendCmdFilename: Suspended cnt");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION, expEventInd,
                  "CF_AppPipe, SuspendCmdFilename: Ind Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SuspendCmdFilename: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdInvFilename
 */
void Test_CF_AppPipe_SuspendCmdInvFilename(void)
{
    CF_CARSCmd_t          SuspendCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    /* Execute the function being tested */
    CF_AppInit();

    /* Suspend */
    strcpy(SuspendCmdMsg.Trans, CF_AppData.Tbl->OuCh[0].PollDir[0].SrcPath);
    strcat(SuspendCmdMsg.Trans, " file.txt");
    CF_Test_PrintCmdMsg((void*)&SuspendCmdMsg, sizeof(SuspendCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Filename in %s must be terminated and have no spaces",
            "Suspend Cmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SuspendCmdInvFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SuspendCmdInvFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdUntermTrans
 */
void Test_CF_AppPipe_SuspendCmdUntermTrans(void)
{
    CF_CARSCmd_t          SuspendCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);
    CFE_PSP_MemSet(SuspendCmdMsg.Trans, 0xFF, MAX_FILE_NAME_LENGTH);

    CF_Test_PrintCmdMsg((void*)&SuspendCmdMsg, sizeof(SuspendCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unterminated string found in %s", "Suspend Cmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SuspendCmdUntermTrans");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SuspendCmdUntermTrans: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SuspendCmdAll
 */
void Test_CF_AppPipe_SuspendCmdAll(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntBefore;
    uint32                QEntryCntAfter;
    uint32                SuspendedCnt;
    TRANSACTION           trans;
    SUMMARY_STATUS        EngStat;
    CF_Test_InPDUInfo_t   InPDUInfo;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_Test_InPDUMsg_t    InMdPDUMsg;
    CF_Test_InPDUMsg_t    InFdPDUMsg;
    CF_Test_InPDUMsg_t    InEofPDUMsg;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName2[MAX_FILE_NAME_LENGTH];
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DATA_PDU;
    InPDUInfo.offset = 0;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.cond_code = NO_ERROR;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    InPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InPDUInfo, hdr_len);

    /* Build Suspend Command Msg */
    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);
    strcpy(SuspendCmdMsg.Trans, "All");

    CF_Test_PrintCmdMsg((void*)&SuspendCmdMsg, sizeof(SuspendCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt
       + CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Suspend */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt
       + CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    CF_ShowQs();
machine_list__display_list();

    sprintf(FullTransString1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans);
    strcpy(FullSrcFileName1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventTr1, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName1);

    sprintf(FullTransString2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans);
    strcpy(FullSrcFileName2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventTr2, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName2);

    sprintf(expEvent, "%s command received.%s", "Suspend", "All");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SuspendCmdAll");

    UtAssert_True((QEntryCntBefore == 3) && (QEntryCntAfter == 3),
                  "CF_AppPipe, SuspendCmdAll: QEntryCnt");

    UtAssert_True(SuspendedCnt == 2,
                  "CF_AppPipe, SuspendCmdAll: Suspended cnt");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION, expEventTr1,
                       "CF_AppPipe, SuspendCmdAll: Trans #1 Event Sent");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION, expEventTr2,
                       "CF_AppPipe, SuspendCmdAll: Trans #2 Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SuspendCmdAll: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ResumeCmdNoTransId
 */
void Test_CF_AppPipe_ResumeCmdNoTransId(void)
{
    uint32                QEntryCnt;
    uint32                SuspendedCnt;
    uint32                ResumedCnt;
    TRANSACTION           trans;
    SUMMARY_STATUS        EngStat;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_CARSCmd_t          ResumeCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventSuspend[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventResume[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    CFE_SB_InitMsg((void*)&ResumeCmdMsg, CF_CMD_MID,
                   sizeof(ResumeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ResumeCmdMsg, (uint16)CF_RESUME_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    sprintf(SuspendCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    sprintf(ResumeCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_50");
    CF_Test_PrintCmdMsg((void*)&ResumeCmdMsg, sizeof(ResumeCmdMsg));

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    /* Suspend */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    /* Resume */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&ResumeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    EngStat = cfdp_summary_status();
    ResumedCnt = SuspendedCnt - EngStat.how_many_suspended;

    cfdp_trans_from_string(SuspendCmdMsg.Trans, &trans);
    sprintf(expEventSuspend, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, PbFileCmdMsg.SrcFilename);

    strcpy(FullTransString, ResumeCmdMsg.Trans);
    sprintf(expEventResume, "cfdp_engine: ignoring User-Request that "
            "references unknown transaction (%s).", FullTransString);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, ResumeCmdNoTransId");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, ResumeCmdNoTransId: QEntryCnt");

    UtAssert_True((SuspendedCnt == 1) && (ResumedCnt == 0),
                  "CF_AppPipe, ResumeCmdNoTransId: Suspended/Resumed Cnt");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION,
                       expEventSuspend,
                       "CF_AppPipe, ResumeCmdNoTransId: Suspend Event Sent");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventResume,
                  "CF_AppPipe, ResumeCmdNoTransId: Resume Err Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ResumeCmdPbFilename
 */
void Test_CF_AppPipe_ResumeCmdPbFilename(void)
{
    uint32                QEntryCnt;
    uint32                SuspendedCnt;
    uint32                ResumedCnt;
    TRANSACTION           trans;
    SUMMARY_STATUS        EngStat;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_CARSCmd_t          ResumeCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventSuspend[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventResume[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build Suspend Command Msg */
    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    /* Build Resume Command Msg */
    CFE_SB_InitMsg((void*)&ResumeCmdMsg, CF_CMD_MID,
                   sizeof(ResumeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ResumeCmdMsg,
                      (uint16)CF_RESUME_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    /* Suspend */
    strcpy(SuspendCmdMsg.Trans, PbFileCmdMsg.SrcFilename);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    /* Resume */
    strcpy(ResumeCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&ResumeCmdMsg, sizeof(ResumeCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&ResumeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    /* get engine status */
    EngStat = cfdp_summary_status();
    ResumedCnt = SuspendedCnt - EngStat.how_many_suspended;

    sprintf(FullTransString, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString, &trans);
    sprintf(expEventSuspend, "Transaction Susupended %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SuspendCmdMsg.Trans);

    sprintf(expEventResume, "Transaction Resumed %d.%d_%d,%s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, ResumeCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, ResumeCmdPbFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, ResumeCmdPbFilename: QEntryCnt");

    UtAssert_True((SuspendedCnt == 1) && (ResumedCnt == 1),
                  "CF_AppPipe, ResumeCmdPbFilename: Suspended/Resumed cnt");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION,
                       expEventSuspend,
                       "CF_AppPipe, ResumeCmdPbFilename: Suspend Event Sent");

    UtAssert_EventSent(CF_IND_XACT_RES_EID, CFE_EVS_INFORMATION,
                       expEventResume,
                       "CF_AppPipe, ResumeCmdPbFilename: Resume Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ResumeCmdUpTransIdIgnore
 */
void Test_CF_AppPipe_ResumeCmdUpTransIdIgnore(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              QEntryCntBefore;
    uint32              QEntryCntAfter;
    uint32              SuspendedCnt;
    uint32              ResumedCnt;
    SUMMARY_STATUS      EngStat;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_CARSCmd_t        SuspendCmdMsg;
    CF_CARSCmd_t        ResumeCmdMsg;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventCfdp1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventCfdp2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventSuspend[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventResume[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build SuspendCmdMsg */
    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);
    sprintf(SuspendCmdMsg.Trans, "%d.%d_%d",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number);

    /* Build ResumeCmdMsg */
    CFE_SB_InitMsg((void*)&ResumeCmdMsg, CF_CMD_MID,
                   sizeof(ResumeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ResumeCmdMsg, (uint16)CF_RESUME_CC);
    strcpy(ResumeCmdMsg.Trans, SuspendCmdMsg.Trans);
    CF_Test_PrintCmdMsg((void*)&ResumeCmdMsg, sizeof(ResumeCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* Suspend */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    /* Resume */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&ResumeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    CF_ShowQs();
machine_list__display_list();

    /* get engine status */
    EngStat = cfdp_summary_status();
    ResumedCnt = SuspendedCnt - EngStat.how_many_suspended;

    strcpy(FullTransString, SuspendCmdMsg.Trans);
    sprintf(expEventCfdp1, "cfdp_engine: ignored event '%s' in state S2 "
            "for trans '%s' <R1>.",
            "received Suspend Request", FullTransString);

    sprintf(expEventCfdp2, "cfdp_engine: ignored event '%s' in state S2 "
            "for trans '%s' <R1>.",
            "received Resume Request", FullTransString);

    sprintf(expEventSuspend, "%s command received.%s", "Suspend",
            FullTransString);

    sprintf(expEventResume, "%s command received.%s", "Resume",
            FullTransString);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, ResumeCmdUpTransIdIgnore");

    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 1) &&
                  (CF_AppData.Hk.App.PDUsRejected == 0),
                  "CF_AppPipe, ResumeCmdUpTransIdIgnore: PDUsReceived cnt");

    UtAssert_True(CF_AppData.Hk.Up.MetaCount == 1,
                  "CF_AppPipe, ResumeCmdUpTransIdIgnore: Up.MetaCount");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 1),
                  "CF_AppPipe, ResumeCmdUpTransIdIgnore: QEntryCnt");

    UtAssert_True((SuspendedCnt == 0) && (ResumedCnt == 0),
             "CF_AppPipe, ResumeCmdUpTransIdIgnore: Suspended/Resumed cnt");

    UtAssert_EventSent(CF_CFDP_ENGINE_WARN_EID, CFE_EVS_INFORMATION,
             expEventCfdp1,
             "CF_AppPipe, ResumeCmdUpTransIdIgnore: Ignore 1 Event Sent");

    UtAssert_EventSent(CF_CFDP_ENGINE_WARN_EID, CFE_EVS_INFORMATION,
             expEventCfdp2,
             "CF_AppPipe, ResumeCmdUpTransIdIgnore: Ignore 2 Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEventSuspend,
             "CF_AppPipe, ResumeCmdUpTransIdIgnore: Suspend Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEventResume,
             "CF_AppPipe, ResumeCmdUpTransIdIgnore: Resume Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, ResumeCmdAll
 */
void Test_CF_AppPipe_ResumeCmdAll(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntBefore;
    uint32                QEntryCntAfter;
    uint32                SuspendedCnt;
    uint32                ResumedCnt;
    TRANSACTION           trans1;
    TRANSACTION           trans2;
    SUMMARY_STATUS        EngStat;
    CF_Test_InPDUInfo_t   InPDUInfo;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_CARSCmd_t          ResumeCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_Test_InPDUMsg_t    InMdPDUMsg;
    CF_Test_InPDUMsg_t    InFdPDUMsg;
    CF_Test_InPDUMsg_t    InEofPDUMsg;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  expEventSuspend1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventSuspend2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventResume1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventResume2[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DATA_PDU;
    InPDUInfo.offset = 0;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.cond_code = NO_ERROR;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    InPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InPDUInfo, hdr_len);

    /* Build SuspendCmdMsg */
    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);
    strcpy(SuspendCmdMsg.Trans, "All");

    /* Build ResumeCmdMsg */
    CFE_SB_InitMsg((void*)&ResumeCmdMsg, CF_CMD_MID,
                   sizeof(ResumeCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&ResumeCmdMsg, (uint16)CF_RESUME_CC);
    strcpy(ResumeCmdMsg.Trans, "All");
    CF_Test_PrintCmdMsg((void*)&ResumeCmdMsg, sizeof(ResumeCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* Create two playback chan 0, active queue entries */
    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt
        + CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Suspend */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* get engine status */
    EngStat = cfdp_summary_status();
    SuspendedCnt = EngStat.how_many_suspended;

    /* Resume */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&ResumeCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* get engine status */
    EngStat = cfdp_summary_status();
    ResumedCnt = SuspendedCnt - EngStat.how_many_suspended;

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt
        + CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    sprintf(FullTransString1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans1);
    sprintf(expEventSuspend1, "Transaction Susupended %d.%d_%d,%s",
            trans1.source_id.value[0], trans1.source_id.value[1],
            (int)trans1.number, PbFileCmdMsg1.SrcFilename);

    sprintf(FullTransString2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans2);
    sprintf(expEventSuspend2, "Transaction Susupended %d.%d_%d,%s",
            trans2.source_id.value[0], trans2.source_id.value[1],
            (int)trans2.number, PbFileCmdMsg2.SrcFilename);

    sprintf(expEventResume1, "Transaction Resumed %d.%d_%d,%s",
            trans1.source_id.value[0], trans1.source_id.value[1],
            (int)trans1.number, PbFileCmdMsg1.SrcFilename);

    sprintf(expEventResume2, "Transaction Resumed %d.%d_%d,%s",
            trans2.source_id.value[0], trans2.source_id.value[1],
            (int)trans2.number, PbFileCmdMsg2.SrcFilename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 4) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, ResumeCmdAll");

    UtAssert_True((QEntryCntBefore == 3) && (QEntryCntAfter == 3),
                  "CF_AppPipe, ResumeCmdAll: QEntryCnt");

    UtAssert_True((SuspendedCnt == 2) && (ResumedCnt == 2),
                  "CF_AppPipe, ResumeCmdAll: Suspended/Resumed count");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION,
                       expEventSuspend1,
                       "CF_AppPipe, ResumeCmdAll: File1 Suspend Event Sent");

    UtAssert_EventSent(CF_IND_XACT_SUS_EID, CFE_EVS_INFORMATION,
                       expEventSuspend2,
                       "CF_AppPipe, ResumeCmdAll: File2 Suspend Event Sent");

    UtAssert_EventSent(CF_IND_XACT_RES_EID, CFE_EVS_INFORMATION,
                       expEventResume1,
                       "CF_AppPipe, ResumeCmdAll: File1 Resume Event Sent");

    UtAssert_EventSent(CF_IND_XACT_RES_EID, CFE_EVS_INFORMATION,
                       expEventResume2,
                       "CF_AppPipe, ResumeCmdAll: File2 Resume Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, CancelCmdNoTransId
 */
void Test_CF_AppPipe_CancelCmdNoTransId(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              QEntryCnt;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_CARSCmd_t        CancelCmdMsg;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Cancel Command */
    CFE_SB_InitMsg((void*)&CancelCmdMsg, CF_CMD_MID,
                   sizeof(CancelCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CancelCmdMsg, (uint16)CF_CANCEL_CC);
    sprintf(CancelCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER + 1);
    CF_Test_PrintCmdMsg((void*)&CancelCmdMsg, sizeof(CancelCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Cancel */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CancelCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    strcpy(FullTransString, CancelCmdMsg.Trans);
    sprintf(expEventCfdp, "cfdp_engine: ignoring User-Request that "
            "references unknown transaction (%s).", FullTransString);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, CancelCmd");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, CancelCmd: QEntry Cnt");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
                       "CF_AppPipe, CancelCmd: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, CancelCmdAllPb
 */
void Test_CF_AppPipe_CancelCmdAllPb(void)
{
    uint32                QEntryCntActBefore;
    uint32                QEntryCntActAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    TRANSACTION           trans1;
    TRANSACTION           trans2;
    TRANS_STATUS          trans_status1;
    TRANS_STATUS          trans_status2;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_CARSCmd_t          CancelCmdMsg;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build Cancel Command */
    CFE_SB_InitMsg((void*)&CancelCmdMsg, CF_CMD_MID,
                   sizeof(CancelCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CancelCmdMsg, (uint16)CF_CANCEL_CC);
    strcpy(CancelCmdMsg.Trans, "All");
    CF_Test_PrintCmdMsg((void*)&CancelCmdMsg, sizeof(CancelCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* create two playback chan 0, active queue entries */
    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCntActBefore =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CancelCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActAfter =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    sprintf(FullTransString1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans1);
    cfdp_transaction_status(trans1, &trans_status1);

    sprintf(FullTransString2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans2);
    cfdp_transaction_status(trans2, &trans_status2);

    sprintf(expEvent, "%s command received.%s", "Cancel", CancelCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, CancelCmdAllPb");

    UtAssert_True((QEntryCntActBefore == 2) && (QEntryCntActAfter == 2) &&
                  (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 0),
                  "CF_AppPipe, CancelCmdAllPb: QEntryCnt");

    UtAssert_True(trans_status1.cancelled == TRUE,
                  "CF_AppPipe, CancelCmdAllPb: File1 Cancelled");

    UtAssert_True(trans_status2.cancelled == TRUE,
                  "CF_AppPipe, CancelCmdAllPb: File2 Cancelled");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, CancelCmdAllPb: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, CancelCmdAllUp
 */
void Test_CF_AppPipe_CancelCmdAllUp(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntActBefore;
    uint32                QEntryCntActAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_Test_InPDUInfo_t   InPDUInfo1;
    CF_Test_InPDUInfo_t   InPDUInfo2;
    CF_CARSCmd_t          CancelCmdMsg;
    CF_Test_InPDUMsg_t    InMdPDUMsg1;
    CF_Test_InPDUMsg_t    InMdPDUMsg2;
    char  FullDstFilename1[MAX_FILE_NAME_LENGTH];
    char  FullDstFilename2[MAX_FILE_NAME_LENGTH];
    char  expEventInd1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInd2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build Cancel Command */
    CFE_SB_InitMsg((void*)&CancelCmdMsg, CF_CMD_MID,
                   sizeof(CancelCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CancelCmdMsg, (uint16)CF_CANCEL_CC);
    strcpy(CancelCmdMsg.Trans, "All");
    CF_Test_PrintCmdMsg((void*)&CancelCmdMsg, sizeof(CancelCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg1 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming MD PDU Msg2 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Cancel Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CancelCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    strcpy(FullDstFilename1, InPDUInfo1.dst_filename);
    sprintf(expEventInd1, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, "CANCELLED", "CANCEL_REQ_RCVD",
            FullDstFilename1);

    strcpy(FullDstFilename2, InPDUInfo2.dst_filename);
    sprintf(expEventInd2, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, "CANCELLED", "CANCEL_REQ_RCVD",
            FullDstFilename2);

    sprintf(expEvent, "%s command received.%s", "Cancel",
            CancelCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, CancelCmdAllUp");

    UtAssert_True((QEntryCntActBefore == 2) && (QEntryCntActAfter == 0) &&
                  (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 2),
                  "CF_AppPipe, CancelCmdAllUp: QEntryCnt");

    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 2) &&
                  (CF_AppData.Hk.App.PDUsRejected == 0),
                  "CF_AppPipe, CancelCmdAllUp: PDUsReceived cnt");

    UtAssert_True(CF_AppData.Hk.Up.MetaCount == 2,
                  "CF_AppPipe, CancelCmdAllUp: Up.MetaCount");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd1,
                       "CF_AppPipe, CancelCmdAllUp: Up1 Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd2,
                       "CF_AppPipe, CancelCmdAllUp: Up2 Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, CancelCmdAllUp: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, AbandonCmdNoFile
 */
void Test_CF_AppPipe_AbandonCmdNoFile(void)
{
    uint32                QEntryCnt;
    CF_CARSCmd_t          AbandonCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&AbandonCmdMsg, CF_CMD_MID,
                   sizeof(AbandonCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AbandonCmdMsg, (uint16)CF_ABANDON_CC);
    sprintf(AbandonCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile1);
    CF_Test_PrintCmdMsg((void*)&AbandonCmdMsg, sizeof(AbandonCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    /* Abandon */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AbandonCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(expEvent, "%s Cmd Error,File %s Not Active",
            "Abandon", AbandonCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, AbandonCmdNoFile");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, AbandonCmdNoFile: QEntry Cnt");

    UtAssert_EventSent(CF_CARS_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, AbandonCmdNoFile: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, AbandonCmdNoTransId
 */
void Test_CF_AppPipe_AbandonCmdNoTransId(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              QEntryCnt;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_CARSCmd_t        AbandonCmdMsg;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Abandon Command Msg */
    CFE_SB_InitMsg((void*)&AbandonCmdMsg, CF_CMD_MID,
                   sizeof(AbandonCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AbandonCmdMsg, (uint16)CF_ABANDON_CC);
    sprintf(AbandonCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER + 1);
    CF_Test_PrintCmdMsg((void*)&AbandonCmdMsg, sizeof(AbandonCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Abandon */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AbandonCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    strcpy(FullTransString, AbandonCmdMsg.Trans);
    sprintf(expEventCfdp, "cfdp_engine: ignoring User-Request that "
            "references unknown transaction (%s).", FullTransString);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, AbandonCmdNoTransId");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, AbandonCmdNoTransId: QEntry Cnt");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
                       "CF_AppPipe, AbandonCmdNoTransId: Cfdp Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, AbandonCmdAllPb
 */
void Test_CF_AppPipe_AbandonCmdAllPb(void)
{
    uint32                QEntryCntActBefore;
    uint32                QEntryCntActAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_CARSCmd_t          AbandonCmdMsg;
    TRANSACTION           trans;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  FullSrcFilename1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFilename2[MAX_FILE_NAME_LENGTH];
    char  expEventInd1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInd2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&AbandonCmdMsg, CF_CMD_MID,
                   sizeof(AbandonCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AbandonCmdMsg,
                      (uint16)CF_ABANDON_CC);
    strcpy(AbandonCmdMsg.Trans, "All");
    CF_Test_PrintCmdMsg((void*)&AbandonCmdMsg, sizeof(AbandonCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* create two playback chan 0, active queue entries */
    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCntActBefore =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    /* Abandon */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AbandonCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActAfter =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter =
       CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    sprintf(FullTransString1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans);
    strcpy(FullSrcFilename1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventInd1, "Outgoing trans %d.%d_%d %s,"
            "CondCode %s,Src %s,Ch %d ",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, "ABANDONED", "NO_ERR",
            FullSrcFilename1, PbFileCmdMsg1.Channel);

    sprintf(FullTransString2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans);
    strcpy(FullSrcFilename2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventInd2, "Outgoing trans %d.%d_%d %s,"
            "CondCode %s,Src %s,Ch %d ",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, "ABANDONED", "NO_ERR",
            FullSrcFilename2, PbFileCmdMsg2.Channel);

    sprintf(expEvent, "%s command received.%s",
            "Abandon", AbandonCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, AbandonCmdAllPb");

    UtAssert_True((QEntryCntActBefore == 2) && (QEntryCntActAfter == 0) &&
                  (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 2),
                  "CF_AppPipe, AbandonCmdAllPb: QEntryCnt");

    UtAssert_EventSent(CF_OUT_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd1,
                  "CF_AppPipe, AbandonCmdAllPb: Pb1 Abandon Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd2,
                  "CF_AppPipe, AbandonCmdAllPb: Pb2 Abandon Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, AbandonCmdAllPb: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, AbandonCmdAllUp
 */
void Test_CF_AppPipe_AbandonCmdAllUp(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCntActBefore;
    uint32                QEntryCntActAfter;
    uint32                QEntryCntHistBefore;
    uint32                QEntryCntHistAfter;
    CF_Test_InPDUInfo_t   InPDUInfo1;
    CF_Test_InPDUInfo_t   InPDUInfo2;
    CF_CARSCmd_t          AbandonCmdMsg;
    CF_Test_InPDUMsg_t    InMdPDUMsg1;
    CF_Test_InPDUMsg_t    InMdPDUMsg2;
    char  expEventInd1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInd2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build Abandon Command */
    CFE_SB_InitMsg((void*)&AbandonCmdMsg, CF_CMD_MID,
                   sizeof(AbandonCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AbandonCmdMsg,
                      (uint16)CF_ABANDON_CC);
    strcpy(AbandonCmdMsg.Trans, "All");
    CF_Test_PrintCmdMsg((void*)&AbandonCmdMsg, sizeof(AbandonCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg1 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming MD PDU Msg2 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    QEntryCntHistBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Abandon */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AbandonCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntActAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    QEntryCntHistAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    sprintf(expEventInd1, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, "ABANDONED", "NO_ERR",
            InPDUInfo1.dst_filename);

    sprintf(expEventInd2, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, "ABANDONED", "NO_ERR",
            InPDUInfo2.dst_filename);

    sprintf(expEvent, "%s command received.%s",
            "Abandon", AbandonCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, AbandonCmdAllUp");

    UtAssert_True((QEntryCntActBefore == 2) && (QEntryCntActAfter == 0) &&
             (QEntryCntHistBefore == 0) && (QEntryCntHistAfter == 2),
             "CF_AppPipe, AbandonCmdAllUp: QEntryCnt");

    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 2) &&
                  (CF_AppData.Hk.App.PDUsRejected == 0),
                  "CF_AppPipe, AbandonCmdAllUp: PDUsReceived cnt");

    UtAssert_True(CF_AppData.Hk.Up.MetaCount == 2,
                  "CF_AppPipe, AbandonCmdAllUp: Up.MetaCount");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd1,
                       "CF_AppPipe, AbandonCmdAllUp: InPDU1 Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInd2,
                       "CF_AppPipe, AbandonCmdAllUp: InPDU2 Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, AbandonCmdAllUp: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdSaveIncompleteFiles
 */
void Test_CF_AppPipe_SetMibCmdSaveIncompleteFiles(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    strcpy(CmdMsg.Value, "yes");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->SaveIncompleteFiles);

    sprintf(expEventCfdp, "%s",
            "cfdp_engine: 'save_incomplete_files' set to 'yes'.");

    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdSaveIncompleteFiles");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
           "CF_AppPipe, SetMibCmdSaveIncompleteFiles: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
             "CF_AppPipe, SetMibCmdSaveIncompleteFiles: Cfdp Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SetMibCmdSaveIncompleteFiles: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdSaveIncompleteFilesInvLen
 */
void Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesInvLen(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    strcpy(CmdMsg.Value, "yes");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_SET_MIB_PARAM_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdSaveIncompleteFilesInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, SetMibCmdSaveIncompleteFilesInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermParam
 */
void Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermParam(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    CFE_PSP_MemSet(CmdMsg.Param, 0xFF, CF_MAX_CFG_PARAM_CHARS);
    strcpy(CmdMsg.Value, "yes");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "Unterminated string found in SetMib Cmd, Param parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermParam");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
        "CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermParam: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermValue
 */
void Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermValue(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    CFE_PSP_MemSet(CmdMsg.Value, 0xFF, CF_MAX_CFG_VALUE_CHARS);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "Unterminated string found in SetMib Cmd, Value parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermValue");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
       "CF_AppPipe, SetMibCmdSaveIncompleteFilesUntermValue: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdAckLimit
 */
void Test_CF_AppPipe_SetMibCmdAckLimit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "ack_limit");
    strcpy(CmdMsg.Value, "4");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->AckLimit);

    sprintf(expEventCfdp, "cfdp_engine: 'ack_limit' set to '%lu'.",
            (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdAckLimit");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdAckLimit: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdAckLimit: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SetMibCmdAckLimit: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdAckLimitNonDigit
 */
void Test_CF_AppPipe_SetMibCmdAckLimitNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "ack_limit");
    strcpy(CmdMsg.Value, "four");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set 'ack_limit'; "
            "value (%s) must be numeric.", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdAckLimitNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
                  "CF_AppPipe, SetMibCmdAckLimitNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdAckTimeout
 */
void Test_CF_AppPipe_SetMibCmdAckTimeout(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "ACK_TIMEOUT");
    strcpy(CmdMsg.Value, "25");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->AckTimeout);

    sprintf(expEventCfdp, "cfdp_engine: 'ack_timeout' set to '%lu'.",
            (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdAckTimeout");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdAckTimeout: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdAckTimeout: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SetMibCmdAckTimeout: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdAckTimeoutNonDigit
 */
void Test_CF_AppPipe_SetMibCmdAckTimeoutNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "ACK_TIMEOUT");
    strcpy(CmdMsg.Value, "ab");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set 'ack_timeout'; "
            "value (%s) must be numeric.", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdAckTimeoutNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
             "CF_AppPipe, SetMibCmdAckTimeoutNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdInactTimeout
 */
void Test_CF_AppPipe_SetMibCmdInactTimeout(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "INACTIVITY_TIMEOUT");
    strcpy(CmdMsg.Value, "40");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->InactivityTimeout);

    sprintf(expEventCfdp, "cfdp_engine: 'inactivity_timeout' set to '%lu'.",
            (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdInactTimeout");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdInactTimeout: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdInactTimeout: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SetMibCmdInactTimeout: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdInactTimeoutNonDigit
 */
void Test_CF_AppPipe_SetMibCmdInactTimeoutNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "INACTIVITY_TIMEOUT");
    strcpy(CmdMsg.Value, "cde");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set 'inactivity_timeout'; "
            "value (%s) must be numeric.", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdInactTimeoutNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
              "CF_AppPipe, SetMibCmdInactTimeoutNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdNakLimit
 */
void Test_CF_AppPipe_SetMibCmdNakLimit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "nak_limit");
    strcpy(CmdMsg.Value, "5");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->NakLimit);

    sprintf(expEventCfdp, "cfdp_engine: 'nak_limit' set to '%lu'.",
            (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdNakLimit");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdNakLimit: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdNakLimit: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SetMibCmdNakLimit: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdNakLimitNonDigit
 */
void Test_CF_AppPipe_SetMibCmdNakLimitNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "nak_limit");
    strcpy(CmdMsg.Value, "FIVE");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set 'nak_limit'; "
            "value (%s) must be numeric.", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdNakLimitNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
                 "CF_AppPipe, SetMibCmdNakLimitNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdNakTimeout
 */
void Test_CF_AppPipe_SetMibCmdNakTimeout(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "NAK_TIMEOUT");
    strcpy(CmdMsg.Value, "2");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->NakTimeout);

    sprintf(expEventCfdp, "cfdp_engine: 'nak_timeout' set to '%lu'.",
            (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdNakTimeout");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdNakTimeout: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdNakTimeout: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SetMibCmdNakTimeout: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdNakTimeoutNonDigit
 */
void Test_CF_AppPipe_SetMibCmdNakTimeoutNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "NAK_TIMEOUT");
    strcpy(CmdMsg.Value, "hg");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set 'nak_timeout'; "
            "value (%s) must be numeric.", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdNakTimeoutNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
               "CF_AppPipe, SetMibCmdNakTimeoutNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdFileChunkSize
 */
void Test_CF_AppPipe_SetMibCmdFileChunkSize(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "OUTGOING_FILE_CHUNK_SIZE");
    strcpy(CmdMsg.Value, "250");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->OutgoingFileChunkSize);

    sprintf(expEventCfdp, "cfdp_engine: 'outgoing_file_chunk_size' "
            "set to '%lu'.", (uint32)atoi(CmdMsg.Value));
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdFileChunkSize");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdFileChunkSize: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                      "CF_AppPipe, SetMibCmdFileChunkSize: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, SetMibCmdFileChunkSize: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdFileChunkSizeNonDigit
 */
void Test_CF_AppPipe_SetMibCmdFileChunkSizeNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "OUTGOING_FILE_CHUNK_SIZE");
    strcpy(CmdMsg.Value, "cde");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "cfdp_engine: can't set "
            "'outgoing_file_chunk_size'; value (%s) must be numeric.",
            CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdFileChunkSizeNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
            "CF_AppPipe, SetMibCmdFileChunkSizeNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdFileChunkOverLimit
 */
void Test_CF_AppPipe_SetMibCmdFileChunkOverLimit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "OUTGOING_FILE_CHUNK_SIZE");
    strcpy(CmdMsg.Value, "10000");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cannot set OUTGOING_FILE_CHUNK_SIZE(%d) > "
            "CF_MAX_OUTGOING_CHUNK_SIZE(%d)",
            atoi(CmdMsg.Value), CF_MAX_OUTGOING_CHUNK_SIZE);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdFileChunkOverLimit");

    UtAssert_EventSent(CF_SET_MIB_CMD_ERR1_EID, CFE_EVS_ERROR, expEvent,
            "CF_AppPipe, SetMibCmdFileChunkOverLimit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdMyId
 */
void Test_CF_AppPipe_SetMibCmdMyId(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  TableValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "MY_ID");
    strcpy(CmdMsg.Value, "25.29");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(TableValue, CF_AppData.Tbl->FlightEntityId);

    sprintf(expEventCfdp, "cfdp_engine: entity-id set to '%s'.",
            CmdMsg.Value);
    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmdMyId");

    UtAssert_True(strcmp(TableValue, CmdMsg.Value) == 0,
                  "CF_AppPipe, SetMibCmdMyId: Config Tbl updated");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG, expEventCfdp,
                       "CF_AppPipe, SetMibCmdMyId: CFDP Event Sent");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SetMibCmdMyId: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdMyIdInvalid
 */
void Test_CF_AppPipe_SetMibCmdMyIdInvalid(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "MY_ID");
    strcpy(CmdMsg.Value, "10000");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cannot set Flight Entity Id to %s, must be 2 byte, "
            "dotted decimal fmt", CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdMyIdInvalid");

    UtAssert_EventSent(CF_SET_MIB_CMD_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetMibCmdMyIdInvalid: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetMibCmdMyIdNonDigit
 */
void Test_CF_AppPipe_SetMibCmdMyIdNonDigit(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  upperValue[CF_MAX_CFG_VALUE_CHARS];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "MY_ID");
    strcpy(CmdMsg.Value, "ab.10");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(upperValue, CF_Test_ToUpperCase(CmdMsg.Value));

    sprintf(expEventCfdp, "cfdp_engine: can't set entity-id to illegal "
            "value (%s).", upperValue);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdMyIdNonDigit");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
                  "CF_AppPipe, SetMibCmdMyIdNonDigit: CFDP Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdSaveIncompleteFiles
 */
void Test_CF_AppPipe_GetMibCmdSaveIncompleteFiles(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expValue[CF_MAX_CFG_VALUE_CHARS];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "save_incomplete_files");
    strcpy(SetMibCmdMsg.Value, "yes");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "save_incomplete_files");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(expValue, CF_Test_ToUpperCase(SetMibCmdMsg.Value));
    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, expValue);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdSaveIncompleteFiles");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                 "CF_AppPipe, GetMibCmdSaveIncompleteFiles: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdSaveIncompleteFilesInvLen
 */
void Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvLen(void)
{
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "save_incomplete_files");

    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_GET_MIB_PARAM_CC,
            sizeof(GetMibCmdMsg), sizeof(GetMibCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GetMibCmdSaveIncompleteFilesInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, GetMibCmdSaveIncompleteFilesInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdSaveIncompleteFilesUntermParam
 */
void Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesUntermParam(void)
{
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    CFE_PSP_MemSet(GetMibCmdMsg.Param, 0xFF, CF_MAX_CFG_PARAM_CHARS);

    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unterminated string found in %s",
            "GetMib Cmd, Param parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                   "CF_AppPipe, GetMibCmdSaveIncompleteFilesUntermParam");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
        "CF_AppPipe, GetMibCmdSaveIncompleteFilesUntermParam: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdSaveIncompleteFilesInvParam
 */
void Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvParam(void)
{
    CF_GetMibParam_t  GetMibCmdMsg;
    char  strUpperParam[CF_MAX_CFG_PARAM_CHARS];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "save_the_bay");

    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(strUpperParam, CF_Test_ToUpperCase(GetMibCmdMsg.Param));

    sprintf(expEvent, "cfdp_engine: can't get value of unrecognized MIB "
            "parameter (%s).", strUpperParam);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GetMibCmdSaveIncompleteFilesInvParam");

    UtAssert_EventSent(CF_CFDP_ENGINE_WARN_EID, CFE_EVS_INFORMATION,
                       expEvent, "CF_AppPipe, "
                       "GetMibCmdSaveIncompleteFilesInvParam: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdAckLimit
 */
void Test_CF_AppPipe_GetMibCmdAckLimit(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "ack_limit");
    strcpy(SetMibCmdMsg.Value, "3");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "ack_limit");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdAckLimit");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdAckLimit: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdAckTimeout
 */
void Test_CF_AppPipe_GetMibCmdAckTimeout(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "ack_timeout");
    strcpy(SetMibCmdMsg.Value, "25");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "ack_timeout");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdAckTimeout");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdAckTimeout: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdInactTimeout
 */
void Test_CF_AppPipe_GetMibCmdInactTimeout(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "INACTIVITY_TIMEOUT");
    strcpy(SetMibCmdMsg.Value, "40");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "INACTIVITY_TIMEOUT");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdInactTimeout");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdInactTimeout: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdNakLimit
 */
void Test_CF_AppPipe_GetMibCmdNakLimit(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "nak_limit");
    strcpy(SetMibCmdMsg.Value, "5");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "nak_limit");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdNakLimit");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdNakLimit: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdNakTimeout
 */
void Test_CF_AppPipe_GetMibCmdNakTimeout(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "NAK_TIMEOUT");
    strcpy(SetMibCmdMsg.Value, "2");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "NAK_TIMEOUT");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdNakTimeout");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdNakTimeout: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdFileChunkSize
 */
void Test_CF_AppPipe_GetMibCmdFileChunkSize(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "OUTGOING_FILE_CHUNK_SIZE");
    strcpy(SetMibCmdMsg.Value, "250");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "OUTGOING_FILE_CHUNK_SIZE");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdFileChunkSize");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdFileChunkSize: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GetMibCmdMyId
 */
void Test_CF_AppPipe_GetMibCmdMyId(void)
{
    CF_SetMibParam_t  SetMibCmdMsg;
    CF_GetMibParam_t  GetMibCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SetMibCmdMsg, CF_CMD_MID,
                   sizeof(SetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SetMibCmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(SetMibCmdMsg.Param, "MY_ID");
    strcpy(SetMibCmdMsg.Value, "25.29");
    CF_Test_PrintCmdMsg((void*)&SetMibCmdMsg, sizeof(SetMibCmdMsg));

    CFE_SB_InitMsg((void*)&GetMibCmdMsg, CF_CMD_MID,
                   sizeof(GetMibCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&GetMibCmdMsg,
                      (uint16)CF_GET_MIB_PARAM_CC);
    strcpy(GetMibCmdMsg.Param, "MY_ID");
    CF_Test_PrintCmdMsg((void*)&GetMibCmdMsg, sizeof(GetMibCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&GetMibCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Get MIB command received.Param %s Value %s",
            GetMibCmdMsg.Param, SetMibCmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GetMibCmdMyId");

    UtAssert_EventSent(CF_GET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, GetMibCmdMyId: Event Sent");

    CF_ResetEngine();
}


/**
 * Hook to support: Test CF_AppPipe, SendCfgParamsCmd
 */
int32 Test_CF_AppPipe_SendCfgParamsCmd_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    pBuff = (unsigned char *)MsgPtr;
    printf("###SendCfgParamsCmd_SendMsgHook(msgLen %u): ", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = CF_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case CF_CONFIG_TLM_MID:
        {
            SendCfgParamsHook_MsgId = CF_CONFIG_TLM_MID;
            printf("Sent CF_CONFIG_TLM_MID\n");
            memcpy((void *)&CfgHookPkt, (void *)MsgPtr, sizeof(CfgHookPkt));

            printf("EngCycPerWakeup: %lu\n", CfgHookPkt.EngCycPerWakeup);
            printf("AckLimit: %lu\n", CfgHookPkt.AckLimit);
            printf("AckTimeout: %lu\n", CfgHookPkt.AckTimeout);
            printf("NakLimit: %lu\n", CfgHookPkt.NakLimit);
            printf("NakTimeout: %lu\n", CfgHookPkt.NakTimeout);
            printf("InactTimeout: %lu\n", CfgHookPkt.InactTimeout);
            printf("DefOutgoingChunkSize: %lu\n",
                   CfgHookPkt.DefOutgoingChunkSize);
            printf("PipeDepth: %lu\n", CfgHookPkt.PipeDepth);
            printf("MaxSimultaneousTrans: %lu\n",
                   CfgHookPkt.MaxSimultaneousTrans);
            printf("IncomingPduBufSize: %lu\n",
                   CfgHookPkt.IncomingPduBufSize);
            printf("OutgoingPduBufSize: %lu\n",
                    CfgHookPkt.OutgoingPduBufSize);
            printf("NumInputChannels: %lu\n", CfgHookPkt.NumInputChannels);
            printf("MaxPlaybackChans: %lu\n", CfgHookPkt.MaxPlaybackChans);
            printf("MaxPollingDirsPerChan: %lu\n",
                   CfgHookPkt.MaxPollingDirsPerChan);
            printf("MemPoolBytes: %lu\n", CfgHookPkt.MemPoolBytes);
            printf("DebugCompiledIn: %lu\n", CfgHookPkt.DebugCompiledIn);
            printf("SaveIncompleteFiles: %s\n",
                   CfgHookPkt.SaveIncompleteFiles);
            printf("PipeName: %s\n", CfgHookPkt.PipeName);
            printf("TmpFilePrefix: %s\n", CfgHookPkt.TmpFilePrefix);
            printf("CfgTblName: %s\n", CfgHookPkt.CfgTblName);
            printf("CfgTbleFilename: %s\n", CfgHookPkt.CfgTbleFilename);
            printf("DefQInfoFilename: %s\n", CfgHookPkt.DefQInfoFilename);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test CF_AppPipe, SendCfgParamsCmd
 */
void Test_CF_AppPipe_SendCfgParamsCmd(void)
{
    cf_config_table_t *pTbl;
    CF_NoArgsCmd_t    CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SEND_CFG_PARAMS_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    SendCfgParamsHook_MsgId = 0;
    memset((void *)&CfgHookPkt, 0x00, sizeof(CfgHookPkt));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_SendCfgParamsCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    pTbl = CF_AppData.Tbl;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "CF:Sending Configuration Pkt");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SendCfgParamsCmd");

    UtAssert_True(SendCfgParamsHook_MsgId == CF_CONFIG_TLM_MID,
                  "CF_AppPipe, SendCfgParamsCmd: Sent CF_CONFIG_TLM_MID");

    UtAssert_True((CfgHookPkt.EngCycPerWakeup ==
                              pTbl->NumEngCyclesPerWakeup)
             && (CfgHookPkt.AckLimit == (uint32)atoi(pTbl->AckLimit))
             && (CfgHookPkt.AckTimeout == (uint32)atoi(pTbl->AckTimeout))
             && (CfgHookPkt.NakLimit == (uint32)atoi(pTbl->NakLimit))
             && (CfgHookPkt.NakTimeout == (uint32)atoi(pTbl->NakTimeout))
             && (CfgHookPkt.InactTimeout ==
                            (uint32)atoi(pTbl->InactivityTimeout))
             && (CfgHookPkt.DefOutgoingChunkSize ==
                            (uint32)atoi(pTbl->OutgoingFileChunkSize))
             && (strcmp(CfgHookPkt.SaveIncompleteFiles,
                    CF_Test_ToUpperCase(pTbl->SaveIncompleteFiles)) == 0),
             "CF_AppPipe, SendCfgParamsCmd: Config Params from Table");

    UtAssert_True((CfgHookPkt.PipeDepth == CF_PIPE_DEPTH)
             && (CfgHookPkt.MaxSimultaneousTrans ==
                            CF_MAX_SIMULTANEOUS_TRANSACTIONS)
             && (CfgHookPkt.IncomingPduBufSize == CF_INCOMING_PDU_BUF_SIZE)
             && (CfgHookPkt.OutgoingPduBufSize == CF_OUTGOING_PDU_BUF_SIZE)
             && (CfgHookPkt.NumInputChannels == CF_NUM_INPUT_CHANNELS)
             && (CfgHookPkt.MaxPlaybackChans == CF_MAX_PLAYBACK_CHANNELS)
             && (CfgHookPkt.MaxPollingDirsPerChan ==
                            CF_MAX_POLLING_DIRS_PER_CHAN)
             && (CfgHookPkt.MemPoolBytes == CF_MEMORY_POOL_BYTES),
             "CF_AppPipe, SendCfgParamsCmd: Config Params 1");

    UtAssert_True((strcmp(CfgHookPkt.PipeName, CF_PIPE_NAME) == 0)
               && (strcmp(CfgHookPkt.TmpFilePrefix,
                          CF_ENGINE_TEMP_FILE_PREFIX) == 0)
               && (strcmp(CfgHookPkt.CfgTblName, CF_CONFIG_TABLE_NAME) == 0)
               && (strcmp(CfgHookPkt.CfgTbleFilename,
                          CF_CONFIG_TABLE_FILENAME) == 0)
               && (strcmp(CfgHookPkt.DefQInfoFilename,
                          CF_DEFAULT_QUEUE_INFO_FILENAME) == 0),
               "CF_AppPipe, SendCfgParamsCmd: Config Params 2");

    UtAssert_EventSent(CF_SND_CFG_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, SendCfgParamsCmd: Event Sent");
}


/**
 * Test CF_AppPipe, SendCfgParamsCmdInvLen
 */
void Test_CF_AppPipe_SendCfgParamsCmdInvLen(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SEND_CFG_PARAMS_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_SEND_CFG_PARAMS_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendCfgParamsCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SendCfgParamsCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, WriteQueueCmdCreatErr
 */
void Test_CF_AppPipe_WriteQueueCmdCreatErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX,
                               OS_FS_ERR_INVALID_POINTER, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "WriteQueueCmd:Error creating file %s, stat=0x%x",
            CF_DEFAULT_QUEUE_INFO_FILENAME, OS_FS_ERR_INVALID_POINTER);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdCreatErr");

    UtAssert_EventSent(CF_SND_QUE_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdCreatErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdInvLen
 */
void Test_CF_AppPipe_WriteQueueCmdInvLen(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_WRITE_QUEUE_INFO_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdUpQValueErr
 */
void Test_CF_AppPipe_WriteQueueCmdUpQValueErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_UPLINK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PENDINGQ;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Invalid Queue Param %u in WriteQueueInfoCmd 1=active,2=history",
            CmdMsg.Queue);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdUpQValueErr");

    UtAssert_EventSent(CF_WR_CMD_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdUpQValueErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdUpDefFilename
 */
void Test_CF_AppPipe_WriteQueueCmdUpDefFilename(void)
{
    int                      i;
    uint16                   PDataLen;
    uint16                   hdr_len;
    int                      bytesWritten;
    uint32                   QEntryCnt;
    FILE                     *fp;
    CFE_FS_Header_t          QInfoHdr;
    CF_QueueInfoFileEntry_t  QInfoEntry[100];
    CF_Test_InPDUInfo_t      InPDUInfo[2];
    CF_WriteQueueCmd_t       WriteQCmdMsg;
    CF_Test_InPDUMsg_t       InMdPDUMsg[2];
    char  AssertString[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg[0], CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg[0]), TRUE);
    InPDUInfo[0].pdu_type = FILE_DIR_PDU;
    InPDUInfo[0].direction = TEST_TO_RECEIVER;
    InPDUInfo[0].mode = TEST_UNACK_MODE;
    InPDUInfo[0].use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo[0].trans.source_id);
    InPDUInfo[0].trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo[0].dest_id);
    InPDUInfo[0].segmentation_control = NO;
    InPDUInfo[0].file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo[0].src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo[0].dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo[0].src_filename) + 1;
    PDataLen += strlen(InPDUInfo[0].dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg[0], &InPDUInfo[0], PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg[0], &InPDUInfo[0], hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg[1], CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg[1]), TRUE);
    InPDUInfo[1].pdu_type = FILE_DIR_PDU;
    InPDUInfo[1].direction = TEST_TO_RECEIVER;
    InPDUInfo[1].mode = TEST_UNACK_MODE;
    InPDUInfo[1].use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo[1].trans.source_id);
    InPDUInfo[1].trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo[1].dest_id);
    InPDUInfo[1].segmentation_control = NO;
    InPDUInfo[1].file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo[1].src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo[1].dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo[1].src_filename) + 1;
    PDataLen += strlen(InPDUInfo[1].dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg[1], &InPDUInfo[1], PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg[1], &InPDUInfo[1], hdr_len);

    /* Build WriteQ Command Msg */
    CFE_SB_InitMsg((void*)&WriteQCmdMsg, CF_CMD_MID,
                   sizeof(WriteQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WriteQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WriteQCmdMsg.Type = CF_UPLINK;
    WriteQCmdMsg.Chan = 0;
    WriteQCmdMsg.Queue = CF_ACTIVEQ;
    strcpy(WriteQCmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&WriteQCmdMsg, sizeof(WriteQCmdMsg));

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    Ut_CFE_FS_SetFunctionHook(UT_CFE_FS_WRITEHDR_INDEX,
                              (void *)&CFE_FS_WriteHeaderHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU 1 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg[0];
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming MD PDU 2 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg[1];
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Write Q Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WriteQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    fp = fopen(CF_DEFAULT_QUEUE_INFO_FILENAME, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_SET);
        fread(&QInfoHdr, 1, sizeof(CFE_FS_Header_t), fp);
        for (i = 0; i < QEntryCnt; i++)
        {
            fread(&QInfoEntry[i], 1, sizeof(CF_QueueInfoFileEntry_t), fp);
        }
        fclose(fp);
    }
    else
    {
        int errnum;

        errnum = errno;
        printf("fopen errno: %d\n", errno);
        printf("fopen error string: %s\n", strerror(errnum));
    }

    bytesWritten = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEvent, "%s written:Size=%d,Entries=%d",
            CF_DEFAULT_QUEUE_INFO_FILENAME, bytesWritten, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteQueueCmdUpDefFilename");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, WriteQueueCmdUpDefFilename: QEntryCnt");

    UtAssert_True(strcmp(QInfoHdr.Description, "CF Queue Information") == 0,
               "CF_AppPipe, WriteQueueCmdUpDefFilename: Written QInfoHdr");

    for (i = 0; i < QEntryCnt; i++)
    {
        sprintf(AssertString, "CF_AppPipe, WriteQueueCmdUpDefFilename: "
                "Written QInfoEntry[%d]", i);

        UtAssert_True((QInfoEntry[i].TransStatus == CF_STAT_ACTIVE)
          && (QInfoEntry[i].TransNum == TEST_IN_TRANS_NUMBER + i)
          && (strcmp(QInfoEntry[i].SrcEntityId,
                    cfdp_id_as_string(InPDUInfo[i].trans.source_id)) == 0)
          && (strcmp(QInfoEntry[i].SrcFile, InPDUInfo[i].src_filename) == 0),
          AssertString);
    }

    UtAssert_EventSent(CF_SND_Q_INFO_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, WriteQueueCmdUpDefFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdUpCustomFilename
 */
void Test_CF_AppPipe_WriteQueueCmdUpCustomFilename(void)
{
    int                      i;
    uint16                   PDataLen;
    uint16                   hdr_len;
    int                      bytesWritten;
    uint32                   QEntryCnt;
    FILE                     *fp;
    CFE_FS_Header_t          QInfoHdr;
    CF_QueueInfoFileEntry_t  QInfoEntry[100];
    CF_Test_InPDUInfo_t      InPDUInfo;
    CF_WriteQueueCmd_t       WriteQCmdMsg;
    CF_Test_InPDUMsg_t       InMdPDUMsg;
    CF_Test_InPDUMsg_t       InFdPDUMsg;
    CF_Test_InPDUMsg_t       InEofPDUMsg;
    char  AssertString[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DATA_PDU;
    InPDUInfo.offset = 0;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InPDUInfo, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.cond_code = NO_ERROR;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    InPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InPDUInfo, hdr_len);

    /* Build WriteQ Command Msg */
    CFE_SB_InitMsg((void*)&WriteQCmdMsg, CF_CMD_MID,
                   sizeof(WriteQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WriteQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WriteQCmdMsg.Type = CF_UPLINK;
    WriteQCmdMsg.Chan = 0;
    WriteQCmdMsg.Queue = CF_HISTORYQ;
    sprintf(WriteQCmdMsg.Filename, "%s%s", TestQInfoDir,
            "WriteQueueCmdUpCustomFilename.dat");

    CF_Test_PrintCmdMsg((void*)&WriteQCmdMsg, sizeof(WriteQCmdMsg));

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    /* Return the read bytes */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    Ut_CFE_FS_SetFunctionHook(UT_CFE_FS_WRITEHDR_INDEX,
                              (void *)&CFE_FS_WriteHeaderHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* WriteQ Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WriteQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    fp = fopen(WriteQCmdMsg.Filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_SET);
        fread(&QInfoHdr, 1, sizeof(CFE_FS_Header_t), fp);
        for (i = 0; i < QEntryCnt; i++)
        {
            fread(&QInfoEntry[i], 1, sizeof(CF_QueueInfoFileEntry_t), fp);
        }
        fclose(fp);
    }
    else
    {
        int errnum;

        errnum = errno;
        printf("fopen errno: %d\n", errno);
        printf("fopen error string: %s\n", strerror(errnum));
    }

    bytesWritten = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEvent, "%s written:Size=%d,Entries=%d",
            WriteQCmdMsg.Filename, bytesWritten, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteQueueCmdUpCustomFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, WriteQueueCmdUpCustomFilename: QEntry Cnt");

    UtAssert_True(strcmp(QInfoHdr.Description, "CF Queue Information") == 0,
            "CF_AppPipe, WriteQueueCmdUpCustomFilename: Written QInfoHdr");

    for (i = 0; i < QEntryCnt; i++)
    {
        sprintf(AssertString, "CF_AppPipe, WriteQueueCmdUpCustomFilename: "
                "Written QInfoEntry[%d]", i);

        UtAssert_True((QInfoEntry[i].TransStatus == CF_STAT_SUCCESS)
          && (QInfoEntry[i].TransNum == InPDUInfo.trans.number)
          && (strcmp(QInfoEntry[i].SrcEntityId,
                     cfdp_id_as_string(InPDUInfo.trans.source_id)) == 0)
          && (strcmp(QInfoEntry[i].SrcFile, InPDUInfo.src_filename) == 0),
          AssertString);
    }

    UtAssert_EventSent(CF_SND_Q_INFO_EID, CFE_EVS_DEBUG, expEvent,
                 "CF_AppPipe, WriteQueueCmdUpCustomFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutQValueErr
 */
void Test_CF_AppPipe_WriteQueueCmdOutQValueErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_HISTORYQ + 1;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Write Queue Info Error, Queue Value %u > max %u",
            CmdMsg.Queue, 2);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdOutQValueErr");

    UtAssert_EventSent(CF_WR_CMD_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdOutQValueErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutQTypeErr
 */
void Test_CF_AppPipe_WriteQueueCmdOutQTypeErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK + 1;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Write Queue Info Error, Type Num %u not valid.",
            CmdMsg.Type);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdOutQTypeErr");

    UtAssert_EventSent(CF_WR_CMD_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdOutQTypeErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutChanErr
 */
void Test_CF_AppPipe_WriteQueueCmdOutChanErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    strcpy(CmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Write Queue Info Error, Channel Value %u > max %u",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdOutChanErr");

    UtAssert_EventSent(CF_WR_CMD_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdOutChanErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdWriteHdrErr
 */
void Test_CF_AppPipe_WriteQueueCmdWriteHdrErr(void)
{
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    sprintf(CmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, 0, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "File write,byte cnt err,file %s,request=%d,actual=%d",
            CmdMsg.Filename, sizeof(CFE_FS_Header_t), 0);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdWriteHdrErr");

    UtAssert_EventSent(CF_FILEWRITE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteQueueCmdWriteHdrErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdEntryWriteErr
 */
void Test_CF_AppPipe_WriteQueueCmdEntryWriteErr(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_WriteQueueCmd_t    WrQCmdMsg;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEventPb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWrQ[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrQCmdMsg, CF_CMD_MID, sizeof(WrQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WrQCmdMsg.Type = CF_PLAYBACK;
    WrQCmdMsg.Chan = 0;
    WrQCmdMsg.Queue = CF_PB_PENDINGQ;
    sprintf(WrQCmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);
    CF_Test_PrintCmdMsg((void*)&WrQCmdMsg, sizeof(WrQCmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t) - 4, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    /* Write Queue Info */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[WrQCmdMsg.Chan].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventPb, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg.Class, PbFileCmdMsg.Channel,
            PbFileCmdMsg.Priority, PbFileCmdMsg.Preserve,
            PbFileCmdMsg.PeerEntityId, FullSrcFileName);
    sprintf(expEventWrQ,
            "File write,byte cnt err,file %s,request=%d,actual=%d",
            WrQCmdMsg.Filename, sizeof(CF_QueueInfoFileEntry_t),
            sizeof(CF_QueueInfoFileEntry_t) - 4);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdEntryWriteErr");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, WriteQueueCmdEntryWriteErr: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb,
        "CF_AppPipe, WriteQueueCmdEntryWriteErr: Event Sent");

    UtAssert_EventSent(CF_FILEWRITE_ERR_EID, CFE_EVS_ERROR, expEventWrQ,
        "CF_AppPipe, WriteQueueCmdEntryWriteErr: Write Queue Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdInvFilenameErr
 */
void Test_CF_AppPipe_WriteQueueCmdInvFilenameErr(void)
{
    CF_WriteQueueCmd_t    WrQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrQCmdMsg, CF_CMD_MID, sizeof(WrQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WrQCmdMsg.Type = CF_PLAYBACK;
    WrQCmdMsg.Chan = 0;
    WrQCmdMsg.Queue = CF_PB_PENDINGQ;
    sprintf(WrQCmdMsg.Filename, "%s%s", TestQInfoDir, "qinf ofile1.dat");
    CF_Test_PrintCmdMsg((void*)&WrQCmdMsg, sizeof(WrQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Write Queue Info */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Filename in %s must be terminated and have no spaces",
            "WriteQueueCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteQueueCmdInvFilenameErr");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, WriteQueueCmdInvFilenameErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutDefFilename
 */
void Test_CF_AppPipe_WriteQueueCmdOutDefFilename(void)
{
    uint32                QEntryCnt;
    CF_WriteQueueCmd_t    WriteQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WriteQCmdMsg, CF_CMD_MID,
                   sizeof(WriteQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WriteQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WriteQCmdMsg.Type = CF_PLAYBACK;
    WriteQCmdMsg.Chan = 0;
    WriteQCmdMsg.Queue = CF_PB_PENDINGQ;
    strcpy(WriteQCmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&WriteQCmdMsg, sizeof(WriteQCmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t), 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WriteQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
        CF_AppData.Chan[WriteQCmdMsg.Chan].PbQ[WriteQCmdMsg.Queue].EntryCnt;

    sprintf(expEvent, "%s written:Size=%d,Entries=%d",
            CF_DEFAULT_QUEUE_INFO_FILENAME, sizeof(CFE_FS_Header_t), 0);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteQueueCmdOutDefFilename");

    UtAssert_True(QEntryCnt == 0,
                  "CF_AppPipe, WriteQueueCmdOutDefFilename: QEntryCnt");

    UtAssert_EventSent(CF_SND_Q_INFO_EID, CFE_EVS_DEBUG, expEvent,
                  "CF_AppPipe, WriteQueueCmdOutDefFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutCustomFilename
 */
void Test_CF_AppPipe_WriteQueueCmdOutCustomFilename(void)
{
    uint32              QEntryCnt;
    CF_WriteQueueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    CmdMsg.Type = CF_PLAYBACK;
    CmdMsg.Chan = 0;
    CmdMsg.Queue = CF_PB_PENDINGQ;
    sprintf(CmdMsg.Filename, "%s%s", TestQInfoDir, TestInDir0);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t), 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* Execute the function being tested */
    CF_AppInit();

    /* Write Queue Info */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.Chan[CmdMsg.Chan].PbQ[CmdMsg.Queue].EntryCnt;

    sprintf(expEvent, "%s written:Size=%d,Entries=%d",
            CmdMsg.Filename, sizeof(CFE_FS_Header_t), 0);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteQueueCmdOutCustomFilename");

    UtAssert_True(QEntryCnt == 0,
                  "CF_AppPipe, WriteQueueCmdOutCustomFilename: QEntryCnt");

    UtAssert_EventSent(CF_SND_Q_INFO_EID, CFE_EVS_DEBUG, expEvent,
                  "CF_AppPipe, WriteQueueCmdOutCustomFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteQueueCmdOutWithEntries
 */
void Test_CF_AppPipe_WriteQueueCmdOutWithEntries(void)
{
    int                      i;
    int                      bytesWritten;
    uint32                   QEntryCnt;
    FILE                     *fp;
    CFE_FS_Header_t          QInfoHdr;
    CF_QueueInfoFileEntry_t  QInfoEntry[100];
    CF_PlaybackFileCmd_t     PbFileCmdMsg[2];
    CF_WriteQueueCmd_t       WrQCmdMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH][100];
    char  FullSrcFilename[MAX_FILE_NAME_LENGTH][100];
    char  AssertString[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWrQ[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrQCmdMsg, CF_CMD_MID, sizeof(WrQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrQCmdMsg,
                      (uint16)CF_WRITE_QUEUE_INFO_CC);
    WrQCmdMsg.Type = CF_PLAYBACK;
    WrQCmdMsg.Chan = 0;
    WrQCmdMsg.Queue = CF_PB_PENDINGQ;
    sprintf(WrQCmdMsg.Filename, "%s%s", TestQInfoDir,
            "WriteQueueCmdOutWithEntries.dat");
    CF_Test_PrintCmdMsg((void*)&WrQCmdMsg, sizeof(WrQCmdMsg));

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    Ut_CFE_FS_SetFunctionHook(UT_CFE_FS_WRITEHDR_INDEX,
                              (void *)&CFE_FS_WriteHeaderHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbPendingQueueEntry(&PbFileCmdMsg[0], &PbFileCmdMsg[1]);

    /* Write Queue Info */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[WrQCmdMsg.Chan].PbQ[WrQCmdMsg.Queue].EntryCnt;

    fp = fopen(WrQCmdMsg.Filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_SET);
        fread(&QInfoHdr, 1, sizeof(CFE_FS_Header_t), fp);
        for (i = 0; i < QEntryCnt; i++)
        {
            fread(&QInfoEntry[i], 1, sizeof(CF_QueueInfoFileEntry_t), fp);
        }
        fclose(fp);
    }
    else
    {
        int errnum;

        errnum = errno;
        printf("fopen errno: %d\n", errno);
        printf("fopen error string: %s\n", strerror(errnum));
    }

    strcpy(&FullTransString[0][0], PbFileCmdMsg[0].PeerEntityId);
    strcpy(&FullSrcFilename[0][0], PbFileCmdMsg[0].SrcFilename);
    sprintf(expEventPb1, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d"
            ",Peer %s,File %s", PbFileCmdMsg[0].Class, PbFileCmdMsg[0].Channel,
            PbFileCmdMsg[0].Priority, PbFileCmdMsg[0].Preserve,
            &FullTransString[0][0], &FullSrcFilename[0][0]);

    strcpy(&FullTransString[0][1], PbFileCmdMsg[1].PeerEntityId);
    strcpy(&FullSrcFilename[0][1], PbFileCmdMsg[1].SrcFilename);
    sprintf(expEventPb2, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d"
            ",Peer %s,File %s", PbFileCmdMsg[1].Class, PbFileCmdMsg[1].Channel,
            PbFileCmdMsg[1].Priority, PbFileCmdMsg[1].Preserve,
            &FullTransString[0][1], &FullSrcFilename[0][1]);

    bytesWritten = sizeof(CFE_FS_Header_t) +
                     (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEventWrQ, "%s written:Size=%d,Entries=%d",
            WrQCmdMsg.Filename, bytesWritten, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteQueueCmdOutWithEntries");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, WriteQueueCmdOutWithEntries: QEntryCnt");

    UtAssert_True(strcmp(QInfoHdr.Description, "CF Queue Information") == 0,
             "CF_AppPipe, WriteQueueCmdOutWithEntries: Written QInfoHdr");

    for (i = 0; i < QEntryCnt; i++)
    {
        sprintf(AssertString, "CF_AppPipe, WriteQueueCmdOutWithEntries: "
                "Written QInfoEntry[%d]", i);

        UtAssert_True((QInfoEntry[i].TransStatus == CF_STAT_PENDING)
          && (QInfoEntry[i].TransNum == 0)
          && (strcmp(QInfoEntry[i].SrcEntityId,
                     CF_AppData.Tbl->FlightEntityId) == 0)
          && (strcmp(QInfoEntry[i].SrcFile, PbFileCmdMsg[i].SrcFilename) == 0),
          AssertString);
    }

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb1,
        "CF_AppPipe, WriteQueueCmdOutWithEntries: PbFile 1 Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb2,
        "CF_AppPipe, WriteQueueCmdOutWithEntries: PbFile 2 Event Sent");

    UtAssert_EventSent(CF_SND_Q_INFO_EID, CFE_EVS_DEBUG, expEventWrQ,
         "CF_AppPipe, WriteQueueCmdOutWithEntries: Write Queue Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdInvLen
 */
void Test_CF_AppPipe_WriteActiveTransCmdInvLen(void)
{
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_ALL;
    strcpy(WrActTrCmdMsg.Filename, "");

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_WR_ACTIVE_TRANS_CC,
            sizeof(WrActTrCmdMsg), sizeof(WrActTrCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, WriteActiveTransCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdInvFilename
 */
void Test_CF_AppPipe_WriteActiveTransCmdInvFilename(void)
{
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK;
    strcpy(WrActTrCmdMsg.Filename, TestQInfoDir);
    strcat(WrActTrCmdMsg.Filename, " qinfofile1.dat");

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "Filename in %s must be terminated and have no spaces",
            "WriteActiveTransCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdInvFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, WriteActiveTransCmdInvFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdCreatFail
 */
void Test_CF_AppPipe_WriteActiveTransCmdCreatFail(void)
{
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK;
    sprintf(WrActTrCmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX,
                               OS_FS_ERR_INVALID_POINTER, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "SendActiveTransCmd:Error creating file %s, stat=0x%x",
            WrActTrCmdMsg.Filename, OS_FS_ERR_INVALID_POINTER);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdCreatFail");

    UtAssert_EventSent(CF_WRACT_ERR2_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, WriteActiveTransCmdCreatFail: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdWrHdrFail
 */
void Test_CF_AppPipe_WriteActiveTransCmdWrHdrFail(void)
{
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK;
    sprintf(WrActTrCmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t) - 5, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "File write,byte cnt err,file %s,request=%d,actual=%d",
            WrActTrCmdMsg.Filename, sizeof(CFE_FS_Header_t),
            sizeof(CFE_FS_Header_t) - 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdWrHdrFail");

    UtAssert_EventSent(CF_FILEWRITE_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, WriteActiveTransCmdWrHdrFail: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdInvWhichQs
 */
void Test_CF_AppPipe_WriteActiveTransCmdInvWhichQs(void)
{
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK + 1;
    sprintf(WrActTrCmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Write Active Cmd Error,Type Value %d > max %d",
            WrActTrCmdMsg.Type, CF_PLAYBACK);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdInvWhichQs");

    UtAssert_EventSent(CF_WRACT_ERR1_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, WriteActiveTransCmdInvWhichQs: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdEntryWriteErr
 */
void Test_CF_AppPipe_WriteActiveTransCmdEntryWriteErr(void)
{
    uint32                    QEntryCnt;
    CF_PlaybackFileCmd_t      PbFileCmdMsg;
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_ALL;
    sprintf(WrActTrCmdMsg.Filename, "%s%s", TestQInfoDir, TestQInfoFile1);

    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t) - 4, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    sprintf(expEvent, "File write,byte cnt err,file %s,request=%d,actual=%d",
            WrActTrCmdMsg.Filename, sizeof(CF_QueueInfoFileEntry_t),
            sizeof(CF_QueueInfoFileEntry_t) - 4);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, WriteActiveTransCmdEntryWriteErr");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, WriteActiveTransCmdEntryWriteErr: QEntryCnt");

    UtAssert_EventSent(CF_FILEWRITE_ERR_EID, CFE_EVS_ERROR, expEvent,
                 "CF_AppPipe, WriteActiveTransCmdEntryWriteErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdPbDefaultFilename
 */
void Test_CF_AppPipe_WriteActiveTransCmdPbDefaultFilename(void)
{
    uint32                    QEntryCnt;
    int                       WrittenBytes;
    CF_PlaybackFileCmd_t      PbFileCmdMsg1;
    CF_PlaybackFileCmd_t      PbFileCmdMsg2;
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  FullSrcFileName1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName2[MAX_FILE_NAME_LENGTH];
    char  expEventPb1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWr[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* build cmd to write all active entries to a file */
    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK;
    strcpy(WrActTrCmdMsg.Filename, "");
    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    /* force the file create to return a valid file descriptor (5) */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t), 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* create two playback chan 0, active queue entry */
    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCnt = CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(FullSrcFileName1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventPb1, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg1.Class, PbFileCmdMsg1.Channel,
            PbFileCmdMsg1.Priority, PbFileCmdMsg1.Preserve,
            PbFileCmdMsg1.PeerEntityId, FullSrcFileName1);

    strcpy(FullSrcFileName2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventPb2, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg2.Class, PbFileCmdMsg2.Channel,
            PbFileCmdMsg2.Priority, PbFileCmdMsg2.Preserve,
            PbFileCmdMsg2.PeerEntityId, FullSrcFileName2);

    /* 1 header size + 2 entry size */
    WrittenBytes = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEventWr, "%s written:Size=%d,Entries=%d",
            CF_DEFAULT_QUEUE_INFO_FILENAME, WrittenBytes, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteActiveTransCmdPbDefaultFilename");

    UtAssert_True(QEntryCnt == 2,
          "CF_AppPipe, WriteActiveTransCmdPbDefaultFilename: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb1,
      "CF_AppPipe, WriteActiveTransCmdPbDefaultFilename: PbFile1 Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb2,
      "CF_AppPipe, WriteActiveTransCmdPbDefaultFilename: PbFile2 Event Sent");

    UtAssert_EventSent(CF_WRACT_TRANS_EID, CFE_EVS_DEBUG, expEventWr,
         "CF_AppPipe, WriteActiveTransCmdPbDefaultFilename: Write Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdUpDefaultFilename
 */
void Test_CF_AppPipe_WriteActiveTransCmdUpDefaultFilename(void)
{
    uint16                    PDataLen;
    uint16                    hdr_len;
    uint32                    QEntryCnt;
    int                       WrittenBytes;
    CF_Test_InPDUInfo_t       InPDUInfo;
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    CF_Test_InPDUMsg_t        InMdPDUMsg;
    char  expEventWr[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* build cmd to write all active entries to a file */
    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_UPLINK;
    strcpy(WrActTrCmdMsg.Filename, "");
    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    /* force the file create to return a valid file descriptor (5) */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSE_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_CLOSE_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX,
                               sizeof(CF_QueueInfoFileEntry_t), 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX,
                            sizeof(CFE_FS_Header_t), 1);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* Write Active Trans Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* 1 header size + 1 entry size */
    WrittenBytes = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEventWr, "%s written:Size=%d,Entries=%d",
            CF_DEFAULT_QUEUE_INFO_FILENAME, WrittenBytes, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteActiveTransCmdUpDefaultFilename");

    UtAssert_True(QEntryCnt == 1,
          "CF_AppPipe, WriteActiveTransCmdUpDefaultFilename: QEntryCnt");

    UtAssert_EventSent(CF_WRACT_TRANS_EID, CFE_EVS_DEBUG, expEventWr,
         "CF_AppPipe, WriteActiveTransCmdUpDefaultFilename: Write Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdPbCustFilename
 */
void Test_CF_AppPipe_WriteActiveTransCmdPbCustFilename(void)
{
    int                       i;
    int                       WrittenBytes;
    uint32                    QEntryCnt;
    FILE                      *fp;
    CFE_FS_Header_t           QInfoHdr;
    CF_QueueInfoFileEntry_t   QInfoEntry[100];
    CF_PlaybackFileCmd_t      PbFileCmdMsg1;
    CF_PlaybackFileCmd_t      PbFileCmdMsg2;
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH][100];
    char  AssertString[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWr[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* build cmd to write all active entries to a file */
    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_PLAYBACK;
    strcpy(WrActTrCmdMsg.Filename, TestQInfoDir);
    strcat(WrActTrCmdMsg.Filename, "WriteActiveTransCmdPbCustFilename.dat");
    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    Ut_CFE_FS_SetFunctionHook(UT_CFE_FS_WRITEHDR_INDEX,
                              (void *)&CFE_FS_WriteHeaderHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* create two playback chan 0, active queue entry */
    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    fp = fopen(WrActTrCmdMsg.Filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_SET);
        fread(&QInfoHdr, 1, sizeof(CFE_FS_Header_t), fp);
        for (i = 0; i < QEntryCnt; i++)
        {
            fread(&QInfoEntry[i], 1, sizeof(CF_QueueInfoFileEntry_t), fp);
        }
        fclose(fp);
    }
    else
    {
        int errnum;

        errnum = errno;
        printf("fopen errno: %d\n", errno);
        printf("fopen error string: %s\n", strerror(errnum));
    }

    strcpy(&FullSrcFileName[0][0], PbFileCmdMsg1.SrcFilename);
    sprintf(expEventPb1, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg1.Class, PbFileCmdMsg1.Channel,
            PbFileCmdMsg1.Priority, PbFileCmdMsg1.Preserve,
            PbFileCmdMsg1.PeerEntityId, &FullSrcFileName[0][0]);

    strcpy(&FullSrcFileName[0][1], PbFileCmdMsg2.SrcFilename);
    sprintf(expEventPb2, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg2.Class, PbFileCmdMsg2.Channel,
            PbFileCmdMsg2.Priority, PbFileCmdMsg2.Preserve,
            PbFileCmdMsg2.PeerEntityId, &FullSrcFileName[0][1]);

    /* 1 header size + 2 entry size */
    WrittenBytes = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEventWr, "%s written:Size=%d,Entries=%d",
            WrActTrCmdMsg.Filename, WrittenBytes, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteActiveTransCmdPbCustFilename");

    UtAssert_True((QEntryCnt == 2),
          "CF_AppPipe, WriteActiveTransCmdPbCustFilename: QEntryCnt");

    UtAssert_True(strcmp(QInfoHdr.Description,
                  "CF Active Trans Information") == 0,
      "CF_AppPipe, WriteActiveTransCmdPbCustFilename: Written QInfoHdr");

    for (i = 0; i < QEntryCnt; i++)
    {
        sprintf(AssertString, "CF_AppPipe, WriteActiveTransCmdPbCust"
                "Filename: Written QInfoEntry[%d]", i);

        UtAssert_True((QInfoEntry[i].TransStatus == CF_STAT_ACTIVE)
                && (QInfoEntry[i].TransNum == i + 1)
                && (strcmp(QInfoEntry[i].SrcEntityId,
                           CF_AppData.Tbl->FlightEntityId) == 0)
                && (strcmp(QInfoEntry[i].SrcFile, &FullSrcFileName[0][i]) == 0),
                AssertString);
    }

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb1,
      "CF_AppPipe, WriteActiveTransCmdPbCustFilename: PbFile1 Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb2,
      "CF_AppPipe, WriteActiveTransCmdPbCustFilename: PbFile2 Event Sent");

    UtAssert_EventSent(CF_WRACT_TRANS_EID, CFE_EVS_DEBUG, expEventWr,
      "CF_AppPipe, WriteActiveTransCmdPbCustFilename: Write Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, WriteActiveTransCmdUpCustFilename
 */
void Test_CF_AppPipe_WriteActiveTransCmdUpCustFilename(void)
{
    int                       i;
    uint16                    PDataLen;
    uint16                    hdr_len;
    int                       WrittenBytes;
    uint32                    QEntryCnt;
    FILE                      *fp;
    CFE_FS_Header_t           QInfoHdr;
    CF_QueueInfoFileEntry_t   QInfoEntry[100];
    CF_Test_InPDUInfo_t       InPDUInfo;
    CF_WriteActiveTransCmd_t  WrActTrCmdMsg;
    CF_Test_InPDUMsg_t        InMdPDUMsg;
    char  AssertString[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWr[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* build cmd to write all active entries to a file */
    CFE_SB_InitMsg((void*)&WrActTrCmdMsg, CF_CMD_MID,
                   sizeof(WrActTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&WrActTrCmdMsg,
                      (uint16)CF_WR_ACTIVE_TRANS_CC);
    WrActTrCmdMsg.Type = CF_UPLINK;
    strcpy(WrActTrCmdMsg.Filename, TestQInfoDir);
    strcat(WrActTrCmdMsg.Filename, "WriteActiveTransCmdUpCustFilename.dat");
    CF_Test_PrintCmdMsg((void*)&WrActTrCmdMsg, sizeof(WrActTrCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    Ut_CFE_FS_SetFunctionHook(UT_CFE_FS_WRITEHDR_INDEX,
                              (void *)&CFE_FS_WriteHeaderHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* incoming MD PDU Msg */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Write Active Trans Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WrActTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    fp = fopen(WrActTrCmdMsg.Filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_SET);
        fread(&QInfoHdr, 1, sizeof(CFE_FS_Header_t), fp);
        for (i = 0; i < QEntryCnt; i++)
        {
            fread(&QInfoEntry[i], 1, sizeof(CF_QueueInfoFileEntry_t), fp);
        }
        fclose(fp);
    }
    else
    {
        int errnum;

        errnum = errno;
        printf("fopen errno: %d\n", errno);
        printf("fopen error string: %s\n", strerror(errnum));
    }

    /* 1 header size + 1 entry size */
    WrittenBytes = sizeof(CFE_FS_Header_t) +
                   (QEntryCnt * sizeof(CF_QueueInfoFileEntry_t));
    sprintf(expEventWr, "%s written:Size=%d,Entries=%d",
            WrActTrCmdMsg.Filename, WrittenBytes, (int)QEntryCnt);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, WriteActiveTransCmdUpCustFilename");

    UtAssert_True(QEntryCnt == 1,
            "CF_AppPipe, WriteActiveTransCmdUpCustFilename: QEntryCnt");

    UtAssert_True(strcmp(QInfoHdr.Description,
                         "CF Active Trans Information") == 0,
         "CF_AppPipe, WriteActiveTransCmdUpCustFilename: Written QInfoHdr");

    for (i = 0; i < QEntryCnt; i++)
    {
        sprintf(AssertString, "CF_AppPipe, WriteActiveTransCmdUpCust"
                "Filename: Written QInfoEntry[%d]", i);

        UtAssert_True((QInfoEntry[i].TransStatus == CF_STAT_ACTIVE)
              && (QInfoEntry[i].TransNum == TEST_IN_TRANS_NUMBER + i)
              && (strcmp(QInfoEntry[i].SrcEntityId,
                         cfdp_id_as_string(InPDUInfo.trans.source_id)) == 0)
              && (strcmp(QInfoEntry[i].SrcFile, InPDUInfo.src_filename) == 0),
              AssertString);
    }

    UtAssert_EventSent(CF_WRACT_TRANS_EID, CFE_EVS_DEBUG, expEventWr,
      "CF_AppPipe, WriteActiveTransCmdUpCustFilename: Write Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdFileNotFound
 */
void Test_CF_AppPipe_SendTransDiagCmdFileNotFound(void)
{
    uint32                QEntryCnt;
    CF_SendTransCmd_t     SndTrCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile2);
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt +
                CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_PENDINGQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_HISTORYQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_PENDINGQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_HISTORYQ].EntryCnt;

    /* Send Trans Diag */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Send Trans Cmd Error, %s not found.",
            SndTrCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendTransDiagCmdFileNotFound");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, SendTransDiagCmdFileNotFound: QEntryCnt");

    UtAssert_EventSent(CF_SND_TRANS_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, SendTransDiagCmdFileNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdTransIdNotFound
 */
void Test_CF_AppPipe_SendTransDiagCmdTransIdNotFound(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              QEntryCnt;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_SendTransCmd_t   SndTrCmdMsg;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Send Trans Diag Msg */
    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER + 1);

    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt +
                CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_PENDINGQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[0].PbQ[CF_PB_HISTORYQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_PENDINGQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt +
                CF_AppData.Chan[1].PbQ[CF_PB_HISTORYQ].EntryCnt;

    /* Send Trans Diag Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Send Trans Cmd Error, %s not found.",
            SndTrCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendTransDiagCmdTransIdNotFound");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, SendTransDiagCmdTransIdNotFound: QEntryCnt");

    UtAssert_EventSent(CF_SND_TRANS_ERR_EID, CFE_EVS_ERROR, expEvent,
                 "CF_AppPipe, SendTransDiagCmdTransIdNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdInvLen
 */
void Test_CF_AppPipe_SendTransDiagCmdInvLen(void)
{
    CF_SendTransCmd_t     SndTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile1);
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Send Trans Diag */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_SEND_TRANS_DIAG_DATA_CC,
            sizeof(SndTrCmdMsg), sizeof(SndTrCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendTransDiagCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SendTransDiagCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdUntermString
 */
void Test_CF_AppPipe_SendTransDiagCmdUntermString(void)
{
    CF_SendTransCmd_t     SndTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    CFE_PSP_MemSet(SndTrCmdMsg.Trans, 0xFF, MAX_FILE_NAME_LENGTH);

    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unterminated string found in %s",
            "SendTransData Cmd, Trans parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendTransDiagCmdUntermString");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, SendTransDiagCmdUntermString: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdInvFilename
 */
void Test_CF_AppPipe_SendTransDiagCmdInvFilename(void)
{
    CF_SendTransCmd_t     SndTrCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s%s", TestPbDir0, "This name has spaces");
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Send Trans Diag */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Filename in %s must be terminated and have no spaces",
            "SendTransDataCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SendTransDiagCmdInvFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SendTransDiagCmdInvFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Hook to support: Test CF_AppPipe, SendTransDiagCmd
 */
int32 Test_CF_AppPipe_SendTransDiagCmd_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;
    CF_TransPacket_t    *pPkt;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    pBuff = (unsigned char *)MsgPtr;
    printf("###SendTransDiagCmd_SendMsgHook(msgLen %u): ", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = CF_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case CF_TRANS_TLM_MID:
        {
            SendTransDiagHook_MsgId[SendTransDiagHook_CalledCnt]
                                    = CF_TRANS_TLM_MID;
            printf("Sent CF_TRANS_TLM_MID\n");
            pPkt = &(TransPkt[SendTransDiagHook_CalledCnt]);
            memcpy((void *)pPkt, (void *)MsgPtr, sizeof(CF_TransPacket_t));

            printf("Eng.TransLen: %u\n", pPkt->Eng.TransLen);
            printf("Eng.TransVal: %u\n", pPkt->Eng.TransVal);
            printf("Eng.Naks: %u\n", pPkt->Eng.Naks);
            printf("Eng.PartLen: %u\n", pPkt->Eng.PartLen);
            printf("Eng.PartVal: %u\n", pPkt->Eng.PartVal);
            printf("Eng.Phase: %u\n", pPkt->Eng.Phase);
            printf("Eng.Flags: %lu\n", pPkt->Eng.Flags);
            printf("Eng.TransNum: %lu\n", pPkt->Eng.TransNum);
            printf("Eng.Attempts: %lu\n", pPkt->Eng.Attempts);
            printf("Eng.CondCode: %lu\n", pPkt->Eng.CondCode);
            printf("Eng.DeliCode: %lu\n", pPkt->Eng.DeliCode);
            printf("Eng.FdOffset: %lu\n", pPkt->Eng.FdOffset);
            printf("Eng.FdLength: %lu\n", pPkt->Eng.FdLength);
            printf("Eng.Checksum: %lu\n", pPkt->Eng.Checksum);
            printf("Eng.FinalStat: %lu\n", pPkt->Eng.FinalStat);
            printf("Eng.FileSize: %lu\n", pPkt->Eng.FileSize);
            printf("Eng.RcvdFileSize: %lu\n", pPkt->Eng.RcvdFileSize);
            printf("Eng.Role: %lu\n", pPkt->Eng.Role);
            printf("Eng.State: %lu\n", pPkt->Eng.State);
            printf("Eng.StartTime: %lu\n", pPkt->Eng.StartTime);
            printf("Eng.SrcFile: %s\n", pPkt->Eng.SrcFile);
            printf("Eng.DstFile: %s\n", pPkt->Eng.DstFile);
            printf("Eng.TmpFile: %s\n", pPkt->Eng.TmpFile);

            printf("App.Status: %lu\n", pPkt->App.Status);
            printf("App.CondCode: %lu\n", pPkt->App.CondCode);
            printf("App.Priority: %lu\n", pPkt->App.Priority);
            printf("App.Class: %lu\n", pPkt->App.Class);
            printf("App.ChanNum: %lu\n", pPkt->App.ChanNum);
            printf("App.Source: %lu\n", pPkt->App.Source);
            printf("App.NodeType: %lu\n", pPkt->App.NodeType);
            printf("App.TransNum: %lu\n", pPkt->App.TransNum);
            printf("App.SrcEntityId: %s\n", pPkt->App.SrcEntityId);
            printf("App.SrcFile: %s\n", pPkt->App.SrcFile);
            printf("App.DstFile: %s\n", pPkt->App.DstFile);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    SendTransDiagHook_CalledCnt ++;

    return CFE_SUCCESS;
}


/**
 * Test CF_AppPipe, SendTransDiagCmdFilename
 */
void Test_CF_AppPipe_SendTransDiagCmdFilename(void)
{
    int                   i;
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_SendTransCmd_t     SndTrCmdMsg;
    CF_TransPacket_t      *pPkt;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);

    memset((void *)SendTransDiagHook_MsgId, 0x00,
           sizeof(SendTransDiagHook_MsgId));
    SendTransDiagHook_CalledCnt = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_SendTransDiagCmd_SendMsgHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void*)&CFE_TIME_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void*)&CFE_SB_TimeStampMsgHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    /* Send Trans Diag */
    strcpy(SndTrCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
          CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(expEvent, "CF:Sending Transaction Pkt %s", SndTrCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SendTransDiagCmdFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, SendTransDiagCmdFilename: QEntryCnt");

    UtAssert_True(SendTransDiagHook_CalledCnt == 1,
                  "CF_AppPipe, SendTransDiagCmdFilename: Hook CalledCnt");

    for (i = 0; i < SendTransDiagHook_CalledCnt; i++)
    {
        UtAssert_True(SendTransDiagHook_MsgId[i] == CF_TRANS_TLM_MID,
             "CF_AppPipe, SendTransDiagCmdFilename: Sent CF_TRANS_TLM_MID");

        pPkt = &(TransPkt[i]);
        UtAssert_True((CFE_TST(pPkt->Eng.Flags, 0) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 1) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 2) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 3) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 4) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 5) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 6) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 7) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 8) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 9) == FALSE) &&
                      (pPkt->Eng.TransNum == 0) &&
                      (strcmp(pPkt->Eng.SrcFile, "") == 0) &&
                      (strcmp(pPkt->Eng.DstFile, "") == 0),
                      "CF_AppPipe, SendTransDiagCmdFilename: Eng params");

        UtAssert_True((pPkt->App.Status == CF_STAT_PENDING) &&
                      (pPkt->App.Source == CF_PLAYBACKFILECMD) &&
                      (pPkt->App.NodeType == CF_PLAYBACK) &&
                      (strcmp(pPkt->App.SrcEntityId,
                              CF_AppData.Tbl->FlightEntityId) == 0) &&
                      (strcmp(pPkt->App.SrcFile, SndTrCmdMsg.Trans) == 0) &&
                      (strcmp(pPkt->App.DstFile,
                              PbFileCmdMsg.DstFilename) == 0),
                      "CF_AppPipe, SendTransDiagCmdFilename: App params");
    }

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdPendingQTransId
 */
void Test_CF_AppPipe_SendTransDiagCmdPendingQTransId(void)
{
    int                   i;
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg[2];
    CF_SendTransCmd_t     SndTrCmdMsg;
    CF_TransPacket_t      *pPkt;
    char  AssertStringEng[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  AssertStringApp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s%s", TestFlightEntityId, "_0");
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    memset((void *)SendTransDiagHook_MsgId, 0x00,
           sizeof(SendTransDiagHook_MsgId));
    SendTransDiagHook_CalledCnt = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_SendTransDiagCmd_SendMsgHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void*)&CFE_TIME_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void*)&CFE_SB_TimeStampMsgHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbPendingQueueEntry(&PbFileCmdMsg[0], &PbFileCmdMsg[1]);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
       CF_AppData.Chan[PbFileCmdMsg[0].Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(expEvent, "CF:Sending Transaction Pkt %s", SndTrCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SendTransDiagCmdPendingQTransId");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, SendTransDiagCmdPendingQTransId: QEntryCnt");

    UtAssert_True(SendTransDiagHook_CalledCnt == 2,
             "CF_AppPipe, SendTransDiagCmdPendingQTransId: Hook CalledCnt");

    for (i = 0; i < SendTransDiagHook_CalledCnt; i++)
    {
        sprintf(AssertStringEng, "CF_AppPipe, "
                "SendTransDiagCmdPendingQTransId: Eng params %d", i);
        sprintf(AssertStringApp, "CF_AppPipe, "
                "SendTransDiagCmdPendingQTransId: App params %d", i);

        UtAssert_True(SendTransDiagHook_MsgId[i] == CF_TRANS_TLM_MID,
          "CF_AppPipe, SendTransDiagCmdPendingQTransId: Sent CF_TRANS_TLM_MID");

        pPkt = &(TransPkt[i]);
        UtAssert_True((CFE_TST(pPkt->Eng.Flags, 0) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 1) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 2) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 3) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 4) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 5) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 6) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 7) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 8) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 9) == FALSE) &&
                      (pPkt->Eng.TransNum == 0) &&
                      (strcmp(pPkt->Eng.SrcFile, "") == 0) &&
                      (strcmp(pPkt->Eng.DstFile, "") == 0),
                      AssertStringEng);

        UtAssert_True((pPkt->App.Status == CF_STAT_PENDING) &&
                      (pPkt->App.Source == CF_PLAYBACKFILECMD) &&
                      (pPkt->App.NodeType == CF_PLAYBACK) &&
                      (strcmp(pPkt->App.SrcEntityId,
                              CF_AppData.Tbl->FlightEntityId) == 0) &&
                      (strcmp(pPkt->App.DstFile,
                              PbFileCmdMsg[i].DstFilename) == 0),
                      AssertStringApp);
    }

    UtAssert_EventSent(CF_SND_TRANS_CMD_EID, CFE_EVS_DEBUG, expEvent,
                  "CF_AppPipe, SendTransDiagCmdPendingQTransId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SendTransDiagCmdPbTransId
 */
void Test_CF_AppPipe_SendTransDiagCmdPbTransId(void)
{
    int                   i, j;
    uint32                totalQEntryCnt = 0;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_SendTransCmd_t     SndTrCmdMsg;
    CF_TransPacket_t      *pPkt;
    char  AssertStringEng[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  AssertStringApp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build Suspend Cmd Msg */
    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);
    sprintf(SuspendCmdMsg.Trans, "%s%s", TestFlightEntityId, "_3");

    /* Build Send Trans Diag Cmd Msg */
    CFE_SB_InitMsg((void*)&SndTrCmdMsg, CF_CMD_MID,
                   sizeof(SndTrCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SndTrCmdMsg,
                      (uint16)CF_SEND_TRANS_DIAG_DATA_CC);
    sprintf(SndTrCmdMsg.Trans, "%s%s", TestFlightEntityId, "_3");
    CF_Test_PrintCmdMsg((void*)&SndTrCmdMsg, sizeof(SndTrCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    memset((void *)SendTransDiagHook_MsgId, 0x00,
           sizeof(SendTransDiagHook_MsgId));
    SendTransDiagHook_CalledCnt = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_SendTransDiagCmd_SendMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void*)&CFE_TIME_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void*)&CFE_SB_TimeStampMsgHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg1);
    CF_TstUtil_FinishPbActiveQueueEntries();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
    CF_ShowQs();
machine_list__display_list();

    for (i = 0; i < CF_NUM_UPLINK_QUEUES; i++)
    {
        totalQEntryCnt += CF_AppData.UpQ[i].EntryCnt;
    }

    for (i = 0; i < CF_MAX_PLAYBACK_CHANNELS; i++)
    {
        for (j = 0; j < CF_QUEUES_PER_CHAN; j++)
        {
            totalQEntryCnt += CF_AppData.Chan[i].PbQ[j].EntryCnt;
        }
    }

    /* Send Suspend Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Send TransDiag Cmd */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SndTrCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:Sending Transaction Pkt %s", SndTrCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 5) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SendTransDiagCmdPbTransId");

    UtAssert_True(totalQEntryCnt == 3,
                  "CF_AppPipe, SendTransDiagCmdPbTransId: QEntryCnt");

    UtAssert_True(SendTransDiagHook_CalledCnt == 1,
                  "CF_AppPipe, SendTransDiagCmdPbTransId: Hook CalledCnt");

    for (i = 0; i < SendTransDiagHook_CalledCnt; i ++)
    {
        sprintf(AssertStringEng, "CF_AppPipe, "
                "SendTransDiagCmdPbTransId: Eng params %d", i);
        sprintf(AssertStringApp, "CF_AppPipe, "
                "SendTransDiagCmdPbTransId: App params %d", i);

        UtAssert_True(SendTransDiagHook_MsgId[i] == CF_TRANS_TLM_MID,
             "CF_AppPipe, SendTransDiagCmdPbTransId: Sent CF_TRANS_TLM_MID");

        pPkt = &(TransPkt[i]);
        UtAssert_True((CFE_TST(pPkt->Eng.Flags, 0) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 1) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 2) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 3) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 4) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 5) == FALSE) &&
                      (CFE_TST(pPkt->Eng.Flags, 6) == FALSE) &&
                      /* Suspended */
                      (CFE_TST(pPkt->Eng.Flags, 7) == TRUE) &&
                      /* File transfer requested */
                      (CFE_TST(pPkt->Eng.Flags, 8) == TRUE) &&
                      (CFE_TST(pPkt->Eng.Flags, 9) == FALSE),
                      "CF_AppPipe, SendTransDiagCmdPbTransId: Eng Flags");

        UtAssert_True((pPkt->Eng.TransLen == 2) && /* Src Id length */
                      (pPkt->Eng.TransVal == 0) && /* Src Id 1'st number */
                      (pPkt->Eng.PartLen == 2) &&  /* Partner Id length */
                      (pPkt->Eng.PartVal == 0) &&  /* Partner Id 1'st number */
                      (pPkt->Eng.Phase == 1) &&    /* Initialized */
                      (pPkt->Eng.TransNum == 3) && /* Transaction number */
                      (pPkt->Eng.Attempts == 0) &&
                      (pPkt->Eng.CondCode == NO_ERROR) &&
                      (pPkt->Eng.DeliCode == DATA_INCOMPLETE) &&
                      (pPkt->Eng.FinalStat == FINAL_STATUS_UNKNOWN) &&
                      (pPkt->Eng.Role == CLASS_1_SENDER) &&
                      (pPkt->Eng.State == S1) &&
                      (strcmp(pPkt->Eng.SrcFile,
                              PbFileCmdMsg2.SrcFilename) == 0) &&
                      (strcmp(pPkt->Eng.DstFile,
                              PbFileCmdMsg2.DstFilename) == 0),
                      AssertStringEng);

        UtAssert_True((pPkt->App.Status == CF_STAT_ACTIVE) &&
                      (pPkt->App.CondCode == NO_ERROR) &&
                      (pPkt->App.Priority == PbFileCmdMsg2.Priority) &&
                      (pPkt->App.Class == PbFileCmdMsg2.Class) &&
                      (pPkt->App.ChanNum == PbFileCmdMsg2.Channel) &&
                      (pPkt->App.Source == CF_PLAYBACKFILECMD) &&
                      (pPkt->App.NodeType == CF_OUTGOING) &&
                      (pPkt->App.TransNum == 3) &&
                      (strcmp(pPkt->App.SrcEntityId,
                              TestFlightEntityId) == 0) &&
                      (strcmp(pPkt->App.SrcFile,
                              PbFileCmdMsg2.SrcFilename) == 0) &&
                      (strcmp(pPkt->App.DstFile,
                              PbFileCmdMsg2.DstFilename) == 0),
                      AssertStringApp);
    }

    UtAssert_EventSent(CF_SND_TRANS_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, SendTransDiagCmdPbTransId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvLen
 */
void Test_CF_AppPipe_SetPollParamCmdInvLen(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_SET_POLL_PARAM_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvChan
 */
void Test_CF_AppPipe_SetPollParamCmdInvChan(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Chan Param %u in SetPollParamCmd,Max %u",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvChan");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvDir
 */
void Test_CF_AppPipe_SetPollParamCmdInvDir(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = CF_MAX_POLLING_DIRS_PER_CHAN;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid PollDir Param %u in SetPollParamCmd,Max %u",
            CmdMsg.Dir, CF_MAX_POLLING_DIRS_PER_CHAN-1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvDir");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvDir: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvClass
 */
void Test_CF_AppPipe_SetPollParamCmdInvClass(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_2 + 1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Class Param %u in "
            "SetPollParamCmd,Min %u,Max %u",
            CmdMsg.Class, CF_CLASS_1, CF_CLASS_2);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvClass");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvClass: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvPreserve
 */
void Test_CF_AppPipe_SetPollParamCmdInvPreserve(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE + 1;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Preserve Param %u in SetPollParamCmd,Max %u",
            CmdMsg.Preserve, CF_KEEP_FILE);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvPreserve");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR4_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, SetPollParamCmdInvPreserve: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvSrc
 */
void Test_CF_AppPipe_SetPollParamCmdInvSrc(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, "/cf /");
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "SrcPath in SetPollParam Cmd must be terminated,"
            "have no spaces,slash at end");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvSrc");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR5_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvSrc: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvDst
 */
void Test_CF_AppPipe_SetPollParamCmdInvDst(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, "gnd path");

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "DstPath in SetPollParam Cmd must be "
            "terminated and have no spaces");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvDst");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR6_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvDst: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdInvId
 */
void Test_CF_AppPipe_SetPollParamCmdInvId(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, "234200");
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "PeerEntityId %s in SetPollParam Cmd must "
            "be 2 byte,dotted decimal fmt.ex 0.24", CmdMsg.PeerEntityId);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetPollParamCmdInvId");

    UtAssert_EventSent(CF_SET_POLL_PARAM_ERR7_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetPollParamCmdInvId: Event Sent");
}


/**
 * Test CF_AppPipe, SetPollParamCmdNominal
 */
void Test_CF_AppPipe_SetPollParamCmdNominal(void)
{
    CF_SetPollParamCmd_t  CmdMsg;
    cf_config_table_t     *pTbl;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_POLL_PARAM_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 1;
    CmdMsg.Class = CF_CLASS_1;
    CmdMsg.Priority = 1;
    CmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(CmdMsg.PeerEntityId, TestPbPeerEntityId1);
    strcpy(CmdMsg.SrcPath, TestPbDir0);
    strcpy(CmdMsg.DstPath, TestDstDir0);

    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    pTbl = CF_AppData.Tbl;

    sprintf(expEvent, "SetPollParam Cmd Rcvd,Ch=%u,Dir=%u,Cl=%u,Pri=%u,Pre=%u",
            CmdMsg.Chan, CmdMsg.Dir, CmdMsg.Class,
            CmdMsg.Priority, CmdMsg.Preserve);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetPollParamCmdNominal");

    UtAssert_True(pTbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].Preserve
           == CF_KEEP_FILE,
           "CF_AppPipe, SetPollParamCmdNominal: Config Tbl Polling Preserve");

    UtAssert_True(strcmp(
                  pTbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].PeerEntityId,
                  CmdMsg.PeerEntityId) == 0,
       "CF_AppPipe, SetPollParamCmdNominal: Config Tbl Polling PeerEntityId");

    UtAssert_True(strcmp(
            pTbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].SrcPath,
            CmdMsg.SrcPath) == 0,
            "CF_AppPipe, SetPollParamCmdNominal: Config Tbl Polling SrcPath");

    UtAssert_True(strcmp(
            pTbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].DstPath,
            CmdMsg.DstPath) == 0,
            "CF_AppPipe, SetPollParamCmdNominal: Config Tbl Polling DstPath");

    UtAssert_EventSent(CF_SET_POLL_PARAM1_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, SetPollParamCmdNominal: Event Sent");
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdInvLen
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdInvLen(void)
{
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID,
                   sizeof(DeQCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    sprintf(DeQCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER);

    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_DELETE_QUEUE_NODE_CC,
            sizeof(DeQCmdMsg), sizeof(DeQCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DeleteQueueNodeCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdTransUnterm
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdTransUnterm(void)
{
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    CFE_PSP_MemSet(DeQCmdMsg.Trans, 0xFF, MAX_FILE_NAME_LENGTH);

    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unterminated string found in %s",
            "DequeueNode Cmd, Trans parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdTransUnterm");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdTransUnterm: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdInvFilename
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdInvFilename(void)
{
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    strcpy(DeQCmdMsg.Trans, TestPbDir0);
    strcat(DeQCmdMsg.Trans, "p bfile1.dat");
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Delete Q Node */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Filename in %s must be terminated and have no spaces",
            "DequeueNodeCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdInvFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, DeleteQueueNodeCmdInvFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdFileNotFound
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdFileNotFound(void)
{
    uint32               QEntryCnt;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    sprintf(DeQCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile2);
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt +
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt +
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    /* Delete Q Node */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Dequeue Node Cmd Error, %s not found.",
            DeQCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdFileNotFound");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, DeleteQueueNodeCmdFileNotFound: QEntryCnt");

    UtAssert_EventSent(CF_DEQ_NODE_ERR1_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdFileNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdTransIdNotFound
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdTransIdNotFound(void)
{
    uint16               PDataLen;
    uint16               hdr_len;
    uint32               QEntryCnt;
    CF_Test_InPDUInfo_t  InPDUInfo;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    CF_Test_InPDUMsg_t   InMdPDUMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build DeleteQ Command Msg */
    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    sprintf(DeQCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER + 1);

    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* DeleteQ Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Dequeue Node Cmd Error, %s not found.",
            DeQCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdTransIdNotFound");

    UtAssert_True(QEntryCnt == 1,
             "CF_AppPipe, DeleteQueueNodeCmdTransIdNotFound: QEntryCnt");

    UtAssert_EventSent(CF_DEQ_NODE_ERR1_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdTransIdNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdUpActive
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdUpActive(void)
{
    uint16               PDataLen;
    uint16               hdr_len;
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_Test_InPDUInfo_t  InPDUInfo;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    CF_Test_InPDUMsg_t   InMdPDUMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventWarn[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build DeleteQ Command Msg */
    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    sprintf(DeQCmdMsg.Trans, "%d.%d_%d",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number);
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU Msg */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* This first dequeue command will produce the warning */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* This identical second cmd will dequeue without warning */
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    sprintf(FullTransString, "%s", DeQCmdMsg.Trans);
    sprintf(expEventWarn, "DequeueNodeCmd:Trans %s is ACTIVE! Must send "
            "cmd again to remove", FullTransString);

    sprintf(expEvent, "DequeueNodeCmd %s Removed from %s Queue,Stat %d",
            FullTransString, "Incoming Active", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdUpActive");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdUpActive: QEntryCnt");

    UtAssert_EventSent(CF_DEQ_NODE_ERR2_EID, CFE_EVS_CRITICAL, expEventWarn,
             "CF_AppPipe, DeleteQueueNodeCmdUpActive: Warning Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE1_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdUpActive: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdUpHist
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdUpHist(void)
{
    uint16               PDataLen;
    uint16               hdr_len;
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_Test_InPDUInfo_t  InPDUInfo;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    CF_Test_InPDUMsg_t   InMdPDUMsg;
    CF_Test_InPDUMsg_t   InFdPDUMsg;
    CF_Test_InPDUMsg_t   InEofPDUMsg;
    char  expEventInTrans[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Incoming FD PDU Msg */
    CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DATA_PDU;
    InPDUInfo.offset = 0;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Incoming EOF PDU Msg */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.cond_code = NO_ERROR;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    InPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InPDUInfo, hdr_len);

    /* Build DeleteQ Command */
    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    sprintf(DeQCmdMsg.Trans, "%d.%d_%d",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number);
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Delete Q Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    sprintf(expEventInTrans, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number, InPDUInfo.dst_filename);

    sprintf(expEvent, "DequeueNodeCmd %s Removed from %s Queue,Stat %d",
            DeQCmdMsg.Trans, "Incoming History", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdUpHist");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdUpHist: QEntryCnt");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION,
             expEventInTrans,
             "CF_AppPipe, DeleteQueueNodeCmdUpHist: InTrans Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE1_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdUpHist: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdPbPendFilename
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdPbPendFilename(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEventPb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    /* Delete Q Node */
    strcpy(DeQCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventPb, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,"
            "Pre %d,Peer %s,File %s", PbFileCmdMsg.Class,
            PbFileCmdMsg.Channel, PbFileCmdMsg.Priority,
            PbFileCmdMsg.Preserve, PbFileCmdMsg.PeerEntityId,
            FullSrcFileName);
    sprintf(expEvent,
            "DequeueNodeCmd %s Removed from Chan %u,%s Queue,Stat %d",
            FullSrcFileName, PbFileCmdMsg.Channel,
            "Outgoing Pending", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbPendFilename");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 0),
          "CF_AppPipe, DeleteQueueNodeCmdPbPendFilename: PbPendQ EntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb,
         "CF_AppPipe, DeleteQueueNodeCmdPbPendFilename: PbFile Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE2_EID, CFE_EVS_DEBUG, expEvent,
        "CF_AppPipe, DeleteQueueNodeCmdPbPendFilename: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdPbPendTransId
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdPbPendTransId(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg1;
    CF_PlaybackFileCmd_t PbFileCmdMsg2;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();
    sprintf(DeQCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_0");
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    CF_TstUtil_CreateTwoPbPendingQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_ShowQs();

    strcpy(FullTransString, DeQCmdMsg.Trans);
    sprintf(expEvent,
            "DequeueNodeCmd %s Removed from Chan %u,%s Queue,Stat %d",
            FullTransString, PbFileCmdMsg1.Channel,
            "Outgoing Pending", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbPendTransId");

    UtAssert_True((QEntryCntBefore == 2) && (QEntryCntAfter == 0),
          "CF_AppPipe, DeleteQueueNodeCmdPbPendTransId: PbPendQ EntryCnt");

    UtAssert_EventSent(CF_DEQ_NODE2_EID, CFE_EVS_DEBUG, expEvent,
                  "CF_AppPipe, DeleteQueueNodeCmdPbPendTransId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdPbActive
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdPbActive(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    TRANSACTION          trans;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventPb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPbTrans[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventWr[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);
    QEntryCntBefore =
         CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    sprintf(DeQCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* This first dequeue command will produce the warning */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* This identical second cmd will dequeue without warning */
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
         CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventPb, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,"
            "Pre %d,Peer %s,File %s", PbFileCmdMsg.Class,
            PbFileCmdMsg.Channel, PbFileCmdMsg.Priority,
            PbFileCmdMsg.Preserve, PbFileCmdMsg.PeerEntityId,
            FullSrcFileName);

    cfdp_trans_from_string(DeQCmdMsg.Trans, &trans);
    sprintf(expEventPbTrans, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName);

    strcpy(FullTransString, DeQCmdMsg.Trans);
    sprintf(expEventWr, "DequeueNodeCmd:Trans %s is ACTIVE! "
            "Must send cmd again to remove", FullTransString);

    sprintf(expEvent, "DequeueNodeCmd %s Removed from Chan %u,%s "
            "Queue,Stat %d", FullTransString, PbFileCmdMsg.Channel,
            "Outgoing Active", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbActive");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbActive: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb,
             "CF_AppPipe, DeleteQueueNodeCmdPbActive: PbFile Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
              expEventPbTrans,
              "CF_AppPipe, DeleteQueueNodeCmdPbActive: Trans Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE_ERR3_EID, CFE_EVS_CRITICAL, expEventWr,
             "CF_AppPipe, DeleteQueueNodeCmdPbActive: Warning Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE2_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdPbActive: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdPbHist
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdPbHist(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    TRANSACTION          trans;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventPbTrans[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);
    CF_TstUtil_FinishPbActiveQueueEntries();
    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    sprintf(DeQCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    cfdp_trans_from_string(DeQCmdMsg.Trans, &trans);
    sprintf(expEventPbTrans, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName);

    strcpy(FullTransString, DeQCmdMsg.Trans);
    sprintf(expEvent, "DequeueNodeCmd %s Removed from Chan %u,%s "
            "Queue,Stat %d", FullTransString, PbFileCmdMsg.Channel,
            "Outgoing History", CF_SUCCESS);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbHist");

    UtAssert_True((QEntryCntBefore == 1) && (QEntryCntAfter == 0),
                  "CF_AppPipe, DeleteQueueNodeCmdPbHist: QEntryCnt");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
             expEventPbTrans,
             "CF_AppPipe, DeleteQueueNodeCmdPbHist: Trans Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE2_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdPbHist: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdPbHistFailedDeliverDelFail
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdPbHistFailedDeliverDelFail(void)
{
    uint32               QEntryCnt;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    TRANSACTION          trans;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  FullTransString[MAX_FILE_NAME_LENGTH];
    char  expEventPb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPbTrans[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventAb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);
    strcpy(DeQCmdMsg.Trans, "0.24_1");
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_ZEROCOPYGETPTR_INDEX, (int32)NULL, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(
                                          UT_CFE_SB_ZEROCOPYGETPTR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_READ_INDEX, OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_READ_INDEX);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbHistoryQueueEntry(&PbFileCmdMsg);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventPb, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,"
            "Pre %d,Peer %s,File %s", PbFileCmdMsg.Class,
            PbFileCmdMsg.Channel, PbFileCmdMsg.Priority,
            PbFileCmdMsg.Preserve, PbFileCmdMsg.PeerEntityId,
            FullSrcFileName);

    strcpy(FullTransString, CF_AppData.Tbl->FlightEntityId);
    strcat(FullTransString, "_1");
    cfdp_trans_from_string(FullTransString, &trans);
    sprintf(expEventPbTrans, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName);

    sprintf(expEventAb, "%s command received.%s", "Abandon", "All");

    sprintf(expEvent, "Dequeue Node Cmd Error, %s not found.",
            DeQCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                 (CF_AppData.Hk.ErrCounter == 1),
                 "CF_AppPipe, DeleteQueueNodeCmdPbHistFailedDeliverDelFail");

    UtAssert_True(QEntryCnt == 1, "CF_AppPipe, DeleteQueueNodeCmdPbHist"
                  "FailedDeliverDelFail: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb,
                       "CF_AppPipe, DeleteQueueNodeCmdPbHistFailedDeliver"
                       "DelFail: PbFile Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                       expEventPbTrans, "CF_AppPipe, DeleteQueueNodeCmd"
                       "PbHistFailedDeliverDelFail: Trans Event Sent");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEventAb,
                       "CF_AppPipe, DeleteQueueNodeCmdPbHistFailedDeliver"
                       "DelFail: Abandon Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DeleteQueueNodeCmdPbHistFailedDeliver"
                       "DelFail: Fail Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, DeleteQueueNodeCmdInvType
 */
void Test_CF_AppPipe_DeleteQueueNodeCmdInvType(void)
{
    uint32               QEntryCnt;
    CF_QueueEntry_t      *pQ;
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_DequeueNodeCmd_t  DeQCmdMsg;
    char  FullSrcFileName[MAX_FILE_NAME_LENGTH];
    char  expEventPb[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&DeQCmdMsg, CF_CMD_MID, sizeof(DeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DeQCmdMsg,
                      (uint16)CF_DELETE_QUEUE_NODE_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    pQ = CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].TailPtr;
    pQ->NodeType = CF_PLAYBACK + 1;

    /* Delete Q Node */
    strcpy(DeQCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&DeQCmdMsg, sizeof(DeQCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcFileName, PbFileCmdMsg.SrcFilename);
    sprintf(expEventPb, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,"
            "Peer %s,File %s", PbFileCmdMsg.Class, PbFileCmdMsg.Channel,
            PbFileCmdMsg.Priority, PbFileCmdMsg.Preserve,
            PbFileCmdMsg.PeerEntityId, FullSrcFileName);

    sprintf(expEvent, "Unexpected NodeType %d in Queue node", pQ->NodeType);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DeleteQueueNodeCmdInvType");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, DeleteQueueNodeCmdInvType: QEntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb,
             "CF_AppPipe, DeleteQueueNodeCmdInvType: PbFile Event Sent");

    UtAssert_EventSent(CF_DEQ_NODE_ERR4_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, DeleteQueueNodeCmdInvType: Fail Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdInvLen
 */
void Test_CF_AppPipe_PurgeQueueCmdInvLen(void)
{
    CF_PurgeQueueCmd_t  PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_PURGE_QUEUE_CC,
            sizeof(PurgeQCmdMsg), sizeof(PurgeQCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PurgeQueueCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdUplinkActiveErr
 */
void Test_CF_AppPipe_PurgeQueueCmdUplinkActiveErr(void)
{
    CF_PurgeQueueCmd_t  PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_INCOMING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_ACTIVEQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "PurgeQueueCmd Err:Cannot purge Incoming ACTIVE Queue");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdUplinkActiveErr");

    UtAssert_EventSent(CF_PURGEQ_ERR1_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, PurgeQueueCmdUplinkActiveErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdUpHistory
 */
void Test_CF_AppPipe_PurgeQueueCmdUpHistory(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              QEntryCntBefore;
    uint32              QEntryCntAfter;
    CF_Test_InPDUInfo_t InPDUInfo1;
    CF_Test_InPDUInfo_t InPDUInfo2;
    CF_PurgeQueueCmd_t  PurgeQCmdMsg;
    CF_Test_InPDUMsg_t  InMdPDUMsg1;
    CF_Test_InPDUMsg_t  InFdPDUMsg1;
    CF_Test_InPDUMsg_t  InEofPDUMsg1;
    CF_Test_InPDUMsg_t  InMdPDUMsg2;
    CF_Test_InPDUMsg_t  InFdPDUMsg2;
    CF_Test_InPDUMsg_t  InEofPDUMsg2;
    char  expEventIn1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventIn2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /**** File 1 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DATA_PDU;
    InPDUInfo1.offset = 0;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo1.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg1, &InPDUInfo1, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.cond_code = NO_ERROR;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;
    InPDUInfo1.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg1, &InPDUInfo1, hdr_len);

    /**** File 2 ****/
    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DATA_PDU;
    InPDUInfo2.offset = 0;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;

    PDataLen = InPDUInfo2.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.cond_code = NO_ERROR;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;
    InPDUInfo2.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg2, &InPDUInfo2, hdr_len);

    /* Build PurgeQ Command Msg */
    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_INCOMING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_HISTORYQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* InPDU1 MD */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* InPDU1 FD */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* InPDU1 EOF */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* InPDU2 MD */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* InPDU2 FD */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* InPDU2 EOF */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Purge Queue */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    sprintf(expEventIn1, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, InPDUInfo1.dst_filename);

    sprintf(expEventIn2, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, InPDUInfo2.dst_filename);

    sprintf(expEvent, "PurgeQueueCmd Removed %u Nodes from "
            "Uplink History Queue", (unsigned int)QEntryCntBefore);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PurgeQueueCmdUpHistory");

    UtAssert_True((QEntryCntBefore == 2) && (QEntryCntAfter == 0),
                  "CF_AppPipe, PurgeQueueCmdUpHistory: QEntryCnt");

    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 6) &&
                  (CF_AppData.Hk.App.PDUsRejected == 0),
                  "CF_AppPipe, PurgeQueueCmdUpHistory: PDUsReceived Cnt");

    UtAssert_True(CF_AppData.Hk.Up.MetaCount == 2,
                  "CF_AppPipe, PurgeQueueCmdUpHistory: Up.MetaCount");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventIn1,
                  "CF_AppPipe, PurgeQueueCmdUpHistory: Trans#1 Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventIn2,
                  "CF_AppPipe, PurgeQueueCmdUpHistory: Trans#2 Event Sent");

    UtAssert_EventSent(CF_PURGEQ1_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, PurgeQueueCmdUpHistory: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdUpInvalidQ
 */
void Test_CF_AppPipe_PurgeQueueCmdUpInvalidQ(void)
{
    CF_PurgeQueueCmd_t  PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_INCOMING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_HISTORYQ + 1;  /* 0=pending,1=active,2=history */

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Queue Param %u in PurgeQueueCmd",
            PurgeQCmdMsg.Queue);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdUpInvalidQ");

    UtAssert_EventSent(CF_PURGEQ_ERR2_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, PurgeQueueCmdUpInvalidQ: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdPbActiveQErr
 */
void Test_CF_AppPipe_PurgeQueueCmdPbActiveQErr(void)
{
    CF_PurgeQueueCmd_t  PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_ACTIVEQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "PurgeQueueCmd Err:Cannot purge Outgoing ACTIVE Queue");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdPbActiveQErr");

    UtAssert_EventSent(CF_PURGEQ_ERR3_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, PurgeQueueCmdPbActiveQErr: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdPbPendingQ
 */
void Test_CF_AppPipe_PurgeQueueCmdPbPendingQ(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg1;
    CF_PlaybackFileCmd_t PbFileCmdMsg2;
    CF_PurgeQueueCmd_t   PurgeQCmdMsg;
    char  FullSrcFileName1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName2[MAX_FILE_NAME_LENGTH];
    char  expEventPb1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventPb2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_PENDINGQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbPendingQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    strcpy(FullSrcFileName1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventPb1, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,"
            "Pre %d,Peer %s,File %s", PbFileCmdMsg1.Class,
            PbFileCmdMsg1.Channel, PbFileCmdMsg1.Priority,
            PbFileCmdMsg1.Preserve, PbFileCmdMsg1.PeerEntityId,
            FullSrcFileName1);

    strcpy(FullSrcFileName2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventPb2, "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,"
            "Pre %d,Peer %s,File %s", PbFileCmdMsg2.Class,
            PbFileCmdMsg2.Channel, PbFileCmdMsg2.Priority,
            PbFileCmdMsg2.Preserve, PbFileCmdMsg2.PeerEntityId,
            FullSrcFileName2);

    sprintf(expEvent,
            "PurgeQueueCmd Removed %u Nodes from Chan %u,%s Queue",
            2, PurgeQCmdMsg.Chan, "Pending");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PurgeQueueCmdPbPendingQ");

    UtAssert_True((QEntryCntBefore == 2) && (QEntryCntAfter == 0),
             "CF_AppPipe, PurgeQueueCmdPbPendingQ: PbPendQ EntryCnt");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb1,
             "CF_AppPipe, PurgeQueueCmdPbPendingQ: PbFile1 Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEventPb2,
             "CF_AppPipe, PurgeQueueCmdPbPendingQ: PbFile2 Event Sent");

    UtAssert_EventSent(CF_PURGEQ2_EID, CFE_EVS_INFORMATION, expEvent,
             "CF_AppPipe, PurgeQueueCmdPbPendingQ: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdPbHistQ
 */
void Test_CF_AppPipe_PurgeQueueCmdPbHistQ(void)
{
    uint32               QEntryCntBefore;
    uint32               QEntryCntAfter;
    CF_PlaybackFileCmd_t PbFileCmdMsg1;
    CF_PlaybackFileCmd_t PbFileCmdMsg2;
    CF_PurgeQueueCmd_t   PurgeQCmdMsg;
    TRANSACTION          trans;
    char  FullTransString1[MAX_FILE_NAME_LENGTH];
    char  FullTransString2[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName1[MAX_FILE_NAME_LENGTH];
    char  FullSrcFileName2[MAX_FILE_NAME_LENGTH];
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_HISTORYQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
    CF_TstUtil_FinishPbActiveQueueEntries();

    QEntryCntBefore =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCntAfter =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_HISTORYQ].EntryCnt;

    sprintf(FullTransString1, "%s%s",
            CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(FullTransString1, &trans);
    strcpy(FullSrcFileName1, PbFileCmdMsg1.SrcFilename);
    sprintf(expEventTr1, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName1);

    sprintf(FullTransString2, "%s%s",
            CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(FullTransString2, &trans);
    strcpy(FullSrcFileName2, PbFileCmdMsg2.SrcFilename);
    sprintf(expEventTr2, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, FullSrcFileName2);

    sprintf(expEvent, "PurgeQueueCmd Removed %u Nodes from "
            "Chan %u,%s Queue", 2, PurgeQCmdMsg.Chan, "History");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PurgeQueueCmdPbHistQ");

    UtAssert_True((QEntryCntBefore == 2) && (QEntryCntAfter == 0),
                  "CF_AppPipe, PurgeQueueCmdPbHistQ: QEntryCnt");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr1,
                  "CF_AppPipe, PurgeQueueCmdPbHistQ: Trans1 Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventTr2,
                  "CF_AppPipe, PurgeQueueCmdPbHistQ: Trans2 Event Sent");

    UtAssert_EventSent(CF_PURGEQ2_EID, CFE_EVS_INFORMATION, expEvent,
             "CF_AppPipe, PurgeQueueCmdPbHistQ: Success Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdPbInvalidQ
 */
void Test_CF_AppPipe_PurgeQueueCmdPbInvalidQ(void)
{
    CF_PurgeQueueCmd_t   PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_HISTORYQ + 1;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Queue Param %u in PurgeQueueCmd,Max %u",
            PurgeQCmdMsg.Queue, CF_PB_HISTORYQ);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdPbInvalidQ");

    UtAssert_EventSent(CF_PURGEQ_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PurgeQueueCmdPbInvalidQ: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdPbInvalidChan
 */
void Test_CF_AppPipe_PurgeQueueCmdPbInvalidChan(void)
{
    CF_PurgeQueueCmd_t   PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING;
    PurgeQCmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    PurgeQCmdMsg.Queue = CF_PB_PENDINGQ;

    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Chan Param %u in PurgeQueueCmd,Max %u",
            PurgeQCmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdPbInvalidChan");

    UtAssert_EventSent(CF_PURGEQ_ERR5_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, PurgeQueueCmdPbInvalidChan: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, PurgeQueueCmdInvalidType
 */
void Test_CF_AppPipe_PurgeQueueCmdInvalidType(void)
{
    CF_PurgeQueueCmd_t   PurgeQCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&PurgeQCmdMsg, CF_CMD_MID,
                   sizeof(PurgeQCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PurgeQCmdMsg,
                      (uint16)CF_PURGE_QUEUE_CC);
    PurgeQCmdMsg.Type = CF_OUTGOING + 1;
    PurgeQCmdMsg.Chan = 0;
    PurgeQCmdMsg.Queue = CF_PB_PENDINGQ;
    CF_Test_PrintCmdMsg((void*)&PurgeQCmdMsg, sizeof(PurgeQCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PurgeQCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Type Param %u in PurgeQueueCmd,"
            "must be uplink %u or playback %u", PurgeQCmdMsg.Type,
            CF_UPLINK, CF_PLAYBACK);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PurgeQueueCmdInvalidType");

    UtAssert_EventSent(CF_PURGEQ_ERR6_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, PurgeQueueCmdInvalidType: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, EnableDequeueCmdInvLen
 */
void Test_CF_AppPipe_EnableDequeueCmdInvLen(void)
{
    CF_EnDisDequeueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DEQUEUE_CC);
    CmdMsg.Chan = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_ENABLE_DEQUEUE_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, EnableDequeueCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, EnableDequeueCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, EnableDequeueCmdInvChan
 */
void Test_CF_AppPipe_EnableDequeueCmdInvChan(void)
{
    CF_EnDisDequeueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DEQUEUE_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Enable Dequeue Cmd Param Err Chan %d,Max is %d",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, EnableDequeueCmdInvChan");

    UtAssert_EventSent(CF_DQ_CMD_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, EnableDequeueCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, EnableDequeueCmdNominal
 */
void Test_CF_AppPipe_EnableDequeueCmdNominal(void)
{
    CF_EnDisDequeueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DEQUEUE_CC);
    CmdMsg.Chan = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Tbl->OuCh[CmdMsg.Chan].DequeueEnable = CF_DISABLED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Channel %d Dequeue Enabled", CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, EnableDequeueCmdNominal");

    UtAssert_True(CF_AppData.Tbl->OuCh[CmdMsg.Chan].DequeueEnable
                  == CF_ENABLED,
                  "CF_AppPipe, EnableDequeueCmdNominal: Table updated");

    UtAssert_True(CFE_TST(CF_AppData.Hk.Chan[CmdMsg.Chan].Flags, 0) == TRUE,
                  "CF_AppPipe, EnableDequeueCmdNominal: Flags set");

    UtAssert_EventSent(CF_ENA_DQ_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, EnableDequeueCmdNominal: Event Sent");
}


/**
 * Test CF_AppPipe, DisableDequeueCmdInvLen
 */
void Test_CF_AppPipe_DisableDequeueCmdInvLen(void)
{
    CF_EnDisDequeueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DEQUEUE_CC);
    CmdMsg.Chan = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_DISABLE_DEQUEUE_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DisableDequeueCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DisableDequeueCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, DisableDequeueCmdInvChan
 */
void Test_CF_AppPipe_DisableDequeueCmdInvChan(void)
{
    CF_EnDisDequeueCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DEQUEUE_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Disable Dequeue Cmd Param Err Chan %d,Max is %d",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DisableDequeueCmdInvChan");

    UtAssert_EventSent(CF_DQ_CMD_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DisableDequeueCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, DisableDequeueCmdNominal
 */
void Test_CF_AppPipe_DisableDequeueCmdNominal(void)
{
    CF_EnDisDequeueCmd_t  EnableCmdMsg;
    CF_EnDisDequeueCmd_t  DisableCmdMsg;
    char  expEventEnable[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventDisable[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Initialize Enable Command */
    CFE_SB_InitMsg((void*)&EnableCmdMsg, CF_CMD_MID,
                   sizeof(EnableCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&EnableCmdMsg,
                      (uint16)CF_ENABLE_DEQUEUE_CC);
    EnableCmdMsg.Chan = 0;

    /* Initialize Disable Command */
    CFE_SB_InitMsg((void*)&DisableCmdMsg, CF_CMD_MID,
                   sizeof(DisableCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&DisableCmdMsg,
                      (uint16)CF_DISABLE_DEQUEUE_CC);
    DisableCmdMsg.Chan = 0;
    CF_Test_PrintCmdMsg((void*)&DisableCmdMsg, sizeof(DisableCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Enable */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&EnableCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Disable */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&DisableCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventEnable, "Channel %d Dequeue Enabled",
            EnableCmdMsg.Chan);
    sprintf(expEventDisable, "Channel %d Dequeue Disabled",
            DisableCmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DisableDequeueCmdNominal");

    UtAssert_True(CF_AppData.Tbl->OuCh[DisableCmdMsg.Chan].DequeueEnable
                  == CF_DISABLED,
                  "CF_AppPipe, DisableDequeueCmdNominal: Table updated");

    UtAssert_True(CFE_TST(CF_AppData.Hk.Chan[DisableCmdMsg.Chan].Flags, 0)
                  == FALSE,
                  "CF_AppPipe, DisableDequeueCmdNominal: Flags cleared");

    UtAssert_EventSent(CF_ENA_DQ_CMD_EID, CFE_EVS_DEBUG, expEventEnable,
               "CF_AppPipe, DisableDequeueCmdNominal: Enable Event Sent");

    UtAssert_EventSent(CF_DIS_DQ_CMD_EID, CFE_EVS_DEBUG, expEventDisable,
               "CF_AppPipe, DisableDequeueCmdNominal: Disable Event Sent");
}


/**
 * Test CF_AppPipe, EnablePollCmdInvLen
 */
void Test_CF_AppPipe_EnablePollCmdInvLen(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_ENABLE_DIR_POLLING_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, EnablePollCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, EnablePollCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, EnablePollCmdInvChan
 */
void Test_CF_AppPipe_EnablePollCmdInvChan(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DIR_POLLING_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Channel Param Err in EnPollCmd.Chan %d,Max %d",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, EnablePollCmdInvChan");

    UtAssert_EventSent(CF_ENA_POLL_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, EnablePollCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, EnablePollCmdInvDir
 */
void Test_CF_AppPipe_EnablePollCmdInvDir(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = CF_MAX_POLLING_DIRS_PER_CHAN;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Directory Param Err in EnPollCmd.Dir %d,0-%d "
            "and 255 allowed",
            CmdMsg.Dir, CF_MAX_POLLING_DIRS_PER_CHAN - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, EnablePollCmdInvDir");

    UtAssert_EventSent(CF_ENA_POLL_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, EnablePollCmdInvDir: Event Sent");
}


/**
 * Test CF_AppPipe, EnablePollCmdNominal
 */
void Test_CF_AppPipe_EnablePollCmdNominal(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EntryInUse
                    = CF_ENTRY_IN_USE;
    CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EnableState
                    = CF_DISABLED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Polling Directory %d on Channel %d Enabled",
            CmdMsg.Dir, CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, EnablePollCmdNominal");

    UtAssert_True(
         CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EnableState
         == CF_ENABLED,
         "CF_AppPipe, EnablePollCmdNominal: Table updated");

    UtAssert_EventSent(CF_ENA_POLL_CMD2_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, EnablePollCmdNominal: Event Sent");
}


/**
 * Test CF_AppPipe, EnablePollCmdAll
 */
void Test_CF_AppPipe_EnablePollCmdAll(void)
{
    int                i;
    uint32             ChecksumBefore = 0;
    uint32             ChecksumAfter = 0;
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0xFF;  /* 0xFF for all directory */
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    for (i = 0; i < CF_MAX_POLLING_DIRS_PER_CHAN; i++)
    {
        CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EntryInUse
                        = CF_ENTRY_IN_USE;
        CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState
                        = CF_DISABLED;

        ChecksumBefore +=
                CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState;
    }

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    for (i = 0; i < CF_MAX_POLLING_DIRS_PER_CHAN; i++)
    {
        ChecksumAfter +=
                CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState;
    }

    sprintf(expEvent, "All In-use Polling Directories on Channel %d Enabled",
            CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, EnablePollCmdAll");

    UtAssert_True((ChecksumBefore == 0) && (ChecksumAfter == 16),
                  "CF_AppPipe, EnablePollCmdAll: Table updated");

    UtAssert_EventSent(CF_ENA_POLL_CMD1_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, EnablePollCmdAll: Event Sent");
}


/**
 * Test CF_AppPipe, DisablePollCmdInvLen
 */
void Test_CF_AppPipe_DisablePollCmdInvLen(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_DISABLE_DIR_POLLING_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DisablePollCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DisablePollCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, DisablePollCmdInvChan
 */
void Test_CF_AppPipe_DisablePollCmdInvChan(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DIR_POLLING_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Channel Param Err in DisPollCmd.Chan %d,Max %d",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DisablePollCmdInvChan");

    UtAssert_EventSent(CF_DIS_POLL_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DisablePollCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, DisablePollCmdInvDir
 */
void Test_CF_AppPipe_DisablePollCmdInvDir(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = CF_MAX_POLLING_DIRS_PER_CHAN;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Directory Param Err in DisPollCmd."
            "Dir %d,0-%d and 255 allowed",
            CmdMsg.Dir, CF_MAX_POLLING_DIRS_PER_CHAN - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, DisablePollCmdInvDir");

    UtAssert_EventSent(CF_DIS_POLL_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, DisablePollCmdInvDir: Event Sent");
}


/**
 * Test CF_AppPipe, DisablePollCmdNominal
 */
void Test_CF_AppPipe_DisablePollCmdNominal(void)
{
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EntryInUse
                    = CF_ENTRY_IN_USE;
    CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EnableState
                    = CF_ENABLED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Polling Directory %d on Channel %d Disabled",
            CmdMsg.Dir, CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DisablePollCmdNominal");

    UtAssert_True(
         CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[CmdMsg.Dir].EnableState
         == CF_DISABLED,
         "CF_AppPipe, DisablePollCmdNominal: Table update");

    UtAssert_EventSent(CF_DIS_POLL_CMD2_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, DisablePollCmdNominal: Event Sent");
}


/**
 * Test CF_AppPipe, DisablePollCmdAll
 */
void Test_CF_AppPipe_DisablePollCmdAll(void)
{
    int                i;
    uint32             ChecksumBefore = 0;
    uint32             ChecksumAfter = 0;
    CF_EnDisPollCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_DISABLE_DIR_POLLING_CC);
    CmdMsg.Chan = 0;
    CmdMsg.Dir = 0xFF;  /* 0xFF for all directory */
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    for (i = 0; i < CF_MAX_POLLING_DIRS_PER_CHAN; i++)
    {
        CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EntryInUse
                        = CF_ENTRY_IN_USE;
        CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState
                        = CF_ENABLED;

        ChecksumBefore +=
             CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState;
    }

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    for (i = 0; i < CF_MAX_POLLING_DIRS_PER_CHAN; i++)
    {
        ChecksumAfter +=
             CF_AppData.Tbl->OuCh[CmdMsg.Chan].PollDir[i].EnableState;
    }

    sprintf(expEvent, "All In-use Polling Directories on Channel %d Disabled",
            CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, DisablePollCmdAll");

    UtAssert_True((ChecksumBefore == 16) && (ChecksumAfter == 0),
                  "CF_AppPipe, DisablePollCmdAll: Table updated");

    UtAssert_EventSent(CF_DIS_POLL_CMD1_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, DisablePollCmdAll: Event Sent");
}


/**
 * Test CF_AppPipe, KickStartCmdInvLen
 */
void Test_CF_AppPipe_KickStartCmdInvLen(void)
{
    CF_KickstartCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_KICKSTART_CC);
    CmdMsg.Chan = 1;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_KICKSTART_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, KickStartCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, KickStartCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, KickStartCmdInvChan
 */
void Test_CF_AppPipe_KickStartCmdInvChan(void)
{
    CF_KickstartCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_KICKSTART_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Chan Param %u in KickstartCmd,Max %u",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, KickStartCmdInvChan");

    UtAssert_EventSent(CF_KICKSTART_ERR1_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, KickStartCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, KickStartCmdNominal
 */
void Test_CF_AppPipe_KickStartCmdNominal(void)
{
    CF_KickstartCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_KICKSTART_CC);
    CmdMsg.Chan = 1;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Chan[CmdMsg.Chan].DataBlast = CF_IN_PROGRESS;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Kickstart cmd received, chan %u", CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, KickStartCmdNominal");

    UtAssert_True(CF_AppData.Chan[CmdMsg.Chan].DataBlast
                  == CF_NOT_IN_PROGRESS,
                  "CF_AppPipe, KickStartCmdNominal: Chan updated");

    UtAssert_EventSent(CF_KICKSTART_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, KickStartCmdNominal: Event Sent");
}


/**
 * Test CF_AppPipe, QuickStatusCmdInvLen
 */
void Test_CF_AppPipe_QuickStatusCmdInvLen(void)
{
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);
    sprintf(QSCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile1);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Quick Status */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_QUICKSTATUS_CC,
            sizeof(QSCmdMsg), sizeof(QSCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, QuickStatusCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, QuickStatusCmdInvLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdUntermString
 */
void Test_CF_AppPipe_QuickStatusCmdUntermString(void)
{
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);
    CFE_PSP_MemSet(QSCmdMsg.Trans, 0xFF, MAX_FILE_NAME_LENGTH);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Unterminated string found in %s",
            "QuickStatusCmd, Trans parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, QuickStatusCmdUntermString");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, QuickStatusCmdUntermString: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdInvFilename
 */
void Test_CF_AppPipe_QuickStatusCmdInvFilename(void)
{
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);
    sprintf(QSCmdMsg.Trans, "%s%s", TestPbDir0, "invalid filename");
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    /* Quick Status */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Filename in %s must be terminated and have no spaces",
            "QuickStatusCmd");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, QuickStatusCmdInvFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, QuickStatusCmdInvFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdFilenameNotFound
 */
void Test_CF_AppPipe_QuickStatusCmdFilenameNotFound(void)
{
    uint32                QEntryCnt;
    CF_QuickStatCmd_t     QSCmdMsg;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);
    sprintf(QSCmdMsg.Trans, "%s%s", TestPbDir0, TestPbFile2);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    QEntryCnt =
         CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    /* Quick Status */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Quick Status Cmd Error,Trans %s Not Found",
            QSCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, QuickStatusCmdFilenameNotFound");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdFilenameNotFound: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_ERR1_EID, CFE_EVS_ERROR, expEvent,
                "CF_AppPipe, QuickStatusCmdFilenameNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdTransIdNotFound
 */
void Test_CF_AppPipe_QuickStatusCmdTransIdNotFound(void)
{
    uint16                PDataLen;
    uint16                hdr_len;
    uint32                QEntryCnt;
    CF_Test_InPDUInfo_t   InPDUInfo;
    CF_QuickStatCmd_t     QSCmdMsg;
    CF_Test_InPDUMsg_t    InMdPDUMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build QuickStatus Cmd Msg */
    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);
    sprintf(QSCmdMsg.Trans, "%s_%d",
            TestInSrcEntityId1, TEST_IN_TRANS_NUMBER + 1);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;

    /* QuickStatus Command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Quick Status Cmd Error,Trans %s Not Found",
            QSCmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, QuickStatusCmdTransIdNotFound");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdTransIdNotFound: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_ERR1_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, QuickStatusCmdTransIdNotFound: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdPendingFilename
 */
void Test_CF_AppPipe_QuickStatusCmdPendingFilename(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateOnePbPendingQueueEntry(&PbFileCmdMsg);

    /* Quick Status */
    strcpy(QSCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
       CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    sprintf(expEvent, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 0, QSCmdMsg.Trans,
            "PENDING", "NO_ERR");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, QuickStatusCmdPendingFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdPendingFilename: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, QuickStatusCmdPendingFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdPendingTransId
 */
void Test_CF_AppPipe_QuickStatusCmdPendingTransId(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg1;
    CF_PlaybackFileCmd_t  PbFileCmdMsg2;
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEventTr1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventTr2[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_TstUtil_CreateTwoPbPendingQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_PENDINGQ].EntryCnt;

    /* Quick Status */
    sprintf(QSCmdMsg.Trans, "%s%s", CF_AppData.Tbl->FlightEntityId, "_0");
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventTr1, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 0, PbFileCmdMsg1.SrcFilename,
            "PENDING", "NO_ERR");

    sprintf(expEventTr2, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 0, PbFileCmdMsg2.SrcFilename,
            "PENDING", "NO_ERR");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, QuickStatusCmdPendingTransId");

    UtAssert_True(QEntryCnt == 2,
                  "CF_AppPipe, QuickStatusCmdPendingTransId: QEntryEnt");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEventTr1,
             "CF_AppPipe, QuickStatusCmdPendingTransId: Trans 1 Event Sent");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEventTr2,
             "CF_AppPipe, QuickStatusCmdPendingTransId: Trans 2 Event Sent");
}


/**
 * Test CF_AppPipe, QuickStatusCmdActiveFilename
 */
void Test_CF_AppPipe_QuickStatusCmdActiveFilename(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_QuickStatCmd_t     QSCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    /* Quick Status */
    strcpy(QSCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
       CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    sprintf(expEvent, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 1, QSCmdMsg.Trans,
            "ACTIVE", "NO_ERR");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, QuickStatusCmdActiveFilename");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdActiveFilename: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEvent,
             "CF_AppPipe, QuickStatusCmdActiveFilename: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdActiveTransId
 */
void Test_CF_AppPipe_QuickStatusCmdActiveTransId(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_QuickStatCmd_t     QSCmdMsg;
    char  FullSrcFilename[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg,
                      (uint16)CF_QUICKSTATUS_CC);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    strcpy(QSCmdMsg.Trans, CF_AppData.Tbl->FlightEntityId);
    strcat(QSCmdMsg.Trans, "_1");
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);
    QEntryCnt =
       CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    strcpy(FullSrcFilename, PbFileCmdMsg.SrcFilename);
    sprintf(expEvent, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 1, FullSrcFilename,
            "ACTIVE", "NO_ERR");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, QuickStatusCmdActiveTransId");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdActiveTransId: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, QuickStatusCmdActiveTransId: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, QuickStatusCmdActiveSuspended
 */
void Test_CF_AppPipe_QuickStatusCmdActiveSuspended(void)
{
    uint32                QEntryCnt;
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    CF_CARSCmd_t          SuspendCmdMsg;
    CF_QuickStatCmd_t     QSCmdMsg;
    char  FullSrcFilename[MAX_FILE_NAME_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&SuspendCmdMsg, CF_CMD_MID,
                   sizeof(SuspendCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&SuspendCmdMsg,
                      (uint16)CF_SUSPEND_CC);

    CFE_SB_InitMsg((void*)&QSCmdMsg, CF_CMD_MID, sizeof(QSCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg, (uint16)CF_QUICKSTATUS_CC);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateOnePbActiveQueueEntry(&PbFileCmdMsg);

    /* Suspend */
    strcpy(SuspendCmdMsg.Trans, PbFileCmdMsg.SrcFilename);

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&SuspendCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Quick Status */
    strcpy(QSCmdMsg.Trans, PbFileCmdMsg.SrcFilename);
    CF_Test_PrintCmdMsg((void*)&QSCmdMsg, sizeof(QSCmdMsg));

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
       CF_AppData.Chan[PbFileCmdMsg.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;

    strcpy(FullSrcFilename, QSCmdMsg.Trans);
    sprintf(expEvent, "Trans %s_%u %s Stat=%s,CondCode=%s",
            CF_AppData.Tbl->FlightEntityId, 1, FullSrcFilename,
            "ACTIVE/SUSPENDED", "NO_ERR");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 3) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, QuickStatusCmdActiveSuspended");

    UtAssert_True(QEntryCnt == 1,
                  "CF_AppPipe, QuickStatusCmdActiveSuspended: QEntryCnt");

    UtAssert_EventSent(CF_QUICK_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, QuickStatusCmdActiveSuspended: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdInvLen
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdInvLen(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_GIVE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_GIVETAKE_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdInvSemId
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdInvSemId(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_GIVE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Chan[CmdMsg.Chan].HandshakeSemId = CF_INVALID;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Param.Chan %d is not using a semaphore "
            "(not being throttled)", CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvSemId");

    UtAssert_EventSent(CF_GIVETAKE_ERR1_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvSemId: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdInvChan
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdInvChan(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = CF_MAX_PLAYBACK_CHANNELS;
    CmdMsg.GiveOrTakeSemaphore = CF_GIVE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid Chan Param %u in GiveTakeCmd,Max %u",
            CmdMsg.Chan, CF_MAX_PLAYBACK_CHANNELS - 1);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvChan");

    UtAssert_EventSent(CF_GIVETAKE_ERR2_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvChan: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdInvGiveTake
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdInvGiveTake(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_TAKE_SEMAPHORE + 1;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Invalid GiveOrTake Param %u in "
            "GiveTakeSemaphoreCmd,Must be %d or %d",
            CmdMsg.GiveOrTakeSemaphore,
            CF_GIVE_SEMAPHORE, CF_TAKE_SEMAPHORE);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GiveTakeSemaphoreCmdInvGiveTake");

    UtAssert_EventSent(CF_GIVETAKE_ERR3_EID, CFE_EVS_ERROR, expEvent,
             "CF_AppPipe, GiveTakeSemaphoreCmdInvGiveTake: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdTakeFail
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeFail(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_TAKE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX, OS_SEM_TIMEOUT, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF Semaphore %s error on chan %u,Rtn Val %d",
            "Take", CmdMsg.Chan, OS_SEM_TIMEOUT);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, GiveTakeSemaphoreCmdTakeFail");

    UtAssert_EventSent(CF_GIVETAKE_ERR4_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdTakeFail: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdTakeNominal
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeNominal(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_TAKE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF Semaphore %s on chan %u was successful",
            "Take", CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GiveTakeSemaphoreCmdTakeNominal");

    UtAssert_EventSent(CF_GIVETAKE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdTakeNominal: Event Sent");
}


/**
 * Test CF_AppPipe, GiveTakeSemaphoreCmdGiveNominal
 */
void Test_CF_AppPipe_GiveTakeSemaphoreCmdGiveNominal(void)
{
    CF_GiveTakeCmd_t  CmdMsg;
    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_GIVETAKE_CC);
    CmdMsg.Chan = 1;
    CmdMsg.GiveOrTakeSemaphore = CF_GIVE_SEMAPHORE;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMGIVE_INDEX, OS_SUCCESS, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(UT_OSAPI_COUNTSEMGIVE_INDEX);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF Semaphore %s on chan %u was successful",
            "Give", CmdMsg.Chan);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, GiveTakeSemaphoreCmdGiveNominal");

    UtAssert_EventSent(CF_GIVETAKE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                  "CF_AppPipe, GiveTakeSemaphoreCmdGiveNominal: Event Sent");
}


/**
 * Test CF_AppPipe, AutoSuspendEnCmdInvLen
 */
void Test_CF_AppPipe_AutoSuspendEnCmdInvLen(void)
{
    CF_AutoSuspendEnCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENADIS_AUTO_SUSPEND_CC);
    CmdMsg.EnableDisable = CF_ENABLED;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_CMD_MID, CF_ENADIS_AUTO_SUSPEND_CC,
            sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, AutoSuspendEnCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, AutoSuspendEnCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, AutoSuspendEnCmdEnable
 */
void Test_CF_AppPipe_AutoSuspendEnCmdEnable(void)  // check this
{
    uint32                 QEntryCnt;
    CF_NoArgsCmd_t         WakeUpReq;
    CF_AutoSuspendEnCmd_t  AutoSusCmdMsg;
    CF_QuickStatCmd_t      QSCmdMsg1;
    CF_QuickStatCmd_t      QSCmdMsg2;
    CF_PlaybackFileCmd_t   PbFileCmdMsg1;
    CF_PlaybackFileCmd_t   PbFileCmdMsg2;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build AutoSuspend Msg */
    CFE_SB_InitMsg((void*)&AutoSusCmdMsg, CF_CMD_MID,
                   sizeof(AutoSusCmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&AutoSusCmdMsg,
                      (uint16)CF_ENADIS_AUTO_SUSPEND_CC);
    AutoSusCmdMsg.EnableDisable = CF_ENABLED;
    CF_Test_PrintCmdMsg((void*)&AutoSusCmdMsg,
                        sizeof(AutoSusCmdMsg));

    /* Build WakeUpRequest Msg */
    CFE_SB_InitMsg((void*)&WakeUpReq, CF_WAKE_UP_REQ_CMD_MID,
                   sizeof(WakeUpReq), TRUE);

    /* Build QuickStatusCmd Msg1 */
    CFE_SB_InitMsg((void*)&QSCmdMsg1, CF_CMD_MID, sizeof(QSCmdMsg1), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg1,
                      (uint16)CF_QUICKSTATUS_CC);

    /* Build QuickStatusCmd Msg2 */
    CFE_SB_InitMsg((void*)&QSCmdMsg2, CF_CMD_MID, sizeof(QSCmdMsg2), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&QSCmdMsg2,
                      (uint16)CF_QUICKSTATUS_CC);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Return a buf pointer to send a PDU through SB */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                             (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    CF_TstUtil_CreateTwoPbActiveQueueEntry(&PbFileCmdMsg1, &PbFileCmdMsg2);
CF_ShowQs();
machine_list__display_list();

    /* AutoSuspend */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&AutoSusCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* WakeupReq Msg 1 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WakeUpReq;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* WakeupReq Msg 2 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WakeUpReq;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* WakeupReq Msg 3 */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&WakeUpReq;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* QuickStatusMsg 1 */
    strcpy(QSCmdMsg1.Trans, PbFileCmdMsg1.SrcFilename);
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg1;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* QuickStatusMsg 2 */
    strcpy(QSCmdMsg2.Trans, PbFileCmdMsg2.SrcFilename);
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&QSCmdMsg2;
    CF_AppPipe(CF_AppData.MsgPtr);

    QEntryCnt =
        CF_AppData.Chan[PbFileCmdMsg1.Channel].PbQ[CF_PB_ACTIVEQ].EntryCnt;
CF_ShowQs();
machine_list__display_list();

    sprintf(expEvent, "Auto Suspend enable flag set to %u",
            (unsigned int)AutoSusCmdMsg.EnableDisable);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 5) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, AutoSuspendEnCmdEnable");

    UtAssert_True(CF_AppData.Hk.AutoSuspend.EnFlag == CF_ENABLED,
                  "CF_AppPipe, AutoSuspendEnCmdEnable: Updated Hk");

    UtAssert_EventSent(CF_ENDIS_AUTO_SUS_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, AutoSuspendEnCmdEnable: Event Sent");
}


/**
 * Test CF_AppPipe, AutoSuspendEnCmdDisable
 */
void Test_CF_AppPipe_AutoSuspendEnCmdDisable(void)
{
    CF_AutoSuspendEnCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_ENADIS_AUTO_SUSPEND_CC);
    CmdMsg.EnableDisable = CF_DISABLED;
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();
    CF_AppData.Hk.AutoSuspend.EnFlag = CF_ENABLED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Auto Suspend enable flag set to %u",
            (unsigned int)CmdMsg.EnableDisable);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, AutoSuspendEnCmdDisable");

    UtAssert_True(CF_AppData.Hk.AutoSuspend.EnFlag == CF_DISABLED,
                  "CF_AppPipe, AutoSuspendEnCmdDisable: Updated Hk");

    UtAssert_EventSent(CF_ENDIS_AUTO_SUS_CMD_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, AutoSuspendEnCmdDisable: Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CF_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_CF_AppPipe_InvalidCmdMessage,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_InvalidCmdMessage");
    UtTest_Add(Test_CF_AppPipe_InvalidCmdCode,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_InvalidCmdCode");

    UtTest_Add(Test_CF_AppPipe_NoopCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_NoopCmd");
    UtTest_Add(Test_CF_AppPipe_NoopCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_NoopCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_RstCtrsCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_RstCtrsCmd");
    UtTest_Add(Test_CF_AppPipe_RstCtrsCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_RstCtrsCmdInvLen");

    UtTest_Add(Test_CF_AppPipe_PbFileCmd,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmd");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdNoMem,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdNoMem");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdParamErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdParamErr");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdChanNotInUse,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdChanNotInUse");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdInvSrcFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdInvSrcFilename");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdInvDstFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdInvDstFilename");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdPendQFull,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdPendQFull");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdInvPeerId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdInvPeerId");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdFileOpen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdFileOpen");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdFileOnQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdFileOnQ");

    UtTest_Add(Test_CF_AppPipe_PbDirCmdNoFileSuccess,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdNoFileSuccess");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdOpenErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdOpenErr");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdParamErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdParamErr");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdChanNotInUse,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdChanNotInUse");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdInvSrcPath,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdInvSrcPath");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdInvDstPath,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdInvDstPath");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdInvPeerId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdInvPeerId");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdQFull,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdQFull");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdNoMem,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdNoMem");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdFileOnQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdFileOnQ");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdFileOpen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdFileOpen");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdSuccess,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdSuccess");

    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdPbSuspend,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdPbSuspend");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdPbFreeze,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdPbFreeze");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdPbSuccess,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdPbSuccess");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdUpFreezeWarn,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdUpFreezeWarn");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdUpSuccess,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdUpSuccess");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdValPending,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdValPending");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmdUpdatePending,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmdUpdatePending");

    UtTest_Add(Test_CF_AppPipe_FreezeCmd,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_FreezeCmd");
    UtTest_Add(Test_CF_AppPipe_FreezeCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_FreezeCmdInvLen");

    UtTest_Add(Test_CF_AppPipe_ThawCmd,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ThawCmd");
    UtTest_Add(Test_CF_AppPipe_ThawCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ThawCmdInvLen");

    UtTest_Add(Test_CF_AppPipe_SuspendCmdTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdTransId");
    UtTest_Add(Test_CF_AppPipe_SuspendCmdTransIdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdTransIdInvLen");
    UtTest_Add(Test_CF_AppPipe_SuspendCmdFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdFilename");
    UtTest_Add(Test_CF_AppPipe_SuspendCmdInvFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdInvFilename");
    UtTest_Add(Test_CF_AppPipe_SuspendCmdUntermTrans,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdUntermTrans");
    UtTest_Add(Test_CF_AppPipe_SuspendCmdAll,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendCmdAll");

    UtTest_Add(Test_CF_AppPipe_ResumeCmdNoTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ResumeCmdNoTransId");
    UtTest_Add(Test_CF_AppPipe_ResumeCmdPbFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ResumeCmdPbFilename");
    UtTest_Add(Test_CF_AppPipe_ResumeCmdUpTransIdIgnore,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ResumeCmdUpTransIdIgnore");
    UtTest_Add(Test_CF_AppPipe_ResumeCmdAll,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_ResumeCmdAll");

    UtTest_Add(Test_CF_AppPipe_CancelCmdNoTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_CancelCmdNoTransId");
    UtTest_Add(Test_CF_AppPipe_CancelCmdAllPb,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_CancelCmdAllPb");
    UtTest_Add(Test_CF_AppPipe_CancelCmdAllUp,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_CancelCmdAllUp");

    UtTest_Add(Test_CF_AppPipe_AbandonCmdNoFile,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AbandonCmdNoFile");
    UtTest_Add(Test_CF_AppPipe_AbandonCmdNoTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AbandonCmdNoTransId");
    UtTest_Add(Test_CF_AppPipe_AbandonCmdAllPb,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AbandonCmdAllPb");
    UtTest_Add(Test_CF_AppPipe_AbandonCmdAllUp,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AbandonCmdAllUp");

    UtTest_Add(Test_CF_AppPipe_SetMibCmdSaveIncompleteFiles,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdSaveIncompleteFiles");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesInvLen");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermParam,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermParam");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermValue,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdSaveIncompleteFilesUntermValue");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdAckLimit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdAckLimit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdAckLimitNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdAckLimitNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdAckTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdAckTimeout");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdAckTimeoutNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdAckTimeoutNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdInactTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdInactTimeout");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdInactTimeoutNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdInactTimeoutNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdNakLimit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdNakLimit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdNakLimitNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdNakLimitNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdNakTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdNakTimeout");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdNakTimeoutNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdNakTimeoutNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdFileChunkSize,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdFileChunkSize");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdFileChunkSizeNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdFileChunkSizeNonDigit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdFileChunkOverLimit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdFileChunkOverLimit");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdMyId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdMyId");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdMyIdInvalid,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdMyIdInvalid");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdMyIdNonDigit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdMyIdNonDigit");

    UtTest_Add(Test_CF_AppPipe_GetMibCmdSaveIncompleteFiles,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdSaveIncompleteFiles");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvLen");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesUntermParam,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesUntermParam");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvParam,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdSaveIncompleteFilesInvParam");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdAckLimit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdAckLimit");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdAckTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdAckTimeout");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdInactTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdInactTimeout");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdNakLimit,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdNakLimit");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdNakTimeout,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdNakTimeout");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdFileChunkSize,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdFileChunkSize");
    UtTest_Add(Test_CF_AppPipe_GetMibCmdMyId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GetMibCmdMyId");

    UtTest_Add(Test_CF_AppPipe_SendCfgParamsCmd,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendCfgParamsCmd");
    UtTest_Add(Test_CF_AppPipe_SendCfgParamsCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendCfgParamsCmdInvLen");

    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdCreatErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdCreatErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdUpQValueErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdUpQValueErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdUpDefFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdUpDefFilename");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdUpCustomFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdUpCustomFilename");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutQValueErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutQValueErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutQTypeErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutQTypeErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutChanErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutChanErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdWriteHdrErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdWriteHdrErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdEntryWriteErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdEntryWriteErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdInvFilenameErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdInvFilenameErr");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutDefFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutDefFilename");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutCustomFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutCustomFilename");
    UtTest_Add(Test_CF_AppPipe_WriteQueueCmdOutWithEntries,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteQueueCmdOutWithEntries");

    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdInvFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdInvFilename");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdCreatFail,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdCreatFail");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdWrHdrFail,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdWrHdrFail");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdInvWhichQs,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdInvWhichQs");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdEntryWriteErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdEntryWriteErr");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdPbDefaultFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdPbDefaultFilename");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdUpDefaultFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdUpDefaultFilename");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdPbCustFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdPbCustFilename");
    UtTest_Add(Test_CF_AppPipe_WriteActiveTransCmdUpCustFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_WriteActiveTransCmdUpCustFilename");

    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdFileNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdFileNotFound");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdTransIdNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdTransIdNotFound");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdUntermString,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdUntermString");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdInvFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdInvFilename");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdFilename");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdPendingQTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdPendingQTransId");
    UtTest_Add(Test_CF_AppPipe_SendTransDiagCmdPbTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SendTransDiagCmdPbTransId");

    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvDir,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvDir");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvClass,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvClass");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvPreserve,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvPreserve");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvSrc,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvSrc");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvDst,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvDst");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdInvId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdInvId");
    UtTest_Add(Test_CF_AppPipe_SetPollParamCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_SetPollParamCmdNominal");

    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdTransUnterm,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdTransUnterm");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdInvFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdInvFilename");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdFileNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdFileNotFound");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdTransIdNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdTransIdNotFound");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdUpActive,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdUpActive");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdUpHist,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdUpHist");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdPbPendFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdPbPendFilename");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdPbPendTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdPbPendTransId");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdPbActive,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdPbActive");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdPbHist,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdPbHist");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdPbHistFailedDeliverDelFail,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdPbHistFailedDeliverDelFail");
    UtTest_Add(Test_CF_AppPipe_DeleteQueueNodeCmdInvType,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DeleteQueueNodeCmdInvType");

    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdUplinkActiveErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdUplinkActiveErr");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdUpHistory,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdUpHistory");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdUpInvalidQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdUpInvalidQ");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdPbActiveQErr,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdPbActiveQErr");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdPbPendingQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdPbPendingQ");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdPbHistQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdPbHistQ");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdPbInvalidQ,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdPbInvalidQ");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdPbInvalidChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdPbInvalidChan");
    UtTest_Add(Test_CF_AppPipe_PurgeQueueCmdInvalidType,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_PurgeQueueCmdInvalidType");

    UtTest_Add(Test_CF_AppPipe_EnableDequeueCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnableDequeueCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_EnableDequeueCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnableDequeueCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_EnableDequeueCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnableDequeueCmdNominal");

    UtTest_Add(Test_CF_AppPipe_DisableDequeueCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisableDequeueCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_DisableDequeueCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisableDequeueCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_DisableDequeueCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisableDequeueCmdNominal");

    UtTest_Add(Test_CF_AppPipe_EnablePollCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnablePollCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_EnablePollCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnablePollCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_EnablePollCmdInvDir,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnablePollCmdInvDir");
    UtTest_Add(Test_CF_AppPipe_EnablePollCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnablePollCmdNominal");
    UtTest_Add(Test_CF_AppPipe_EnablePollCmdAll,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_EnablePollCmdAll");

    UtTest_Add(Test_CF_AppPipe_DisablePollCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisablePollCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_DisablePollCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisablePollCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_DisablePollCmdInvDir,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisablePollCmdInvDir");
    UtTest_Add(Test_CF_AppPipe_DisablePollCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisablePollCmdNominal");
    UtTest_Add(Test_CF_AppPipe_DisablePollCmdAll,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_DisablePollCmdAll");

    UtTest_Add(Test_CF_AppPipe_KickStartCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_KickStartCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_KickStartCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_KickStartCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_KickStartCmdNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_KickStartCmdNominal");

    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdUntermString,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdUntermString");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdInvFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdInvFilename");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdFilenameNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdFilenameNotFound");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdTransIdNotFound,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdTransIdNotFound");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdPendingFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdPendingFilename");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdPendingTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdPendingTransId");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdActiveFilename,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdActiveFilename");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdActiveTransId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdActiveTransId");
    UtTest_Add(Test_CF_AppPipe_QuickStatusCmdActiveSuspended,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_QuickStatusCmdActiveSuspended");

    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdInvSemId,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdInvSemId");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdInvChan,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdInvChan");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdInvGiveTake,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdInvGiveTake");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeFail,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeFail");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdTakeNominal");
    UtTest_Add(Test_CF_AppPipe_GiveTakeSemaphoreCmdGiveNominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_GiveTakeSemaphoreCmdGiveNominal");

    UtTest_Add(Test_CF_AppPipe_AutoSuspendEnCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AutoSuspendEnCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_AutoSuspendEnCmdEnable,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AutoSuspendEnCmdEnable");
    UtTest_Add(Test_CF_AppPipe_AutoSuspendEnCmdDisable,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppPipe_AutoSuspendEnCmdDisable");
}
