include(../set-vars.cmake)

# Put target specific functions here.  This is where we build most everything.  Everything that either uses the
# cross toolchain or has target independent functions.

buildliner_initialize(
    PSP ${PSP}
    OSAL ${OSAL}
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc
    FILESYS
        /cf/apps
        /cf/log
        /cf/downlink/ch0/poll0
        /cf/downlink/ch0/poll1
        /cf/downlink/ch0/poll2
        /cf/downlink/ch0/poll3
        /cf/downlink/ch0/poll4
        /cf/downlink/ch0/poll5
        /cf/downlink/ch0/poll6
        /cf/downlink/ch0/poll7
        /cf/downlink/ch1/poll0
        /cf/downlink/ch1/poll1
        /cf/downlink/ch1/poll2
        /cf/downlink/ch1/poll3
        /cf/downlink/ch1/poll4
        /cf/downlink/ch1/poll5
        /cf/downlink/ch1/poll6
        /cf/downlink/ch1/poll7
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
    
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/cfe_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/cfe_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/osconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cfe_platform_cfg.h

    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe/cfe_msg_overrides.json
)


buildliner_add_app(
    cfs_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_custom_rt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sch_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sch_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/sch_perfids.h
    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_msg_overrides.json
)

buildliner_add_app(
    cf
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cf/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/cf
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cf_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cf_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/cf_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/cf/cf_msg_overrides.json
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/cs
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cs_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cs_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/cs_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/cs_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/cs/cs_msg_overrides.json
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ds
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ds_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ds_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/ds_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/ds/ds_msg_overrides.json
)

buildliner_add_app(
    fm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/fm
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/fm_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/fm_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/fm_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/fm/fm_msg_overrides.json
)

buildliner_add_app(
    hk
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hk
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hk_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hk_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/hk_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/hk/hk_msg_overrides.json
)

buildliner_add_app(
    hs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hs/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hs
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/hs/hs_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hs_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hs_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/hs_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/hs/hs_msg_overrides.json
)

buildliner_add_app(
    lc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/lc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/lc/src/lc_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/lc_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/lc_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/lc_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/lc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/lc/lc_msg_overrides.json
)

buildliner_add_app(
    md
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/md
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/md_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/md_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/md_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/md/md_msg_overrides.json
)

buildliner_add_app(
    mm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mm
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mm_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mm_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/mm_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/mm_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/mm/mm_msg_overrides.json
)

buildliner_add_app(
    sc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sc_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sc_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/sc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/sc/sc_msg_overrides.json
)

buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ci_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ci_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/ci_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/ci_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_msg_overrides.json
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/to
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom_udp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/to_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/to_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/to_mission_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc/to_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_msg_overrides.json
)
