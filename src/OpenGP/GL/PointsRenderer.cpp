#include <OpenGP/types.h>
#include <OpenGP/GL/PointsRenderer.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

void PointsRenderer::init(){
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

void PointsRenderer::display(){
    if (_data.cols()==0) return;
    _vao.bind();
    program.bind();
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, _data.cols());
        glDisable(GL_PROGRAM_POINT_SIZE);
    program.release();
    _vao.release();
}

Box3 PointsRenderer::bounding_box(){ 
    /// TODO: test
    Vec3 _min = _data.rowwise().minCoeff();
    Vec3 _max = _data.rowwise().maxCoeff();
    return Box3(_min, _max);
}

void PointsRenderer::set_colors(const MatMxN &M){
    CHECK(program.is_valid());
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

void PointsRenderer::set_vsize(float val){
    CHECK(program.is_valid());
    program.bind();
        program.set_attribute("vsize", val);
    program.release();
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
