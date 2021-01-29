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
// Copyright (C) 2000 - 2003 Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Class
//	AppClass
//
// Member Functions
//	AppClass() 					 - Default constructor.
//	Run()						 - Start the system running.
//	ReceiveRequest(const char *) - Process a request.
//	CheckForRequest()			 - See if a request has arrived.
//	DoRequest()					 - Do the actual request processing here.
//	ProcessingCompleted()		 - All done.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.10  2015/08/02 19:44:35  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.9  2014/09/06 19:55:33  fperrad
// remove hard tab
//
// Revision 1.8  2014/09/06 09:03:43  fperrad
// pragma only for MS compiler
//
// Revision 1.7  2014/07/12 10:48:50  fperrad
// remove _rcs_id
//
// Revision 1.6  2009/03/01 18:20:38  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.5  2005/06/08 11:09:12  cwrapp
// + Updated Python code generator to place "pass" in methods with empty
//   bodies.
// + Corrected FSM errors in Python example 7.
// + Removed unnecessary includes from C++ examples.
// + Corrected errors in top-level makefile's distribution build.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:29:32  charlesr
// Initial revision
//

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#if !defined(WIN32)
#include <unistd.h>
#include <sys/time.h>
#endif
#include <memory.h>
#include "AppClass.h"

using namespace std;

AppClass::AppClass()
#ifdef CRTP
: _number_of_requests(0),
  _continue_running(1)
#else
: _fsm(*this),
  _number_of_requests(0),
  _continue_running(1)
#endif
{
#ifdef FSM_DEBUG
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif
}

void AppClass::Run()
{
#ifdef WIN32
    DWORD SleepTime;
#endif

#ifdef CRTP
    enterStartState();
#else
    _fsm.enterStartState();
#endif
    while (_continue_running == 1)
    {
#ifdef WIN32
        // Sleep for half a second at a time.
        // This will allow for timely receipt of
        // SIGINTs.
        for (SleepTime = 5000; SleepTime > 0; SleepTime -= 500)
        {
            Sleep(500);
        }

        ProcessingCompleted();
#else
        pause();
#endif
    }

    return;
} // end of AppClass::Run()

void AppClass::ReceiveRequest(const char *message)
{
    if (strcmp(message, "stop") == 0)
    {
        // Stop processing messages.
        _continue_running = 0;
    }
    else
    {
        // Increment the request count.
        ++_number_of_requests;

        // Process this message.
#ifdef CRTP
        RequestReceived();
#else
        _fsm.RequestReceived();
#endif
    }

    return;
} // end of AppClass::ReceiveRequest(const char*)

void AppClass::CheckForRequest()
{
    if (_number_of_requests > 0)
    {
#ifdef CRTP
        ProcessRequest();
#else
        _fsm.ProcessRequest();
#endif
    }
    else if (_number_of_requests < 0)
    {
        cout << "The number of outstanding requests is less than zero (";
        cout << _number_of_requests << "); resetting to zero." << endl;
        _number_of_requests = 0;
    }

    return;
} // end of AppClass::CheckForRequest()

void AppClass::DoRequest()
{
    // Decrement the request count.
    --_number_of_requests;

#ifdef WIN32
    cout << "Processing request ..." << endl;
#else
    // Local variable decalarations.
    itimerval nextTimeout;

    // Sleep on this request.
    (void) memset((char *) &nextTimeout, 0, sizeof(nextTimeout));
    nextTimeout.it_value.tv_sec = 5;
    if (setitimer(ITIMER_REAL, &nextTimeout, (itimerval *) NULL) < 0)
    {
        // Failed to start process timer - quit.
        _continue_running = 0;
    }
    else
    {
        cout << "Processing request ..." << endl;
    }
#endif

    return;
} // end of AppClass::DoRequest()

void AppClass::ProcessingCompleted()
{
    cout << "... Processing completed." << endl;

#ifdef CRTP
    ProcessingDone();
#else
    _fsm.ProcessingDone();
#endif
} // end of AppClass::ProcessingCompleted()
