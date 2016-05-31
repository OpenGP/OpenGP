#include "TrackballWindow.h"
#include <chrono> ///< for double-click

bool OpenGP::TrackballWindow::mouse_press_callback(int button, int action, int mods) {
    if( action == GLFW_RELEASE ){
        static auto before = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::now();
        double diff_ms = std::chrono::duration <double, std::milli> (now - before).count();
        before = now;
        if(diff_ms>10 && diff_ms<200){
            action = GLFW_DOUBLECLICK;
            // mDebug() << "doubleclick";
        }
    }   
    
    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
        double x_window, y_window;
        getFramebufferCursorPos(&x_window, &y_window);
        Vec3 pos_window(x_window, y_window, 0.0f);
        Vec3 pos_clip = window_to_clip(pos_window);
        scene.trackball_camera.begin_rotate(pos_clip);
        return true;
    }

    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE)) {
        scene.trackball_camera.finish_rotate();
        return true;
    }

    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_DOUBLECLICK) && (mods == GLFW_MOD_NONE)) {
        double x_window, y_window;
        getFramebufferCursorPos(&x_window,&y_window);

        /// Fetch the depth by querying the OpenGL depth buffer
        float z_window = 1.0f;
        glReadBuffer(GL_FRONT);
        glReadPixels(int(x_window), _height - int(y_window), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_window);

        /// If we clicked on something visible focus the trackball (center it) on the current point
        if (z_window != 1.0f) {
            Vec3 pos_window(x_window, y_window, z_window);
            Vec3 pos_clip = window_to_clip(pos_window);
            scene.trackball_camera.focus(pos_clip);
        }
        return true;
    }
    
    return false;
}

bool OpenGP::TrackballWindow::mouse_move_callback(double x_window, double y_window) {
    x_window *= scale_factor_retina();
    y_window *= scale_factor_retina();
    // mLogger() << x_window << y_window;
    
    bool left_down = (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    bool middle_down = (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    Vec3 pos_window(x_window, y_window, 0.0f);
    Vec3 pos_clip = window_to_clip(pos_window);

    bool managed = false;
    
    // Rotate
    if (left_down && _mod_current == GLFW_MOD_NONE) {
        scene.trackball_camera.rotate(pos_clip);
        managed = true;
    }

    // Pan
    if (middle_down || (left_down && _mod_current == GLFW_MOD_SUPER)) {
        scene.trackball_camera.translate(pos_clip, old_pos_clip);
        managed = true;
    }

    // Scale
    if (left_down && (_mod_current == GLFW_MOD_SHIFT)) {
        scene.trackball_camera.scale(5.0f * (float)(pos_clip(1) - old_pos_clip(1)));
        managed = true;
    }

    old_pos_clip = pos_clip;
    return managed;
}

bool OpenGP::TrackballWindow::scroll_callback(double, double y_offset) {
    if (_mod_current == GLFW_MOD_NONE) {
        scene.trackball_camera.scale(scroll_multiplier * (double)y_offset);
        return true;
    }
    
    /// BUG: when button is pressed y_offset just gives 0!
    if (_mod_current == GLFW_MOD_SHIFT) {
        scene.trackball_camera.adjust_fov(y_offset);
        return true;
    }
    
    return false;
}

bool OpenGP::TrackballWindow::framebuffer_size_callback(int width, int height) {
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height);
    scene.trackball_camera.screen_resize(_width, _height);
    return true;
}

bool OpenGP::TrackballWindow::key_callback(int key, int scancode, int action, int mods){
    if( Super::key_callback(key, scancode, action, mods) )
        return true;
        
    if(action == GLFW_PRESS) 
        _mod_current = mods;
    if(action == GLFW_RELEASE && (_mod_current!=GLFW_MOD_NONE)) 
        _mod_current = GLFW_MOD_NONE;
    
    /// Reset the camera
    /// TODO: check meshlab bindings
    if(key=='R'){
        scene.trackball_camera = TrackballCamera(_width, _height);
        return true;   
    }
    return false;
}

OpenGP::Vec3 OpenGP::TrackballWindow::window_to_clip(const Vec3& pos_window) {
    Vec3 retval;
    retval(0) = 2.0f * static_cast<float>(pos_window(0)) / _width - 1.0f;
    retval(1) = 1.0f - 2.0f * static_cast<float>(pos_window(1)) / _height;
    retval(2) = 2.0f * pos_window(2) - 1.0f;
    return retval;
}
