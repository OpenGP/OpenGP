#include <OpenGP/Surface_mesh.h>
#include <OpenGP/surface_mesh/remesh.h>
#include <OpenGP/surface_mesh/bounding_box.h>

using namespace std;
using namespace OpenGP;

int main(int argc, char** argv){
    if(argc!=3){
        cout << "usage:" << endl << "isoremesh bunny.obj remeshed.obj" << endl;
        return EXIT_FAILURE;
    }
    std::string input(argv[1]);
    std::string output(argv[2]);
    
    ///--- Load mesh
    Surface_mesh mesh;
    mesh.read(input);
    if(mesh.n_vertices()==0){
        cout << "Input mesh has 0 vertices" << endl;
        return EXIT_FAILURE;
    }
    
    ///--- Remesher is only for triangulations!
    mesh.triangulate();

    ///--- Compute bounding box
    Scalar bbox_diag_length = bounding_box(mesh).diagonal().norm();
    cout << "#vertices: " << mesh.n_vertices() << endl;
    cout << "bbox_diag_length: " << bbox_diag_length << endl;
            
    ///--- Perform remeshing    
    IsotropicRemesher remesher(mesh);
    remesher.num_iterations = 10;
    remesher.sharp_feature_deg = 45;
    remesher.longest_edge_length = 0.02*bbox_diag_length;
    remesher.keep_short_edges = false;
    remesher.reproject_to_surface = true;
    remesher.myout = &std::cout; ///< print output to...
    remesher.execute();
    
    ///--- Write to file
    mesh.write(output);
    return EXIT_SUCCESS;
}




