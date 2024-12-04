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

    Texture2D::Texture2D(const TextureProperties& properties)
        : m_Properties(properties), m_Width(properties.Width), m_Height(properties.Height)
    {
        ZoneScoped;

        Texture2D(m_Width, m_Height, m_Properties.Format);
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height, ImageFormat imageFormat)
        : Texture(), m_Width(width), m_Height(height), m_Properties({ imageFormat, width, height })
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

    Texture2D::Texture2D(const std::filesystem::path& path, bool srgb)
        : Texture()
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

    Texture2D::~Texture2D()
    {
        ZoneScoped;

        glDeleteTextures(1, &m_textureID);
    }

    void Texture2D::Bind(uint32_t slot)
    {
        ZoneScoped;

        glBindTextureUnit(slot, m_textureID);
    }

    void Texture2D::Resize(uint32_t width, uint32_t height)
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
        //Texture2D(m_Width, m_Height, m_Properties.Format);
    }

    void Texture2D::Clear(glm::vec4 color)
    {
        ZoneScoped;

        glBindTexture(GL_TEXTURE_2D, m_textureID);

        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);
        glClearTexImage(m_textureID, 0, format, GL_FLOAT, &color);
    }

    void Texture2D::SetData(void* data, uint32_t size)
    {
        ZoneScoped;

        GLenum format = ImageFormatToOpenGLFormat(m_Properties.Format);
        glTextureSubImage2D(m_textureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_textureID);
    }

    Ref<Texture2D> Texture2D::Load(const std::filesystem::path& path, bool srgb)
    {
        return ResourceLoader::LoadTexture2D(path, srgb);
    }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, ImageFormat format)
    {
        return CreateRef<Texture2D>(width, height, format);
    }

    Cubemap::Cubemap(const std::vector<std::filesystem::path>& paths)
    {
        ZoneScoped;
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < paths.size(); i++)
        {
            unsigned char* data = stbi_load(paths[i].string().c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                GLenum format;
                if (nrChannels == 1)
                    format = GL_RED;
                else if (nrChannels == 3)
                    format = GL_RGB;
                else if (nrChannels == 4)
                    format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                COFFEE_CORE_ERROR("Cubemap texture failed to load at path: {0}", paths[i].string());
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    };
    Cubemap::Cubemap(const std::filesystem::path& path)
    {
        // Load the combined image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            COFFEE_CORE_ERROR("Failed to load cubemap texture: {0}", path.string());
            return;
        }

        // Calculate the face dimensions
        int faceWidth = width / 6;
        int faceHeight = height;

        // Generate and bind the cubemap texture
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        // Extract and upload each face
        GLenum targets[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };

        for (int i = 0; i < 6; ++i) {
            // Calculate the offset in the combined image
            unsigned char* faceData = data + (i * faceWidth * nrChannels);

            // Create a temporary buffer for the face
            unsigned char* faceBuffer = new unsigned char[faceWidth * faceHeight * nrChannels];
            for (int y = 0; y < faceHeight; ++y) {
                memcpy(
                    faceBuffer + y * faceWidth * nrChannels,
                    faceData + y * width * nrChannels,
                    faceWidth * nrChannels
                );
            }

            // Upload the face data
            glTexImage2D(
                targets[i],
                0, GL_RGBA, faceWidth, faceHeight,
                0, GL_RGBA, GL_UNSIGNED_BYTE, faceBuffer
            );

            delete[] faceBuffer;
        }

        stbi_image_free(data);

        // Set cubemap texture parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    Cubemap::~Cubemap()
    {
        ZoneScoped;
        glDeleteTextures(1, &m_textureID);
    }

    void Cubemap::Bind(uint32_t slot)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    }
    Ref<Cubemap> Cubemap::Load(const std::filesystem::path& path) { return nullptr; }
    Ref<Cubemap> Cubemap::Create(const std::vector<std::filesystem::path>& paths) { return nullptr; }

} // namespace Coffee
