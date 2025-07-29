#include "UIImageComponent.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Renderer/Texture.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    UIImageComponent::UIImageComponent() { Texture = Texture2D::Load("assets/textures/UVMap-Grid.jpg"); }

    template <class Archive> 
    void UIImageComponent::save(Archive& archive, std::uint32_t const version) const
    {
        archive(cereal::make_nvp("TextureUUID", Texture ? Texture->GetUUID() : UUID(0)),
                cereal::make_nvp("Color", Color), cereal::make_nvp("UVRect", UVRect));
        UIComponent::save(archive, version);
    }

    template <class Archive> 
    void UIImageComponent::load(Archive& archive, std::uint32_t const version)
    {
        UUID textureUUID;
        archive(cereal::make_nvp("TextureUUID", textureUUID));
        if (version >= 1)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("UVRect", UVRect));
        }
        if (textureUUID != UUID(0))
            Texture = ResourceLoader::GetResource<Texture2D>(textureUUID);
        UIComponent::load(archive, version);
    }

    // Explicit template instantiations for common cereal archives
    template void UIImageComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void UIImageComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void UIImageComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void UIImageComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
