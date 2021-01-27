# Set source for custom code.
set(APP_CUSTOM_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_transmit_udp/vc_transmit_udp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_dev_io_udp/vc_dev_io_udp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_custom.c 
    PARENT_SCOPE
    )

