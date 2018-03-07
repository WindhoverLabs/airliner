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

#ifndef VM_NAVIGATION_H
#define VM_NAVIGATION_H

#include "VM_NavigationContext.h"
#include "cfe.h"

class VM;

class VM_Navigation
{
protected:
	VM_Navigation();

public:
	VM_Navigation(VM &inVmApp);
	~VM_Navigation();

    void EnteredManual();
    void EnteredAltitudeControl();
    void EnteredPositionControl();
    void EnteredAutoLoiter();
    void EnteredAutoReturnToLaunch();
    void EnteredAcrobatic();
    void EnteredStabilize();
    void EnteredRattitude();
    void EnteredAutoTakeoff();
    void EnteredAutoLand();

	uint32 GetCurrentStateID(void);

	void DoAction(void);
	boolean IsTransitionAltCtlValid(void);
	boolean IsTransitionPosCtlValid(void);
	boolean IsTransitionAcrobaticValid(void);

	boolean IsTransitionAutoLoiterValid(void);
	boolean IsTransitionRtlValid(void);
	boolean IsTransitionStabilizeValid(void);
	boolean IsTransitionRattitudeValid(void);
	boolean IsTransitionAutoTakeoffValid(void);
	boolean IsTransitionAutoLandValid(void);

	boolean IsAllMessagesReady(void);
	boolean IsStabilizationRequired(void);
	boolean IsConditionLocalPositionValid(void);
	boolean IsVehicleArmed(void);

	VM_NavigationContext FSM;
	VM &App;
};


#endif
