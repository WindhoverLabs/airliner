set(APP_NAME sed)

buildliner_add_table(
    ${APP_NAME}
    NAME sed_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sed_config.c
)
