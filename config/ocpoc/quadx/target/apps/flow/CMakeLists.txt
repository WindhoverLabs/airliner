set(APP_NAME flow)

buildliner_add_table(
    ${APP_NAME}
    NAME flow_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/flow_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
