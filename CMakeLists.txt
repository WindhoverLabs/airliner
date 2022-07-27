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
 
# The build system is tested with cmake version 3.3.2
cmake_minimum_required(VERSION 3.3.2)

project(Airliner)

include(CTest)
find_program( MEMORYCHECK_COMMAND valgrind )
find_package(Threads)
set( CTEST_MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leak-check=full --show-reachable=yes --undef-value-errors=yes --track-origins=no --child-silent-after-fork=no --trace-children=no --xml=yes --xml-file=memcheck-%p.xml" )
set( CTEST_VALGRIND_COMMAND_OPTIONS "--tool=memcheck --leak-check=full --show-reachable=yes --undef-value-errors=yes --track-origins=no --child-silent-after-fork=no --trace-children=no --xml=yes --xml-file=memcheck-%p.xml")
enable_testing()

set(ELF2CFETBL_BIN ${PROJECT_BINARY_DIR}/../host/tools/elf2cfetbl)
set(_MAVLINK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools/mavlink/v2.0)

# Set paths from all 3 major core components:  CFE, OSAL, and PSP.
include(core/base/cfe/set-paths.cmake)
include(core/base/osal/set-paths.cmake)
include(core/base/psp/set-paths.cmake)

# Include the file with all the build functions that we will call in the configuration directory.
include(core/base/cfe/build-functions.cmake)

add_subdirectory(config/${BUILDNAME}/${BUILDTYPE} ${BUILDTYPE})
