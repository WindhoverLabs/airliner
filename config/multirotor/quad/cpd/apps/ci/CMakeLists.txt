add_subdirectory(${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/ci private)

buildliner_add_app_unit_test_src(ci SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ci_config.c)

buildliner_add_table(
    ci
    NAME ci_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/ci_config.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

