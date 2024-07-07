#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <glad/glad.h>

namespace Tea {

    GLint ImageChannelsToOpenGLComponents(ImageChannels channels)
    {
        switch (channels) {
            case ImageChannels::R8: return GL_R8; break;
            case ImageChannels::RGB8: return GL_RGB8; break;
            case ImageChannels::RGBA8: return GL_RGBA8; break;
        }
    }

    TextureFormat ImageChannelsToTextureFormat(ImageChannels channels)
    {
        switch (channels) {
            case ImageChannels::R8: return TextureFormat::R; break;
            case ImageChannels::RGB8: return TextureFormat::RGB; break;
            case ImageChannels::RGBA8: return TextureFormat::RGBA; break;
        }
    }

    GLenum TextureFormatToOpenGLFormat(TextureFormat format)
    {
        switch (format) {
            case TextureFormat::R: return GL_RED; break;
            case TextureFormat::RGB: return GL_RGB; break;
            case TextureFormat::RGBA: return GL_RGBA; break;
        }
    }

    Texture::Texture(std::string& path)
    {
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        Ref<Image> img = Image::Load(path);

        GLint channels = ImageChannelsToOpenGLComponents(img->GetChannels());

        m_Format = ImageChannelsToTextureFormat(img->GetChannels());

        GLenum format = TextureFormatToOpenGLFormat(m_Format);

        glTexImage2D(GL_TEXTURE_2D, 0, channels, img->GetSize().first, img->GetSize().second, 0, format, GL_UNSIGNED_BYTE, img->GetData());
    }

}