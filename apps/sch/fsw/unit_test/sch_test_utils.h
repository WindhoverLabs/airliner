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

#ifndef SCH_TEST_UTILS_H
#define SCH_TEST_UTILS_H


#include "sch_app.h"
#include "sch_apipriv.h"


extern SCH_AppData_t  SCH_AppData;
extern SCH_LibData_t  SCH_LibData;

extern int32 SCH_ChildTaskInit(void);
extern void  SCH_ADChildTask(void);
extern void  SCH_ActivityComplete(CFE_SB_MsgId_t MsgID);

extern SCH_ScheduleEntry_t  *SCH_DefaultScheduleTable;
extern SCH_MessageEntry_t   *SCH_DefaultMessageTable;


#ifdef __cplusplus
extern "C" {
#endif

#define SCH_TEST_MID      0x1FFF
#define SCH_TEST_GROUP    0x80000000

void SCH_Test_Setup(void);
void SCH_Test_SetupUnitTest(void);
void SCH_Test_TearDown(void);

void SCH_Test_PrintCmdMsg(void *pMsg, uint32 size);

time_t SCH_Test_GetTimeFromTimestamp(uint64 timestamp);
time_t SCH_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


#ifdef __cplusplus
}
#endif


#endif /* SCH_TEST_UTILS_H */
