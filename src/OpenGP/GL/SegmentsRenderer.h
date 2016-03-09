#pragma once
#include <OpenGP/types.h>
#include <OpenGP/headeronly.h>
#include <OpenGP/GL/SceneObject.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SegmentsRenderer : public SceneObject{
public:
    typedef std::vector<std::pair<Vec3, Vec3>> Segments;
protected:
    VertexArrayObject _vao;           ///< OpenGL object storing data/attributes
    MatMxN _data;                     ///< reference to data to be rendered  
    ArrayBuffer<Vec3> _buffer_vpos;   ///< per-vertex position
    ArrayBuffer<Vec3> _buffer_vcolor; ///< per-vertex color (optional)

/// @{ constructors
public:
    SegmentsRenderer(){}
    SegmentsRenderer(const Segments& segments){ load(segments); }
    SegmentsRenderer(const MatMxN& P1, const MatMxN& P2){ load(P1,P2); }
protected:
    HEADERONLY_INLINE void load(const MatMxN& P1, const MatMxN& P2);
    HEADERONLY_INLINE void load(const Segments& segments);
/// @} 

public:        
    HEADERONLY_INLINE void init();
    HEADERONLY_INLINE void display();
    HEADERONLY_INLINE Box3 bounding_box();

protected:
    const GLchar* vshader = R"GLSL( 
        #version 330
        uniform mat4 M;    
        uniform mat4 MV; 
        uniform mat4 MVP;
        in vec3 vpoint;
        in vec3 vcolor;
        out vec3 fcolor;
        void main() {  
            gl_Position = MVP * vec4(vpoint, 1.0);
            fcolor = vcolor;
        }
    )GLSL";
        
    const char* fshader = R"GLSL(
        #version 330
        in vec3 fcolor;
        out vec4 color;
        void main(){ color = vec4(fcolor,1); }        
    )GLSL";
};

//=============================================================================

void SegmentsRenderer::load(const MatMxN &P1, const MatMxN &P2){ 
    CHECK(P1.cols() == P2.cols());
    CHECK((P1.rows() == 3) && (P2.rows() == 3));
    _data.resize( 3, 2*P1.cols() );
    for(int i=0; i<P1.cols(); i++){
        _data.col(i*2+0) = P1.col(i);
        _data.col(i*2+1) = P2.col(i);
    }
}

void SegmentsRenderer::load(const SegmentsRenderer::Segments &segments){
    _data.resize( 3, 2*segments.size() );
    for(uint i=0; i<segments.size(); i++){
        _data.col(i*2+0) = segments[i].first;
        _data.col(i*2+1) = segments[i].second;
    }
}

void SegmentsRenderer::init(){
    ///--- Shader
    program.add_vshader_from_source(vshader);
    program.add_fshader_from_source(fshader);
    program.link();
    
    ///--- Data
    _buffer_vpos.upload_raw(_data.data(), _data.cols());
    
    ///--- Attributes
    program.bind();
    _vao.bind();
    program.set_attribute("vpoint", _buffer_vpos);  
    program.set_attribute("vcolor", color); ///< default use object color
    _vao.release();
    program.release();        
}

void SegmentsRenderer::display(){
    if (_data.cols()==0) return;
    _vao.bind();
    program.bind();
    glDrawArrays(GL_LINES, 0, _data.cols());
    program.release();
    _vao.release();
}

Box3 SegmentsRenderer::bounding_box(){
    Box3 box;
    Vec3 _min = _data.rowwise().minCoeff();
    Vec3 _max = _data.rowwise().maxCoeff();
    return Box3(_min, _max);
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
