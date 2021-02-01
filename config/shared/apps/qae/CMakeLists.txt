set(APP_NAME qae)

buildliner_add_table(
    ${APP_NAME}
    NAME qae_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/qae_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/qae_config.c)
