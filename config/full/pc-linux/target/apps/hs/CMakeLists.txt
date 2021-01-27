set(APP_NAME hs)

buildliner_add_app_src(
    ${APP_NAME}
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/hs_custom.c
)

buildliner_add_table(
    ${APP_NAME}
    NAME hs_amt
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hs_amt.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME hs_emt
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hs_emt.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME hs_mat
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hs_mat.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME hs_xct
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hs_xct.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME hs_xct_test
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/hs_xct_test.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)


