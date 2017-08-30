#pragma once

#include <vector>
#include <list>
#include <OpenGP/types.h>

#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>
#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/GL/Framebuffer.h>
#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/GL/check_error_gl.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SyntheticDepthmap {
private:

    const char* vshader = R"GLSL(
        #version 330 core

        uniform mat4 MV;
        uniform mat4 P;

        in vec3 vposition;
        out vec3 fpos;

        void main(){
            vec4 pos = MV * vec4(vposition, 1);
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

    RGB32FTexture points;
    D16Texture depth;

    Framebuffer framebuffer;

    Shader shader;

    float *data;

    GLsizei width, height;

public:

    SyntheticDepthmap(GLsizei width, GLsizei height) : width(width), height(height) {

        shader.add_vshader_from_source(vshader);
        shader.add_fshader_from_source(fshader);
        shader.link();

        points.allocate(width, height);
        depth.allocate(width, height);

        framebuffer.attach_color_texture(points);
        framebuffer.attach_depth_texture(depth);

    }

    void clear() {

        framebuffer.bind();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        framebuffer.unbind();

    }

    void render(GPUMesh &mesh, const Eigen::Matrix4f &MV, const Eigen::Matrix4f &P) {

        glViewport(0, 0, width, height);

        framebuffer.bind();
        shader.bind();

        shader.set_uniform("MV", MV);
        shader.set_uniform("P", P);

        mesh.set_attributes(shader);

        mesh.draw();

        shader.unbind();
        framebuffer.unbind();

    }

    void download() {

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB32F, GL_FLOAT, data);

    }

    RGB32FTexture &get_texture() {

        return points;

    }

    const float *cpu_data() const { return data; }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
