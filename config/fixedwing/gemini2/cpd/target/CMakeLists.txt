include(../set-vars.cmake)

buildliner_initialize(
    EMBED_SYMTAB
    CPU_ID CPD
    CORE_BINARY_NAME airliner.elf
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /ram/apps
        /cf/log
        /cf/uplink
        /cf/downlink/ch0/dir0/
        /cf/downlink/ch0/dir1/
        /cf/downlink/ch0/dir2/
        /cf/downlink/ch0/dir3/
        /cf/downlink/ch0/dir4/
        /cf/downlink/ch0/dir5/
        /cf/downlink/ch0/dir6/
        /cf/downlink/ch0/dir7/
        /cf/downlink/ch1/dir0/
        /cf/downlink/ch1/dir1/
        /cf/downlink/ch1/dir2/
        /cf/downlink/ch1/dir3/
        /cf/downlink/ch1/dir4/
        /cf/downlink/ch1/dir5/
        /cf/downlink/ch1/dir6/
        /cf/downlink/ch1/dir7/
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
        
    COMMANDER_WORKSPACE
        ${CMAKE_BINARY_DIR}/../../commander_workspace
)


buildliner_add_app(
    cfs_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    mbp_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/mbp_lib/fsw/for_build
)

buildliner_add_app(
    sliplib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sliplib/fsw/for_build
)

buildliner_add_app(
    ipc_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/ipc_lib/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ipc_lib/ipc_lib_custom.c
)

buildliner_add_app(
    pq_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
    # TODO move this table build to sbn module?
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/pq_lib
)


#buildliner_add_app(
#    x_lib
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/x_lib/fsw/for_build
#)


buildliner_add_app(
    sedlib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sedlib/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sedlib/sedlib_config.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sedlib/sedlib_custom.c
)

buildliner_add_app(
    io_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/io_lib/fsw/for_build
)

buildliner_add_app(
    px4lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
)

#buildliner_add_app(
    #sed
    #EMBEDDED
    #DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sed/fsw/for_build
    #CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sed
#)


# sch_custom_v1.c:  Traditional platform independent layer that uses OS_Timer 
#                   to drive minor frames.
# sch_custom_v2.c:  SED specific layer that uses 250Hz minor frame interrupts
#                   to drive minor frames.  This assumes you are using the
#                   Windhover Labs avionics platform with the major and minor
#                   frame timers controlled by the Sensor and Effector Domain 
#                   (SED).  The interrupt handler included in this file does
#                   not clear the interrupt.  The SED clears the interrupt
#                   automatically.
buildliner_add_app(
    sch
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sch
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sch/sch_custom_v2.c
)
buildliner_add_app_unit_test_src(
    sch
    SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sch/sch_custom_v2.c
)

buildliner_add_app(
    sbnd
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbnd/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbnd
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sbnd/shm/sbnd_custom.cpp
)

buildliner_add_app(
    cf
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/cf
)

buildliner_add_app(
    fm
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/fm
)

buildliner_add_app(
    cs
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/cs
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

#buildliner_add_app(
#    hs
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/hs
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/hs/hs_custom.c
#)

# This is temporarily removed until we can remove the OS dependency
# in cf, i.e. "arpa/inet.h"
#buildliner_add_app(
#    cf
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/cf
#)
#
#buildliner_add_app(
#    cs
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
#)
#
#buildliner_add_app(
#    fm
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/fm
#)
#
#buildliner_add_app(
#    hk
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hk
#)
#
#buildliner_add_app(
#    hs
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hs
#    CONFIG_SOURCES
#        ${PROJECT_SOURCE_DIR}/config/shared/apps/hs/hs_custom.c
#)

buildliner_add_app(
    lc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/lc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/lc/src/lc_custom.c
)

buildliner_add_app(
    ci
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ci
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ci/ci_slip_sed.c
)

buildliner_add_app(
    to
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/to
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to/to_slip_sed.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/to/tables/to_backup_cfg.c
)


buildliner_add_app(
    amc
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/amc
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/amc/amc_custom.cpp
)


buildliner_add_app(
    sens
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sens
)

buildliner_add_app(
    vm
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/vm
)

buildliner_add_app(
    ld
    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/multicopter/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fixedwing/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/fixedwing/gemini2/cpd/apps/ld
)

buildliner_add_app(
    nav
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/nav
)

buildliner_add_app(
    rcin
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/rcin
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/rcin/sed/src/rcin_custom.c
)

buildliner_add_app(
    pe
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/pe
)

buildliner_add_app(
    qae
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/qae
)

buildliner_add_app(
    sc
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sc
)

buildliner_add_app(
    icm20689
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/icm20689/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/icm20689
)


buildliner_add_app(
    hmc5883
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/hmc5883/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/hmc5883
    CONFIG_SOURCES ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/hmc5883/src/hmc5883_custom.c
)


buildliner_add_app(
    ms5611
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/ms5611/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ms5611
    CONFIG_SOURCES ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ms5611/src/ms5611_custom.c
)

buildliner_add_app(
    gps
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/gps/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_interface_uart/gps_serial.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/gps/src/gps_custom.c
)

buildliner_add_app(
    aspd4525
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/aspd4525/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/aspd4525
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/aspd4525/src/aspd4525_custom.c
)

buildliner_add_app(
    fpc
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fpc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/fpc
)

buildliner_add_app(
    fac
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fac/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/fac
)



buildliner_add_app(
    zusup
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/zusup/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/zusup
)

#buildliner_add_app(
#    sbn
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn/src/sbn_remap_tbl.c
#)

#buildliner_add_app(
#    sbn_shm
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbn_shm/fsw/for_build
#)

## This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    ulr
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ulr/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr/ulr_custom.cpp
#)
#
## This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    rgbled
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled/src/rgbled_custom.c
#)

# This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    gps
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_interface_uart/gps_sed.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_custom.c
#)
#
#
## This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    bat
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat/bat_custom.cpp
#)



## This is temporarily removed until we can geet px4lib back in
#buildliner_add_app(
#    lgc
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc/lgc_custom.cpp
#)


#buildliner_add_app(
#    sbn_shm
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbn_shm/fsw/for_build
#)

#buildliner_add_app(
#    sbn
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn/src/sbn_remap_tbl.c
#)
