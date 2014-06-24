/// @see http://qt-project.org/wiki/How_to_use_OpenGL_Core_Profile_with_Qt
#include <QApplication>
#include "QGLMeshLabViewer.h"
#include <OpenGP/Surface_mesh.h>
#include <OpenGP/surface_mesh/bounding_box.h>
#include "OpenGP/GL/shader_helpers.h"

#include <QGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>

using namespace opengp;

/// Vertex position of the triangle
const GLfloat vertices[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,};

class Viewer : public QGLMeshLabViewer {
protected:
    Surface_mesh& mesh;
    QGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QGLBuffer vertexbuffer = QGLBuffer(QGLBuffer::VertexBuffer);

public:
    Viewer(Surface_mesh& mesh):mesh(mesh){}    
public:
    void init(){        
        ///--- Create an array object to store properties
        {
            bool success = vao.create();
            assert(success);
            vao.bind();
        }
        
        ///--- Load/compile shaders
        {
            bool vok = program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl");
            bool fok = program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl");
            bool lok = program.link ();
            assert(lok && vok && fok);
            bool success = program.bind();
            assert(success);
        }
        
        ///--- Create vertex buffer/attributes "position"
        {
            bool success = vertexbuffer.create();
            assert(success);
            vertexbuffer.setUsagePattern( QGLBuffer::StaticDraw ); 
            success = vertexbuffer.bind();
            assert(success);
            
            vertexbuffer.allocate( vertices, 3 * 3 * sizeof( float ) );
            program.setAttributeBuffer( "position", GL_FLOAT, 0, 3 );
            program.enableAttributeArray("position");
        }
        
        ///--- Setup camera
        {        
            camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
            camera()->setSceneCenter(qglviewer::Vec(0,0,0));
            camera()->setSceneRadius(1);
            camera()->showEntireScene();
        }

        // auto vpoints = mesh.get_vertex_property<Vec3>("v:point");      
    }

    void draw(){
        ///--- Update modelview
        setup_modelview(camera(), program);
        
        ///--- clear & draw
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};


int main(int argc, char** argv) {
    QApplication application(argc,argv);
    if(argc!=2) return EXIT_FAILURE;
    
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
