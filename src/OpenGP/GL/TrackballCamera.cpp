#include "TrackballCamera.h"
#include <OpenGP/GL/Eigen.h> 
#include <OpenGP/MLogger.h>

void OpenGP::TrackballCamera::begin_rotate(OpenGP::Vec3 pos) {
    _current_pos = pos;
    project_onto_surface(_current_pos);
    _anchor_pos = _current_pos;
}

void OpenGP::TrackballCamera::finish_rotate() {
    _old_rotation = _rotation;
}

void OpenGP::TrackballCamera::rotate(Vec3 pos) {
    _current_pos= pos;
    project_onto_surface(_current_pos);
    
    Vec3 axis = _anchor_pos.cross(_current_pos);
    float angle = _angle_boost * atan2(axis.norm(), _anchor_pos.dot(_current_pos));
    
    if (angle > 0.0f) {
        axis.normalize();
        _rotation = Eigen::Affine3f(Eigen::AngleAxisf(angle, axis)).matrix();
        _rotation *= _old_rotation;
        
        update_matrices();
    }
}

void OpenGP::TrackballCamera::translate(OpenGP::Vec3 pos, OpenGP::Vec3 old_pos) {
    //TODO: Should probably check for more degenerate cases
    //TODO: Package lines and planes in structs?
    
    //Construct plane perpendicular to view direction and passing through trackball center
    Vec3 _camera_position = camera_position();
    Vec3 plane_normal = (_camera_position - _center).normalized();
    
    //Construct line from camera center to unprojected points
    Vec3 unprojected_pos = unproject(pos);
    Vec3 unprojected_old_pos = unproject(old_pos);
    
    Vec3 line_direction = unprojected_pos - _camera_position;
    Vec3 line_direction_old = unprojected_old_pos - _camera_position;
    
    Vec3 intersection_pos, intersection_old_pos;
    
    bool found_intersections = true;
    found_intersections &= plane_line_intersection(plane_normal, _center, line_direction, unprojected_pos, intersection_pos);
    found_intersections &= plane_line_intersection(plane_normal, _center, line_direction_old, unprojected_old_pos, intersection_old_pos);
    
    if (found_intersections) {
        Vec3 translation_pos = intersection_pos - intersection_old_pos;
        _translation *= Eigen::Affine3f(Eigen::Translation3f(translation_pos)).matrix();
    }
    
    update_matrices();
}

void OpenGP::TrackballCamera::scale(Scalar offset_y) {
    _translation_scale *= std::pow(1.2f, -offset_y);
    _translation(0,0) = _translation(1,1) = _translation(2,2) = _translation_scale;
    
    update_matrices();
}

void OpenGP::TrackballCamera::focus(OpenGP::Vec3 pos) {
    pos = unproject(pos);
    set_center(pos);
    
    //TODO: Really should be a better way of resetting these matrices
    _view_matrix = OpenGP::lookAt(_camera_position, _center, _camera_up);
    _translation = Mat4x4::Identity();
    _translation(0,0) = _translation(1,1) = _translation(2,2) = _translation_scale;
    
    update_matrices();
}

void OpenGP::TrackballCamera::adjust_fov(Scalar offset){
    /// TODO: this does not work like in meshlab, why?
    _fov += offset;
    /// Bound _fov [0, 90] (meshlab does the same)
    _fov = std::max<Scalar>(0, std::min<Scalar>(_fov, 90));
    _projection_matrix = OpenGP::perspective(_fov, _aspect_ratio, _near, _far);       
}

void OpenGP::TrackballCamera::screen_resize(int width, int height) {
    _aspect_ratio = (Scalar)width / (Scalar)height;
    _projection_matrix = OpenGP::perspective(_fov, _aspect_ratio, _near, _far);
}

void OpenGP::TrackballCamera::set_center(OpenGP::Vec3 center) {
    _center = center;
    _translate_center = Eigen::Affine3f(Eigen::Translation3f(_center)).matrix();
    _translate_minus_center = Eigen::Affine3f(Eigen::Translation3f(-center)).matrix();
}

OpenGP::TrackballCamera::TrackballCamera(Scalar aspect_ratio) {
    _aspect_ratio = aspect_ratio;
    set_center(Vec3(0, 0, 0));
    set_defaults();
}

void OpenGP::TrackballCamera::set_defaults() {
    //TODO: Make default parameters adjustable?
    _imodel_matrix = Mat4x4::Identity();
    
    _rotation = Mat4x4::Identity();
    _old_rotation = Mat4x4::Identity();
    _translation = Mat4x4::Identity();
    _translation_scale = 1.0f;
    
    //TODO: This should get resized
    _projection_matrix = OpenGP::perspective(_fov, _aspect_ratio, _near, _far);
    
    _camera_position = _camera_position_default;
    _camera_up = Vec3(0,1,0);
    _view_matrix = OpenGP::lookAt(_camera_position, _center, _camera_up);
    
    update_matrices();
//    _view_imodel_matrix = _view_matrix * _imodel_matrix;
}

void OpenGP::TrackballCamera::update_matrices() {
    _imodel_matrix =  _translate_center * _rotation * _translation * _translate_minus_center;
    _view_imodel_matrix = _view_matrix * _imodel_matrix;
}

//--------------------------------------------------------------------------------------------------

OpenGP::Vec3 OpenGP::TrackballCamera::camera_position() {
    // TODO: (Maybe) don't return by value?
    Vec4 result(0.0f, 0.0f, 0.0f, 1.0f);
    result = _view_imodel_matrix.inverse() * result;
    return Vec3(result(0), result(1), result(2));
}

OpenGP::Vec3 OpenGP::TrackballCamera::unproject(Vec3 pos) {
    Vec4 result(pos(0), pos(1), pos(2), 1.0f);
    result = (_projection_matrix * _view_imodel_matrix).inverse() * result;
    return Vec3(result(0), result(1), result(2)) / result(3);;
}

void OpenGP::TrackballCamera::project_onto_surface(OpenGP::Vec3 &pos) {
    const float radius2 = _radius * _radius;
    const float length2 = pos.squaredNorm();
    if (length2 <= radius2 * 0.5f) {
        pos[2] = std::sqrt(radius2 - length2);
    } else {
        pos[2] = radius2 / (2.0f * std::sqrt(length2));
    }
    float length = pos.norm();
    pos /= length;
}

//TODO: this function should really be somewhere else?
bool OpenGP::TrackballCamera::plane_line_intersection(const OpenGP::Vec3 plane_normal, const OpenGP::Vec3 plane_point, const OpenGP::Vec3 line_direction, const OpenGP::Vec3 line_point, OpenGP::Vec3 &intersection_point) {
    const float epsilon = 1e-8; //< TODO: how to make this safe?
    float k = plane_normal.dot(line_direction);
    if (k > -epsilon && k < epsilon)
        return false;
    
    float s = plane_normal.dot(plane_point - line_point) / k;
    intersection_point = line_point + s * line_direction;
    
    return true;
}
