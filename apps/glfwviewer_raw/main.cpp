#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>
#include <OpenGP/GL/glfw_helpers.h>
#include <OpenGP/GL/glfw_trackball.h>
#include <OpenGP/GL/Eigen.h>
#include <OpenGP/MLogger.h>

using namespace OpenGP;
using namespace std;

/// Viewer global status
SurfaceMesh mesh;
std::vector<unsigned int> triangles; 

/// @todo Find a better place where to put it
GLuint programID;

/// Matrix stack
Eigen::Matrix4f projection;
Eigen::Matrix4f model;
Eigen::Matrix4f view;

void set_uniform_vector(GLuint programID, const char* NAME, const Eigen::Vector3f& vector){
    GLuint matrix_id = glGetUniformLocation(programID, NAME);   
    glUniform3fv(matrix_id, 1, vector.data());
}
void set_uniform_matrix(GLuint programID, const char* NAME, const Eigen::Matrix4f& matrix){
    GLuint matrix_id = glGetUniformLocation(programID, NAME);   
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, matrix.data());
}

void update_projection_matrix() {
    /// Define projection matrix (FOV, aspect, near, far)
    projection = OpenGP::perspective(45.0f, static_cast<float>(_width)/static_cast<float>(_height), 0.1f, 10.f);
    // cout << projection << endl;
}

/// OpenGL initialization
void init(){
    ///----------------------- DATA ----------------------------
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
    assert(vpoints);
    assert(vnormals);    
    
    ///---------------------- TRIANGLES ------------------------
    triangles.clear();
    for(auto f: mesh.faces())
        for(auto v: mesh.vertices(f))
            triangles.push_back(v.idx());
    
    ///---------------------- OPENGL GLOBALS--------------------
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); ///< background
    glEnable(GL_DEPTH_TEST); // Enable depth test
    // glDisable(GL_CULL_FACE); // Cull back-facing
        
    /// Compile the shaders
    programID = load_shaders("vshader.glsl", "fshader.glsl");
    if(!programID) exit(EXIT_FAILURE);
    glUseProgram( programID );
    
    ///---------------------- CAMERA ----------------------------
    {
        typedef Eigen::Vector3f vec3;
        typedef Eigen::Matrix4f mat4;

        update_projection_matrix();

        /// Define the view matrix (camera extrinsics)
        vec3 cam_pos(0,0,5);
        vec3 cam_look(0,0,-1); /// Remember: GL swaps viewdir
        vec3 cam_up(0,1,0);
        view = OpenGP::lookAt(cam_pos, cam_look, cam_up);
        // cout << view << endl;
        
        /// Define the modelview matrix
        model = mat4::Identity();
        // cout << model << endl;  
        
        /// Set initial matrices
        set_uniform_matrix(programID,"M",model); ///< to get world coordinates
        set_uniform_matrix(programID,"MV",view*model); ///< to get camera coordinates
        set_uniform_matrix(programID,"MVP",projection*view*model); ///< to get clip coordinates         
    }
    
    ///---------------------- LIGHT -----------------------------
    {
        Vec3 light_dir(0,0,1);
        set_uniform_vector(programID,"LDIR",light_dir); ///< to get camera coordinates
    }
    
    ///---------------------- VARRAY ----------------------------    
    {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);  
    }
        
    ///---------------------- BUFFERS ----------------------------    
    GLuint vertexbuffer, normalbuffer, trianglebuffer; 
    {
        /// Load mesh vertices
        glGenBuffers(1, &vertexbuffer); 
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(Vec3), vpoints.data(), GL_STATIC_DRAW); 
        
        /// Load mesh normals    
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(Vec3), vnormals.data(), GL_STATIC_DRAW);     
        
        /// Triangle indexes buffer
        glGenBuffers(1, &trianglebuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglebuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), &triangles[0], GL_STATIC_DRAW);
    }

    ///---------------------- SHADER ATTRIBUTES ----------------------------    
    {
        /// Vertex positions in shader variable "vposition"
        GLuint vposition = glGetAttribLocation(programID, "vposition");        
        glEnableVertexAttribArray(vposition);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(vposition, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        
        /// Vertex normals in in shader variable "vnormal"
        GLuint vnormal = glGetAttribLocation(programID, "vnormal");        
        glEnableVertexAttribArray(vnormal);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(vnormal, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    }
}

/// 
void update_matrices(Eigen::Matrix4f model){
    set_uniform_matrix(programID,"M",model);
    set_uniform_matrix(programID,"MV",view*model);
    set_uniform_matrix(programID,"MVP",projection*view*model);
}

/// OpenGL render loop
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
}

/// Entry point
int main(int argc, char** argv){
    if(argc!=2) mFatal("usage: glfwviewer bunny.obj");    
    int success = mesh.read(argv[1]);
    if(!success) mFatal() << "File not found";
    
    mesh.triangulate();
    mesh.update_vertex_normals();
    cout << "input: '" << argv[1] << "' num vertices " << mesh.vertices_size() << endl;
    cout << "BBOX: " << bounding_box(mesh) << endl;

    glfwInitWindowSize(_width, _height);
    if (glfwMakeWindow(__FILE__) == EXIT_FAILURE)
        return EXIT_FAILURE;

    glfwDisplayFunc(display);
    glfwTrackball(update_matrices, update_projection_matrix);
    init();
    glfwMainLoop();

    return EXIT_SUCCESS;
}
