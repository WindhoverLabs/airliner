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
#
# RCS ID
# $Id$
#
# CHANGE LOG
# $Log$
# Revision 1.12  2015/08/02 19:44:34  cwrapp
# Release 6.6.0 commit.
#
# Revision 1.11  2008/07/29 06:50:56  fperrad
# - add a target 'png'
#
# Revision 1.10  2008/04/24 09:29:52  fperrad
# + fix #1934494 : remove 'test' from 'all' target
#
# Revision 1.9  2008/04/23 16:20:25  fperrad
# + fix #1934474 : wrong include path in C++ examples
#
# Revision 1.8  2008/02/04 11:10:02  fperrad
# + Exhibit options
# + add a target 'table'
# + add a target 'graph'
#
# Revision 1.7  2007/12/28 12:34:40  cwrapp
# Version 5.0.1 check-in.
#
# Revision 1.6  2006/09/17 16:26:06  cwrapp
# Initial v. 4.3.3 check-in.
#
# Revision 1.5  2005/05/28 13:31:18  cwrapp
# Updated C++ examples.
#
# Revision 1.0  2003/12/14 19:15:54  charlesr
# Initial revision
#

#################################################################
# Macros.
#

SM_SOURCES=     AppClass.sm
SOURCES=        $(SM_SOURCES:.sm=_sm.cpp) \
                AppClass.cpp \
                Main.cpp
HEADERS=        $(SM_SOURCES:.sm=_sm.h) \
                AppClass.h
OBJECTS=        $(SOURCES:.cpp=.o)
TARGET=         checkstring
CHECKSTRING=    ./$(TARGET)

# Uncomment to turn off IOStreams for debug.
# NO_STREAMS=     -nostreams

# Uncomment to see state machine debug output.
# FSM_DEBUG=      -DFSM_DEBUG

# Uncomment to turn on debug message generation.
TRACE=          -g $(NO_STREAMS)

# Uncomment to turn off try/catch/rethrow generation.
# NO_CATCH=       -nocatch

# Uncomment to turn off exception throws.
# NO_EXCEPT=      -noex

# Uncomment to turn on CRTP code and -crtp SMC flag.
# CRTP=           -crtp
# CRTP_FLAG=      -DCRTP

SMC=            java -jar ../../../bin/Smc.jar
SMC_FLAGS=      -c++ $(TRACE) $(NO_CATCH) $(NO_EXCEPT) $(CRTP)

CXX=            c++
CPPFLAGS=       -g -Wall -Wextra -I../../../lib/C++ $(FSM_DEBUG) $(CRTP_FLAG)

RM_F=           rm -f

#################################################################
# Rules.
#

%_sm.h %_sm.cpp : %.sm
		$(SMC) $(SMC_FLAGS) $<

%_sm.dot :      %.sm
		$(SMC) -graph -glevel 1 $<

%_sm.png :      %_sm.dot
		dot -T png -o $@ $<

%_sm.html :     %.sm
		$(SMC) -table $<

all :           $(TARGET)

$(TARGET) :     $(HEADERS) $(SOURCES)
		$(CXX) $(CPPFLAGS) -o $@ $(SOURCES)

test :          $(TARGET)
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
		-$(RM_F) $(TARGET)
		-$(RM_F) $(OBJECTS)
		-$(RM_F) *_sm.h
		-$(RM_F) *_sm.cpp
		-$(RM_F) *_sm.dot
		-$(RM_F) *_sm.png
		-$(RM_F) *_sm.html
