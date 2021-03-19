set(APP_NAME sbn)

buildliner_add_table(
    ${APP_NAME}
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/tables/uio_mbox/SbnModuleData.dat
)

buildliner_add_table(
    ${APP_NAME}
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/tables/uio_mbox/SbnPeerData.dat
)
