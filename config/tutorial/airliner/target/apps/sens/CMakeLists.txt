set(APP_NAME sens)

buildliner_add_table(
    ${APP_NAME}
    NAME sens_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sens_config.c
)
