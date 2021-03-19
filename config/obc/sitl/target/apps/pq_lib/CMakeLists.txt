set(APP_NAME pq_lib)

buildliner_add_table(
    ${APP_NAME}
    NAME pq_cfg
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/pq_cfg.c
)
