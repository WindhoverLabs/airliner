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

#include "sch_cmds_test.h"
#include "sch_test_utils.h"
#include "sch_app.h"
#include "sch_cmds.h"
#include "sch_msg.h"
#include "sch_msgdefs.h"
#include "sch_msgids.h"
#include "sch_grpids.h"
#include "sch_events.h"
#include "sch_version.h"

#include "uttest.h"
#include "utassert.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"

#include <time.h>


#define SCH_TEST_GROUP    0x01000000


CFE_SB_MsgId_t     SendHK_SendMsgHook_MsgId = 0;
CFE_SB_MsgId_t     SendDiagTlm_SendMsgHook_MsgId = 0;


/**************************************************************************
 * Tests for SCH ProcessCommands()
 **************************************************************************/
/**
 * SCH_ProcessCommands_Test, SendHK_SendMsgHook
 */
int32 SCH_ProcessCommands_Test_SendHK_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    SCH_HkPacket_t     HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = SCH_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case SCH_HK_TLM_MID:
        {
            SendHK_SendMsgHook_MsgId = SCH_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));

            printf("Sent SCH_HK_TLM_MID:\n");
            printf("CmdCounter: %u\n", HkMsg.CmdCounter);
            printf("ErrCounter: %u\n", HkMsg.ErrCounter);
            printf("SyncToMET: %u\n", HkMsg.SyncToMET);
            printf("MajorFrameSource: %u\n", HkMsg.MajorFrameSource);
            printf("ScheduleActivitySuccessCount: %lu\n",
                     HkMsg.ScheduleActivitySuccessCount);
            printf("ScheduleActivityFailureCount: %lu\n",
                     HkMsg.ScheduleActivityFailureCount);
            printf("SlotsProcessedCount: %lu\n", HkMsg.SlotsProcessedCount);
            printf("SkippedSlotsCount: %u\n", HkMsg.SkippedSlotsCount);
            printf("MultipleSlotsCount: %u\n", HkMsg.MultipleSlotsCount);
            printf("SameSlotCount: %u\n", HkMsg.SameSlotCount);
            printf("BadTableDataCount: %u\n", HkMsg.BadTableDataCount);
            printf("TableVerifySuccessCount: %u\n",
                    HkMsg.TableVerifySuccessCount);
            printf("TableVerifyFailureCount: %u\n",
                    HkMsg.TableVerifyFailureCount);
            printf("TablePassCount: %lu\n", HkMsg.TablePassCount);
            printf("ValidMajorFrameCount: %lu\n", HkMsg.ValidMajorFrameCount);
            printf("MissedMajorFrameCount: %lu\n", HkMsg.MissedMajorFrameCount);
            printf("UnexpectedMajorFrameCount: %lu\n",
                    HkMsg.UnexpectedMajorFrameCount);
            printf("MinorFramesSinceTone: %u\n", HkMsg.MinorFramesSinceTone);
            printf("NextSlotNumber: %u\n", HkMsg.NextSlotNumber);
            printf("LastSyncMETSlot: %u\n", HkMsg.LastSyncMETSlot);
            printf("IgnoreMajorFrame: %u\n", HkMsg.IgnoreMajorFrame);
            printf("UnexpectedMajorFrame: %u\n", HkMsg.UnexpectedMajorFrame);
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
 * SCH_ProcessCommands_Test, SendHK
 */
void SCH_ProcessCommands_Test_SendHK(void)
{
    int32             Result;
    int32             CmdPipe;
    SCH_NoArgsCmd_t   CmdMsg = {0};

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_SEND_HK_MID, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                     (void *)&SCH_ProcessCommands_Test_SendHK_SendMsgHook);

    /* Execute the function being tested */
    SCH_AppInit();
    Result = SCH_ProcessCommands();
    
    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 3");

    UtAssert_True(SendHK_SendMsgHook_MsgId == SCH_HK_TLM_MID,
                  "ProcessCommands_Test, SendHK Packet Sent");
}


/**
 * SCH_ProcessCommands_Test, Noop
 */
void SCH_ProcessCommands_Test_Noop(void)
{
    int32             Result;
    int32             CmdPipe;
    SCH_NoArgsCmd_t   CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();
    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "NO-op command. Version %d.%d.%d.%d",
                           SCH_MAJOR_VERSION, SCH_MINOR_VERSION,
                           SCH_REVISION, SCH_MISSION_REV);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, Noop Command counter");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_NOOP_CMD_EID, CFE_EVS_INFORMATION,
              expEventText, "ProcessCommands_Test, Noop Event Sent");
}

/**
 * SCH_ProcessCommands_Test, Reset
 */
void SCH_ProcessCommands_Test_Reset(void)
{
    int32             Result;
    int32             CmdPipe;
    SCH_NoArgsCmd_t   CmdMsg = {0};
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)(&CmdMsg), (uint16)SCH_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    SCH_AppData.CmdCounter = 5;
    SCH_AppData.ValidMajorFrameCount = 100;
    SCH_AppData.MissedMajorFrameCount = 3;
    SCH_AppData.UnexpectedMajorFrameCount = 10;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "%s", "RESET command");

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 0) &&
                  (SCH_AppData.ValidMajorFrameCount == 0) &&
                  (SCH_AppData.MissedMajorFrameCount == 0) &&
                  (SCH_AppData.UnexpectedMajorFrameCount == 0),
                  "ProcessCommands_Test, Reset");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_RESET_CMD_EID, CFE_EVS_DEBUG,
               expEventText, "ProcessCommands_Test, Reset Event Sent");
}

/**
 * SCH_ProcessCommands_Test, Enable
 */
void SCH_ProcessCommands_Test_Enable(void)
{
    uint16               Idx;
    int32                Result;
    int32                CmdPipe;
    SCH_EntryCmd_t       CmdMsg = {0};
    SCH_ScheduleEntry_t  *pSchConfig = NULL;
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)(&CmdMsg), (uint16)SCH_ENABLE_CC);
    CmdMsg.SlotNumber = 0;
    CmdMsg.EntryNumber = 4;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    Idx = (CmdMsg.SlotNumber * SCH_ENTRIES_PER_SLOT)
                  + CmdMsg.EntryNumber;
    pSchConfig = &SCH_AppData.ScheduleTable[Idx];
    pSchConfig->EnableState = SCH_DISABLED;
    pSchConfig->Type = SCH_ACTIVITY_SEND_MSG;
    pSchConfig->Frequency = 1;
    pSchConfig->Remainder = 0;
    pSchConfig->MessageIndex = 97;
    pSchConfig->GroupData = SCH_GROUP_NONE;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "ENABLE command: slot = %d, entry = %d",
                          CmdMsg.SlotNumber, CmdMsg.EntryNumber);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(SCH_AppData.ScheduleTable[Idx].EnableState == SCH_ENABLED,
                  "ProcessCommands_Test, Enable Enabled");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, Enable Command Counter");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_ENABLE_CMD_EID, CFE_EVS_DEBUG,
                  expEventText, "ProcessCommands_Test, Enable Event Sent");

}

/**
 * SCH_ProcessCommands_Test, Disable
 */
void SCH_ProcessCommands_Test_Disable(void)
{
    uint16               Idx;
    int32                Result;
    int32                CmdPipe;
    SCH_EntryCmd_t       CmdMsg = {0};
    SCH_ScheduleEntry_t  *pSchConfig = NULL;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_DISABLE_CC);
    CmdMsg.SlotNumber = 0;
    CmdMsg.EntryNumber = 4;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    Idx = (CmdMsg.SlotNumber * SCH_ENTRIES_PER_SLOT)
           + CmdMsg.EntryNumber;
    pSchConfig = &SCH_AppData.ScheduleTable[Idx];
    pSchConfig->EnableState = SCH_ENABLED;
    pSchConfig->Type = SCH_ACTIVITY_SEND_MSG;
    pSchConfig->Frequency = 1;
    pSchConfig->Remainder = 0;
    pSchConfig->MessageIndex = 97;
    pSchConfig->GroupData = SCH_GROUP_NONE;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "DISABLE command: slot = %d, entry = %d",
                          CmdMsg.SlotNumber, CmdMsg.EntryNumber);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(SCH_AppData.ScheduleTable[Idx].EnableState == SCH_DISABLED,
                  "ProcessCommands_Test, Disable Disabled");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, Disable Command Counter");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_DISABLE_CMD_EID, CFE_EVS_DEBUG,
               expEventText, "ProcessCommands_Test, Disable Event Sent");
}

/**
 * SCH_ProcessCommands_Test, EnableGroup
 */
void SCH_ProcessCommands_Test_EnableGroup(void)
{
    uint16          Idx;
    int32           Result;
    int32           CmdPipe;
    SCH_GroupCmd_t  CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_ENABLE_GROUP_CC);
    CmdMsg.GroupData = SCH_TEST_GROUP;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    SCH_AppData.ScheduleTable[3].EnableState = SCH_DISABLED;
    SCH_AppData.ScheduleTable[3].GroupData = SCH_TEST_GROUP;
    SCH_AppData.ScheduleTable[4].EnableState = SCH_ENABLED;
    SCH_AppData.ScheduleTable[4].GroupData = SCH_TEST_GROUP;
    SCH_AppData.ScheduleTable[5].EnableState = SCH_DISABLED;
    SCH_AppData.ScheduleTable[5].GroupData = SCH_TEST_GROUP;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "ENABLE GROUP command: match count = %d", 3);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, EnableGroup Command Counter");

    UtAssert_True((SCH_AppData.ScheduleTable[3].EnableState == SCH_ENABLED)
                && (SCH_AppData.ScheduleTable[4].EnableState == SCH_ENABLED)
                && (SCH_AppData.ScheduleTable[5].EnableState == SCH_ENABLED),
                "ProcessCommands_Test, EnableGroup Group Enabled");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG, expEventText,
                       "ProcessCommands_Test, EnableGroup Event Sent");

}

/**
 * SCH_ProcessCommands_Test, DisableGroup
 */
void SCH_ProcessCommands_Test_DisableGroup(void)
{
    uint16          Idx;
    int32           Result;
    int32           CmdPipe;
    SCH_GroupCmd_t  CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_DISABLE_GROUP_CC);
    CmdMsg.GroupData = SCH_TEST_GROUP;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    SCH_AppData.ScheduleTable[3].EnableState = SCH_ENABLED;
    SCH_AppData.ScheduleTable[3].GroupData = SCH_TEST_GROUP;
    SCH_AppData.ScheduleTable[4].EnableState = SCH_ENABLED;
    SCH_AppData.ScheduleTable[4].GroupData = SCH_TEST_GROUP;
    SCH_AppData.ScheduleTable[5].EnableState = SCH_ENABLED;
    SCH_AppData.ScheduleTable[5].GroupData = SCH_TEST_GROUP;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "DISABLE GROUP command: match count = %d", 3);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, DisableGroup Command Counter");

    UtAssert_True((SCH_AppData.ScheduleTable[3].EnableState == SCH_DISABLED)
              && (SCH_AppData.ScheduleTable[4].EnableState == SCH_DISABLED)
              && (SCH_AppData.ScheduleTable[5].EnableState == SCH_DISABLED),
              "ProcessCommands_Test, DisableGroup Group Disabled");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG, expEventText,
                       "ProcessCommands_Test, DisableGroup Event Sent");
}

/**
 * SCH_ProcessCommands_Test, EnableSync
 */
void SCH_ProcessCommands_Test_EnableSync(void)
{
    int32             Result;
    int32             CmdPipe;
    SCH_NoArgsCmd_t   CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_ENABLE_SYNC_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();

    SCH_AppData.IgnoreMajorFrame = TRUE;
    SCH_AppData.UnexpectedMajorFrame = TRUE;
    SCH_AppData.ConsecutiveNoisyFrameCounter = 10;

    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "%s", "Major Frame Synchronization Enabled");

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, EnableSync Command Counter");

    UtAssert_True((SCH_AppData.IgnoreMajorFrame == FALSE) &&
                  (SCH_AppData.UnexpectedMajorFrame == FALSE) &&
                  (SCH_AppData.ConsecutiveNoisyFrameCounter == 0),
                  "ProcessCommands_Test, EnableSync Sync Enabled");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_ENA_SYNC_CMD_EID, CFE_EVS_DEBUG, expEventText,
                       "ProcessCommands_Test, EnableSync Event Sent");
}

/**
 * SCH_ProcessCommands_Test, SendDiagTlm_SendMsgHook
 */
int32 SCH_ProcessCommands_Test_SendDiagTlm_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    SCH_DiagPacket_t   DiagMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###SendDiagTlm_SendMsgHook: MsgLen(%u)\n", msgLen);

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = SCH_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case SCH_DIAG_TLM_MID:
        {
            uint16  entryStates;
            uint16  StatesIndex;
            uint32  TblIndex;
            uint32  BitIndex;

            SendDiagTlm_SendMsgHook_MsgId = SCH_DIAG_TLM_MID;
            CFE_PSP_MemCpy((void*)&DiagMsg, (void*)MsgPtr, sizeof(DiagMsg));

            printf("Sent SCH_DIAG_TLM_MID:\n");
            for (TblIndex = 0; TblIndex < SCH_TABLE_ENTRIES; TblIndex++)
            {
                StatesIndex = TblIndex / 8;
                BitIndex = (7 - (TblIndex % 8)) * 2;
                entryStates = (DiagMsg.EntryStates[StatesIndex] >> BitIndex)
                               & 0x00000003;
                printf("EntryStates[%lu]: 0x%04x\n", TblIndex, entryStates);
                printf("MsgIDs[%lu]: 0x%04X\n\n",
                        TblIndex, DiagMsg.MsgIDs[TblIndex]);
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
 * SCH_ProcessCommands_Test, SendDiagTlm
 */
void SCH_ProcessCommands_Test_SendDiagTlm(void)
{
    int32            Result;
    int32            CmdPipe;
    SCH_NoArgsCmd_t  CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)SCH_SEND_DIAG_TLM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                 (void *)&SCH_ProcessCommands_Test_SendDiagTlm_SendMsgHook);

    /* Execute the function being tested */
    SCH_AppInit();
    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "%s", "Transmitting Diagnostic Message");

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 1) &&
                  (SCH_AppData.ErrCounter == 0),
                  "ProcessCommands_Test, SendDiagTlm Command Counter");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_True(SendDiagTlm_SendMsgHook_MsgId == SCH_DIAG_TLM_MID,
                  "ProcessCommands_Test, SendDiagTlm Diag Msg Sent");

    UtAssert_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG, expEventText,
                       "ProcessCommands_Test, SendDiagTlm Event Sent");
}

/**
 * SCH_ProcessCommands_Test, InvalidCommandCode
 */
void SCH_ProcessCommands_Test_InvalidCommandCode(void)
{
    int32            Result;
    int32            CmdPipe;
    SCH_NoArgsCmd_t  CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)(&CmdMsg), (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();
    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "Invalid command code: ID = 0x%04X, CC = %d",
                          SCH_CMD_MID, 100);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True((SCH_AppData.CmdCounter == 0) &&
           (SCH_AppData.ErrCounter == 1),
           "ProcessCommands_Test, InvalidCommandCode Command Counter");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_CC_ERR_EID, CFE_EVS_ERROR, expEventText,
             "ProcessCommands_Test, InvalidCommandCode Event Sent");
}

/**
 * SCH_ProcessCommands_Test, InvalidMessageID
 */
void SCH_ProcessCommands_Test_InvalidMessageID(void)
{
    int32            Result;
    int32            CmdPipe;
    SCH_NoArgsCmd_t  CmdMsg = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(SCH_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, SCH_UNUSED_MID, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    SCH_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    SCH_AppInit();
    Result = SCH_ProcessCommands();
    
    sprintf(expEventText, "Msg with Invalid message ID Rcvd -- ID = 0x%04X",
                          SCH_UNUSED_MID);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 4,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 4");

    UtAssert_EventSent(SCH_MD_ERR_EID, CFE_EVS_ERROR, expEventText,
                  "ProcessCommands_Test, InvalidMessageID Event Sent");
}

/**
 * SCH_HousekeepingCmd_Test
 */
void SCH_HousekeepingCmd_Test(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg(&CmdPacket, SCH_SEND_HK_MID, sizeof(CmdPacket), TRUE);

    /* Execute the function being tested */
    SCH_AppInit();

    SCH_AppData.CmdCounter = 1;
    SCH_AppData.ErrCounter = 2;
    SCH_AppData.ScheduleActivitySuccessCount = 3;
    SCH_AppData.ScheduleActivityFailureCount = 4;
    SCH_AppData.SlotsProcessedCount = 5;
    SCH_AppData.SkippedSlotsCount = 6;
    SCH_AppData.MultipleSlotsCount = 7;
    SCH_AppData.SameSlotCount = 8;
    SCH_AppData.BadTableDataCount = 9;
    SCH_AppData.TableVerifySuccessCount = 10;
    SCH_AppData.TableVerifyFailureCount = 11;
    SCH_AppData.TablePassCount = 12;
    SCH_AppData.ValidMajorFrameCount = 13;
    SCH_AppData.MissedMajorFrameCount = 14;
    SCH_AppData.UnexpectedMajorFrameCount = 15;
    SCH_AppData.MinorFramesSinceTone = 16;
    SCH_AppData.NextSlotNumber = 17;
    SCH_AppData.LastSyncMETSlot = 18;
    SCH_AppData.IgnoreMajorFrame = 19;
    SCH_AppData.UnexpectedMajorFrame = 20;
    SCH_AppData.SyncToMET = 21;
    SCH_AppData.MajorFrameSource = 22;

    Result = SCH_HousekeepingCmd((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.HkPacket.CmdCounter == 1,
                  "SCH_AppData.HkPacket.CmdCounter == 1");
    UtAssert_True(SCH_AppData.HkPacket.ErrCounter == 2,
                  "SCH_AppData.HkPacket.ErrCounter == 2");
    UtAssert_True(SCH_AppData.HkPacket.ScheduleActivitySuccessCount == 3,
                  "SCH_AppData.HkPacket.ScheduleActivitySuccessCount == 3");
    UtAssert_True(SCH_AppData.HkPacket.ScheduleActivityFailureCount == 4,
                  "SCH_AppData.HkPacket.ScheduleActivityFailureCount == 4");
    UtAssert_True(SCH_AppData.HkPacket.SlotsProcessedCount == 5,
                  "SCH_AppData.HkPacket.SlotsProcessedCount == 5");
    UtAssert_True(SCH_AppData.HkPacket.SkippedSlotsCount == 6,
                  "SCH_AppData.HkPacket.SkippedSlotsCount == 6");
    UtAssert_True(SCH_AppData.HkPacket.MultipleSlotsCount == 7,
                  "SCH_AppData.HkPacket.MultipleSlotsCount == 7");
    UtAssert_True(SCH_AppData.HkPacket.SameSlotCount == 8,
                  "SCH_AppData.HkPacket.SameSlotCount == 8");
    UtAssert_True(SCH_AppData.HkPacket.BadTableDataCount == 9,
                  "SCH_AppData.HkPacket.BadTableDataCount == 9");
    UtAssert_True(SCH_AppData.HkPacket.TableVerifySuccessCount == 10,
                  "SCH_AppData.HkPacket.TableVerifySuccessCount == 10");
    UtAssert_True(SCH_AppData.HkPacket.TableVerifyFailureCount == 11,
                  "SCH_AppData.HkPacket.TableVerifyFailureCount == 11");
    UtAssert_True(SCH_AppData.HkPacket.TablePassCount == 12,
                  "SCH_AppData.HkPacket.TablePassCount == 12");
    UtAssert_True(SCH_AppData.HkPacket.ValidMajorFrameCount == 13,
                  "SCH_AppData.HkPacket.ValidMajorFrameCount == 13");
    UtAssert_True(SCH_AppData.HkPacket.MissedMajorFrameCount == 14,
                  "SCH_AppData.HkPacket.MissedMajorFrameCount == 14");
    UtAssert_True(SCH_AppData.HkPacket.UnexpectedMajorFrameCount == 15,
                  "SCH_AppData.HkPacket.UnexpectedMajorFrameCount == 15");
    UtAssert_True(SCH_AppData.HkPacket.MinorFramesSinceTone == 16,
                  "SCH_AppData.HkPacket.MinorFramesSinceTone == 16");
    UtAssert_True(SCH_AppData.HkPacket.NextSlotNumber == 17,
                  "SCH_AppData.HkPacket.NextSlotNumber == 17");
    UtAssert_True(SCH_AppData.HkPacket.LastSyncMETSlot == 18,
                  "SCH_AppData.HkPacket.LastSyncMETSlot == 18");
    UtAssert_True(SCH_AppData.HkPacket.IgnoreMajorFrame == 19,
                  "SCH_AppData.HkPacket.IgnoreMajorFrame == 19");
    UtAssert_True(SCH_AppData.HkPacket.UnexpectedMajorFrame == 20,
                  "SCH_AppData.HkPacket.UnexpectedMajorFrame == 20");
    UtAssert_True(SCH_AppData.HkPacket.SyncToMET == 21,
                  "SCH_AppData.HkPacket.SyncToMET == 21");
    UtAssert_True(SCH_AppData.HkPacket.MajorFrameSource == 22,
                  "SCH_AppData.HkPacket.MajorFrameSource == 22");

    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 3,
                   "Ut_CFE_EVS_GetEventQueueDepth() == 3");
}

/**
 * SCH_NoopCmd_Test_Error
 */
void SCH_NoopCmd_Test_Error(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_NoopCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1,
                   "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */
}

/**
 * SCH_NoopCmd_Test_Nominal
 */
void SCH_NoopCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};
    char             Message[125];

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Execute the function being tested */
    SCH_NoopCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    sprintf(Message, "NO-op command. Version %d.%d.%d.%d",
           SCH_MAJOR_VERSION, SCH_MINOR_VERSION, SCH_REVISION, SCH_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(SCH_NOOP_CMD_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (SCH_AppData.CmdCounter == 1, "SCH_AppData.CmdCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1,
                   "Ut_CFE_EVS_GetEventQueueDepth() == 1");
}

/**
 * SCH_ResetCmd_Test_Error
 */
void SCH_ResetCmd_Test_Error(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_ResetCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */
}

/**
 * SCH_ResetCmd_Test_Nominal
 */
void SCH_ResetCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    /* Execute the function being tested */
    SCH_ResetCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True (SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0,
                 "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0,
                 "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0,
                  "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0,
                  "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0,
                  "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0,
                  "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.BadTableDataCount   == 0,
                  "SCH_AppData.BadTableDataCount   == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0,
                  "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0,
                  "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount      == 0,
                  "SCH_AppData.ValidMajorFrameCount      == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount     == 0,
                  "SCH_AppData.MissedMajorFrameCount     == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount == 0,
                  "SCH_AppData.UnexpectedMajorFrameCount == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_RESET_CMD_EID, CFE_EVS_DEBUG, "RESET command"),
        "RESET command");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_ResetCmd_Test_Nominal */

/**
 * SCH_EnableCmd_Test_InvalidCmdLength
 */
void SCH_EnableCmd_Test_InvalidCmdLength(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableCmd_Test_InvalidCmdLength */

/**
 * SCH_EnableCmd_Test_InvalidArgumentSlotNumber
 */
void SCH_EnableCmd_Test_InvalidArgumentSlotNumber(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = SCH_TOTAL_SLOTS;
    CmdPacket.EntryNumber = 1;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    sprintf(expEventText,
            "ENABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
            CmdPacket.SlotNumber, SCH_TOTAL_SLOTS, CmdPacket.EntryNumber,
            SCH_ENTRIES_PER_SLOT);

    /* Verify results */
    UtAssert_EventSent(SCH_ENABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "EnableCmd_Test_InvalidArgumentSlotNumber Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidArgumentSlotNumber */

/**
 * SCH_EnableCmd_Test_InvalidArgumentEntryNumber
 */
void SCH_EnableCmd_Test_InvalidArgumentEntryNumber(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg(&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 1;
    CmdPacket.EntryNumber = SCH_ENTRIES_PER_SLOT;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    sprintf(expEventText,
            "ENABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
            CmdPacket.SlotNumber, SCH_TOTAL_SLOTS, CmdPacket.EntryNumber,
            SCH_ENTRIES_PER_SLOT);

    /* Verify results */
    UtAssert_EventSent(SCH_ENABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "EnableCmd_Test_InvalidArgumentEntryNumber Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidArgumentEntryNumber */

/**
 * SCH_EnableCmd_Test_InvalidState
 */
void SCH_EnableCmd_Test_InvalidState(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg(&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = 99;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR,
         "ENABLE command: invalid state = 99, slot = 0, entry = 0"),
         "ENABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidState */

/**
 * SCH_EnableCmd_Test_Nominal
 */
void SCH_EnableCmd_Test_Nominal(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg(&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_EID, CFE_EVS_DEBUG,
        "ENABLE command: slot = 0, entry = 0"),
        "ENABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_Nominal */

/**
 * SCH_DisableCmd_Test_InvalidCmdLength
 */
void SCH_DisableCmd_Test_InvalidCmdLength(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_DisableCmd_Test_InvalidCmdLength */

/**
 * SCH_DisableCmd_Test_InvalidArgumentSlotNumber
 */
void SCH_DisableCmd_Test_InvalidArgumentSlotNumber(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.SlotNumber  = SCH_TOTAL_SLOTS;
    CmdPacket.EntryNumber = 1;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    sprintf(expEventText,
            "DISABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
            CmdPacket.SlotNumber, SCH_TOTAL_SLOTS, CmdPacket.EntryNumber,
            SCH_ENTRIES_PER_SLOT);

    /* Verify results */
    UtAssert_EventSent(SCH_DISABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "DisableCmd_Test_InvalidArgumentSlotNumber Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidArgumentSlotNumber */

/**
 * SCH_DisableCmd_Test_InvalidArgumentEntryNumber
 */
void SCH_DisableCmd_Test_InvalidArgumentEntryNumber(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.SlotNumber  = 1;
    CmdPacket.EntryNumber = SCH_ENTRIES_PER_SLOT;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    sprintf(expEventText,
            "DISABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
            CmdPacket.SlotNumber, SCH_TOTAL_SLOTS, CmdPacket.EntryNumber,
            SCH_ENTRIES_PER_SLOT);

    /* Verify results */
    UtAssert_EventSent(SCH_DISABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "DisableCmd_Test_InvalidArgumentEntryNumber Event Sent");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidArgumentEntryNumber */

/**
 * SCH_DisableCmd_Test_InvalidState
 */
void SCH_DisableCmd_Test_InvalidState(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = 99;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR,
        "DISABLE command: invalid state = 99, slot = 0, entry = 0"),
        "DISABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidState */

/**
 * SCH_DisableCmd_Test_Nominal
 */
void SCH_DisableCmd_Test_Nominal(void)
{
    SCH_EntryCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_EID, CFE_EVS_DEBUG,
        "DISABLE command: slot = 0, entry = 0"),
        "DISABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_Nominal */

/**
 * SCH_EnableGroupCmd_Test_InvalidCmdLength
 */
void SCH_EnableGroupCmd_Test_InvalidCmdLength(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableGroupCmd_Test_InvalidCmdLength */

/**
 * SCH_EnableGroupCmd_Test_InvalidArgument
 */
void SCH_EnableGroupCmd_Test_InvalidArgument(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_ERR_EID, CFE_EVS_ERROR,
        "ENABLE GROUP command: invalid argument, no groups selected"),
        "ENABLE GROUP command: invalid argument, no groups selected");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_InvalidArgument */

/**
 * SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused
 */
void SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = SCH_GROUP_NUMBER_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_GROUP_NUMBER_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG,
        "ENABLE GROUP command: match count = 1"),
        "ENABLE GROUP command: match count = 1");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused */

/**
 * SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused
 */
void SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = SCH_MULTI_GROUP_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_MULTI_GROUP_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG,
        "ENABLE GROUP command: match count = 1"),
        "ENABLE GROUP command: match count = 1");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused */

/**
 * SCH_EnableGroupCmd_Test_GroupNotFound
 */
void SCH_EnableGroupCmd_Test_GroupNotFound(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = 0xFFFFFFFF;
    SCH_AppData.ScheduleTable[0].GroupData = 0x00000000;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR,
        "ENABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found"),
        "ENABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_GroupNotFound */

/**
 * SCH_DisableGroupCmd_Test_InvalidCmdLength
 */
void SCH_DisableGroupCmd_Test_InvalidCmdLength(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_DisableGroupCmd_Test_InvalidCmdLength */

/**
 * SCH_DisableGroupCmd_Test_InvalidArgument
 */
void SCH_DisableGroupCmd_Test_InvalidArgument(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_ERR_EID, CFE_EVS_ERROR,
        "DISABLE GROUP command: invalid argument, no groups selected"),
        "DISABLE GROUP command: invalid argument, no groups selected");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_InvalidArgument */

/**
 * SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused
 */
void SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = SCH_GROUP_NUMBER_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_GROUP_NUMBER_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG,
        "DISABLE GROUP command: match count = 1"),
        "DISABLE GROUP command: match count = 1");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused */

/**
 * SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused
 */
void SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = SCH_MULTI_GROUP_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_MULTI_GROUP_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED,
                 "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG,
        "DISABLE GROUP command: match count = 1"),
        "DISABLE GROUP command: match count = 1");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused */

/**
 * SCH_DisableGroupCmd_Test_GroupNotFound
 */
void SCH_DisableGroupCmd_Test_GroupNotFound(void)
{
    SCH_GroupCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    CmdPacket.GroupData = 0xFFFFFFFF;
    SCH_AppData.ScheduleTable[0].GroupData = 0x00000000;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR,
        "DISABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found"),
        "DISABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_GroupNotFound */

/**
 * SCH_EnableSyncCmd_Test_InvalidCmdLength
 */
void SCH_EnableSyncCmd_Test_InvalidCmdLength(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableSyncCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableSyncCmd_Test_InvalidCmdLength */

/**
 * SCH_EnableSyncCmd_Test_Nominal
 */
void SCH_EnableSyncCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    /* Execute the function being tested */
    SCH_EnableSyncCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame == FALSE,
                  "SCH_AppData.IgnoreMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE,
                  "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.ConsecutiveNoisyFrameCounter == 0,
                  "SCH_AppData.ConsecutiveNoisyFrameCounter == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_SYNC_CMD_EID, CFE_EVS_DEBUG,
        "Major Frame Synchronization Enabled"),
        "Major Frame Synchronization Enabled");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableSyncCmd_Test_Nominal */

/**
 * SCH_SendDiagTlmCmd_Test_InvalidCmdLength
 */
void SCH_SendDiagTlmCmd_Test_InvalidCmdLength(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_SendDiagTlmCmd_Test_InvalidCmdLength */

/**
 * SCH_SendDiagTlmCmd_Test_Enabled
 */
void SCH_SendDiagTlmCmd_Test_Enabled(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.EntryStates[0] == (1 << 14),
                 "SCH_AppData.DiagPacket.EntryStates[0] == (1 << 14)");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG,
        "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Enabled */

/**
 * SCH_SendDiagTlmCmd_Test_Disabled
 */
void SCH_SendDiagTlmCmd_Test_Disabled(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.EntryStates[0] == (2 << 14),
                 "SCH_AppData.DiagPacket.EntryStates[0] == (2 << 14)");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG,
        "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Disabled */

/**
 * SCH_SendDiagTlmCmd_Test_Other
 */
void SCH_SendDiagTlmCmd_Test_Other(void)
{
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg((void*)&CmdPacket, SCH_CMD_MID, sizeof(CmdPacket), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = 99;
    SCH_AppData.DiagPacket.MsgIDs[0]         = 0x1111;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.MsgIDs[0] == 0x0000,
                 "SCH_AppData.DiagPacket.MsgIDs[0] == 0x0000");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG,
        "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                 "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Other */

void SCH_VerifyCmdLength_Test_LengthError(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), 1);

    /* Execute the function being tested */
    Result = SCH_VerifyCmdLength((CFE_SB_MsgPtr_t)(&CmdPacket), 99);
    
    /* Verify results */
    if (SCH_CMD_MID == CMD_MSG(102))
    {
        UtAssert_True
            (Ut_CFE_EVS_EventSent(SCH_CMD_LEN_ERR_EID, CFE_EVS_ERROR,
                "Cmd Msg with Bad length Rcvd: ID = 0x1A66, CC = 1, Exp Len = 99, Len = 8"),
            "Cmd Msg with Bad length Rcvd: ID = 0x1A66, CC = 1, Exp Len = 99, Len = 8");
    }
    else
    {
        UtAssert_True
            (Ut_CFE_EVS_EventSent(SCH_CMD_LEN_ERR_EID, CFE_EVS_ERROR,
                "Cmd Msg with Bad length Rcvd: ID = 0x1895, CC = 1, Exp Len = 99, Len = 8"),
            "Cmd Msg with Bad length Rcvd: ID = 0x1895, CC = 1, Exp Len = 99, Len = 8");
    }

    UtAssert_True(Result == SCH_BAD_MSG_LENGTH_RC, "Result == SCH_BAD_MSG_LENGTH_RC");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_VerifyCmdLength_Test_LengthError */

void SCH_VerifyCmdLength_Test_Success(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket = {0};

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), 1);

    /* Execute the function being tested */
    Result = SCH_VerifyCmdLength((CFE_SB_MsgPtr_t)(&CmdPacket), sizeof(SCH_NoArgsCmd_t));
    
    /* Verify results */
    UtAssert_True(Result == SCH_SUCCESS, "Result == SCH_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_VerifyCmdLength_Test_Success */

void SCH_PostCommandResult_Test_GoodCommand(void)
{
    boolean   GoodCommand = TRUE;

    /* Execute the function being tested */
    SCH_PostCommandResult(GoodCommand);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.CmdCounter == 1, "SCH_AppData.CmdCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_PostCommandResult_Test_GoodCommand */

void SCH_PostCommandResult_Test_Error(void)
{
    boolean   GoodCommand = FALSE;

    /* Execute the function being tested */
    SCH_PostCommandResult(GoodCommand);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_PostCommandResult_Test_Error */

void SCH_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(SCH_ProcessCommands_Test_SendHK, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_SendHK");
    UtTest_Add(SCH_ProcessCommands_Test_Noop, SCH_Test_Setup , SCH_Test_TearDown, "SCH_ProcessCommands_Test_Noop");
    UtTest_Add(SCH_ProcessCommands_Test_Reset, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_Reset");
    UtTest_Add(SCH_ProcessCommands_Test_Enable, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_Enable");
    UtTest_Add(SCH_ProcessCommands_Test_Disable, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_Disable");
    UtTest_Add(SCH_ProcessCommands_Test_EnableGroup, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_EnableGroup");
    UtTest_Add(SCH_ProcessCommands_Test_DisableGroup, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_DisableGroup");
    UtTest_Add(SCH_ProcessCommands_Test_EnableSync, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_EnableSync");
    UtTest_Add(SCH_ProcessCommands_Test_SendDiagTlm, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_SendDiagTlm");
    UtTest_Add(SCH_ProcessCommands_Test_InvalidCommandCode, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_InvalidCommandCode");
    UtTest_Add(SCH_ProcessCommands_Test_InvalidMessageID, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_InvalidMessageID");

    UtTest_Add(SCH_HousekeepingCmd_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_HousekeepingCmd_Test");

    UtTest_Add(SCH_NoopCmd_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Error");
    UtTest_Add(SCH_NoopCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Nominal");
    UtTest_Add(SCH_ResetCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ResetCmd_Test_Nominal");

    UtTest_Add(SCH_ResetCmd_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ResetCmd_Test_Error");
    UtTest_Add(SCH_NoopCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Nominal");

    UtTest_Add(SCH_EnableCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableCmd_Test_InvalidArgumentSlotNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidArgumentSlotNumber");
    UtTest_Add(SCH_EnableCmd_Test_InvalidArgumentEntryNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidArgumentEntryNumber");
    UtTest_Add(SCH_EnableCmd_Test_InvalidState, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidState");
    UtTest_Add(SCH_EnableCmd_Test_Nominal, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_EnableCmd_Test_Nominal");

    UtTest_Add(SCH_DisableCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_DisableCmd_Test_InvalidArgumentSlotNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidArgumentSlotNumber");
    UtTest_Add(SCH_DisableCmd_Test_InvalidArgumentEntryNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidArgumentEntryNumber");
    UtTest_Add(SCH_DisableCmd_Test_InvalidState, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidState");
    UtTest_Add(SCH_DisableCmd_Test_Nominal, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_DisableCmd_Test_Nominal");

    UtTest_Add(SCH_EnableGroupCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableGroupCmd_Test_InvalidArgument, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_InvalidArgument");
    UtTest_Add(SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused");
    UtTest_Add(SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused");
    UtTest_Add(SCH_EnableGroupCmd_Test_GroupNotFound, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_GroupNotFound");

    UtTest_Add(SCH_DisableGroupCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_DisableGroupCmd_Test_InvalidArgument, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_InvalidArgument");
    UtTest_Add(SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused");
    UtTest_Add(SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused");
    UtTest_Add(SCH_DisableGroupCmd_Test_GroupNotFound, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_GroupNotFound");

    UtTest_Add(SCH_EnableSyncCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableSyncCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableSyncCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableSyncCmd_Test_Nominal");

    UtTest_Add(SCH_SendDiagTlmCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Enabled, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Enabled");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Disabled, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Disabled");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Other, SCH_Test_SetupUnitTest, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Other");


    UtTest_Add(SCH_VerifyCmdLength_Test_LengthError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_VerifyCmdLength_Test_LengthError");
    UtTest_Add(SCH_VerifyCmdLength_Test_Success, SCH_Test_Setup, SCH_Test_TearDown, "SCH_VerifyCmdLength_Test_Success");

    UtTest_Add(SCH_PostCommandResult_Test_GoodCommand, SCH_Test_Setup, SCH_Test_TearDown, "SCH_PostCommandResult_Test_GoodCommand");
    UtTest_Add(SCH_PostCommandResult_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_PostCommandResult_Test_Error");

} /* end SCH_Cmds_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
