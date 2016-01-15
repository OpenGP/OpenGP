#pragma once
#include <Eigen/Dense>
#include <OpenGP/GL/gl.h>
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
class ShaderProgram{
/// @{
private:
    GLuint pid = 0; ///< 0: invalid
public:
    bool verbose = false; ///< prints messages
/// @}

public:
    ShaderProgram(){ pid = glCreateProgram(); }
    GLuint programId() const { return pid; }
    void bind(){ glUseProgram(pid); } // TODO: check program valid
    void release(){ glUseProgram(0); }
    
    void set_uniform(const char* name, const float& scalar){
        bind(); ///< todo: rather than binding check for bound?
            GLint loc = glGetUniformLocation(pid, name);
            glUniform1f(loc, scalar);
        release();
    }
    
    void set_uniform(const char* name, const Eigen::Vector3f& vector){
        bind(); //< todo: rather than binding check for bound?
            GLint loc = glGetUniformLocation(pid, name);
            glUniform4fv(loc, 3, vector.data());
        release();
    }
    
    void set_uniform(const char* name, const Eigen::Matrix4f& matrix){
        bind(); //< todo: check if bound instead
            GLint loc = glGetUniformLocation(pid, name);
            glUniformMatrix4fv(loc, 1, GL_FALSE, matrix.data());
        release();
    }
    
    /// Set vertex attribute that is *constant* across vertices
    void set_attribute(const char* name, float value){
        bind(); //< todo: check if bound instead
            GLint loc = glGetAttribLocation(pid, name);
            glVertexAttrib1f(loc, value);
        release();
    }
    
    /// Set vertex attribute that is *constant* across vertices
    void set_attribute(const char* name, const Eigen::Vector3f& vector){
        bind(); //< todo: check if bound instead
            GLint loc = glGetAttribLocation(pid, name);
            glVertexAttrib3fv(loc, vector.data());
        release();
    }
    
    void set_attribute(const char* name, ArrayBuffer<Eigen::Vector3f>& buffer){
        bind(); //< todo: check if bound instead
            GLint location = glGetAttribLocation(pid, name); ///< property to modify
            glEnableVertexAttribArray(location); ///< cached in VAO
            buffer.bind(); ///< memory the description below refers to
            glVertexAttribPointer(location, /*vec3*/ 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);   
        release();
    }
    
public:
    bool add_vshader_from_source(const char* code){
        if(verbose) mDebug() << "Compiling Vertex shader";
        
        /// Create the Vertex Shader
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    
        /// Compile Vertex Shader
        glShaderSource(VertexShaderID, 1, &code, NULL);
        glCompileShader(VertexShaderID);
    
        /// Check Vertex Shader
        GLint Success = GL_FALSE;
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
        if(!Success){
            int InfoLogLength;
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> VertexShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            // fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
            mDebug() << std::string(&VertexShaderErrorMessage[0]);
        } else {
            glAttachShader(pid, VertexShaderID);        
        }
        return Success;
    }
    
    bool add_fshader_from_source(const char* code){ 
        if(verbose) mDebug() << "Compiling Fragment shader";

        /// Create the Fragment Shader
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
        /// Compile Fragment Shader
        glShaderSource(FragmentShaderID, 1, &code, NULL);
        glCompileShader(FragmentShaderID);
    
        /// Check Fragment Shader
        GLint Success = GL_FALSE;
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
        if(!Success) {
            int InfoLogLength;            
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            mDebug() << std::string(&FragmentShaderErrorMessage[0]);
        } else {
            glAttachShader(pid, FragmentShaderID);            
        }

        return Success;
    }
    
    bool link(){
       if(verbose) mDebug() << "Linking shader program";
        glLinkProgram(pid);
    
        /// Check the program
        GLint Success = GL_FALSE;
        glGetProgramiv(pid, GL_LINK_STATUS, &Success);
        if(!Success){
            int InfoLogLength;
            glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
            glGetProgramInfoLog(pid, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            mDebug() << "Failed: " << &ProgramErrorMessage[0];
        } 
        
        return Success;
    
        // TODO: should this be done?
        // glDeleteShader(VertexShaderID);
        // glDeleteShader(FragmentShaderID);
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
