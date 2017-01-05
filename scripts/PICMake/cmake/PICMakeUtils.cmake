# collect all main

macro(autosetMakeType)
	if(BUILD_SHARED_LIBS)
		set(MAKE_TYPE "shared")
	else()
		set(MAKE_TYPE "static")
	endif()

	foreach(SRC_FILE ${SOURCE_FILES_ALL})
		get_filename_component(SRC_FILE_NAME ${SRC_FILE} NAME_WE)
		#message(SRC_FILE_NAME: ${SRC_FILE_NAME})
		if(SRC_FILE_NAME STREQUAL "main")
			set(MAKE_TYPE "bin")
		endif()
	endforeach()
endmacro()

# Results collection
macro(PIL_CHECK_DEPENDENCY LIBNAME)
	if(NOT ${LIBNAME}_INCLUDES)
		if(${LIBNAME}_INCLUDE_DIR)
			set(${LIBNAME}_INCLUDES ${${LIBNAME}_INCLUDE_DIR})
		endif(${LIBNAME}_INCLUDE_DIR)
	endif(NOT ${LIBNAME}_INCLUDES)

	if(NOT ${LIBNAME}_LIBRARIES)
		if(${LIBNAME}_LIBRARY)
			set(${LIBNAME}_LIBRARIES ${${LIBNAME}_LIBRARY})
		elseif(${LIBNAME}_LIBS)
			set(${LIBNAME}_LIBRARIES ${${LIBNAME}_LIBS})
		endif()
	endif(NOT ${LIBNAME}_LIBRARIES)

	string(TOUPPER ${LIBNAME} LIBNAME_UPPER)

	if(${LIBNAME}_FOUND})
		set(${LIBNAME_UPPER}_FOUND ${${LIBNAME}_FOUND})
	endif()

	if(${LIBNAME}_VERSION)
		set(${LIBNAME_UPPER}_VERSION ${${LIBNAME}_VERSION})
	endif()

	if(${LIBNAME}_INCLUDES)
		set(${LIBNAME_UPPER}_INCLUDES ${${LIBNAME}_INCLUDES})
	endif()

	if(${LIBNAME}_LIBRARIES)
		set(${LIBNAME_UPPER}_LIBRARIES ${${LIBNAME}_LIBRARIES})
	endif()
	
endmacro()

macro(PIL_ECHO_LIBINFO LIBNAME)
	PIL_CHECK_DEPENDENCY(${LIBNAME})
	message("----------------------------------------------------------")
	if(${LIBNAME_UPPER}_VERSION)
		message("--${LIBNAME_UPPER}(VERSION ${${LIBNAME_UPPER}_VERSION}):")
	else()
		message(--${LIBNAME_UPPER}:)
	endif()

	foreach(INCLUDES_IT ${${LIBNAME_UPPER}_INCLUDES})
		message("	" ${INCLUDES_IT})
	endforeach()

	foreach(LIB_IT ${${LIBNAME_UPPER}_LIBRARIES})
		message("	" ${LIB_IT})
	endforeach()

	#message("     " ${${LIBNAME}_INCLUDES})
	#message("     " ${${LIBNAME}_LIBRARIES})
endmacro()

macro(filtSOURCE_FILES_ALL DIR)
	foreach(SRC_FILE ${SOURCE_FILES_ALL})
		string(REGEX MATCH "${DIR}" SHOULDREMOVE ${SRC_FILE})
		if(SHOULDREMOVE)
			list(REMOVE_ITEM SOURCE_FILES_ALL ${SRC_FILE})
		endif()
	endforeach()
endmacro()

macro(reportTargets)
	get_property(LIBS2COMPILE GLOBAL PROPERTY LIBS2COMPILE)
	get_property(APPS2COMPILE GLOBAL PROPERTY APPS2COMPILE)

	message(STATUS "The following targets will to be build:")
	message(STATUS "LIBS(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}): " )
	foreach(IT ${LIBS2COMPILE})
		message("	" ${IT})
	endforeach()

	message(STATUS "APPS(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}): ")
	foreach(IT  ${APPS2COMPILE})
		message("	" ${IT})
	endforeach()
endmacro()

set(PICMAKE_UTILS_LOADED TRUE)
