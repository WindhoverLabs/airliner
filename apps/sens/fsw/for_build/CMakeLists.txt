
buildliner_add_app_def(sens
    FILE SENS
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_symbols.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/sens/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/sens/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(sens-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_app_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_cmds_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_config_tbl_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sens_test_utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/stubs.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_LowPassFilter2p_hooks.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_LowPassFilter2p_hooks.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_LowPassFilter2p_stubs.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_LowPassFilter2p_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sens_version.h
        ${PX4LIB_LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
