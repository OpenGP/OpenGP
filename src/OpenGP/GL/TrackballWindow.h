#pragma once
#include <Eigen/Dense>
#include <OpenGP/headeronly.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/GL/TrackballCamera.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// A-la MeshLab like window with trackball controller.
/// 
/// Mouse controls:
/// - doubleclick(mouse_left):      set pivot to current 3D mouse position
/// - drag(mouse_left):             rotate (about pivot)
/// - drag(mouse_middle):           pan (translate on 2D projective plane)
/// - drag(mouse_left + super):     pan 
/// - wheel:                        zooming
/// - drag(mouse_left + shift):     zooming
/// - wheel+shift:                  change FoV (BUGGY!!!)
///
/// Keyboard controls:
/// - R:                            reset the camera 
/// 
class TrackballWindow : public GlfwWindow {
    typedef GlfwWindow Super;
private:
    const int GLFW_DOUBLECLICK = (GLFW_REPEAT+1);
    const int GLFW_MOD_NONE = 0x0000;
    int _mod_current = GLFW_MOD_NONE;
    Vec3 old_pos_clip = Vec3::Zero();
    
/// @{ simple parameters (modify at own risk)
public:
    Scalar scroll_multiplier = .25;
/// @} 

public:
    TrackballWindow(const std::string& title="glfw", int width=640, int height=480) : GlfwWindow(title, width, height) {}
    HEADERONLY_INLINE bool mouse_press_callback(int button, int action, int mods) override;
    HEADERONLY_INLINE bool mouse_move_callback(double x_window, double y_window) override;
    HEADERONLY_INLINE bool scroll_callback(double /*x_offset*/, double y_offset) override;
    HEADERONLY_INLINE bool framebuffer_size_callback(int width, int height) override;
    HEADERONLY_INLINE bool key_callback(int key, int scancode, int action, int mods) override;

private:
    HEADERONLY_INLINE Vec3 window_to_clip(const Vec3& pos_window);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
#include "TrackballWindow.cpp"
#endif
