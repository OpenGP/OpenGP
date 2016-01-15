//== INCLUDES =================================================================


#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/IO/IO.h>
#include <cstdio>


//== NAMESPACES ===============================================================


namespace OpenGP {


//== IMPLEMENTATION ===========================================================


//-----------------------------------------------------------------------------


bool read_poly(SurfaceMesh& mesh, const std::string& filename)
{
    unsigned int n_items;

    // open file (in binary mode)
    FILE* in = fopen(filename.c_str(), "rb");
    if (!in) return false;


    // clear mesh
    mesh.clear();


    // how many elements?
    unsigned int nv, ne, nh, nf;
    read(in, nv);
    read(in, ne);
    read(in, nf);
    nh = 2*ne;


    // resize containers
    mesh.vprops_.resize(nv);
    mesh.hprops_.resize(nh);
    mesh.eprops_.resize(ne);
    mesh.fprops_.resize(nf);


    // get properties
    SurfaceMesh::Vertex_property<SurfaceMesh::Vertex_connectivity>      vconn = mesh.vertex_property<SurfaceMesh::Vertex_connectivity>("v:connectivity");
    SurfaceMesh::Halfedge_property<SurfaceMesh::Halfedge_connectivity>  hconn = mesh.halfedge_property<SurfaceMesh::Halfedge_connectivity>("h:connectivity");
    SurfaceMesh::Face_property<SurfaceMesh::Face_connectivity>          fconn = mesh.face_property<SurfaceMesh::Face_connectivity>("f:connectivity");
    SurfaceMesh::Vertex_property<Vec3>                                  point = mesh.vertex_property<Vec3>("v:point");

    // read properties from file
    n_items = fread((char*)vconn.data(), sizeof(SurfaceMesh::Vertex_connectivity),   nv, in);
    n_items = fread((char*)hconn.data(), sizeof(SurfaceMesh::Halfedge_connectivity), nh, in);
    n_items = fread((char*)fconn.data(), sizeof(SurfaceMesh::Face_connectivity),     nf, in);
    n_items = fread((char*)point.data(), sizeof(Vec3),                               nv, in);
    (void)n_items; //< unused warning

    fclose(in);
    return true;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
