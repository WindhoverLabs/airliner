set(APP_NAME vm)

buildliner_add_table(
    ${APP_NAME}
    NAME vm_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/vm_config.c
)

buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/vm_config.c)
