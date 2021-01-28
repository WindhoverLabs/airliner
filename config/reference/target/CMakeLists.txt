include(../set-vars.cmake)

# Put target specific functions here.  This is where we build most everything.  Everything that either uses the
# cross toolchain or has target independent functions.

buildliner_initialize(
     
    REFERENCE

    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${PROJECT_SOURCE_DIR}/core/base/mission_inc/
        ${PROJECT_SOURCE_DIR}/core/base/platform_inc/
    FILESYS
        /cf/apps
        /cf/download
        /cf/log
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
)


buildliner_add_app(
    cfs_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    px4lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
)

buildliner_add_app(
    prmlib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/for_build
)

buildliner_add_app(
    sim
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sim/fsw/for_build
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    )

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
)

buildliner_add_app(
    fm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
)

buildliner_add_app(
    hk
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
)

buildliner_add_app(
    hs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
)

buildliner_add_app(
    lc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
)

buildliner_add_app(
    md
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
)

buildliner_add_app(
    mm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
)

buildliner_add_app(
    sc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
)

buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
)

buildliner_add_app(
    vc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vc/fsw/for_build
)

buildliner_add_app(
    amc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/platform_inc
        ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/platform_inc
)

buildliner_add_app(
    mac
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
    INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/platform_inc
        ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/platform_inc
)

buildliner_add_app(
    ulr
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ulr/fsw/for_build
)

buildliner_add_app(
    rgbled
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
)

buildliner_add_app(
    ms5611
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5611/fsw/for_build
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
)

buildliner_add_app(
    sens
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
)

buildliner_add_app(
    mpc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
)

buildliner_add_app(
    vm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
)

buildliner_add_app(
    ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
)

buildliner_add_app(
    nav
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
)

buildliner_add_app(
    rcin
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
)

buildliner_add_app(
    mpu9250
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu9250/fsw/for_build
)

buildliner_add_app(
    hmc5883
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hmc5883/fsw/for_build
)

buildliner_add_app(
    pe
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
)

buildliner_add_app(
    qae
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
)

buildliner_add_app(
    lgc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
)

buildliner_add_app(
    ak8963
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ak8963/fsw/for_build
)

buildliner_add_app(
    cvt
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/for_build
)

buildliner_add_app(
    mpu6050
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu6050/fsw/for_build
)

buildliner_add_app(
    ms5607
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5607/fsw/for_build
)

# PX4BR is not yet converted to the Windhover cmake build system.  Omit this
# from the build, for now.
#buildliner_add_app(
#    px4br
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4br/fsw/for_build
#)

# SBN is not yet converted to the Windhover cmake build system.  Omit this
# from the build, for now.
#buildliner_add_app(
#    sbn
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
#)

# SG33BL is not yet fully to the Windhover cmake build system.  Omit this
# from the build, for now.
#buildliner_add_app(
#    sg33bl
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sg33bl/fsw/for_build
#)

buildliner_add_app(
    sonar
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sonar/fsw/for_build
)



configure_file(${PROJECT_SOURCE_DIR}/docs/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/../docs/conf.py)

# Generate the breathe_projects.py file
get_property(AIRLINER_APPS GLOBAL PROPERTY AIRLINER_APPS_PROPERTY)
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/../docs/breathe_projects.py)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/../docs/breathe_projects.py "breathe_projects = {\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/breathe_projects.py "    \"cfe\": \"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/cfe/xml\",\n")
foreach(APP_NAME ${AIRLINER_APPS})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/breathe_projects.py "    \"${APP_NAME}\": \"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/apps/${APP_NAME}/xml\",\n")
endforeach()
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/breathe_projects.py "}\n")

# Generate the doxylinks_tags.py file
get_property(AIRLINER_APPS GLOBAL PROPERTY AIRLINER_APPS_PROPERTY)
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/../docs/doxylink_tags.py)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/../docs/doxylink_tags.py "doxylink = {\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/doxylink_tags.py "    \"cfe\": (\"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/cfe/cfe.tag\", \"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/cfe/\"),\n")
foreach(APP_NAME ${AIRLINER_APPS})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/doxylink_tags.py "    \"${APP_NAME}\": (\"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/apps/${APP_NAME}/${APP_NAME}.tag\", \"${CMAKE_CURRENT_BINARY_DIR}/../docs/html/doxy/apps/${APP_NAME}/\"),\n")
endforeach()
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../docs/doxylink_tags.py "}\n")
