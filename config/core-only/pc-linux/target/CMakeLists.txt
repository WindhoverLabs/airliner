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
)

