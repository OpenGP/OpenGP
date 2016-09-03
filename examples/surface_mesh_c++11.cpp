/// Barycenter example re-written with C++11 syntax
#include <OpenGP/MLogger.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;
using namespace std;

int main(int argc, char** argv) {
    std::string file = (argc>1) ? argv[1] : "bunny.obj";

    SurfaceMesh mesh;
    bool success = mesh.read(file);
    CHECK(success);

    auto points = mesh.get_vertex_property<Vec3>("v:point");
    Vec3 p(0,0,0);
    for (auto vit: mesh.vertices())
        p += points[vit];
    p /= mesh.n_vertices();

    std::cout << "barycenter: " << p << std::endl;
}
