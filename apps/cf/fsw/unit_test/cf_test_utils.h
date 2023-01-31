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

#ifndef CF_TEST_UTILS_H
#define CF_TEST_UTILS_H

#include "cf_app.h"

extern CF_AppData_t        CF_AppData;
extern cf_config_table_t   CF_ConfigTable;
extern cf_config_table_t   CF_ConfigTableUnitTest;

extern const char TestPbFile1[];
extern const char TestPbFile2[];
extern const char TestPbFile3[];

extern const char TestInFile1[];
extern const char TestInFile2[];
extern const char TestInNoFile[];

extern const char TestQInfoFile1[];

extern const char TestPbDir[];
extern const char TestDstDir[];
extern const char TestInDir[];
extern const char TestQInfoDir[];

extern const char TestPbPeerEntityId[];
extern const char TestInSrcEntityId1[];
extern const char TestInSrcEntityId2[];


typedef struct
{
    uint8         CmdHeader[CFE_SB_CMD_HDR_SIZE];

    union
    {
        CF_PDU_Hdr_t  PHdr;
        uint8         Content[MAX_DATA_LENGTH];
    } PduContent;

} CF_Test_InPDUMsg_t;


#ifdef __cplusplus
extern "C" {
#endif


void CF_Test_Setup(void);
void CF_Test_SetupUnitTest(void);
void CF_Test_TearDown(void);

void CF_ShowQs();

void  CF_TstUtil_InitApp();
int32 CF_TstUtil_VerifyListOrder(char *OrderGiven);

void  CF_TstUtil_CreateOnePbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_CreateTwoPbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                              CF_PlaybackFileCmd_t *pCmd2);
void  CF_TstUtil_CreateOnePbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_CreateTwoPbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                             CF_PlaybackFileCmd_t *pCmd2);
void  CF_TstUtil_FinishPbActiveQueueEntries();
void  CF_TstUtil_FinishOnePbActiveQueueEntryByInd(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_FinishTwoPbActiveQueueEntryByInd(CF_PlaybackFileCmd_t *pCmd1,
                                                 CF_PlaybackFileCmd_t *pCmd2);
void  CF_TstUtil_CreateOnePbHistoryQueueEntry(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_CreateTwoPbHistoryQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                              CF_PlaybackFileCmd_t *pCmd2);

void  CF_TstUtil_CreateOneUpActiveQueueEntry(CF_Test_InPDUMsg_t *pCmd);
void  CF_TstUtil_CreateTwoUpActiveQueueEntry(CF_Test_InPDUMsg_t *pCmd1,
                                             CF_Test_InPDUMsg_t *pCmd2);
void  CF_TstUtil_CreateOneUpActiveQueueEntryWithFile(CF_Test_InPDUMsg_t *pCmd);
void  CF_TstUtil_CreateTwoUpActiveQueueEntryWithFile(CF_Test_InPDUMsg_t *pCmd1,
                                                    CF_Test_InPDUMsg_t *pCmd2);
void  CF_TstUtil_CreateOneUpActiveQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd);
void  CF_TstUtil_CreateTwoUpActiveQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd1,
                                                  CF_Test_InPDUMsg_t *pCmd2);
void  CF_TstUtil_CreateOneUpHistoryQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd);
void  CF_TstUtil_CreateTwoUpHistoryQueueEntryByInd(CF_Test_InPDUMsg_t *pCmd1,
                                                  CF_Test_InPDUMsg_t *pCmd2);
void CF_TstUtil_CreateTwoUpHistoryQueueEntryWithFileByInd(
                       CF_Test_InPDUMsg_t *pCmd1, CF_Test_InPDUMsg_t *pCmd2);

void  CF_ResetEngine(void);

void   CF_Test_PrintCmdMsg(void *pMsg, uint32 size);
time_t CF_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


#ifdef __cplusplus
}
#endif


#endif /* CF_TEST_UTILS_H */
