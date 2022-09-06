set(APP_NAME fm)

buildliner_add_table(
    ${APP_NAME}
    NAME fm_freespace
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/fm_freespace.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)


