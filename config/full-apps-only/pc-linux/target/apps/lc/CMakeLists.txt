set(APP_NAME lc)

buildliner_add_table(
    ${APP_NAME}
    NAME lc_def_adt
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/lc_def_adt.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME lc_def_wdt
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/lc_def_wdt.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_def_src(
    ${APP_NAME}
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lc_custom.c
)