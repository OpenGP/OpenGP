#pragma once
#include <OpenGP/types.h>
#include <OpenGP/GL/SceneObject.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class PointCloudRenderer : public SceneObject{
    VertexArrayObject _vao;  
    MatMxN _data; ///< reference to data to be rendered
    ArrayBuffer<Vec3> _buffer_vpos;   ///< per-vertex position
    ArrayBuffer<Vec3> _buffer_vcolor; ///< per-vertex color (optional)

    const GLchar* vshader = R"GLSL( 
        #version 330
        uniform mat4 M;    
        uniform mat4 MV; 
        uniform mat4 MVP; 
        in vec3 vpoint;
        in vec3 vcolor;
        in float vsize;
        out vec3 fcolor;
        void main() {  
            gl_Position = MVP * vec4(vpoint, 1.0);
            gl_PointSize = vsize;
            fcolor = vcolor;
        }
    )GLSL";
        
    const char* fshader = R"GLSL(
        #version 330
        in vec3 fcolor;
        out vec4 color;
        void main(){ color = vec4(fcolor,1); }        
    )GLSL";

public:
    PointCloudRenderer(const MatMxN& data) : _data(data){}
        
    void init(){
        ///--- Shader
        program.add_vshader_from_source(vshader);
        program.add_fshader_from_source(fshader);
        program.link();

        ///--- Data
        _buffer_vpos.upload(_data.data(), _data.cols(), sizeof(Vec3));
        
        ///--- Attributes
        program.bind();
        _vao.bind();
            program.set_attribute("vpoint", _buffer_vpos);  
            program.set_attribute("vcolor", color); ///< default use object color
            program.set_attribute("vsize", 3.0f); ///< default point size
        _vao.release();
        program.release();        
    }
        
    /// Sets the per-point color to the given values
    void set_colors(const MatMxN& M){
        CHECK(M.rows()==_data.rows());
        CHECK(M.cols()==_data.cols());
        
        ///--- Upload data
        _buffer_vcolor.upload(M.data(), M.cols(), M.rows()*sizeof(Scalar) );
        ///--- Set attribute
        program.bind();
        _vao.bind();
            program.set_attribute("vcolor", _buffer_vcolor);
        _vao.release();
        program.release();        
    }
        
    /// Sets the size of splats to a value (same across points)
    void set_vsize(float val){
        program.bind();
            program.set_attribute("vsize", val);
        program.release();
    }
    
    void display(){
        if (_data.cols()==0) return;
        _vao.bind();
        program.bind();
            glEnable(GL_PROGRAM_POINT_SIZE);
                glDrawArrays(GL_POINTS, 0, _data.cols());
            glDisable(GL_PROGRAM_POINT_SIZE);
        program.release();
        _vao.release();
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
