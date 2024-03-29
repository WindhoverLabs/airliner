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


#ifndef AMC_TEST_UTILS_HPP
#define AMC_TEST_UTILS_HPP

/*
 * Includes
 */

#include "amc_app.h"

extern "C" void AMC_AppMain();

extern "C" AMC_ConfigTbl_t AMC_ConfigTbl;
extern "C" AMC_Mixer_ConfigTable_t AMC_MixerCfgTbl;

extern "C" uint64 PX4LIB_GetPX4TimeUs();

/*
 * Function Definitions
 */

void AMC_Test_Setup(void);
void AMC_Test_Setup_CfgTblInvalid(void);
void AMC_Test_TearDown(void);

double GetConfigTblChecksum(AMC *pAMC);
double GetMultirotorMixerConfigTblChecksum(AMC *pAMC);
double GetSimpleMixerConfigTblChecksum(AMC *pAMC);
void   AMC_Test_PrintCmdMsg(void *pMsg, uint32 size);
void   AMC_Test_PrintSimpleMixerOutputScaler(AMC *pAMC, uint8 MixerIndex);
void   AMC_Test_PrintSimpleMixerControls(AMC *pAMC, uint8 MixerIndex, uint8 Control);
void   AMC_Test_PrintSimpleMixerControlsScaler(AMC *pAMC, uint8 MixerIndex, uint8 Control);
time_t AMC_Test_GetTimeFromTimestamp(uint64 timestamp);
time_t AMC_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


#endif /* AMC_TEST_UTILS_HPP */
