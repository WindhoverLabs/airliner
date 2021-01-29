#ifndef _H_STOPLIGHT
#define _H_STOPLIGHT

//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy
// of the License at http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an
// "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
// 
// The Original Code is State Machine Compiler (SMC).
// 
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2009. Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Name
//	TheContext
//
// Description
//	When a state map executes an action, it is really calling a
//	member function in the context class.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.10  2015/08/02 19:44:35  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.9  2015/02/16 21:43:06  cwrapp
// SMC v. 6.5.0
//
// SMC - The State Machine Compiler v. 6.5.0
//
// Major changes:
//
// (Java)
//     Added a new "-java7" target language. This version represents
//     the FSM as a transition table. The transition table maps the
//     current state and the transition to a
//     java.lang.invoke.MethodHandle. The transition is executed by
//     calling MethodHandle.invokeExact, which is only slightly
//     slower than a compiled method call.
//
//     The -java7 generated code is compatible with -java generated
//     code. This allows developers to switch between the two
//     without changing application code.
//
//     NOTE: -java7 requires Java 1.7 or latter to run.
//
//
// Minor changes:
//
// (None.)
//
//
// Bug Fixes:
//
// (Objective-C)
//     Incorrect initWithOwner body generated. Same fundamental
//     problem as SF bug 200. See below.
//     (SF bug 198)
//
// (Website)
//     Corrected broken link in FAQ page.
//     (SF bug 199)
//
// (C++)
//     Corrected the invalid generated FSM class name.
//     (SF bug 200)
//
// (C)
//     EXIT_STATE() #define macro not generated.
//     (SF bug 201)
//
// (Manual)
//     Corrected examples which showed %fsmclass and %map set to the
//     same name. This is invalid for most target languages since
//     that would mean the nested map class would have the same name
//     as the containing FSM class.
//
//
//
// ++++++++++++++++++++++++++++++++++++++++
//
// If you have any questions or bugs, please surf
// over to http://smc.sourceforge.net and check out
// the discussion and bug forums. Note: you must be
// a SourceForge member to add articles or bugs. You
// do not have to be a member to read posted
// articles or bugs.
//
// Revision 1.8  2009/11/25 22:30:18  cwrapp
// Fixed problem between %fsmclass and sm file names.
//
// Revision 1.7  2009/03/27 09:41:45  cwrapp
// Added F. Perrad changes back in.
//
// Revision 1.6  2009/03/01 18:20:37  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.5  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:26:30  charlesr
// Initial revision
//

#include "stoplightdefs.h"
#include "stoplight_sm.h"

namespace cpp_ex4
{
#ifdef CRTP
    class Stoplight : public StoplightContext<Stoplight>
#else
    class Stoplight
#endif
    {
    // Member data.
    public:
    protected:
    private:
#ifndef CRTP
        StoplightContext _fsm;
#endif

    // Member functions.
    public:

        // Destructor.
        virtual ~Stoplight()
        {};

        void start();

        // Change a stoplight's color.
        void TurnLight(StopLights light, LightColors color);

        // Set a timer for the specified number of seconds.
        void SetTimer(int time);

        // This routine is called by SigalmHandler()
        // when a timer expires.
        inline void SigTimeout()
            {
#ifdef CRTP
                Timeout();
#else
                _fsm.Timeout();
#endif
            };

        // Sets the initial state of the state map and the
        // initial timer.
        static Stoplight* Initialize(Directions direction);

    private:

        // Specify the initial direction with
        // the green light.
        Stoplight(const statemap::State& state);

    }; // end of class Stoplight
}; // end of namespace cpp_ex4

#endif
