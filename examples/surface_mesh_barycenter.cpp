#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/MLogger.h>
using namespace OpenGP;

int main(int argc, char** argv) {
    std::string file = (argc>1) ? argv[1] : "bunny.obj";
    
    SurfaceMesh mesh;
    bool success = mesh.read(file);
    CHECK(success);

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
