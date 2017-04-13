cmake_minimum_required(VERSION 2.8.6)
project(PIL)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

# Build libs
if(NOT LIB_TYPE)
  set(LIB_TYPE "SHARED")
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3 -mtune=generic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -mtune=generic")
endif()

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
  set(PROJECT_VERSION ${PIL_VERSION_MAJOR}.${PIL_VERSION_MINOR}.${PIL_VERSION_PATCH})
  set(PROJECT_SOVERSION ${PIL_VERSION_MAJOR}.${PIL_VERSION_MINOR})
endmacro()
#####################################################################
PIL_FIND_VERSION()

message("----------------------------------------------------------")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/scripts/cmake")
include(PICMake)

pi_collect_packages(COLLECTED_PACKAGES VERBOSE MODULES OpenCV OpenGL GLEW GLUT Qt QGLViewer REQUIRED System)

set(MODULES SYSTEM  OpenGL Qt QGLViewer)


# Now do build things
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,.:./libs:../libs:${CMAKE_INSTALL_PREFIX}/lib -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-rpath,.:./libs:../libs:${CMAKE_INSTALL_PREFIX}/lib -fPIC")


include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

#add_subdirectory(src)
pi_add_target(pi_base ${LIB_TYPE} src/base REQUIRED SYSTEM)
pi_add_target(pi_hardware ${LIB_TYPE} src/hardware REQUIRED pi_base)
pi_add_target(pi_network ${LIB_TYPE} src/network REQUIRED System pi_base)
pi_add_target(pi_cv ${LIB_TYPE} src/cv REQUIRED OPENCV pi_base)
pi_add_target(pi_gui ${LIB_TYPE} src/gui REQUIRED Qt OpenGL GLEW GLUT QGLViewer pi_base)


pi_add_target(TestLibrary SHARED apps/ClassLoaderTest/TestLibrary.cpp REQUIRED pi_base)

# Build apps
#add_subdirectory(apps)
pi_add_target(Tests BIN apps/Tests REQUIRED pi_base pi_network MODULES pi_cv pi_gui pi_hardware)

pi_add_target(SvarTest BIN apps/SvarTest REQUIRED pi_base)
pi_add_target(TimerTest BIN apps/TimerTest REQUIRED pi_base)
pi_add_target(ClassLoaderTest BIN apps/ClassLoaderTest/TestPlugin.cpp REQUIRED pi_base)
pi_add_target(CameraTest BIN apps/CameraTest REQUIRED pi_base pi_cv)
pi_add_target(GUI_Test BIN apps/GUI_Test REQUIRED QT QGLVIEWER OPENGL GLEW GLUT pi_base pi_gui)
message("----------------------------------------------------------")
pi_report_target(LIBS2COMPILE APPS2COMPILE)

#####################################################################
message("----------------------------------------------------------")

pi_install(HEADERS src/base src/hardware src/network src/cv src/gui HEADER_DESTINATION include/pil
           TARGETS pi_base pi_hardware pi_network pi_cv pi_gui
           CMAKE "${PROJECT_SOURCE_DIR}/scripts/cmake/FindPIL.cmake.in")

