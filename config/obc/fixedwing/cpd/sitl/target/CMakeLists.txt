include(../set-vars.cmake)

buildliner_initialize(
    EMBED_SYMTAB
    CPU_ID CPD
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
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
    pq_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
    # TODO move this table build to sbn module?
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/pq_lib
)

buildliner_add_app(
    sedlib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sedlib/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sedlib/sedlib_shmem.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/sedlib/sedlib_config.c
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


# sch_custom_v1.c:  Traditional platform independent layer that uses OS_Timer 
#                   to drive minor frames.
# sch_custom_v2.c:  SED specific layer that uses 250Hz minor frame interrupts
#                   to drive minor frames.  This assumes you are using the
#                   Windhover Labs avionics platform with the major and minor
#                   frame timers controlled by the Sensor and Effector Domain 
#                   (SED).  The interrupt handler included in this file does
#                   not clear the interrupt.  The SED clears the interrupt
#                   automatically.
#buildliner_add_app(
#    sch
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch/sch_custom_v1.c
#)

buildliner_add_app(
    cf
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/cf
)

buildliner_add_app(
    ci
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ci
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ci/ci_slip_sed.c
)

buildliner_add_app(
    to
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to/to_slip_sed.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to/tables/to_backup_cfg.c
)

buildliner_add_app(
    fac
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fac/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/fac
)

buildliner_add_app(
    aspd4525
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/aspd4525/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/obc/fixedwing/cpd/apps/aspd4525
)
