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
 # 3. Neither the name Windhover Labs nor the names of its contributors
 #    may be used to endorse or promote products derived from this software
 #    without specific prior written permission.
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

buildliner_add_app_def(lc
    FILE LC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_action.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_action.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_tbl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_watch.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_watch.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/lc/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/lc/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(lc-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_action.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_action.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_tbl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_watch.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_watch.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_action_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_action_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_custom_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_custom_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_watch_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lc_watch_test.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lc_custom.c
)
