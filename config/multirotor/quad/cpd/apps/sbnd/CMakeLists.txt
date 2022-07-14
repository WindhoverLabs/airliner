set(APP_NAME sbnd)

buildliner_add_table(
    ${APP_NAME}
    NAME sbnd_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sbnd_config.c
)
