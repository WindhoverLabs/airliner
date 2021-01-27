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

#ifndef VM_ARMING_H
#define VM_ARMING_H

#include "VM_ArmingContext.h"
#include "cfe.h"

class VM;

class VM_Arming
{
protected:
    /************************************************************************/
    /** \brief VM_Arming default constructor.  Don't use this constructor.
     **
     **  \par Description
     **       This is the default constructor of the VM_Arming class.  Do not
     **       use this constructor because it does not set VM app reference.
     **       The non-default constructor sets the VM app reference.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    VM_Arming();

public:
    /************************************************************************/
    /** \brief VM_Arming constructor.
     **
     **  \par Description
     **       This is the non-default constructor of the VM_Arming class that
     **       sets the VM application reference.  Use this constructor rather
     **       than the default constructor.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    VM_Arming(VM &inApp);

    /************************************************************************/
    /** \brief VM_Arming destructor.
     **
     **  \par Description
     **       This is the VM_Arming class destructor.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    ~VM_Arming();

    /************************************************************************/
    /** \brief Initialize the VM_Arming object.
     **
     **  \par Description
     **       This function should be called after object creation, to
     **       initialize the VM_Arming object.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void Init(void);

    /************************************************************************/
    /** \brief Execute all stateful behavior.
     **
     **  \par Description
     **       This function will execute all stateful behavior defined by the
     **       VM_Arming.sm SMC state machine definition file and implemented
     **       by the auto generated VM_Arming context.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void DoAction(void);

    /************************************************************************/
    /** \brief Get the current state of the VM_Arming state machine.
     **
     **  \par Description
     **       This function will return the current state of the VM_Arming
     **       state machine.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     **  \returns
     **  Return values are defined by the SMC state machine input file
     **  (#VM_Arming.sm)
     **  \endreturns
     *************************************************************************/
    uint32 GetCurrentStateID(void);

    /************************************************************************/
    /** \brief Function called on entry to the Standby state.
     **
     **  \par Description
     **       This function will be called by the VM_Arming state machine
     **       when the Standby state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredStandby(void);

    /************************************************************************/
    /** \brief Function called on entry to the Armed state.
     **
     **  \par Description
     **       This function will be called by the VM_Arming state machine
     **       when the Armed state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredArmed(void);

    /************************************************************************/
    /** \brief Check if PreFlight check has cleared.
     **
     **  \par Description
     **       This function will return true if the PreFlight check is clear.
     **       Passing checks are:
     **           Position estimator is initialized
     **           Battery is not low
     **           Safety switch is not enabled
     **           Sensors are initialized
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    osalbool PreFlightCheckCleared(void);

    /************************************************************************/
    /** \brief Function called on exit from the Armed state.
     **
     **  \par Description
     **       This function will be called by the VM_Arming state machine
     **       when the Armed state is exited.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void ExitedArmed(void);

    /************************************************************************/
    /** \brief Function called on entry to the StandbyError state.
     **
     **  \par Description
     **       This function will be called by the VM_Arming state machine
     **       when the StandbyError state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredStandbyError(void);

    /************************************************************************/
    /** \brief Function called on entry to the ArmedError state.
     **
     **  \par Description
     **       This function will be called by the VM_Arming state machine
     **       when the ArmedError state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredArmedError(void);

    /************************************************************************/
    /** \brief Fine State Machine context.
     **
     **  \par Description
     **       This is the #VM_ArmingContext class auto generated by SMC from
     **       the #VM_Arming.sm file.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    VM_ArmingContext FSM;

private:
    /************************************************************************/
    /** \brief Signal a transition to the Armed state.
     **
     **  \par Description
     **       This is an internal function to signal a transition to the Armed
     **       state.  The SMC auto generated context object defined by the
     **       #VM_Arming.sm file.  Conditions are enforced by the context.
     **       See the state machine definition to determine what conditions,
     **       if any, are required to successfully transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void Arm(void);

    /************************************************************************/
    /** \brief Signal a transition to the Disarmed state.
     **
     **  \par Description
     **       This is an internal function to signal a transition to the
     **       Disarmed state.  The SMC auto generated context object defined by
     **       the #VM_Arming.sm file.  Conditions are enforced by the context.
     **       See the state machine definition to determine what conditions,
     **       if any, are required to successfully transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void Disarm(void);

    /*************************************************************************/
    /** \brief Signal a transition to the next state defined after
     *         initialization.
     **
     **  \par Description
     **       This is an internal function to signal a transition to the next
     **       state after initialization.  The SMC auto generated context
     **       object defined by the #VM_Arming.sm file.  States, transitions,
     **       and conditions are enforced by the context.  See the state
     **       machine definition to determine what the correct transition is.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void InitComplete(void);

    /*************************************************************************/
    /** \brief Reference to the VM application.
     **
     **  \par Description
     **       This is a reference to the parent VM application and used
     **       internally to access VM member functions and attributes.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    VM &App;
};

#endif
