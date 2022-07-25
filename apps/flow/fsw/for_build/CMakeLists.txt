
buildliner_add_app_def(flow
    FILE FLOW
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_version.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc

    COMMANDER_PLUGIN
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)


buildliner_add_app_unit_test(flow-ut
	UTASSERT
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_app_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_app_test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_cmds_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_cmds_test.h
	${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_custom_stubs.h
	${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_custom_stubs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_config_tbl_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_config_tbl_test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_testrunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_test_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/flow_test_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_app.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_config_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_events.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_msg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_tbldefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/flow_version.h
    ${PX4LIB_LIB_SOURCES}
    
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp

)