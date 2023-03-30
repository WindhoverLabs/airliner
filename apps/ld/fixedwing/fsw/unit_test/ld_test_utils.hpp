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


#ifndef LD_TEST_UTILS_HPP
#define LD_TEST_UTILS_HPP

/*
 * Includes
 */

#include "ld_app.h"


extern "C" void LD_AppMain();

extern "C" LD_ConfigTbl_t LD_ConfigTbl;

extern LD oLD;


/*
 * Function Definitions
 */

void    LD_Test_Setup(void);
void    LD_Test_TearDown(void);

void    GetActuatorArmedMsg(PX4_ActuatorArmedMsg_t *pMsg);
void    GetVehicleLocalPositionMsg(PX4_VehicleLocalPositionMsg_t *pMsg);
void    GetAirspeedMsg(PX4_AirspeedMsg_t *pMsg);
void    GetBatteryStatusMsg(PX4_BatteryStatusMsg_t *pMsg);
void    GetManualControlSetpointMsg(PX4_ManualControlSetpointMsg_t *pMsg);

void    LD_Test_PrintCmdMsg(void *pMsg, uint32 size);
uint64  LD_Test_GetTimeUs(void);
time_t  LD_Test_GetTimeFromTimestamp(uint64 timestamp);
time_t  LD_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time);


#endif /* LD_TEST_UTILS_HPP */
