#pragma once
#include <OpenGP/types.h>
#include <OpenGP/headeronly.h>
#include <Eigen/Geometry>

//=============================================================================
namespace OpenGP {
//=============================================================================

class TrackballCamera {
private:
    Scalar _angle_boost = 1.0;    
    float _radius = 1.0;
    float _translation_scale = 1.0;
    Vec3 _center;
    Vec3 _anchor_pos;
    Vec3 _current_pos;
    Vec3 _camera_position;
    Vec3 _camera_position_default = Vec3(0,0,2);
    Vec3 _camera_up;
    Mat4x4 _rotation, _old_rotation;
    Mat4x4 _translation, _translate_center, _translate_minus_center;

/// @{ core matrices
private:
    Mat4x4 _imodel_matrix = Mat4x4::Identity(); ///< internal model matrix
    Mat4x4 _view_matrix = Mat4x4::Identity();
    Mat4x4 _view_imodel_matrix = Mat4x4::Identity();
public:
    HEADERONLY_INLINE const Mat4x4& view_matrix() { return _view_imodel_matrix; }
/// @} 

/// @{ projection matrix setup
public:
    HEADERONLY_INLINE const Mat4x4& projection_matrix() { return _projection_matrix; }    
private:
    Mat4x4 _projection_matrix = Mat4x4::Identity();
    Scalar _fov = 45; ///< field of view
    Scalar _near = 0.1f; ///< near plane: assert(>0)
    Scalar _far = 10.0f; ///< far plane: assert(_far>_near)
    Scalar _aspect_ratio = 1.0; ///< WARNING: see screen_resize()
/// @}
    
/// @{ constructors
public:
    HEADERONLY_INLINE TrackballCamera(Scalar aspect_ratio=1.0);
    TrackballCamera(int width, int height) 
        : TrackballCamera((float)width/height){}
private:
    HEADERONLY_INLINE void set_defaults();
/// @}

public:
    HEADERONLY_INLINE void begin_rotate(Vec3 pos);
    HEADERONLY_INLINE void rotate(Vec3 pos);
    HEADERONLY_INLINE void finish_rotate();
    HEADERONLY_INLINE void translate(Vec3 pos, Vec3 old_pos);
    HEADERONLY_INLINE void scale(Scalar offset_y);
    HEADERONLY_INLINE void focus(Vec3 pos);
    HEADERONLY_INLINE void adjust_fov(Scalar offset);
    HEADERONLY_INLINE void set_center(Vec3 center);
    HEADERONLY_INLINE void screen_resize(int width, int height);

private:
    HEADERONLY_INLINE void update_matrices();
    HEADERONLY_INLINE Vec3 camera_position();
    HEADERONLY_INLINE Vec3 unproject(Vec3 pos);
    HEADERONLY_INLINE void project_onto_surface(Vec3& pos);
    HEADERONLY_INLINE bool plane_line_intersection(const Vec3 plane_normal, const Vec3 plane_point,
                                                   const Vec3 line_direction, const Vec3 line_point,
                                                   Vec3& intersection_point);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "TrackballCamera.cpp"
#endif
