#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
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

    GlfwWindow window("glfwviewer",640,480);

    SurfaceMeshRenderShaded shaded(mesh);
    shaded.model = OpenGP::translate(-.5,0,0);
    window.scene.add(shaded);
    
    SurfaceMeshRenderFlat flat(mesh);
    flat.model = OpenGP::translate(+.5,0,0);
    window.scene.add(flat);
    
    return window.run();
}
