set(APP_NAME ms5611)

buildliner_add_table(
    ${APP_NAME}
    NAME ms5611_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ms5611_config.c
)
