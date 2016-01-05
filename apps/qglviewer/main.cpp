/// @see http://qt-project.org/wiki/How_to_use_OpenGL_Core_Profile_with_Qt

///--- Uncomment to force-disable QGLViewer
// #undef WITH_QGLVIEWER

///--- Qt
#include <QApplication>
#include <QGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>

///--- Support for OpenGL (also #define GL_GLEXT_PROTOTYPES=1)
#if __unix__
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

///--- Surface_mesh
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

#ifdef WITH_QGLVIEWER
    #include <OpenGP/qglviewer/QGLMeshLabViewer.h>
    #include <OpenGP/qglviewer/helpers.h>
    typedef QGLMeshLabViewer QGLWidgetSuperClass;
#else
    #include <OpenGP/GL/QGLWidget32.h>
    #include <OpenGP/GL/EigenOpenGLSupport3.h>
    typedef OpenGP::QGLWidget32 QGLWidgetSuperClass;
#endif

using namespace OpenGP;
using namespace std;

class Viewer : public QGLWidgetSuperClass {
protected:
    SurfaceMesh& mesh;
    QGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QGLBuffer vertexbuffer = QGLBuffer(QGLBuffer::VertexBuffer);
    QGLBuffer normalbuffer = QGLBuffer(QGLBuffer::VertexBuffer);
    QGLBuffer indexbuffer = QGLBuffer(QGLBuffer::IndexBuffer);
    std::vector<unsigned int> triangles;
    
public:
    Viewer(SurfaceMesh& mesh):mesh(mesh){}
    
public:
    /// @overload QGLWidget
    void initializeGL(){     
        printf("OpenGL %d.%d\n",this->format().majorVersion(),this->format().minorVersion());
 
        ///--- Background
        glClearColor(1.0, 1.0, 1.0, 1.0);
        
        ///--- Viewport (simple, for unresizeable window)
        glViewport(0, 0, this->width(), this->height());
        
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
        
#ifdef WITH_QGLVIEWER
        ///--- Setup camera
        {        
            Box3 bbox = OpenGP::bounding_box(mesh);
            camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
            camera()->setSceneCenter(qglviewer::tr(bbox.center()));
            camera()->setSceneRadius(bbox.diagonal().norm()/2.0);
            camera()->showEntireScene();
        }
#endif
        
        ///--- Unbind to avoid pollution
        vao.release();
        program.release();
    }

    /// @overload QGLWidget
    void paintGL(){
        vao.bind();
        program.bind();
        {
            
        ///--- Update modelview
#ifdef WITH_QGLVIEWER
            /// use the trackball to specify the matrices
            setup_modelview(camera(), program);
#else
            ///--- simple unit cube orthographic view
            static Eigen::Matrix4f M = Eigen::Matrix4f::Identity();
            static Eigen::Matrix4f P = Eigen::ortho(-1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f);
            static Eigen::Matrix4f V = Eigen::Matrix4f::Identity();
            static Eigen::Matrix4f MV = V*M;
            static Eigen::Matrix4f MVP = P*MV;
            GLint MVP_id = glGetUniformLocation(program.programId(), "MVP");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());
            GLint MV_id = glGetUniformLocation(program.programId(), "MV");
            glUniformMatrix4fv(MV_id, 1, GL_FALSE, MV.data());
#endif      
                        
            ///--- clear & draw
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
        }
        vao.release();
        program.release();
    }
};

int main(int argc, char** argv) {
    QApplication application(argc,argv);
    if(argc!=2){
        cout << "One argument necessary, given: " << (argc-1) << endl;
        return EXIT_FAILURE;
    }
    
    SurfaceMesh mesh;
    bool ok = mesh.read(argv[1]);
    if(!ok){
        cout << "could not open file: " << argv[1] << endl;
        return EXIT_FAILURE;
    }
    
    ///--- Preprocess
    mesh.triangulate();
    mesh.update_vertex_normals();

    ///--- Shutdown on close GUI
    QObject::connect(&application, &QApplication::lastWindowClosed, &application, &QApplication::quit);
    
    Viewer viewer(mesh);
    viewer.setWindowTitle("OpenGP/apps/qglviewer");
    viewer.show();
    return application.exec();
}

