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

# Add a step to generate XTCE.
include(${PROJECT_SOURCE_DIR}/core/tools/auto-yamcs/build-functions.cmake) 
 
#psp_buildliner_initialize(
#    PSP    pc-linux
#    OSAL   posix
#    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools/elf2cfetbl
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
function(psp_buildliner_initialize)
    # Define the function arguments.
    cmake_parse_arguments(PARSED_ARGS "REFERENCE;APPS_ONLY" "INSTALL_DIR;CORE_BINARY;CORE_BINARY_NAME;OSAL;STARTUP_SCRIPT;CPU_ID;COMMANDER_WORKSPACE;COMMANDER_DISPLAYS;COMMANDER_WORKSPACE_OVERLAY;COMMANDER_CUSTOM_MACRO_BLOCK" "CONFIG;CONFIG_SOURCES;FILESYS;CONFIG_DEFINITION" ${ARGN})
    
    # Set the location to put applications and tables, if specified. 
    if(PARSED_ARGS_INSTALL_DIR)
        set(INSTALL_DIR ${PARSED_ARGS_INSTALL_DIR})
    endif()
    set_property(GLOBAL PROPERTY INSTALL_DIR_PROPERTY ${INSTALL_DIR})
    
    # Create all the target directories the caller requested.
    foreach(dir ${PARSED_ARGS_FILESYS})
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe/${dir})
    endforeach()
    
    # Is this a reference build?
    if(PARSED_ARGS_REFERENCE)
        message("This is a reference build.")
        set_property(GLOBAL PROPERTY IS_REFERENCE_BUILD true)
    endif()
    
#    if(PARSED_ARGS_COMMANDER_WORKSPACE)
#        set(COMMANDER_WORKSPACE ${PARSED_ARGS_COMMANDER_WORKSPACE})
#    else()
#        set(COMMANDER_WORKSPACE ${CMAKE_BINARY_DIR}/commander_workspace)
#    endif()
#    set(COMMANDER_DISPLAYS ${COMMANDER_WORKSPACE}/Displays)
#    set_property(GLOBAL PROPERTY COMMANDER_WORKSPACE ${COMMANDER_WORKSPACE})
#    set_property(GLOBAL PROPERTY COMMANDER_DISPLAYS ${COMMANDER_DISPLAYS})
    
    if(NOT PARSED_ARGS_CPU_ID)
        set(PARSED_ARGS_CPU_ID cfs)
    endif()
    set(BUILDLINER_CDR_CPUID ${PARSED_ARGS_CPU_ID})
    set_property(GLOBAL PROPERTY BUILDLINER_CDR_CPUID ${BUILDLINER_CDR_CPUID})
    
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    configure_file(${PROJECT_SOURCE_DIR}/core/base/psp/fsw/inc/git_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/git_version.h @ONLY)
    
    # Generate the XTCE file
    add_custom_target(ground-tools)
    
    # Generate the templated code
    set_property(GLOBAL PROPERTY BASELINER_CONFIG_FILE_PROPERTY ${CMAKE_BINARY_DIR}/wh_defs.yaml)
    set_property(GLOBAL PROPERTY BASELINER_GENERATED_CODE_DIR_PROPERTY ${CMAKE_CURRENT_BINARY_DIR}/generated_code)
			        
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	    COMMAND ${CMAKE_COMMAND} -E make_directory generated_code
	    COMMAND python ${PROJECT_SOURCE_DIR}/core/base/psp/make/parse_configuration.py ${CMAKE_BINARY_DIR}/wh_defs.yaml generated_code ${COMMANDER_WORKSPACE}/Displays/${PARSED_ARGS_CPU_ID}
	    #COMMAND ${CMAKE_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/generated_code
	)
        
    # Add the 'build-file-system' target.  This is used to trigger the steps to embed the initial ramdisk 
    # after all the build products have been built.
    add_custom_target(build-file-system)
        
    # Parse the OSAL CMake files that will specify the various source files.
    add_subdirectory(${PARSED_ARGS_OSAL} core/osal)
                
    set(TARGET_INCLUDES 
        ${PARSED_ARGS_CONFIG}
        ${CMAKE_CURRENT_BINARY_DIR} 
        ${CFE_INC_DIRS}
        ${OSAL_INC_DIRS}
        ${PSP_INC_DIRS}
        ${PSP_SHARED_INC})
        
    # Set a higher priority set of includes, if this is a reference build.
    if(PARSED_ARGS_REFERENCE_INCLUDES)
        set(TARGET_INCLUDES 
            ${PROJECT_SOURCE_DIR}/core/mission_inc 
            ${PROJECT_SOURCE_DIR}/core/platform_inc 
            ${PROJECT_SOURCE_DIR}/core/platform_inc/cpu1
            ${TARGET_INCLUDES}
        )
    endif()
    
    include_directories(${TARGET_INCLUDES})
    
    set_property(GLOBAL PROPERTY TARGET_INCLUDES_PROP ${TARGET_INCLUDES})

    if(NOT docs)        
        add_custom_target(docs)
    endif()

    if(NOT ${PARSED_ARGS_APPS_ONLY})
        # Copy the startup script into the default location.
        if(EXISTS ${PARSED_ARGS_STARTUP_SCRIPT})
            file(COPY ${PARSED_ARGS_STARTUP_SCRIPT} DESTINATION ${INSTALL_DIR})
        endif()

        if(${BUILD_CORE_FROM_SOURCE})
            # Do the things that we only do when we build the core binary from source.
        
            # Set what we're going to call the executable file.
            if(PARSED_ARGS_CORE_BINARY_NAME)
                set(CFE_EXEC_FILE ${PARSED_ARGS_CORE_BINARY_NAME})
            else()
                set(CFE_EXEC_FILE airliner)
            endif()
        
            # Parse the OSAL CMake files that will specify the various source files.
            if(NOT EXISTS ${PARSED_ARGS_OSAL})
                message(FATAL_ERROR "*** The path to the OSAL is either incorrect, or does not include source code.")
            endif()
        
            psp_add_executable(core-binary 
                FILE_NAME ${CFE_EXEC_FILE}
                
                SOURCES
                    ${CFE_SRC}
                    ${OSAL_SRC}
                    ${PSP_PLATFORM_SRC}
                    ${PSP_SHARED_SRC}
 
                INSTALL_PATH ${CFE_INSTALL_DIR}
            )

            # Add the executable to the combined design+configuration yaml file
            commander_add_module(core
                OUTPUT_FILE        ${CMAKE_BINARY_DIR}/wh_defs.yaml
                YAML_PATH          modules.core
                TARGET_NAME        core-binary 
            )
        
            ## Generate documentation
            find_package(Doxygen)
            if(DOXYGEN_FOUND)
                string (REPLACE ";" " " OSAL_SRC_FILES "${OSAL_SRC}")
                string (REPLACE ";" " " CONFIG_SOURCES "${PARSED_ARGS_CONFIG_SOURCES}")
            
                set(CFS_DOCS_HTML_DIR ${CMAKE_BINARY_DIR}/docs/html/doxy/)
                set(CFS_DOCS_LATEX_DIR ${CMAKE_BINARY_DIR}/docs/latex/doxy/)      
                configure_file(${CFE_DOCS_DIR}/detail_doxy.in ${CMAKE_CURRENT_BINARY_DIR}/detail_doxy @ONLY)
            
                add_custom_target(cfe-docs
                    COMMAND mkdir -p ${CFS_DOCS_HTML_DIR}/cfe/
                    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/detail_doxy
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
                )
                add_dependencies(docs cfe-docs)
            endif(DOXYGEN_FOUND) 
        else()
            # Do the things that we only do when we are assuming the core binary is already built.
        
            # Copy the core binary to the correct location.
            if(EXISTS ${PARSED_ARGS_CORE_BINARY})
                file(COPY ${PARSED_ARGS_CORE_BINARY} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/exe)
            endif()
        endif()
    endif()
endfunction(psp_buildliner_initialize)



function(psp_add_executable)    
    # Define the function arguments.
    cmake_parse_arguments(PARSED_ARGS "EXCLUDE_FROM_ALL" "FILE_NAME;INSTALL_PATH;COMPILE_FLAGS;LINK_FLAGS" "SOURCES;LIBS;INCLUDES;BEFORE_INCLUDES;WRAPPERS" ${ARGN})
    
    set(TARGET_NAME ${ARGV0})
    set(TARGET_BINARY_WITHOUT_SYMTAB ${TARGET_NAME}_no_symtab)
    set(FILE_NAME_WITHOUT_SYMTAB ${PARSED_ARGS_FILE_NAME}_no_symtab)
    set(TARGET_SYMTAB ${TARGET_NAME}_symtab)
    set(SYMTAB_FILE_NAME ${TARGET_NAME}_dl_exports.txt)
    set(SYMTAB_SOURCE_FILE ${TARGET_NAME}_dl_exports.c)
    set(NULL_SYMTAB_SOURCE_FILE ${TARGET_NAME}_null_dl_exports.c)
    set(NULL_INITRD_SOURCE_FILE ${TARGET_NAME}_null_initrd.c)
    set(INITRD_TARGET ${TARGET_NAME}_initrd)
    set(INITRD_FILE_NAME ${TARGET_NAME}_initrd.tar)
    set(INITRD_SOURCE_FILE ${TARGET_NAME}_initrd.c)
    set(TARGET_BINARY ${TARGET_NAME})
    
    # Now build using the various source files that we just parsed.
    add_library(${TARGET_BINARY_WITHOUT_SYMTAB} STATIC ${PARSED_ARGS_SOURCES})
    add_executable(${TARGET_BINARY} ${PARSED_ARGS_SOURCES})
    target_link_libraries(${TARGET_BINARY} PUBLIC ${TARGET_BINARY_WITHOUT_SYMTAB})
    
    # Exclude from all, if requested.  Normally, unit tests are excluded.
    if(${PARSED_ARGS_EXCLUDE_FROM_ALL})
        set_target_properties(${TARGET_BINARY_WITHOUT_SYMTAB} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        set_target_properties(${TARGET_BINARY} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
    
    if(PARSED_ARGS_LIBS)
        target_link_libraries(${TARGET_BINARY} PUBLIC ${PARSED_ARGS_LIBS})
    endif()
    
    if(COMPILE_FLAGS OR PARSED_ARGS_COMPILE_FLAGS)
        set_target_properties(${TARGET_BINARY_WITHOUT_SYMTAB} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} ${PARSED_ARGS_COMPILE_FLAGS}")
        set_target_properties(${TARGET_BINARY} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} ${PARSED_ARGS_COMPILE_FLAGS}")
    endif()

    if(LINK_FLAGS OR PARSED_ARGS_LINK_FLAGS)
        set_target_properties(${TARGET_BINARY_WITHOUT_SYMTAB} PROPERTIES LINK_FLAGS "${LINK_FLAGS} ${PARSED_ARGS_LINK_FLAGS}")
        set_target_properties(${TARGET_BINARY} PROPERTIES LINK_FLAGS "${LINK_FLAGS} ${PARSED_ARGS_LINK_FLAGS}")
    endif()
        
    # Set the output name
    if(PARSED_ARGS_FILE_NAME)
        set_target_properties(${TARGET_BINARY_WITHOUT_SYMTAB} PROPERTIES OUTPUT_NAME ${FILE_NAME_WITHOUT_SYMTAB})
        set_target_properties(${TARGET_BINARY}                PROPERTIES OUTPUT_NAME ${PARSED_ARGS_FILE_NAME})
    endif()
    
    if(PARSED_ARGS_BEFORE_INCLUDES)
        target_include_directories(${TARGET_BINARY_WITHOUT_SYMTAB} BEFORE PUBLIC ${PARSED_ARGS_BEFORE_INCLUDES})
        target_include_directories(${TARGET_BINARY}                BEFORE PUBLIC ${PARSED_ARGS_BEFORE_INCLUDES})
    endif()
    
    # Set the output path
    if(PARSED_ARGS_INSTALL_PATH)
        set_target_properties(${TARGET_BINARY} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PARSED_ARGS_INSTALL_PATH})
    endif()
     
    # Add the include directories
    target_include_directories(${TARGET_BINARY_WITHOUT_SYMTAB} PUBLIC ${PARSED_ARGS_INCLUDES})
    target_include_directories(${TARGET_BINARY}                PUBLIC ${PARSED_ARGS_INCLUDES})
        
    # Make sure we enable exports, to prevent the compiler from optimizing them out.  This needs to be done so the
    # applications can call all the CFE functions.  Otherwise, the compiler will just remove most of the functions
    # because they aren't being used.
    set_target_properties(${TARGET_BINARY_WITHOUT_SYMTAB} PROPERTIES ENABLE_EXPORTS TRUE)
    set_target_properties(${TARGET_BINARY}                PROPERTIES ENABLE_EXPORTS TRUE)
        
    # Link in the various libraries specified by the PSP
    target_link_libraries(${TARGET_BINARY_WITHOUT_SYMTAB} PUBLIC ${LINK_FLAGS} ${PSP_LIBS})
    target_link_libraries(${TARGET_BINARY}                PUBLIC ${LINK_FLAGS} ${PSP_LIBS})
    
    foreach(FUNCTION ${PARSED_ARGS_WRAPPERS})
        set_property(TARGET ${TARGET_BINARY_WITHOUT_SYMTAB}
            APPEND_STRING
            PROPERTY
            LINK_FLAGS
            " -Wl,--wrap=${FUNCTION} "
        )
        set_property(TARGET ${TARGET_BINARY}
            APPEND_STRING
            PROPERTY
            LINK_FLAGS
            " -Wl,--wrap=${FUNCTION} "
        )
    endforeach()
 
    if(EMBED_SYMTAB)   
        add_custom_target(${TARGET_SYMTAB}
            BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_FILE_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_SOURCE_FILE}
            COMMAND nm $<TARGET_FILE:${TARGET_BINARY_WITHOUT_SYMTAB}> > ${SYMTAB_FILE_NAME}
            COMMAND echo ${PROJECT_SOURCE_DIR}/core/base/tools/symtab/symtab_gen.py -i ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_FILE_NAME} -o ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_SOURCE_FILE} -t ${PROJECT_SOURCE_DIR}/core/base/tools/symtab/core_symtab.jinja2
            COMMAND python ${PROJECT_SOURCE_DIR}/core/base/tools/symtab/symtab_gen.py -i ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_FILE_NAME} -o ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_SOURCE_FILE} -t ${PROJECT_SOURCE_DIR}/core/base/tools/symtab/core_symtab.jinja2
       )
       add_dependencies(${TARGET_SYMTAB} ${TARGET_BINARY_WITHOUT_SYMTAB}) 
       add_dependencies(${TARGET_BINARY} ${TARGET_SYMTAB}) 
       target_sources(${TARGET_BINARY} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${SYMTAB_SOURCE_FILE})
    endif()     
    
    if(EMBED_INITRD)
        add_custom_target(${INITRD_TARGET}
            BYPRODUCTS ${INITRD_FILE_NAME} ${INITRD_SOURCE_FILE}
            COMMAND tar -b 512 -cvf ${INITRD_FILE_NAME} -C ${CFE_INSTALL_DIR}/cf .
            COMMAND bin2c --name OS_InitialRamdiskFile ${INITRD_FILE_NAME} > ${INITRD_SOURCE_FILE}
            COMMAND echo "unsigned int OS_InitialRamdiskFileSize = sizeof(OS_InitialRamdiskFile);" >> ${INITRD_SOURCE_FILE} VERBATIM
        )
        add_dependencies(${INITRD_TARGET} ${TARGET_BINARY_WITHOUT_SYMTAB}) 
        add_dependencies(${TARGET_BINARY} ${INITRD_TARGET}) 
        target_sources(${TARGET_BINARY} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${INITRD_SOURCE_FILE})
        add_dependencies(${TARGET_BINARY} build-file-system)  
        add_dependencies(${INITRD_TARGET} build-file-system)  
    endif()  
        
endfunction(psp_add_executable)


function(psp_add_test)
    set(TEST_NAME ${ARGV0})
    # Define the function arguments.
    cmake_parse_arguments(PARSED_ARGS "NO_MEMCHECK;NO_HELGRIND;NO_MASSIF" "VALGRIND_SUPPRESSION_FILE" "SOURCES;LIBS;INCLUDES;WRAPPERS;DEFINES" ${ARGN})
    
    foreach(dir ${PARSED_ARGS_FILESYS})
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${dir})
    endforeach()
    
    psp_add_executable(${TEST_NAME}
        EXCLUDE_FROM_ALL
        
        SOURCES
            ${PARSED_ARGS_SOURCES}
            
        LIBS
            ${PARSED_ARGS_LIBS}
                
        INCLUDES
            ${PARSED_ARGS_INCLUDES}
    )
    
    if(${GCOV_SUPPORTED})
        psp_add_executable(${TEST_NAME}-gcov  
            EXCLUDE_FROM_ALL
                  
            SOURCES
                ${PARSED_ARGS_SOURCES}
                
            INCLUDES
                ${PARSED_ARGS_INCLUDES}
            
            COMPILE_FLAGS
                " -fprofile-arcs -ftest-coverage "
            
            LINK_FLAGS
                " -lgcov --coverage "
            
            LIBS
                ${PARSED_ARGS_LIBS}
                
            WRAPPERS
                ${PARSED_ARGS_WRAPPERS}            
        )
    endif()
    
    add_test(${TEST_NAME}-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME})
    if(NOT ${CTEST_BUILD_ONLY})
        add_test(${TEST_NAME} ${TEST_NAME})
        set_tests_properties(${TEST_NAME} PROPERTIES DEPENDS ${TEST_NAME}-build)
    endif()
    
    if(${GCOV_SUPPORTED})
        add_test(${TEST_NAME}-gcov-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME}-gcov)
        if(NOT ${CTEST_BUILD_ONLY})
            add_test(${TEST_NAME}-gcov ${TEST_NAME}-gcov)
            set_tests_properties(${TEST_NAME}-gcov PROPERTIES DEPENDS ${TEST_NAME}-gcov-build)
        endif()
    endif()

    if(${VALGRIND_SUPPORTED})
        if(EXISTS ${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(MEMCHECK_COMMAND ${MEMCHECK_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(HELGRIND_COMMAND ${HELGRIND_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
        endif()
    
        if(NOT PARSED_ARGS_NO_MEMCHECK)
            add_test(${TEST_NAME}-memcheck-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-memcheck ${MEMCHECK_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-memcheck PROPERTIES DEPENDS ${TEST_NAME}-memcheck-build)
            endif()
        endif()
    
        if(NOT PARSED_ARGS_NO_HELGRIND)
            add_test(${TEST_NAME}-helgrind-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-helgrind ${HELGRIND_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-helgrind PROPERTIES DEPENDS ${TEST_NAME}-helgrind-build)
            endif()
        endif()
    
        if(NOT PARSED_ARGS_NO_MASSIF)
            add_test(${TEST_NAME}-massif-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-massif ${MASSIF_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-massif PROPERTIES DEPENDS ${TEST_NAME}-massif-build)
            endif()
        endif()
    endif()
endfunction(psp_add_test)


#buildliner_add_app(
#    DEFINITION
#        ${PROJECT_SOURCE_DIR}/apps/cfs_lib
#    CONFIG
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/cfs_lib
#)
# This function doesn't actually add the application.  It calls the CMakeLists.txt in the application source directory,
# which actually adds the application.
function(psp_buildliner_add_app)
    # Define the application name.
    set(PARSED_ARGS_APP_NAME ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "EMBEDDED" "DESIGN_DEFINITION" "CONFIG;CONFIG_SOURCES;INCLUDES;CONFIG_DEFINITION;COMPILE_OPTIONS" ${ARGN})

    message("Adding ${PARSED_ARGS_APP_NAME}")
    
    # Set the embedded property, if present.
    if(PARSED_ARGS_EMBEDDED)
        set_property(GLOBAL PROPERTY ${PARSED_ARGS_APP_NAME}_EMBEDDED true)
    endif()

    # Call the CMake file that actually defines the application.
    add_subdirectory(${PARSED_ARGS_DEFINITION} ${CMAKE_CURRENT_BINARY_DIR}/apps/${PARSED_ARGS_APP_NAME})
    
    # Add this application to the master list, in case something else is interested in parsing it.
    set_property(GLOBAL APPEND PROPERTY AIRLINER_APPS_PROPERTY ${PARSED_ARGS_APP_NAME})

    # Most applications also include a CMakeLists.txt file that will define the application tailoring or configuration,
    # but not all.  If a configuration is supplied, call it.
    if(NOT ${PARSED_ARGS_CONFIG} STREQUAL "")
        add_subdirectory(${PARSED_ARGS_CONFIG} ${CMAKE_CURRENT_BINARY_DIR}/custom/apps/${PARSED_ARGS_APP_NAME})
    endif()
    
    if(PARSED_ARGS_EMBEDDED)
        target_sources(core-binary PUBLIC ${PARSED_ARGS_CONFIG_SOURCES})
        target_sources(core-binary_no_symtab PUBLIC ${PARSED_ARGS_CONFIG_SOURCES})
    
        # Add include directories, if provided.
        if(PARSED_ARGS_INCLUDES)
            target_include_directories(core-binary PUBLIC ${PARSED_ARGS_INCLUDES})
            target_include_directories(core-binary_no_symtab PUBLIC ${PARSED_ARGS_INCLUDES})
        endif()
    else()
        target_sources(${PARSED_ARGS_APP_NAME} PUBLIC ${PARSED_ARGS_CONFIG_SOURCES})
    
        # Add include directories, if provided.
        if(PARSED_ARGS_INCLUDES)
            target_include_directories(${PARSED_ARGS_APP_NAME} PUBLIC ${PARSED_ARGS_INCLUDES})
        endif()
    
        if(NOT ${PARSED_ARGS_COMPILE_OPTIONS} STREQUAL "")
            target_compile_options(${PARSED_ARGS_APP_NAME} PRIVATE ${PARSED_ARGS_COMPILE_OPTIONS})
        else()
            if(NOT ${DESIGN_COMPILE_OPTIONS} STREQUAL "")
                target_compile_options(${PARSED_ARGS_APP_NAME} PRIVATE ${DESIGN_COMPILE_OPTIONS})
            endif()
        endif()
        
        get_property(DESIGN_DOCS_INPUT TARGET ${PARSED_ARGS_APP_NAME} PROPERTY DESIGN_DOCS_INPUT)  
        get_property(APP_DEFINITION_DIR TARGET ${PARSED_ARGS_APP_NAME} PROPERTY APP_DEFINITION_DIR)   
        get_property(DEFINITION_SOURCES_LIST TARGET ${PARSED_ARGS_APP_NAME} PROPERTY APP_DEFINITION_SRC) 
        get_property(DESIGN_COMPILE_OPTIONS TARGET ${PARSED_ARGS_APP_NAME} PROPERTY DESIGN_COMPILE_OPTIONS) 
    
        string (REPLACE ";" " " CONFIG_SOURCES "${PARSED_ARGS_CONFIG_SOURCES}")
        string (REPLACE ";" " " DEFINITION_SOURCES "${DEFINITION_SOURCES_LIST}")
        
        # Generate documentation
        find_package(Doxygen)
        if(DOXYGEN_FOUND) 
            if(DESIGN_DOCS_INPUT)
                configure_file(${DESIGN_DOCS_INPUT} ${CMAKE_CURRENT_BINARY_DIR}/target/config/apps/${PARSED_ARGS_APP_NAME}/detail_doxy @ONLY)
                if(NOT TARGET ${PARSED_ARGS_APP_NAME}-docs)
                    add_custom_target(${PARSED_ARGS_APP_NAME}-docs
                        COMMAND mkdir -p ${CFS_DOCS_HTML_DIR}/apps/${PARSED_ARGS_APP_NAME}
                        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/target/config/apps/${PARSED_ARGS_APP_NAME}/detail_doxy
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/target/config/apps/${PARSED_ARGS_APP_NAME}
                    )
                    add_dependencies(docs ${PARSED_ARGS_APP_NAME}-docs)
                endif()
            endif(DESIGN_DOCS_INPUT)
        endif(DOXYGEN_FOUND)
    endif()
endfunction(psp_buildliner_add_app)



#buildliner_add_app_def(sch
#    FILE SCH
#    SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_api.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_app.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sch_cmds.c
#
#    INCLUDES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
#        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
#)
function(psp_buildliner_add_app_def)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS ""  "FILE;DESIGN_DEFINITION" "COMPILE_OPTIONS;SOURCES;LIBS;INCLUDES;PUBLIC_INCLUDES;DESIGN_DOCS;REFERENCE_CONFIG;COMMANDER_DISPLAYS" ${ARGN})
    
    get_property(PUBLIC_APP_INCLUDES GLOBAL PROPERTY PUBLIC_APP_INCLUDES_PROPERTY)
    set(PUBLIC_APP_INCLUDES "${PUBLIC_APP_INCLUDES} ${PARSED_ARGS_PUBLIC_INCLUDES}")

    # If this is a reference build, include the reference configuration directories
    # in the public includes
    get_property(IS_REFERENCE_BUILD GLOBAL PROPERTY IS_REFERENCE_BUILD)
    if(IS_REFERENCE_BUILD)
        if(PARSED_ARGS_REFERENCE_CONFIG)  
            set(PUBLIC_APP_INCLUDES "${PUBLIC_APP_INCLUDES} ${PARSED_ARGS_REFERENCE_CONFIG}")
        endif()
    endif()
    
    set_property(GLOBAL PROPERTY PUBLIC_APP_INCLUDES_PROPERTY ${PUBLIC_APP_INCLUDES})
    separate_arguments(PUBLIC_APP_INCLUDES)

    # See if this is an embedded build
    get_property(IS_EMBEDDED GLOBAL PROPERTY ${PARSED_ARGS_TARGET}_EMBEDDED)

    if(IS_EMBEDDED)        
        target_sources(core-binary PRIVATE ${PARSED_ARGS_SOURCES}) 
        target_sources(core-binary_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
        target_include_directories(core-binary PUBLIC ${PARSED_ARGS_INCLUDES})
        target_include_directories(core-binary_no_symtab PUBLIC ${PARSED_ARGS_INCLUDES})
        target_include_directories(core-binary PUBLIC ${PUBLIC_APP_INCLUDES})
        target_include_directories(core-binary_no_symtab PUBLIC ${PUBLIC_APP_INCLUDES})

        if(PARSED_ARGS_LIBS)
            target_link_libraries(core-binary PUBLIC ${PARSED_ARGS_LIBS})
        endif()
	    
        add_custom_target(${PARSED_ARGS_TARGET})
        add_dependencies(core-binary ${PARSED_ARGS_TARGET})
    else()
        add_library(${PARSED_ARGS_TARGET} MODULE ${PARSED_ARGS_SOURCES})
        add_dependencies(build-file-system ${PARSED_ARGS_TARGET})
        set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})
        set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES PREFIX "")
	
        if(NOT ${PARSED_ARGS_FILE} EQUAL "")
            set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES OUTPUT_NAME ${PARSED_ARGS_FILE})
        endif()
	    
        target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${PARSED_ARGS_INCLUDES})
        target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${PUBLIC_APP_INCLUDES})
        target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${CFE_INC_DIRS})
        target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${OSAL_INC_DIRS})
        target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${PSP_INC_DIRS})
	    
        if(PARSED_ARGS_SOURCES)
            set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES APP_DEFINITION_SRC "${PARSED_ARGS_SOURCES}")
        endif()
	    
        if(NOT ${PARSED_ARGS_DESIGN_DOCS} EQUAL "")
            set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES DESIGN_DOCS_INPUT "${PARSED_ARGS_DESIGN_DOCS}")
        endif()
	    
        set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES APP_DEFINITION_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	    
        if(NOT ${PARSED_ARGS_COMPILE_OPTIONS} STREQUAL "")
            set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES DESIGN_COMPILE_OPTIONS "${PARSED_ARGS_COMPILE_OPTIONS}")
        endif()

        if(PARSED_ARGS_LIBS)
            target_link_libraries(${PARSED_ARGS_TARGET} PUBLIC ${PARSED_ARGS_LIBS})
        endif()
	    
        # If this is a reference build, include the reference configuration directories
        get_property(IS_REFERENCE_BUILD GLOBAL PROPERTY IS_REFERENCE_BUILD)
        if(IS_REFERENCE_BUILD)
            if(PARSED_ARGS_REFERENCE_CONFIG)
                target_include_directories(${PARSED_ARGS_TARGET} PUBLIC ${PARSED_ARGS_REFERENCE_CONFIG})
            endif()
        endif()
    endif()

    if(IS_EMBEDDED) 
        # Add the core binary file to the combined design+configuration yaml file
        commander_add_module(${PARSED_ARGS_TARGET}
            OUTPUT_FILE        ${CMAKE_BINARY_DIR}/wh_defs.yaml
            YAML_PATH          modules.apps.modules.${PARSED_ARGS_TARGET}
            TARGET_NAME        core-binary 
        )
    else()
        # Add the binary file to the combined design+configuration yaml file
        commander_add_module(${PARSED_ARGS_TARGET}
            OUTPUT_FILE        ${CMAKE_BINARY_DIR}/wh_defs.yaml
            YAML_PATH          modules.apps.modules.${PARSED_ARGS_TARGET}
            TARGET_NAME        ${PARSED_ARGS_TARGET}
        )
    endif()
       
endfunction(psp_buildliner_add_app_def)



function(buildliner_add_app_dependencies)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "" "TARGETS" ${ARGN})

    # See if this is an embedded build
    get_property(IS_EMBEDDED GLOBAL PROPERTY ${PARSED_ARGS_TARGET}_EMBEDDED)

    if(IS_EMBEDDED) 
        # Add the dependency to the core binary
        add_dependencies(core-binary_no_symtab
            ${PARSED_ARGS_TARGETS}
        )
    else() 
        # Add the dependency to the application binary
        add_dependencies(${PARSED_ARGS_TARGET}
            ${PARSED_ARGS_TARGETS}
        )
    endif()
    
endfunction(buildliner_add_app_dependencies)


function(psp_buildliner_add_app_unit_test)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "UTASSERT;NO_MEMCHECK;NO_HELGRIND;NO_MASSIF" "COMPILE_OPTIONS;FILE;VALGRIND_SUPPRESSION_FILE" "SOURCES;LIBS;INCLUDES;WRAPPERS;REFERENCE_CUSTOM_SOURCE" ${ARGN})
        
    get_property(PUBLIC_APP_INCLUDES GLOBAL PROPERTY PUBLIC_APP_INCLUDES_PROPERTY)
    separate_arguments(PUBLIC_APP_INCLUDES)
    
    if(PARSED_ARGS_UTASSERT)
        set(UTASSERT_SRC
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utassert.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_evs_hooks.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_evs_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_fs_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_eeprom_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memrange_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_memutils_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_ram_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_timer_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_psp_watchdog_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_sb_hooks.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_sb_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_tbl_hooks.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_tbl_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_time_hooks.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_time_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/utlist.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osapi_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_osfileapi_stubs.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttest.c
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/uttools.c)
    endif()
    
    get_property(IS_REFERENCE_BUILD GLOBAL PROPERTY IS_REFERENCE_BUILD)
    if(IS_REFERENCE_BUILD)
        if(PARSED_ARGS_REFERENCE_CUSTOM_SOURCE)
            set(REFERENCE_CUSTOM_SOURCE ${PARSED_ARGS_REFERENCE_CUSTOM_SOURCE})
        endif()
    endif()
    
    psp_add_executable(${PARSED_ARGS_TARGET}
        EXCLUDE_FROM_ALL

        SOURCES
            ${PARSED_ARGS_SOURCES}
            ${UTASSERT_SRC}
            ${REFERENCE_CUSTOM_SOURCE}
            
        BEFORE_INCLUDES
            ${PARSED_ARGS_INCLUDES}
                
        INCLUDES 
            ${PUBLIC_APP_INCLUDES}
            ${PROJECT_SOURCE_DIR}/core/base/ut_assert/inc/
            ${CFE_INC_DIRS}
            ${OSAL_INC_DIRS}
            ${PSP_INC_DIRS}
            
        LIBS
            ${PARSED_ARGS_LIBS}
        
        COMPILE_FLAGS 
            ${PARSED_ARGS_COMPILE_OPTIONS}
                
        WRAPPERS
            ${PARSED_ARGS_WRAPPERS}
    )
    
    if(${GCOV_SUPPORTED})

        psp_add_executable(${PARSED_ARGS_TARGET}-gcov
            EXCLUDE_FROM_ALL

            SOURCES
                ${PARSED_ARGS_SOURCES}
                ${UTASSERT_SRC}
                ${REFERENCE_CUSTOM_SOURCE}
            
            BEFORE_INCLUDES
                ${PARSED_ARGS_INCLUDES}
                
            INCLUDES 
                ${PUBLIC_APP_INCLUDES}
                ${PROJECT_SOURCE_DIR}/core/base/ut_assert/inc/
                ${CFE_INC_DIRS}
                ${OSAL_INC_DIRS}
                ${PSP_INC_DIRS}
            
            COMPILE_FLAGS
                " ${PARSED_ARGS_COMPILE_OPTIONS} -fprofile-arcs -ftest-coverage "
            
            LINK_FLAGS
                " -lgcov --coverage "
            
            LIBS
                ${PARSED_ARGS_LIBS}
                
            WRAPPERS
                ${PARSED_ARGS_WRAPPERS}
        )
    endif()
    
    # Add all the tests
    add_test(${PARSED_ARGS_TARGET}-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PARSED_ARGS_TARGET})
    if(NOT ${CTEST_BUILD_ONLY})
        add_test(${PARSED_ARGS_TARGET} ${PARSED_ARGS_TARGET})
        set_tests_properties(${PARSED_ARGS_TARGET} PROPERTIES DEPENDS ${PARSED_ARGS_TARGET}-build)
    endif()
            
    if(${GCOV_SUPPORTED})
        add_test(${PARSED_ARGS_TARGET}-gcov-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PARSED_ARGS_TARGET}-gcov)
        if(NOT ${CTEST_BUILD_ONLY})
            add_test(${PARSED_ARGS_TARGET}-gcov ${PARSED_ARGS_TARGET}-gcov)
            set_tests_properties(${PARSED_ARGS_TARGET}-gcov PROPERTIES DEPENDS ${PARSED_ARGS_TARGET}-gcov-build)
        endif()
    endif()

    if(${VALGRIND_SUPPORTED})
	    if(EXISTS ${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(MEMCHECK_COMMAND ${MEMCHECK_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(HELGRIND_COMMAND ${HELGRIND_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
        endif()
        
        if(NOT PARSED_ARGS_NO_MEMCHECK)
            add_test(${PARSED_ARGS_TARGET}-memcheck-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PARSED_ARGS_TARGET})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${PARSED_ARGS_TARGET}-memcheck ${MEMCHECK_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_TARGET})
                set_tests_properties(${PARSED_ARGS_TARGET}-memcheck PROPERTIES DEPENDS ${PARSED_ARGS_TARGET}-memcheck-build)
            endif()
        endif()

        if(NOT PARSED_ARGS_NO_HELGRIND)
            add_test(${PARSED_ARGS_TARGET}-helgrind-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PARSED_ARGS_TARGET})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${PARSED_ARGS_TARGET}-helgrind ${HELGRIND_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_TARGET})
                set_tests_properties(${PARSED_ARGS_TARGET} PROPERTIES DEPENDS ${PARSED_ARGS_TARGET}-helgrind-build)
            endif()
        endif()

        if(NOT PARSED_ARGS_NO_MASSIF)
            add_test(${PARSED_ARGS_TARGET}-massif-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${PARSED_ARGS_TARGET})
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${PARSED_ARGS_TARGET}-massif ${MASSIF_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${PARSED_ARGS_TARGET})
                set_tests_properties(${PARSED_ARGS_TARGET} PROPERTIES DEPENDS ${PARSED_ARGS_TARGET}-massif-build)
            endif()
        endif()
    endif()
endfunction(psp_buildliner_add_app_unit_test)



function(buildliner_set_global_includes)
    include_directories(${CFE_INC_DIRS})
    include_directories(${OSAL_INC_DIRS})
    include_directories(${PSP_INC_DIRS})
    include_directories(${PARSED_ARGS_CONFIG})
endfunction(buildliner_set_global_includes)


function(psp_buildliner_add_cfe_unit_test)
    set(TEST_NAME ${ARGV0}) 

    psp_add_executable(${TEST_NAME}
        EXCLUDE_FROM_ALL

        SOURCES
            ${PARSED_ARGS_SOURCES}
            ${PSP_BB_UT_BSP_SRC}
                
        INCLUDES 
            ${PARSED_ARGS_INCLUDES}
            ${TARGET_INCLUDES}
    )
    if(${GCOV_SUPPORTED})
        psp_add_executable(${TEST_NAME}-gcov
            EXCLUDE_FROM_ALL

            SOURCES
                ${PARSED_ARGS_SOURCES}
                ${PSP_BB_UT_BSP_SRC}
                
            INCLUDES ${PARSED_ARGS_INCLUDES}
            
            COMPILE_FLAGS
                " -fprofile-arcs -ftest-coverage "
            
            LINK_FLAGS
                " -lgcov --coverage "
        )
    endif()

    # Add all the tests
    add_test(${TEST_NAME}-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME})
    if(${GCOV_SUPPORTED})
        add_test(${TEST_NAME}-gcov-build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${TEST_NAME}-gcov)
    endif()
    if(NOT ${CTEST_BUILD_ONLY})
        add_test(${TEST_NAME} ${TEST_NAME})
        set_tests_properties(${TEST_NAME} PROPERTIES DEPENDS ${TEST_NAME}-build)
        if(${GCOV_SUPPORTED})
            add_test(${TEST_NAME}-gcov ${TEST_NAME}-gcov)
            set_tests_properties(${TEST_NAME}-gcov PROPERTIES DEPENDS ${TEST_NAME}-gcov-build)
        endif()
    endif()
     
    if(${VALGRIND_SUPPORTED}) 
        if(EXISTS ${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(MEMCHECK_COMMAND ${MEMCHECK_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
            set(HELGRIND_COMMAND ${HELGRIND_COMMAND} --suppressions=${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE})
        endif()
        
        if(NOT PARSED_ARGS_NO_MEMCHECK)
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-memcheck ${MEMCHECK_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-memcheck PROPERTIES DEPENDS ${TEST_NAME}-build)
            endif()
        endif()
    
        if(NOT PARSED_ARGS_NO_HELGRIND)
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-helgrind ${HELGRIND_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-helgrind PROPERTIES DEPENDS ${TEST_NAME}-build)
            endif()
        endif()
    
        if(NOT PARSED_ARGS_NO_MASSIF)
            if(NOT ${CTEST_BUILD_ONLY})
                add_test(${TEST_NAME}-massif ${MASSIF_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME})
                set_tests_properties(${TEST_NAME}-massif PROPERTIES DEPENDS ${TEST_NAME}-build)
            endif()
        endif()
    endif()
endfunction(psp_buildliner_add_cfe_unit_test)


function(JOIN VALUES GLUE OUTPUT)
    string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
    string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
    set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()



##################################################################
#
# FUNCTION: get_current_cflags
#
# Convert the input string, which is a simple text string of compiler flags such
# as CMAKE_C_FLAGS or CMAKE_CXX_FLAGS, and convert it to a list of individual options
#
# In addition, the "-I" options from include_directories() and -D options from
# add_definitions() will be added to the output list.  The contents of these will be
# obtained via the properities of the current source directory.
#
function(psp_get_app_cflags APP_NAME OUTPUT_LIST INPUT_FLAGS)

    # Start by converting the supplied string to a list
    set(FLAGLIST)
    foreach (FLGSTR ${INPUT_FLAGS} ${ARGN})
        string(REGEX REPLACE " +" ";" TEMPFLG ${FLGSTR})
        list(APPEND FLAGLIST ${TEMPFLG})
    endforeach (FLGSTR ${INPUT_FLAGS} ${ARGN})

    # Append any compile definitions from the directory properties
    get_directory_property(CURRENT_DEFS COMPILE_DEFINITIONS)
    foreach(DEF ${CURRENT_DEFS})
        list(APPEND FLAGLIST "-D${DEF}")
    endforeach(DEF ${CURRENT_DEFS})


    # See if this is an embedded build
    get_property(IS_EMBEDDED GLOBAL PROPERTY ${APP_NAME}_EMBEDDED)
    
    if(IS_EMBEDDED)
        # Append any include directories from the directory properties
        get_target_property(APP_INCS core-binary INCLUDE_DIRECTORIES)
        foreach(INC ${APP_INCS})
            list(APPEND FLAGLIST "-I${INC}")
        endforeach(INC ${APP_INCS})
    else()
        # Append any include directories from the directory properties
        get_target_property(APP_INCS ${APP_NAME} INCLUDE_DIRECTORIES)
        foreach(INC ${APP_INCS})
            list(APPEND FLAGLIST "-I${INC}")
        endforeach(INC ${APP_INCS})
    endif()

    get_directory_property(CURRENT_INCDIRS INCLUDE_DIRECTORIES)
    foreach(INC ${CURRENT_INCDIRS})
        list(APPEND FLAGLIST "-I${INC}")
    endforeach(INC ${CURRENT_INCDIRS})

    set(${OUTPUT_LIST} ${FLAGLIST} PARENT_SCOPE)

endfunction(psp_get_app_cflags OUTPUT_LIST INPUT_FLAGS)



function(psp_buildliner_add_table)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "NAME" "SOURCES;INCLUDES;COPY" ${ARGN})

    psp_get_app_cflags(${PARSED_ARGS_TARGET} TBL_CFLAGS ${CMAKE_C_FLAGS})
    
    get_property(INSTALL_DIR GLOBAL PROPERTY INSTALL_DIR_PROPERTY)
    
    # Get the target includes used by all the CMake managed targets
    #    get_property(TARGET_INCLUDES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    #    foreach(INC ${TARGET_INCLUDES})
    #        list(APPEND FLAGLIST "-I${INC}")
    #    endforeach(INC ${TARGET_INCLUDES})

    if(PARSED_ARGS_NAME AND PARSED_ARGS_SOURCES)
        add_custom_command(
            OUTPUT ${PARSED_ARGS_NAME}.tbl
            COMMAND ${CMAKE_C_COMPILER} ${TBL_CFLAGS} -c -o ${PARSED_ARGS_NAME}.o ${PARSED_ARGS_SOURCES}
            COMMAND ${ELF2CFETBL_BIN}/elf2cfetbl ${PARSED_ARGS_NAME}.o
            COMMAND cp ${PARSED_ARGS_NAME}.tbl ${INSTALL_DIR}
            BYPRODUCTS ${PARSED_ARGS_NAME}.tbl
            DEPENDS ${PARSED_ARGS_SOURCES}
        )
        add_custom_target(${PARSED_ARGS_NAME} ALL
            DEPENDS ${PARSED_ARGS_NAME}.tbl ${PARSED_ARGS_SOURCES}
        )
        add_dependencies(build-file-system ${PARSED_ARGS_NAME})
    endif()

    # Copy any files
    if(PARSED_ARGS_COPY)
        file(COPY ${PARSED_ARGS_COPY} DESTINATION ${INSTALL_DIR})
    endif()

endfunction(psp_buildliner_add_table)



function(psp_buildliner_add_app_unit_test_src)
    set(PARSED_ARGS_TARGET ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "" "" "SOURCES" ${ARGN})

    if(TARGET ${PARSED_ARGS_TARGET}-ut)
        target_sources(${PARSED_ARGS_TARGET}-ut PRIVATE ${PARSED_ARGS_SOURCES})
        target_sources(${PARSED_ARGS_TARGET}-ut_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
    endif(TARGET ${PARSED_ARGS_TARGET}-ut)

    if(TARGET ${PARSED_ARGS_TARGET}-ut-gcov)
        target_sources(${PARSED_ARGS_TARGET}-ut-gcov PRIVATE ${PARSED_ARGS_SOURCES})
        target_sources(${PARSED_ARGS_TARGET}-ut-gcov_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
    endif(TARGET ${PARSED_ARGS_TARGET}-ut-gcov)

    if(TARGET ${PARSED_ARGS_TARGET}-ut-memcheck)
        target_sources(${PARSED_ARGS_TARGET}-ut-memcheck PRIVATE ${PARSED_ARGS_SOURCES})
        target_sources(${PARSED_ARGS_TARGET}-ut-memcheck_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
    endif(TARGET ${PARSED_ARGS_TARGET}-ut-memcheck)

    if(TARGET ${PARSED_ARGS_TARGET}-ut-helgrind)
        target_sources(${PARSED_ARGS_TARGET}-ut-helgrind PRIVATE ${PARSED_ARGS_SOURCES})
        target_sources(${PARSED_ARGS_TARGET}-ut-helgrind_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
    endif(TARGET ${PARSED_ARGS_TARGET}-ut-helgrind)

    if(TARGET ${PARSED_ARGS_TARGET}-ut-massif)
        target_sources(${PARSED_ARGS_TARGET}-ut-massif PRIVATE ${PARSED_ARGS_SOURCES})
        target_sources(${PARSED_ARGS_TARGET}-ut-massif_no_symtab PRIVATE ${PARSED_ARGS_SOURCES})
    endif(TARGET ${PARSED_ARGS_TARGET}-ut-massif)
endfunction(psp_buildliner_add_app_unit_test_src)
