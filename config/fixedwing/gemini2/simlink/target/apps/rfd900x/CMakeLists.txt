set(APP_NAME rfd900x)

buildliner_add_table(
    ${APP_NAME}
    NAME rfd900x_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/rfd900x_config.c
)
