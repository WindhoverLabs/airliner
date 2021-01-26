

function(osal_set_blackbox_unit_test)
    set(TEST_NAME ${ARGV0})
    cmake_parse_arguments(PARSED_ARGS "NO_MEMCHECK;NO_HELGRIND;NO_MASSIF" "VALGRIND_SUPPRESSION_FILE" "OSAL_SOURCE" ${ARGN})

    set(VALGRIND_EXCLUSIONS "")
    if(PARSED_ARGS_NO_MEMCHECK)
        set(VALGRIND_EXCLUSIONS "NO_MEMCHECK ")
    endif()
    if(PARSED_ARGS_NO_HELGRIND)
        set(VALGRIND_EXCLUSIONS ${VALGRIND_EXCLUSIONS} "NO_HELGRIND ")
    endif()
    if(PARSED_ARGS_NO_MASSIF)
        set(VALGRIND_EXCLUSIONS ${VALGRIND_EXCLUSIONS} "NO_MASSIF")
    endif()

    psp_add_test(osal-bb-${TEST_NAME}
        ${VALGRIND_EXCLUSIONS}
        SOURCES
    	    ${OSAL_UT_ASSERT_DIR}/src/utassert.c
    	    ${OSAL_UT_ASSERT_DIR}/src/utlist.c
    	    ${OSAL_UT_ASSERT_DIR}/src/utstubs.c
    	    ${OSAL_UT_ASSERT_DIR}/src/uttest.c
    	    ${OSAL_UT_ASSERT_DIR}/src/uttools.c
            ${PARSED_ARGS_OSAL_SOURCE}
            ${PSP_BB_UT_BSP_SRC}
            ${OSAL_BLACK_BOX_TEST_DIR}/${TEST_NAME}/${TEST_NAME}.c

        INCLUDES
            ${OSAL_UT_ASSERT_DIR}/inc/
            ${CMAKE_CURRENT_SOURCE_DIR}/shared
            ${OSAL_INCS}
        
        VALGRIND_SUPPRESSION_FILE
            ${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE}
   )

endfunction(osal_set_blackbox_unit_test)

function(osal_set_blackbox_unit_tests)
    cmake_parse_arguments(PARSED_ARGS "" "VALGRIND_SUPPRESSION_DIR" "OSAL_SOURCE" ${ARGN})

    osal_set_blackbox_unit_test(bin-sem-flush-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/bin-sem-flush-test.supp)
    osal_set_blackbox_unit_test(bin-sem-test NO_MASSIF OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/bin-sem-test.supp)
    osal_set_blackbox_unit_test(bin-sem-timeout-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/bin-sem-timeout-test.supp)
    osal_set_blackbox_unit_test(count-sem-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/count-sem-test.supp)
    osal_set_blackbox_unit_test(file-api-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/file-api-test.supp)
    osal_set_blackbox_unit_test(mutex-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/mutex-test.supp)
    osal_set_blackbox_unit_test(osal-core-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/osal-core-test.supp)
    osal_set_blackbox_unit_test(queue-timeout-test NO_MASSIF OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/queue-timeout-test.supp)
    osal_set_blackbox_unit_test(symbol-api-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/symbol-api-test.supp)
    osal_set_blackbox_unit_test(timer-test NO_MASSIF OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/timer-test.supp)
    #osal_set_blackbox_unit_test(concurrency-test OSAL_SOURCE ${PARSED_ARGS_OSAL_SOURCE} VALGRIND_SUPPRESSION_FILE ${PARSED_ARGS_VALGRIND_SUPPRESSION_DIR}/concurrency-test.supp)
endfunction(osal_set_blackbox_unit_tests)


function(osal_add_whitebox_unit_test)
    set(TEST_NAME ${ARGV0})    
    cmake_parse_arguments(PARSED_ARGS "" "VALGRIND_SUPPRESSION_FILE" "SOURCE;FILESYS;WRAPPERS;DEFINES;INCLUDES" ${ARGN})

    psp_add_test(osal-wb-${TEST_NAME}
        SOURCES
            ${PSP_WB_UT_BSP_SRC}
            ${PARSED_ARGS_SOURCE}

        INCLUDES
            ${PARSED_ARGS_INCLUDES}
            
        FILESYS
            ${PARSED_ARGS_FILESYS}
            
        VALGRIND_SUPPRESSION_FILE
            ${PARSED_ARGS_VALGRIND_SUPPRESSION_FILE}

        WRAPPERS
            ${PARSED_ARGS_WRAPPERS}
            
        DEFINES
            ${PARSED_ARGS_DEFINES}
    )
endfunction(osal_add_whitebox_unit_test)
