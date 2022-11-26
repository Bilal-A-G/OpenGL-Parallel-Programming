#pragma once
namespace TESLA
{
    enum class ShaderType
    {
        Fragment,
        Vertex
    };
    
    class Shader
    {
    public:
        Shader(TESLA::ShaderType type, std::string fileName = "");
        TESLA::ShaderType GetEnumType(){return m_type;}
        std::string GetStringType();
        
        static uint32_t CompileProgram(std::initializer_list<TESLA::Shader> shaders);
    private:
        uint32_t CompileShader(std::string& path);
        TESLA::ShaderType m_type;
        uint32_t m_glShader;
    };
}
