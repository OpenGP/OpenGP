#pragma once
#include <Eigen/Dense>
#include <OpenGP/headeronly.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/GL/TrackballCamera.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class TrackballWindow : public GlfwWindow
{
private:
    const int GLFW_DOUBLECLICK = (GLFW_REPEAT+1);
    const int GLFW_MOD_NONE = 0x0000;
    int current_state_ = GLFW_MOD_NONE;
    Vec3 old_pos_clip = Vec3::Zero();
    
public:
    TrackballWindow(const std::string& title, int width, int height) : GlfwWindow(title, width, height){ }
    HEADERONLY_INLINE void mouse_press_callback(int button, int action, int mods) override;
    HEADERONLY_INLINE void mouse_move_callback(double x_window, double y_window) override;
    HEADERONLY_INLINE void scroll_callback(double /*x_offset*/, double y_offset) override;
    HEADERONLY_INLINE void framebuffer_size_callback(int width, int height) override;

private:
    HEADERONLY_INLINE Vec3 window_to_clip(const Vec3& pos_window);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "TrackballWindow.cpp"
#endif
