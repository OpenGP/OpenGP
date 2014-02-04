// Copyright (C) 2014 by Andrea Tagliasacchi
// Simple mesh viewer based on GLFW
// @see https://code.google.com/p/opengl-tutorial-org/source/browse/tutorial08_basic_shading/tutorial08.cpp

#include <OpenGeometry/surface_mesh/Surface_mesh.h>
#include <OpenGeometry/GL/simple_glfw_window.h>

/// @todo update once Eigen integrates the changes
#include <OpenGeometry/GL/EigenOpenGLSupport3.h> 

using namespace surface_mesh;

const int VPOS_NUMELS = 3;       ///< Vertex position attribute (x,y,z)
const int NUM_VERTS = 3;         ///< Number of vertices to draw

Surface_mesh mesh;
       
void init(){
    ///---------------------- DATA ----------------------------
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
    auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
    assert(vpoints);
    assert(vnormals);    
    
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
        mat4 projection = Eigen::perspective(45.0f, 4.0f/3.0f, -10.f, 10.f);
        // std::cout << projection << std::endl;
    
        /// Define the view matrix (camera extrinsics)
        vec3 cam_pos(0,0,5);
        vec3 cam_look(0,0,1);
        vec3 cam_up(0,1,0);
        mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
        // std::cout << view << std::endl;
        
        /// Define the modelview matrix
        mat4 model = mat4::Identity();
        
        /// Assemble the "Model View Projection" matrix
        // mat4 mvp = projection * view * model;
        mat4 mvp = mat4::Identity();
         
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
    glDrawArrays(GL_TRIANGLES, 0, mesh.n_vertices());
}

int main(int argc, char** argv){
    assert(argc==2);
    mesh.read(argv[1]);
    mesh.update_vertex_normals();
    // mesh.property_stats();
    std::cout << "num vertices " << mesh.vertices_size() << std::endl;
    simple_glfw_window("mesh viewer", 640, 480, init, display);
}
