include(../set-vars.cmake)

buildliner_initialize(
    EMBED_SYMTAB
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /ram/apps
        /cf/download
        /cf/log
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
)


buildliner_add_app(
    cfs_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    pq_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
    # TODO move this table build to sbn module?
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/pq_lib
)

# This is temporarily removed until we can remove the OS dependency
# in px4lib, i.e. "CLOCK_MONOTONIC"
#buildliner_add_app(
#    px4lib
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
#)

buildliner_add_app(
    prmlib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/prm
)

buildliner_add_app(
    sch
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_custom_rt.c
)

# This is temporarily removed until we can remove the OS dependency
# in cf, i.e. "arpa/inet.h"
#buildliner_add_app(
#    cf
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/cf
#)

buildliner_add_app(
    cs
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
)

buildliner_add_app(
    fm
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/fm
)

buildliner_add_app(
    hk
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hk
)

buildliner_add_app(
    hs
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hs
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/hs/hs_custom.c
)

buildliner_add_app(
    lc
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/lc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/lc/src/lc_custom.c
)

buildliner_add_app(
    md
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/md
)

buildliner_add_app(
    mm
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
)

buildliner_add_app(
    sc
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sc
)

buildliner_add_app(
    sbn
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sbn
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sbn/src/sbn_remap_tbl.c
)

buildliner_add_app(
    sbn_mbox
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/modules/mbox/fsw/for_build
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/pq_lib/tables/pq_backup_cfg.c
)

# This is temporarily removed until we can remove the OS dependency
# in ci, i.e. "arpa/inet.h"
#buildliner_add_app(
#    ci
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
#)

# This is temporarily removed until we can remove the OS dependency
# in ci, i.e. "arpa/inet.h"
#buildliner_add_app(
#    to
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/to
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_backup_cfg.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
#)


# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    amc
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc/amc_custom.cpp
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    mac
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mac
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    ulr
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ulr/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr/ulr_custom.cpp
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    rgbled
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled/src/rgbled_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    ms5611
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5611/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611/src/ms5611_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    gps
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_interface_uart/gps_serial.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    sens
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sens
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    mpc
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpc
#)

# This is temporarily commented out until we can figure out how to 
# get cmake to generate the SMC files before it does anything else
#buildliner_add_app(
#    vm
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/vm
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    ld
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ld
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    nav
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/nav
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    rcin
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin/src/rcin_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    bat
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat/bat_custom.cpp
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    mpu9250
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu9250/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpu9250
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpu9250/src/mpu9250_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    hmc5883
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hmc5883/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883/src/hmc5883_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    pe
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/pe
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    qae
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/qae
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    lgc
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc/lgc_custom.cpp
#)
