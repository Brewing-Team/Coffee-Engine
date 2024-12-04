#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"

#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cstdint>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Coffee {

    enum class ImageFormat
    {
        R8,
        RG8,
        RGB8,
        SRGB8,
        RGBA8,
        SRGBA8,
        RGBA32F,
        DEPTH24STENCIL8
    };

    struct TextureProperties
    {
        ImageFormat Format;
        uint32_t Width, Height;
        bool GenerateMipmaps = true;
        bool srgb = true;

        template<class Archive>
        void serialize(Archive& archive)
        {
            int formatInt = static_cast<int>(Format);
            archive(formatInt, Width, Height, GenerateMipmaps, srgb);
            Format = static_cast<ImageFormat>(formatInt);
        } 
    };

    class Texture : public Resource
    {
    public:
        Texture() : Resource(ResourceType::Texture) {};
        virtual ~Texture() = default;

        virtual void Bind(uint32_t slot) = 0;
        virtual uint32_t GetWidth() = 0;
        virtual uint32_t GetHeight() = 0;
        virtual uint32_t GetID() = 0;
        virtual ImageFormat GetImageFormat() = 0;
    private:
        friend class cereal::access;
        
        template<class Archive>
        void save(Archive& archive) const
        {
            archive(cereal::base_class<Resource>(this));
        }

        template<class Archive>
        void load(Archive& archive)
        {
            archive(cereal::base_class<Resource>(this));
        }
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        Texture2D(const TextureProperties& properties);
        Texture2D(uint32_t width, uint32_t height, ImageFormat imageFormat);
        Texture2D(const std::filesystem::path& path, bool srgb = true);
        ~Texture2D();

        void Bind(uint32_t slot) override;
        void Resize(uint32_t width, uint32_t height);
        std::pair<uint32_t, uint32_t> GetSize() { return std::make_pair(m_Width, m_Height); };
        uint32_t GetWidth() override { return m_Width; };
        uint32_t GetHeight() override { return m_Height; };
        uint32_t GetID() override { return m_textureID; };
        ImageFormat GetImageFormat() override { return m_Properties.Format; };

        void Clear(glm::vec4 color);
        void SetData(void* data, uint32_t size);

        static Ref<Texture2D> Load(const std::filesystem::path& path, bool srgb = true);
        static Ref<Texture2D> Create(uint32_t width, uint32_t height, ImageFormat format);

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const
        {
            archive(m_Properties, m_Data, m_Width, m_Height, cereal::base_class<Texture>(this));
        }

        template <class Archive>
        void load(Archive& archive)
        {
            archive(m_Properties, m_Data, m_Width, m_Height, cereal::base_class<Texture>(this));
        }

        template <class Archive>
        static void load_and_construct(Archive& data, cereal::construct<Texture2D>& construct)
        {
            TextureProperties properties;
            data(properties);
            construct(properties.Width, properties.Height, properties.Format);

            data(construct->m_Data, construct->m_Width, construct->m_Height,
                 cereal::base_class<Texture>(construct.ptr()));
            construct->m_Properties = properties;
            construct->SetData(construct->m_Data.data(), construct->m_Data.size());
        }
    private:
        TextureProperties m_Properties;
        std::vector<unsigned char> m_Data;
        uint32_t m_textureID;
        int m_Width, m_Height;
    };

    class Cubemap : public Texture
    {
    public:
        Cubemap() = default;
        Cubemap(const std::filesystem::path& path);
        Cubemap(const std::vector<std::filesystem::path>& paths);

        void Bind(uint32_t slot) override;;
        uint32_t GetID() override { return m_textureID; };

        uint32_t GetWidth() override { return 0; };
        uint32_t GetHeight() override { return 0; };
        ImageFormat GetImageFormat() override { return ImageFormat::RGBA8; };

        static Ref<Cubemap> Load(const std::filesystem::path& path);
        static Ref<Cubemap> Create(const std::vector<std::filesystem::path>& paths);
    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const
        {
            archive(m_textureID, cereal::base_class<Resource>(this));
        }

        template <class Archive>
        void load(Archive& archive)
        {
            archive(m_textureID, cereal::base_class<Resource>(this));
        }

/*         template <class Archive>
        static void load_and_construct(Archive& data, cereal::construct<Cubemap>& construct)
        {
            std::vector<unsigned char> data;
            data(data);
            construct(data);
        }*/

    private:
        uint32_t m_textureID;
    };

}

CEREAL_REGISTER_TYPE(Coffee::Texture);
CEREAL_REGISTER_TYPE(Coffee::Texture2D);
CEREAL_REGISTER_TYPE(Coffee::Cubemap);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Texture);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Texture, Coffee::Texture2D);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Texture, Coffee::Cubemap);