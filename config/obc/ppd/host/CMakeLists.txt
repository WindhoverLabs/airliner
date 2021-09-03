include(../set-vars.cmake)

# Put host specific functions here.  This allows us to build things like elf2cfetbl which are built using the host
# toolchain, not the cross toolchain.
buildliner_build_cfe_host_tools(
    PSP ${PSP}
    OSAL ${OSAL}
    CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
        ${PROJECT_SOURCE_DIR}/config/shared/inc
)

add_subdirectory(${PROJECT_SOURCE_DIR}/tools/sitl_gazebo gazebo)
set(GAZEBO_PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/gazebo)
set(GAZEBO_WORLDS_DIR ${PROJECT_SOURCE_DIR}/tools/sitl_gazebo/worlds)
set(GAZEBO_MODELS_DIR ${PROJECT_SOURCE_DIR}/tools/sitl_gazebo/models)
set(JMAVSIM_DIR ${PROJECT_SOURCE_DIR}/tools/jMAVSim)
target_include_directories(gazebo_gst_camera_plugin PRIVATE "/usr/include/gstreamer-1.0")
target_include_directories(gazebo_gst_camera_plugin PRIVATE "/usr/include/glib-2.0")
target_include_directories(gazebo_gst_camera_plugin PRIVATE "/usr/lib/x86_64-linux-gnu/glib-2.0/include")
target_include_directories(gazebo_gst_camera_plugin PRIVATE "/usr/lib/x86_64-linux-gnu/gstreamer-1.0/include")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/start-gazebo.in ${CMAKE_CURRENT_BINARY_DIR}/start-gazebo @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/start-jmavsim.in ${CMAKE_CURRENT_BINARY_DIR}/start-jmavsim @ONLY)
