#pragma once

namespace TESLA
{
    enum TextureType
    {
        Albedo,
    };

    enum TextureExtension
    {
        Png,
        Jpg
    };
    
    class Texture
    {
    public:
        Texture(TESLA::TextureType type, TESLA::TextureExtension extension, std::string fileName = "");
        uint32_t GetGLTexture(){return m_texture;}
    private:
        std::string GetStringExtension();
        std::string GetStringType();
    private:
        TESLA::TextureType m_type;
        TESLA::TextureExtension m_extension;
        uint32_t m_texture;
    };
}
    
