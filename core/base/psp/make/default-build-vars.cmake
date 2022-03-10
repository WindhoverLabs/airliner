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
 
# Collect PSP includes and source
set(PSP_INC_DIRS ${CMAKE_CURRENT_LIST_DIR}/../fsw/inc)

# Set libraries to link into the main executable
set(PSP_LIBS pthread dl rt )

# Set compiler flags for apps and the main executable.
set(COMPILE_FLAGS "-m32 -g -fstack-protector-all" )
set(LINK_FLAGS "-m32")

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)

# which compilers to use for C and C++
set(TOOLCHAIN_PREFIX "")
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_C_FLAGS "-m32 -g -O0 -Wno-pointer-to-int-cast")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_CXX_FLAGS "-m32 -g -fstack-protector-all -O0 -std=c++11")

set(INSTALL_DIR	      ${CMAKE_CURRENT_BINARY_DIR}/exe/cf/apps)
set(CFE_INSTALL_DIR   ${CMAKE_CURRENT_BINARY_DIR}/exe)

set(MEMCHECK_COMMAND "valgrind --tool=memcheck --leak-check=full --show-reachable=yes --undef-value-errors=yes --num-callers=20 --track-origins=yes --child-silent-after-fork=no --trace-children=no --xml=yes --xml-file=dynamic-analysis-%p.xml --gen-suppressions=all")
set(HELGRIND_COMMAND "valgrind --tool=helgrind --num-callers=20 --child-silent-after-fork=no --trace-children=no --xml=yes --xml-file=dynamic-analysis-%p.xml --gen-suppressions=all")
set(MASSIF_COMMAND "valgrind --tool=massif --heap=yes --stacks=yes --massif-out-file=massif-%p.out")

set(PSP_UNIT_TEST_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/fsw/../unit_test)

set(PSP_WB_UT_BSP_SRC
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_start.c
)

set(PSP_BB_UT_BSP_SRC
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_ut.c
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_voltab.c 
)

set(CTEST_BUILD_ONLY false)
set(GCOV_SUPPORTED true)
set(VALGRIND_SUPPORTED true)

set(BUILD_CORE_FROM_SOURCE TRUE)

set(EMBED_SYMTAB false)
set(EMBED_INITRD false)

set(CMAKE_LIBRARY_PATH_TERMINATOR "/")
