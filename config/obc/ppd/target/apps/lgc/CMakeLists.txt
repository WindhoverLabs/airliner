set(APP_NAME lgc)

buildliner_add_table(
    ${APP_NAME}
    NAME lgc_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/lgc_config.c
)
