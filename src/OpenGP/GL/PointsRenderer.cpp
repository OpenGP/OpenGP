#include <OpenGP/types.h>
#include <OpenGP/GL/PointsRenderer.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

const static GLchar* PointsRenderer_vshader = R"GLSL(
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

const static char* PointsRenderer_fshader = R"GLSL(
    #version 330
    in vec3 fcolor;
    out vec4 color;
    void main(){ color = vec4(fcolor,1); }        
)GLSL";

void PointsRenderer::init() {
    ///--- Shader
    program.add_vshader_from_source(PointsRenderer_vshader);
    program.add_fshader_from_source(PointsRenderer_fshader);
    program.link();

    ///--- Data
    _buffer_vpos.upload_raw(_data.data(), _data.cols());

    ///--- Attributes
    program.bind();
    _vao.bind();
    program.set_attribute("vpoint", _buffer_vpos);
    program.set_attribute("vcolor", color); ///< default use object color
    program.set_attribute("vsize", 3.0f); ///< default point size
    _vao.release();
    program.release();
}

void PointsRenderer::display() {
    if (_data.cols()==0) return;
    _vao.bind();
    program.bind();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, _data.cols());
    glDisable(GL_PROGRAM_POINT_SIZE);
    program.release();
    _vao.release();
}

Box3 PointsRenderer::bounding_box() {
    /// TODO: test
    Vec3 _min = _data.rowwise().minCoeff();
    Vec3 _max = _data.rowwise().maxCoeff();
    return Box3(_min, _max);
}

void PointsRenderer::init_data(MatMxN& data) {
    _data = data;
    _buffer_vpos.upload_raw(_data.data(), data.cols());
}

void PointsRenderer::set_colors(const MatMxN& M) {
    CHECK(program.is_valid());
    CHECK(M.rows()==_data.rows());
    CHECK(M.cols()==_data.cols());

    ///--- Upload data
    _buffer_vcolor.upload_raw(M.data(), M.cols());
    ///--- Set attribute
    program.bind();
    _vao.bind();
    program.set_attribute("vcolor", _buffer_vcolor);
    _vao.release();
    program.release();
}

void PointsRenderer::set_vsize(float val) {
    CHECK(program.is_valid());
    program.bind();
    program.set_attribute("vsize", val);
    program.release();
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
