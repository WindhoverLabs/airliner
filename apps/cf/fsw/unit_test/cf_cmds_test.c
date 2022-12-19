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

#include "cf_events.h"
#include "cf_version.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
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
            printf("App.LastFailedTrans: ");
            for (i = 0; i < CF_MAX_TRANSID_CHARS; i++)
            {
                printf("%d ", HkTlm.App.LastFailedTrans[i]);
            }
            printf("\n");

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

            printf("Eng.FlightEngineEntityId: ");
            for (i = 0; i < CF_MAX_CFG_VALUE_CHARS; i++)
            {
                printf("%d ", HkTlm.Eng.FlightEngineEntityId[i]);
            }
            printf("\n");
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
    CF_TstUtil_CreateOneUpActiveQueueEntry();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&CmdMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Verify results */
    UtAssert_True(CF_AppData.Hk.ErrCounter == 0,
                  "CF_AppPipe, HousekeepingCmd: No ErrCount");

    UtAssert_True(SendHkHook_MsgId == CF_HK_TLM_MID,
                  "CF_AppPipe, HousekeepingCmd: Sent HK Telemetry");

    UtAssert_True(CF_AppData.Hk.App.QNodesAllocated == 1,
                  "CF_AppPipe, HousekeepingCmd: Hk.App.QNodesAllocated");

    UtAssert_True(CF_AppData.Hk.Up.UplinkActiveQFileCnt == 1,
                  "CF_AppPipe, HousekeepingCmd: Hk.Up.UplinkActiveQFileCnt");
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
}
