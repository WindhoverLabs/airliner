buildliner_add_app_def(mfa
	FILE MFA 
	
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_app.c
		${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_symbols.c
	
    INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/
            ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc
            
    REFERENCE_CONFIG 
           ${PROJECT_SOURCE_DIR}/apps/mfa/fsw/platform_inc
)