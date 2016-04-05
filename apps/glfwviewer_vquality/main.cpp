#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/MATLAB/random.h>

///--- Surface mesh related
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>

using namespace OpenGP;
int main(int argc, char** argv){   
    std::string file = (argc==2) ? argv[1] : "bunny.obj";
    
    SurfaceMesh mesh;
    bool success = mesh.read(file);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    TrackballWindow window(__FILE__,640,480);  
    
    ///--- Create some test vertex quality
    Box3 box = bounding_box(mesh);  
    Scalar xmin = box.min()[0];
    Scalar xmax = box.max()[0];
    
    ///--- Colormap according to vertex position
    auto vquality = mesh.vertex_property<Scalar>("v:quality");
    for(SurfaceMesh::Vertex v: mesh.vertices())
        vquality[v] = mesh.position(v)[0];
    
    ///--- Smooth shading w/ colormapped vquality
    SurfaceMeshRenderShaded shaded(mesh);
    window.scene.add(shaded);
    shaded.colormap_enabled(true);
    shaded.colormap_set_range(xmin,xmax);
   
    return window.run();
}


