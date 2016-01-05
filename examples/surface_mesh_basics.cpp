#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;

int main(void)
{
    // instantiate a SurfaceMesh object
    SurfaceMesh mesh;

    // instantiate 4 vertex handles
    SurfaceMesh::Vertex v0,v1,v2,v3;

    // add 4 vertices
    v0 = mesh.add_vertex(Vec3(0,0,0));
    v1 = mesh.add_vertex(Vec3(1,0,0));
    v2 = mesh.add_vertex(Vec3(0,1,0));
    v3 = mesh.add_vertex(Vec3(0,0,1));

    // add 4 triangular faces
    mesh.add_triangle(v0,v1,v3);
    mesh.add_triangle(v1,v2,v3);
    mesh.add_triangle(v2,v0,v3);
    mesh.add_triangle(v0,v2,v1);

    std::cout << "vertices: " << mesh.n_vertices() << std::endl;
    std::cout << "edges: "    << mesh.n_edges()    << std::endl;
    std::cout << "faces: "    << mesh.n_faces()    << std::endl;

    return 0;
}
