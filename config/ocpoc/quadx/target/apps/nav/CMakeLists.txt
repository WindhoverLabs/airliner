set(APP_NAME nav)

buildliner_add_table(
    ${APP_NAME}
    NAME nav_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/nav_config.c
)
