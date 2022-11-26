#include "TSpch.h"
#include "Texture.h"
#include "../../core/Core.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage.h"
#include "glad/glad.h"

std::string TESLA::Texture::GetStringExtension()
{
    switch (m_extension)
    {
    case TESLA::TextureExtension::Png:
        return ".png";
    case TESLA::TextureExtension::Jpg:
        return ".jpg";
    default:
        return "";
    }
}

std::string TESLA::Texture::GetStringType()
{
    switch (m_type)
    {
    case TESLA::TextureType::Albedo:
        return "albedo";
    default:
        return "";
    }
}

TESLA::Texture::Texture(TESLA::TextureType type, TESLA::TextureExtension extension, std::string fileName)
{
    m_type = type;
    m_extension = extension;
    m_texture = 1;
    
    std::string path;
    
    if(fileName == "")
    {
        path = engineResourcePath + texturesDirectory + GetStringType() + GetStringExtension();
    }
    else
    {
        path = resourcePath + texturesDirectory + fileName + GetStringExtension();
    }

    int width, height, numChannels;
    unsigned char* data = stbi_load(path.data(), &width, &height, &numChannels, 0);

#ifdef TS_DEBUG
    if(!data)
    {
        TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Failed to load texture at path {0}", path);
    }
    else
    {
        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Successfully loaded texture at path {0}", path);
    }
#endif

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    GLint format = m_extension == TextureExtension::Jpg ? GL_RGB : GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}


