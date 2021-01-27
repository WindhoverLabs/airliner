include(../set-vars.cmake)

buildliner_build_cfe_host_tools(
    PSP ${PSP}
    OSAL ${OSAL}
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc
)
