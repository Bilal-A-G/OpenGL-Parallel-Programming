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
    case TESLA::TextureType::Compute:
        return "compute";
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

    if(type != TESLA::TextureType::Compute)
    {
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
    else
    {
        TS_LOG_ASSERTION(true, TESLA_LOGGER::ERR, "Error, compute shaders cannot be loaded through this constructor");
    }
}

TESLA::Texture::Texture(TESLA::TextureType type, int width, int height)
{
    m_type = type;

    if(type != TESLA::TextureType::Compute)
    {
        TS_LOG_ASSERTION(true, TESLA_LOGGER::ERR, "Error, only compute shaders can be loaded through this constructor");
    }
    
    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
        
    glTexParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, m_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}



