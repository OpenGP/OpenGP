#pragma once
#include "GL/glew.h" ///< must be included before GLFW
#include "GLFW/glfw3.h"
#include "shader_helpers.h"

#if __cplusplus <= 199711L
#define nullptr NULL
#endif

/// Convenience constants
static const int ONE = 1;
static const bool DONT_NORMALIZE = false;
static const bool DONT_TRANSPOSE = false;
static const int ZERO_STRIDE = 0;
static const void* ZERO_BUFFER_OFFSET = 0;

namespace OpenGP{

static int _width = 640;
static int _height = 480;
static void (*_display)(void) = NULL;
static GLFWwindow *window = NULL;

void glfwInitWindowSize(int width, int height){
    _width = width;
    _height = height;
}

void error_callback(int /*error*/, const char* description)
{
    puts(description);
}

int glfwMakeWindow(const char* title){
    // GLFW Initialization
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);
    
    /// Hint GLFW that we would like an OpenGL 3 context (at least)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef __APPLE__ 
    // TODO test if we can use this on Windows and Linux (makes all calls below 3.2 obsolete)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    /// Attempt to open the window: fails if required version unavailable
    /// @note Intel GPUs do not support OpenGL 3.0
    if( !(window = glfwCreateWindow(_width, _height, title, nullptr, nullptr)) ) {
        fprintf( stderr, "Failed to open OpenGL 3 GLFW window.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    /// Outputs the OpenGL version
    int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    std::cout << "Opened GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;

    glfwMakeContextCurrent(window);
    if(!glfwGetCurrentContext()) {
        std::cerr << "Couldn't create OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // can be GLFW_CURSOR_HIDDEN

    // GLEW Initialization (must have a context)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if( err != GLEW_OK ){
        fprintf( stderr, "Failed to initialize GLEW\n");
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return EXIT_FAILURE;
    }

    /// Wipe Startup Errors (Are they caused by GLEW?)
    while (glGetError() != GL_NO_ERROR) {}
        
    return EXIT_SUCCESS;
}


/// @see glutDisplayFunc
void glfwDisplayFunc(void (*display)(void)){    
    _display = display;
}

/// @see glutMainLoop
void glfwMainLoop(){
    assert(_display!=NULL);
    
    /// Render loop & keyboard input
    while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && !glfwWindowShouldClose(window)){
        _display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    /// Close OpenGL window and terminate GLFW
    glfwTerminate();
}

} // OpenGP::
