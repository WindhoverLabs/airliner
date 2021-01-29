# Set source for custom code.
set(APP_CUSTOM_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/vc_dev_io_i2c/vc_dev_io_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_transmit_udp/vc_transmit_udp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_dev_io_v4l/vc_dev_io_v4l.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vc_custom.c 
    PARENT_SCOPE
    )

