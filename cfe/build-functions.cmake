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
 
set(CFE_PSP_DIR "")
set(CFE_OSAL_DIR "")

#initialize_airliner_build(
#    PREFIX 
#    PSP    pc-linux
#    OSAL   posix
#    CONFIG
#        ${CMAKE_CURRENT_SOURCE_DIR}/inc
#        ${CMAKE_CURRENT_SOURCE_DIR}/../shared/inc
#    FILESYS
#        /cf/apps
#        /cf/download
#        /cf/log
#        /cf/upload
#        /ram
#    STARTUP_SCRIPT
#        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
#)
function(initialize_airliner_build)
    # Define the function arguments.
    cmake_parse_arguments(PARSED_ARGS "" "CORE_BINARY;PSP;OSAL;STARTUP_SCRIPT;CORE_TOOLS" "CONFIG;CONFIG_SOURCES;FILESYS" ${ARGN})
    
    # Reset the global variables that are only global to a CFS build.
    set_property(GLOBAL PROPERTY PUBLIC_APP_INCLUDES_PROPERTY "")
    set_property(GLOBAL PROPERTY AIRLINER_BUILD_PREFIX_PROPERTY ${PARSED_ARGS_PREFIX})
    set_property(GLOBAL PROPERTY AIRLINER_CORE_TOOLS_PROPERTY ${PARSED_ARGS_CORE_TOOLS})
    
    define_property(TARGET PROPERTY DESIGN_DOCS_INPUT
        BRIEF_DOCS "Doxygen input file for user documentation."
        FULL_DOCS "Doxygen input file for design documentation.")
    
    define_property(TARGET PROPERTY APP_DEFINITION_DIR
        BRIEF_DOCS "Application definition directory."
        FULL_DOCS "Application definition directory.")
    
    define_property(TARGET PROPERTY APP_DEFINITION_SRC
        BRIEF_DOCS "Application definition source files."
        FULL_DOCS "Application definition source files.")
    
    define_property(TARGET PROPERTY APP_CONFIG_SRC
        BRIEF_DOCS "Application configuration source files."
        FULL_DOCS "Application configuration source files.")
            
    set(CFS_DOCS_DIR ${CMAKE_BINARY_DIR}/docs)
    set(CFS_DOCS_HTML_DIR ${CFS_DOCS_DIR}/html/doxy)

    set(CFE_PSP_DIR ${PARSED_ARGS_PSP})
    set(CFE_OSAL_DIR ${PARSED_ARGS_OSAL})

    include(${PROJECT_SOURCE_DIR}/core/psp/make/default-build-vars.cmake)
    if(EXISTS ${PARSED_ARGS_PSP}/make/build-vars.cmake)
        include(${PARSED_ARGS_PSP}/make/build-vars.cmake)
    endif()
    if(EXISTS ${PARSED_ARGS_PSP}/make/build-functions.cmake)
        include(${PARSED_ARGS_PSP}/make/build-functions.cmake)
    endif()
    
    # Separate some of the arguments
    separate_arguments(MEMCHECK_COMMAND)
    separate_arguments(HELGRIND_COMMAND)
    separate_arguments(MASSIF_COMMAND)
    
    include(${PROJECT_SOURCE_DIR}/core/psp/make/build-functions.cmake)
    set_global_airliner_includes(${PARSED_ARGS_CONFIG})
    
    # Make all variables scoped to the parent.
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        set(${_variableName} ${${_variableName}} PARENT_SCOPE)
    endforeach()
    
    if(${BUILD_CORE_FROM_SOURCE})    
        add_subdirectory(${PARSED_ARGS_PSP}/src core/psp/platform)
        add_subdirectory(${PSP_SHARED_DIR} core/psp/shared)
    
        # Parse the various CFE component CMake files that will specify the various source files.
        add_subdirectory(${CFE_CORE_SRC}/make core/cfe)
    endif()
    
    psp_initialize_airliner_build(${ARGN})
    
endfunction(initialize_airliner_build)



#add_airliner_app(
#    DEFINITION
#        ${PROJECT_SOURCE_DIR}/apps/cfs_lib
#    CONFIG
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/cfs_lib
#)
# This function doesn't actually add the application.  It calls the CMakeLists.txt in the application source directory,
# which actually adds the application.
function(add_airliner_app)
    # Define the application name.
    set(PARSED_ARGS_APP_NAME ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "DEFINITION" "CONFIG" ${ARGN})

    psp_add_airliner_app(${ARGN})

endfunction(add_airliner_app)



#build_airliner_cfe_host_tools(
#    PSP pc-linux
#    OSAL posix
#    CONFIG
#        ${CMAKE_CURRENT_SOURCE_DIR}/inc
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/inc
#)
# Build all the host tools.  Currently, this is just the elf2cfetbl.
function(build_airliner_cfe_host_tools)
    cmake_parse_arguments(PARSED_ARGS "" "PSP;OSAL" "CONFIG" ${ARGN})

    include_directories(${PARSED_ARGS_CONFIG})
    include_directories(${CFE_INC_DIRS})
    include_directories(${OSAL_INC_DIRS})
    include_directories(${PSP_INC_DIRS})
    include_directories(${PARSED_ARGS_PSP}/inc)

    add_subdirectory(${CFE_TOOLS}/elf2cfetbl ${ELF2CFETBL_BIN})

endfunction(build_airliner_cfe_host_tools)


#add_airliner_app_def(sch
#    FILE SCH
#    SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_api.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.c
#
#    INCLUDES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
#        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
#   
#    UNIT_TEST_SOURCES
#
#    UNIT_TEST_INCLUDES
#)
function(add_airliner_app_def)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "FILE;DESIGN_DEFINITION" "SOURCES;LIBS;INCLUDES;PUBLIC_INCLUDES;DESIGN_DOCS" ${ARGN})

    psp_add_airliner_app_def(${ARGN})
endfunction(add_airliner_app_def)



function(add_airliner_app_src)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "" "SOURCES" ${ARGN})

    target_sources(${PARSED_ARGS_TARGET} PUBLIC ${PARSED_ARGS_SOURCES})

endfunction(add_airliner_app_src)



function(add_airliner_app_unit_test_src)
    add_airliner_app_unit_test_src(${ARGN})
endfunction(add_airliner_app_unit_test_src)



function(set_airliner_app_unit_test_options)
    psp_set_airliner_app_unit_test_options(${ARGN})
endfunction(set_airliner_app_unit_test_options)



function(add_airliner_app_unit_test)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "UTF;UTASSERT" "FILE;VALGRIND_SUPPRESSION_FILE" "SOURCES;LIBS;INCLUDES;WRAPPERS" ${ARGN})

    psp_add_airliner_app_unit_test(${ARGN})
endfunction(add_airliner_app_unit_test)



function(add_airliner_cfe_unit_test)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "NO_MEMCHECK;NO_HELGRIND;NO_MASSIF" "VALGRIND_SUPPRESSION_FILE" "SOURCES;INCLUDES" ${ARGN})
    
    psp_add_airliner_cfe_unit_test(${ARGN})
endfunction(add_airliner_cfe_unit_test)



#function(JOIN VALUES GLUE OUTPUT)
#    string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
#    string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
#    set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
#endfunction()



function(add_airliner_app_table)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "NAME" "SOURCES;INCLUDES" ${ARGN})

    psp_add_airliner_app_table(${ARGN})

endfunction(add_airliner_app_table)

