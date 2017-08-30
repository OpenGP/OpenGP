#pragma once
#include <OpenGP/GL/gl.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class GenericTexture {
protected:

    GLuint _id;

public:

    GenericTexture() {
        glGenTextures(1, &_id);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unbind();
    }

    GenericTexture(const GenericTexture&) = delete;
    GenericTexture &operator=(const GenericTexture&) = delete;

    virtual ~GenericTexture() {
        glGenTextures(1, &_id);
    }

    void bind() const { glBindTexture(GL_TEXTURE_2D, _id); }
    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

    void enable_filter() {
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unbind();
    }

    void disable_filter() {
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        unbind();
    }

    GLuint id() const { return _id; }

};

template <GLenum INTERNAL_FORMAT, GLenum FORMAT, GLenum TYPE>
class Texture : public GenericTexture {
public:

    void allocate(GLsizei width, GLsizei height) {

        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT, width, height, 0, FORMAT, TYPE, nullptr);

        unbind();

    }

};

using RGBA8Texture = Texture<GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE>;
using RGB8Texture = Texture<GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE>;
using RGB32FTexture = Texture<GL_RGB32F, GL_RGB, GL_FLOAT>;
using R32Texture = Texture<GL_R32F, GL_RED, GL_FLOAT>;
using D16Texture = Texture<GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT>;

//=============================================================================
} // namespace OpenGP
//=============================================================================
