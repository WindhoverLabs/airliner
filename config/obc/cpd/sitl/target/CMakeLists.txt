include(../set-vars.cmake)

buildliner_initialize(
    CPU_ID CPD
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /cf/apps
        /cf/download
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
        ${CMAKE_CURRENT_SOURCE_DIR}/../../target/cfe_es_startup.scr
        
    COMMANDER_WORKSPACE
        ${PROJECT_SOURCE_DIR}/build/obc/sitl_commander_workspace
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
    px4lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
)

buildliner_add_app(
    io_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/io_lib/fsw/for_build
)

buildliner_add_app(
    pq_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
    # TODO move this table build to sbn module?
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/pq_lib
)

buildliner_add_app(
    prmlib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/prm
)

buildliner_add_app(
    sch
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch/sch_custom_rt.c
)

buildliner_add_app(
    cf
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/cf
)

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
    sbn
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sbn
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sbn/src/sbn_remap_tbl.c
)

#buildliner_add_app(
#    sbn_mbox
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbn_mbox/fsw/for_build
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/pq_lib/tables/pq_backup_cfg.c
#)

buildliner_add_app(
    ci
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ci
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
)

buildliner_add_app(
    to
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
)

#buildliner_add_app(
#    amc
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/amc
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc/amc_custom.cpp
#)
#
#buildliner_add_app(
#    md
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/md
#)
#
#buildliner_add_app(
#    mm
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
#)
#
#buildliner_add_app(
#    sc
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
#    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/sc
#)
#
#buildliner_add_app(
#    sbn_udp
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/modules/udp/fsw/for_build
#)

