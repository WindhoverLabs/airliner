set(APP_NAME mpu9250)

buildliner_add_table(
    ${APP_NAME}
    NAME mpu9250_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpu9250_config.c
)
