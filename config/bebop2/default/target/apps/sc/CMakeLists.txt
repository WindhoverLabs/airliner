set(APP_NAME sc)

buildliner_add_table(
    ${APP_NAME}
    NAME sc_ats1
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sc_ats1.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME sc_rts001
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sc_rts001.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME sc_rts002
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sc_rts002.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
