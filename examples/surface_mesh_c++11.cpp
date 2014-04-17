/// Barycenter example re-written with C++11 syntax

#include <OpenGP/Surface_mesh.h>
using namespace opengp;
int main(int /*argc*/, char** argv)
{
    Surface_mesh mesh;
    mesh.read(argv[1]);
    auto points = mesh.get_vertex_property<Vec3>("v:point");
    Vec3 p(0,0,0);
    for(auto vit: mesh.vertices())
        p += points[vit];
    p /= mesh.n_vertices();
    
    std::cout << "barycenter: " << p << std::endl;
}
