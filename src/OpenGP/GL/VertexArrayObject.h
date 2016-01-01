#pragma once
#include <OpenGP/GL/glfw_helpers.h> //< TODO 

//=============================================================================
namespace opengp {
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
