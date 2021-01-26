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
 
include(${PROJECT_SOURCE_DIR}/osal/src/make/build-functions.cmake)

add_executable(${PARSED_ARGS_PREFIX}psp-unit-test ${PSP_UNIT_TEST_SRC})
target_link_libraries(${PARSED_ARGS_PREFIX}psp-unit-test -fprofile-arcs gcov ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(${PARSED_ARGS_PREFIX}psp-unit-test PROPERTIES COMPILE_FLAGS "-g -O0 -Wformat=0 -Wno-int-to-pointer-cast -fprofile-arcs -ftest-coverage")
set_property(TARGET ${PARSED_ARGS_PREFIX}psp-unit-test
    APPEND_STRING
    PROPERTY
    LINK_FLAGS
    "-Wl,--wrap=clock_gettime"
)

target_include_directories(${PARSED_ARGS_PREFIX}psp-unit-test PUBLIC
    ${PROJECT_SOURCE_DIR}/tools/ut_assert/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test
    ${OSAL_INC_DIRS}
)


if(PSP_UNIT_TEST_COUNT)
    set(TEST_CASE 0)
    while(TEST_CASE LESS PSP_UNIT_TEST_COUNT)
        math(EXPR TEST_CASE "${TEST_CASE}+1")
        add_test(${PARSED_ARGS_PREFIX}psp-unit-test-${TEST_CASE} ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test --test ${TEST_CASE})
        add_test(${PARSED_ARGS_PREFIX}psp-unit-test-${TEST_CASE}-memcheck ${MEMCHECK_COMMAND} --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test --test ${TEST_CASE} )
        add_test(${PARSED_ARGS_PREFIX}psp-unit-test-${TEST_CASE}-helgrind ${HELGRIND_COMMAND} --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test --test ${TEST_CASE})
        add_test(${PARSED_ARGS_PREFIX}psp-unit-test-${TEST_CASE}-massif ${MASSIF_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test --test ${TEST_CASE})
    endwhile()
else()
    add_test(${PARSED_ARGS_PREFIX}psp-unit-test ${CMAKE_CURRENT_BINARY_DIR}/psp-unit-test)
    add_test(${PARSED_ARGS_PREFIX}psp-unit-test-memcheck ${MEMCHECK_COMMAND} --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test)
    add_test(${PARSED_ARGS_PREFIX}psp-unit-test-helgrind ${HELGRIND_COMMAND} --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_PREFIX}psp-unit-test)
    add_test(${PARSED_ARGS_PREFIX}psp-unit-test-massif ${MASSIF_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/psp-unit-test)
endif()

find_program(RSM_EXEC NAMES rsm.64.lnx PATHS /opt/rsm)
