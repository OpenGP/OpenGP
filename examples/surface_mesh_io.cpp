#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/MLogger.h>
using namespace OpenGP;

int main(int argc, char** argv) {
    std::string in_file = (argc>1) ? argv[1] : "bunny.obj";
    std::string out_file = (argc>2) ? argv[2] : "output.obj";

    // instantiate a SurfaceMesh object
    SurfaceMesh mesh;

    // read a mesh specified as the first command line argument
    bool success = mesh.read(in_file);
    CHECK(success);

    // ...
    // do fancy stuff with the mesh
    // ...

    // write the mesh to the file specified as second argument
    mesh.write(out_file);
    CHECK(success);

    mLogger() << "read:" << in_file << "wrote:" << out_file;
    return EXIT_SUCCESS;
}
