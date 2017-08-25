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
    //ArrayBuffer<Vec2> vtexcoord;

    ElementArrayBuffer<unsigned int> triangles;

public:

    GPUMesh(const SurfaceMesh &mesh) {

        vao.bind();

        auto vpoints = mesh.get_vertex_property<Vec3>("v:point");
        vpoint.upload_raw(vpoints.data(), mesh.n_vertices());

        auto vnormals = mesh.get_vertex_property<Vec3>("v:normal");
        vnormal.upload_raw(vnormals.data(), mesh.n_vertices());

        std::vector<unsigned int> triangles;
        for(auto f: mesh.faces())
            for(auto v: mesh.vertices(f))
                triangles.push_back(v.idx());
        this->triangles.upload(triangles);

        vao.unbind();

    }

    void draw(Shader &shader) {

        vao.bind();

        shader.set_attribute("vposition", vpoint);
        shader.set_attribute("vnormal", vnormal);



        //shader.set_uniform

        vao.unbind();

    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
