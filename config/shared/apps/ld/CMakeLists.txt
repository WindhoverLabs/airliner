set(APP_NAME ld)

buildliner_add_table(
    ${APP_NAME}
    NAME ld_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ld_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ld_config.c)
