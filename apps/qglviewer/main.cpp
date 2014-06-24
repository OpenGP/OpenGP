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
public:
    Viewer(Surface_mesh& mesh):mesh(mesh){}

protected:
    Surface_mesh& mesh;
    QGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    
public:
    ///--- MUST not be called more than once!
    void init(){        
        ///--- Load shaders
        bool vok = program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl");
        bool fok = program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl");
        bool lok = program.link ();
        assert(lok && vok && fok);        
        program.bind();
        
        ///--- Setup camera
        camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
        camera()->setSceneCenter(qglviewer::Vec(0,0,0));
        camera()->setSceneRadius(1);
        camera()->showEntireScene();
               
        ///--- Create an array object to store properties
        bool vaook = vao.create();
        assert(vaook);
        vao.bind();
        
#if 0
        ///--- Setup vertex positions property
        {
            ///--- buffer
            QGLBuffer vertex_buffer_ = QGLBuffer (QGLBuffer::VertexBuffer);
            bool success = vertex_buffer_.create();
            vertex_buffer_.setUsagePattern (QGLBuffer::StaticDraw);
            assert(success);
            vertex_buffer_.bind();            
            ///--- attribute
            
        }
#else   
        GLuint vertexbuffer; 
        glGenBuffers(1, &vertexbuffer); 
        /// The subsequent commands will affect the specified buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); 
        /// Pass the vertex positions to OpenGL
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
#endif
        
        /// @todo explain what are vertex attributes
        /// Vertex Attribute ID for Vertex Positions
        GLuint position = glGetAttribLocation(program.programId(), "position");
        glEnableVertexAttribArray(position);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(position, 3, GL_FLOAT, /*DONT_NORMALIZE*/0, /*ZERO_STRIDE*/0, /*ZERO_BUFFER_OFFSET*/0);        
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
