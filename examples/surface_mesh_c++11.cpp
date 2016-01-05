/// Barycenter example re-written with C++11 syntax

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;
using namespace std;

int main(int argc, char** argv){
    SurfaceMesh mesh;
    if(!(argc==2 && mesh.read(argv[1]))){ 
        cout << "!!! file doesn't exist or no file provided" << endl;
        exit(0);
    }
    
    auto points = mesh.get_vertex_property<Vec3>("v:point");
    Vec3 p(0,0,0);
    for(auto vit: mesh.vertices())
        p += points[vit];
    p /= mesh.n_vertices();
    
    std::cout << "barycenter: " << p << std::endl;
}
