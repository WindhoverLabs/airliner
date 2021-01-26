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

osal_add_whitebox_unit_test(
    oscore-test
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../osapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_binsem_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_countsem_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_exception_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_interrupt_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_misc_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_mutex_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_queue_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_task_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut_oscore_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osloader_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osfile_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osfilesys_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osloader_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osnetwork_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_os_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_ostimer_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_psp_voltab_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/ut_osrtm_stubs.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)
