set(APP_NAME icm20689)

buildliner_add_table(
    ${APP_NAME}
    NAME icm20689_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/icm20689_config.c
)
