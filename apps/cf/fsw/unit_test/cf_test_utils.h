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

#define      TEST_TRANS_SEQ_NUM_LENGTH          4
#define      TEST_FILE_SIZE1                    123
#define      TEST_FILE_SIZE2                    420
#define      TEST_FILE_CHUNK_SIZE               200
#define      TEST_IN_TRANS_NUMBER               500

/* The last positive int32 number defined in CF app */
#define      TEST_FD_ZERO_REPLACEMENT           0x7FFFFFFF

extern CF_AppData_t        CF_AppData;
extern cf_config_table_t   CF_ConfigTable;
extern cf_config_table_t   CF_ConfigTableUnitTest;

extern const char TestPbFile1[];
extern const char TestPbFile2[];
extern const char TestPbFile3[];
extern const char TestPbFile4[];
extern const char TestPbFile5[];
extern const char TestPbFile6[];

extern const char TestInFile1[];
extern const char TestInFile2[];
extern const char TestInFileNominal[];
extern const char TestInFileTimerExpired[];
extern const char TestInFileDataLoss[];
extern const char TestInNoFile[];

extern const char TestQInfoFile1[];

extern const char TestPbDir0[];
extern const char TestPbDir1[];
extern const char TestPbDir2[];
extern const char TestPbDir3[];

extern const char TestDstDir0[];
extern const char TestDstDir1[];
extern const char TestDstDir2[];
extern const char TestDstDir3[];

extern const char TestInDir0[];
extern const char TestInDir1[];
extern const char TestInDir2[];
extern const char TestInDir3[];

extern const char TestQInfoDir[];

extern const char TestFlightEntityId[];

extern const char TestPbPeerEntityId1[];
extern const char TestPbPeerEntityId2[];

extern const char TestInSrcEntityId1[];
extern const char TestInSrcEntityId2[];


typedef enum
{
    TEST_TO_RECEIVER,
    TEST_TO_SENDER
} TEST_PDU_DIRECTION;

typedef enum
{
    TEST_ACK_MODE,
    TEST_UNACK_MODE
} TEST_PDU_MODE;

typedef enum
{
    TEST_EOF_PDU = 4,
    TEST_FIN_PDU,
    TEST_ACK_PDU,
    TEST_MD_PDU,
    TEST_NAK_PDU
} TEST_FILE_DIR_CODE;

typedef struct
{
    uint8         CmdHeader[CFE_SB_CMD_HDR_SIZE];

    union
    {
        CF_PDU_Hdr_t  PHdr;
        uint8         Content[MAX_DATA_LENGTH];
    } PduContent;

} CF_Test_InPDUMsg_t;

typedef struct
{
    PDU_TYPE           pdu_type;
    TEST_PDU_DIRECTION direction;
    TEST_PDU_MODE      mode;
    boolean            use_crc;
    TRANSACTION        trans;
    ID                 dest_id;
    boolean            file_transfer;
    boolean            segmentation_control;
    CONDITION_CODE     cond_code;
    uint32             offset;
    uint32             file_size;
    uint32             checksum;
    char               src_filename[MAX_FILE_NAME_LENGTH];
    char               dst_filename[MAX_FILE_NAME_LENGTH];
} CF_Test_InPDUInfo_t;


#ifdef __cplusplus
extern "C" {
#endif


void CF_Test_Setup(void);
void CF_Test_SetupUnitTest(void);
void CF_Test_TearDown(void);

void CF_ShowQs();
void CF_ResetEngine(void);

int32 CF_TstUtil_VerifyListOrder(char *OrderGiven);

void  CF_TstUtil_CreateOnePbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_CreateTwoPbPendingQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                              CF_PlaybackFileCmd_t *pCmd2);
void  CF_TstUtil_CreateOnePbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd);
void  CF_TstUtil_CreateTwoPbActiveQueueEntry(CF_PlaybackFileCmd_t *pCmd1,
                                             CF_PlaybackFileCmd_t *pCmd2);
void  CF_TstUtil_FinishPbActiveQueueEntries();

uint16 CF_TstUtil_GenPDUHeader(CF_Test_InPDUMsg_t *pCmd,
                               CF_Test_InPDUInfo_t *pInfo, uint16 PDataLen);
void   CF_TstUtil_BuildMDPdu(CF_Test_InPDUMsg_t *pCmd,
                             CF_Test_InPDUInfo_t *pInfo, uint16 hdr_len);
void   CF_TstUtil_BuildFDPdu(CF_Test_InPDUMsg_t *pCmd,
                             CF_Test_InPDUInfo_t *pInfo, uint16 hdr_len);
void   CF_TstUtil_BuildEOFPdu(CF_Test_InPDUMsg_t *pCmd,
                              CF_Test_InPDUInfo_t *pInfo, uint16 hdr_len);

char*  CF_Test_ToUpperCase(const char *inStr);
void   CF_Test_PrintCmdMsg(void *pMsg, uint32 size);
time_t CF_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


#ifdef __cplusplus
}
#endif


#endif /* CF_TEST_UTILS_H */
