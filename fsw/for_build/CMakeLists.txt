add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/sm_src
    COMMAND java -jar ${PROJECT_SOURCE_DIR}/tools/smc/bin/Smc.jar -c++ -d ${CMAKE_CURRENT_BINARY_DIR}/sm_src ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm 
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm
)
add_custom_target(vm-smc-gen ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.h
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/smc/lib/C++

    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
)

add_dependencies(vm
    vm-smc-gen
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
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.h
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/smc/lib/C++
        ${CMAKE_CURRENT_BINARY_DIR}/sm_src/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)

add_dependencies(vm-ut
    vm-smc-gen
)
