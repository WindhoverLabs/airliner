set(APP_NAME sedpwm)

buildliner_add_table(
    ${APP_NAME}
    NAME sedpwm_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sedpwm_config.c
)
