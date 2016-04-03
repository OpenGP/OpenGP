#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/remesh.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>
#include <OpenGP/MLogger.h>

using namespace std;
using namespace OpenGP;

// usage: isoremesh bunny.obj remeshed.obj
int main(int argc, char** argv){
    std::string in_file = (argc>1) ? argv[1] : "bunny.obj";
    std::string out_file = (argc>2) ? argv[2] : "remeshed.obj";
    
    ///--- Load mesh
    SurfaceMesh mesh;
    bool success = mesh.read(in_file);
    CHECK(success);
    
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
    mesh.write(out_file);
    return EXIT_SUCCESS;
}




