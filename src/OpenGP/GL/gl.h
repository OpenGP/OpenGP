#pragma once

///--- Load OpenGL here (GLEW is for cross platform)
#include <GL/glew.h> //< must be before glfw

///--- Linux needs extensions for framebuffers
#if __unix__
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
