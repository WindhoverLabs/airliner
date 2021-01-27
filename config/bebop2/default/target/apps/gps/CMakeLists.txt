set(APP_NAME gps)

buildliner_add_table(
    ${APP_NAME}
    NAME gps_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/gps_config.c
)
