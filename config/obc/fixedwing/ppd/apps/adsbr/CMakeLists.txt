set(APP_NAME adsbr)

buildliner_add_table(
    ${APP_NAME}
    NAME adsbr_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/adsbr_config.c
)
