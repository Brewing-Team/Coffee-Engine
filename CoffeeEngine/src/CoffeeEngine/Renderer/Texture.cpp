#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceLoader.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/vec4.hpp>
#include <tracy/Tracy.hpp>

namespace Coffee {

    GLenum ImageFormatToOpenGLInternalFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::R8: return GL_R8; break;
            case ImageFormat::RG8: return GL_RG8; break;
            case ImageFormat::RGB8: return GL_RGB8; break;
            case ImageFormat::SRGB8: return GL_SRGB8; break;
            case ImageFormat::RGBA8: return GL_RGBA8; break;
            case ImageFormat::SRGBA8: return GL_SRGB8_ALPHA8; break;
            case ImageFormat::RGBA32F: return GL_RGBA32F; break;
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
            case ImageFormat::SRGB8: return GL_RGB; break;
            case ImageFormat::RGBA8: return GL_RGBA; break;
            case ImageFormat::SRGBA8: return GL_RGBA; break;
            case ImageFormat::RGBA32F: return GL_RGBA; break;
            case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL; break;
        }
    }

    Texture::Texture(const TextureProperties& properties)
        : m_Properties(properties), m_Width(properties.Width), m_Height(properties.Height)
    {
        ZoneScoped;

        Texture(m_Width, m_Height, m_Properties.Format);
    }

    Texture::Texture(uint32_t width, uint32_t height, ImageFormat imageFormat)
        : Resource(ResourceType::Texture), m_Width(width), m_Height(height), m_Properties({ imageFormat, width, height })
    {
        ZoneScoped;

        int mipLevels = 1 + floor(log2(std::max(m_Width, m_Height)));

        GLenum internalFormat = ImageFormatToOpenGLInternalFormat(m_Properties.Format);
        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
        glTextureStorage2D(m_textureID, mipLevels, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Add an option to choose the anisotropic filtering level
        glTextureParameterf(m_textureID, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
    }

    Texture::Texture(const std::filesystem::path& path, bool srgb)
        : Resource(ResourceType::Texture)
    {
        ZoneScoped;

        m_FilePath = path;
        m_Name = path.filename().string();

        m_Properties.srgb = srgb;

        int nrComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(m_FilePath.string().c_str(), &m_Width, &m_Height, &nrComponents, 0);
        
        m_Properties.Width = m_Width, m_Properties.Height = m_Height;
        
        if(data)
        {
            m_Data = std::vector<unsigned char>(data, data + m_Width * m_Height * nrComponents);
            stbi_image_free(data);

            switch (nrComponents)
            {
                case 1:
                    m_Properties.Format = ImageFormat::R8;
                break;
                case 3:
                    m_Properties.Format = m_Properties.srgb ? ImageFormat::SRGB8 : ImageFormat::RGB8;
                break;
                case 4:
                    m_Properties.Format = m_Properties.srgb ? ImageFormat::SRGBA8 : ImageFormat::RGBA8;
                break;
            }

            int mipLevels = 1 + floor(log2(std::max(m_Width, m_Height)));

            GLenum internalFormat = ImageFormatToOpenGLInternalFormat(m_Properties.Format);
            GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);

            glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
            glTextureStorage2D(m_textureID, mipLevels, internalFormat, m_Width, m_Height);

            glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //Add an option to choose the anisotropic filtering level
            glTextureParameterf(m_textureID, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

            glTextureSubImage2D(m_textureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, m_Data.data());

            glGenerateTextureMipmap(m_textureID);
        }
        else
        {
            COFFEE_CORE_ERROR("Failed to load texture: {0} (REASON: {1})", m_FilePath.string(), stbi_failure_reason());
            m_textureID = 0; // Set texture ID to 0 to indicate failure
        }
    }

    Texture::~Texture()
    {
        ZoneScoped;

        glDeleteTextures(1, &m_textureID);
    }

    void Texture::Bind(uint32_t slot)
    {
        ZoneScoped;

        glBindTextureUnit(slot, m_textureID);
    }

    void Texture::Resize(uint32_t width, uint32_t height)
    {
        ZoneScoped;

        m_Width = width;
        m_Height = height;

        glDeleteTextures(1, &m_textureID);

        int mipLevels = 1 + floor(log2(std::max(m_Width, m_Height)));

        GLenum internalFormat = ImageFormatToOpenGLInternalFormat(m_Properties.Format);
        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
        glTextureStorage2D(m_textureID, mipLevels, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Add an option to choose the anisotropic filtering level
        glTextureParameterf(m_textureID, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);


        //Te code above is the same as the constructor but for some reason it doesn't work
        //Texture(m_Width, m_Height, m_Properties.Format);
    }

    void Texture::Clear(glm::vec4 color)
    {
        ZoneScoped;

        glBindTexture(GL_TEXTURE_2D, m_textureID);

        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);
        glClearTexImage(m_textureID, 0, format, GL_FLOAT, &color);
    }

    void Texture::SetData(void* data, uint32_t size)
    {
        ZoneScoped;

        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);
        glTextureSubImage2D(m_textureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_textureID);
    }

    Ref<Texture> Texture::Load(const std::filesystem::path& path, bool srgb)
    {
        return ResourceLoader::LoadTexture(path, srgb);
    }

    Ref<Texture> Texture::Create(uint32_t width, uint32_t height, ImageFormat format)
    {
        return CreateRef<Texture>(width, height, format);
    }

}
