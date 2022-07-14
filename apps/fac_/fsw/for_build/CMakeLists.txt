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
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_Controller.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_Controller.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_PitchController.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_PitchController.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_RollController.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_RollController.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_WheelController.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_WheelController.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_YawController.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/ECL_YawController.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_app.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_app.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_fac.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_cds_task.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_cds_task.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_cmds_task.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_cmds_task.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_config_utils.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_config_utils.hpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_symbols.cpp
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_events.h
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_msg.h
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_private_ids.h
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_private_types.h
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_tbldefs.h
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/fac_version.h

    INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/src/

    DESIGN_DOCS ${PROJECT_SOURCE_DIR}/apps/fac/fsw/for_build/detail_doxy.in

#    MSG_OVERRIDES ${CMAKE_CURRENT_SOURCE_DIR}/fac_msg_overrides.json

#    COMMANDER_PLUGIN
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr

    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/fac/fsw/platform_inc
)

buildliner_add_app_unit_test(FAC-UT
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cds_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_fac.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.hpp

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    WRAPPERS
        socket
        bind
        sendto

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_custom.supp

    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/fac_config.c
)

#buildliner_add_app_unit_test(FAC-CUSTOM-UT
#    UTASSERT
#    NO_HELGRIND
#    NO_MASSIF

#    SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_app_test.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cds_tbl_test.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cds_tbl_test.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_cmds_test.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_config_tbl_test.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_testrunner.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_test_utils.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_app.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_cds_utils.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_cds_utils.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/fac_config_utils.h

#    INCLUDES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

#    WRAPPERS
#        socket
#        bind
#        sendto

#    VALGRIND_SUPPRESSION_FILE
#        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fac_custom.supp

#    REFERENCE_CUSTOM_SOURCE
#        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/fac_config.c
#)
