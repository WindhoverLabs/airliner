add_subdirectory(${PROJECT_SOURCE_DIR}/../private/config/obc/cpd/apps/to private)

buildliner_add_table(
    to
    NAME to_config
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/to_config.c
)