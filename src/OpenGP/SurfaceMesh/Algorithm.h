#pragma once
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// Provides shortened access to all names of Suface_mesh;
class SurfaceMeshAlgorithm{
protected:
    template <class T> using VertexProperty = SurfaceMesh::Vertex_property<T>;
    template <class T> using EdgeProperty = SurfaceMesh::Edge_property<T>;
    template <class T> using FaceProperty = SurfaceMesh::Face_property<T>;
    typedef SurfaceMesh::Vertex_iterator Vertex_iterator;
    typedef SurfaceMesh::Edge_iterator Edge_iterator;
    typedef SurfaceMesh::Face_iterator Face_iterator;
    typedef SurfaceMesh::Vertex_around_vertex_circulator Vertex_around_vertex_circulator;
    typedef SurfaceMesh::Halfedge_around_vertex_circulator Halfedge_around_vertex_circulator;
    typedef ::OpenGP::SurfaceMesh SurfaceMesh;
    typedef ::OpenGP::Point Point;
    typedef ::OpenGP::Normal Normal;
    typedef ::OpenGP::Scalar Scalar;
    typedef SurfaceMesh::Vertex Vertex;
    typedef SurfaceMesh::Face Face;
    typedef SurfaceMesh::Edge Edge;
    typedef SurfaceMesh::Halfedge Halfedge;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
