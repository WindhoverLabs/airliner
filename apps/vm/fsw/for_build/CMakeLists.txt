#add_custom_command(
    #OUTPUT
        #${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
        #${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_NavigationContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_NavigationContext.h
    #COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/sm_src
    #COMMAND java -jar ${PROJECT_SOURCE_DIR}/tools/smc/bin/Smc.jar -c++ -d ${CMAKE_CURRENT_BINARY_DIR}/sm_src ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm
    #COMMAND java -jar ${PROJECT_SOURCE_DIR}/tools/smc/bin/Smc.jar -c++ -d ${CMAKE_CURRENT_BINARY_DIR}/sm_src ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Navigation.sm
    #COMMAND java -jar ${PROJECT_SOURCE_DIR}/tools/smc/bin/Smc.jar -graph -glevel 0 -d ${CMAKE_CURRENT_BINARY_DIR}/sm_src ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm
    #COMMAND java -jar ${PROJECT_SOURCE_DIR}/tools/smc/bin/Smc.jar -graph -glevel 0 -d ${CMAKE_CURRENT_BINARY_DIR}/sm_src ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Navigation.sm

    #BYPRODUCTS
        #${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
        #${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_MainContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_NavigationContext.h
    #DEPENDS
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm
        #${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Navigation.sm
#)
#add_custom_target(vm-smc-gen ALL
    #DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_ArmingContext.h
    #DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_NavigationContext.cpp ${CMAKE_CURRENT_BINARY_DIR}/sm_src/VM_NavigationContext.h
#)



buildliner_add_app_def(vm
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
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_ArmingContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_ArmingContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Navigation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Navigation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_NavigationContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_NavigationContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Arming.sm
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_Navigation.sm
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_symbols.cpp

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/smc/lib/C++

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/vm/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/vm/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

#add_dependencies(vm
    #vm-smc-gen
#)

buildliner_add_app_unit_test(vm-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    COVERAGE_EXCLUSIONS
        ${PROJECT_SOURCE_DIR}/tools/smc/lib/C++

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
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/vm_stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Arming.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_ArmingContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_ArmingContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Navigation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/vm_Navigation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_NavigationContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/VM_NavigationContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/CautionWarningHelper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c
        #${PX4LIB_LIB_SOURCES}
        # Adding lib sources individually so stubs can be used.
        # Math
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Dcm.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Euler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix3F3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Quaternion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector3F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector4F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix4F4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F3.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Dcm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Euler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix3F3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Quaternion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector3F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector4F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix4F4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F3.cpp
        
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../prmlib/fsw/src/ParamsConsumer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../prmlib/fsw/src/prm_lib.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/smc/lib/C++

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)

#add_dependencies(vm-ut
    #vm-smc-gen
#)
