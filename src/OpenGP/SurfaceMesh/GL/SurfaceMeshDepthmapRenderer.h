#pragma once

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/GL/SyntheticDepthmap.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshDepthmapRenderer : public DepthmapRenderer {
private:

    const char* vshader = R"GLSL(
        #version 330 core

        uniform mat4 M;
        uniform mat4 V;
        uniform mat4 P;

        in vec3 vposition;
        out vec3 fpos;

        void main(){
            vec4 pos = V * M * vec4(vposition, 1);
            fpos = pos.xyz;

            gl_Position = P * pos;
        }
    )GLSL";

    const char* fshader = R"GLSL(
        #version 330 core

        in vec3 fpos;
        out vec4 fcolor;

        void main(){
            fcolor = vec4(fpos, 1);
        }
    )GLSL";

    Shader shader;

    const SurfaceMesh &mesh;

    GPUMesh gpu_mesh;

public:

    SurfaceMeshDepthmapRenderer(const SurfaceMesh &mesh) : mesh(mesh) {

        shader.add_vshader_from_source(vshader);
        shader.add_fshader_from_source(fshader);
        shader.link();

    }

    void init() {

        gpu_mesh.init_from_mesh(mesh);

    }

    void draw() {

        gpu_mesh.set_attributes(shader);

        gpu_mesh.draw();

    }

    Shader &get_shader() {
        return shader;
    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
