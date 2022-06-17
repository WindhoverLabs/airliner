set(APP_NAME aspd4525)

buildliner_add_table(
    ${APP_NAME}
    NAME aspd4525_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/aspd4525_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/aspd4525_config.c)
