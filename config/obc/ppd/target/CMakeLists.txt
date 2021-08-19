include(../set-vars.cmake)

buildliner_initialize(
    PSP ${PSP}
    OSAL ${OSAL}
    CPU_ID PPD
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
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/../cfe_es_startup.scr
        
    COMMANDER_WORKSPACE
        ${CMAKE_BINARY_DIR}/../../commander_workspace
        
    COMMANDER_WORKSPACE_OVERLAY
        ${CMAKE_CURRENT_SOURCE_DIR}/cdr_workspace_overlay
)

#add_subdirectory(${PROJECT_SOURCE_DIR}/../private/tools/hitl_bridge hitl_bridge)

buildliner_add_app(
    cfs_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    mbp_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/mbp_lib/fsw/for_build
)

buildliner_add_app(
    px4lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
)

buildliner_add_app(
    io_lib
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/io_lib/fsw/for_build
)

buildliner_add_app(
    prmlib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/prm
)

buildliner_add_app(
    ipc_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/ipc_lib/fsw/for_build
)

buildliner_add_app(
    pq_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pq_lib/fsw/for_build
    # TODO move this table build to sbn module?
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/pq_lib
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sch/sch_custom_rt.c
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/cf
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ds
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
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ci
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ci/ci_custom.c
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/to
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/to/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/to/to_custom.c
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ea
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/ea/src/ea_custom.c
)

buildliner_add_app(
    sbn
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/sbn/src/sbn_remap_tbl.c
)

buildliner_add_app(
    sbn_udp
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sbn/modules/udp/fsw/for_build
)

buildliner_add_app(
    sbn_uio_shm
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbn_uio_shm/fsw/for_build
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_interface_uart/gps_serial.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../apps/gps/src/gps_custom.c
)

buildliner_add_app(
    hmc5883
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hmc5883/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/hmc5883
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/apps/hmc5883/src/hmc5883_custom.c
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/bat_custom.cpp
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/ads1115.cpp
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/ads1115_custom.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/ads1115.h
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/ads1115_custom.h
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/bat/ads1115_map.h
)

buildliner_add_app(
    sbnd
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sbnd/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/sbnd
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/ppd/apps/sbnd/shm/sbnd_custom.cpp
)
