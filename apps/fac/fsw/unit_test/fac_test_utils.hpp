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

#ifndef FAC_TEST_UTILS_HPP
#define FAC_TEST_UTILS_HPP

/*
 * Includes
 */

#include "fac_app.hpp"


extern "C" void FAC_AppMain();

extern FAC oFAC;

extern "C" FAC_ParamTbl_t FAC_ParamTbl;


/*
 * Function Definitions
 */

void       FAC_Test_Setup(void);
void       FAC_Test_Setup_TailSitter(void);
void       FAC_Test_TearDown(void);

void       FAC_Test_PrintCmdMsg(void *pMsg, uint32 size);
void       FAC_Test_PrintEclValues();
uint32     FAC_Test_ValidateEclValues();
time_t     FAC_Test_GetTimeFromTimestamp(uint64 timestamp);
time_t     FAC_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


extern "C" uint64 PX4LIB_GetPX4TimeUs(void);
extern "C" uint64 PX4LIB_GetPX4ElapsedTimeUs(uint64 then);


#endif /* FAC_TEST_UTILS_HPP */
