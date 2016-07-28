set(CMAKE_BASE_FROM_DIR ${CMAKE_CURRENT_LIST_DIR})
set(MAKE_TYPE "shared")

get_filename_component(TARGET_NAME ${CMAKE_BASE_FROM_DIR} NAME)
string(REPLACE " " "_" TARGET_NAME ${TARGET_NAME})
set(TARGET_NAME pi_${TARGET_NAME})

list(APPEND COMPILEFLAGS ${CMAKE_CURRENT_LIST_DIR}/..)

if(true)
	list(APPEND LINKFLAGS pthread dl)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../scripts/CMakeListBase.cmake)


