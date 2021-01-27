set(APP_NAME ea)

buildliner_add_app_unit_test(ea-custom-ut
    UTASSERT 
	NANOPB
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_custom_test.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/ea_custom.c
        ${PROJECT_SOURCE_DIR}/apps/ea/fsw/src/ea_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_mock_os_calls.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_test_utils.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        
	INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/ea/fsw/src/
        
    WRAPPERS
        fork
        execvp
        waitpid
        kill
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/ea-custom-ut.supp
)

buildliner_add_app_unit_test(ea-perfmon-ut
    UTASSERT
	NANOPB
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_perfmon_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_perfmon_test.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/ea_custom.c
        ${PROJECT_SOURCE_DIR}/apps/ea/fsw/src/ea_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ea_test_utils.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        
	INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/ea/fsw/src/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/ea-perfmon-ut.supp
)

add_custom_target(
    ea-ut-custom-input-files
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/sleep.py ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/fib.py ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/noop.py ${CMAKE_CURRENT_BINARY_DIR}
)

add_dependencies(ea-custom-ut ea-ut-custom-input-files)
add_dependencies(ea-perfmon-ut ea-ut-custom-input-files)

