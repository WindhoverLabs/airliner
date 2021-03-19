
buildliner_add_app_def(mpc
    FILE MPC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/platform_inc
)

buildliner_add_app_unit_test(mpc-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_test_tables.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mpc_nominal_config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpc_version.h
        ${PX4LIB_LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
