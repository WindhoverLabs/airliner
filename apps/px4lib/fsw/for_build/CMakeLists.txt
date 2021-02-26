 #############################################################################
 #
 #   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #
 # 1. Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 # 2. Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in
 #    the documentation and/or other materials provided with the
 #    distribution.
 # 3. Neither the name Windhover Labs nor the names of its contributors
 #    may be used to endorse or promote products derived from this software
 #    without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 # FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 # COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 # INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 # BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 # OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 # LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 # ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 # POSSIBILITY OF SUCH DAMAGE.
 #
 #############################################################################

set(PX4LIB_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc)

add_subdirectory(../src/ src)

buildliner_add_app_def(px4lib
    FILE PX4LIB
    SOURCES
        ${PX4LIB_SOURCES}

    INCLUDES
        ${PX4LIB_INCLUDES}
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    PUBLIC_INCLUDES
        ${PX4LIB_PUBLIC_INCLUDE_DIR}/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in

    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/px4_msg_overrides.json
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/platform_inc
)

buildliner_add_app_unit_test(px4lib-vector1f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector1F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector1F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector1F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector1F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector1F_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector1f-ut.supp
)


buildliner_add_app_unit_test(px4lib-vector2f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector2F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector2F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector2F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector2F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector2F_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector2f-ut.supp
)


buildliner_add_app_unit_test(px4lib-vector3f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector3F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector3F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector3F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector3F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector3F_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector3f-ut.supp
)


buildliner_add_app_unit_test(px4lib-vector4f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector4F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector4F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector4F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector4F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector4F_test_utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/math/Vector4F.cpp
        ${PX4LIB_PUBLIC_INCLUDE_DIR}/math/Vector4F.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector4f-ut.supp
)


buildliner_add_app_unit_test(px4lib-vector6f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector6F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector6F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector6F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector6F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector6F_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector6f-ut.supp
)


buildliner_add_app_unit_test(px4lib-vector10f-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector10F_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector10F_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector10F_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector10F_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Vector10F_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-vector10f-ut.supp
)


buildliner_add_app_unit_test(px4lib-quaternion-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Quaternion_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Quaternion_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Quaternion_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Quaternion_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Quaternion_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-quaternion-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix1f2-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F2_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F2_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F2_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F2_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F2_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix1f2-ut.supp
)

buildliner_add_app_unit_test(px4lib-matrix1f3-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F3_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F3_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F3_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F3_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F3_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix1f3-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix1f6-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F6_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F6_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F6_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F6_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F6_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix1f6-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix2f2-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F2_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F2_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F2_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F2_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F2_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix2f2-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix2f10-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F10_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F10_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F10_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F10_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix2F10_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix2f10-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix3f3-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F3_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F3_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F3_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F3_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F3_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix3f3-ut.supp
)

buildliner_add_app_unit_test(px4lib-matrix3f10-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F10_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F10_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F10_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F10_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix3F10_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix3f3-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix4f4-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix4F4_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix4F4_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix4F4_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix4F4_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix4F4_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix4f4-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix6f1-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F1_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F1_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F1_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F1_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F1_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix6f1-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix6f6-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F6_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F6_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F6_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F6_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F6_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix6f6-ut.supp
)



buildliner_add_app_unit_test(px4lib-matrix1f10-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F10_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F10_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F10_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F10_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F10_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix1f10-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix10f1-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F1_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F1_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F1_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F1_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F1_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix10f1-ut.supp
)

buildliner_add_app_unit_test(px4lib-matrix1f1-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F1_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F1_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F1_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F1_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix1F1_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix1f1-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix10f2-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F2_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F2_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F2_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F2_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F2_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix10f2-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix10f3-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F3_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F3_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F3_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F3_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F3_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix10f3-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix10f6-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F6_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F6_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F6_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F6_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F6_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix10f6-ut.supp
)


buildliner_add_app_unit_test(px4lib-matrix10f10-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F10_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F10_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F10_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F10_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix10F10_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix10f10-ut.supp
)



buildliner_add_app_unit_test(px4lib-matrix6f10-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F10_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F10_test.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F10_testrunner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F10_test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/Matrix6F10_test_utils.hpp
        ${PX4LIB_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../cfs_lib/fsw/src/cfs_utils.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
        ${PX4LIB_PUBLIC_INCLUDE_DIR}

    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/px4lib-matrix6f10-ut.supp
)

set(PX4LIB_LIB_SOURCES ${PX4LIB_LIB_SOURCES} CACHE STRING "PX4LIB library source files")
