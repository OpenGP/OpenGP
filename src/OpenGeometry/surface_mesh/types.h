//=============================================================================
// Copyright (C) 2013 Graphics & Geometry Group, Bielefeld University
// Copyright (C) 2014 Andrea Tagliasacchi, EPFL
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation, version 2.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef SURFACE_MESH_TYPES_H
#define SURFACE_MESH_TYPES_H


//== INCLUDES =================================================================

#if defined(USE_EIGEN)
    #include <Eigen/Dense>
    #include "Surface_mesh_Vector_back_compatibility.h"
#else
    #include "Vector.h"
#endif 

//=============================================================================

namespace surface_mesh {

//=============================================================================

typedef float Scalar;               ///< Scalar type
#ifndef USE_EIGEN
    ///< 3D vector type
    typedef Vector<Scalar,3> Vec3;      
#else
    ///< 3D vector type
    typedef Eigen::Matrix<Scalar, 3, 1> Vec3; 
#endif
typedef Vec3 Point;                     ///< Point type
typedef Vec3 Normal;                    ///< Normal type
typedef Vec3 Color;                     ///< Color type
typedef Vec3 Texture_coordinate;        ///< Texture coordinate type

//=============================================================================
} // namespace surface_mesh



//=============================================================================
#endif // SURFACE_MESH_TYPES_H
//============================================================================
