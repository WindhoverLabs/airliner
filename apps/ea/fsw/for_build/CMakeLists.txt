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
 
buildliner_add_app_def(ea
    FILE EA
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_version.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc
)

buildliner_add_app_unit_test(ea-ut
    UTASSERT 
        
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_custom_stub.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_custom_stub.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ea_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ea_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)

#add_dependencies(ea-ut ea)
#add_dependencies(ea-ut-gcov ea)

add_custom_target(
    ea-ut-input-files
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sleep.py ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/fib.py ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/noop.py ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(ea-ut ea-ut-input-files)

