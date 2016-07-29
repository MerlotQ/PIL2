#ifndef OPENGL_H
#define OPENGL_H

#include "base/Environment.h"

#if defined(PIL_OS_FAMILY_UNIX)
    #ifndef INCLUDED_GL_H
    #define INCLUDED_GL_H
    #include <GL/glew.h>
    //#include <GL/gl.h>
    #include <GL/glut.h>
    #endif
#elif defined(PIL_OS_FAMILY_WINDOWS)
    #include <GL/glew.h>
    //#include <GL/gl.h>
    #include "gui/GL_headers/glext.h"
#endif

#endif // OPENGL_H

