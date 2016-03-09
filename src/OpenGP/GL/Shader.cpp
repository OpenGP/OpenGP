#include <OpenGP/GL/Shader.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

void Shader::set_uniform(const char* name, int scalar) {
    assert( check_is_current() );
    GLint loc = glGetUniformLocation(pid, name);
    glUniform1i(loc, scalar);
}

void Shader::set_uniform(const char* name, float scalar) {
    assert( check_is_current() );
    GLint loc = glGetUniformLocation(pid, name);
    glUniform1f(loc, scalar);
}

void Shader::set_uniform(const char* name, const Eigen::Vector3f& vector) {
    assert( check_is_current() );
    GLint loc = glGetUniformLocation(pid, name);
    glUniform4fv(loc, 3, vector.data());
}

void Shader::set_uniform(const char* name, const Eigen::Matrix4f& matrix) {
    assert( check_is_current() );
    GLint loc = glGetUniformLocation(pid, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix.data());
}

void Shader::set_attribute(const char* name, float value) {
    assert( check_is_current() );
    GLint loc = glGetAttribLocation(pid, name);
    glVertexAttrib1f(loc, value);
}

void OpenGP::Shader::set_attribute(const char* name, const Eigen::Vector3f& vector) {
    assert( check_is_current() );
    GLint loc = glGetAttribLocation(pid, name);
    glVertexAttrib3fv(loc, vector.data());
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<float>& buffer) {
    assert( check_is_current() );
    GLint location = glGetAttribLocation(pid, name); ///< property to modify
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec1*/ 1, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<Eigen::Vector2f>& buffer) {
    assert( check_is_current() );
    GLint location = glGetAttribLocation(pid, name); ///< property to modify
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec3*/ 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<Eigen::Vector3f>& buffer) {
    assert( check_is_current() );
    GLint location = glGetAttribLocation(pid, name); ///< property to modify
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec3*/ 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}

bool OpenGP::Shader::add_vshader_from_source(const char* code) {
    if (verbose) mDebug() << "Compiling Vertex shader";

    /// Create the Vertex Shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    /// Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, &code, NULL);
    glCompileShader(VertexShaderID);

    /// Check Vertex Shader
    GLint Success = GL_FALSE;
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
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

bool OpenGP::Shader::add_fshader_from_source(const char* code) {
    if (verbose) mDebug() << "Compiling Fragment shader";

    /// Create the Fragment Shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /// Compile Fragment Shader
    glShaderSource(FragmentShaderID, 1, &code, NULL);
    glCompileShader(FragmentShaderID);

    /// Check Fragment Shader
    GLint Success = GL_FALSE;
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
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

bool OpenGP::Shader::link() {
    if (verbose) mDebug() << "Linking shader program";
    glLinkProgram(pid);

    /// Check the program
    GLint Success = GL_FALSE;
    glGetProgramiv(pid, GL_LINK_STATUS, &Success);
    if (!Success) {
        int InfoLogLength;
        glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
        glGetProgramInfoLog(pid, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        mDebug() << "Failed: " << &ProgramErrorMessage[0];
    }

    _is_valid = true;
    return Success;

    // TODO: should this be done?
    // glDeleteShader(VertexShaderID);
    // glDeleteShader(FragmentShaderID);
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
