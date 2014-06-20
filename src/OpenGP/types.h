// Copyright (C) 2013 Graphics & Geometry Group, Bielefeld University
// Copyright (C) 2014 Andrea Tagliasacchi, EPFL
#pragma once

#if defined(USE_EIGEN)
    #include <Eigen/Dense>
#else
    #include "surface_mesh/Vector.h"
#endif 

//=============================================================================
namespace opengp {
//=============================================================================

/// Customizable (yet global) scalar type
#ifdef OPENGP_SCALAR_TYPE
    typedef OPENGP_SCALAR_TYPE Scalar
#else
    typedef float Scalar;
#endif
 

#ifdef USE_EIGEN
    typedef Eigen::Matrix<Scalar, 2, 1> Vec2; ///< 2D vector type
    typedef Eigen::Matrix<Scalar, 3, 1> Vec3; ///< 3D vector type

    #ifdef SURFACE_MESH_VECTOR_BACK_COMPATIBILITY
        #include "surface_mesh/Vector_back_compatibility.h"
    #endif 
#else
    typedef Vector<Scalar,2> Vec2;   /// 2D vector type    
    typedef Vector<Scalar,3> Vec3;   /// 3D vector type    
    typedef Vec3 Point;              ///< Point type
    typedef Vec3 Normal;             ///< Normal type
    typedef Vec3 Color;              ///< Color type
    typedef Vec3 Texture_coordinate; ///< Texture coordinate type
#endif

//=============================================================================
} // namespace opengp
//=============================================================================
