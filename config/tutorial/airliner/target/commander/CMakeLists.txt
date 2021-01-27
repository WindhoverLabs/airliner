
file(INSTALL "${CMAKE_CURRENT_SOURCE_DIR}/commander_workspace" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CDR_WORKSPACE ${CMAKE_CURRENT_BINARY_DIR}/commander_workspace)
set(AIRLINER_MSG_DEF_PATH ${EXPLAIN_DIR})
set(AIRLINER_PROTO_PATH ${EXPLAIN_DIR}/src/protos)
set(COMMANDER_PATH ${PROJECT_SOURCE_DIR}/tools/commander)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setvars.in ${CMAKE_CURRENT_BINARY_DIR}/setvars @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/start_commander.in ${CMAKE_CURRENT_BINARY_DIR}/start_commander @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_commander.in ${CMAKE_CURRENT_BINARY_DIR}/test_commander @ONLY)

add_custom_target(start-commander)
add_custom_command(TARGET start-commander
    DEPENDS ground_tools
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/start_commander
)

add_custom_target(test-commander)
add_custom_command(TARGET test-commander
    DEPENDS ground_tools
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_commander
)

set(COMMANDER_WORKSPACE_PLUGINS_DIR ${CDR_WORKSPACE}/plugins PARENT_SCOPE)
