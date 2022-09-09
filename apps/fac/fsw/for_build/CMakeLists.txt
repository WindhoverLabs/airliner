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


buildliner_add_app_def(fac
    FILE FAC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_Controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_Controller.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_PitchController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_PitchController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_RollController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_RollController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_WheelController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_WheelController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_YawController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_YawController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_symbols.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_version.h

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

buildliner_add_app_unit_test(fac-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_Controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_Controller.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_PitchController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_PitchController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_RollController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_RollController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_WheelController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_WheelController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_YawController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECL_YawController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_symbols.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_version.h

        ${PX4LIB_LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac-ut.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/fac_param.c
)
