// @see http://image.diku.dk/research/trackballs/index.html.
#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace OpenGP{

class Trackball {
public:
    Trackball() {
        radius_ = 1.0f;
    }
    explicit Trackball(float radius) : radius_(radius) {}

    const Eigen::Matrix4f& incremental_rotation() const {
        return incremental_rotation_;
    }

    void BeginDrag(float x, float y) {
        // TODO(stefalie): Is the identity loading required?
        incremental_rotation_ = Eigen::Matrix4f::Identity();

        anchor_pos_= Eigen::Vector3f(x, y, 0.0f);
        ProjectOntoSurface(anchor_pos_);

        current_pos_= anchor_pos_;
    }
    void Drag(float x, float y) {
        current_pos_= Eigen::Vector3f(x, y, 0.0f);
        ProjectOntoSurface(current_pos_);
        ComputeIncremental();
    }

private:
    void ProjectOntoSurface(Eigen::Vector3f& p) const {
        // We could make this static, but maybe we want to add methods for changing
        // the radius later on.
        const float radius2 = radius_ * radius_;

        const float length2 = p.squaredNorm();

        if (length2 <= radius2 * 0.5f) {
            p[2] = std::sqrt(radius2 - length2);
        } else {
            p[2] = radius2 / (2.0f * std::sqrt(length2));
        }
        float length = p.norm();
        p /= length;
    }

    void ComputeIncremental() {
        const float angle_boost = 10.0f;
        Eigen::Vector3f axis = anchor_pos_.cross(current_pos_);
        float angle = angle_boost * atan2(axis.norm(), anchor_pos_.dot(current_pos_));
        axis.normalize();
        incremental_rotation_ = Eigen::Affine3f(Eigen::AngleAxisf(angle, axis)).matrix();
    }

    float radius_;
    Eigen::Vector3f anchor_pos_;
    Eigen::Vector3f current_pos_;
    Eigen::Matrix4f incremental_rotation_;
};

} // OpenGP::
