set(APP_NAME pe)

buildliner_add_table(
    ${APP_NAME}
    NAME pe_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/pe_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/pe_config.c)