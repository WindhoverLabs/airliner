#############################################################################
#
#   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name Windhover Labs nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#############################################################################
buildliner_add_app_def(rcin
    FILE RCIN
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_custom.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/platform_inc
)

buildliner_add_app_unit_test(rcin-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_custom_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_custom_stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/rcin_custom_hooks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/rcin_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../private/config/obc/cpd/apps/rcin/sed/src/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
