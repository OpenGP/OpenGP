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
    if(argc!=2) mFatal("usage: glfwviewer bunny.obj");
    
    SurfaceMesh mesh;
    bool success = mesh.read(argv[1]);
    if(!success) mFatal() << "File not found";
    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    GlfwWindow window("glfwviewer",800,300);

    ///--- Smooth shading
    SurfaceMeshRenderShaded shaded(mesh);
    shaded.model = translate(-1.0,0,0);
    window.scene.add(shaded);
    
    ///--- Flat shading (with wiremesh)
    SurfaceMeshRenderFlat flat(mesh);
    flat.model = translate(+1.0,0,0);
    window.scene.add(flat);
        
    ///--- Point cloud w/ normals
    SurfaceMeshRenderCloud cloud(mesh);
    SurfaceMeshRenderVertexNormals vnormals(mesh, .05);
    window.scene.add(cloud);
    window.scene.add(vnormals);
    
    return window.run();
}


