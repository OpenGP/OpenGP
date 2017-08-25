#pragma once

#include <OpenGP/types.h>

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class FullscreenQuad {
private:

    VertexArrayObject vao;

    ArrayBuffer<Vec3> vpoint;

    Shader shader;

public:

    FullscreenQuad() {

        const GLfloat verts[] = {
            -1, -1, 0,
             1, -1, 0,
            -1,  1, 0,
             1,  1, 0
        };

        vao.bind();
        vpoint.bind();

        

        vpoint.unbind();
        vao.unbind();

    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
