set(APP_NAME ublox_m8n)

buildliner_add_table(
    ${APP_NAME}
    NAME ubm8n_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ublox_m8n_config.c
)
