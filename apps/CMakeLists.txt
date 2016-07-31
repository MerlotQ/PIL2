set(APPS2BUILD Camera_Test ClassLoader_Test CV_Test GUI_Test Svar_Test Timer_Test Tests)

file(GLOB APPS2BUILD "*")
message("APPS2BUILD: " ${APPS2BUILD})

foreach(APPNAME ${APPS2BUILD})
	if(IS_DIRECTORY ${APPNAME})
		add_subdirectory(${APPNAME})
	endif()
endforeach()
