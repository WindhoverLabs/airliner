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
 
buildliner_add_app_def(cs
    FILE CS
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_compute.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_compute.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_eeprom_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_eeprom_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_memory_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_memory_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_processing.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_version.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/cs/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/cs/fsw/platform_inc
)



buildliner_add_app_unit_test(cs-ut 
    NO_HELGRIND     
    NO_MASSIF  
   
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_app_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_app_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_compute_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_compute_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_eeprom_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_eeprom_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_memory_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_memory_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_table_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_table_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_table_processing_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_table_processing_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_utils_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/cs_utils_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_cfe_tbl_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_compute.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_compute.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_eeprom_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_eeprom_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_memory_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_memory_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_msgdefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_table_processing.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_verify.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cs_version.h
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utassert.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_evs_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_evs_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_fs_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_eeprom_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memrange_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memutils_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_ram_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_timer_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_watchdog_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_sb_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_sb_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_tbl_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_time_hooks.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_time_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utlist.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osapi_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osfileapi_stubs.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttest.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttools.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/inc/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
