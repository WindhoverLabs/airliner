
buildliner_add_app_def(sonar
    FILE SONAR
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_version.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/sonar/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/sonar/fsw/platform_inc
)

#buildliner_add_app_unit_test(sonar-ut
    #UTASSERT
    #NO_HELGRIND
    #NO_MASSIF
        
    #SOURCES
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_app_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_app_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_cmds_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_cmds_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_config_tbl_test.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_config_tbl_test.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_testrunner.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_test_utils.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/sonar_test_utils.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_sonar_custom_hooks.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_sonar_custom_hooks.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_sonar_custom_stubs.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/stubs/ut_sonar_custom_stubs.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_app.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_app.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_config_utils.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_events.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_msg.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_tbldefs.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/sonar_version.h

    #INCLUDES
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    #VALGRIND_SUPPRESSION_FILE
        #${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
#)
