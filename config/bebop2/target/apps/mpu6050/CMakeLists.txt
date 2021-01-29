set(APP_NAME mpu6050)

buildliner_add_table(
    ${APP_NAME}
    NAME mpu6050_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpu6050_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mpu6050_config.c)
