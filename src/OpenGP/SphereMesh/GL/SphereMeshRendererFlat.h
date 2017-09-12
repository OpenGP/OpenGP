#pragma once

#include <iostream>

#include <OpenGP/SphereMesh/SphereMesh.h>
#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/SphereMesh/bounding_box.h>
#include <OpenGP/SphereMesh/helpers.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SphereMeshRendererFlat : public OpenGP::SceneObject {
private:

    const GLchar* vshader = R"GLSL(
        #version 330 core

        uniform float ratio;
        uniform float nrotate;

        uniform vec3 v0;
        uniform vec3 v1;
        uniform vec3 v2;
        uniform vec3 vn;
        uniform float vweight;

        uniform mat4 M;
        uniform mat4 MV;
        uniform mat4 MVP;

        in vec3 vposition;   ///< per-vertex position
        in vec3 vnormal;     ///< per-vertex normal

        out vec3 fnormal;    ///< per-fragment normal

        /// http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
        mat4 rotationMatrix(vec3 axis, float angle) {
            axis = normalize(axis);
            float s = sin(angle);
            float c = cos(angle);
            float oc = 1.0 - c;

            return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                        0.0,                                0.0,                                0.0,                                1.0);
        }

        void main(){
            vec3 vpos_prime = vposition;
            vpos_prime.x *= 1 + vpos_prime.z * (ratio - 1);
            vpos_prime.y *= 1 + vpos_prime.z * (ratio - 1);

            vec3 tpos = vpos_prime.x * v0 + vpos_prime.y * v1 + vpos_prime.z * v2;
            vpos_prime = mix(vpos_prime, tpos, vweight);

            gl_Position = MVP * vec4(vpos_prime, 1.0);

            vec3 vnorm_prime = mix(vnormal, vn, vweight);
            vec3 axis = cross(vnorm_prime, vec3(0, 0, 1));
            mat4 rot = rotationMatrix(axis, nrotate);
            vnorm_prime = (rot * vec4(vnorm_prime, 1)).xyz;
            fnormal = normalize( inverse(transpose(mat3(MV))) * vnorm_prime );
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

    const float PI = 3.141592653589793238462643383;

    const int segments = 64;

    SphereMesh &mesh;

    GPUMesh sphere, cone, triangle;

public:

    SphereMeshRendererFlat(SphereMesh &mesh) : mesh(mesh) {

        { // Sphere generation
            std::vector<Vec3> vpoint;
            std::vector<Vec3> vnormal;
            std::vector<unsigned int> triangles;

            int n = 0;
            for (int i = 0;i <= segments;i++) {
                for (int j = 0;j <= segments;j++) {
                    Scalar lon = 2 * PI * ((Scalar)i) / segments;
                    Scalar lat = PI * (((Scalar)j) / segments - 0.5);
                    Vec3 vert(std::cos(lon) * std::cos(lat), std::sin(lon) * std::cos(lat), std::sin(lat));
                    vpoint.push_back(vert);
                    vnormal.push_back(vert);
                    if (i > 0 && j > 0) {
                        int v0 = n;
                        int v1 = v0 - 1;
                        int v2 = v0 - segments - 1;
                        int v3 = v1 - segments - 1;
                        triangles.push_back(v0);
                        triangles.push_back(v1);
                        triangles.push_back(v2);
                        triangles.push_back(v1);
                        triangles.push_back(v2);
                        triangles.push_back(v3);
                    }
                    n++;
                }
            }

            sphere.set_vpoint(vpoint);
            sphere.set_vnormal(vnormal);
            sphere.set_triangles(triangles);
        }

        { // Cone generation
            std::vector<Vec3> vpoint;
            std::vector<Vec3> vnormal;
            std::vector<unsigned int> triangles;

            for (int i = 0;i <= segments;i++) {
                Scalar theta = 2 * PI * ((Scalar)i) / segments;
                Vec3 vert(std::cos(theta), std::sin(theta), 0);
                vpoint.push_back(vert);
                vnormal.push_back(vert);
                vpoint.push_back(vert + Vec3(0, 0, 1));
                vnormal.push_back(vert);
                if (i > 0) {
                    int v0 = 1 + 2 * i;
                    int v1 = v0 - 1;
                    int v2 = v0 - 2;
                    int v3 = v0 - 3;
                    triangles.push_back(v0);
                    triangles.push_back(v1);
                    triangles.push_back(v2);
                    triangles.push_back(v1);
                    triangles.push_back(v2);
                    triangles.push_back(v3);
                }
            }

            cone.set_vpoint(vpoint);
            cone.set_vnormal(vnormal);
            cone.set_triangles(triangles);
        }

        { // Triangle generation
            std::vector<Vec3> vpoint;
            std::vector<Vec3> vnormal;
            std::vector<unsigned int> triangles;

            vpoint.push_back(Vec3(1, 0, 0));
            vnormal.push_back(Vec3(0, 0, 1));

            vpoint.push_back(Vec3(0, 1, 0));
            vnormal.push_back(Vec3(0, 0, 1));

            vpoint.push_back(Vec3(0, 0, 1));
            vnormal.push_back(Vec3(0, 0, 1));

            triangles.push_back(0);
            triangles.push_back(1);
            triangles.push_back(2);

            triangle.set_vpoint(vpoint);
            triangle.set_vnormal(vnormal);
            triangle.set_triangles(triangles);
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

        Mat4x4 MV, MVP, V, VP;
        program.get_uniform("MV", MV);
        program.get_uniform("MVP", MVP);
        V = MV * model.inverse();
        VP = MVP * model.inverse();

        auto draw_sphere = [&](SphereMesh::Vertex v){

            Vec4 point = vpoint[v];

            Mat4x4 t = Eigen::Affine3f(Eigen::Translation3f(point.block<3, 1>(0, 0))).matrix();
            Mat4x4 s = Mat4x4::Identity();
            s(0, 0) = s(1, 1) = s(2, 2) = point(3);

            Mat4x4 M = model * t * s;

            program.set_uniform("M", M);
            program.set_uniform("MV", Mat4x4(V * M));
            program.set_uniform("MVP", Mat4x4(VP * M));
            program.set_uniform("ratio", 1.0f);
            program.set_uniform("vweight", 0.0f);
            program.set_uniform("nrotate", 0.0f);

            sphere.draw();

        };

        for (auto s : mesh.spheres()) {
            draw_sphere(mesh.vertex(s));
        }

        for (auto e : mesh.edges()) {
            draw_sphere(mesh.vertex(e, 0));
            draw_sphere(mesh.vertex(e, 1));
        }

        for (auto f : mesh.faces()) {
            draw_sphere(mesh.vertex(f, 0));
            draw_sphere(mesh.vertex(f, 1));
            draw_sphere(mesh.vertex(f, 2));
        }

        cone.set_attributes(program);

        auto draw_cone = [&](SphereMesh::Vertex v0, SphereMesh::Vertex v1) {

            Vec4 s0 = vpoint[v0];
            Vec4 s1 = vpoint[v1];
            Vec3 p0 = s0.block<3, 1>(0, 0);
            Vec3 p1 = s1.block<3, 1>(0, 0);
            Scalar r0 = s0(3);
            Scalar r1 = s1(3);

            Vec2 tangent = pill_tangent(s0, s1);
            Scalar beta = tangent(0);
            Scalar alpha = tangent(1);

            Scalar angle = std::acos(Vec3(0, 0, 1).dot((p1 - p0).normalized()));
            Vec3 axis = Vec3(0, 0, 1).cross(p1 - p0).normalized();
            Mat4x4 r = Eigen::Affine3f(Eigen::AngleAxis<Scalar>(angle, axis)).matrix();

            Mat4x4 s = Mat4x4::Identity();
            Vec3 o;
            s(0, 0) = s(1, 1) = r0 * beta;
            if (r1 > r0) {
                s(2, 2) = (p1 - p0).norm() + alpha * (r0 - r1);
                o = -alpha * r0 * (p1 - p0).normalized();
            } else {
                s(2, 2) = (p1 - p0).norm() + alpha * (r1 - r0);
                o = alpha * r0 * (p1 - p0).normalized();
            }

            Mat4x4 t = Eigen::Affine3f(Eigen::Translation3f(p0 + o)).matrix();

            Scalar ratio = r1 / r0;
            Scalar nrotate = std::atan2(beta * (r1 - r0), s(2, 2));

            Mat4x4 M = model * t * r * s;

            program.set_uniform("M", M);
            program.set_uniform("MV", Mat4x4(V * M));
            program.set_uniform("MVP", Mat4x4(VP * M));
            program.set_uniform("ratio", ratio);
            program.set_uniform("vweight", 0.0f);
            program.set_uniform("nrotate", nrotate);

            cone.draw();

        };

        for (auto e : mesh.edges()) {
            draw_cone(mesh.vertex(e, 0), mesh.vertex(e, 1));
        }

        for (auto f : mesh.faces()) {
            draw_cone(mesh.vertex(f, 0), mesh.vertex(f, 1));
            draw_cone(mesh.vertex(f, 1), mesh.vertex(f, 2));
            draw_cone(mesh.vertex(f, 2), mesh.vertex(f, 0));
        }

        triangle.set_attributes(program);

        auto draw_triangles = [&](SphereMesh::Vertex v0, SphereMesh::Vertex v1, SphereMesh::Vertex v2) {

            Vec4 s0 = vpoint[v0];
            Vec4 s1 = vpoint[v1];
            Vec4 s2 = vpoint[v2];
            Vec3 p0 = s0.block<3, 1>(0, 0);
            Vec3 p1 = s1.block<3, 1>(0, 0);
            Vec3 p2 = s2.block<3, 1>(0, 0);
            Scalar r0 = s0(3);
            Scalar r1 = s1(3);
            Scalar r2 = s2(3);

            Vec3 sn = (p2 - p0).cross(p1 - p0).normalized();

            Vec2 ta, tb;
            Vec3 ms_tangent_a, ms_tangent_b, tn;
            ta = pill_tangent(s0, s1);
            tb = pill_tangent(s0, s2);
            ms_tangent_a = ta(0) * (p1 - p0).normalized() - ta(1) * sn;
            ms_tangent_b = tb(0) * (p2 - p0).normalized() - tb(1) * sn;
            tn = ms_tangent_a.cross(ms_tangent_b).normalized();

            // tangent point 0
            Vec3 t0 = p0 + r0 * tn;

            // tangent point 1
            Vec3 t1 = p1 + r1 * tn;

            // tangent point 2
            Vec3 t2 = p2 + r2 * tn;

            Mat4x4 M = Mat4x4::Identity();

            program.set_uniform("M", M);
            program.set_uniform("MV", Mat4x4(V * M));
            program.set_uniform("MVP", Mat4x4(VP * M));
            program.set_uniform("ratio", 1.0f);
            program.set_uniform("v0", t0);
            program.set_uniform("v1", t1);
            program.set_uniform("v2", t2);
            program.set_uniform("vn", tn);
            program.set_uniform("vweight", 1.0f);
            program.set_uniform("nrotate", 0.0f);

            triangle.draw();

            ms_tangent_a = ta(0) * (p1 - p0).normalized() + ta(1) * sn;
            ms_tangent_b = tb(0) * (p2 - p0).normalized() + tb(1) * sn;
            tn = ms_tangent_b.cross(ms_tangent_a).normalized();

            // tangent point 0
            t0 = p0 + r0 * tn;

            // tangent point 1
            t1 = p1 + r1 * tn;

            // tangent point 2
            t2 = p2 + r2 * tn;

            program.set_uniform("v0", t0);
            program.set_uniform("v1", t1);
            program.set_uniform("v2", t2);
            program.set_uniform("vn", tn);

            triangle.draw();

        };

        for (auto f : mesh.faces()) {
            draw_triangles(mesh.vertex(f, 0), mesh.vertex(f, 1), mesh.vertex(f, 2));
        }

    }

    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
