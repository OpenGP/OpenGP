#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;

int main(int /*argc*/, char** argv)
{
    // instantiate a SurfaceMesh object
    SurfaceMesh mesh;

    // read a mesh specified as the first command line argument
    mesh.read(argv[1]);

    // ...
    // do fancy stuff with the mesh
    // ...

    // write the mesh to the file specified as second argument
    mesh.write(argv[2]);

    return 0;
}
