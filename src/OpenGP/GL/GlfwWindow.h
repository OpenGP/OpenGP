#pragma once
#include <map>
#include <list>
#include <OpenGP/GL/glfw.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/GlfwFpsCounter.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

inline void glfw_error_callback(int error, const char* description){
    mFatal("ERROR%d: %s", error, description);
}

// TODO: factor out methods in GlfwWindow.cpp
class GlfwWindow{    
/// @{
protected:
    GLFWwindow* _window = nullptr;
    int _width = 640;
    int _height = 480;
    bool _enable_wait_events = false; ///< glfw3 poll v.s. wait
    bool _enable_verbose_log = false;
public:
    SceneGraph scene;
/// @}

public:
    /// @brief Analogous to QApplication::exec()
    static int run(){ return Manager::run(); }
    
    /// @brief called in the main game look (once per frame)
    virtual void display(){ scene.display(); }
    
    /// @brief the ratio between framebuffer and window sizes
    /// (used to correct OpenGL behavior with retina displays)
    float scale_factor_retina(){
        int w_window, h_window;
        int w_framebuffer, h_framebuffer;
        glfwGetWindowSize(_window, &w_window, &h_window);
        glfwGetFramebufferSize(_window, &w_framebuffer, &h_framebuffer);
        int w_scale = w_framebuffer / w_window;
        int h_scale = h_framebuffer / h_window;
        assert(w_scale == h_scale);
        return (float) w_scale;
    }
    
    /// @brief retrieves the position of the cursor in the framebuffer
    /// (used to correct point unproject on retina displays)
    void getFramebufferCursorPos(double* x, double* y){
        double x_window, y_window;
        glfwGetCursorPos(_window, &x_window, &y_window);
        float s = scale_factor_retina();
        (*x) = x_window * s;
        (*y) = y_window * s;
    }
    
    /// @brief Initializes GLFW and creates a 3.1 context
    GlfwWindow(const std::string& title="", int width=640, int height=480){
        //--- GLFW initialization
        {
            // note: can be called multiple times without problems            
            // docs: "additional calls to glfwInit() after successful initialization but before 
            //        termination will return GL_TRUE immediately"
            if( !glfwInit() )
                mFatal() << "Failed to initialize GLFW";
            
            // Setup 
            glfwSetErrorCallback(glfw_error_callback);
            
            // Hint GLFW that we would like a shader-based OpenGL context
            // docs: "sets hints for the next call to glfwCreateWindow"
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
            if(_enable_verbose_log){
                int major = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MAJOR);
                int minor = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MINOR);
                int revision = glfwGetWindowAttrib(_window, GLFW_CONTEXT_REVISION);
                std::cout << "Opened GLFW OpenGL " << major << "." << minor << "." << revision << std::endl;
            }
            
            glfwMakeContextCurrent(_window);
            if(!glfwGetCurrentContext())
                mFatal() << "Failed to make GLFW context current.";
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // can be GLFW_CURSOR_HIDDEN
        
            // GLEW Initialization (must have created a context)
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if( err != GLEW_OK )
                mFatal() << "Failed GLEW initialization";
        
            /// Wipe Startup Errors (TODO: check who causes them? GLEW?)
            while (glGetError() != GL_NO_ERROR) {}            
        }
        
        //--- OpenGL globals
        {
            glClearColor(1.0f, 1.0f, 1.0f, 0.0f); ///< background
            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST); // Enable depth test
            glEnable(GL_MULTISAMPLE);
            // glDisable(GL_CULL_FACE); // back face triangle cull
            
            //--- Enable transparency (vec4 color)
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        //--- Hack (manual) for retina display
        glfwGetFramebufferSize(_window,&_width,&_height);
        scene.screen_resize(_width,_height);
        
        //--- Register the current window with the manager 
        Manager::bind(this);
    }
   
/// @{ Callbacks (@return true if the event has been managed)
    virtual bool mouse_press_callback(int /*button*/, int /*action*/, int /*mods*/) { return false; }
    virtual bool mouse_move_callback(double /*xPos*/, double /*yPos*/) { return false; }
    virtual bool scroll_callback(double /*xOffset*/, double /*yOffset*/) { return false; }
    virtual bool framebuffer_size_callback(int width, int height){
        _width = width;
        _height = height;
        scene.screen_resize(_width, _height);
        return true;
    }
    virtual bool key_callback(int key, int /*scancode*/, int action, int /*mods*/){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, GL_TRUE);
            return true;
        }
        return false;
    }
/// @}

class Manager{
    friend class GlfwWindow;  
    typedef std::pair<GLFWwindow*, GlfwWindow*> Pair;
    
protected:
    /// Utility to support FPS counting
    static GlfwFpsCounter* fps_counter(){
        static GlfwFpsCounter fps_counter;
        return &fps_counter;
    }
    
    /// @note workaround to allow initialization of static member
    /// TODO: verify this doesn't cause problems across compilation units
    static std::map<GLFWwindow*, GlfwWindow*>* active_windows(){
        static std::map<GLFWwindow*, GlfwWindow*> _active_windows;
        return &_active_windows;
    }

public:
    static void bind(GlfwWindow* window){
        GLFWwindow* _window = window->_window;
        active_windows()->emplace(_window, window);
        //--- connect the dispatcher       
        glfwSetKeyCallback(_window, glfw_key_callback);
        glfwSetMouseButtonCallback(_window, glfw_mouse_press_callback);
        glfwSetCursorPosCallback(_window, glfw_mouse_move_callback);
        glfwSetFramebufferSizeCallback(_window, glfw_framebuffer_size_callback);
        glfwSetScrollCallback(_window, glfw_scroll_callback);
    }
    static void unbind(GlfwWindow* window){
        GLFWwindow* _window = window->_window;
        active_windows()->erase(_window);
        glfwSetKeyCallback(_window, nullptr);
        glfwSetMouseButtonCallback(_window, nullptr);
        glfwSetCursorPosCallback(_window, nullptr);
        glfwSetFramebufferSizeCallback(_window, nullptr);
        glfwSetScrollCallback(_window, nullptr);
    }

/// @{ Main event loop
    static int run(){        
        while(true){        
            // FPS counter
            fps_counter()->tick();
            
            // Empty the event poll to make sure no event is called on deleted window
            bool _enable_wait_events = false; ///< glfw3 poll v.s. wait
            _enable_wait_events ? glfwWaitEvents() : glfwPollEvents();
            
            std::list<GlfwWindow*> todelete;
            for(Pair win: *active_windows()){
                if(glfwWindowShouldClose(win.first))
                    todelete.push_back(win.second);
                else{
                    glfwMakeContextCurrent(win.first);
                    win.second->display();
                    glfwSwapBuffers(win.first);
                }
            }
            
            // Now delete the window from the event subsystem
            for(GlfwWindow* win: todelete){
                Manager::unbind(win);
                glfwDestroyWindow(win->_window);
            }
                                
            // Should we terminate the event loop?
            if(active_windows()->empty()){
                glfwTerminate();
                return EXIT_SUCCESS;            
            }
        }   
    }
/// @}

/// @{ GLFW Event dispatching
private:     
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){ active_windows()->at(window)->key_callback(key, scancode, action, mods); }
    static void glfw_mouse_press_callback(GLFWwindow* window, int button, int action, int mods) { active_windows()->at(window)->mouse_press_callback(button, action, mods); }
    static void glfw_mouse_move_callback(GLFWwindow* window, double xPos, double yPos) { active_windows()->at(window)->mouse_move_callback(xPos, yPos); }
    static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) { active_windows()->at(window)->framebuffer_size_callback(width, height); }
    static void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) { active_windows()->at(window)->scroll_callback(xOffset, yOffset); }
/// @} 
};
    
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
