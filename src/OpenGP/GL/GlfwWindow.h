#pragma once
#include <map>
#include <OpenGP/GL/glfw.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/GlfwFpsCounter.h>
#include <OpenGP/GL/ArcballCamera.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

void glfw_error_callback(int error, const char* description){
    mFatal("ERROR%d: %s", error, description);
}

// TODO: factor out methods in GlfwWindow.cpp
class GlfwWindow{
/// @{
protected:
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
    GlfwWindow(const std::string& title, int width, int height){
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
            if( !(_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) )
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
            glfwSetMouseButtonCallback(_window, glfw_mouse_press_callback);
            glfwSetCursorPosCallback(_window, glfw_mouse_move_callback);
            glfwSetFramebufferSizeCallback(_window, glfw_framebuffer_size_callback);
            glfwSetScrollCallback(_window, glfw_scroll_callback);

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

        ///--- Hack (manual) for retina display
        glfwGetFramebufferSize(_window,&_width,&_height);
        scene.screen_resize(_width,_height);       
    }

    virtual int run(){
        while(!glfwWindowShouldClose(_window)){
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
    virtual void key_callback(int key, int /*scancode*/, int action, int /*mods*/){ 
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, GL_TRUE);
        }
    }
    virtual void mouse_press_callback(int /*button*/, int /*action*/, int /*mods*/) {}
    virtual void mouse_move_callback(double /*xPos*/, double /*yPos*/) {}
    virtual void scroll_callback(double /*xOffset*/, double /*yOffset*/) {}    
    virtual void framebuffer_size_callback(int width, int height){
        _width = width;
        _height = height;
        scene.screen_resize(_width, _height);
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

    static void glfw_mouse_press_callback(GLFWwindow* window, int button, int action, int mods) {
        active_windows()->at(window)->mouse_press_callback(button, action, mods);
    }

    static void glfw_mouse_move_callback(GLFWwindow* window, double xPos, double yPos) {
        active_windows()->at(window)->mouse_move_callback(xPos, yPos);
    }

    static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        active_windows()->at(window)->framebuffer_size_callback(width, height);
    }

    static void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
        active_windows()->at(window)->scroll_callback(xOffset, yOffset);
    }
/// @} 
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
