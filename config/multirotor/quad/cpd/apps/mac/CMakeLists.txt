set(APP_NAME mac)

buildliner_add_table(
    ${APP_NAME}
    NAME mac_param
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mac_param.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/mac_param.c)
