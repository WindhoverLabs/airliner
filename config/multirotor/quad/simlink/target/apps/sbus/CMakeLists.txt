set(APP_NAME sbus)

buildliner_add_table(
    ${APP_NAME}
    NAME sbus_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sbus_config.c
)
