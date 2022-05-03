# Put host specific functions here.  This allows us to build things like elf2cfetbl which are built using the host
# toolchain, not the cross toolchain.
buildliner_build_cfe_host_tools(
    PSP ${PROJECT_SOURCE_DIR}/core/psp/pc-linux-wh
    
    OSAL ${PROJECT_SOURCE_DIR}/core/osal/posix-fast
    
    CONFIG
        ${PROJECT_SOURCE_DIR}/config/training/inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
)
