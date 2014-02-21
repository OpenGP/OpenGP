#ifndef OPENGP_TRACKBALL_H_
#define OPENGP_TRACKBALL_H_

#include <glm/glm.hpp>

// The following trackball class is heavily inspired by:
// http://image.diku.dk/research/trackballs/index.html.
class Trackball {
 public:
  Trackball(){ radius_(1.0f); }
  explicit Trackball(float radius) : radius_(radius){}

  const glm::mat4& incremental_rotation() const {
    return incremental_rotation_;
  }

  void BeginDrag(float x, float y)
  {
      // TODO(stefalie): Is the identity loading required?
      incremental_rotation_ = glm::mat4(1.0f);

      anchor_pos_= glm::vec3(x, y, 0.0f);
      ProjectOntoSurface(&anchor_pos_);

      current_pos_= anchor_pos_;
  }
  void Drag(float x, float y)
  {
      current_pos_= glm::vec3(x, y, 0.0f);
      ProjectOntoSurface(&current_pos_);
      ComputeIncremental();
  }

 private:
  void ProjectOntoSurface(glm::vec3* p) const{
      // We could make this static, but maybe we want to add methods for changing
      // the radius later on.
      const float radius2 = radius_ * radius_;

      const float length2 = p->x * p->x + p->y * p->y;

      if (length2 <= radius2 * 0.5f) {
        p->z = std::sqrt(radius2 - length2);
      } else {
        p->z = radius2 / (2.0f * std::sqrt(length2));
      }
      float length = std::sqrt(length2 + p->z * p->z);
      *p /= length;
  }

  void ComputeIncremental(){
      const float angle_boost = 80.0f;

      const glm::vec3 axis = glm::cross(anchor_pos_, current_pos_);
      float angle = angle_boost * atan2(glm::length(axis), glm::dot(anchor_pos_,
                                                                    current_pos_
                                                                        
      incremental_rotation_ = glm::rotate(angle, glm::normalize(axis));
  }

  float radius_;
  glm::vec3 anchor_pos_;
  glm::vec3 current_pos_;
  glm::mat4 incremental_rotation_;
};

#endif  // OPENGP_TRACKBALL_H_
