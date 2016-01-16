#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

const static GLchar* SurfaceMeshRenderShaded_vshader = R"GLSL(
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
    
const static char* SurfaceMeshRenderShaded_fshader = R"GLSL(
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

void SurfaceMeshRenderShaded::init(){
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
    program.add_vshader_from_source(SurfaceMeshRenderShaded_vshader);
    program.add_fshader_from_source(SurfaceMeshRenderShaded_fshader);
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

void OpenGP::SurfaceMeshRenderShaded::display(){
    program.bind();        
    vao.bind();
    glDrawElements(GL_TRIANGLES, i_buffer.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
    vao.release();
    program.release();
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
