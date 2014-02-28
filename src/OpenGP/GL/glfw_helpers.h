// Copyright (C) 2014 - Andrea Tagliasacchi

#pragma once
#include "GL/glew.h" ///< must be included before GLFW
#include "GL/glfw.h"
#include "shader_helpers.h"

namespace opengp{

/// Some constants used in OpenGL applications
#define ZERO_BUFFER_OFFSET ((void*) 0)
#define ZERO_STRIDE 0
#define NOT_NORMALIZED GL_FALSE

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
