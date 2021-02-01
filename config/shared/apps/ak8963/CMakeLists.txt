set(APP_NAME ak8963)

buildliner_add_table(
    ${APP_NAME}
    NAME ak8963_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ak8963_config.c
)
