
buildliner_add_app_def(qae
    FILE QAE
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/qae/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/qae/fsw/platform_inc
)

buildliner_add_app_unit_test(qae-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/qae_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/qae_version.h
        ${PX4LIB_LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/qae_config.c
)
