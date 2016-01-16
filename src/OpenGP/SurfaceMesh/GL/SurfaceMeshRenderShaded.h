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
    ElementArrayBuffer<uint> i_buffer;
    
public:
    SurfaceMeshRenderShaded(SurfaceMesh& mesh) : mesh(mesh){}
    HEADERONLY_INLINE void init();
    HEADERONLY_INLINE void display();
};
    
//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "SurfaceMeshRenderShaded.cpp"
#endif
