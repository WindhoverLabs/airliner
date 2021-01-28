set(APP_NAME vc)

buildliner_add_app_unit_test(vc-custom-ut
    UTASSERT
    SOURCES
        # Unit test runner
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_testrunner.c
        # Unit test for custom device module v4l




        # Custom device code udp 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_udp_test/vc_app_custom_device_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_udp_test/vc_app_custom_device_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_udp_test/vc_custom_device_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_udp_test/vc_custom_device_test_utils.h
        # Unit test for custom transmit module
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/vc_app_custom_transmit_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/vc_app_custom_transmit_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/vc_custom_transmit_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/vc_custom_transmit_test_utils.h
        # Unit test for shared custom module
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_shared_test/vc_app_custom_shared_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_shared_test/vc_app_custom_shared_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_shared_test/vc_custom_shared_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_shared_test/vc_custom_shared_test_utils.h
        # Platform stubs
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_platform_stubs/vc_platform_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_platform_stubs/vc_platform_stubs.h
        # VC App stubs
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_app_stubs/vc_app_stubs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_app_stubs/vc_app_stubs.h
        # Custom code (code under test)
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_transmit_udp/vc_transmit_udp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_transmit_udp/vc_transmit_udp.h
        # Custom device code v4l 


        # Custom device code udp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_dev_io_udp/vc_dev_io_udp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_dev_io_udp/vc_dev_io_udp.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_custom_shared.h

    INCLUDES
        # Custom code under test
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        # Custom v4l code

        # Custom udp code
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_dev_io_udp/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vc_transmit_udp/
        # Test code
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/
        # Custom v4l code
        #${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_v4l_test/
        # Custom udp code 
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_dev_io_udp_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_transmit_udp_test/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_platform_stubs/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_app_stubs/
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/vc_custom_shared_test/
        # Sealed code
        ${PROJECT_SOURCE_DIR}/apps/vc/fsw/src/

    WRAPPERS
        # Wrapped functions (see platform stubs)
        ioctl
        open
        close
        select
        socket
        bind
        sendto
        recv
)
