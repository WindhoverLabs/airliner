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
 
buildliner_add_app_def(to
    FILE TO
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_channel.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_channel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_classifier.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_classifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_message_flow.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_message_flow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_output_queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_output_queue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_priority_queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_priority_queue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_private_ids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_private_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_scheduler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_scheduler.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_version.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
          
    DESIGN_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in  
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/to/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/to/fsw/platform_inc
)


buildliner_add_app_unit_test(to-ut
    UTASSERT 
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_custom_hooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_osapi_hooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_osapi_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_cfe_es_hooks_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_cfe_es_hooks_ex.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_hooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_table_names.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_channel.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_channel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_classifier.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_classifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_message_flow.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_message_flow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_output_queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_output_queue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_priority_queue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_priority_queue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_scheduler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_scheduler.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp  
)


buildliner_add_app_unit_test(to-unreachable-code-ut
    UTASSERT 
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/test_utilities.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/test_utilities.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_app_test_unreachable.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_testrunner_unreachable.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_osapi_hooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_osapi_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_cfe_es_hooks_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_cfe_es_hooks_ex.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_hooks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ut_to_custom_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/to_table_names.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/to_config_utils.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp  
)
