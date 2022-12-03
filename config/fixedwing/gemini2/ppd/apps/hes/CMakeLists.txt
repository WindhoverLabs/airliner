set(APP_NAME hes)

buildliner_add_table(
    ${APP_NAME}
    NAME hes_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hes_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# buildliner_add_app_unit_test_src(${APP_NAME} SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hes_config.c)

# buildliner_add_app_unit_test(hes-custom-ut
#     UTASSERT
#     NO_HELGRIND
#     NO_MASSIF

#     SOURCES
#         ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/hes_testrunner.c
#         ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/hes_custom_test.c
#         ${CMAKE_CURRENT_SOURCE_DIR}/hes_custom.c
#         ${PROJECT_SOURCE_DIR}/apps/hes/fsw/src/hes_app.c
#         ${PROJECT_SOURCE_DIR}/apps/hes/fsw/src/hes_config_utils.c
#         ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/hes_mock_os_calls.c
#         ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/hes_test_utils.c
#         ${PROJECT_SOURCE_DIR}/core/base/ut_assert/src/ut_cfe_es_hooks.c
        
#     INCLUDES
#         ${PROJECT_SOURCE_DIR}/apps/hes/fsw/src/
        
#     WRAPPERS
#         socket
#         bind
    
#     VALGRIND_SUPPRESSION_FILE
#         ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/hes-custom.supp
# )
