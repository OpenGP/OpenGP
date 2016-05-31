#pragma once
#include <Eigen/Dense>
#include <OpenGP/GL/Trackball.h>

#define GLFW_MOD_NONE 0
#warning "glfw_trackball is obsolete"

namespace OpenGP{

/// @todo DOCUMENT
void glfwTrackball(void (*update_matrices)(Eigen::Matrix4f), void (*update_projection_matrix)());

namespace{
    static Eigen::Matrix4f scale_;
    static Eigen::Matrix4f rotation_;
    static Eigen::Matrix4f old_rotation_;
    static Eigen::Matrix4f translation_;
    static Trackball trackball_;
    static void (*_update_matrices)(Eigen::Matrix4f) = NULL;
    static void (*_update_projection_matrix)() = NULL;
    
    int current_state_ = GLFW_MOD_NONE;

    void update_matrices() {
        assert(_update_matrices!=NULL);
        Eigen::Matrix4f model = translation_ * rotation_ * scale_;
        _update_matrices(model);
    }
    
    void resize(GLFWwindow */*window*/, int width, int height) {
        assert(_update_projection_matrix!=NULL);
        _width = width;
        _height = height;
        glViewport(0, 0, width, height);
        _update_projection_matrix();
        update_matrices();
    }
    
    void keyboard(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_SPACE:
                    break;
            }
        }
    }
    
    void mouse_button(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            old_rotation_ = rotation_;
            double x_i, y_i;
            glfwGetCursorPos(window, &x_i, &y_i);
            const float x = 2.0f * static_cast<float>(x_i) / _width - 1.0f;
            const float y = 1.0f - 2.0f * static_cast<float>(y_i) / _height;
            trackball_.BeginDrag(x, y);
        }

        current_state_ = mods;
    }
 
    void mouse_move(GLFWwindow *window, double x, double y, double old_x, double old_y) {
        bool view_matrices_changed = false;
 
        bool left_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool middle_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
        bool right_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        
        // Maya style controls.
        if ( left_down && current_state_ == GLFW_MOD_NONE ) {
            const float x_f = 2.0f * static_cast<float>(x) / _width - 1.0f;
            const float y_f = 1.0f - 2.0f * static_cast<float>(y) / _height;
            trackball_.Drag(x_f, y_f);
            rotation_ = trackball_.incremental_rotation() * old_rotation_;
            view_matrices_changed = true;
        }
 
        const float dx = x - old_x;
        const float dy = y - old_y;
 
        // Pan
        if ( middle_down || (left_down && (current_state_ == GLFW_MOD_SHIFT || current_state_ == GLFW_MOD_CONTROL)) ) {
            const float scale = 0.05f;
            translation_ *= Eigen::Affine3f(Eigen::Translation3f(scale * dx, -scale * dy, 0.0f)).matrix();
            view_matrices_changed = true;
        }
 
        // Zoom
        if ( right_down || (left_down && (current_state_ == GLFW_MOD_ALT || current_state_ == GLFW_MOD_SUPER)) ) {
            const float scale = 0.05f;
            translation_ *= Eigen::Affine3f(Eigen::Translation3f(0.0f, 0.0f, scale * dy)).matrix();
            view_matrices_changed = true;
        }
 
        if (view_matrices_changed) {
            update_matrices();
        }
    }

    void mouse_pos(GLFWwindow *window, double x, double y) {
        static double old_x = x;
        static double old_y = y;
        mouse_move(window, x, y, old_x, old_y);
        old_x = x;
        old_y = y;
    }

    static void mouse_wheel(GLFWwindow */*window*/, double dx, double dy) {
        const float scale = 0.05f;
        translation_ *= Eigen::Affine3f(Eigen::Translation3f(scale * dx, 0.0f, scale * dy)).matrix();
        update_matrices();
    }
    
    void hook_trackball_callbacks( void (*update_matrices)(Eigen::Matrix4f), void (*update_projection_matrix)() ){
        /// Save the callback
        _update_matrices = update_matrices;
        assert(_update_matrices!=NULL);

        _update_projection_matrix = update_projection_matrix;
        assert(_update_projection_matrix!=NULL);

        translation_ = Eigen::Matrix4f::Identity();
        rotation_ = Eigen::Matrix4f::Identity();
        old_rotation_ = Eigen::Matrix4f::Identity();
        scale_ = Eigen::Matrix4f::Identity();

        glfwSetKeyCallback(window, keyboard);
        glfwSetMouseButtonCallback(window, mouse_button);
        glfwSetCursorPosCallback(window, mouse_pos);
        glfwSetScrollCallback(window, mouse_wheel);
        glfwSetWindowSizeCallback(window, resize);
    }
}

void glfwTrackball(void (*update_matrices)(Eigen::Matrix4f), void (*update_projection_matrix)()){
    hook_trackball_callbacks(update_matrices, update_projection_matrix);
}

} // OpenGP::
