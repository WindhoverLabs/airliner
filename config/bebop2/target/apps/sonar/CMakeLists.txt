set(APP_NAME sonar)

buildliner_add_table(
    ${APP_NAME}
    NAME sonar_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sonar_config.c
)
