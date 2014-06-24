#include <QApplication>
#include "QGLMeshLabViewer.h"
#include <OpenGP/Surface_mesh.h>
#include <QGLShaderProgram>
using namespace opengp;

class Viewer : public QGLMeshLabViewer {
public:
    Viewer(Surface_mesh& mesh):mesh(mesh){}
protected:
    Surface_mesh& mesh;
    QGLShaderProgram program;
    void init(){}
    void draw(){}
};


int main(int argc, char** argv) {
    QApplication application(argc,argv);
 
    Surface_mesh mesh;
    bool ok = mesh.read(argv[1]);
    if(!ok) return EXIT_FAILURE;
    
    ///--- Preprocess
    mesh.triangulate();
    mesh.update_vertex_normals();
    
    Viewer viewer(mesh);
    viewer.setWindowTitle("OpenGP/apps/qglviewer");
    viewer.show();
    return application.exec();
}
