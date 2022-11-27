#include "TSpch.h"
#include "Shader.h"
#include "glad/glad.h"
#include "../../core/Core.h"

std::string TESLA::Shader::GetStringType()
{
    switch (m_type)
    {
    case TESLA::ShaderType::Fragment:
        return "fragment";
    case TESLA::ShaderType::Vertex:
        return "vertex";
    case TESLA::ShaderType::Compute:
        return "compute";
    default:
        TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Error, attempt to compile unknown shader");
        return "";
    }    
}

GLenum GetGLType(TESLA::ShaderType type)
{
    switch (type)
    {
    case TESLA::ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    case TESLA::ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case TESLA::ShaderType::Compute:
        return GL_COMPUTE_SHADER;
    default:
        return NULL;
    }
}

TESLA::Shader::Shader(TESLA::ShaderType type, std::string fileName)
{
    m_type = type;
    std::string path;
    
    if(path == "")
    {
        path = engineResourcePath + shaderDirectory + GetStringType() + shaderExtension;
    }
    else
    {
        path = resourcePath + shaderDirectory + fileName + shaderExtension;
    }
    
    m_glShader = CompileShader(path);
}


uint32_t TESLA::Shader::CompileShader(std::string& filePath)
{
    std::ifstream shaderFile;
    std::string shaderCode;

    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::string file_line;
        shaderFile.open(filePath, std::ios::in);

        std::stringstream shader_stream;
        shader_stream << shaderFile.rdbuf();

        shaderFile.close();
        shaderCode = shader_stream.str();
    }
    catch (std::ifstream::failure &_)
    {
        TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Failed to read from or open shader file");
    }
    
    const char* source = shaderCode.c_str();
    const GLuint shader = glCreateShader(GetGLType(m_type));
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
#ifdef TS_DEBUG
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Failed to compile {0} shader: {1}", GetStringType(), infoLog);
    }
    else
    {
        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Compiled {0} shader successfully", GetStringType());
    }
#endif
    return shader;
}

uint32_t TESLA::Shader::CompileProgram(std::initializer_list<Shader> shaders)
{
    uint32_t shaderProgram = glCreateProgram();

    for (TESLA::Shader shader : shaders)
    {
        glAttachShader(shaderProgram, shader.m_glShader);
        glDeleteShader(shader.m_glShader);
    }

    glLinkProgram(shaderProgram);

#ifdef TS_DEBUG
    int success;
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Failed to link shader program: {1}", infoLog);
    }
    else
    {
        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Shader program successfully linked");
    }
#endif
    return shaderProgram;
}


