#pragma once

#include <OpenGP/GL/gl.h>
#include <OpenGP/GL/Buffer.h>
#include <OpenGP/GL/VertexArrayObject.h>

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class GPUMesh {
private:

    VertexArrayObject vao;

    ArrayBuffer<Vec3> vpoint;
    ArrayBuffer<Vec3> vnormal;
    ArrayBuffer<Vec2> vtexcoord;

    ElementArrayBuffer<unsigned int> triangles;

    int element_count = 0;

    GLenum mode = GL_TRIANGLES;

public:

    GPUMesh() {};

    GPUMesh(const SurfaceMesh &mesh) {

        init_from_mesh(mesh);

    }

    virtual ~GPUMesh() {}

    void init_from_mesh(const SurfaceMesh &mesh) {

        vao.bind();

        auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
        vpoint.upload_raw(vpoints.data(), mesh.n_vertices());

        auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
        if (vnormals) {
            vnormal.upload_raw(vnormals.data(), mesh.n_vertices());
        }

        element_count = 0;
        std::vector<unsigned int> triangles;
        for(auto f: mesh.faces()) {
            for(auto v: mesh.vertices(f)) {
                element_count++;
                triangles.push_back(v.idx());
            }
        }
        this->triangles.upload(triangles);

        vao.unbind();

        mode = GL_TRIANGLES;

    }

    void set_mode(GLenum mode) {
        this->mode = mode;
    }

    void set_vpoint(const std::vector<Vec3> &vpoint) {
        vao.bind();
        this->vpoint.upload(vpoint);
        vao.unbind();
    }

    void set_vnormal(const std::vector<Vec3> &vnormal) {
        vao.bind();
        this->vnormal.upload(vnormal);
        vao.unbind();
    }

    void set_vnormal(const std::vector<Vec2> &vtexcoord) {
        vao.bind();
        this->vtexcoord.upload(vtexcoord);
        vao.unbind();
    }

    void set_triangles(const std::vector<unsigned int> &triangles) {
        vao.bind();
        this->triangles.upload(triangles);
        vao.unbind();
        element_count = triangles.size();
    }

    void set_attributes(Shader &shader) {

        vao.bind();

        if (shader.has_attribute("vposition")) {
            shader.set_attribute("vposition", vpoint);
        }

        if (shader.has_attribute("vnormal")) {
            shader.set_attribute("vnormal", vnormal);
        }

        if (shader.has_attribute("vtexcoord")) {
            shader.set_attribute("vtexcoord", vtexcoord);
        }

        vao.unbind();

    }

    void draw() {

        vao.bind();
        triangles.bind();

        glDrawElements(mode, element_count, GL_UNSIGNED_INT, 0);

        triangles.unbind();
        vao.unbind();

    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
