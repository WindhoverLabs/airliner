
buildliner_add_app_def(pe
    FILE PE
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/baro.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/gps.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/land.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/dist.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/flow.cpp

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/pe/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/pe/fsw/platform_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)

buildliner_add_app_unit_test(pe-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_app_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_cmds_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_config_tbl_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_stubs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_block_delay_stubs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pe_block_delay_stubs.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_config_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pe_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/baro.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/gps.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/land.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/dist.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensors/flow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c
        #${PX4LIB_LIB_SOURCES}
        # Adding lib sources individually so stubs can be used.
        # Math
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Dcm.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Derivative.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Euler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Expo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Integrator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Limits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/LowPass.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/HighPass.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/LowPassVector10F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F1.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F6.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix1F10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix2F2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix2F10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix3F3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix3F10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix4F4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix6F1.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix6F6.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix6F10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix10F1.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix10F2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix10F3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix10F6.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Matrix10F10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Quaternion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Stats1F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Stats2F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Stats6F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector1F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector2F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector3F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector4F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector6F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/Vector10F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/filters/LowPassFilter2p.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Dcm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Derivative.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Euler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Expo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Integrator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Limits.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/LowPass.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/HighPass.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/LowPassVector10F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F6.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix1F10.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix2F2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix2F10.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix3F3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix3F10.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix4F4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix6F1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix6F6.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix6F10.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix10F1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix10F2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix10F3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix10F6.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Matrix10F10.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Quaternion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Stats1F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Stats2F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Stats6F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector1F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector2F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector3F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector4F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector6F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/Vector10F.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/filters/LowPassFilter2p.cpp

        # Geo (stubbed out)
        #${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/geo/geo.h
        #${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/geo/geo.c
        
        # Block Delay (stubbed out)
        #${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/src/math/BlockDelay.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/../../../px4lib/fsw/public_inc/math/BlockDelay.hpp

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
        
    REFERENCE_CUSTOM_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tables/pe_config.c
)
