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

buildliner_add_app_def(hes
    FILE HES
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hes_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hes_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hes_symbols.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
    #USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    # MSG_OVERRIDES ${CMAKE_CURRENT_SOURCE_DIR}/hes_msg_overrides.json

    # COMMANDER_PLUGIN
    #     ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/hes/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/hes/fsw/platform_inc
)

buildliner_add_app_unit_test(HES-UT
    UTASSERT 
    NO_HELGRIND
    NO_MASSIF
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/hes_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/hes_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/hes_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/hes_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/hes_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hes_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hes_config_utils.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
