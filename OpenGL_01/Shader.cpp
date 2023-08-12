#include "Shader.h"

Shader::Shader()
{
	shaderId = 0;
	uniformProjection = 0;
	uniformModel = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexShader = vertexString.c_str();
    const char* fragmentShader = fragmentString.c_str();

    CompileShader(vertexShader, fragmentShader);
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s! - The file does not exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderId = glCreateProgram(); // Crear un programa de shaders

    if (!shaderId)
    {
        printf("Error creating shader program!");
        return;
    }

    // Agregar los shaders al programa
    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);     // Agregar el shader de vértices
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER); // Agregar el shader de fragmentos

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId); // Vincular los shaders en el programa
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result); // Obtener el estado de vinculación del programa

    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderId); // Validar el programa
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result); // Obtener el estado de validación del programa

    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformView = glGetUniformLocation(shaderId, "view");
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint _shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(_shader, 1, code, codeLength); // Cargar el código del shader
    glCompileShader(_shader);                     // Compilar el shader

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(_shader, GL_COMPILE_STATUS, &result); // Obtener el estado de compilación del shader
    if (!result)
    {
        glGetShaderInfoLog(_shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, _shader); // Adjuntar el shader al programa
}

void Shader::UseShader()
{
    glUseProgram(shaderId);
}

void Shader::ClearShader()
{
    if (shaderId != 0)
    {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
    uniformProjection = 0;
    uniformModel = 0;
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

Shader::~Shader() 
{
    ClearShader();
}