#
# The contents of this file are subject to the Mozilla Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code is State Machine Compiler (SMC).
#
# The Initial Developer of the Original Code is Charles W. Rapp.
# Portions created by Charles W. Rapp are
# Copyright (C) 2000 Charles W. Rapp.
# All Rights Reserved.
#
# Contributor(s):
#       Port to JavaScript by Francois Perrad, francois.perrad@gadz.org
#
# RCS ID
# $Id$
#
# CHANGE LOG
# $Log$
# Revision 1.1  2013/12/15 16:35:19  fperrad
# full refactor of JavaScript
#
#

#################################################################
# Macros.
#

SM_SOURCES=     AppClass.sm
SOURCES=        $(SM_SOURCES:%.sm=%_sm.js)
CHECKSTRING=    nodejs checkstring.js

# Uncomment to turn on debug message generation.
TRACE=          -g

# Uncomment to turn on reflection.
# REFLECT=        -reflect

SMC=            java -jar ../../../bin/Smc.jar
SMC_FLAGS=      -js $(TRACE) $(REFLECT)

RM_F=           rm -f

#################################################################
# Rules.
#

%_sm.js :       %.sm
		$(SMC) $(SMC_FLAGS) $<

%_sm.dot :      %.sm
		$(SMC) -graph -glevel 1 $<

%_sm.png :      %_sm.dot
		dot -T png -o $@ $<

%_sm.html :     %.sm
		$(SMC) -table $<

all :           checkstring

checkstring :   $(SOURCES)

test:
		-$(CHECKSTRING) ""
		-$(CHECKSTRING) 000
		-$(CHECKSTRING) 00011
		-$(CHECKSTRING) 111
		-$(CHECKSTRING) 000111100
		-$(CHECKSTRING) 00011a1b10c0

graph :         $(SM_SOURCES:%.sm=%_sm.dot)

png :           $(SM_SOURCES:%.sm=%_sm.png)

table :         $(SM_SOURCES:%.sm=%_sm.html)

clean :
		-$(RM_F) *_sm.js
		-$(RM_F) *_sm.dot
		-$(RM_F) *_sm.png
		-$(RM_F) *_sm.html
