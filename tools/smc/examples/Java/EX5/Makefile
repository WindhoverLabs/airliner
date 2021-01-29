#
# The contents of this file are subject to the Mozilla Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.mozilla.org/MPL/
# 
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code is State Machine Compiler (SMC).
#
# The Initial Developer of the Original Code is Charles W. Rapp.
# Portions created by Charles W. Rapp are
# Copyright (C) 2000 - 2006. Charles W. Rapp.
# All Rights Reserved.
#
# Contributor(s):
#
# RCS ID
# $Id$
#
# CHANGE LOG
# (See the bottom of this file.)
#

#################################################################
# Macros.
#

TARGET=         taskdemo.jar

SM_SOURCES=     Task.sm \
                TaskManager.sm

SOURCES=        TaskFSM.java \
                TaskManagerFSM.java \
                Task.java \
                TaskManager.java \
                TaskController.java \
                TaskEventListener.java \
                TaskDialog.java \
                TaskPopupMenu.java \
                TaskTable.java \
                TaskMessages.java \
                taskdemo.java

OBJECTS=        $(SOURCES:%.java=%.class)

PACKAGE_DIR=    smc_ex5

# Uncomment to turn on debug message generation.
TRACE=          -g

# Uncomment to turn on serialization.
# SERIAL=         -serial

# Uncomment to turn on synchronized.
SYNC=           -sync

# Uncomment to turn on reflection.
# REFLECT=        -reflect

# Uncomment either to turn on generic collections
# (but only if REFLECT is uncommented).
# GENERIC=        -generic
# GENERIC=        -generic7

# JAVA_BIN=	/Library/Java/JavaVirtualMachines/jdk1.7.0_71.jdk/Contents/Home/bin/
JAVAC=          $(JAVA_BIN)javac
JAVAC_FLAGS=    -d . -g -Xlint -classpath $(FSM_LIB_DIR)/statemap.jar

JAR=                    $(JAVA_BIN)jar
JAR_CREATE_FLAGS=       cmf manifest.txt
JAR_UPDATE_FLAGS=       uf

JAVA=           $(JAVA_BIN)java

SMC=            $(JAVA) -jar ../../../bin/Smc.jar
TARGET_LANG=    -java
# TARGET_LANG=    -java7
SMC_FLAGS=      $(TARGET_LANG) $(TRACE) $(SERIAL) $(SYNC) $(REFLECT) $(GENERIC)

FSM_LIB_DIR=    ../../../lib/Java

GRAPH_LEVEL=   1

RM_F=           rm -f
RM_RF=          rm -rf

########################################
# Rules.
#

all :           $(TARGET)

TaskFSM.java :	Task.sm
		$(SMC) $(SMC_FLAGS) Task.sm

TaskManagerFSM.java :	Task.sm
		$(SMC) $(SMC_FLAGS) TaskManager.sm

%_sm.dot :      %.sm
		$(SMC) -graph -glevel $(GRAPH_LEVEL) $<

%_sm.png :      %_sm.dot
		dot -T png -o $@ $<

%_sm.html :     %.sm
		$(SMC) -table $<

$(TARGET) :     $(SOURCES)
		$(JAVAC) $(JAVAC_FLAGS) $(SOURCES)
		$(JAR) $(JAR_CREATE_FLAGS) $(TARGET) \
			$(PACKAGE_DIR)/*.class
		$(JAR) $(JAR_UPDATE_FLAGS) $(TARGET) \
			-C $(FSM_LIB_DIR) statemap

test :          $(TARGET)
		$(JAVA) -jar $(TARGET)

graph :         $(SM_SOURCES:%.sm=%_sm.dot)

png :           $(SM_SOURCES:%.sm=%_sm.png)

table :         $(SM_SOURCES:%.sm=%_sm.html)

clean :
		-$(RM_F) $(TARGET)
		-$(RM_F) TaskFSM.java
		-$(RM_F) TaskManagerFSM.java
		-$(RM_F) *_sm.dot
		-$(RM_F) *_sm.png
		-$(RM_F) *_sm.html
		-$(RM_RF) $(PACKAGE_DIR)

#
# CHANGE LOG
# $Log$
# Revision 1.18  2015/08/02 19:44:35  cwrapp
# Release 6.6.0 commit.
#
# Revision 1.17  2015/02/16 21:43:09  cwrapp
# SMC v. 6.5.0
#
# SMC - The State Machine Compiler v. 6.5.0
#
# Major changes:
#
# (Java)
#     Added a new "-java7" target language. This version represents
#     the FSM as a transition table. The transition table maps the
#     current state and the transition to a
#     java.lang.invoke.MethodHandle. The transition is executed by
#     calling MethodHandle.invokeExact, which is only slightly
#     slower than a compiled method call.
#
#     The -java7 generated code is compatible with -java generated
#     code. This allows developers to switch between the two
#     without changing application code.
#
#     NOTE: -java7 requires Java 1.7 or latter to run.
#
#
# Minor changes:
#
# (None.)
#
#
# Bug Fixes:
#
# (Objective-C)
#     Incorrect initWithOwner body generated. Same fundamental
#     problem as SF bug 200. See below.
#     (SF bug 198)
#
# (Website)
#     Corrected broken link in FAQ page.
#     (SF bug 199)
#
# (C++)
#     Corrected the invalid generated FSM class name.
#     (SF bug 200)
#
# (C)
#     EXIT_STATE() #define macro not generated.
#     (SF bug 201)
#
# (Manual)
#     Corrected examples which showed %fsmclass and %map set to the
#     same name. This is invalid for most target languages since
#     that would mean the nested map class would have the same name
#     as the containing FSM class.
#
#
#
# ++++++++++++++++++++++++++++++++++++++++
#
# If you have any questions or bugs, please surf
# over to http://smc.sourceforge.net and check out
# the discussion and bug forums. Note: you must be
# a SourceForge member to add articles or bugs. You
# do not have to be a member to read posted
# articles or bugs.
#
# Revision 1.16  2013/09/02 14:45:56  cwrapp
# SMC 6.3.0 commit.
#
# Revision 1.15  2009/11/25 22:30:18  cwrapp
# Fixed problem between %fsmclass and sm file names.
#
# Revision 1.14  2009/03/01 18:20:39  cwrapp
# Preliminary v. 6.0.0 commit.
#
# Revision 1.13  2008/07/29 06:50:59  fperrad
# - add a target 'png'
#
# Revision 1.12  2008/05/20 18:31:12  cwrapp
# ----------------------------------------------------------------------
#
# Committing release 5.1.0.
#
# Modified Files:
# 	Makefile README.txt smc.mk tar_list.txt bin/Smc.jar
# 	examples/Ant/EX1/build.xml examples/Ant/EX2/build.xml
# 	examples/Ant/EX3/build.xml examples/Ant/EX4/build.xml
# 	examples/Ant/EX5/build.xml examples/Ant/EX6/build.xml
# 	examples/Ant/EX7/build.xml examples/Ant/EX7/src/Telephone.java
# 	examples/Java/EX1/Makefile examples/Java/EX4/Makefile
# 	examples/Java/EX5/Makefile examples/Java/EX6/Makefile
# 	examples/Java/EX7/Makefile examples/Ruby/EX1/Makefile
# 	lib/statemap.jar lib/C++/statemap.h lib/Java/Makefile
# 	lib/Php/statemap.php lib/Scala/Makefile
# 	lib/Scala/statemap.scala net/sf/smc/CODE_README.txt
# 	net/sf/smc/README.txt net/sf/smc/Smc.java
# ----------------------------------------------------------------------
#
# Revision 1.11  2008/04/24 09:29:53  fperrad
# + fix #1934494 : remove 'test' from 'all' target
#
# Revision 1.10  2008/02/04 12:37:01  fperrad
# + Exhibit options
# + add a target 'table'
#
# Revision 1.9  2006/09/17 16:26:06  cwrapp
# Initial v. 4.3.3 check-in.
#
# Revision 1.8  2005/11/07 19:34:54  cwrapp
# Changes in release 4.3.0:
# New features:
#
# + Added -reflect option for Java, C#, VB.Net and Tcl code
#   generation. When used, allows applications to query a state
#   about its supported transitions. Returns a list of transition
#   names. This feature is useful to GUI developers who want to
#   enable/disable features based on the current state. See
#   Programmer's Manual section 11: On Reflection for more
#   information.
#
# + Updated LICENSE.txt with a missing final paragraph which allows
#   MPL 1.1 covered code to work with the GNU GPL.
#
# + Added a Maven plug-in and an ant task to a new tools directory.
#   Added Eiten Suez's SMC tutorial (in PDF) to a new docs
#   directory.
#
# Fixed the following bugs:
#
# + (GraphViz) DOT file generation did not properly escape
#   double quotes appearing in transition guards. This has been
#   corrected.
#
# + A note: the SMC FAQ incorrectly stated that C/C++ generated
#   code is thread safe. This is wrong. C/C++ generated is
#   certainly *not* thread safe. Multi-threaded C/C++ applications
#   are required to synchronize access to the FSM to allow for
#   correct performance.
#
# + (Java) The generated getState() method is now public.
#
# Revision 1.7  2005/06/08 11:09:13  cwrapp
# + Updated Python code generator to place "pass" in methods with empty
#   bodies.
# + Corrected FSM errors in Python example 7.
# + Removed unnecessary includes from C++ examples.
# + Corrected errors in top-level makefile's distribution build.
#
# Revision 1.6  2005/05/28 13:51:24  cwrapp
# Update Java examples 1 - 7.
#
# Revision 1.1  2004/10/30 12:01:39  charlesr
# Added Graphviz rules.
#
# Revision 1.0  2003/12/14 20:11:32  charlesr
# Initial revision
#
