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


buildliner_add_app_def(mac
    FILE MAC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${PX4_MSGS_DIR}

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/mac/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/mac/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(mac-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_app_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_cmds_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_config_tbl_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac_test_utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mac_config_utils.cpp
        ${PX4LIB_LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mac-ut.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/mac_param.c
)
