set(APP_NAME amc)

buildliner_add_table(
    ${APP_NAME}
    NAME amc_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/amc_config.c
)


buildliner_add_table(
    ${APP_NAME}
    NAME amc_mixercfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/amc_mixercfg.c
)