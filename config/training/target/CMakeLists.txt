buildliner_initialize(
    PSP ${PROJECT_SOURCE_DIR}/core/psp/pc-linux-wh
    
    OSAL ${PROJECT_SOURCE_DIR}/core/osal/posix-fast
    
    CORE_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools
    
    CONFIG
        ${PROJECT_SOURCE_DIR}/config/training/inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
)