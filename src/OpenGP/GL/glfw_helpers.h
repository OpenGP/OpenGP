#pragma once
#include <OpenGP/GL/gl.h>
#include <OpenGP/GL/glfw.h>
#include <OpenGP/GL/shader_helpers.h>

namespace opengp{

static int _width = 640;
static int _height = 480;
static void (*_display)(void) = NULL;

void glfwInitWindowSize(int width, int height){
    _width = width;
    _height = height;
}

int glfwCreateWindow(const char* title){
    // GLFW Initialization
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return EXIT_FAILURE;
    }    
    
    /// Hint GLFW that we would like an OpenGL 3 context (at least)
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); ///< anti-aliasing
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /// Attempt to open the window: fails if required version unavailable
    /// @note Intel GPUs do not support OpenGL 3.0
    if( !glfwOpenWindow(_width, _height, 0,0,0,0, 32,0, GLFW_WINDOW ) ){
        fprintf( stderr, "Failed to open OpenGL 3 GLFW window.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    /// Outputs the OpenGL version
    int major, minor, revision;
    glfwGetGLVersion(&major, &minor,&revision);
    std::cout << "Opened GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;
    
    // GLEW Initialization (must have a context)
    glewExperimental = true;
    if( glewInit() != GLEW_NO_ERROR ){
        fprintf( stderr, "Failed to initialize GLEW\n"); 
        return EXIT_FAILURE;
    }

    /// Wipe Startup Errors (Are they caused by GLEW?)
    while (glGetError() != GL_NO_ERROR) {}
    
    /// Set window title
    glfwSetWindowTitle(title);
    
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
    while(glfwGetKey(GLFW_KEY_ESC)!=GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED)){
        _display();
        glfwSwapBuffers();
    }
    
    /// Close OpenGL window and terminate GLFW
    glfwTerminate();
}

} // opengp::
