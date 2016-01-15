#pragma once
#include <Eigen/Dense>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// Customizable (yet global) scalar type
#ifdef OPENGP_SCALAR_TYPE
    typedef OPENGP_SCALAR_TYPE Scalar
#else
    typedef float Scalar;
#endif

typedef	unsigned char uchar;
typedef	unsigned int uint;

typedef Eigen::Matrix<Scalar, 2, 1> Vec2; ///< 2D vector type
typedef Eigen::Matrix<Scalar, 3, 1> Vec3; ///< 3D vector type
typedef Vec3 Point;                       ///< Point type
typedef Vec3 Normal;                      ///< Normal type
typedef Vec3 Color;                       ///< Color type
typedef Vec3 TextureCoordinate;           ///< Texture coordinate type

///--- Matrices
typedef Eigen::Matrix<Scalar, 4, 4> Mat4x4; ///< 4x4 matrix 
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> MatMxN;


/// Bindings for special values
inline Scalar nan(){ return std::numeric_limits<Scalar>::quiet_NaN(); }
inline Scalar inf(){ return std::numeric_limits<Scalar>::max(); } 

//=============================================================================
} // namespace OpenGP
//=============================================================================
