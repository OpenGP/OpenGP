#pragma once
#include <vector>
#include <OpenGP/MLogger.h>
#include <OpenGP/Surface_mesh.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/gl.h>

//=============================================================================
namespace opengp {
//=============================================================================

class SurfaceMeshRenderShaded : public SceneObject{
private:
    Surface_mesh& mesh;
    VertexArrayObject vao;    
    ArrayBuffer<Vec3> v_buffer;
    ArrayBuffer<Vec3> n_buffer;
    ElementArrayBuffer<uint> i_buffer;
    
private:
    const GLchar* vshader = R"GLSL(
        #version 330 core 
        uniform mat4 M;
        uniform mat4 MV;
        uniform mat4 MVP; 
        in vec3 vposition;  ///< per-vertex position
        in vec3 vnormal;    ///< per-vertex normal
        out vec3 fnormal;   ///< per-fragment normal
        
        void main(){ 
            gl_Position = MVP * vec4(vposition, 1.0); 
            fnormal = normalize( inverse(transpose(mat3(MV))) * vnormal );
        }
    )GLSL";
        
    const char* fshader = R"GLSL(
        #version 330 core
        // uniform vec3 LDIR; ///< TODO: fix me
        in vec3 fnormal; ///< normal camera coords
        out vec4 fcolor;
    
        void main(){
            vec3 LDIR = vec3(0,0,1);
            vec3 ldir = normalize(LDIR);
            float albedo = max( dot( normalize(fnormal), ldir ), 0 );   
            vec3 basecolor = vec3(1,0,0);
            fcolor = vec4(basecolor*albedo, 1);        
            // fcolor = vec4(fnormal,1); ///< normal map
        }
    )GLSL";
    
public:
    SurfaceMeshRenderShaded(Surface_mesh& mesh) : mesh(mesh){}
    
    void init(){
        ///--- Properties for upload
        auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
        auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
        CHECK(vpoints);
        CHECK(vnormals);
        
        ///--- Creates index buffer data
        std::vector<unsigned int> triangles;
        for(auto f: mesh.faces())
            for(auto v: mesh.vertices(f))
                triangles.push_back(v.idx());
        
        ///--- Shader
        program.add_vshader_from_source(vshader);
        program.add_fshader_from_source(fshader);
        program.link();
        
        ///--- Data
        v_buffer.upload(vpoints.data(), mesh.n_vertices(), sizeof(Vec3));
        n_buffer.upload(vnormals.data(), mesh.n_vertices(), sizeof(Vec3));
        i_buffer.upload(triangles);
        
        ///--- Attributes
        program.bind();
        vao.bind();
            program.set_attribute("vposition", v_buffer);
            program.set_attribute("vnormal", n_buffer);
        vao.release();
        program.release();        
    }
    
    void display(){
        program.bind();        
        vao.bind();
            glDrawElements(GL_TRIANGLES, i_buffer.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
        vao.release();
        program.release();
    }
};
    
//=============================================================================
} // namespace opengp
//=============================================================================
