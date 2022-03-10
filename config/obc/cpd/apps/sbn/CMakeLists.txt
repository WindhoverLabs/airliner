set(APP_NAME sbn)

buildliner_add_table(
    ${APP_NAME}
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/tables/shm/SbnModuleData.dat
)

buildliner_add_table(
    ${APP_NAME}
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/tables/shm/SbnPeerData.dat
)
