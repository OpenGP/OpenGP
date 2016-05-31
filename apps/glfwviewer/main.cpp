#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/GlfwWindow.h>

///--- Mesh rendering
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderCloud.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderVertexNormals.h>

using namespace OpenGP;
int main(int argc, char** argv){   
    std::string file = (argc==2) ? argv[1] : "bunny.obj";
    
    SurfaceMesh mesh;
    bool success = mesh.read(file);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    ///--- Smooth shading
    GlfwWindow window1("shaded",640,480);
    SurfaceMeshRenderShaded shaded(mesh);
    window1.scene.add(shaded);
    
    ///--- Flat shading (with wiremesh)
    GlfwWindow window2("wiremesh",640,480);
    SurfaceMeshRenderFlat flat(mesh);
    window2.scene.add(flat);
        
    ///--- Point cloud w/ normals
    GlfwWindow window3("cloud",640,480);
    SurfaceMeshRenderCloud cloud(mesh);
    SurfaceMeshRenderVertexNormals vnormals(mesh, .05);
    window3.scene.add(cloud);
    window3.scene.add(vnormals);
    
    return GlfwWindow::run();
}


