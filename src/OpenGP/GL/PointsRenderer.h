#pragma once
#include <OpenGP/types.h>
#include <OpenGP/headeronly.h>
#include <OpenGP/GL/SceneObject.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class PointsRenderer : public SceneObject{
protected:
    VertexArrayObject _vao;  
    MatMxN _data; ///< reference to data to be rendered
    ArrayBuffer<Vec3> _buffer_vpos;   ///< per-vertex position
    ArrayBuffer<Vec3> _buffer_vcolor; ///< per-vertex color (optional)

/// @{ constructors
public:
    PointsRenderer(){}
    PointsRenderer(const MatMxN& data){ load(data); }
protected:
    void load(const MatMxN& data){ _data=data; }
/// @}
    
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
    HEADERONLY_INLINE void init();  
    HEADERONLY_INLINE void display();
    HEADERONLY_INLINE Box3 bounding_box();
    
    /// Sets the per-point color to the given values
    HEADERONLY_INLINE void set_colors(const MatMxN& M);
    
    /// Sets the size of splats to a value (same across points)
    HEADERONLY_INLINE void set_vsize(float val);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "PointsRenderer.cpp"
#endif
