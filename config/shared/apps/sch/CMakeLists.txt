set(APP_NAME sch)

buildliner_add_app_unit_test_src(
    ${APP_NAME}
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/sch_custom_rt.c
)

buildliner_add_table(
    ${APP_NAME}
    NAME sch_def_schtbl
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sch_def_schtbl.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_table(
    ${APP_NAME}
    NAME sch_def_msgtbl
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tables/sch_def_msgtbl.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

buildliner_add_app_unit_test(sch-custom-ut
    UTASSERT
    NO_HELGRIND
    NO_MASSIF
 
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/sch_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/sch_custom_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/sch_test_utils.c
	${CMAKE_CURRENT_SOURCE_DIR}/sch_custom_rt.c

    INCLUDES
        ${PROJECT_SOURCE_DIR}/apps/sch/fsw/src/
        ${PROJECT_SOURCE_DIR}/apps/sch/fsw/public_inc/
)


