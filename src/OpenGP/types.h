#pragma once
#include <Eigen/Dense>

//=============================================================================
namespace opengp {
//=============================================================================

/// Customizable (yet global) scalar type
#ifdef OPENGP_SCALAR_TYPE
    typedef OPENGP_SCALAR_TYPE Scalar
#else
    typedef float Scalar;
#endif

typedef Eigen::Matrix<Scalar, 2, 1> Vec2; ///< 2D vector type
typedef Eigen::Matrix<Scalar, 3, 1> Vec3; ///< 3D vector type
typedef Vec3 Point;              ///< Point type
typedef Vec3 Normal;             ///< Normal type
typedef Vec3 Color;              ///< Color type
typedef Vec3 Texture_coordinate; ///< Texture coordinate type

/// Bindings for special values
inline Scalar nan(){ return std::numeric_limits<Scalar>::quiet_NaN(); }
inline Scalar inf(){ return std::numeric_limits<Scalar>::max(); } 

//=============================================================================
} // namespace opengp
//=============================================================================
