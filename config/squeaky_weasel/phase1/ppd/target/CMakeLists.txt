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
        ${PROJECT_SOURCE_DIR}/config/shared/inc
    FILESYS
        /cf/apps
        /cf/download
        /cf/log
        /cf/upload
        /ram
    STARTUP_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe_es_startup.scr
    
    CONFIG_SOURCES
        ${PROJECT_SOURCE_DIR}/config/shared/inc/cfe_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/cfe_msgids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/osconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cfe_platform_cfg.h

    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/cfe/cfe_msg_overrides.json
        ${PROJECT_SOURCE_DIR}/config/shared/defs/airliner_overrides.json
)

add_subdirectory(allstop allstop)
set_target_properties(allstop
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe
)

buildliner_add_app(
    cfs_lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cfs_lib/fsw/for_build
)

buildliner_add_app(
    px4lib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/px4lib/fsw/for_build
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/px4lib/px4_msg_overrides.json
)

buildliner_add_app(
    prmlib
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/prmlib/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/prm
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/prm/prm_custom_overrides.json
)

buildliner_add_app(
    sch
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sch/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/sch/sch_custom_rt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sch_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sch_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/sch_perfids.h
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
        ${PROJECT_SOURCE_DIR}/config/shared/inc/cf_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/cf/cf_msg_overrides.json
)

buildliner_add_app(
    cs
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/cs/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/cs
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cs_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cs_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/cs_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/cs_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/cs/cs_msg_overrides.json
)

buildliner_add_app(
    ds
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ds/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ds
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ds_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ds_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ds_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/ds/ds_msg_overrides.json
)

buildliner_add_app(
    fm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/fm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/fm
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/fm_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/fm_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/fm_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/fm/fm_msg_overrides.json
)

buildliner_add_app(
    hk
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hk/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hk
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hk_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hk_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/hk_perfids.h
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
        ${PROJECT_SOURCE_DIR}/config/shared/inc/hs_perfids.h
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
        ${PROJECT_SOURCE_DIR}/config/shared/inc/lc_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/lc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/lc/lc_msg_overrides.json
)

buildliner_add_app(
    md
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/md/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/md
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/md_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/md_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/md_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/md/md_msg_overrides.json
)

buildliner_add_app(
    mm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mm
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mm_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mm_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mm_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mm_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/mm/mm_msg_overrides.json
)

buildliner_add_app(
    sc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sc_platform_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sc_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/sc_perfids.h
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
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ci_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ci_perfids.h
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
        ${PROJECT_SOURCE_DIR}/config/shared/inc/to_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/to_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/to/to_msg_overrides.json
)

buildliner_add_app(
    ea
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ea/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ea
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ea/src/ea_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ea_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ea_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ea_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/ea/ea_msg_overrides.json
)

buildliner_add_app(
    vc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_dev_io_v4l/vc_dev_io_v4l.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_dev_io_v4l/vc_dev_io_v4l.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_transmit_udp/vc_transmit_udp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_transmit_udp/vc_transmit_udp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/src/vc_custom_shared.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/vc_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/vc_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/vc_mission_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/vc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/vc/vc_msg_overrides.json
)

buildliner_add_app(
    amc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/amc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc/amc_custom.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/amc_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/amc_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/amc_perfids.h
    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/amc/amc_msg_overrides.json
)

buildliner_add_app(
    mac
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mac/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mac
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mac_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mac_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mac_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/mac/mac_msg_overrides.json
)

buildliner_add_app(
    ulr
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ulr/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ulr/ulr_custom.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ulr_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ulr_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ulr_perfids.h
)

buildliner_add_app(
    rgbled
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rgbled/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled/src/rgbled_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled/src/rgbled_driver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rgbled_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/rgbled_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/rgbled_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/rgbled/rgbled_msg_overrides.json
)

buildliner_add_app(
    ms5611
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ms5611/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611/src/ms5611_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/ms5611/src/ms5611_spi.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ms5611_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ms5611_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ms5611_perfids.h
)

buildliner_add_app(
    gps
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/gps/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_interface_uart/gps_serial.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_interface_uart/gps_serial.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_ubx_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_nav.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_ack.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_parser_ubx/gps_parser_ubx_mon.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_custom_shared.h
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/src/gps_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/gps_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/gps_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/gps_perfids.h
    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/gps/gps_msg_overrides.json
)

buildliner_add_app(
    sens
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/sens/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/sens
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/sens_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/sens_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/sens_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/sens/sens_msg_overrides.json
)

buildliner_add_app(
    mpc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/mpc_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mpc_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mpc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpc/mpc_msg_overrides.json
)

buildliner_add_app(
    vm
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/vm/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/vm
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/vm_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/vm_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/vm_perfids.h
    DESIGN_DEFINITION
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/vm/vm_msg_overrides.json
)

buildliner_add_app(
    ld
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/ld/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/ld
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ld_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ld_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/ld_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/ld/ld_msg_overrides.json
)

buildliner_add_app(
    nav
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/nav/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/nav
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/nav_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/nav_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/nav_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/nav/nav_msg_overrides.json
)

buildliner_add_app(
    rcin
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/rcin/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin/src/rcin_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin/src/rcin_sbus.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rcin_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/rcin_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/rcin_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/rcin/rcin_msg_overrides.json
)

buildliner_add_app(
    bat
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/bat/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat/bat_custom.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/bat_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/bat_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/bat_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/bat/bat_msg_overrides.json
)

buildliner_add_app(
    mpu9250
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/mpu9250/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpu9250
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpu9250/src/mpu9250_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/mpu9250/src/mpu9250_spi.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/ms5611_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mpu9250_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/mpu9250_perfids.h
)

buildliner_add_app(
    hmc5883
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/hmc5883/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883/src/hmc5883_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/hmc5883/src/hmc5883_i2c.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/hmc5883_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/hmc5883_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/hmc5883_perfids.h
)

buildliner_add_app(
    pe
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/pe/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/pe
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/pe_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/pe_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/pe_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/pe/pe_msg_overrides.json
)

buildliner_add_app(
    qae
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/qae/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/qae
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/qae_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/qae_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/qae_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/qae/qae_msg_overrides.json
)

buildliner_add_app(
    lgc
    DEFINITION ${PROJECT_SOURCE_DIR}/apps/lgc/fsw/for_build
    CONFIG     ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc
    CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc/lgc_custom.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc/lgc_platform_cfg.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/lgc_msgids.h
        ${PROJECT_SOURCE_DIR}/config/shared/inc/lgc_perfids.h
    DESIGN_DEFINITION ${CMAKE_CURRENT_SOURCE_DIR}/apps/lgc/lgc_msg_overrides.json
)
