#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>

///--- Mesh rendering
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>

///--- Point clouds
#include <OpenGP/MATLAB/random.h>
#include <OpenGP/GL/PointCloudRenderer.h>

using namespace OpenGP;

int main(int argc, char** argv){   
    if(argc!=2) mFatal("usage: glfwviewer bunny.obj");
    
    SurfaceMesh mesh;
    bool success = mesh.read(argv[1]);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    GlfwWindow window("glfwviewer",640,480);

    SurfaceMeshRenderShaded shaded(mesh);
    shaded.model = OpenGP::translate(-.5,0,0);
    window.scene.add(shaded);
    
    SurfaceMeshRenderFlat flat(mesh);
    flat.model = OpenGP::translate(+.5,0,0);
    window.scene.add(flat);
    
    
    ///--- Renders some colored points 
    int n = 100;
    MatMxN P = .3f * OpenGP::randn(3,n);
    MatMxN C = OpenGP::rand(3,n);
    PointCloudRenderer rend(P);
    rend.model = OpenGP::translate(0,+.5,0);
    window.scene.add(rend);
    rend.set_vsize(15);
    rend.set_colors(C);
    
    return window.run();
}


