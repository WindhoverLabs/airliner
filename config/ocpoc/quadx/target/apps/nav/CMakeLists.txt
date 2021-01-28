set(APP_NAME nav)

buildliner_add_table(
    ${APP_NAME}
    NAME nav_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/nav_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/nav_config.c)
