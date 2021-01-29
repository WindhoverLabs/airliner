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
// Class
//	AppClass
//
// Member Functions
//	AppClass()				  - Default constructor.
//	CheckString(const char *) - Is this string acceptable?
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.11  2015/08/02 19:44:34  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.10  2014/09/07 17:16:44  fperrad
// explicit condition
//
// Revision 1.9  2014/09/06 19:54:11  fperrad
// remove hard tab
//
// Revision 1.8  2014/09/06 09:03:00  fperrad
// pragma only for MS compiler
//
// Revision 1.7  2014/07/12 10:48:49  fperrad
// remove _rcs_id
//
// Revision 1.6  2009/12/17 19:51:42  cwrapp
// Testing complete.
//
// Revision 1.5  2009/03/01 18:20:37  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:18:49  charlesr
// Initial revision
//

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#include "AppClass.h"
#include <fcntl.h>

AppClass::AppClass()
#ifdef CRTP
: isAcceptable(false)
#else
: _fsm(*this),
  isAcceptable(false)
#endif
{
    // Uncomment to see debug output.
#ifdef FSM_DEBUG
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif
}

bool AppClass::CheckString(const char *theString)
{
#ifdef CRTP
    enterStartState();
    while(*theString != '\0')
    {
        switch(*theString)
        {
        case '0':
            Zero();
            break;

        case '1':
            One();
            break;

        case 'c':
        case 'C':
#ifdef SERIALIZE
            if (serialize("foobar.txt") < 0)
            {
                std::cerr << "FSM serialization failed." << std::endl;
            }
            else if (deserialize("foobar.txt") < 0)
            {
                std::cerr << "FSM deserialization failed." << std::endl;
            }
            else
            {
                C();
            }
#else
            C();
#endif
            break;

        default:
            Unknown();
            break;
        }
        ++theString;
    }

    // end of string has been reached - send the EOS transition.
    EOS();
#else
    _fsm.enterStartState();
    while(*theString != '\0')
    {
        switch(*theString)
        {
        case '0':
            _fsm.Zero();
            break;

        case '1':
            _fsm.One();
            break;

        case 'c':
        case 'C':
#ifdef SERIALIZE
            if (serialize("foobar.txt") < 0)
            {
                std::cerr << "FSM serialization failed." << std::endl;
            }
            else if (deserialize("foobar.txt") < 0)
            {
                std::cerr << "FSM deserialization failed." << std::endl;
            }
            else
            {
                _fsm.C();
            }
#endif
            _fsm.C();
            break;

        default:
            _fsm.Unknown();
            break;
        }
        ++theString;
    }

    // end of string has been reached - send the EOS transition.
    _fsm.EOS();
#endif

    return isAcceptable;
}

#ifdef SERIALIZE
int AppClass::serialize(const std::string& filename)
{
    int fd(open(filename.c_str(),
                (O_WRONLY | O_CREAT | O_TRUNC),
                (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)));
    int retval(-1);

    if (fd >= 0)
    {
#ifdef CRTP
        int stackDepth(getStateStackDepth());
#else
        int stackDepth(_fsm.getStateStackDepth());
#endif
        int size(stackDepth + 2);
        int bufferSize(size * sizeof(int));
        int buffer[size];
        int i;
        int stateId;

#ifdef FSM_DEBUG
        printf("stack depth: %d, array size: %d, buffer size: %d\n",
               stackDepth,
               size,
               bufferSize);
#endif

        buffer[0] = size;
#ifdef CRTP
        stateId = getState().getId();
#else
        stateId = (_fsm.getState()).getId();
#endif
        buffer[size - 1] = stateId;

#ifdef FSM_DEBUG
        printf("states: %d", stateId);
#endif

        // Note: stop at index 1 because index 0 contains the
        // array size.
        for (i = (size - 2); i > 0; --i)
        {
#ifdef CRTP
            popState();
            stateId = getState().getId();
#else
            _fsm.popState();
            stateId = (_fsm.getState()).getId();
#endif
            buffer[i] = stateId;

#ifdef FSM_DEBUG
            printf(" %d", stateId);
#endif
        }

#ifdef FSM_DEBUG
        printf("\n");
#endif

        retval = write(fd, buffer, bufferSize);

        (void) close(fd);
        fd = -1;
    }

    return (retval);
} // end of AppClass::serialize(const std::string&)

int AppClass::deserialize(const std::string& filename)
{
    int fd(open(filename.c_str(), O_RDONLY));
    int size;
    int retval(-1);

#ifdef CRTP
    clearState();
    emptyStateStack();
#else
    _fsm.clearState();
    _fsm.emptyStateStack();
#endif

    if (fd >= 0 && read(fd, &size, sizeof(int)) == sizeof(int))
    {
#ifdef FSM_DEBUG
        int stackDepth = (size - 2);
#endif
        int arraySize = (size - 1);
        int bufferSize(arraySize * sizeof(int));
        int buffer[arraySize];
        int stateId;

#ifdef FSM_DEBUG
        printf("stack depth: %d, array size: %d, buffer size: %d\n",
               stackDepth,
               size,
               (int) (size * sizeof(int)));
#endif

        if (read(fd, buffer, bufferSize) == bufferSize)
        {
            int i;

            retval = (bufferSize + sizeof(int));

#ifdef FSM_DEBUG
            printf("states:");
#endif

            // Iterate over the entire state ID buffer because
            // the final push makes the last state the current
            // state.
            for (i = 0; i < arraySize; i++)
            {
                stateId = buffer[i];
#ifdef CRTP
                pushState(valueOf(stateId));
#else
                _fsm.pushState(_fsm.valueOf(stateId));
#endif

#ifdef FSM_DEBUG
                printf(" %d", stateId);
#endif
            }
        }
    }

    if (fd >= 0)
    {
        (void) close(fd);
        fd = -1;
    }

    return (retval);
} // end of AppClass::deserialize(const std::string&)
#endif
