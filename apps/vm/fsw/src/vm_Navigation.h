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
    /************************************************************************/
    /** \brief VM_Navigation default constructor.  Don't use this constructor.
     **
     **  \par Description
     **       This is the default constructor of the VM_Navigation class.  Do
     **       not use this constructor because it does not set VM app
     **       reference.  The non-default constructor sets the VM app
     **       reference.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    VM_Navigation();

public:
    /************************************************************************/
    /** \brief VM_Navigation constructor.
     **
     **  \par Description
     **       This is the non-default constructor of the VM_Navigation class
     **       that sets the VM application reference.  Use this constructor
     **       rather than the default constructor.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    VM_Navigation(VM &inVmApp);

    /************************************************************************/
    /** \brief VM_Navigation destructor.
     **
     **  \par Description
     **       This is the VM_Navigation class destructor.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    ~VM_Navigation();

    /************************************************************************/
    /** \brief Function called on entry to the Manual state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the Manual state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredManual();

    /************************************************************************/
    /** \brief Function called on entry to the AltitudeControl state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the AltitudeControl state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAltitudeControl();

    /************************************************************************/
    /** \brief Function called on entry to the PositionControl state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the PositionControl state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredPositionControl();
    void EnteredAutoMission();

    /************************************************************************/
    /** \brief Function called on entry to the AutoLoiter state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the AutoLoiter state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAutoLoiter();

    /************************************************************************/
    /** \brief Function called on entry to the AutoReturnToLaunch state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the AutoReturnToLaunch state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAutoReturnToLaunch();

    /************************************************************************/
    /** \brief Function called on entry to the Acrobatic state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the Acrobatic state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAcrobatic();

    /************************************************************************/
    /** \brief Function called on entry to the Stabilize state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the Stabilize state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredStabilize();

    /************************************************************************/
    /** \brief Function called on entry to the Rattitude state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the Rattitude state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredRattitude();

    /************************************************************************/
    /** \brief Function called on entry to the AutoTakeoff state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the AutoTakeoff state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAutoTakeoff();

    /************************************************************************/
    /** \brief Function called on entry to the AutoLand state.
     **
     **  \par Description
     **       This function will be called by the VM_Navigation state machine
     **       when the AutoLand state is entered.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    void EnteredAutoLand();

    /************************************************************************/
    /** \brief Get the current state of the VM_Navigation state machine.
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
     **  (#VM_Navigation.sm)
     **  \endreturns
     *************************************************************************/
    uint32 GetCurrentStateID(void);

    /************************************************************************/
    /** \brief Execute all stateful behavior.
     **
     **  \par Description
     **       This function will execute all stateful behavior defined by the
     **       VM_Navigation.sm SMC state machine definition file and implemented
     **       by the auto generated VM_Navigation context.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    void DoAction(void);

    /************************************************************************/
    /** \brief Guard condition on transition to AltCtl state
     **
     **  \par Description
     **       This function determines if is legal to transition to the AltCtl
     **       state at this time.  This is called by the context defined by the
     **       #VM_Navigation.sm file and autogenerated to source code by SMC.
     **       See the #VM_Navigation.sm file to see the states and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionAltCtlValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to PosCtl state
     **
     **  \par Description
     **       This function determines if is legal to transition to the PosCtl
     **       state at this time.  This is called by the context defined by the
     **       #VM_Navigation.sm file and autogenerated to source code by SMC.
     **       See the #VM_Navigation.sm file to see the states and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionPosCtlValid(void);
    osalbool IsTransitionAutoMissionValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to Acrobatic state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       Acrobatic state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionAcrobaticValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to AutoLoiter state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       AutoLoiter state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionAutoLoiterValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to RTL state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       AutoLoiter state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionRtlValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to Stabilize state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       AutoLoiter state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionStabilizeValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to Rattitude state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       Rattitude state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionRattitudeValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to AutoTakeoff state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       AutoTakeoff state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionAutoTakeoffValid(void);

    /************************************************************************/
    /** \brief Guard condition on transition to AutoLand state
     **
     **  \par Description
     **       This function determines if is legal to transition to the
     **       AutoLand state at this time.  This is called by the context
     **       defined by the #VM_Navigation.sm file and autogenerated to source
     **       code by SMC.  See the #VM_Navigation.sm file to see the states
     **       and transitions.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsTransitionAutoLandValid(void);

    /************************************************************************/
    /** \brief Return true if all messages have been received at least once.
     **
     **  \par Description
     **       This function will return true if all the subscribed messages
     **       have been received at least once.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool AllMessagesReceivedAtLeastOnce(void);

    /************************************************************************/
    /** \brief Return true if stabilization is required.
     **
     **  \par Description
     **       This function will return true if the current mode requires
     **       stabilization.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsStabilizationRequired(void);

    /************************************************************************/
    /** \brief Return true the Local Position message is valid.
     **
     **  \par Description
     **       This function will return true if the Local Position is valid.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsLocalPositionIsValid(void);

    /************************************************************************/
    /** \brief Return true the the vehicle is armed.
     **
     **  \par Description
     **       This function will return true if the vehicle is armed.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     **
     *************************************************************************/
    osalbool IsVehicleArmed(void);

    /************************************************************************/
    /** \brief Fine State Machine context.
     **
     **  \par Description
     **       This is the #VM_NavigationContext class auto generated by SMC from
     **       the #VM_Navigation.sm file.
     **
     **  \par Assumptions, External Events, and Notes:
     **        None
     *************************************************************************/
    VM_NavigationContext FSM;

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
