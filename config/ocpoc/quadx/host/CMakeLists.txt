include(../set-vars.cmake)

# Put host specific functions here.  This allows us to build things like elf2cfetbl which are built using the host
# toolchain, not the cross toolchain.
buildliner_build_cfe_host_tools(
    PSP ${PSP}
    OSAL ${OSAL}
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared/inc
)