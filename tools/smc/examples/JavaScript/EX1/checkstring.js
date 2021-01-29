#!/usr/bin/env nodejs

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
// Contributor(s): 
//       Port to Lua by Francois Perrad, francois.perrad@gadz.org
//
// Function
//   Main
//
// Description
//  This routine starts the finite state machine running.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.1  2013/12/15 16:35:19  fperrad
// full refactor of JavaScript
//
//

require('../../../lib/JavaScript/statemap.js');
require('./AppClass_sm.js');
require('./AppClass.js');

var retcode = 0;
if (process.argv.length < 3) {
    process.stderr.write("No string to check.\n");
    retcode = 2;
}
else if (process.argv.length > 3) {
    process.stderr.write("Only one argument is accepted.\n");
    retcode = 3;
}
else {
    var appobject = new AppClass();
    var str = process.argv[2];
    var result;
    if (appobject.CheckString(str)) {
        result = 'acceptable';
    }
    else {
        result = 'not acceptable';
        retcode = 1;
    }
    process.stdout.write('The string "' + str + '" is ' + result + '.\n');
}
process.exit(retcode);

