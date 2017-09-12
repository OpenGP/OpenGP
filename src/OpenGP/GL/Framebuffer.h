#pragma once
#include <OpenGP/GL/gl.h>

#include <OpenGP/GL/Texture.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class Framebuffer{
private:

    GLuint fbo;

public:

    Framebuffer() {
        glGenFramebuffers(1, &fbo);
    }

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer &operator=(const Framebuffer&) = delete;

    ~Framebuffer() {
        glDeleteFramebuffers(1, &fbo);
    }

    void attach_color_texture(GenericTexture &texture, int attachment = 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texture.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void attach_depth_texture(GenericTexture &texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
