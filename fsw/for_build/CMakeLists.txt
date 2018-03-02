
add_airliner_app_def(vm
    FILE VM
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.hpp

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
)

add_airliner_app_unit_test(vm-ut
    UTASSERT
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_version.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)
