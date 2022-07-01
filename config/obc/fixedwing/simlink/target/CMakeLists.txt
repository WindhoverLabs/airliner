set(EMBED_SYMTAB true)
set(EMBED_INITRD true)

buildliner_initialize(
    CPU_ID     SIMLINK
    PSP        ${PROJECT_SOURCE_DIR}/core/psp/pc-linux-wh
    OSAL       ${PROJECT_SOURCE_DIR}/core/osal/posix-fast
    CORE_BINARY_NAME simlink
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../shared/inc
    FILESYS
        /cf/apps
        /cf/log
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/term-sitl.cfg DESTINATION ${CMAKE_BINARY_DIR}/../..)


buildliner_add_app(
    cfs_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    cvt
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cvt/fsw/for_build
)

buildliner_add_app(
    simlink
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/simlink/gazebo9/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/simlink
)
