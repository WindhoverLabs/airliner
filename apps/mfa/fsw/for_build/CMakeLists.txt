buildliner_add_app_def(mfa
    FILE MFA

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_symbols.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/arduart/uah_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/arduart/uah_pipe.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/arduart/
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/arduart/uart_target

)