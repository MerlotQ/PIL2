include(PICMake)
pi_add_target(pi_gui ${MAKE_TYPE} . REQUIRED QT OpenGL GLEW GLUT QGLViewer DEPENDENCY pi_base)

