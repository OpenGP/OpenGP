#pragma once

#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/GL/Texture.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class FullscreenQuad : public GPUMesh {
private:

    const char* vshader = R"GLSL(
        #version 330 core

        in vec3 vposition;
        out vec2 uv;

        void main(){
            gl_Position = vec4(vposition, 1);
            uv = vec2((vposition.x + 1) / 2, (vposition.y + 1) / 2);
        }
    )GLSL";

    const char* fshader = R"GLSL(
        #version 330 core

        uniform sampler2D tex;

        in vec2 uv;
        out vec4 fcolor;

        void main(){
            fcolor = texture2D(tex, uv);
        }
    )GLSL";

    Shader default_shader;

public:

    FullscreenQuad() {

        default_shader.add_vshader_from_source(vshader);
        default_shader.add_fshader_from_source(fshader);
        default_shader.link();

        std::vector<Vec3> vpoint;
        vpoint.push_back(Vec3(1, 1, 0));
        vpoint.push_back(Vec3(-1, 1, 0));
        vpoint.push_back(Vec3(1, -1, 0));
        vpoint.push_back(Vec3(-1, -1, 0));

        set_vpoint(vpoint);

        std::vector<unsigned int> triangles;
        triangles.push_back(0);
        triangles.push_back(3);
        triangles.push_back(1);
        triangles.push_back(0);
        triangles.push_back(2);
        triangles.push_back(3);

        set_triangles(triangles);

        default_shader.bind();

        set_attributes(default_shader);
        default_shader.set_uniform("tex", 0);

        default_shader.unbind();

        set_mode(GL_TRIANGLES);

    }

    virtual ~FullscreenQuad() {}

    void draw_texture(GenericTexture &texture) {

        default_shader.bind();

        glActiveTexture(GL_TEXTURE0); // is this needed?
        texture.bind();

        draw();

        texture.unbind();

        default_shader.unbind();

    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
