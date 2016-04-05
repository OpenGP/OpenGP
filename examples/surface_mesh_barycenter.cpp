#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;

int main(int /*argc*/, char** argv) {
    SurfaceMesh mesh;

    mesh.read(argv[1]);

    // get (pre-defined) property storing vertex positions
    SurfaceMesh::Vertex_property<Vec3> points = mesh.get_vertex_property<Vec3>("v:point");

    SurfaceMesh::Vertex_iterator vit, vend = mesh.vertices_end();

    Vec3 p(0,0,0);

    for (vit = mesh.vertices_begin(); vit != vend; ++vit) {
        // access point property like an array
        p += points[*vit];
    }

    p /= mesh.n_vertices();

    std::cout << "barycenter: " << p << std::endl;
}
