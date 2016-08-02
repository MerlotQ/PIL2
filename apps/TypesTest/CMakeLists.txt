set(APPS2BUILD SE3_Test SIM3_Test SO3_Test)
#  SE3_Test

foreach(APPNAME ${APPS2BUILD})
	if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${APPNAME})
		add_subdirectory(${APPNAME})
	endif()
endforeach()
