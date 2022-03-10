set(APP_NAME hmc5883)

buildliner_add_table(
    ${APP_NAME}
    NAME hmc5883_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hmc5883_config.c
)
