
buildliner_add_app_def(mavlink
    FILE MAVLINK
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_cds_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_config_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_private_ids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_private_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_version.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${PROJECT_SOURCE_DIR}/mavlink/include/mavlink/v1.0/common/

    DESIGN_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in 
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/mavlink/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/mavlink/fsw/platform_inc  
)

buildliner_add_app_unit_test(MAVLINK-UT
    UTASSERT
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/mavlink_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mavlink_custom.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
)
