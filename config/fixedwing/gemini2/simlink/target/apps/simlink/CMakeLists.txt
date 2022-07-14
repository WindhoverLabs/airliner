set(APP_NAME simlink)

buildliner_add_table(
    ${APP_NAME}
    NAME simlink_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/simlink_config.c
)
