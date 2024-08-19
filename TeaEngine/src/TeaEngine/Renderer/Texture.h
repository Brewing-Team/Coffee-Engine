#pragma once

#include "TeaEngine/Core/Assert.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Image.h"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Tea {

    enum class ImageFormat
    {
        //Color
        R8,
        RG8,
        RGB8,
        SRGB8,
        RGBA8,
        SRGBA8,
        RGBA32F,


        //Depth
        DEPTH24STENCIL8
    };

    struct TextureProperties
    {
        ImageFormat Format;
        uint32_t Width, Height;
        bool GenerateMipmaps = true;
        bool srgb = true;
    };

    class Texture
    {
    public:
        Texture(const TextureProperties& properties);
        Texture(uint32_t width, uint32_t height, ImageFormat imageFormat);
        Texture(const std::string& path, bool srgb = true);
        ~Texture();

        void Bind(uint32_t slot);

        void Resize(uint32_t width, uint32_t height);

        std::pair<uint32_t, uint32_t> GetSize() { return std::make_pair(m_Width, m_Height); };
        uint32_t GetWidth() { return m_Width; };
        uint32_t GetHeight() { return m_Width; };
        uint32_t GetID() { return m_textureID; };

        const std::string& GetPath() { TEA_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; };

        void SetData(void* data, uint32_t size);
        ImageFormat GetImageFormat() { return m_Properties.Format; };

        static Ref<Texture> Load(const std::string& path, bool srgb = true);
        static Ref<Texture> Create(uint32_t width, uint32_t height, ImageFormat format);
    private:
        TextureProperties m_Properties;

        std::string m_FilePath;
        uint32_t m_textureID;
        int m_Width, m_Height;
    };

    class TextureLibrary
    {
    public:
        static void Add(const std::string& name, const Ref<Texture>& texture) { m_Textures[name] = texture; }
        static Ref<Texture> Get(const std::string& name) { return m_Textures[name]; }
        static bool Exists(const std::string& name) { return m_Textures[name] ? true : false; }
    private:
        static std::unordered_map<std::string, Ref<Texture>> m_Textures;
    };

}
