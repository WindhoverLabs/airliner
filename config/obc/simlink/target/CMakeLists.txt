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
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc
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


buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_custom_rt.c
)


buildliner_add_app(
    ci
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ci/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ci/ci_custom.c
)


buildliner_add_app(
    to
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/to/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/to
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/tables/to_backup_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_custom.c
)


buildliner_add_app(
    sedlib
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sedlib/fsw/for_build
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sedlib/sedlib_config.c
        ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sedlib/sedlib_shmem.c
)


buildliner_add_app(
    ublox_m8n
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/gps/simlink/ublox_m8n/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ublox_m8n
)


buildliner_add_app(
    hmc5883
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/hmc5883/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883
)

buildliner_add_app(
    aspd4525
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/aspd4525/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/aspd4525
)


buildliner_add_app(
    icm20689
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/icm20689/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/icm20689
)


buildliner_add_app(
    ms5611
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5611/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611
)


buildliner_add_app(
    rfd900x
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rfd900x/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rfd900x
)


buildliner_add_app(
    sedpwm
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sedpwm/simlink/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sedpwm
)


buildliner_add_app(
    sbus
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sedsbus/simlink/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sbus
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sbus/sbus_custom.c
)

buildliner_add_app(
    sed
    DEFINITION ${PROJECT_SOURCE_DIR}/../private/apps/sed/simlink/for_build
    CONFIG     ${PROJECT_SOURCE_DIR}/../private/config/obc/simlink/target/apps/sed
)
