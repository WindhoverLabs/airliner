# This is where we actually build the elf2cfetbl tool
set(CMAKE_C_FLAGS "-g -Wformat=0")
set(CMAKE_CXX_FLAGS "-g -Wformat=0")
add_executable(hitl_bridge 
    ${CMAKE_CURRENT_SOURCE_DIR}/ccsds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uio_mailbox.cpp
)

target_include_directories(hitl_bridge PUBLIC ${PROJECT_SOURCE_DIR}/tools/mavlink/v2.0/common)
target_link_libraries(hitl_bridge PUBLIC "-pthread")