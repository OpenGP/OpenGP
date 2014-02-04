// Copyright (C) 2014 by Andrea Tagliasacchi
// Simple mesh viewer based on GLFW
// @see https://code.google.com/p/opengl-tutorial-org/source/browse/tutorial08_basic_shading/tutorial08.cpp

#include <OpenGeometry/surface_mesh/Surface_mesh.h>
#include <OpenGeometry/GL/simple_glfw_window.h>

/// @todo update once Eigen integrates the changes
#include <OpenGeometry/GL/EigenOpenGLSupport3.h> 

using namespace surface_mesh;

Surface_mesh mesh;
std::vector<unsigned int> triangles; 

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
    
    /// Enable desired OpenGL states    
    // glEnable(GL_DEPTH_TEST); // Enable depth test
    // glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
    // glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera
        
    /// Compile the shaders
    GLuint programID = load_shaders( "vshader.glsl", "fshader.glsl" );
    if(!programID) exit(EXIT_FAILURE);
    glUseProgram( programID );
    
    ///---------------------- CAMERA ----------------------------
    {
        typedef Eigen::Vector3f vec3;
        typedef Eigen::Matrix4f mat4;
        
        /// Define projection matrix (FOV, aspect, near, far)
        mat4 projection = Eigen::perspective(45.0f, 4.0f/3.0f, 0.f, 10.f);
        
        /// Define the view matrix (camera extrinsics)
        vec3 cam_pos(0,0,3);
        vec3 cam_look(0,0,-1); /// Remember: GL swaps viewdir
        vec3 cam_up(0,1,0);
        mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
        
        /// Define the modelview matrix
        mat4 model = mat4::Identity();
        
        /// Assemble the "Model View Projection" matrix
        mat4 mvp = projection * view * model; 
        // std::cout << mvp << std::endl;
         
        /// Pass the matrix to the shader
        /// The glUniform call below is equivalent to the OpenGL function call:
        /// \code glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &mvp[0][0]); \endcode
        const char* MVP = "MVP"; ///< Name of variable in the shader
        GLuint mvp_id = glGetUniformLocation(programID, MVP);
        Eigen::glUniform(mvp_id, mvp);
    }
    
    ///---------------------- VARRAY ----------------------------    
    {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);  
    }
        
    ///---------------------- BUFFERS ----------------------------    
    GLuint vertexbuffer, normalbuffer; 
    {
        /// Load mesh vertices
        glGenBuffers(1, &vertexbuffer); 
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.n_vertices()) * sizeof(Vec3f), vpoints.data(), GL_STATIC_DRAW); 
    
        /// Load mesh normals    
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.n_vertices()) * sizeof(Vec3f), vnormals.data(), GL_STATIC_DRAW);     
        
        /// Triangle indexes buffer
        GLuint trianglebuffer;
        glGenBuffers(1, &trianglebuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglebuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), &triangles[0], GL_STATIC_DRAW);
    }

    ///---------------------- SHADER ATTRIBUTES ----------------------------    
    {
        /// Readability constants
        enum ATTRIBUTES{VPOS=0, VNOR=1};
        const bool UNNORMALIZED = false;
        const int STRIDE = 0;
        const void* BUFFER_OFFSET = 0;
        
        /// Vertex positions in VPOS
        glEnableVertexAttribArray(VPOS);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(VPOS, 3, GL_FLOAT, UNNORMALIZED, STRIDE, BUFFER_OFFSET);
        
        /// Vertex normals in VNOR
        glEnableVertexAttribArray(VNOR);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(VNOR, 3, GL_FLOAT, UNNORMALIZED, STRIDE, BUFFER_OFFSET);
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}

int main(int argc, char** argv){
    assert(argc==2);
    mesh.read(argv[1]);
    mesh.update_vertex_normals();
    // mesh.property_stats();
    std::cout << "num vertices " << mesh.vertices_size() << std::endl;
    simple_glfw_window("mesh viewer", 640, 480, init, display);
}
