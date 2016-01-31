#pragma once
#include <vector>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderShaded : public SceneObject{
private:
    SurfaceMesh& mesh;
    VertexArrayObject vao;    
    ArrayBuffer<Vec3> v_buffer;
    ArrayBuffer<Vec3> n_buffer;
    ArrayBuffer<float> q_buffer;
    ElementArrayBuffer<uint> i_buffer;
    GLuint _tex; ///< Colormap Texture ID
    
public:
    SurfaceMeshRenderShaded(SurfaceMesh& mesh) : mesh(mesh){}
    HEADERONLY_INLINE void init();
    HEADERONLY_INLINE void display();
    HEADERONLY_INLINE Box3 bounding_box();
    
/// @{ color quality mapping
private:
    bool _use_colormap = false;
    Scalar _colormap_min = 0.0f;
    Scalar _colormap_max = 1.0f;
public:
    HEADERONLY_INLINE void colormap_enabled(bool);
    HEADERONLY_INLINE void colormap_set_range(Scalar min, Scalar max);
/// @}
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "SurfaceMeshRenderShaded.cpp"
#endif
