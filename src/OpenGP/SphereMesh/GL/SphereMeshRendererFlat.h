#pragma once

#include <OpenGP/SphereMesh/SphereMesh.h>
#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/SphereMesh/bounding_box.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SphereMeshRendererFlat : public OpenGP::SceneObject {
private:

    const GLchar* vshader = R"GLSL(
        #version 330 core

        uniform mat4 M;
        uniform mat4 MV;
        uniform mat4 MVP;

        in vec3 vposition;   ///< per-vertex position
        in vec3 vnormal;     ///< per-vertex normal

        out vec3 fnormal;    ///< per-fragment normal

        void main(){
            gl_Position = MVP * vec4(vposition, 1.0);
            fnormal = normalize( inverse(transpose(mat3(MV))) * vnormal );
        }
    )GLSL";

    const char* fshader = R"GLSL(
        #version 330 core

        in vec3 fnormal;

        out vec4 fcolor;

        void main(){
            ///--- Face flat shading
            vec3 LDIR = vec3(0,0,1);
            vec3 basecolor = vec3(1,0,0);
            vec3 ldir = normalize(LDIR);
            float albedo = max( dot( normalize(fnormal), ldir ), 0 );
            fcolor = vec4(basecolor*albedo, 1);
        }
    )GLSL";

    const int segments = 32;

    SphereMesh &mesh;

    GPUMesh sphere, cone, triangle;

public:

    SphereMeshRendererFlat(SphereMesh &mesh) : mesh(mesh) {

        { // Sphere generation
            std::vector<Vec3> vpoint;
            std::vector<Vec3> vnormal;
            std::vector<unsigned int> triangles;

            for (int i = 0;i < segments;i++) {
                for (int j = 0;j < segments;j++) {
                    Scalar lon = 2 * M_PI * ((Scalar)i) / segments;
                    Scalar lat = M_PI * (((Scalar)j) / segments - 0.5);
                    Vec3 vert(std::cos(lon) * std::cos(lat), std::sin(lon) * std::cos(lat), std::sin(lat));
                    vpoint.push_back(vert);
                    vnormal.push_back(vert);
                    if (i > 0 && j > 0) {
                        int v0 = j + i * segments;
                        int v1 = v0 - 1;
                        int v2 = v0 - segments;
                        int v3 = v1 - segments;
                        triangles.push_back(v0);
                        triangles.push_back(v1);
                        triangles.push_back(v2);
                        triangles.push_back(v0);
                        triangles.push_back(v2);
                        triangles.push_back(v3);
                    }
                }
            }

            sphere.set_vpoint(vpoint);
            sphere.set_vnormal(vnormal);
            sphere.set_triangles(triangles);
        }

    }

    void init() {

        program.add_vshader_from_source(vshader);
        program.add_fshader_from_source(fshader);
        program.link();

    }

    void display() {

        auto vpoint = mesh.get_vertex_property<Vec4>("v:point");

        sphere.set_attributes(program);

        for (auto v : mesh.vertices()) {
            Vec4 point = vpoint[v];

            Mat4x4 t = Eigen::Affine3f(Eigen::Translation3f(point.block<3, 1>(0, 0))).matrix();
            Mat4x4 s = Mat4x4::Zero();
            s(0, 0) = s(1, 1) = s(2, 2) = point(3);

            Mat4x4 M = model * t * s;

            program.set_uniform("M", M);

            sphere.draw();

        }

    }

    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
