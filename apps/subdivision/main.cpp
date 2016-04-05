#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/SurfaceMesh/Subdivision/Loop.h>
#include <OpenGP/GL/GlfwWindow.h>
using namespace OpenGP;

struct MainWindow : public GlfwWindow{
    SurfaceMesh mesh;
    SurfaceMeshRenderFlat renderer = SurfaceMeshRenderFlat(mesh);
 
    MainWindow(int argc, char** argv) : GlfwWindow(__FILE__,400,400){
        if(argc!=2) mFatal("application requires one parameter! e.g. sphere.obj");
        bool success = mesh.read(argv[1]);
        if(!success) mFatal() << "File not found: " << argv[1];
        mesh.update_face_normals(); ///< shading
        this->scene.add(renderer);
    }
    
    bool key_callback(int key, int scancode, int action, int mods) override{
        if( GlfwWindow::key_callback(key, scancode, action, mods) )
            return true;
        if(key==GLFW_KEY_SPACE && action==GLFW_RELEASE){
            SurfaceMeshSubdivideLoop::exec(mesh);
            mesh.update_face_normals();
            renderer.init_data();
            return true;
        }
        return false;
    }
};

int main(int argc, char** argv){
    MainWindow window(argc, argv);
    return window.run();
}
