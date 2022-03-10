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
 # 3. Neither the name Windhover Labs nor the names of its contributors may be
 #    used to endorse or promote products derived from this software
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
 
buildliner_add_app_def(sch
    FILE SCH
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_apipriv.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/sch_api.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_symbols.c
        
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/sch/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/sch/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(sch-ut
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_apipriv.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/sch_api.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_api_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_api_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_mock_os.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sch_mock_os.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc/sch_perfids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc/sch_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc/sch_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utassert.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_evs_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_fs_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_eeprom_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memrange_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memutils_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_ram_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_timer_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_watchdog_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_sb_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_tbl_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_tbl_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_time_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utlist.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osapi_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osfileapi_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttest.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttools.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/inc/
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_custom.c
)
