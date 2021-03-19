set(APP_NAME prm)

buildliner_add_table(
    ${APP_NAME}
    NAME prm_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/prm_config.c
)
