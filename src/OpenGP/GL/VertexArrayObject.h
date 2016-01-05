#pragma once
#include <OpenGP/GL/gl.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class VertexArrayObject{
    GLuint VAO;
public:
    VertexArrayObject(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    ~VertexArrayObject(){ glDeleteVertexArrays(1, &VAO); } 
    void bind(){ glBindVertexArray(VAO); }
    void release(){ glBindVertexArray(0); }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
