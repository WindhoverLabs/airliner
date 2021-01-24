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
 

set(CFE_ES_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_api.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_apps.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_cds.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_cds_mempool.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_erlog.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_esmempool.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_objtab.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_perf.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_shell.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_start.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_symbols.c
)

	
add_airliner_cfe_unit_test(cfe-es-ut
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/es_UT.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_bsp_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_evs_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_sb_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_fs_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_tbl_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_time_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ut_osprintf_stubs.c
        ${OSAL_UT_ASSERT_DIR}/src/utassert.c
        ${OSAL_UT_ASSERT_DIR}/src/utstubs.c
        ${OSAL_UT_ASSERT_DIR}/src/uttest.c
        ${OSAL_UT_ASSERT_DIR}/src/utlist.c
        ${OSAL_UT_ASSERT_DIR}/src/uttools.c
        ${OSAL_UT_STUBS_DIR}/osapi_stubs.c
        ${OSAL_UT_STUBS_DIR}/osfileapi_stubs.c
        ${CFE_ES_SRC}
        
    INCLUDES
        ${OSAL_UT_ASSERT_DIR}/inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/ 
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../unit-test/cfe_es.supp
)


set(CFE_ES_SRC ${CFE_ES_SRC} PARENT_SCOPE)
