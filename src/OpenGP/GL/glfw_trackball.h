#pragma once
#include <Eigen/Dense>
#include <OpenGP/GL/Trackball.h>

namespace opengp{

/// @todo DOCUMENT
void glfwTrackball(void (*update_matrices)(Eigen::Matrix4f ));

namespace{
    static Eigen::Matrix4f scale_;
    static Eigen::Matrix4f rotation_;
    static Eigen::Matrix4f old_rotation_;
    static Eigen::Matrix4f translation_;
    static Trackball trackball_;
    static int width_;
    static int height_;
    static void (*_update_matrices)(Eigen::Matrix4f) = NULL;
        
    void update_matrices(){
        assert(_update_matrices!=NULL);
        Eigen::Matrix4f model = translation_ * rotation_ * scale_;
        _update_matrices(model);
    }
    
    void resize(int width, int height){
        width_ = width;
        height_ = height;
        glViewport(0, 0, width, height);
    }    
    
    void keyboard(int key, int action){
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_ESC:
                glfwCloseWindow();
                break;
            case GLFW_KEY_SPACE:
                break;
            }
        }
    }
    
    void mouse_button(int button, int action) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            old_rotation_ = rotation_;
            int x_i, y_i;
            glfwGetMousePos(&x_i, &y_i);
            const float x = 2.0f * static_cast<float>(x_i) / width_ - 1.0f;
            const float y = 1.0f - 2.0f * static_cast<float>(y_i) / height_;
            trackball_.BeginDrag(x, y);
        }
    }
 
    void mouse_move(int x, int y, int old_x, int old_y){
        bool view_matrices_changed = false;
 
        // Maya style controls.
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            const float x_f = 2.0f * static_cast<float>(x) / width_ - 1.0f;
            const float y_f = 1.0f - 2.0f * static_cast<float>(y) / height_;
            trackball_.Drag(x_f, y_f);
            rotation_ = trackball_.incremental_rotation() * old_rotation_;
            view_matrices_changed = true;
        }
 
        const float dx = x - old_x;
        const float dy = y - old_y;
 
        // Pan
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            const float scale = 0.05f;
            translation_ *= Eigen::Affine3f(Eigen::Translation3f(scale * dx, -scale * dy, 0.0f)).matrix();
            view_matrices_changed = true;
        }
 
        // Zoom
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            const float scale = 0.05f;
            translation_ *= Eigen::Affine3f(Eigen::Translation3f(0.0f, 0.0f, scale * dy)).matrix();
            view_matrices_changed = true;
        }
 
        if (view_matrices_changed) {
            update_matrices();
        }
    }
 
    void mouse_pos(int x, int y) {
        static int old_x = x;
        static int old_y = y;
        mouse_move(x, y, old_x, old_y);
        old_x = x;
        old_y = y;
    }
    
    /// @todo implement mouse movement
    static void mouse_wheel(int /*pos*/) {
        
    }
    
    void hook_trackball_callbacks( void (*update_matrices)(Eigen::Matrix4f ) ){
        /// Save the callback
        _update_matrices = update_matrices;
        assert(_update_matrices!=NULL);
                
        translation_ = Eigen::Matrix4f::Identity();
        rotation_ = Eigen::Matrix4f::Identity();
        old_rotation_ = Eigen::Matrix4f::Identity();
        scale_ = Eigen::Matrix4f::Identity();
        
        glfwSetKeyCallback(keyboard);
        glfwSetMouseButtonCallback(mouse_button);
        glfwSetMousePosCallback(mouse_pos);
        glfwSetMouseWheelCallback(mouse_wheel);
        glfwSetWindowSizeCallback(resize);        
    }
}

void glfwTrackball(void (*update_matrices)(Eigen::Matrix4f )){
    hook_trackball_callbacks(update_matrices);
}

} // opengp::
