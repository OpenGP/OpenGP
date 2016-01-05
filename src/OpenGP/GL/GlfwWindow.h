#pragma once
#include <map>
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
    bool _enable_wait_events = false; ///< glfw3 poll v.s. wait
    GLFWwindow* _window = nullptr;
    int _width = 640;
    int _height = 480;
    
public:
    SceneGraph scene;
    GlfwFpsCounter fps_counter;
/// @}
    
public:
    ~GlfwWindow(){ active_windows()->erase(_window); }
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
            if( !(_window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr)) )
                mFatal() << "Failed to open OpenGL 3 GLFW window.";
            
            /// Outputs the OpenGL version
            int major = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MAJOR);
            int minor = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MINOR);
            int revision = glfwGetWindowAttrib(_window, GLFW_CONTEXT_REVISION);
            std::cout << "Opened GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;
        
            glfwMakeContextCurrent(_window);
            if(!glfwGetCurrentContext())
                mFatal() << "Failed to make GLFW context current.";
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // can be GLFW_CURSOR_HIDDEN
        
            // GLEW Initialization (must have a context)
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if( err != GLEW_OK )
                mFatal() << "Failed GLEW initialization";
        
            /// Wipe Startup Errors (Are they caused by GLEW?)
            while (glGetError() != GL_NO_ERROR) {}            
        }
        
        ///--- Event Callbacks and Dispatcher
        {
            active_windows()->emplace(_window,this);
            glfwSetKeyCallback(_window, glfw_key_callback);
        }
        
        ///--- OpenGL globals
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
        while(glfwGetKey(_window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && !glfwWindowShouldClose(_window)){
            scene.display();
            glfwSwapBuffers(_window);
            _enable_wait_events ? glfwWaitEvents() : glfwPollEvents();
            fps_counter.tick();
        }
        
        /// Closes OpenGL window and terminates GLFW
        glfwTerminate();
        return EXIT_SUCCESS;
    }
    
/// @{ TODO: add more callbacks
    virtual void key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/){ 
        // mDebug() << key << scancode << action << mods; 
    }
/// @}

/// @{ GLFW Event dispatching
private:    
    /// @note workaround to allow initialization of static member
    /// TODO: verify this doesn't cause problems across compilation units
    static std::map<GLFWwindow*, GlfwWindow*>* active_windows(){
        static std::map<GLFWwindow*, GlfwWindow*> _active_windows;
        return &_active_windows;
    }    
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
        /// Asks the dispatcher which callback we should invoke
        active_windows()->at(window)->key_callback(key, scancode, action, mods);
    }
/// @} 
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
