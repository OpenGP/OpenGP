#include <fstream>
#include <iostream>
#include <vector>

GLuint compile_shaders(const char * vshader,const char * fshader){
    const int SHADER_LOAD_FAILED = 0;
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Compile Vertex Shader
    char const * VertexSourcePointer = vshader;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");
    
    // Compile Fragment Shader
    char const * FragmentSourcePointer = fshader;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &FragmentShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");
    
    // Link the program
    fprintf(stdout, "Linking shader program: ");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &ProgramErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    // make sure you see the text in terminal
    fflush(stdout);
    
    return ProgramID;
}


/// @brief Parses the files provided in input and compiles the shaders
/// @see http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
GLuint load_shaders(const char * vertex_file_path,const char * fragment_file_path){
    const int SHADER_LOAD_FAILED = 0; 
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    } else {
        printf("Could not open file: %s\n",vertex_file_path);
        return SHADER_LOAD_FAILED;
    }   
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    } else {
        printf("Could not open file: %s\n",fragment_file_path);
        return SHADER_LOAD_FAILED;
    }   

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling vertex shader: %s ", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");
    
    // Compile Fragment Shader
    printf("Compiling fragment shader: %s ", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &FragmentShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");
    
    // Link the program
    fprintf(stdout, "Linking shader program: ");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if(InfoLogLength>0){
        fprintf(stdout, "Failed:\n%s\n", &ProgramErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    // make sure you see the text in terminal
    fflush(stdout);
    
    return ProgramID;
}
