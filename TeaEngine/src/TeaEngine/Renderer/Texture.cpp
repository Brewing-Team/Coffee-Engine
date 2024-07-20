#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <cstdint>
#include <glad/glad.h>
#include <stb_image.h>

namespace Tea {

    GLenum ImageFormatToOpenGLInternalFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::R8: return GL_R8; break;
            case ImageFormat::RG8: return GL_RG8; break;
            case ImageFormat::RGB8: return GL_RGB8; break;
            case ImageFormat::RGBA8: return GL_RGBA8; break;
            case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8; break;
        }
    }

    GLenum ImageFormatToOpenGLFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::R8: return GL_RED; break;
            case ImageFormat::RG8: return GL_RG; break;
            case ImageFormat::RGB8: return GL_RGB; break;
            case ImageFormat::RGBA8: return GL_RGBA; break;
            case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL; break;
        }
    }

    Texture::Texture(uint32_t width, uint32_t height, ImageFormat imageFormat)
        : m_Width(width), m_Height(height), m_Format(imageFormat)
    {
        GLenum internalFormat = ImageFormatToOpenGLInternalFormat(m_Format);
        GLenum format = ImageFormatToOpenGLFormat(m_Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
        glTextureStorage2D(m_textureID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture::Texture(const std::string& path)
    {
        m_FilePath = path;

        int nrComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* m_Data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &nrComponents, 0);
        if(m_Data)
        {
            switch (nrComponents)
            {
                case 1:
                    m_Format = ImageFormat::R8;
                break;
                case 3:
                    m_Format = ImageFormat::RGB8;
                break;
                case 4:
                    m_Format = ImageFormat::RGBA8;
                break;
            }

            GLenum internalFormat = ImageFormatToOpenGLInternalFormat(m_Format);
            GLenum format = ImageFormatToOpenGLFormat(m_Format);

            glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
            glTextureStorage2D(m_textureID, 1, internalFormat, m_Width, m_Height);

            glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTextureSubImage2D(m_textureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, m_Data);

            stbi_image_free(m_Data);
        }
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_textureID);
    }

    void Texture::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_textureID);
    }

    void Texture::SetData(void* data, uint32_t size)
    {
        GLenum format = ImageFormatToOpenGLFormat(m_Format);
        glTextureSubImage2D(m_textureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);
    }

    Ref<Texture> Texture::Load(const std::string& path)
    {
        return CreateRef<Texture>(path);
    }

    Ref<Texture> Texture::Create(uint32_t width, uint32_t height, ImageFormat format)
    {
        return CreateRef<Texture>(width, height, format);
    }

}
