/// @see http://qt-project.org/wiki/How_to_use_OpenGL_Core_Profile_with_Qt
#include <QApplication>
#include "QGLMeshLabViewer.h"
#include <OpenGP/Surface_mesh.h>
#include <OpenGP/surface_mesh/bounding_box.h>

#include <QGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>

using namespace opengp;
using namespace std;

inline bool isDepthTestEnabled(){
    GLboolean val = 0;
    glGetBooleanv(GL_DEPTH_TEST,&val);
    return val;        
}

class Viewer : public QGLMeshLabViewer {
protected:
    Surface_mesh& mesh;
    QGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QGLBuffer vertexbuffer = QGLBuffer(QGLBuffer::VertexBuffer);
    QGLBuffer normalbuffer = QGLBuffer(QGLBuffer::VertexBuffer);
    QGLBuffer indexbuffer = QGLBuffer(QGLBuffer::IndexBuffer);
    std::vector<unsigned int> triangles;
    
public:
    Viewer(Surface_mesh& mesh):mesh(mesh){}
    
public:
    void init(){     
        ///--- Setup opengl flags
        glEnable(GL_DEPTH_TEST);
        
        ///--- Create the triangle index buffer
        {
            assert(mesh.is_triangle_mesh());
            triangles.clear();
            for(auto f: mesh.faces())
                for(auto v: mesh.vertices(f))
                    triangles.push_back(v.idx());
        }
        
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
            auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
            bool success = vertexbuffer.create();
            assert(success);
            vertexbuffer.setUsagePattern( QGLBuffer::StaticDraw ); 
            success = vertexbuffer.bind();
            assert(success);
            vertexbuffer.allocate( vpoints.data(), sizeof(Vec3) * mesh.n_vertices() );
            program.setAttributeBuffer("vpoint", GL_FLOAT, 0, 3 );
            program.enableAttributeArray("vpoint");
        }
        
        ///--- Create vertex buffer/attributes "normal"
        {
            auto vnormal = mesh.get_vertex_property<Vec3>("v:normal");
            bool success = normalbuffer.create();
            assert(success);
            normalbuffer.setUsagePattern( QGLBuffer::StaticDraw ); 
            success = normalbuffer.bind();
            assert(success);
            normalbuffer.allocate( vnormal.data(), sizeof(Vec3) * mesh.n_vertices() );
            program.setAttributeBuffer("vnormal", GL_FLOAT, 0, 3 );
            program.enableAttributeArray("vnormal");
        }
        
        ///--- Create the index "triangle" buffer
        {
            bool success = indexbuffer.create();
            assert(success);
            indexbuffer.setUsagePattern( QGLBuffer::StaticDraw ); 
            success = indexbuffer.bind();
            assert(success);
            indexbuffer.allocate(&triangles[0], triangles.size()*sizeof(unsigned int));
        }
        
        ///--- Setup camera
        {        
            camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
            camera()->setSceneCenter(qglviewer::Vec(0,0,0));
            camera()->setSceneRadius(1);
            camera()->showEntireScene();
        }
        
        ///--- Unbind to avoid pollution
        vao.release();
        program.release();
    }

    void draw(){
#ifdef FORCE_REENABLE_DEPTH
        glEnable(GL_DEPTH_TEST);
#endif
        
        cout << __LINE__ << ": " << isDepthTestEnabled() << endl;        
        vao.bind();
        program.bind();
        cout << __LINE__ << ": " << isDepthTestEnabled() << endl;        
        {
            ///--- Update modelview
            setup_modelview(camera(), program);
                        
            ///--- clear & draw
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
        }
        cout << __LINE__ << ": " << isDepthTestEnabled() << endl;        
        vao.release();
        program.release();
        cout << __LINE__ << ": " << isDepthTestEnabled() << endl << endl;        
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

