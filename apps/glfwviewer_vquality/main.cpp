#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/MATLAB/random.h>

///--- Mesh rendering
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>

using namespace OpenGP;
int main(int argc, char** argv){   
    if(argc!=2) mFatal("usage: glfwviewer bunny.obj");
    
    SurfaceMesh mesh;
    bool success = mesh.read(argv[1]);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    GlfwWindow window("glfwviewer",300,300);  
    
    ///--- Create some test vertex quality
    auto vquality = mesh.vertex_property<Scalar>("v:quality");
    for(SurfaceMesh::Vertex v: mesh.vertices())
        vquality[v] = OpenGP::_rand();
    
    ///--- Smooth shading w/ colormapped vertex quality
    SurfaceMeshRenderShaded shaded(mesh);
    window.scene.add(shaded);
    shaded.colormap_enabled(true);
    shaded.colormap_set_range(0.0,1.0);
   
    return window.run();
}


