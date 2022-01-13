set(APP_NAME em91x)

buildliner_add_table(
    ${APP_NAME}
    NAME em91x_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/em91x_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)


