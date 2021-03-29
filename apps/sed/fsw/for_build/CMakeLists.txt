#############################################################################
#
#   Copyright (c) 2020 Windhover Labs, L.L.C. All rights reserved.
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
buildliner_add_app_def(sed
    FILE SED
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in

    #MSG_OVERRIDES ${CMAKE_CURRENT_SOURCE_DIR}/sed_msg_overrides.json

    #COMMANDER_PLUGIN
    #    ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/sed/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/sed/fsw/platform_inc
)

#buildliner_add_app_unit_test(sed-ut
    #UTASSERT
    #SOURCES
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_app_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_app_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_cmds_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_cmds_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_config_tbl_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_config_tbl_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_testrunner.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_test_utils.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sed_test_utils.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_app.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_app.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_device.c
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_device.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_spi.c
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_spi.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_events.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_msg.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_version.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sed_config_utils.cpp
        #${PX4LIB_LIB_SOURCES}

    #INCLUDES
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    #VALGRIND_SUPPRESSION_FILE
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
#)
