#pragma once
#include <OpenGP/GL/glfw_helpers.h>
#include <OpenGP/GL/Scene.h>

//=============================================================================
namespace opengp {
//=============================================================================

class GlfwWindow{
public:
    Scene scene;    
    int width = 640;
    int height = 480;
    
public:
    GlfwWindow(std::string title, int width, int height){
        this->width = width;
        this->height = height;
        glfwInitWindowSize(width, height);
        glfwCreateWindow(title.c_str());
        
        ///---------------------- OPENGL GLOBALS--------------------
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

public:
    int run(){
        while(glfwGetKey(GLFW_KEY_ESC)!=GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED)){
            scene.display();
            glfwSwapBuffers();
        }
        
        /// Closes OpenGL window and terminates GLFW
        glfwTerminate();
        return EXIT_SUCCESS;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
