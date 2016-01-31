#pragma once
#include <OpenGP/GL/gl.h>
#include <vector>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <GLenum TARGET, class T>
class Buffer{
private:
    GLuint     _buffer =0;     ///< 0: invalid
    GLsizeiptr _num_elems = 0; ///< # of uploaded elements
    GLsizeiptr _elem_size = 0; ///< size of a single elements (bytes)
    
public:
    Buffer(){ glGenBuffers(1, &_buffer); }
    ~Buffer(){ glDeleteBuffers(1, &_buffer); }
    
    void bind(){ glBindBuffer(TARGET, _buffer); }    
    void unbind(){ glBindBuffer(TARGET, 0); }
    GLsizeiptr size() const{ return _num_elems; } 
    
    void upload(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW){
        this->_num_elems = data.size();
        this->_elem_size = sizeof(T);
        upload(data.data(), _num_elems, _elem_size, usage);
    }
    
    /// @note use the other upload functions whenever possible
    void upload(const GLvoid* raw_data_ptr, GLsizeiptr num_elems, GLsizeiptr elem_size_in_bytes, GLenum usage=GL_STATIC_DRAW){ 
        glBindBuffer(TARGET, _buffer);
        glBufferData(TARGET, num_elems * elem_size_in_bytes, raw_data_ptr, usage);
        this->_num_elems = num_elems;
        this->_elem_size = elem_size_in_bytes;
    }
};

///--- Specializations

template <class T>
using ArrayBuffer = Buffer<GL_ARRAY_BUFFER, T>;

template <class T>
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, T>;

//=============================================================================
} // namespace OpenGP
//=============================================================================
