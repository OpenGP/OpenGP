#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// Customizable (yet global) scalar type
/// WARNING: OpenGL behavior undefined if type is changed!
#ifdef OPENGP_SCALAR_TYPE
    typedef OPENGP_SCALAR_TYPE Scalar
#else
    typedef float Scalar;
#endif

typedef	unsigned char uchar;
typedef	unsigned int uint;

///--- Vectors
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> VecN; ///< nD vector type
typedef Eigen::Matrix<Scalar, 2, 1> Vec2;              ///< 2D vector type
typedef Eigen::Matrix<Scalar, 3, 1> Vec3;              ///< 3D vector type
typedef Eigen::Matrix<Scalar, 4, 1> Vec4;              ///< 4D vector type
typedef Vec3 Point;                                    ///< Point type
typedef Vec3 Normal;                                   ///< Normal type
typedef Vec3 Color;                                    ///< Color type
typedef Vec3 TextureCoordinate;                        ///< Texture coordinate type

///--- Matrices
typedef Eigen::Matrix<Scalar, 4, 4> Mat4x4; ///< 4x4 matrix 
typedef Eigen::Matrix<Scalar, 3, 3> Mat3x3; ///< 3x3 matrix
typedef Eigen::Matrix<Scalar, 3, Eigen::Dynamic> Mat3xN; ///< 3xN matrix
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> MatMxN; ///< MxN matrix

///--- Geometric types (Eigen/Geometry)
typedef Eigen::AlignedBox<Scalar,3> Box3; 

///--- Bindings for special values
inline Scalar nan(){ return std::numeric_limits<Scalar>::quiet_NaN(); }
inline bool isnan(Scalar v){ return std::isnan(v); }
inline Scalar inf(){ return std::numeric_limits<Scalar>::max(); } 

//=============================================================================
} // namespace OpenGP
//=============================================================================
