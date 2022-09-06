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

buildliner_add_app_def(fpc
    FILE FPC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_landing_slope.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_catapult_launch_method.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_launch_detector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_runway_takeoff.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_ecl_l1_pos_controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_tecs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_symbols.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${PX4_MSGS_DIR}

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(FPC-UT
    UTASSERT 
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fpc_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fpc_config_utils.cpp
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
