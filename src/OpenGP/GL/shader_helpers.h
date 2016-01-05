#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace OpenGP{

/// Compiles the vertex, geometry and fragment shaders stored in the given strings
GLuint compile_shaders(const char * vshader, const char * fshader, const char * gshader = NULL) {
    const int SHADER_LOAD_FAILED = 0;
    GLint Success = GL_FALSE;
    int InfoLogLength;

    /// Create the Vertex Shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    /// Compile Vertex Shader
    fprintf(stdout, "Compiling Vertex shader: ");
    char const * VertexSourcePointer = vshader;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    /// Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(!Success) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    /// Create the Fragment Shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /// Compile Fragment Shader
    fprintf(stdout, "Compiling Fragment shader: ");
    char const * FragmentSourcePointer = fshader;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    /// Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(!Success) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "Failed:\n%s\n", &FragmentShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    GLuint GeometryShaderID = 0;
    if(gshader != NULL) {
        /// Create the Geometry Shader
        GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

        /// Compile Geometry Shader
        fprintf(stdout, "Compiling Geometry shader: ");
        char const * GeometrySourcePointer = gshader;
        glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
        glCompileShader(GeometryShaderID);

        /// Check Geometry Shader
        glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Success);
        glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(!Success) {
            std::vector<char> GeometryShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
            fprintf(stdout, "Failed:\n%s\n", &GeometryShaderErrorMessage[0]);
            return SHADER_LOAD_FAILED;
        }
        else
            fprintf(stdout, "Success\n");
    }

    /// Link the program
    fprintf(stdout, "Linking shader program: ");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    if(gshader != NULL) glAttachShader(ProgramID, GeometryShaderID);
    glLinkProgram(ProgramID);

    /// Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if(!Success) {
        fprintf(stdout, "Failed:\n%s\n", &ProgramErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    if(gshader != NULL) glDeleteShader(GeometryShaderID);

    /// make sure you see the text in terminal
    fflush(stdout);

    return ProgramID;
}


/// Compiles the vertex, geometry and fragment shaders using file path
GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path = NULL) {
    const int SHADER_LOAD_FAILED = 0; 

    std::string VertexShaderCode, FragmentShaderCode, GeometryShaderCode;
    {
        /// Read the Vertex Shader code from the file
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()) {
            VertexShaderCode = std::string(std::istreambuf_iterator<char>(VertexShaderStream),
                                           std::istreambuf_iterator<char>());
            VertexShaderStream.close();
        } else {
            printf("Could not open file: %s\n", vertex_file_path);
            return SHADER_LOAD_FAILED;
        }   
    
        /// Read the Fragment Shader code from the file
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()) {
            FragmentShaderCode = std::string(std::istreambuf_iterator<char>(FragmentShaderStream),
                                             std::istreambuf_iterator<char>());
            FragmentShaderStream.close();
        } else {
            printf("Could not open file: %s\n", fragment_file_path);
            return SHADER_LOAD_FAILED;
        }

        /// Read the Geometry Shader code from the file
        if(geometry_file_path != NULL) {
            std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
            if(GeometryShaderStream.is_open()) {
                GeometryShaderCode = std::string(std::istreambuf_iterator<char>(GeometryShaderStream),
                                                 std::istreambuf_iterator<char>());
                GeometryShaderStream.close();
            } else {
                printf("Could not open file: %s\n", geometry_file_path);
                return SHADER_LOAD_FAILED;
            }
        }
    }

    /// Compile them
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    char const * GeometrySourcePointer = NULL;
    if(geometry_file_path != NULL) GeometrySourcePointer = GeometryShaderCode.c_str();
    return compile_shaders(VertexSourcePointer, FragmentSourcePointer, GeometrySourcePointer);
}

} //< OpenGP::
