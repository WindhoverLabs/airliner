set(APP_NAME mpc)

buildliner_add_table(
    ${APP_NAME}
    NAME mpc_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpc_config.c
)
