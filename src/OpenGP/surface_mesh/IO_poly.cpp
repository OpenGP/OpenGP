//== INCLUDES =================================================================


#include <OpenGP/Surface_mesh.h>
#include <OpenGP/surface_mesh/IO.h>
#include <cstdio>


//== NAMESPACES ===============================================================


namespace opengp {


//== IMPLEMENTATION ===========================================================


//-----------------------------------------------------------------------------


bool read_poly(Surface_mesh& mesh, const std::string& filename)
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
    Surface_mesh::Vertex_property<Surface_mesh::Vertex_connectivity>      vconn = mesh.vertex_property<Surface_mesh::Vertex_connectivity>("v:connectivity");
    Surface_mesh::Halfedge_property<Surface_mesh::Halfedge_connectivity>  hconn = mesh.halfedge_property<Surface_mesh::Halfedge_connectivity>("h:connectivity");
    Surface_mesh::Face_property<Surface_mesh::Face_connectivity>          fconn = mesh.face_property<Surface_mesh::Face_connectivity>("f:connectivity");
    Surface_mesh::Vertex_property<Vec3>                                  point = mesh.vertex_property<Vec3>("v:point");

    // read properties from file
    n_items = fread((char*)vconn.data(), sizeof(Surface_mesh::Vertex_connectivity),   nv, in);
    n_items = fread((char*)hconn.data(), sizeof(Surface_mesh::Halfedge_connectivity), nh, in);
    n_items = fread((char*)fconn.data(), sizeof(Surface_mesh::Face_connectivity),     nf, in);
    n_items = fread((char*)point.data(), sizeof(Vec3),                               nv, in);

    fclose(in);
    return true;
}


//=============================================================================
} // namespace opengp
//=============================================================================
