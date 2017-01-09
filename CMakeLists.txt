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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/scripts/cmake")
include(PICMake)

pi_collect_packages(COLLECTED_PACKAGES VERBOSE MODULES OpenCV OpenGL GLEW GLUT Qt QGLViewer REQUIRED System)

set(MODULES SYSTEM  OpenGL Qt QGLViewer)


# Now do build things
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,.:./libs:../libs")


include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

# Build libs
set(LIB_TYPE "shared")
#add_subdirectory(src)
pi_add_target(pi_base ${LIB_TYPE} src/base REQUIRED SYSTEM)
pi_add_target(pi_hardware ${LIB_TYPE} src/hardware DEPENDENCY pi_base)
pi_add_target(pi_network ${LIB_TYPE} src/network REQUIRED System DEPENDENCY pi_base)
pi_add_target(pi_cv ${LIB_TYPE} src/cv REQUIRED OPENCV DEPENDENCY pi_base)
pi_add_target(pi_gui ${LIB_TYPE} src/gui REQUIRED Qt OpenGL GLEW GLUT QGLViewer DEPENDENCY pi_base)


pi_add_target(TestLibrary SHARED apps/ClassLoaderTest/TestLibrary.cpp REQUIRED pi_base)

# Build apps
#add_subdirectory(apps)
pi_add_target(Tests BIN apps/Tests REQUIRED pi_base pi_network MODULES pi_cv pi_gui pi_hardware)

pi_add_target(SvarTest BIN apps/SvarTest DEPENDENCY pi_base)
pi_add_target(TimerTest BIN apps/TimerTest DEPENDENCY pi_base)
pi_add_target(ClassLoaderTest BIN apps/ClassLoaderTest/TestPlugin.cpp DEPENDENCY pi_base)
pi_add_target(CameraTest BIN apps/CameraTest DEPENDENCY pi_base pi_cv)
pi_add_target(GUI_Test BIN apps/GUI_Test REQUIRED QT QGLVIEWER OPENGL GLEW GLUT DEPENDENCY pi_base pi_gui)
message("----------------------------------------------------------")
pi_report_target()

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


