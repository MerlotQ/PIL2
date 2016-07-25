set(APPS2BUILD CameraTest ClassLoaderTest CV_Test GUI_Test Svar_Test Timer_Test Tests)
#  SE3_Test

foreach(APPNAME ${APPS2BUILD})
	if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${APPNAME})
		add_subdirectory(${APPNAME})
	endif()
endforeach()
