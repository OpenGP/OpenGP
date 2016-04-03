#pragma once
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/GL/TrackballCamera.h>
#include <Eigen/Dense>

#define GLFW_MOD_NONE 0x0000

//=============================================================================
namespace OpenGP {
//=============================================================================

class TrackballWindow : public OpenGP::GlfwWindow
{

public:
    TrackballWindow(const std::string& title, int width, int height) : GlfwWindow(title, width, height){ }

    ~TrackballWindow(){ }

    void mouse_press_callback(int button, int action, int mods) override 
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
        {      
            double x_window, y_window;
            glfwGetCursorPos(_window, &x_window, &y_window);

            Eigen::Vector3f pos_window(x_window, y_window, 0.0f);
            Eigen::Vector3f pos_clip;

            window_to_clip(pos_window, pos_clip);

            scene.trackball_camera.BeginRotate(pos_clip);           
        }

        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            scene.trackball_camera.FinishRotate();
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && mods == GLFW_MOD_NONE)
        {
            double x_window, y_window;
            glfwGetCursorPos(_window, &x_window, &y_window);

            float z_window;
            glReadBuffer(GL_FRONT);
            glReadPixels(int(x_window), _height - int(y_window), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_window);

            if(z_window != 1.0f)
            {
                Eigen::Vector3f pos_window(x_window, y_window, z_window);
                Eigen::Vector3f pos_clip;

                window_to_clip(pos_window, pos_clip);
                scene.trackball_camera.Focus(pos_clip);
            }
        }

        current_state_ = mods;
    }

    void mouse_move_callback(double x_window, double y_window) override 
    {
        bool left_down = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool middle_down = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

        Eigen::Vector3f pos_window(x_window, y_window, 0.0f);
        Eigen::Vector3f pos_clip;

        window_to_clip(pos_window, pos_clip);

        //Rotate
        if (left_down && current_state_ == GLFW_MOD_NONE)
        {
            scene.trackball_camera.Rotate(pos_clip);
        }

        //Pan
        if (middle_down || (left_down && current_state_ == GLFW_MOD_CONTROL))
        {
            scene.trackball_camera.Translate(pos_clip, old_pos_clip);    
        }

        //Scale
        if(left_down && (current_state_ == GLFW_MOD_SHIFT))
        {
            scene.trackball_camera.Scale(5.0f * (float)(pos_clip(1) - old_pos_clip(1)));
        }
  
        old_pos_clip = pos_clip;
    }

    void scroll_callback(double /*xOffset*/, double yOffset) override 
    {
        scene.trackball_camera.Scale((float)yOffset);
    }

    void framebuffer_size_callback(int width, int height) override
    {
        _width = width;
        _height = height;
        glViewport(0, 0, _width, _height);
        scene.trackball_camera.ResizeProjection(_width, _height);
    }

private:
    void window_to_clip(Eigen::Vector3f pos_window, Eigen::Vector3f &pos_clip)
    {
        pos_clip(0) = 2.0f * static_cast<float>(pos_window(0)) / _width - 1.0f;
        pos_clip(1) = 1.0f - 2.0f * static_cast<float>(pos_window(1)) / _height;
        pos_clip(2) = 2.0f * pos_window(2) - 1.0f;
    }

    int current_state_ = GLFW_MOD_NONE;
    Eigen::Vector3f old_pos_clip;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
