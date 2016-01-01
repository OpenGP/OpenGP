#pragma once
#include <OpenGP/GL/glfw.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/GlfwFpsCounter.h>

//=============================================================================
namespace opengp {
//=============================================================================

void glfw_error_callback(int error, const char* description){
    mFatal("ERROR%d: %s", error, description);
}

class GlfwWindow{
/// @{
private:
    GLFWwindow *window = nullptr;
    int _width = 640;
    int _height = 480;
    
public:
    SceneGraph scene;
    GlfwFpsCounter fps_counter;
/// @}  
   
public:
    GlfwWindow(std::string title, int width, int height){
        this->_width = width;
        this->_height = height;
        
        {
            if( !glfwInit() )
                mFatal() << "Failed to initialize GLFW";
            
            glfwSetErrorCallback(glfw_error_callback);
            
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
            if( !(window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr)) )
                mFatal() << "Failed to open OpenGL 3 GLFW window.";
            
            /// Outputs the OpenGL version
            int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
            int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
            int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
            std::cout << "Opened GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;
        
            glfwMakeContextCurrent(window);
            if(!glfwGetCurrentContext())
                mFatal() << "Failed to make GLFW context current.";
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // can be GLFW_CURSOR_HIDDEN
        
            // GLEW Initialization (must have a context)
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if( err != GLEW_OK )
                mFatal() << "Failed GLEW initialization";
        
            /// Wipe Startup Errors (Are they caused by GLEW?)
            while (glGetError() != GL_NO_ERROR) {}            
        }
        
        ///--- OPENGL GLOBALS
        {
            glClearColor(1.0f, 1.0f, 1.0f, 0.0f); ///< background
            glEnable(GL_DEPTH_TEST); // Enable depth test
            glEnable(GL_MULTISAMPLE);
            // glDisable(GL_CULL_FACE); // back face triangle cull
            
            ///--- Enable transparency (vec4 color)
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    int run(){
        while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && !glfwWindowShouldClose(window)){
            scene.display();
            glfwSwapBuffers(window);
            glfwPollEvents();
            fps_counter.tick();
        }
        
        /// Closes OpenGL window and terminates GLFW
        glfwTerminate();
        return EXIT_SUCCESS;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
