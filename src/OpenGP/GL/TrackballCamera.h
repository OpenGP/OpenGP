#pragma once
#include <OpenGP/types.h>
#include <Eigen/Geometry>
#include <Eigen/Dense>

//=============================================================================
namespace OpenGP {
//=============================================================================

class TrackballCamera {
private:
    const Scalar _angle_boost = 1.0;    
    float _radius = 1.0;
    float _scale = 1.0;
    Vec3 _center;
    Vec3 _anchor_pos;
    Vec3 _current_pos;
    Vec3 _camera_position;
    Vec3 _camera_up;
    Mat4x4 _rotation, _old_rotation;
    Mat4x4 _translation, _translate_center, _translate_minus_center;

/// @{ core matrices
private:
    Mat4x4 _model_matrix;
    Mat4x4 _view_matrix;
    Mat4x4 _view_model_matrix;
    Mat4x4 _projection_matrix;
public:
    const Mat4x4& view_model_matrix() { return _view_model_matrix; }
    const Mat4x4& projection_matrix() { return _projection_matrix; }    
/// @} 

/// @{ constructors
public:
    TrackballCamera();
    TrackballCamera(float radius, Vec3 center);
private:
    void set_defaults();
/// @}

public:
    void begin_rotate(Vec3 pos);
    void rotate(Vec3 pos);
    void finish_rotate();
    void translate(Vec3 pos, Vec3 old_pos);
    void scale(float offset_y);
    void focus(Vec3 pos);
    void set_center(Vec3 center);
    void resize_projection(int width, int height);

private:
    void update_matrices();
    Vec3 camera_position();
    Vec3 unproject(Vec3 pos);
    void project_onto_surface(Vec3& pos);
    bool plane_line_intersection(const Vec3 plane_normal, const Vec3 plane_point,
                               const Vec3 line_direction, const Vec3 line_point,
                               Vec3& intersection_point);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "TrackballCamera.cpp"
#endif
