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


#ifndef RCIN_TEST_UTILS_H
#define RCIN_TEST_UTILS_H

/*
 * Includes
 */

#include "rcin_app.h"

extern "C" void RCIN_AppMain();

extern RCIN oRCIN;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void RCIN_Test_Setup(void);
void RCIN_Test_TearDown(void);

void RCIN_Util_Stream_Task(void);

void   RCIN_Test_PrintCmdMsg(void *pMsg, uint32 size);
time_t RCIN_Test_GetTimeFromTimestamp(uint64 timestamp);
time_t RCIN_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);

uint64 PX4LIB_GetPX4TimeUs(void);

#ifdef __cplusplus
}
#endif

#endif /* RCIN_TEST_UTILS_H */

