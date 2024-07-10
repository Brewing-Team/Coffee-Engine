#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

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
        }
    }

    Texture::Texture(std::string& path)
    {
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        m_FilePath = path;

        int nrComponents;
        unsigned char* m_Data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &nrComponents, 0);

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

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_Data);

        stbi_image_free(m_Data);
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Ref<Texture> Texture::Load(std::string& path)
    {
        return CreateRef<Texture>(path);
    }

}
