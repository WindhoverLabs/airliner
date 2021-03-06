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
        /cf/downlink/ch0/poll0
        /cf/downlink/ch0/poll1
        /cf/downlink/ch0/poll2
        /cf/downlink/ch0/poll3
        /cf/downlink/ch0/poll4
        /cf/downlink/ch0/poll5
        /cf/log
        /cf/uplink
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
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/prm
)

buildliner_add_app(
    io_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/io_lib/fsw/for_build
)

buildliner_add_app(
    sim
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sim/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sim
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sch
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/sch/sch_custom_rt.c
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
    vc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/vc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/vc/src/vc_dev_io_udp/vc_dev_io_udp.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/vc/src/vc_transmit_udp/vc_transmit_udp.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/vc/src/vc_custom.c
)

buildliner_add_app(
    amc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/amc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/amc/amc_custom.cpp
)

buildliner_add_app(
    mac
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/mac
)

buildliner_add_app(
    rgbled
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/rgbled
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/rgbled/src/rgbled_custom.c
)

buildliner_add_app(
    ms5607
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5607/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ms5607
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/ms5607/src/ms5607_custom.cpp
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/gps
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${PROJECT_SOURCE_DIR}/config/shared/apps/gps/src/gps_custom.c
)

buildliner_add_app(
    sens
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sens
)

buildliner_add_app(
    mpc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/mpc
)

buildliner_add_app(
    vm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/vm
)

buildliner_add_app(
    ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ld
)

buildliner_add_app(
    nav
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/nav
)

buildliner_add_app(
    rcin
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/rcin
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/rcin/src/rcin_custom.c
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/bat
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/bat/bat_custom.cpp
)

buildliner_add_app(
    mpu6050
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu6050/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/mpu6050
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/mpu6050/src/mpu6050_custom.cpp
)

buildliner_add_app(
    ak8963
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ak8963/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ak8963
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/ak8963/src/ak8963_custom.cpp
)

buildliner_add_app(
    pe
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/pe
)

buildliner_add_app(
    qae
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/qae
)

buildliner_add_app(
    lgc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/lgc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/lgc/lgc_custom.cpp
)
