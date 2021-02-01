include(../set-vars.cmake)

buildliner_initialize(
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /cf/apps
        /cf/download
        /cf/log
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
)

add_subdirectory(allstop allstop)
set_target_properties(allstop
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe
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
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/prm
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/sch/sch_custom_rt.c
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cf
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ds
)

buildliner_add_app(
    fm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/fm
)

buildliner_add_app(
    hk
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hk
)

buildliner_add_app(
    hs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hs
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/hs/hs_custom.c
)

buildliner_add_app(
    lc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/lc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/lc/src/lc_custom.c
)

buildliner_add_app(
    md
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/md
)

buildliner_add_app(
    mm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
)

buildliner_add_app(
    sc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sc
)

buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ci
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/ci/ci_custom.c
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/to
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/to/tables/to_backup_cfg.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/to/to_custom.c
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ea
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/ea/src/ea_custom.c
)

buildliner_add_app(
    amc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc/amc_custom.cpp
)

buildliner_add_app(
    mac
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mac
)

buildliner_add_app(
    ulr
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ulr/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/ulr
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/ulr/ulr_custom.cpp
)

buildliner_add_app(
    rgbled
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/rgbled
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/rgbled/src/rgbled_custom.c
)

buildliner_add_app(
    ms5611
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5611/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ms5611
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/ms5611/src/ms5611_custom.c
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_interface_uart/gps_serial.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/gps/src/gps_custom.c
)

buildliner_add_app(
    sens
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/sens
)

buildliner_add_app(
    mpc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpc
)

buildliner_add_app(
    vm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/vm
)

buildliner_add_app(
    ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/ld
)

buildliner_add_app(
    nav
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/nav
)

buildliner_add_app(
    rcin
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/rcin
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/apps/rcin/src/rcin_custom.c
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat/bat_custom.cpp
)

buildliner_add_app(
    mpu9250
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu9250/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/mpu9250
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/mpu9250/src/mpu9250_custom.c
)

buildliner_add_app(
    hmc5883
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hmc5883/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hmc5883
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/hmc5883/src/hmc5883_custom.c
)

buildliner_add_app(
    pe
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/pe
)
