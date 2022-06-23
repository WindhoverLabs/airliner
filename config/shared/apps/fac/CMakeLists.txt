set(APP_NAME fac)

buildliner_add_table(
    ${APP_NAME}
    NAME fac_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/fac_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/fac_config.c)
