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
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/prm
)

buildliner_add_app(
    sim
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sim/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/sim
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/sch
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/sch/sch_custom_rt.c
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/cf
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/cs
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ds
)

buildliner_add_app(
    fm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/fm
)

buildliner_add_app(
    hk
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/hk
)

buildliner_add_app(
    hs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/hs
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/hs/hs_custom.c
)

buildliner_add_app(
    lc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/lc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/lc/src/lc_custom.c
)

buildliner_add_app(
    md
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/md
)

buildliner_add_app(
    mm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/mm
)

buildliner_add_app(
    sc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/sc
)

buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ci
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/ci/ci_custom.c
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/to
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/to/tables/to_backup_cfg.c
        ${PROJECT_SOURCE_DIR}/config/shared/to/to_custom.c
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ea
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/ea/src/ea_custom.c
)

buildliner_add_app(
    vc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/vc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/vc/src/vc_dev_io_udp/vc_dev_io_udp.c
        ${PROJECT_SOURCE_DIR}/config/shared/vc/src/vc_transmit_udp/vc_transmit_udp.c
        ${PROJECT_SOURCE_DIR}/config/shared/vc/src/vc_custom.c
)

buildliner_add_app(
    amc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/amc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/amc/amc_custom.cpp
)

buildliner_add_app(
    mac
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/mac
)

buildliner_add_app(
    rgbled
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/rgbled
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/rgbled/src/rgbled_custom.c
)

buildliner_add_app(
    ms5607
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5607/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ms5607
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/ms5607/src/ms5607_custom.cpp
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/gps
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${PROJECT_SOURCE_DIR}/config/shared/gps/src/gps_custom.c
)

buildliner_add_app(
    sens
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/sens
)

buildliner_add_app(
    mpc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/mpc
)

buildliner_add_app(
    vm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/vm
)

buildliner_add_app(
    ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ld
)

buildliner_add_app(
    nav
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/nav
)

buildliner_add_app(
    rcin
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/rcin
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/rcin/src/rcin_custom.c
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/bat
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/bat/bat_custom.cpp
)

buildliner_add_app(
    mpu6050
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu6050/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/mpu6050
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/mpu6050/src/mpu6050_custom.cpp
)

buildliner_add_app(
    ak8963
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ak8963/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/ak8963
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/ak8963/src/ak8963_custom.cpp
)

buildliner_add_app(
    pe
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/pe
)

buildliner_add_app(
    qae
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/qae
)

buildliner_add_app(
    lgc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/lgc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/lgc/lgc_custom.cpp
)
