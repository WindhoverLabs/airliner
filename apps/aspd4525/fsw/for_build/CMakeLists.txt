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

buildliner_add_app_def(aspd4525
    FILE ASPD4525
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_math.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_atm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_custom.h
        ${PROJECT_SOURCE_DIR}/config/shared/apps/aspd4525/src/aspd4525_custom.c
        #${PROJECT_SOURCE_DIR}/config/shared/apps/aspd4525/src/aspd4525_uart.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    # LIBS 
    #     m 

    #USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    MSG_OVERRIDES ${CMAKE_CURRENT_SOURCE_DIR}/aspd4525_msg_overrides.json

    COMMANDER_PLUGIN
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/aspd4525/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/aspd4525/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(ASPD4525-UT
    UTASSERT 
    NO_HELGRIND
    NO_MASSIF
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_custom_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/aspd4525_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_math.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/aspd4525_atm.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    LIBS 
        m 

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
