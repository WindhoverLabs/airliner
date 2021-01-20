
add_airliner_app_def(ld
    FILE LD
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_state_history.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_state_history.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in

    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/ld_msg_overrides.json
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/ld/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/ld/fsw/platform_inc
)

add_airliner_app_unit_test(ld-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_test_utils.h
    	${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_custom_stubs.h
    	${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ld_custom_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_state_history.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_state_history.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ld_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/ld_config.c
)
