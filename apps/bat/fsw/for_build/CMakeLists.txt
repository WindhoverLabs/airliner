
buildliner_add_app_def(bat
    FILE BAT
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/bat/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/bat/fsw/platform_inc
)

buildliner_add_app_unit_test(bat-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/bat_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_bat_custom_hooks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_bat_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_bat_custom_stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_bat_custom_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/bat_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/bat_config.c
)
