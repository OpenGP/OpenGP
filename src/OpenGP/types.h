// Copyright (C) 2013 Graphics & Geometry Group, Bielefeld University
// Copyright (C) 2014 Andrea Tagliasacchi, EPFL
#ifndef SURFACE_MESH_TYPES_H
#define SURFACE_MESH_TYPES_H

//== INCLUDES =================================================================

#if defined(USE_EIGEN)
    #include <Eigen/Dense>
#else
    #include "surface_mesh/Vector.h"
#endif 

//=============================================================================
namespace opengp {
//=============================================================================

typedef float Scalar;               ///< Scalar type

#if defined(USE_EIGEN)
    /// 3D vector type
    typedef Eigen::Matrix<Scalar, 3, 1> Vec3; 
    #include "surface_mesh/Vector_back_compatibility.h"
#else
    typedef Vector<Scalar,3> Vec3;   /// 3D vector type    
    typedef Vec3 Point;              ///< Point type
    typedef Vec3 Normal;             ///< Normal type
    typedef Vec3 Color;              ///< Color type
    typedef Vec3 Texture_coordinate; ///< Texture coordinate type
#endif

//=============================================================================
} // namespace opengp
//=============================================================================

//=============================================================================
#endif // SURFACE_MESH_TYPES_H
//============================================================================
