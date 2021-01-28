include(../set-vars.cmake)

# Put target specific functions here.  This is where we build most everything.  Everything that either uses the
# cross toolchain or has target independent functions.

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
    sch
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_custom_rt.c
)

buildliner_add_app(
    cs
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/config/shared/apps/cs
)

buildliner_add_app(
    ds
    EMBEDDED
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ds
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

#buildliner_add_app(
#    ci
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
#)
#
#buildliner_add_app(
#    to
#    EMBEDDED
#    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
#    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/to
#    CONFIG_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_backup_cfg.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
#)


