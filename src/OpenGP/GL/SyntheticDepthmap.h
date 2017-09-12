#pragma once

#include <vector>
#include <list>
#include <OpenGP/types.h>

#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>
#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/GL/Framebuffer.h>
#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/check_error_gl.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class DepthmapRenderer {
public:

    virtual void draw() = 0;

    virtual Shader &get_shader() = 0;

};

class SyntheticDepthmap {
private:

    RGB32FTexture points;
    D16Texture depth;

    Framebuffer framebuffer;

    float *data;

    GLsizei width, height;

public:

    SyntheticDepthmap(GLsizei width, GLsizei height) : width(width), height(height) {

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

    void render(DepthmapRenderer &renderer, const Eigen::Matrix4f &M, const Eigen::Matrix4f &V, const Eigen::Matrix4f &P) {

        glViewport(0, 0, width, height);

        framebuffer.bind();
        renderer.get_shader().bind();

        renderer.get_shader().set_uniform("M", M);
        renderer.get_shader().set_uniform("V", V);
        renderer.get_shader().set_uniform("P", P);

        renderer.draw();

        renderer.get_shader().unbind();
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
