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

#include "rcin_custom_hooks.h"
#include "rcin_custom_stubs.h"

#include <string.h>
#include <time.h>
#include <inttypes.h>

#define UT_TEST_BUFFER_SIZE_1MSG         25
#define UT_TEST_BUFFER_SIZE_2MSG         50
#define UT_TEST_BUFFER_SIZE_10MSG        250 

int32  SEDLIB_ReadMsg_Cnt = 0;

uint8 Ut_RxBuffer_1Msg_Nominal[UT_TEST_BUFFER_SIZE_1MSG] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0
};

uint8 Ut_RxBuffer_2Msg_Nominal[UT_TEST_BUFFER_SIZE_2MSG] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 173, 165,  56,  54,   3, 190, 148, 166,  60,  32, 104, 252, 226,  29, 248, 168, 235, 140,  99,  86, 102, 188,   3,  0
};

uint8 Ut_RxBuffer_2Msg_RcLost[UT_TEST_BUFFER_SIZE_2MSG] = {
    15, 164, 169,  76,  26,  98,  97,  17, 206,  73, 115, 161,  86, 229, 132, 245, 223, 251, 117,  25,  64,  50,  54, 217,  0,
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0
};

uint8 Ut_RxBuffer_2Msg_1NoFooter[UT_TEST_BUFFER_SIZE_2MSG] = {
    15, 243, 156,   9, 243, 143, 143, 170, 122, 141,  25,  21, 167, 166, 103, 199,  37, 119,  64, 127, 155, 135,  83, 159,  0,
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10
};

uint8 Ut_RxBuffer_2Msg_1NoHdr[UT_TEST_BUFFER_SIZE_2MSG] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0
};

uint8 Ut_RxBuffer_10Msg_1NoHdr1NoFooter[UT_TEST_BUFFER_SIZE_10MSG] = {
    15, 229, 137, 165, 171, 140,  33,  64,  25,  96,  65, 229,  69,  65,  88,   9, 207,  38, 211,  84, 190, 189,   3, 104,  0,
    42,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155,  0,
    15, 205, 104,   0, 128,  86, 231,  15, 112, 130, 133, 203,  34, 241, 250,   1, 163, 158,  36,  99, 152, 114,  89,  70,  0,
    15, 206, 200, 132,  55, 226,  14, 196, 216, 163,  94, 123, 113, 253,  45,  94,  86, 229, 153, 234, 239, 225, 242,  54,  0,
    15, 230,  35,  63, 195, 226, 255, 126,  74, 180, 238,  22, 133, 136, 183,  12,  50,  15,  62, 253, 226,   6, 252,  76,  0,
    15, 149, 112,  47,  47,  90, 230,  69,  77,  43,  60, 211, 243, 181,  58,  69,  55, 244, 190, 190,  98, 236,  73,  84,  0,
    15, 237, 202,  89, 163, 234,  27, 154,  65, 219, 175, 231, 218, 185,   5, 219, 149,  25, 116,  94,  77,  40,  48, 138,  0,
    15,  60,   8,  46,  25, 208,  91,  76, 234,   9, 246, 251, 176, 105,  50, 151, 246, 149, 227,   9,   9,  90, 112, 155, 10,
    15,   8,  90,  24,  40, 159, 113, 100, 190,  17,  15, 211, 189,  82,  61,  22, 168,  51,   8, 133,  50, 238, 205, 255,  0,
    15, 119,   4,  42, 244, 130, 183, 138,  50,  10, 142, 136,  17,  97,  22,  11,  67,  19, 247, 162, 103, 116, 168,  44,  0
};


SEDLIB_ReturnCode_t SEDLIB_GetPipeHook(char *PipeName,
                                       uint32 Size, uint32 *PipeHandle)
{
    return SEDLIB_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_1Msg_Nominal(uint32 PipeHandle,
                                                    CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, (void *)Ut_RxBuffer_1Msg_Nominal,
           UT_TEST_BUFFER_SIZE_1MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_1MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_Nominal(uint32 PipeHandle,
                                                    CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, (void *)Ut_RxBuffer_2Msg_Nominal,
           UT_TEST_BUFFER_SIZE_2MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_2MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_RcLost(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, (void *)Ut_RxBuffer_2Msg_RcLost,
           UT_TEST_BUFFER_SIZE_2MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_2MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_1NoFooter(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, (void *)Ut_RxBuffer_2Msg_1NoFooter,
           UT_TEST_BUFFER_SIZE_2MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_2MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_1NoHdr(uint32 PipeHandle,
                                                 CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer, (void *)Ut_RxBuffer_2Msg_1NoHdr,
           UT_TEST_BUFFER_SIZE_2MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_2MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_10Msg_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    UART_StatusTlm_t  *pMsg;

    pMsg = (UART_StatusTlm_t *)Msg;
    memcpy((void *)pMsg->RxBuffer,
           (void *)Ut_RxBuffer_10Msg_1NoHdr1NoFooter,
           UT_TEST_BUFFER_SIZE_10MSG);
    pMsg->Hdr.BytesInBuffer = UT_TEST_BUFFER_SIZE_10MSG;

    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_FRESH_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_NoData(uint32 PipeHandle,
                                              CFE_SB_MsgPtr_t Msg)
{
printf("!!!SEDLIB_ReadMsgHook_NoData: SEDLIB_ReadMsg_Cnt(%d)\n", SEDLIB_ReadMsg_Cnt);
    SEDLIB_ReadMsg_Cnt --;
    if (SEDLIB_ReadMsg_Cnt <= 0)
    {
        RCIN_AppCustomData.ContinueFlag = FALSE;
    }

    return SEDLIB_MSG_STALE_OK;
}


void GetMeasureValues(uint8 *data, uint16 *Values)
{
    Values[0] = (uint16)(((data[1] | data[2] << 8)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[1] = (uint16)(((data[2] >> 3 | data[3] << 5)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[2] = (uint16)(((data[3] >> 6 | data[4] << 2
          | data[5] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[3] = (uint16)(((data[5] >> 1 | data[6] << 7)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[4] = (uint16)(((data[6] >> 4 | data[7] << 4)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[5] = (uint16)(((data[7] >> 7 | data[8] << 1
          | data[9] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[6] = (uint16)(((data[9] >> 2 | data[10] << 6)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[7] = (uint16)(((data[10] >> 5 | data[11] << 3)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[8] = (uint16)(((data[12] | data[13] << 8)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[9] = (uint16)(((data[13] >> 3 | data[14] << 5)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[10] = (uint16)(((data[14] >> 6 | data[15] << 2
          | data[16] << 10) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[11] = (uint16)(((data[16] >> 1 | data[17] << 7)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[12] = (uint16)(((data[17] >> 4 | data[18] << 4)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[13] = (uint16)(((data[18] >> 7 | data[19] << 1
          | data[20] << 9) & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f)
          + RCIN_SBUS_SCALE_OFFSET;
    Values[14] = (uint16)(((data[20] >> 2 | data[21] << 6)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;
    Values[15] = (uint16)(((data[21] >> 5 | data[22] << 3)
          & 0x07FF) * RCIN_SBUS_SCALE_FACTOR + .5f) + RCIN_SBUS_SCALE_OFFSET;

    return;
}


uint32 GetChecksum_1Msg_Nominal(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = Ut_RxBuffer_1Msg_Nominal;

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
}


uint32 GetChecksum_2Msg_Nominal(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = &Ut_RxBuffer_2Msg_Nominal[UT_TEST_BUFFER_SIZE_2MSG/2];

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
}


uint32 GetChecksum_2Msg_RcLost(void)
{
    int     i;
    uint32  checksum;
    uint8   *data;
    uint16  Values[PX4_RC_INPUT_MAX_CHANNELS];

    data = &Ut_RxBuffer_2Msg_RcLost[UT_TEST_BUFFER_SIZE_2MSG/2];

    GetMeasureValues(data, Values);

    checksum = 0;
    for (i = 0; i < RCIN_SBUS_CHANNEL_COUNT; i++)
    {
        checksum += Values[i];
    }

    return (checksum);
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


int32 OS_TaskDelayHook(uint32 millisecond)
{
    struct timespec  start_time;
    struct timespec  end_time;
    uint64           elapsed_time;

    clock_gettime(CLOCK_REALTIME, &start_time);
    do
    {
        clock_gettime(CLOCK_REALTIME, &end_time);
        elapsed_time = ((end_time.tv_sec * 1000) + (end_time.tv_nsec / 1000000))
               - ((start_time.tv_sec * 1000) + (start_time.tv_nsec / 1000000));
    } while(elapsed_time < millisecond);
printf("!!!OS_TaskDelayHook: elapsed_time(%" PRId64")\n", elapsed_time);
}
