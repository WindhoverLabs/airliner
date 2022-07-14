set(APP_NAME fpc)

buildliner_add_table(
    ${APP_NAME}
    NAME fpc_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/fpc_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/fpc _config.c)
