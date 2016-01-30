#pragma once
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP{
//=============================================================================

typedef Eigen::Matrix<Scalar, 3, Eigen::Dynamic> VerticesMatrix;
typedef Eigen::Map<VerticesMatrix> VerticesMatrixMap;

typedef Eigen::Matrix<Scalar, 3, Eigen::Dynamic> NormalsMatrix;
typedef Eigen::Map<NormalsMatrix> NormalsMatrixMap;

typedef Eigen::Matrix<int, 3, Eigen::Dynamic> TrianglesMatrix;

inline TrianglesMatrix faces_matrix(SurfaceMesh& mesh){
    /// TODO check there is no garbage
    CHECK(mesh.is_triangle_mesh());
    
    /// mesh must be a triangulation
    assert(mesh.is_triangle_mesh());
    
    TrianglesMatrix faces;
    faces.resize(3,mesh.n_faces());
    for(SurfaceMesh::Face f: mesh.faces()){
        int icntr = 0;
        for(SurfaceMesh::Vertex v: mesh.vertices(f))
            faces(icntr++,f.idx()) = v.idx();
    }
    return faces;
}

inline VerticesMatrixMap vertices_matrix(SurfaceMesh& mesh){
    auto _vpoints = mesh.vertex_property<Vec3>("v:point");
    return VerticesMatrixMap((Scalar *)(_vpoints.data()), 3, mesh.n_vertices());
} 

inline NormalsMatrixMap normals_matrix(SurfaceMesh& mesh){
    auto _vnormals = mesh.vertex_property<Vec3>("v:normal");
    return NormalsMatrixMap((Scalar*)(_vnormals.data()), 3, mesh.n_vertices());
}

/// @todo const version
/// VerticesMatrix vertices_matrix(const SurfaceMesh& mesh){}

//=============================================================================
} // OpenGP::
//=============================================================================
