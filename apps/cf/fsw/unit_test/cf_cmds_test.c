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
#include "misc.h"

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


CFE_SB_MsgId_t  SendHkHook_MsgId = 0;


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
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CF_TstUtil_CreateOnePendingQueueEntry(&PbFileCmdMsg);

    sprintf(FullSrcFileName, "%s%s", TestPbSrcDir, TestPbSrcFile1);
    sprintf(expEvent,
        "Playback File Cmd Rcvd,Cl %d,Ch %d,Pri %d,Pre %d,Peer %s,File %s",
         PbFileCmdMsg.Class, PbFileCmdMsg.Channel, PbFileCmdMsg.Priority,
         PbFileCmdMsg.Preserve, TestPbPeerEntityId, FullSrcFileName);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbFileCmd");

    UtAssert_EventSent(CF_PLAYBACK_FILE_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbFileCmd: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileNoMem
 */
void Test_CF_AppPipe_PbFileNoMem(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Execute a playback file command so that one queue entry is added
       to the pending queue */
    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX,
                            CFE_ES_ERR_MEM_HANDLE, 1);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullSrcFileName, "%s", PbFileCmdMsg.SrcFilename);
    sprintf(expEvent, "PB File %s Cmd Ignored,Error Allocating Queue Node.",
            FullSrcFileName);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileCmdNoMem");

    UtAssert_EventSent(CF_QDIR_NOMEM1_EID, CFE_EVS_ERROR, expEvent,
                   "CF_AppPipe, PbFileCmdNoMem: AllocQ Error Event Sent");
}


/**
 * Test CF_AppPipe, PbFileCmdInvLen
 */
void Test_CF_AppPipe_PbFileCmdInvLen(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);

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
}


/**
 * Test CF_AppPipe, PbFileCmdParamErr
 */
void Test_CF_AppPipe_PbFileCmdParamErr(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 3;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

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
}


/**
 * Test CF_AppPipe, PbFileChanNotInUse
 */
void Test_CF_AppPipe_PbFileChanNotInUse(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 1;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

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
                  "CF_AppPipe, PbFileChanNotInUse");

    UtAssert_EventSent(CF_PB_FILE_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileChanNotInUse: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileInvSrcFilename
 */
void Test_CF_AppPipe_PbFileInvSrcFilename(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, " testfile1.txt");
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

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
                  "CF_AppPipe, PbFileInvSrcFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileInvSrcFilename: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileInvDstFilename
 */
void Test_CF_AppPipe_PbFileInvDstFilename(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    /* dest filename not terminated */
    CFE_PSP_MemSet(PbFileCmdMsg.DstFilename, 0xFF, OS_MAX_PATH_LEN);

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
                  "CF_AppPipe, PbFileInvDstFilename");

    UtAssert_EventSent(CF_INV_FILENAME_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileInvDstFilename: Event Sent");
}


/**
 * Test CF_AppPipe, PbFilePendQFull
 */
void Test_CF_AppPipe_PbFilePendQFull(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

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
                  "CF_AppPipe, PbFilePendQFull");

    UtAssert_EventSent(CF_PB_FILE_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFilePendQFull: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileInvPeerId
 */
void Test_CF_AppPipe_PbFileInvPeerId(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, "2555.255");
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

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
                  "CF_AppPipe, PbFileInvPeerId");

    UtAssert_EventSent(CF_PB_FILE_ERR6_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileInvPeerId: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileFileOpen
 */
void Test_CF_AppPipe_PbFileFileOpen(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    CFE_SB_InitMsg((void*)&PbFileCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbFileCmdMsg,
                      (uint16)CF_PLAYBACK_FILE_CC);
    PbFileCmdMsg.Class = 1;
    PbFileCmdMsg.Channel = 0;
    PbFileCmdMsg.Priority = 0;
    PbFileCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbFileCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbFileCmdMsg.SrcFilename, TestPbSrcDir);
    strcat(PbFileCmdMsg.SrcFilename, TestPbSrcFile1);
    strcpy(PbFileCmdMsg.DstFilename, TestPbDstDir);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_FDGETINFO_INDEX,
                                 (void*)&OS_FDGetInfoHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullSrcFileName, "%s%s", TestPbSrcDir, TestPbSrcFile1);
    sprintf(expEvent, "CF:Playback File Cmd Error, File is Open:%s",
            FullSrcFileName);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileFileOpen");

    UtAssert_EventSent(CF_PB_FILE_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileFileOpen: Event Sent");
}


/**
 * Test CF_AppPipe, PbFileFileOnQ
 */
void Test_CF_AppPipe_PbFileFileOnQ(void)
{
    CF_PlaybackFileCmd_t  PbFileCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CF_TstUtil_CreateOnePendingQueueEntry(&PbFileCmdMsg);

    /* Add same command */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbFileCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullSrcFileName, "%s%s", TestPbSrcDir, TestPbSrcFile1);
    sprintf(expEvent, "CF:Playback File Cmd Error, File is Already "
            "Pending or Active:%s", FullSrcFileName);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbFileFileOnQ");

    UtAssert_EventSent(CF_PB_FILE_ERR5_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbFileFileOnQ: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirCmd
 */
void Test_CF_AppPipe_PbDirCmd(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Force OS_opendir to return success, instead of default NULL */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                          UT_OSFILEAPI_OPENDIR_INDEX);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Playback Dir Cmd Rcvd,Ch %d,Cl %d,Pri %d,Pre %d,"
            "Peer %s, Src %s,Dst %s",
            PbDirCmdMsg.Chan, PbDirCmdMsg.Class, PbDirCmdMsg.Priority,
            PbDirCmdMsg.Preserve, PbDirCmdMsg.PeerEntityId,
            TestPbSrcDir, TestPbDstDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmd");

    UtAssert_EventSent(CF_PLAYBACK_DIR_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbDirCmd: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirCmdOpenErr
 */
void Test_CF_AppPipe_PbDirCmdOpenErr(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 0, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                           UT_OSFILEAPI_OPENDIR_INDEX);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Playback Dir Error %d,cannot open directory %s",
            0, TestPbSrcDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirCmdOpenErr");

    UtAssert_EventSent(CF_OPEN_DIR_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirCmdOpenErr: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirCmdInvLen
 */
void Test_CF_AppPipe_PbDirCmdInvLen(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);

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
}


/**
 * Test CF_AppPipe, PbDirCmdParamErr
 */
void Test_CF_AppPipe_PbDirCmdParamErr(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 3;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Execute the function being tested */
    CF_AppInit();

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
}


/**
 * Test CF_AppPipe, PbDirChanNotInUse
 */
void Test_CF_AppPipe_PbDirChanNotInUse(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 1;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].EntryInUse = CF_ENTRY_UNUSED;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent,
            "CF:Playback Dir Cmd Parameter Error, Chan %u is not in use.",
            PbDirCmdMsg.Chan);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirChanNotInUse");

    UtAssert_EventSent(CF_PB_DIR_ERR2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirChanNotInUse: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirInvSrcPath
 */
void Test_CF_AppPipe_PbDirInvSrcPath(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    /* The last char should be forward slash */
    strcpy(PbDirCmdMsg.SrcPath, "/cf/testfile.txt");
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "SrcPath in PB Dir Cmd must be terminated,"
            "have no spaces,slash at end");

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirInvSrcPath");

    UtAssert_EventSent(CF_PB_DIR_ERR3_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirInvSrcPath: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirInvDstPath
 */
void Test_CF_AppPipe_PbDirInvDstPath(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    /* dest filename not terminated */
    CFE_PSP_MemSet(PbDirCmdMsg.DstPath, 0xFF, OS_MAX_PATH_LEN);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "DstPath in PB Dir Cmd must be terminated "
            "and have no spaces");

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirInvDstPath");

    UtAssert_EventSent(CF_PB_DIR_ERR4_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirInvDstPath: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirInvPeerId
 */
void Test_CF_AppPipe_PbDirInvPeerId(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, "2555.255");
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "CF:PB Dir Cmd Err,PeerEntityId %s must be 2 byte,"
            "dotted decimal fmt.ex 0.24", PbDirCmdMsg.PeerEntityId);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirInvPeerId");

    UtAssert_EventSent(CF_PB_DIR_ERR5_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirInvPeerId: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirQFull
 */
void Test_CF_AppPipe_PbDirQFull(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Force OS_opendir to return success, instead of default NULL */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                    UT_OSFILEAPI_OPENDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                   UT_OSFILEAPI_CLOSEDIR_INDEX);

    ReaddirHookCallCnt = 0;
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* makes the pending queue appear full */
    CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[0].EntryCnt =
                    CF_AppData.Tbl->OuCh[PbDirCmdMsg.Chan].PendingQDepth;

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Queue Dir %s Aborted,Ch %d Pending Queue is Full,"
        "%u Entries", TestPbSrcDir, PbDirCmdMsg.Chan,
        (unsigned int)CF_AppData.Chan[PbDirCmdMsg.Chan].PbQ[0].EntryCnt);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirQFull");

    UtAssert_EventSent(CF_QDIR_PQFUL_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirQFull: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirNoMem
 */
void Test_CF_AppPipe_PbDirNoMem(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_OPENDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_CLOSEDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX,
                            CFE_ES_ERR_MEM_HANDLE, 1);

    ReaddirHookCallCnt = 0;
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "PB Dir %s Aborted,Error Allocating Queue Node.",
            TestPbSrcDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, PbDirNoMem");

    UtAssert_EventSent(CF_QDIR_NOMEM2_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, PbDirNoMem: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirFileOnQ
 */
void Test_CF_AppPipe_PbDirFileOnQ(void)
{
    CF_PlaybackFileCmd_t PbFileCmdMsg;
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CF_TstUtil_CreateOnePendingQueueEntry(&PbFileCmdMsg);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                     UT_OSFILEAPI_OPENDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                    UT_OSFILEAPI_CLOSEDIR_INDEX);

    ReaddirHookCallCnt = 0;
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Execute the function being tested */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullSrcFileName, "%s%s", TestPbSrcDir, TestPbSrcFile1);
    sprintf(expEvent, "File %s not queued because it's active or pending",
            FullSrcFileName);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 2) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirFileOnQ");

    UtAssert_EventSent(CF_QDIR_ACTIVEFILE_EID, CFE_EVS_DEBUG, expEvent,
                       "CF_AppPipe, PbDirFileOnQ: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirFileOpen
 */
void Test_CF_AppPipe_PbDirFileOpen(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  FullSrcFileName[OS_MAX_PATH_LEN/2];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                     UT_OSFILEAPI_OPENDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                               OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                    UT_OSFILEAPI_CLOSEDIR_INDEX);

    ReaddirHookCallCnt = 0;
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_FDGETINFO_INDEX,
                                 (void*)&OS_FDGetInfoHook);

    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(FullSrcFileName, "%s%s", TestPbSrcDir, TestPbSrcFile1);
    sprintf(expEvent, "File %s not queued because it's open",
            FullSrcFileName);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirFileOpen");

    UtAssert_EventSent(CF_QDIR_OPENFILE_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, PbDirFileOpen: Event Sent");
}


/**
 * Test CF_AppPipe, PbDirCmdAllGood
 */
void Test_CF_AppPipe_PbDirCmdAllGood(void)
{
    CF_PlaybackDirCmd_t  PbDirCmdMsg;
    char  expEventTerm[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventLen[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* reset the transactions seq number used by the engine */
    misc__set_trans_seq_num(1);

    /* Setup Inputs */
    CFE_SB_InitMsg((void*)&PbDirCmdMsg, CF_CMD_MID,
                   sizeof(CF_PlaybackDirCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PbDirCmdMsg,
                      (uint16)CF_PLAYBACK_DIR_CC);
    PbDirCmdMsg.Class = 1;
    PbDirCmdMsg.Chan = 0;
    PbDirCmdMsg.Priority = 0;
    PbDirCmdMsg.Preserve = CF_KEEP_FILE;
    strcpy(PbDirCmdMsg.PeerEntityId, TestPbPeerEntityId);
    strcpy(PbDirCmdMsg.SrcPath, TestPbSrcDir);
    strcpy(PbDirCmdMsg.DstPath, TestPbDstDir);

    /* Force OS_opendir to return success, instead of default NULL */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_OPENDIR_INDEX, 5, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                          UT_OSFILEAPI_OPENDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                                    OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                         UT_OSFILEAPI_CLOSEDIR_INDEX);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    ReaddirHookCallCnt = 0;
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* force the GetPoolBuf call for the queue entry to return
       something valid */
    CFE_ES_GetPoolBufHookCallCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&PbDirCmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventTerm,
            "File not queued from %s,Filename not terminated or too long",
            TestPbSrcDir);
    sprintf(expEventLen,
            "File not queued from %s,sum of Pathname,Filename too long",
            TestPbSrcDir);
    sprintf(expEvent, "Playback Dir Cmd Rcvd,Ch %d,Cl %d,Pri %d,Pre %d,"
            "Peer %s, Src %s,Dst %s",
            PbDirCmdMsg.Chan, PbDirCmdMsg.Class, PbDirCmdMsg.Priority,
            PbDirCmdMsg.Preserve, PbDirCmdMsg.PeerEntityId,
            TestPbSrcDir, TestPbDstDir);

    /* Verify Outputs */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, PbDirCmdAllGood");

    UtAssert_EventSent(CF_QDIR_INV_NAME1_EID, CFE_EVS_ERROR, expEventTerm,
             "CF_AppPipe, PbDirCmdAllGood: Termination Error Event Sent");

    UtAssert_EventSent(CF_QDIR_INV_NAME2_EID, CFE_EVS_ERROR, expEventLen,
             "CF_AppPipe, PbDirCmdAllGood: Length Error Event Sent");

    UtAssert_EventSent(CF_PLAYBACK_DIR_EID, CFE_EVS_DEBUG, expEvent,
             "CF_AppPipe, PbDirCmdAllGood: Success Event Sent");
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
    CF_HkPacket_t       HkTlm;

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
            memcpy((void *)&HkTlm, (void *)MsgPtr, sizeof(HkTlm));

            printf("CmdCounter: %u\n", HkTlm.CmdCounter);
            printf("ErrCounter: %u\n", HkTlm.ErrCounter);

            printf("App.WakeupForFileProc: %lu\n",
                    HkTlm.App.WakeupForFileProc);
            printf("App.EngineCycleCount: %lu\n",
                    HkTlm.App.EngineCycleCount);
            printf("App.MemInUse: %lu\n", HkTlm.App.MemInUse);
            printf("App.PeakMemInUse: %lu\n", HkTlm.App.PeakMemInUse);
            printf("App.LowMemoryMark: %lu\n", HkTlm.App.LowMemoryMark);
            printf("App.MaxMemNeeded: %lu\n", HkTlm.App.MaxMemNeeded);
            printf("App.MemAllocated: %lu\n", HkTlm.App.MemAllocated);
            printf("App.BufferPoolHandle: %lu\n",
                    HkTlm.App.BufferPoolHandle);
            printf("App.QNodesAllocated: %lu\n", HkTlm.App.QNodesAllocated);
            printf("App.QNodesDeallocated: %lu\n",
                    HkTlm.App.QNodesDeallocated);
            printf("App.PDUsReceived: %lu\n", HkTlm.App.PDUsReceived);
            printf("App.PDUsRejected: %lu\n", HkTlm.App.PDUsRejected);
            printf("App.TotalInProgTrans: %lu\n",
                    HkTlm.App.TotalInProgTrans);
            printf("App.TotalFailedTrans: %lu\n",
                    HkTlm.App.TotalFailedTrans);
            printf("App.TotalAbandonTrans: %lu\n",
                    HkTlm.App.TotalAbandonTrans);
            printf("App.TotalSuccessTrans: %lu\n",
                    HkTlm.App.TotalSuccessTrans);
            printf("App.TotalCompletedTrans: %lu\n",
                    HkTlm.App.TotalCompletedTrans);
            printf("App.LastFailedTrans: %s\n", HkTlm.App.LastFailedTrans);

            printf("AutoSuspend.EnFlag: %lu\n", HkTlm.AutoSuspend.EnFlag);
            printf("AutoSuspend.LowFreeMark: %lu\n",
                    HkTlm.AutoSuspend.LowFreeMark);

            printf("Cond.PosAckNum: %u\n", HkTlm.Cond.PosAckNum);
            printf("Cond.FileStoreRejNum: %u\n", HkTlm.Cond.FileStoreRejNum);
            printf("Cond.FileChecksumNum: %u\n", HkTlm.Cond.FileChecksumNum);
            printf("Cond.FileSizeNum: %u\n", HkTlm.Cond.FileSizeNum);
            printf("Cond.NakLimitNum: %u\n", HkTlm.Cond.NakLimitNum);
            printf("Cond.InactiveNum: %u\n", HkTlm.Cond.InactiveNum);
            printf("Cond.SuspendNum: %u\n", HkTlm.Cond.SuspendNum);
            printf("Cond.CancelNum: %u\n", HkTlm.Cond.CancelNum);

            printf("Eng.FlightEngineEntityId: %s\n",
                    HkTlm.Eng.FlightEngineEntityId);
            printf("Eng.Flags: %lu\n", HkTlm.Eng.Flags);
            printf("Eng.MachinesAllocated: %lu\n",
                    HkTlm.Eng.MachinesAllocated);
            printf("Eng.MachinesDeallocated: %lu\n",
                    HkTlm.Eng.MachinesDeallocated);
            printf("Eng.are_any_partners_frozen: %u\n",
                    HkTlm.Eng.are_any_partners_frozen);
            printf("Eng.how_many_senders: %lu\n",
                    HkTlm.Eng.how_many_senders);
            printf("Eng.how_many_receivers: %lu\n",
                    HkTlm.Eng.how_many_receivers);
            printf("Eng.how_many_frozen: %lu\n",
                    HkTlm.Eng.how_many_frozen);
            printf("Eng.how_many_suspended: %lu\n",
                    HkTlm.Eng.how_many_suspended);
            printf("Eng.total_files_sent: %lu\n",
                    HkTlm.Eng.total_files_sent);
            printf("Eng.total_files_received: %lu\n",
                    HkTlm.Eng.total_files_received);
            printf("Eng.total_unsuccessful_senders: %lu\n",
                    HkTlm.Eng.total_unsuccessful_senders);
            printf("Eng.total_unsuccessful_receivers: %lu\n",
                    HkTlm.Eng.total_unsuccessful_receivers);

            printf("Up.MetaCount: %lu\n", HkTlm.Up.MetaCount);
            printf("Up.UplinkActiveQFileCnt: %lu\n",
                    HkTlm.Up.UplinkActiveQFileCnt);
            printf("Up.SuccessCounter: %lu\n", HkTlm.Up.SuccessCounter);
            printf("Up.FailedCounter: %lu\n", HkTlm.Up.FailedCounter);
            printf("Up.LastFileUplinked: %s\n", HkTlm.Up.LastFileUplinked);

            for (i = 0; i < CF_MAX_PLAYBACK_CHANNELS; i++)
            {
                printf("Chan[%d].PDUsSent: %lu\n",
                        i, HkTlm.Chan[i].PDUsSent);
                printf("Chan[%d].FilesSent: %lu\n",
                        i, HkTlm.Chan[i].FilesSent);
                printf("Chan[%d].SuccessCounter: %lu\n",
                        i, HkTlm.Chan[i].SuccessCounter);
                printf("Chan[%d].FailedCounter: %lu\n",
                        i, HkTlm.Chan[i].FailedCounter);
                printf("Chan[%d].PendingQFileCnt: %lu\n",
                        i, HkTlm.Chan[i].PendingQFileCnt);
                printf("Chan[%d].ActiveQFileCnt: %lu\n",
                        i, HkTlm.Chan[i].ActiveQFileCnt);
                printf("Chan[%d].HistoryQFileCnt: %lu\n",
                        i, HkTlm.Chan[i].HistoryQFileCnt);
                printf("Chan[%d].Flags: %lu\n", i, HkTlm.Chan[i].Flags);
                printf("Chan[%d].RedLightCntr: %lu\n",
                        i, HkTlm.Chan[i].RedLightCntr);
                printf("Chan[%d].GreenLightCntr: %lu\n",
                        i, HkTlm.Chan[i].GreenLightCntr);
                printf("Chan[%d].PollDirsChecked: %lu\n",
                        i, HkTlm.Chan[i].PollDirsChecked);
                printf("Chan[%d].PendingQChecked: %lu\n",
                        i, HkTlm.Chan[i].PendingQChecked);
                printf("Chan[%d].SemValue: %lu\n",
                        i, HkTlm.Chan[i].SemValue);
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
 * Test CF_AppPipe, HousekeepingCmd
 */
void Test_CF_AppPipe_HousekeepingCmd(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    CF_NoArgsCmd_t  InPDUMsg;

    CFE_SB_InitMsg((void*)&CmdMsg, CF_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    CFE_SB_InitMsg((void*)&InPDUMsg, CF_CMD_MID,
                   sizeof(InPDUMsg), TRUE);

    /* Used to verify HK was transmitted correctly. */
    SendHkHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_CF_AppPipe_HousekeepingCmd_SendMsgHook);

    /* Execute the function being tested */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InPDUMsg;
    CF_TstUtil_CreateOneUpHistoryQueueEntry();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Verify results */
    UtAssert_True(CF_AppData.Hk.ErrCounter == 0,
                  "CF_AppPipe, HousekeepingCmd: No ErrCount");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
                  "CF_AppPipe, HousekeepingCmd: Sent HK Telemetry");

    UtAssert_True(CF_AppData.Hk.App.QNodesAllocated == 1,
                  "CF_AppPipe, HousekeepingCmd: Hk.App.QNodesAllocated");

    UtAssert_True(CF_AppData.Hk.App.TotalSuccessTrans == 1,
                "CF_AppPipe, HousekeepingCmd: Hk.App.TotalSuccessTrans");

    UtAssert_True(CF_AppData.Hk.Up.UplinkActiveQFileCnt == 0,
              "CF_AppPipe, HousekeepingCmd: Hk.Up.UplinkActiveQFileCnt");

    UtAssert_True(strcmp(TestUpLinkDestFile, CF_AppData.Hk.Up.LastFileUplinked)
                  == 0,
                  "CF_AppPipe, HousekeepingCmd: Hk.Up.LastFileUplinked");
}


/**
 * Test CF_AppPipe, FreezeCmd
 */
void Test_CF_AppPipe_FreezeCmd(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_FREEZE_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "Freeze command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, FreezeCmd");

    UtAssert_True(CF_AppData.Hk.Eng.Flags == 1,
                  "CF_AppPipe, FreezeCmd: Flags set");

    UtAssert_EventSent(CF_FREEZE_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, FreezeCmd: Event Sent");
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
}


/**
 * Test CF_AppPipe, ThawCmd
 */
void Test_CF_AppPipe_ThawCmd(void)
{
    CF_NoArgsCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_THAW_CC);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.Hk.Eng.Flags = 1;
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;

    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s", "Thaw command received.");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, ThawCmd");

    UtAssert_True(CF_AppData.Hk.Eng.Flags == 0,
                  "CF_AppPipe, ThawCmd: Flags cleared");

    UtAssert_EventSent(CF_THAW_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, ThawCmd: Event Sent");
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
}


/**
 * Test CF_AppPipe, SuspendTransIdCmd
 */
void Test_CF_AppPipe_SuspendTransIdCmd(void)
{
    CF_CARSCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_SUSPEND_CC);
    strcpy(CmdMsg.Trans, "0.3_1");
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s command received.%s", "Suspend", CmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SuspendTransIdCmd");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SuspendTransIdCmd: Event Sent");
}


/**
 * Test CF_AppPipe, SuspendTransIdCmdInvLen
 */
void Test_CF_AppPipe_SuspendTransIdCmdInvLen(void)
{
    CF_CARSCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_SUSPEND_CC);
    strcpy(CmdMsg.Trans, "0.3_1");
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d",
            CF_CMD_MID, CF_SUSPEND_CC, sizeof(CmdMsg), sizeof(CmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SuspendTransIdCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SuspendTransIdCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, SuspendFilenameCmd
 */
void Test_CF_AppPipe_SuspendFilenameCmd(void)
{
    CF_CARSCmd_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_SUSPEND_CC);
//    strcpy(CmdMsg.Trans, "/cf/file.txt");
    strcpy(CmdMsg.Trans, "/ram/downlink/cpd_to_gnd/class1/priority2/");
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s command received.%s", "Suspend", CmdMsg.Trans);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SuspendFilenameCmd");

    UtAssert_EventSent(CF_CARS_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SuspendFilenameCmd: Event Sent");
}


/**
 * Test CF_AppPipe, SetMibCmd
 */
void Test_CF_AppPipe_SetMibCmd(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    strcpy(CmdMsg.Value, "yes");

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Set MIB command received.Param %s Value %s",
            CmdMsg.Param, CmdMsg.Value);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppPipe, SetMibCmd");

    UtAssert_EventSent(CF_SET_MIB_CMD_EID, CFE_EVS_INFORMATION, expEvent,
                       "CF_AppPipe, SetMibCmd: Event Sent");
}


/**
 * Test CF_AppPipe, SetMibCmdInvLen
 */
void Test_CF_AppPipe_SetMibCmdInvLen(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    strcpy(CmdMsg.Value, "yes");

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
                  "CF_AppPipe, SetMibCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetMibCmdInvLen: Event Sent");
}


/**
 * Test CF_AppPipe, SetMibCmdUntermParam
 */
void Test_CF_AppPipe_SetMibCmdUntermParam(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    CFE_PSP_MemSet(CmdMsg.Param, 0xFF, CF_MAX_CFG_PARAM_CHARS);
    strcpy(CmdMsg.Value, "yes");

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "Unterminated string found in SetMib Cmd, Param parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdUntermParam");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetMibCmdUntermParam: Event Sent");
}


/**
 * Test CF_AppPipe, SetMibCmdUntermValue
 */
void Test_CF_AppPipe_SetMibCmdUntermValue(void)
{
    CF_SetMibParam_t  CmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)CF_SET_MIB_PARAM_CC);
    strcpy(CmdMsg.Param, "save_incomplete_files");
    CFE_PSP_MemSet(CmdMsg.Value, 0xFF, CF_MAX_CFG_VALUE_CHARS);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "%s",
            "Unterminated string found in SetMib Cmd, Value parameter");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppPipe, SetMibCmdUntermValue");

    UtAssert_EventSent(CF_NO_TERM_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppPipe, SetMibCmdUntermValue: Event Sent");
}


#if 0
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
    strcpy(CmdMsg.Param, "outgoing_file_chunk_size");
    strcpy(CmdMsg.Value, "10000");

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEvent, "Cannot set OUTGOING_FILE_CHUNK_SIZE(%d) > "
            "CF_MAX_OUTGOING_CHUNK_SIZE(%d)",
            atoi(CmdMsg.Value), CF_MAX_OUTGOING_CHUNK_SIZE);

    /* Verify results */
}
#endif



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
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmd");
    UtTest_Add(Test_CF_AppPipe_PbFileNoMem,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileNoMem");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_PbFileCmdParamErr,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileCmdParamErr");
    UtTest_Add(Test_CF_AppPipe_PbFileChanNotInUse,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileChanNotInUse");
    UtTest_Add(Test_CF_AppPipe_PbFileInvSrcFilename,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileInvSrcFilename");
    UtTest_Add(Test_CF_AppPipe_PbFileInvDstFilename,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileInvDstFilename");
    UtTest_Add(Test_CF_AppPipe_PbFilePendQFull,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFilePendQFull");
    UtTest_Add(Test_CF_AppPipe_PbFileInvPeerId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileInvPeerId");
    UtTest_Add(Test_CF_AppPipe_PbFileFileOpen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileFileOpen");
    UtTest_Add(Test_CF_AppPipe_PbFileFileOnQ,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbFileFileOnQ");
    UtTest_Add(Test_CF_AppPipe_PbDirCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmd");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdOpenErr,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdOpenErr");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdParamErr,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdParamErr");
    UtTest_Add(Test_CF_AppPipe_PbDirChanNotInUse,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirChanNotInUse");
    UtTest_Add(Test_CF_AppPipe_PbDirInvSrcPath,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirInvSrcPath");
    UtTest_Add(Test_CF_AppPipe_PbDirInvDstPath,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirInvDstPath");
    UtTest_Add(Test_CF_AppPipe_PbDirInvPeerId,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirInvPeerId");
    UtTest_Add(Test_CF_AppPipe_PbDirQFull,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirQFull");
    UtTest_Add(Test_CF_AppPipe_PbDirNoMem,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirNoMem");
    UtTest_Add(Test_CF_AppPipe_PbDirFileOnQ,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirFileOnQ");
    UtTest_Add(Test_CF_AppPipe_PbDirFileOpen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirFileOpen");
    UtTest_Add(Test_CF_AppPipe_PbDirCmdAllGood,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_PbDirCmdAllGood");
    UtTest_Add(Test_CF_AppPipe_HousekeepingCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_HousekeepingCmd");
    UtTest_Add(Test_CF_AppPipe_FreezeCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_FreezeCmd");
    UtTest_Add(Test_CF_AppPipe_FreezeCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_FreezeCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_ThawCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_ThawCmd");
    UtTest_Add(Test_CF_AppPipe_ThawCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_ThawCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_SuspendTransIdCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendTransIdCmd");
    UtTest_Add(Test_CF_AppPipe_SuspendTransIdCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendTransIdCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_SuspendFilenameCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SuspendFilenameCmd");
    UtTest_Add(Test_CF_AppPipe_SetMibCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmd");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdInvLen,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdInvLen");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdUntermParam,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdUntermParam");
    UtTest_Add(Test_CF_AppPipe_SetMibCmdUntermValue,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppPipe_SetMibCmdUntermValue");
}
