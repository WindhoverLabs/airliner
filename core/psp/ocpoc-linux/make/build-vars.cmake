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
set(PSP_INC_DIRS ${PSP_INC_DIRS} ${CMAKE_CURRENT_LIST_DIR}/../inc)

# Set compiler flags for apps and the main executable.
set(COMPILE_FLAGS "-g" )
set(LINK_FLAGS "")

set(TOOLCHAIN_PREFIX "arm-linux-gnueabihf-")

# which compilers to use for C and C++
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_C_FLAGS "-g -O2 -Wno-pointer-to-int-cast")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

set(PSP_UNIT_TEST_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../unit_test)

set(PSP_WB_UT_BSP_SRC
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_start.c
)

set(PSP_BB_UT_BSP_SRC
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_ut.c
    ${PSP_UNIT_TEST_SRC_DIR}/bsp_voltab.c 
)

