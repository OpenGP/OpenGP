#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

const static GLchar* SurfaceMeshRenderShaded_vshader = R"GLSL(
    #version 330 core 
    uniform mat4 M;
    uniform mat4 MV;
    uniform mat4 MVP;
                                                        
    ///--- Colormap 1D texture
    uniform sampler1D colormap; 
    uniform int use_colormap; ///< 0
    uniform float colormap_min; ///< 0.0;
    uniform float colormap_max; ///< 1.0;
                                                       
    ///--- Attributes
    in vec3 vposition;  ///< per-vertex position
    in vec3 vnormal;    ///< per-vertex normal
    in float vquality;  ///< per-vertex quality
                                                       
    ///--- Outputs
    out vec3 fnormal;   ///< per-fragment normal
    out vec3 fcolor;    ///< per-fragment color

    void main(){ 
        gl_Position = MVP * vec4(vposition, 1.0); 
        fnormal = normalize( inverse(transpose(mat3(MV))) * vnormal );
        if(use_colormap==0)
            fcolor = vec3(1,0,0);
        else{
            float vquality_normalized = (vquality - colormap_min) / (colormap_max - colormap_min);
            fcolor = texture(colormap, vquality_normalized).rgb; 
        }
    }
)GLSL";
    
const static char* SurfaceMeshRenderShaded_fshader = R"GLSL(
    #version 330 core
    // uniform vec3 LDIR; ///< TODO: fix me
    in vec3 fnormal; ///< normal camera coords
    in vec3 fcolor;
    out vec4 FragColor;

    void main(){
        vec3 LDIR = vec3(0,0,1);
        vec3 ldir = normalize(LDIR);
        float albedo = max( dot( normalize(fnormal), ldir ), 0 );   
        vec3 basecolor = fcolor;
        FragColor = vec4(basecolor*albedo, 1);        
        // FragColor = vec4(fnormal,1); ///< normal map
    }
)GLSL";

void SurfaceMeshRenderShaded::init(){
    ///--- Shaders
    program.add_vshader_from_source(SurfaceMeshRenderShaded_vshader);
    program.add_fshader_from_source(SurfaceMeshRenderShaded_fshader);
    program.link();
    
    ///--- Vertex positions
    auto vpoints = mesh.get_vertex_property<Vec3>("v:point"); CHECK(vpoints);    
    v_buffer.upload_raw(vpoints.data(), mesh.n_vertices());
    
    ///--- Vertex normals    
    auto vnormals = mesh.get_vertex_property<Vec3>("v:normal"); CHECK(vnormals);
    n_buffer.upload_raw(vnormals.data(), mesh.n_vertices());   
    
    ///--- Vertex quality (Optional)
    auto vqualitys = mesh.get_vertex_property<float>("v:quality");
    if(vqualitys) q_buffer.upload_raw(vqualitys.data(), mesh.n_vertices());
    
    ///--- Creates index/element buffer data
    CHECK(mesh.n_faces()>0);
    std::vector<unsigned int> triangles;
    for(auto f: mesh.faces())
        for(auto v: mesh.vertices(f))
            triangles.push_back(v.idx());
    i_buffer.upload(triangles);
    
    ///--- Attributes
    program.bind();
    vao.bind();
        ///--- Defaulted attributes
        program.set_attribute("vquality", 0.0f);
        
        ///--- Attributes
        program.set_attribute("vposition", v_buffer);
        program.set_attribute("vnormal", n_buffer);
        if(vqualitys) program.set_attribute("vquality", q_buffer);
        
        ///--- Colormap texture setup
        {
            // TODO: wrap this within the Game Engine
            const int sz=3; 
            GLfloat _tex_data[3*sz] = {/*red*/    1.0, 0.0, 0.0, 
                                       /*yellow*/ 1.0, 1.0, 0.0, 
                                       /*green*/  0.0, 1.0, 0.0};
            glGenTextures(1, &_tex);
            glBindTexture(GL_TEXTURE_1D, _tex);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, sz, 0, GL_RGB, GL_FLOAT, _tex_data);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);            
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            program.bind(); ///< note set_attribute unbound them
                GLint tex_id = glGetUniformLocation(program.programId(), "colormap");
                glActiveTexture(GL_TEXTURE0+0);
                glUniform1i(tex_id, 0);
            program.release();
        }
    vao.release();
    program.release();        
}

void OpenGP::SurfaceMeshRenderShaded::display(){
    program.bind();        
    vao.bind();
        ///--- Bind textures
        glActiveTexture(GL_TEXTURE0+0);
        glBindTexture(GL_TEXTURE_1D, _tex);
        
        ///--- Upload settings
        program.set_uniform("use_colormap", (int)  _use_colormap);
        program.set_uniform("colormap_min", (float)_colormap_min);
        program.set_uniform("colormap_max", (float)_colormap_max);
        
        ///--- Draw data
        glDrawElements(GL_TRIANGLES, i_buffer.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
    vao.release();
    program.release();
}

Box3 SurfaceMeshRenderShaded::bounding_box(){
    return OpenGP::bounding_box(mesh);
}

void SurfaceMeshRenderShaded::colormap_enabled(bool enabled){
    _use_colormap = enabled;
}

void SurfaceMeshRenderShaded::colormap_set_range(Scalar min, Scalar max){
    _colormap_min = min;
    _colormap_max = max;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
