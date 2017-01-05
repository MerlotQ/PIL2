cmake_minimum_required(VERSION 2.8.6)
project(PIL)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)#Release
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -march=native")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
#####################################################################
message("----------------------------------------------------------")
set(MODULES SYSTEM OpenCV OpenGL Qt QGLViewer)


set(TARGET_NAME NO_TARGET)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/PICMake)
include(PICMake)

# Now do build things
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,.:./libs:../libs")

# Build libs

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(src)
add_subdirectory(apps)
message("----------------------------------------------------------")
reportTargets()

#####################################################################
message("----------------------------------------------------------")
macro(PIL_FIND_VERSION)
  file(READ "${PROJECT_SOURCE_DIR}/src/base/PIL_VERSION.h" _pil_version_header)

  string(REGEX MATCH "define[ \t]+PIL_VERSION_MAJOR[ \t]+([0-9]+)" _pil_major_version_match "${_pil_version_header}")
  set(PIL_VERSION_MAJOR "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+PIL_VERSION_MINOR[ \t]+([0-9]+)" _pil_minor_version_match "${_pil_version_header}")
  set(PIL_VERSION_MINOR "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+PIL_VERSION_PATCH[ \t]+([0-9]+)" _pil_patch_version_match "${_pil_version_header}")
  set(PIL_VERSION_PATCH "${CMAKE_MATCH_1}")
	
  if(NOT PIL_VERSION_MAJOR)
    set(PIL_VERSION_MAJOR 1)
  endif(NOT PIL_VERSION_MAJOR)
  if(NOT PIL_VERSION_MINOR)
    set(PIL_VERSION_MINOR 1)
  endif(NOT PIL_VERSION_MINOR)
  if(NOT PIL_VERSION_PATCH)
    set(PIL_VERSION_PATCH 0)
  endif(NOT PIL_VERSION_PATCH)
	

  set(PIL_VERSION ${PIL_VERSION_MAJOR}.${PIL_VERSION_MINOR}.${PIL_VERSION_PATCH})
endmacro()

if(NOT CMAKE_INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()

PIL_FIND_VERSION()
set(CMAKE_INSTALL_PIL_ROOT "${CMAKE_INSTALL_PREFIX}/PIL-${PIL_VERSION}")
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/scripts/FindPIL.cmake.in" "${PROJECT_BINARY_DIR}/FindPIL.cmake" @ONLY)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_PIL_ROOT}/pil FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/libs/ DESTINATION ${CMAKE_INSTALL_PIL_ROOT}/lib)
install(FILES "${PROJECT_BINARY_DIR}/FindPIL.cmake" DESTINATION ${CMAKE_ROOT}/Modules)


