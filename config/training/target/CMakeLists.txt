buildliner_initialize(
    PSP ${PROJECT_SOURCE_DIR}/core/psp/pc-linux-wh
    
    OSAL ${PROJECT_SOURCE_DIR}/core/osal/posix-fast
    
    CONFIG
        ${PROJECT_SOURCE_DIR}/config/training/inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
        
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
    
    FILESYS
        /cf/log
        /ram
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/apps/sch/fsw/src/sch_custom.c
)

buildliner_add_table(
    sch
    NAME sch_def_msgtbl
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/tables/sch_def_msgtbl.c
)

buildliner_add_table(
    sch
    NAME sch_def_schtbl
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/tables/sch_def_schtbl.c
)

buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom_udp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_backup_cfg.c
)

buildliner_add_table(
    to
    NAME to_udp_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_udp_cfg.c
)

buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
)

buildliner_add_table(
    ci
    NAME ci_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/tables/ci_config.c
)

buildliner_add_app(
    mfa
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mfa/fsw/for_build
)
