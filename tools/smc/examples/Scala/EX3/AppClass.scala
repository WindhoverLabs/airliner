//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of
// the License at http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS
// IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
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
// Port to Scala by Francois Perrad, francois.perrad@gadz.org
// Copyright 2008, Francois Perrad.
// All Rights Reserved.
//
// Contributor(s):
//
// Name
//  AppClass
//
// Description
//   When a state machine executes an action, it is really calling a
//   member function in the context class.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.3  2012/05/14 07:54:08  fperrad
// fix deprecation with Scala 2.9.1
//
// Revision 1.2  2009/04/23 13:12:08  fperrad
// Added enterStartState method
//
// Revision 1.1  2008/02/04 12:48:00  fperrad
// Added Scala examples 1 - 3
//
//

class AppClass() {
    private val _fsm = new AppClassContext(this)
    private var _is_acceptable: Boolean = false

    // Uncomment to see debug output.
    // _fsm.setDebugFlag(true)

    def CheckString(string: String): Boolean = {
        _fsm.enterStartState()
        for (c <- string.iterator) {
            c match {
                case '0' => _fsm.Zero()
                case '1' => _fsm.One()
                case 'C' => _fsm.C()
                case 'c' => _fsm.C()
                case _   => _fsm.Unknown()
            }
        }
        _fsm.EOS()
        return _is_acceptable
    }

    def Acceptable() {
        _is_acceptable = true
    }

    def Unacceptable() {
        _is_acceptable = false
    }
}
