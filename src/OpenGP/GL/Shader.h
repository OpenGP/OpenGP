#pragma once
#include <Eigen/Dense>
#include <OpenGP/GL/gl.h>
#include <OpenGP/headeronly.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/Buffer.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

namespace{
    /// Convenience constants
    static const int ONE = 1;
    static const bool DONT_NORMALIZE = false;
    static const bool DONT_TRANSPOSE = false;
    static const int ZERO_STRIDE = 0;
    static const void* ZERO_BUFFER_OFFSET = 0;
}

/// @note Inspired by QOpenGLShaderProgram
/// @note Important not to use Scalar
class Shader{
/// @{
private:
    GLuint pid = 0; ///< 0: invalid
    bool _is_valid = false;
public:
    bool verbose = false; ///< prints messages
/// @}

public:
    Shader(){ pid = glCreateProgram(); }
    GLuint programId() const { return pid; }
    void bind(){ assert(_is_valid); glUseProgram(pid); }
    void release(){ glUseProgram(0); }
    bool is_valid(){ return _is_valid; }
    
    bool check_is_current(){
        assert(pid!=0);
        return (current_program_id() == pid);
    }

    static GLuint current_program_id(){ 
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM,&id);
        return (GLuint) id;
    }

/// @{ shaders compile and link
public:
    HEADERONLY_INLINE bool add_vshader_from_source(const char* code);
    HEADERONLY_INLINE bool add_fshader_from_source(const char* code);
    HEADERONLY_INLINE bool link();
/// @}
    
/// @{ uniforms setters
public:
    HEADERONLY_INLINE void set_uniform(const char* name, int scalar);
    HEADERONLY_INLINE void set_uniform(const char* name, float scalar);
    HEADERONLY_INLINE void set_uniform(const char* name, const Eigen::Vector3f& vector);
    HEADERONLY_INLINE void set_uniform(const char* name, const Eigen::Matrix4f& matrix);
/// @}    

/// @{ setters for *constant* vertex attributes 
public:
    HEADERONLY_INLINE void set_attribute(const char* name, float value);
    HEADERONLY_INLINE void set_attribute(const char* name, const Eigen::Vector3f& vector);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<float>& buffer);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<Eigen::Vector2f>& buffer);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<Eigen::Vector3f>& buffer);
/// @}

};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "Shader.cpp"
#endif
