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
 
include(${OSAL_COMMON_BUILD_FUNC_DIR}/build-functions.cmake)

# Collect OSAL includes and sources
set(OSAL_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
)

osal_set_blackbox_unit_tests(
    OSAL_SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        
    VALGRIND_SUPPRESSION_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/bb-suppressions
)

osal_add_whitebox_unit_test(
    oscore-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_binsem_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_countsem_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_exception_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_interrupt_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_misc_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_mutex_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_queue_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_task_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/ut_oscore_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/oscore-test/valgrind.supp
)

osal_add_whitebox_unit_test(
    osfileapi-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfile-test/ut_osfile_dirio_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfile-test/ut_osfile_fileio_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfile-test/ut_osfile_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfile-test/valgrind.supp
)
        
        
osal_add_whitebox_unit_test(
    osfilesys-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfilesys-test/ut_osfilesys_diskio_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfilesys-test/ut_osfilesys_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
        
    FILESYS
        /ram4
    
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osfilesys-test/valgrind.supp
)


osal_add_whitebox_unit_test(
    osloader-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osloader-test/ut_osloader_module_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osloader-test/ut_osloader_symtable_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osloader-test/ut_osloader_test.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
    
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osloader-test/valgrind.supp
)

get_property(AIRLINER_BUILD_PREFIX GLOBAL PROPERTY AIRLINER_BUILD_PREFIX_PROPERTY)
    
set(MOD 32)
while(MOD GREATER 0)
  math(EXPR MOD "${MOD} - 1")
  add_library(${AIRLINER_BUILD_PREFIX}osal-wb-osloader-test-MODULE${MOD} SHARED EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osloader-test/ut_module.c)
  set_target_properties(${AIRLINER_BUILD_PREFIX}osal-wb-osloader-test-MODULE${MOD} PROPERTIES 
    COMPILE_DEFINITIONS "MODULE_NAME=module${MOD}" 
    PREFIX ""
    OUTPUT_NAME MODULE${MOD}
    LIBRARY_OUTPUT_DIRECTORY eeprom1)
  add_dependencies(${PARSED_ARGS_PREFIX}osal-wb-osloader-test ${AIRLINER_BUILD_PREFIX}osal-wb-osloader-test-MODULE${MOD})
endwhile(MOD GREATER 0)


osal_add_whitebox_unit_test(
    osnetwork-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osnetwork-test/ut_osnetwork_misc_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osnetwork-test/ut_osnetwork_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
    
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osnetwork-test/valgrind.supp
)


#osal_add_whitebox_unit_test(
#    osprintf-test
#    SOURCE 
#        ${CMAKE_CURRENT_SOURCE_DIR}/osprintf.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_c.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_f.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_g.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_i.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_ld.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_lf.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_li.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_lu.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_lx_uc.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_lx.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_misc.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_offset_dummy.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_offset.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_p.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_printf.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_s.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_u.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_x_uc.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/osprintf-test/ut_osprintf_x.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_osloader_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_osfile_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_osfilesys_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_osloader_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_osnetwork_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_ostimer_stubs.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
#    INCLUDES
#        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
#)


osal_add_whitebox_unit_test(
    ostimer-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/ostimer-test/ut_ostimer_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/ostimer-test/ut_ostimer_timerio_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osloader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfilesys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osfileapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ostimer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/osnetwork.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared/ut_os_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/shared
        ${OSAL_INCS}
    
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests/ostimer-test/valgrind.supp
)


set(OSAL_SRC ${OSAL_SRC} PARENT_SCOPE)
set(OSAL_INCS ${OSAL_INCS} PARENT_SCOPE)
