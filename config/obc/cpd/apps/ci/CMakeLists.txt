set(APP_NAME ci)

buildliner_add_table(
    ${APP_NAME}
    NAME ci_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ci_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ci_config.c)

buildliner_add_app_unit_test(ci-custom-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ci_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ci_custom_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ci_slip_sed.c
        ${PROJECT_SOURCE_DIR}/apps/ci/fsw/src/ci_app.c
        ${PROJECT_SOURCE_DIR}/apps/ci/fsw/src/ci_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ci_mock_os_calls.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ci_test_utils.c
        ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        
    INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/ci/fsw/src/
        
    WRAPPERS
        socket
        bind
    
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/ci-custom.supp
)
