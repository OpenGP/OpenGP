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


//=============================================================================
namespace OpenGP {
//=============================================================================

class SyntheticDepthmap {
private:

    RGB32FTexture points;
    D16Texture depth;

    Framebuffer framebuffer;

    struct MeshGPUData {
        VertexArrayObject vao;
        ArrayBuffer<Vec3> vpos;
    };

    std::list<MeshGPUData> meshes;

    Shader shader;

    float *data;

public:

    SyntheticDepthmap(GLsizei width, GLsizei height) {

        points.allocate(width, height);
        depth.allocate(width, height);

        framebuffer.attach_color_texture(points);
        framebuffer.attach_depth_texture(depth);

    }

    void clear() {

        framebuffer.bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        framebuffer.unbind();

    }

    void render(const GPUMesh &mesh, const Eigen::Matrix4f &matrix) {

        shader.bind();



        shader.unbind();

    }

    void download() {

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB32F, GL_FLOAT, data);

    }

    const float *cpu_data() { return data; }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
