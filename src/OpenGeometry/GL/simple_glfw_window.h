#pragma once
#include "GL/glew.h" ///< must be included before GLFW
#include "GL/glfw.h"
#include "./load_shaders.h"

/// Some constants used in OpenGL applications
#define ZERO_BUFFER_OFFSET ((void*) 0)
#define ZERO_STRIDE 0
#define NOT_NORMALIZED GL_FALSE

int simple_glfw_window(const char* title, int width, int height, void (*init)(void), void(*display)(void)){  
    // GLFW Initialization
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return EXIT_FAILURE;
    }    
    
    /// Hint GLFW that we would like an OpenGL 3 context (at least)
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /// Attempt to open the window: fails if required version unavailable
    /// @note Intel GPUs do not support OpenGL 3.0
    if( !glfwOpenWindow(width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) ){
        fprintf( stderr, "Failed to open OpenGL 3 GLFW window.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    /// Outputs the OpenGL version
    int major, minor, revision;
    glfwGetGLVersion(&major, &minor,&revision);
    std::cout << "GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;
    
    // GLEW Initialization (must have a context)

#ifdef __APPLE__
    /// @note OSX needs this why... URL
    glewExperimental = true;
#endif
    
    if( glewInit() != GLEW_NO_ERROR )
    {
        fprintf( stderr, "Failed to initialize GLEW\n"); 
        return EXIT_FAILURE;
    }
    
    /// Set window title
    glfwSetWindowTitle(title);
    
    /// Initialize our OpenGL program
    init();

    /// Render loop & keyboard input
    while(glfwGetKey(GLFW_KEY_ESC)!=GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED)){
        display();
        glfwSwapBuffers();
    }
    
    /// Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
