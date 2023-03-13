set(APP_NAME cf)

buildliner_add_table(
    ${APP_NAME}
    NAME cf_cfgtable
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/cf_cfgtable.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/cf_cfgtable.c)
