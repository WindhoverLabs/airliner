buildliner_add_app_def(mfa
	FILE MFA 
	
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_app.c
		${CMAKE_CURRENT_SOURCE_DIR}/../src/mfa_symbols.c
	
	INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/../src/

	LIBS 
        m 
)
