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

buildliner_add_app_def(cvt
    FILE CVT
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_symbols.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/platform_inc
)

buildliner_add_app_def(cvtlib
    FILE CVTLIB
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_lib.c
   
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/platform_inc
)

buildliner_add_app_unit_test(cvt-ut
    UTASSERT 
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvt_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_config_utils.c

    NO_HELGRIND
    NO_MASSIF
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)

buildliner_add_app_unit_test(cvtlib-ut
    UTASSERT 
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvtlib_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvtlib_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cvtlib_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cvt_lib.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
