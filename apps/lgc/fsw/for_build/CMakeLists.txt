
buildliner_add_app_def(lgc
    FILE LGC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/platform_inc
)

buildliner_add_app_unit_test(lgc-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/lgc_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_lgc_custom_hooks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_lgc_custom_hooks.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_lgc_custom_stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_lgc_custom_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/lgc_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
