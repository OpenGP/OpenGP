#pragma once
#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP{
//=============================================================================

typedef Eigen::Matrix<Scalar, 3, Eigen::Dynamic> VerticesMatrix;
typedef Eigen::Map<VerticesMatrix> VerticesMatrixMap;
typedef Eigen::Matrix<int, 3, Eigen::Dynamic> FacesMatrix;

FacesMatrix faces_matrix(SurfaceMesh& mesh){
    /// @todo check there is no garbage
    
    /// mesh must be a triangulation
    assert(mesh.is_triangle_mesh());
    
    FacesMatrix faces;
    faces.resize(3,mesh.n_faces());
    for(SurfaceMesh::Face f: mesh.faces()){
        int icntr = 0;
        for(SurfaceMesh::Vertex v: mesh.vertices(f))
            faces(icntr++,f.idx()) = v.idx();
    }
    return faces;
}

VerticesMatrixMap vertices_matrix(SurfaceMesh& mesh){
    auto _vertices = mesh.vertex_property<Vec3>("v:point");
    return VerticesMatrixMap((Scalar *)(_vertices.data()), 3, mesh.n_vertices());
} 

/// @todo 
/// VerticesMatrix vertices_matrix(const SurfaceMesh& mesh){}

//=============================================================================
} // OpenGP::
//=============================================================================
