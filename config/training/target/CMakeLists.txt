buildliner_initialize(
    PSP ${PROJECT_SOURCE_DIR}/core/psp/pc-linux-wh
    
    OSAL ${PROJECT_SOURCE_DIR}/core/osal/posix-fast
    
    CONFIG
        ${PROJECT_SOURCE_DIR}/config/training/inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
        
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
)

buildliner_add_app(
    mfa
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mfa/fsw/for_build
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
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_backup_cfg.c
)

