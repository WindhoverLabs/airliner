set(APP_NAME to)

buildliner_add_table(
    ${APP_NAME}
    NAME to_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/to_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_unit_test(to-custom-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_app_custom_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_app_custom_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_custom_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_custom_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_custom_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_app_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_app_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_platform_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/to_platform_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/to_slip_sed.c
        ${CMAKE_CURRENT_SOURCE_DIR}/to_slip_sed.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/
        ${PROJECT_SOURCE_DIR}/apps/to/fsw/src
        
    WRAPPERS
        socket
        bind
        sendto
)
