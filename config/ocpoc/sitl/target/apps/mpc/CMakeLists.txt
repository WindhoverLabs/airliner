set(APP_NAME mpc)

buildliner_add_table(
    ${APP_NAME}
    NAME mpc_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpc_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpc_config.c)
