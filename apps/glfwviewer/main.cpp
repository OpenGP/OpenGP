#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/Surface_mesh.h>
#include <OpenGP/surface_mesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/surface_mesh/GL/SurfaceMeshRenderShaded.h>
using namespace opengp;

int main(int argc, char** argv){   
    if(argc!=2) mFatal("usage: glfwviewer bunny.obj");
    
    Surface_mesh mesh;
    bool success = mesh.read(argv[1]);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    GlfwWindow window("Viewer",640,480);

    SurfaceMeshRenderShaded shaded(mesh);
    shaded.model = opengp::translate(-.5,0,0);
    window.scene.add(shaded);
    
    SurfaceMeshRenderFlat flat(mesh);
    flat.model = opengp::translate(+.5,0,0);
    window.scene.add(flat);
    
    return window.run();
}
