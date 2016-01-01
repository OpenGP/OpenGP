#pragma once
#include <OpenGP/GL/glfw_helpers.h>
#include <OpenGP/GL/Scene.h>

//=============================================================================
namespace opengp {
//=============================================================================

// TODO: don't use the single-context glfw_helpers now that we use GLFW3!

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
        glfwInitWindowSize(_width, _height);
        if(glfwMakeWindow(title.c_str()) == EXIT_FAILURE)
            exit(EXIT_FAILURE);
        
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
        while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && !glfwWindowShouldClose(window)){
            scene.display();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        /// Closes OpenGL window and terminates GLFW
        glfwTerminate();
        return EXIT_SUCCESS;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
