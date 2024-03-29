include(../set-vars.cmake)

buildliner_initialize(
    CPU_ID PPD
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /cf/apps
        /cf/log
        /cf/uplink/ppd_to_cpd/class1/priority2/
        /cf/uplink/ppd_to_cpd/class2/priority2/
        /cf/uplink/ppd_to_cpd/class1/priority4/
        /cf/uplink/ppd_to_cpd/class2/priority4/
        /cf/uplink/ppd_to_cpd/class1/priority6/
        /cf/uplink/ppd_to_cpd/class2/priority6/
        /cf/downlink/ppd_to_gnd/class1/priority2/
        /cf/downlink/ppd_to_gnd/class2/priority2/
        /cf/downlink/ppd_to_gnd/class1/priority4/
        /cf/downlink/ppd_to_gnd/class2/priority4/
        /cf/downlink/ppd_to_gnd/class1/priority6/
        /cf/downlink/ppd_to_gnd/class2/priority6/
        /cf/downlink/cpd_to_ppd/class1/priority2/
        /cf/downlink/cpd_to_ppd/class2/priority2/
        /cf/downlink/cpd_to_ppd/class1/priority4/
        /cf/downlink/cpd_to_ppd/class2/priority4/
        /cf/downlink/cpd_to_ppd/class1/priority6/
        /cf/downlink/cpd_to_ppd/class2/priority6/
        /cf/downlink/cpd_to_gnd/class1/priority2/
        /cf/downlink/cpd_to_gnd/class2/priority2/
        /cf/downlink/cpd_to_gnd/class1/priority4/
        /cf/downlink/cpd_to_gnd/class2/priority4/
        /cf/downlink/cpd_to_gnd/class1/priority6/
        /cf/downlink/cpd_to_gnd/class2/priority6/
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cfe_es_startup.scr
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
    io_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/io_lib/fsw/for_build
)

buildliner_add_app(
    ipc_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/ipc_lib/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/ipc_lib/ipc_lib_custom.c
)

#buildliner_add_app(
#    pq_lib
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
#    # TODO move this table build to sbn module?
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/pq_lib
#)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sch/sch_custom_rt.c
)


buildliner_add_app(
    sbnd
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbnd/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sbnd
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/apps/sbnd/fsw/src/shm/sbnd_custom.cpp
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/cf
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/ds
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
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/ci
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/ci/ci_custom.c
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/to
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/to/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/to/to_custom.c
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/ea
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/ea/src/ea_custom.c
)

#buildliner_add_app(
#    sbn
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sbn
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/sbn/src/sbn_remap_tbl.c
#)

#buildliner_add_app(
#    sbn_udp
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/modules/udp/fsw/for_build
#)

buildliner_add_app(
    adsbr
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/adsbr/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/adsbr
)

# buildliner_add_app(
#     hes
#     DEFINITION ${PROJECT_SOURCE_DIR}/apps/hes/fsw/for_build
#     CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/hes
#     # CONFIG_SOURCES
#     #     ${CMAKE_CURRENT_SOURCE_DIR}/../../apps/hes/src/hes_custom.c
# )
