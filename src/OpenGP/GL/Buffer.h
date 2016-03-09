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
    const GLsizeiptr _elem_size; ///< size of a single elements (bytes)
    
public:
    Buffer() : _elem_size(sizeof(T)){
        glGenBuffers(1, &_buffer); 
    }
    ~Buffer(){ glDeleteBuffers(1, &_buffer); }
    
    void bind(){ glBindBuffer(TARGET, _buffer); }    
    void unbind(){ glBindBuffer(TARGET, 0); }
    GLsizeiptr size() const{ return _num_elems; } 
    
    void upload(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW){
        this->_num_elems = data.size();
        upload_raw(data.data(), _num_elems, usage);
    }
    
    /// @note use the other upload functions whenever possible
    void upload_raw(const GLvoid* raw_data_ptr, GLsizeiptr num_elems, GLenum usage=GL_STATIC_DRAW){
        this->_num_elems = num_elems;
        glBindBuffer(TARGET, _buffer);
        glBufferData(TARGET, num_elems*_elem_size, raw_data_ptr, usage);
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
