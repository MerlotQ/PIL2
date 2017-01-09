#file(GLOB APPS2BUILD "*")

set(LIB_TYPE "shared")

pi_add_target(pi_base ${LIB_TYPE} base REQUIRED SYSTEM)
pi_add_target(pi_hardware ${LIB_TYPE} hardware DEPENDENCY pi_base)
pi_add_target(pi_network ${LIB_TYPE} network REQUIRED System DEPENDENCY pi_base)
pi_add_target(pi_cv ${LIB_TYPE} cv REQUIRED OPENCV DEPENDENCY pi_base)
pi_add_target(pi_gui ${LIB_TYPE} gui REQUIRED Qt OpenGL GLEW GLUT QGLViewer DEPENDENCY pi_base)
#foreach(APPNAME ${APPS2BUILD})
#	if(IS_DIRECTORY ${APPNAME} AND EXISTS ${APPNAME}/CMakeLists.txt)
#		add_subdirectory(${APPNAME})
#	endif()
#endforeach()
